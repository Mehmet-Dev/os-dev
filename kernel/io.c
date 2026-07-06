#include "headers/io.h"
#include "headers/vga.h"

/// Prints a string with a chosen color.
void print_color(char* word, unsigned char color) {
    for(int i = 0; word[i] != '\0'; i++) {
        put_char(word[i], color);
    }
}

// Prints a string.
void print(char* word) {
    print_color(word, 0x07);
}
