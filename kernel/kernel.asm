bits 16
section .text ; expliticitly define code section for whatever 
global _start

_start:
    mov si, msg

print:
    mov al, [si]
    cmp al, 0
    je hang

    mov ah, 0x0e
    int 0x10

    inc si
    jmp print

hang:
    hlt
    jmp hang

msg db "Kernel loaded successfully", 0
