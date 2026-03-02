#include <stdint.h>
#include "../arch/io.h"
#include "pic.h"

#define PIC1_COMMAND 0x20
#define PIC1_DATA    0x21
#define PIC2_COMMAND 0xA0
#define PIC2_DATA    0xA1

#define ICW1_INIT    0x10
#define ICW1_ICW4    0x01
#define ICW4_8086    0x01

void pic_remap(void) {
    uint8_t a1 = inb(PIC1_DATA);
    uint8_t a2 = inb(PIC2_DATA);

    // Start initialization
    outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
    outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);

    // Remap offsets
    outb(PIC1_DATA, 0x20);  // Master PIC -> 32
    outb(PIC2_DATA, 0x28);  // Slave PIC -> 40

    // Tell Master about Slave at IRQ2
    outb(PIC1_DATA, 0x04);
    outb(PIC2_DATA, 0x02);

    // 8086 mode
    outb(PIC1_DATA, ICW4_8086);
    outb(PIC2_DATA, ICW4_8086);

    // Mask: enable IRQ0 and IRQ1 only
    outb(PIC1_DATA, 0xFC);  // 11111100
    outb(PIC2_DATA, 0xFF);  // disable all slave IRQs
}

void pic_send_eoi(uint8_t irq) {
    if (irq >= 8)
        outb(PIC2_COMMAND, 0x20);

    outb(PIC1_COMMAND, 0x20);
}
