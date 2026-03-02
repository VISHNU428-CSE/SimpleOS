#include "isr.h"
#include "../drivers/screen.h"

/*
 * Temporary dummy ISR handler.
 * This PREVENTS reboot/triple-fault.
 * Real interrupt handling comes later.
 */

void isr_handler(void) {
    print_string("INTERRUPT RECEIVED\n");
}
