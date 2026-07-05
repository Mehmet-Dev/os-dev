bits 16
section .text ; expliticitly define code section for whatever 
global _start

_start:
    jmp switch_to_pm

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
    
    mov esi, second_msg
    call print_string

    ; THE BEGINNING (Spin forever)
    jmp $

; -------------
; resets screen offset and clears screen
; ------------------------
clear_screen:
    push eax ; we'll need  it later
    push ecx
    push edi
    push esi

    mov esi, 0xb8000
    xor edi, edi ; from 0

    mov ax, 0x0720 ; space character and normal color
    mov ecx, 2000 ; 2000 times

.loop:
    mov [esi + edi], ax
    add edi, 2
    loop .loop

    ; resetting shit
    mov dword [current_offset], 0
    pop esi
    pop edi
    pop ecx
    pop eax
    ret

; -----------------------------------------------------------------------------
; print_char
; AL = ASCII character to print
; Modifies: None (all registers preserved safely)
; -----------------------------------------------------------------------------
print_char:
    push eax
    push ebx
    push edx
    push esi
    push edi

    ; Set up VGA segment
    mov esi, 0xb8000

    ; Load current screen position
    mov edi, [current_offset]

    ; 1. Check for Null Terminator (just in case it gets called directly with 0)
    cmp al, 0
    je .done

    ; 2. Check for Newline
    cmp al, 0x0a
    je .newline

    ; 3. Check Screen Bounds Before Printing
    cmp edi, 4000
    jbe .print_now
    
    call clear_screen
    xor edi, edi              ; Force local DI to top-left

.print_now:
    mov ah, 0x07            ; Attribute: Light gray on black
    mov [esi + edi], ax         ; Write to VGA memory
    add edi, 2               ; Advance cursor 1 char (2 bytes)
    jmp .done

.newline:
    mov eax, edi        ; Move full 32-bit offset into EAX
    xor edx, edx        ; CRITICAL: Clear EDX because DIV uses EDX:EAX as the dividend
    mov ebx, 160        ; Divide by 160 bytes per row
    div ebx             ; EAX = Row, EDX = Remainder (instead of AH)
    
    inc eax             ; Move to next row
    mul ebx             ; EAX = New Row * 160
    mov edi, eax        ; Update full 32-bit EDI

.done:
    ; Save the updated cursor position back to memory
    mov [current_offset], edi

    pop edi
    pop esi
    pop edx
    pop ebx
    pop eax
    ret

; -----------------------------------------------------------------------------
; print_string
; SI = Address of 0-terminated string
; -----------------------------------------------------------------------------
print_string:
    push eax
    push esi

.loop:
    mov al, [esi]
    cmp al, 0               ; Stop if we hit the null terminator
    je .done
    
    call print_char         ; Let print_char do all the heavy lifting!
    inc esi                  ; Next character in string
    jmp .loop

.done:
    pop esi
    pop eax
    ret


; -----------------
; data
; ----------------
msg: db "MEHMET KANKER", 0x0a, "KERNEL",  0
second_msg: db "DIT IS FUCKING COOL", 0
current_offset: dd 0


