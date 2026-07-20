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

// Sends a byte of data to an I/O port
void outb(unsigned char data, unsigned short port) {
    // "a"(data) forces 'data' into the AL register
    // "d"(port) forces 'port' into the DX register
    __asm__ volatile ("outb %0, %1" : : "a"(data), "d"(port));
}

// Reads data from a register.
unsigned char inb(unsigned short port) {
    unsigned char result;
    // "=a"(result) means: when done, take whatever is in AL and save it to 'result'
    // "d"(port) means: put 'port' into DX before running the instruction
    __asm__ volatile ("inb %1, %0" : "=a"(result) : "d"(port));
    return result;
}

// LET HIM WAIT!!!!
void io_wait(void) {
    // Write a garbage byte to an unused port (0x80) to waste a few CPU cycles
    __asm__ volatile ("outb %%al, $0x80" : : "a"(0));
}

/// Gotta re-map this shit
void pic_remap(void) {
    outb(0x11, 0x20); //master command
    io_wait();
    outb(0x11, 0xa0); // slave command
    io_wait();

    // setting the vector offsets or whatever
    outb(0x20, 0x21);
    io_wait();
    outb(0x28, 0xa1);
    io_wait();

    // telling the chain or whatever
    outb(0x04, 0x21); //master data
    io_wait();
    outb(0x02, 0xa1); //slave
    io_wait();

    // settiong 8086 env mode
    outb(0x01, 0x21);
    io_wait();
    outb(0x01, 0xa1);
    io_wait();

    //clearing masks
    outb(0x00, 0x21);
    outb(0x00, 0xa1);
}

// Update the cursor
void update_cursor(void) {
    unsigned short current = get_current_offset() / 2;
    
    // sending high byte
    outb(14, 0x3d4);
    outb((current >> 8), 0x3d5);
    
    // sending low byte
    outb(15, 0x3d4);
    outb((current & 0xff), 0x3d5);
}
