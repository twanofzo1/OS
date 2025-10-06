#include "graphics.h"

#define BLACK 0x000000
#define WHITE 0xFFFFFF
#define RED 0xFF0000
#define GREEN 0x00FF00
#define BLUE 0x0000FF
#define CYAN 0x00FFFF
#define MAGENTA 0xFF00FF
#define YELLOW 0xFFFF00

int graphics_get_width(){
    return framebuffer_request.response->framebuffers[0]->width;
}
int graphics_get_height(){
    return framebuffer_request.response->framebuffers[0]->height;
}

void graphics_draw_pixel(int x, int y, uint32_t color){
    volatile uint32_t *fb_ptr = framebuffer_request.response->framebuffers[0]->address;
    int pitch = framebuffer_request.response->framebuffers[0]->pitch / 4; // pitch is in bytes, convert to pixels
    fb_ptr[y * pitch + x] = color;
}


void graphics_draw_rect(int x, int y, int width, int height, uint32_t color){
    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            graphics_draw_pixel(x + i, y + j, color);
        }
    }
}


void graphics_clear(uint32_t color){
    int screen_width = graphics_get_width();
    int screen_height = graphics_get_height();
    graphics_draw_rect(0, 0, screen_width, screen_height, color);
}




