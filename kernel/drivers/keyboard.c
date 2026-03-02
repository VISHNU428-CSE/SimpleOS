#include "keyboard.h"
#include "screen.h"
#include "../arch/io.h"
#include "../interrupts/pic.h"
#include <stdint.h>

#define BUFFER_SIZE 128

extern void process_command(char* input);

/* ===== Buffers ===== */
static char input_buffer[BUFFER_SIZE];
static char last_command[BUFFER_SIZE];
static char previous_command[BUFFER_SIZE];

static int buffer_index = 0;
static int shift_pressed = 0;
static int extended = 0;
static int history_index = 0;

/* ===== Scancode table ===== */
static char scancode_table[128] = {
    0, 27, '1','2','3','4','5','6','7','8','9','0','-','=', '\b',
    '\t','q','w','e','r','t','y','u','i','o','p','[',']','\n',
    0, 'a','s','d','f','g','h','j','k','l',';','\'','`',
    0, '\\','z','x','c','v','b','n','m',',','.','/',
    0, '*',
    0, ' ',
};

/* ===== Keyboard Handler ===== */
void keyboard_handler(uint32_t irq)
{
    uint8_t scancode = inb(0x60);

    /* ===== Extended key detection ===== */
    if (scancode == 0xE0) {
        extended = 1;
        pic_send_eoi(irq);
        return;
    }

    if (extended) {
        extended = 0;

        /* UP Arrow */
        if (scancode == 0x48) {

            /* Clear current input line */
            while (buffer_index > 0) {
                buffer_index--;
                print_string("\b \b");
            }

            char* source = 0;

            if (history_index == 0 && last_command[0] != '\0') {
                source = last_command;
                history_index = 1;
            }
            else if (history_index == 1 && previous_command[0] != '\0') {
                source = previous_command;
                history_index = 0;
            }

            if (source) {
                int i = 0;
                while (source[i] != '\0') {
                    input_buffer[i] = source[i];
                    print_char(source[i]);
                    i++;
                }
                buffer_index = i;
            }
        }

        pic_send_eoi(irq);
        return;
    }

    /* ===== Shift press ===== */
    if (scancode == 42 || scancode == 54) {
        shift_pressed = 1;
        pic_send_eoi(irq);
        return;
    }

    /* ===== Shift release ===== */
    if (scancode == 170 || scancode == 182) {
        shift_pressed = 0;
        pic_send_eoi(irq);
        return;
    }

    /* ===== Ignore key releases ===== */
    if (scancode & 0x80) {
        pic_send_eoi(irq);
        return;
    }

    char c = scancode_table[scancode];

    if (shift_pressed && c >= 'a' && c <= 'z') {
        c -= 32;
    }

    if (!c) {
        pic_send_eoi(irq);
        return;
    }

    /* ===== ENTER ===== */
    if (c == '\n') {

        input_buffer[buffer_index] = '\0';

        /* Save history (only if not empty) */
        if (input_buffer[0] != '\0') {

            /* Move last -> previous */
            for (int i = 0; i < BUFFER_SIZE; i++) {
                previous_command[i] = last_command[i];
            }

            /* Save current -> last */
            for (int i = 0; i < BUFFER_SIZE; i++) {
                last_command[i] = input_buffer[i];
                if (input_buffer[i] == '\0') break;
            }
        }

        print_string("\n");
        process_command(input_buffer);

        buffer_index = 0;
        history_index = 0;
    }

    /* ===== Backspace ===== */
    else if (c == '\b') {
        if (buffer_index > 0) {
            buffer_index--;
            print_string("\b \b");
        }
    }

    /* ===== Normal characters ===== */
    else {
        if (buffer_index < BUFFER_SIZE - 1) {
            input_buffer[buffer_index++] = c;
            print_char(c);
        }
    }

    pic_send_eoi(irq);
}
