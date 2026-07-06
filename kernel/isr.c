#include "headers/vga.h"
#include "headers/io.h"

typedef struct {
    unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax; // PUSHA FUCK
    unsigned int eip, cs, eflags, useresp, ss; // pushed automatically
} registers;

void print_hex_color(unsigned int value, unsigned char color) {
    char hex_chars[] = "0123456789ABCDEF";
    char buffer[11];
    buffer[0] = '0';
    buffer[1] = 'x';
    buffer[10] = '\0';

    for (int i = 7; i >= 0; i--) {
        buffer[2 + i] = hex_chars[value & 0xF];
        value >>= 4;
    }
    print_color(buffer, color);
}

// Perhaps a bit too ambitious.
void print_cpu_state(registers* regs, unsigned char label_color, unsigned char value_color) {
    print_color("EAX: ", label_color); print_hex_color(regs->eax, value_color); print_color("   ", value_color);
    print_color("EBX: ", label_color); print_hex_color(regs->ebx, value_color); print_color("\n", value_color);

    print_color("ECX: ", label_color); print_hex_color(regs->ecx, value_color); print_color("   ", value_color);
    print_color("EDX: ", label_color); print_hex_color(regs->edx, value_color); print_color("\n", value_color);

    print_color("EDI: ", label_color); print_hex_color(regs->edi, value_color); print_color("   ", value_color);
    print_color("ESI: ", label_color); print_hex_color(regs->esi, value_color); print_color("\n", value_color);

    print_color("EBP: ", label_color); print_hex_color(regs->ebp, value_color); print_color("   ", value_color);
    print_color("ESP: ", label_color); print_hex_color(regs->esp, value_color); print_color("\n", value_color);

    print_color("EIP: ", label_color); print_hex_color(regs->eip, value_color); print_color("   ", value_color);
    print_color("EFL: ", label_color); print_hex_color(regs->eflags, value_color); print_color("\n", value_color);
}

// Handles division by zero.
void isr_0(registers* regs) {
    clear_screen_color(0x40);
    print_color("KERNEL PANIC: Division by zero is not allowed.\n\n", 0x4f);
    print_cpu_state(regs, 0x4e, 0x4f);
    while(1);
}

// Debug.
void isr_1(registers* regs) {
    clear_screen_color(0x10);

    print_color("DEBUG EXCEPTION: Breakpoint/Single-Step trapped\n\n", 0x1e);

    print_cpu_state(regs, 0x1b, 0x1f);
    while(1);
}

// NMI: incase your pc is melting
void isr_2(registers* regs) {
    clear_screen_color(0x00); // completely dark
    
    print_color("!!! HARDWARE EXCEPTION TRAPPED !!!\n", 0x0c);
    print_color("I think a cosmic ray hit your RAM\n\n", 0x0c);

    print_cpu_state(regs, 0x0C, 0x04);

    while(1);
}

// Breakpoints.
void isr_3(registers* regs) {
    clear_screen_color(0x20); // Deep Green background
    print_color("[WINDOWS 12 ACTIVE BREAKPOINT TRAP]\n\n", 0x2F); // White text
    print_cpu_state(regs, 0x2A, 0x2F);
    
    while(1);
}

// Handles Arithmetic Overflow
void isr_4(registers* regs) {
    clear_screen_color(0x60); // Orange/Brown background
    print_color("KERNEL EXCEPTION: Arithmetic Overflow Flag Tripped.\n\n", 0x6F);
    
    // Labels: Yellow (0x6E), Numbers: White (0x6F)
    print_cpu_state(regs, 0x6E, 0x6F);
    
    while(1);
}
