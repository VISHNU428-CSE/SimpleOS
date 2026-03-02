[BITS 32]

global irq0
global irq1

extern irq_handler

irq0:
    push dword 0
    jmp irq_common

irq1:
    push dword 1
    jmp irq_common

irq_common:
    pusha

    mov eax, [esp + 32]
    push eax
    call irq_handler
    add esp, 4

    popa
    add esp, 4
    iretd
