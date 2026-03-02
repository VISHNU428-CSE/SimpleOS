#ifndef PIT_H
#define PIT_H

#include <stdint.h>

extern volatile uint32_t ticks;

/* PIT setup */
void pit_init(void);

/* IRQ0 handler */
void timer_handler(uint32_t irq);

#endif
