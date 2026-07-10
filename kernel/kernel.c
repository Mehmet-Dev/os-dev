// kernel.c
// THE AMAZING CODING THING HOLY FUCKING SHIT
#include <stdint.h>
#include "headers/io.h"
#include "headers/idt.h"

// We use 'void' and explicit attributes to tell the compiler: 
// "Don't add standard user-space wrappers around this function."
void kernel_main(void) {
    init_idt();
    pic_remap();
    __asm__ volatile("sti");
    
    while(1) __asm__ volatile("hlt");
}
