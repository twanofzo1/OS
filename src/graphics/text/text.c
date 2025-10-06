#include "text.h"
#include "../../../font/font.h"



void draw_char(char c, int x, int y, uint32_t color) {
    if (c < 32 || c > 126) {
        return; // Unsupported character
    }

    const CHAR_TYPE* glyph = font[c - 32];

    for (int row = 0; row < CHAR_HEIGHT; row++) {
        for (int col = 0; col < CHAR_WIDTH; col++) {
            if (glyph[row] & (1 << (CHAR_WIDTH - 1 - col))) { // Use CHAR_WIDTH for bit indexing
                graphics_draw_pixel(x + col, y + row, color);
            }
        }
    }
}

void draw_string(const char* str, int x, int y, uint32_t color) {
    int orig_x = x;
    for (char c = *str; c != '\0'; c = *++str) {
        if (c == '\n') {
            y += CHAR_HEIGHT + CHAR_SPACING_ADJUST_Y; // Adjust spacing between lines
            x = orig_x; // Reset x to the original start of the line
        } else {
            draw_char(c, x, y, color);
            x += CHAR_WIDTH + CHAR_SPACING_ADJUST_X; // Adjust spacing between characters
        }
    }
}