#include "headers/idt.h"

idt_entry idt[256];
idt_ptr idt_p;

extern void isr0(void);
extern void isr1(void);
extern void isr2(void);
extern void isr3(void);
extern void isr4(void);
extern void isr5(void);
extern void isr6(void);
extern void isr7(void);
extern void isr8(void);
extern void isr9(void);
extern void isr10(void);
extern void isr11(void);
extern void isr12(void);
extern void isr13(void);
extern void isr14(void);
extern void isr15(void);
extern void isr16(void);
extern void isr17(void);
extern void isr18(void);
extern void isr19(void);

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
    idt_set_gate(BOUND_RANGE_EXCEEDED, isr5, 0x08, 0x8E);
    idt_set_gate(INVALID_OPCODE, isr6, 0x08, 0x8E);
    idt_set_gate(DEVICE_NOT_AVAILABLE, isr7, 0x08, 0x8E);
    idt_set_gate(DOUBLE_FAULT, isr8, 0x08, 0x8E);
    idt_set_gate(CSO, isr9, 0x08, 0x8e);
    idt_set_gate(INVALID_TSS, isr10, 0x08, 0x8e);
    idt_set_gate(SEGMENT_NOT_PRESENT, isr11, 0x08, 0x8E);
    idt_set_gate(STACK_SEGMENT_FAULT, isr12, 0x08, 0x8E);
    idt_set_gate(GENERAL_PROTECTION_FAULT, isr13, 0x08, 0x8e);
    idt_set_gate(PAGE_FAULT, isr14, 0x08, 0x8e);
    idt_set_gate(15, isr15, 0x08, 0x8e);
    idt_set_gate(FLOATING_POINT_ERROR, isr16, 0x08, 0x8e);
    idt_set_gate(ALIGNMENT_CHECK, isr17, 0x08, 0x8e);
    idt_set_gate(MACHINE_CHECK, isr18, 0x08, 0x8e);
    idt_set_gate(FLOATING_POINT_EXCEPTION, isr16, 0x08, 0x8e);

    __asm__ volatile("lidt %0" : : "m"(idt_p));
}
