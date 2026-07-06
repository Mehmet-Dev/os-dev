#ifndef IDT_H
#define IDT_H

#define DIVISION_BY_ZERO 0
#define DEBUG 1
#define NMI 2
#define BREAKPOINT 3
#define OVERFLOW 4
#define BOUND_RANGE_EXCEEDED 5
#define INVALID_OPCODE 6
#define DEVICE_NOT_AVAILABLE 7
#define DOUBLE_FAULT 8
#define CSO 9 // coprocessor segment overrun
#define INVALID_TSS 10
#define SEGMENT_NOT_PRESENT 11
#define STACK_SEGMENT_FAULT 12
#define GENERAL_PROTECTION_FAULT 13
#define PAGE_FAULT 14
// 15 is skipped over cus Intel reserved it. dk when the reservation ends
#define FLOATING_POINT_ERROR 16
#define ALIGNMENT_CHECK 17
#define MACHINE_CHECK 18
#define FLOATING_POINT_EXCEPTION 19

// 20-31 are reserved by intel for the future

// ===== CUSTOM INTERRUPTS START ===========

// ===== CUSTOM INTERRUPTS END =============

typedef struct {
    unsigned short base_lo; // lower 16 bit of address
    unsigned short selector; // code selector
    unsigned char always0; // must always be 0. if it is 1 Intel visits your house
    unsigned char attribute; //flags n shit
    unsigned short base_hi; // higher 16 bits
} __attribute__((packed)) idt_entry;

#endif
