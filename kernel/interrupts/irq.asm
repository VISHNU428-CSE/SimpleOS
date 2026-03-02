global irq0
extern timer_handler

irq0:
    pusha
    call timer_handler
    popa
    iret
