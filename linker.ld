ENTRY(_start)

SECTIONS
{
    /* The kernel will be loaded at physical address 0x1000 */
    . = 0x7E00;

    .text : {
        /* Force the entry object file's text section to be FIRST */
        *kernel_asm.o(.text)
        *(.text)
    }

    .rodata : {
        *(.rodata)
    }

    .data : {
        *(.data)
    }

    .bss : {
        *(.bss)
    }
}
