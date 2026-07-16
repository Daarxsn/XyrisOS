#include "framebuffer.h"

static struct limine_framebuffer *g_framebuffer = 0;

void framebuffer_init(struct limine_framebuffer *fb)
{
    g_framebuffer = fb;
}

uint32_t framebuffer_width(void)
{
    return g_framebuffer->width;
}

uint32_t framebuffer_height(void)
{
    return g_framebuffer->height;
}

uint32_t framebuffer_pitch(void)
{
    return g_framebuffer->pitch;
}

volatile uint32_t *framebuffer_address(void)
{
    return (volatile uint32_t *)g_framebuffer->address;
}