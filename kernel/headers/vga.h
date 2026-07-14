#ifndef VGA_H
#define VGA_H

void clear_screen_color(unsigned char color);
void clear_screen(void);
void check_offset(void);
void put_char(char letter, unsigned char color);
void backspace_trigger(void);

#endif
