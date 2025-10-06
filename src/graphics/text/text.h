#pragma once
#include "../graphics.h"

#define CHAR_SPACING_ADJUST_X -4
#define CHAR_SPACING_ADJUST_Y 0
#define CHAR_TYPE uint16_t
#define CHAR_WIDTH 16
#define CHAR_HEIGHT 24

void draw_char(char c, int x, int y, uint32_t color);
void draw_string(const char* str, int x, int y, uint32_t color);