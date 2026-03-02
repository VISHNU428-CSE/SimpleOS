#ifndef IRQ_H
#define IRQ_H

#include <stdint.h>
//void keyboard_handler(uint32_t irq);
extern void irq0();
extern void irq1();

void irq_install();
void irq_handler(uint32_t irq);

#endif
