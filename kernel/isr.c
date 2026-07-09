#include "headers/vga.h"
#include "headers/io.h"

typedef struct {
    unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;// PUSHA FUCK
    unsigned int int_no;
    unsigned int err_code;
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

void fault_handler(registers* regs) {
    unsigned char screen_color = 0x07; // Default fallback colors
    unsigned char label_color = 0x07;
    unsigned char value_color = 0x0F;
    unsigned char msg1_color = 0x0F;
    unsigned char msg2_color = 0x0F;

    char* msg1 = 0;
    char* msg2 = 0;

    switch(regs->int_no) {
        case 0: // Divide by Zero
            screen_color = 0x40; label_color = 0x4E; value_color = 0x4F;
            msg1_color = 0x4F;
            msg1 = "KERNEL PANIC: Division by zero is not allowed.\n\n";
            break;

        case 1: // Debug
            screen_color = 0x10; label_color = 0x1B; value_color = 0x1F;
            msg1_color = 0x1E;
            msg1 = "DEBUG EXCEPTION: Breakpoint/Single-Step trapped\n\n";
            break;

        case 2: // NMI
            screen_color = 0x00; label_color = 0x0C; value_color = 0x04;
            msg1_color = 0x0C; msg2_color = 0x0C;
            msg1 = "!!! HARDWARE EXCEPTION TRAPPED !!!\n";
            msg2 = "I think a cosmic ray hit your RAM\n\n";
            break;
            
        case 3: // Breakpoint
            screen_color = 0x20; label_color = 0x2A; value_color = 0x2F;
            msg1_color = 0x2F;
            msg1 = "[WINDOWS 12 ACTIVE BREAKPOINT TRAP]\n\n";
            break;
            
        case 4: // Overflow
            screen_color = 0x60; label_color = 0x6E; value_color = 0x6F;
            msg1_color = 0x6F;
            msg1 = "KERNEL EXCEPTION: Arithmetic Overflow Flag Tripped.\n\n";
            break;
            
        case 5: // Bound Range Exceeded
            screen_color = 0x50; label_color = 0x5E; value_color = 0x5F;
            msg1_color = 0x5F; msg2_color = 0x5E;
            msg1 = "KERNEL EXCEPTION: Bound Range Exceeded (#BR)\n";
            msg2 = "STATUS: Array index out of bounds tracking check.\n\n";
            break;
            
        case 6: // Invalid Opcode
            screen_color = 0x30; label_color = 0x30; value_color = 0x3F;
            msg1_color = 0x3F; msg2_color = 0x30;
            msg1 = "KERNEL PANIC: Invalid Opcode (#UD)\n";
            msg2 = "STATUS: CPU attempted to execute an undefined instruction.\n\n";
            break;
            
        case 7: // Device Not Available
            screen_color = 0x70; label_color = 0x70; value_color = 0x74;
            msg1_color = 0x74; msg2_color = 0x70;
            msg1 = "KERNEL EXCEPTION: Device Not Available (#NM)\n";
            msg2 = "STATUS: FPU / Floating Point Coprocessor not ready.\n\n";
            break;
            
        case 8: // Double Fault
            screen_color = 0x4F; label_color = 0x4F; value_color = 0x4F;
            msg1_color = 0x4F; msg2_color = 0x4F;
            msg1 = "!!! KERNEL CRASH: DOUBLE FAULT (#DF) !!!\n";
            msg2 = "The system experienced a fault during exception handling.\n";
            break;
        case 9: // Coprocessor Segment Overrun
            screen_color = 0x10; label_color = 0x17; value_color = 0x1B;
            msg1_color = 0x1F;
            msg1 = "LEGACY HARDWARE EXCEPTION: Coprocessor Segment Overrun (#CSO)\n";
            msg2 = "STATUS: 80387 math coprocessor segment boundary violation.\n\n";
            break;
        case 10: // Invalid TSS
            screen_color = 0x10; label_color = 0x1B; value_color = 0x1F;
            msg1_color = 0x1F; msg2_color = 0x1B;
            msg1 = "KERNEL PANIC: Invalid Task State Segment (#TS)\n";
            msg2 = "STATUS: Task switch configuration structure is corrupted.\n\n";
            break;

        case 11: // Segment Not Present
            screen_color = 0x00; label_color = 0x08; value_color = 0x07;
            msg1_color = 0x07;
            msg1 = "KERNEL EXCEPTION: Segment Not Present (#NP)\n";
            msg2 = "STATUS: Referenced memory segment descriptor is not loaded.\n\n";
            break;
            
        case 12: // Stack Fault
            screen_color = 0x40; label_color = 0x4E; value_color = 0x4F;
            msg1_color = 0x4F; msg2_color = 0x4E;
            msg1 = "!!! KERNEL CRASH: STACK SEGMENT FAULT (#SS) !!!\n";
            msg2 = "STATUS: Stack limit violation or invalid stack operations detected.\n\n";
            break;
        case 13: // General Protection Fault
            screen_color = 0x40; label_color = 0x4E; value_color = 0x4F;
            msg1_color = 0x4F; msg2_color = 0x4E;
            msg1 = "!!! SYSTEM SECURITY LOCKDOWN: GENERAL PROTECTION FAULT (#GP) !!!\n";
            msg2 = "STATUS: Privileged instruction violation, segment boundary breach, or bad selector.\n\n";
            break;
        case 14: // Page Fault
            screen_color = 0x00; label_color = 0x02; value_color = 0x0A;
            msg1_color = 0x0A; msg2_color = 0x02;
            msg1 = "!!! KERNEL PANIC: PAGE FAULT (#PF) !!!\n";
            msg2 = "STATUS: Virtual memory translation failed. CR2 register contains faulting address.\n\n";
            break;
        case 15: // Reserved
            screen_color = 0x00; label_color = 0x08; value_color = 0x07;
            msg1_color = 0x0F; msg2_color = 0x08;
            msg1 = "SYSTEM WARNING: Undefined / Reserved Exception Tripped (#15)\n";
            msg2 = "This should not happen. Please call Intel and tell them about this. Fuck you.\n\n";
            break;
            
        default:
            msg1 = "Unhandled Interrupt Triggered.\n\n";
            break;
    }

    // 1. Paint the background
    clear_screen_color(screen_color);

    // 2. Print the custom banners if they exist
    if (msg1) print_color(msg1, msg1_color);
    if (msg2) print_color(msg2, msg2_color);

    // 3. Special hardware error code injection just for Double Fault
    if (regs->int_no == 8) {
        print_color("ERROR CODE: 0x", 0x4F);
        print_hex_color(regs->err_code, 0x4F); 
        print_color("\n\n", 0x4F);
    }

    // 4. Dump the registers
    print_cpu_state(regs, label_color, value_color);

    // 5. Halt
    while(1);
}
