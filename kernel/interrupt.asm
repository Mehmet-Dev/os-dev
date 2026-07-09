extern fault_handler

global isr0
global isr1;
global isr2;
global isr3;
global isr4
global isr5;
global isr6;
global isr7;
global isr8;
global isr9;
global isr10;
global isr11;
global isr12;
global isr13;
global isr14;
global isr15;

isr_common_stub:
    pusha           ; Save all registers
    mov eax, esp
    push eax        ; Pass the struct to C
    
    call fault_handler ; Call ONE single C function!
    
    pop eax
    popa
    add esp, 8      ; Clear the error code AND the interrupt number
    sti
    iret

; executes when division by 0
isr0:
    cli
    push 0      ; 1. Push dummy error code
    push 0      ; 2. Push the interrupt number (Slot 0)
    jmp isr_common_stub ; 3. Jump to the central router!

isr1:
    cli
    push 0      ; 1. Push dummy error code
    push 1      ; 2. Push the interrupt number (Slot 0)
    jmp isr_common_stub ; 3. Jump to the central router!

isr2:
    cli
    push 0      ; 1. Push dummy error code
    push 2      ; 2. Push the interrupt number (Slot 0)
    jmp isr_common_stub ; 3. Jump to the central router!

isr3:
    cli
    push 0      ; 1. Push dummy error code
    push 3      ; 2. Push the interrupt number (Slot 0)
    jmp isr_common_stub ; 3. Jump to the central router!

isr4:
    cli
    push 0      ; 1. Push dummy error code
    push 4      ; 2. Push the interrupt number (Slot 0)
    jmp isr_common_stub ; 3. Jump to the central router!

isr5:
    cli
    push 0      ; 1. Push dummy error code
    push 5      ; 2. Push the interrupt number (Slot 0)
    jmp isr_common_stub ; 3. Jump to the central router!

isr6:
    cli
    push 0      ; 1. Push dummy error code
    push 6      ; 2. Push the interrupt number (Slot 0)
    jmp isr_common_stub ; 3. Jump to the central router!

isr7:
    cli
    push 0      ; 1. Push dummy error code
    push 7      ; 2. Push the interrupt number (Slot 0)
    jmp isr_common_stub ; 3. Jump to the central router!

isr8:
    cli         ; Disable interrupts
    push 8
    jmp isr_common_stub

isr9:
    cli
    push 0      ; Dummy error code
    push 9      ; Interrupt number
    jmp isr_common_stub

isr10:
    cli
    ; No push 0 here
    push 10     ; Push the interrupt number
    jmp isr_common_stub

isr11:
    cli
    push 11     ; Push interrupt number
    jmp isr_common_stub

isr12:
    cli
    push 12     ; Push interrupt number
    jmp isr_common_stub

isr13:
    cli
    push 13     ; Push the interrupt number
    jmp isr_common_stub

isr14:
    cli
    push 14     ; Push the interrupt number
    jmp isr_common_stub

isr15:
    cli
    push 0      ; Dummy error code
    push 15     ; Interrupt number
    jmp isr_common_stub
