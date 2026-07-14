#include "headers/vga.h"

unsigned int current_offset = 0;

/// Clears the cool screen and sets it to a color.
void clear_screen_color(unsigned char color) {
    volatile char* vga_memory = (volatile char*)0xb8000;

    for(int i = 0; i < 4000; i+=2) {
        vga_memory[i] = 0x20;
        vga_memory[i+1] = color;
    }
}

// Clears the screen (default)
void clear_screen(void) {
    clear_screen_color(0x07);
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
void put_char(char letter, unsigned char color) {
    volatile char* vga_memory = (volatile char*)0xb8000;

    if(letter == '\n') {
        current_offset = (current_offset + 160) - (current_offset % 160);
        return;
    }

    vga_memory[current_offset] = letter;
    vga_memory[current_offset + 1] = color;

    current_offset += 2;
}

// Used for deletion in readline().
void backspace_trigger(void) {
    volatile char* vga_memory = (volatile char*)0xb8000;

    current_offset -= 2;
    vga_memory[current_offset] = 0x20;
}
