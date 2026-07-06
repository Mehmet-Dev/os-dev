// kernel.c
// THE AMAZING CODING THING HOLY FUCKING SHIT
#include <stdint.h>
#include "headers/io.h"
#include "headers/idt.h"

// We use 'void' and explicit attributes to tell the compiler: 
// "Don't add standard user-space wrappers around this function."
void kernel_main(void) {
    init_idt();

    // --- TRIGGER FOR ISR 1 (WINDOWS 12 BLUE SCREEN EDITION) ---
    __asm__ volatile (
        "int $4"
    );

    print("The idt failed loser");
    // Loop forever right here in C
    while(1) {
        // Do nothing, just stay parked
    }
}
