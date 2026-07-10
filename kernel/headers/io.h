#ifndef IO_H
#define IO_H

void print_color(char* word, unsigned char color);
void print(char* word);
void outb(unsigned char data, unsigned short port);
unsigned char inb(unsigned short port);
void pic_remap(void);

typedef struct {
    unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;// PUSHA FUCK
    unsigned int int_no;
    unsigned int err_code;
    unsigned int eip, cs, eflags, useresp, ss; // pushed automatically
} registers;

#endif
