#include "headers/keyboard.h"

// global variables
int shift = 0;
volatile unsigned char write_index = 0;
volatile unsigned char read_index = 0;

unsigned char circular_buffer[256];

unsigned char keyboard_map[128] = {
    0,    27,  '1', '2', '3', '4', '5', '6', '7', '8',  // 0x00 to 0x09
    '9', '0',  '-', '=', '\b', '\t', 'q', 'w', 'e', 'r', // 0x0A to 0x13
    't', 'y',  'u', 'i', 'o', 'p', '[', ']', '\n',   0,  // 0x14 to 0x1D (0x1D is LCtrl)
    'a', 's',  'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',  // 0x1E to 0x27
   '\'', '`',    0, '\\', 'z', 'x', 'c', 'v', 'b', 'n',  // 0x28 to 0x31 (0x2A is LShift)
    'm', ',',  '.', '/',   0, '*',   0, ' ',   0,   0,  // 0x32 to 0x3B (0x36 is RShift, 0x39 is Space)
    0,    0,    0,   0,    0,   0,    0,   0,    0,   0,  // 0x3C to 0x45 (Function keys / NumLock)
    0,   '7',  '8', '9',  '-', '4',  '5', '6',  '+', '1', // 0x46 to 0x4F (Keypad mappings)
   '2', '3',  '0', '.',    0,   0,    0,   0,    0        // 0x50 to 0x58 (F11 is 0x57, F12 is 0x58)
    // Remaining indices up to 127 can be padded with 0
};

unsigned char keyboard_shift_map[128] = {
    0,    27,  '!', '@', '#', '$', '%', '^', '&', '*',  // 0x00 to 0x09
    '(', ')',  '_', '+', '\b', '\t', 'Q', 'W', 'E', 'R', // 0x0A to 0x13
    'T', 'Y',  'U', 'I', 'O', 'P', '{', '}', '\n',   0,  // 0x14 to 0x1D
    'A', 'S',  'D', 'F', 'G', 'H', 'J', 'K', 'L', ':',  // 0x1E to 0x27
    '"', '~',    0, '|',  'Z', 'X', 'C', 'V', 'B', 'N',  // 0x28 to 0x31
    'M', '<',  '>', '?',   0, '*',   0, ' ',   0,   0,  // 0x32 to 0x3B
    0,    0,    0,   0,    0,   0,    0,   0,    0,   0,  // 0x3C to 0x45
    0,   '7',  '8', '9',  '-', '4',  '5', '6',  '+', '1', // 0x46 to 0x4F
   '2', '3',  '0', '.',    0,   0,    0,   0,    0        // 0x50 to 0x58
};

// The action of handling a keypress. AMAZING
void handle_keypress(unsigned char scancode) {
    if(scancode == 0x2a || scancode == 0x36) { // if it's a left or right shift click
        shift = 1;
        return;
    }
    else if(scancode == 0xaa || scancode == 0xb6){ // releasing said shift keys
        shift = 0;
        return;
    }

    if(scancode & 0x80) { return; } // this means a regular key was released
    
    unsigned char character;
    if(shift)
        character = keyboard_shift_map[scancode];
    else
        character = keyboard_map[scancode];

    if(character > 0) {
        circular_buffer[write_index] = character;
        write_index++;
    }
}

// Getting a char
char keyboard_getchar() {
    if(read_index == write_index) return 0;

    char letter = circular_buffer[read_index];
    read_index++;

    return letter;
}
