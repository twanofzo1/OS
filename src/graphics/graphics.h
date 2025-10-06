#pragma once
#include "limine_requests.h"

int graphics_get_width();
int graphics_get_height();

void graphics_draw_pixel(int x, int y, uint32_t color);
void graphics_draw_rect(int x, int y, int width, int height, uint32_t color);
void graphics_clear(uint32_t color);

