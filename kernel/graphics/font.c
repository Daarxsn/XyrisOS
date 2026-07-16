#include <stdint.h>

#include "font.h"
#include "graphics.h"
#include "font8x8_basic.h"

/* -------------------------------------------------
   Draw One Character
------------------------------------------------- */

void draw_char(
    int x,
    int y,
    char c,
    uint32_t color)
{
    const uint8_t *glyph =
        font8x8_basic[(unsigned char)c];

    for (int row = 0; row < 8; row++)
    {
        for (int col = 0; col < 8; col++)
        {
            if (glyph[row] & (1 << col))
            {
                draw_pixel(
                    x + col,
                    y + row,
                    color
                );
            }
        }
    }
}

/* -------------------------------------------------
   Draw String
------------------------------------------------- */

void draw_string(
    int x,
    int y,
    const char *text,
    uint32_t color)
{
    while (*text)
    {
        draw_char(
            x,
            y,
            *text,
            color
        );

        x += 8;
        text++;
    }
}