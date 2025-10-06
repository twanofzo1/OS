#pragma once
#include <stdint.h>

#define TERMINAL_DEFAULT_TEXT_COLOR 0xFFFFFFFF
#define TERMINAL_DEFAULT_BACKGROUND_COLOR 0xFF000000
#define TERMINAL_WIDTH 80
#define TERMINAL_HEIGHT 25

typedef struct
{
    char buffer[TERMINAL_WIDTH * TERMINAL_HEIGHT]; // TODO change to heap allocation
    int x;
    int y;
    uint16_t char_width;
    uint16_t char_height;
    uint16_t cursor_x;
    uint16_t cursor_y;
} Terminal;

void terminal_initialize(Terminal *terminal,int x , int y , uint16_t char_width,uint16_t char_height);
void terminal_set_cursor(Terminal *terminal, uint16_t x, uint16_t y);
void terminal_get_cursor(Terminal *terminal, uint16_t *x, uint16_t *y);
void terminal_put_string(Terminal *terminal, const char *str);
void terminal_clear(Terminal *terminal);
void terminal_draw(Terminal *terminal);
