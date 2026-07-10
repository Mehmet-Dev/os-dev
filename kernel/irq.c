#include "headers/io.h"
#include "headers/vga.h"

volatile unsigned long long timer = 0;

unsigned long long get_ticks(void) {
    // clear interrupt flags
    __asm__ volatile ("cli");
    
    unsigned long long current_ticks = timer;

    __asm__ volatile("sti");
    
    return current_ticks;
}

void irq_handler(registers* regs) {
    switch(regs->int_no) {
        case 32: // Timer
            timer++;
            break;
        case 33:
            unsigned char scancode = inb(0x60);
            if(scancode & 0x80) {//keybard release

            }
            else {
                put_char(scancode, 0x07);
            }
            break;
    }

    if(regs->int_no >= 40) { // signal from pic slave
        outb(0x20, 0xa0); // telling it's done writing
    }

    outb(0x20, 0x20); // always telling the master pic we're done
}
