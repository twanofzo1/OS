#include "terminal.h"
#include <memory.h>
#include <graphics/text/text.h>
// helper functions

static inline void terminal_scroll(Terminal *terminal) {
    // Scroll the terminal buffer up by one line
    for (int y = 1; y < terminal->char_height; y++) {
        for (int x = 0; x < terminal->char_width; x++) {
            terminal->buffer[x + (y - 1) * terminal->char_width] =
                terminal->buffer[x + y * terminal->char_width];
        }
    }
    // Clear the last line
    for (int x = 0; x < terminal->char_width; x++) {
        terminal->buffer[x + (terminal->char_height - 1) * terminal->char_width] = ' ';
    }
}

static inline void terminal_next_line(Terminal *terminal) {
    terminal->cursor_x = 0;
    terminal->cursor_y++;
    // Scroll if at bottom
    if (terminal->cursor_y >= terminal->char_height) {
        terminal->cursor_y = terminal->char_height - 1;
        terminal_scroll(terminal);
    }
}

static inline void terminal_backspace(Terminal *terminal) {
    if (terminal->cursor_x > 0) {
        if (terminal->cursor_x == 0 && terminal->cursor_y > 0) {
            terminal->cursor_y--;
            terminal->cursor_x = terminal->char_width - 1;
        }
        terminal->buffer[terminal->cursor_x - 1 + terminal->cursor_y * terminal->char_width] = ' ';
        terminal->cursor_x--;
    }
}

static inline void terminal_put_character(Terminal *terminal, char c){
    if (c == '\n') {
        terminal_next_line(terminal);
        return;
    } 
    if (c == '\b') {
        terminal_backspace(terminal);
        return;
    }
    // Insert character and advance cursor
    terminal->buffer[terminal->cursor_x + terminal->cursor_y * terminal->char_width] = c;
    terminal->cursor_x++;
    // next line if at end of line
    if (terminal->cursor_x >= terminal->char_width) {
        terminal_next_line(terminal);
    }
}

// public functions

void terminal_initialize(Terminal *terminal,int x , int y , uint16_t char_width,uint16_t char_height){
    terminal->x = x;
    terminal->y = y;
    terminal->char_width = char_width;
    terminal->char_height = char_height;
    terminal->cursor_x = 0;
    terminal->cursor_y = 0;

    // TODO allocate buffer

    //terminal->buffer = (char *)malloc(terminal->char_width * terminal->char_height * sizeof(char));
    terminal_clear(terminal);
}

void terminal_set_cursor(Terminal *terminal, uint16_t x, uint16_t y){
    terminal->cursor_x = x;
    terminal->cursor_y = y;
}


void terminal_get_cursor(Terminal *terminal, uint16_t *x, uint16_t *y){
    *x = terminal->cursor_x;
    *y = terminal->cursor_y;
}


void terminal_put_string(Terminal *terminal, const char *str){
    for (const char *p = str; *p != '\0'; p++) {
        terminal_put_character(terminal, *p);
    }
}

void terminal_clear(Terminal *terminal){
    memset(terminal->buffer, ' ', sizeof(terminal->buffer));
    terminal->cursor_x = 0;
    terminal->cursor_y = 0;
}

void terminal_draw(Terminal *terminal){
    // Clear the terminal area
    graphics_draw_rect(
        terminal->x, 
        terminal->y, 
        terminal->char_width * (CHAR_WIDTH + CHAR_SPACING_ADJUST_X), 
        terminal->char_height * (CHAR_HEIGHT + CHAR_SPACING_ADJUST_Y), 
        TERMINAL_DEFAULT_BACKGROUND_COLOR
    );
    // Draw the terminal buffer to the screen
    for (int y = 0; y < terminal->char_height; y++) {
        for (int x = 0; x < terminal->char_width; x++) {
            char c = terminal->buffer[x + y * terminal->char_width];
            if (c != ' ') { // Only draw non-space characters
                int draw_x = terminal->x + x * (CHAR_WIDTH + CHAR_SPACING_ADJUST_X); // Adjust spacing
                int draw_y = terminal->y + y * CHAR_HEIGHT + y * CHAR_SPACING_ADJUST_Y; // Adjust spacing
                draw_char(c, draw_x, draw_y, TERMINAL_DEFAULT_TEXT_COLOR); // White color
            }
        }
    }
}