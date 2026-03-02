#include "pic.h"
#include "../timer/pit.h"
#include "../drivers/keyboard.h"
#include "idt.h"
extern void irq0(void);
extern void irq1(void);
void irq_install(){
      idt_set_gate(32, (uint32_t)irq0, 0x08, 0x8E);
      idt_set_gate(33, (uint32_t)irq1, 0x08, 0x8E);
//	idt_init();
}
/*
 * Central IRQ dispatcher
 * irq number here is already normalized:
 *   IRQ0 = 0 (timer)
 *   IRQ1 = 1 (keyboard)
 */

void irq_handler(uint32_t irq) {
    switch (irq) {
        case 0:
            // Timer interrupt
            timer_handler(irq);
            break;

        case 1:
            // Keyboard interrupt
            keyboard_handler(irq);
            break;

        default:
            // Unhandled IRQ
            break;
    }

    // VERY IMPORTANT: acknowledge interrupt
    pic_send_eoi(irq);
}
