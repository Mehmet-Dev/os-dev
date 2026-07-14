#ifndef KEYBOARD_H
#define KEYBOARD_H

#define BUFFER_SIZE 256

void handle_keypress(unsigned char scancode);
char keyboard_getchar(void);
char* readline(char* buffer, unsigned int max_size);

#endif
