// kernel.c
// THE AMAZING CODING THING HOLY FUCKING SHIT

#include <stdint.h>

uint32_t current_offset = 0;

/// Clears the cool screen. Maybe paging? I have no clue
void clear_screen(void) {
    volatile char* vga_memory = (volatile char*)0xb8000;

    for(int i = 0; i < 4000; i+=2) {
        vga_memory[i] = 0x20;
        vga_memory[i+1] = 0x07;
    }
}

/// Checks current offset.
/// VGA memory limit is 4000., 
void check_offset(void) {
    if(current_offset >= 4000) {
        current_offset = 0;
        clear_screen();
    }
}

/// Print something.
/// Put a char in the thing idk
void put_char(char letter, uint8_t color) {
    volatile char* vga_memory = (volatile char*)0xb8000;

    if(letter == '\n') {
        current_offset = (current_offset + 160) - (current_offset % 160);
        return;
    }

    vga_memory[current_offset] = letter;
    vga_memory[current_offset + 1] = color;

    current_offset += 2;
}

/// Prints a string with a chosen color.
void print_color(char* word, uint8_t color) {
    for(int i = 0; word[i] != '\0'; i++) {
        put_char(word[i], color);
    }
}

// Prints a string.
void print(char* word) {
    print_color(word, 0x07);
}

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
