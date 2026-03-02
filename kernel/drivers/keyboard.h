#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdint.h>

void keyboard_init(void);
void keyboard_irq_handler(void);
void keyboard_handler(uint32_t irq);
char keyboard_getchar(void);

#endif
