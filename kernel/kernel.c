
#include "drivers/screen.h"
#include "drivers/keyboard.h"
#include "interrupts/pic.h"
#include "interrupts/idt.h"
#include "interrupts/irq.h"
#include "timer/pit.h"
#include "memory/heap.h"
void kernel_main(void) {
    clear_screen();
print_string("\n");
print_string("--------------------------------------------------\n");
print_string("                SimpleOS v1.0\n");
print_string("         32-bit x86 Educational Kernel\n");
print_string("--------------------------------------------------\n\n");
print_string("Type 'help' for available commands.\n\n");
print_string("SimpleOS>");
    pic_remap();
    idt_init();
    irq_install();

    pit_init();
    asm volatile ("sti");  // ONLY NOW
    while (1);
}
