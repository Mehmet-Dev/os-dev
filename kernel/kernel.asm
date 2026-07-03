bits 16
section .text ; expliticitly define code section for whatever 
global _start

_start:
    mov si, msg
    call print_string

hang:
    hlt
    jmp hang

; ------------------
; print_string
; SI = address of 0-terminated string
; -----------------
print_string:
    mov ax, 0xb800
    mov es, ax

    xor bx, bx ; usin it as a counter

.loop:
    mov al, [si]
    cmp al, 0
    je .done

    mov ah, 0x07

    mov [es:bx], ax

    inc si
    add bx, 2
    jmp .loop

.done:
    ret

; -----------------
; data
; ----------------
msg: db "MEHMET KANKER KERNEL", 0
