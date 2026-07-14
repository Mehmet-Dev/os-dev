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

    // Clear screen first, then print boot message
    print("Open Shitting System Booted.\n");

    // Create a 256-byte buffer in RAM to hold the typed string
    char input_buffer[256];

    while(1) {
        print("MehmetOS> ");
        
        // This blocks execution, handles typing, backspacing, 
        // and returns only when you hit Enter!
        readline(input_buffer, 256);

        // Echo the final captured string back to the screen
        print("You typed: ");
        print(input_buffer);
        print("\n");
    }
}
