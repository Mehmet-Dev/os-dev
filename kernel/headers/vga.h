#ifndef VGA_H
#define VGA_H

void clear_screen_color(unsigned char color);
void clear_screen(void);
void check_offset(void);
void put_char(char letter, unsigned char color);
void backspace_trigger(void);

extern unsigned int current_offset;

static inline unsigned int get_current_offset(void) {
    return current_offset;
}

#endif
