#ifndef XYRIS_FONT_H
#define XYRIS_FONT_H

#include <stdint.h>

void draw_char(
    int x,
    int y,
    char c,
    uint32_t color
);

void draw_string(
    int x,
    int y,
    const char *text,
    uint32_t color
);

#endif