#include <stdint.h>
#include "../arch/io.h"
#include "pit.h"

#include "../interrupts/pic.h"

volatile uint32_t ticks = 0;

void timer_handler(uint32_t irq) {
    ticks++;
    pic_send_eoi(irq);
}

void pit_init() {
    uint32_t divisor = 1193180 / 100;

    outb(0x43, 0x36);
    outb(0x40, divisor & 0xFF);
    outb(0x40, (divisor >> 8) & 0xFF);
}
