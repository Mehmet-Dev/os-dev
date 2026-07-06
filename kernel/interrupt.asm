extern isr_0;
extern isr_1;
extern isr_2;
extern isr_3;
extern isr_4

global isr0
global isr1;
global isr2;
global isr3;
global isr4

; executes when division by 0
isr0:
    cli ; disable interrupts
    pusha ; saves everything onto the stack
    
    mov eax, esp ; copies current stack pointer
    push eax

    call isr_0 ; calls code in isr.c

    pop eax ; restores everything 
    popa
    sti ; re-enable again
    iret ;resume the universe

isr1:
    cli
    pusha

    mov eax, esp
    push eax

    call isr_1

    pop eax
    popa
    sti
    iret

isr2:
    cli
    pusha

    mov eax, esp
    push eax

    call isr_2

    pop eax
    popa
    sti
    iret

isr3:
    cli
    pusha
    mov eax, esp
    push eax
    call isr_3
    pop eax
    popa
    sti
    iret

isr4:
    cli
    pusha
    mov eax, esp
    push eax
    call isr_4
    pop eax
    popa
    sti
    iret
