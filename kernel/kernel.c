// kernel.c
// THE AMAZING CODING THING HOLY FUCKING SHIT
#include <stdint.h>
#include "headers/io.h"
#include "headers/vga.h"

// We use 'void' and explicit attributes to tell the compiler: 
// "Don't add standard user-space wrappers around this function."
void kernel_main(void) {
    print("Hello i think this works\nMaybe not. Dont ask me fucking dumbass");
    print_color("\nI should EAT homeless people for no particular reason", 0x0d);
    
    // Loop forever right here in C
    while(1) {
        // Do nothing, just stay parked
    }
}
