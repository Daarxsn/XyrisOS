#include "graphics.h"
#include "framebuffer.h"

void clear_screen(uint32_t color)
{
    volatile uint32_t *fb = framebuffer_address();

    uint32_t width = framebuffer_width();
    uint32_t height = framebuffer_height();
    uint32_t pitch = framebuffer_pitch() / 4;

    for (uint32_t y = 0; y < height; y++)
    {
        for (uint32_t x = 0; x < width; x++)
        {
            fb[y * pitch + x] = color;
        }
    }
}

void draw_pixel(
    uint32_t x,
    uint32_t y,
    uint32_t color)
{
    volatile uint32_t *fb = framebuffer_address();

    if (x >= framebuffer_width())
        return;

    if (y >= framebuffer_height())
        return;

    fb[y * (framebuffer_pitch() / 4) + x] = color;
}
void fill_rect(
    int x,
    int y,
    int width,
    int height,
    uint32_t color)
{
    for (int yy = y; yy < y + height; yy++)
    {
        for (int xx = x; xx < x + width; xx++)
        {
            draw_pixel(xx, yy, color);
        }
    }
}
void draw_rect(
    int x,
    int y,
    int width,
    int height,
    uint32_t color)
{
    for (int i = 0; i < width; i++)
    {
        draw_pixel(x + i, y, color);
        draw_pixel(x + i, y + height - 1, color);
    }

    for (int i = 0; i < height; i++)
    {
        draw_pixel(x, y + i, color);
        draw_pixel(x + width - 1, y + i, color);
    }
}