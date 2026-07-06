bits 16 ; set to 16 bit real mode
org 0x7c00 ; sets origin address

mov [drive_number], dl

mov si, 0 ; initiate source index register to 0 for string index counter

cleanup:
    cli                 ; disable interrupts

    xor ax, ax          ; AX = 0

    mov ds, ax          ; data segments = 0
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov ax, 0x9000
    mov ss, ax
    mov sp, 0xffff
    cld                 ; clear direction flag
    sti

; label for printing loop
clear_screen:
    mov ax, 0x0600 ; AH = 06 scroll up al = 0 clear window
    mov bh, 0x07 ;attribute for blank lines ig idk
    mov cx, 0x0000 ; upper left row 0 col 0 
    mov dx, 0x184F ; lower right: row 24, col 79
    int 0x10; 

    mov dx, 0x0000 ; moving cursor to 0, 0
    mov bh, 0x00
    mov ah, 0x02
    int 0x10


; we load the kernel.asm from here (lives right next to 512 byte boot sector)
kernel_read_setup:
    mov ah, 2 ; function 2 of interrupt 13
    mov al, 40 ; read 40 sectors for now
    mov ch, 0 ; from cylinder 0
    mov cl, 2 ; second sector starts from 1
    mov dh, 0 ; idk bro 

    xor bx, bx ; sets to 0
    mov es, bx ; set to 0 from earlier
    mov bx, 0x7e00 ; starts at address 1000 i guess

read_kernel:
    mov dl, [drive_number] ; chatgpt fuck you
    int 0x13;
    jnc jump_to_kernel ; carry flag not set, success

    xor ah, ah ; bios reset disk function
    int 0x13
    jmp read_kernel ; retry

jump_to_kernel:
    jmp 0x0000:0x7e00 ; FUCK FUCK FUCK

; label for drive number saving
drive_number:
    db 0

times 510 - ($ - $$) db 0 ; pads the file so it becomes 510 bytes
dw 0xaa55 ; boot signature 
