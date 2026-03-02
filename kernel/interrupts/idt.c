#include "idt.h"
//#include <stdint.h>

/* ASM function */
extern void idt_load(void);

/* IRQ ASM stubs */
extern void irq0(void);
extern void irq1(void);

/* IDT entry structure */

struct idt_entry idt[256];
struct idt_ptr idtp;

/* IMPORTANT: NOT STATIC */
void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags) {
    idt[num].base_low  = base & 0xFFFF;
    idt[num].base_high = (base >> 16) & 0xFFFF;
    idt[num].selector  = sel;
    idt[num].zero      = 0;
    idt[num].flags     = flags;
}

void idt_init(void) {
    idtp.limit = sizeof(idt) - 1;
    idtp.base  = (uint32_t)&idt;

    /* Clear IDT */
    for (int i = 0; i < 256; i++) {
        idt_set_gate(i, 0, 0, 0);
    }

    /* IRQ0 → vector 32 */
    idt_set_gate(32, (uint32_t)irq0, 0x08, 0x8E);

    /* IRQ1 → vector 33 */
    idt_set_gate(33, (uint32_t)irq1, 0x08, 0x8E);

    idt_load();
}

