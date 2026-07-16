#ifndef XYRIS_GRAPHICS_H
#define XYRIS_GRAPHICS_H

#include <stdint.h>

void clear_screen(uint32_t color);

void draw_pixel(
    uint32_t x,
    uint32_t y,
    uint32_t color
);

void draw_line(
    int x0,
    int y0,
    int x1,
    int y1,
    uint32_t color
);

void draw_rect(
    int x,
    int y,
    int width,
    int height,
    uint32_t color
);

void fill_rect(
    int x,
    int y,
    int width,
    int height,
    uint32_t color
);

#endif