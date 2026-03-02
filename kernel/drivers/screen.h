#include <stdint.h>
#ifndef SCREEN_H
#define SCREEN_H

void clear_screen(void);
void print_string(const char* str);
void print_char(char c);

void print_int(int num);
void set_cursor_position(int row, int col);
#endif
