#include <stdint.h>

#define VIDEO_ADDRESS 0xB8000
#define WIDTH 80
#define HEIGHT 25
#define WHITE_ON_BLACK 0x0F

static int cursor = 0;

void clear_screen() {
    char* vga = (char*) VIDEO_ADDRESS;

    for (int i = 0; i < WIDTH * HEIGHT; i++) {
        vga[i * 2] = ' ';
        vga[i * 2 + 1] = WHITE_ON_BLACK;
    }

    cursor = 0;
}

void print_char(char c) {

    char* vga = (char*) VIDEO_ADDRESS;

    // Newline
    if (c == '\n') {
        cursor = (cursor / WIDTH + 1) * WIDTH;
    }

    // Backspace
    else if (c == '\b') {
        if (cursor > 0) {
            cursor--;
            vga[cursor * 2] = ' ';
            vga[cursor * 2 + 1] = WHITE_ON_BLACK;
        }
    }

    // Normal character
    else {
        vga[cursor * 2] = c;
        vga[cursor * 2 + 1] = WHITE_ON_BLACK;
        cursor++;
    }

    scroll();
}

void print_string(const char* str) {
    int i = 0;
    while (str[i] != '\0') {
        print_char(str[i]);
        i++;
    }
}
void print_int(int num) {
    char buffer[16];
    int i = 0;

    if (num == 0) {
        print_char('0');
        return;
    }

    while (num > 0) {
        buffer[i++] = '0' + (num % 10);
        num /= 10;
    }

    while (i > 0) {
        print_char(buffer[--i]);
    }
}
void set_cursor_position(int row, int col) {
    cursor = row * WIDTH + col;
}
void scroll() {

    if (cursor < WIDTH * HEIGHT)
        return;

    char* vga = (char*) VIDEO_ADDRESS;

    // Move each line up
    for (int row = 1; row < HEIGHT; row++) {
        for (int col = 0; col < WIDTH; col++) {

            int from = (row * WIDTH + col) * 2;
            int to   = ((row - 1) * WIDTH + col) * 2;

            vga[to]     = vga[from];
            vga[to + 1] = vga[from + 1];
        }
    }

    // Clear last line
    for (int col = 0; col < WIDTH; col++) {
        int pos = ((HEIGHT - 1) * WIDTH + col) * 2;
        vga[pos]     = ' ';
        vga[pos + 1] = WHITE_ON_BLACK;
    }

    cursor = (HEIGHT - 1) * WIDTH;
}
