// kernel.c
// THE AMAZING CODING THING HOLY FUCKING SHIT
#include <stdint.h>
#include "headers/io.h"
#include "headers/idt.h"
#include "headers/keyboard.h"
#include "headers/vga.h"

// We use 'void' and explicit attributes to tell the compiler: 
// "Don't add standard user-space wrappers around this function."
void kernel_main(void) {
    init_idt();
    pic_remap();
    __asm__ volatile("sti");
    
    // Clear the screen or print a boot message first
    print("Open Shitting System Booted Successfully.\nType something: ");

    while(1) {
        // Check our non-blocking buffer
        char c = keyboard_getchar();
        
        if (c != 0) {
            // If a valid key was pulled from the buffer, print it out!
            put_char(c, 0x07); 
        } else {
            // If no key is in the buffer, sleep the CPU until the next IRQ
            __asm__ volatile("hlt");
        }
    }
}
