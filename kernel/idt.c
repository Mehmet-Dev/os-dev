#include "headers/idt.h"

idt_entry idt[256];
idt_ptr idt_p;

extern void isr0(void);
extern void isr1(void);
extern void isr2(void);
extern void isr3(void);
extern void isr4(void);

void idt_set_gate(unsigned char num, void (*handler)(void), unsigned short selector, unsigned char flags) {
    idt_entry entry;
    entry.selector = selector;
    entry.attribute = flags;
    entry.always0 = 0;

    unsigned int address = (unsigned int)(unsigned long)handler;
    entry.base_lo = address & 0xffff;
    entry.base_hi = address >> 16;

    idt[num] = entry;
}

void init_idt(void) {
    idt_p.base = (unsigned int)(unsigned long)&idt;
    idt_p.limit = (sizeof(idt_entry) * 256) - 1;

    idt_set_gate(DIVISION_BY_ZERO, isr0, 0x08, 0x8e);
    idt_set_gate(DEBUG, isr1, 0x08, 0x8e);
    idt_set_gate(NMI, isr2, 0x08, 0x8e);
    idt_set_gate(BREAKPOINT, isr3, 0x08, 0x8e);
    idt_set_gate(OVERFLOW, isr4, 0x08, 0x8e);

    __asm__ volatile("lidt %0" : : "m"(idt_p));
}
