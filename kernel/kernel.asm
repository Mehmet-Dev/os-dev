bits 16
section .text ; expliticitly define code section for whatever 
global _start

_start:
    mov si, msg
    call print_string
    mov si, second_msg
    call print_string

    jmp switch_to_pm

; -----------------------------------------------------------------------------
; print_char
; AL = ASCII character to print
; Modifies: None (all registers preserved safely)
; -----------------------------------------------------------------------------
print_char:
    push ax
    push bx
    push dx
    push es
    push di

    ; Set up VGA segment
    mov bx, 0xb800
    mov es, bx
    
    ; Load current screen position
    mov di, [current_offset]

    ; 1. Check for Null Terminator (just in case it gets called directly with 0)
    cmp al, 0
    je .done

    ; 2. Check for Newline
    cmp al, 0x0a
    je .newline

    ; 3. Check Screen Bounds Before Printing
    cmp di, 4000
    jbe .print_now
    
    call clear_screen
    xor di, di              ; Force local DI to top-left

.print_now:
    mov ah, 0x07            ; Attribute: Light gray on black
    mov [es:di], ax         ; Write to VGA memory
    add di, 2               ; Advance cursor 1 char (2 bytes)
    jmp .done

.newline:
    mov ax, di
    mov bl, 160
    div bl                  ; AL = Row, AH = Remainder
    inc al                  ; Next row
    mul bl                  ; AX = New Row * 160
    mov di, ax              ; Update DI

.done:
    ; Save the updated cursor position back to memory
    mov [current_offset], di

    pop di
    pop es
    pop dx
    pop bx
    pop ax
    ret

; -----------------------------------------------------------------------------
; print_string
; SI = Address of 0-terminated string
; -----------------------------------------------------------------------------
print_string:
    push ax
    push si

.loop:
    mov al, [si]
    cmp al, 0               ; Stop if we hit the null terminator
    je .done
    
    call print_char         ; Let print_char do all the heavy lifting!
    inc si                  ; Next character in string
    jmp .loop

.done:
    pop si
    pop ax
    ret

; -------------
; resets screen offset and clears screen
; ------------------------
clear_screen:
    push ax ; we'll need  it later
    push cx
    push di

    mov ax, 0xb800
    mov es, ax
    xor di, di ; from 0

    mov ax, 0x0720 ; space character and normal color
    mov cx, 2000 ; 2000 times

.loop:
    mov [es:di], ax
    add di, 2
    loop .loop

    ; resetting shit
    mov word [current_offset], 0
    pop di
    pop cx
    pop ax
    ret

; =========================
; 32 bit mode switch
; =========================

switch_to_pm:
    cli ; no interrupts
    lgdt [gdt_descriptor] ; load table into cpu

    mov eax, cr0 ; flip protected mode switch
    or eax, 0x1 ; set lowest bit to 1
    mov cr0, eax

    jmp 0x08:protected_mode_start

; ======================
; gdt table setup
; ======================
align 4 ; aligning good practice or sumnt
gdt_start:
gdt_null:
    dq 0

gdt_code: ; 0x08 code descriptor
    dw 0xffff ; limit
    dw 0x0000 ; base
    db 0x00 ; also base 
    db 10011010b ; access byte
    db 11001111b ; flags
    db 0x00 ; also base for whatever

gdt_data:
    dw 0xffff
    dw 0x0000 ;base
    db 0x00 ; base
    db 10010010b ; access byte present
    db 11001111b ; flags
    db 0x00 ; base again idk

gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1 ; size of gdt
    dd gdt_start ; linear address of gdt

; =============
; landing zone
; =============
bits 32
protected_mode_start:
    mov ax, 0x10 ; points to gdt data segment
    mov ds, ax ; updating all data segment selectors
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    
    ; update stack pointer
    mov esp, 0x90000

    ; =========================================================================
    ; PROOF OF LIFE: Write 'P' for Protected Mode directly to VGA memory
    ; =========================================================================
    ; In a flat memory model, VGA text mode memory is always at 0xb8000.
    ; We will write to the very top-right corner of the screen so it doesn't
    ; overwrite your 16-bit startup messages.
    
    mov ax, 0x4f50                             
    mov [0xb8000 + 158], ax ; Write it to the end of the first row
                            ; (80 columns * 2 bytes per char = 160 bytes per row)

    ; THE BEGINNING (Spin forever)
    jmp $

; -----------------
; data
; ----------------
msg: db "MEHMET KANKER", 0x0a, "KERNEL",  0
second_msg: db "DIT IS FUCKING COOL", 0
current_offset: dw 0


