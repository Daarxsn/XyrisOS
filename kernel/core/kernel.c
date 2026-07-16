#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "boot/limine.h"

/* -------------------------------------------------
   Limine Base Revision
------------------------------------------------- */

__attribute__((used, section(".limine_requests")))
static volatile uint64_t limine_base_revision[] = LIMINE_BASE_REVISION(6);

/* -------------------------------------------------
   Framebuffer Request
------------------------------------------------- */

__attribute__((used, section(".limine_requests")))
static volatile struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST_ID,
    .revision = 0
};

/* -------------------------------------------------
   Request Markers
------------------------------------------------- */

__attribute__((used, section(".limine_requests_start")))
static volatile uint64_t limine_requests_start_marker[] =
    LIMINE_REQUESTS_START_MARKER;

__attribute__((used, section(".limine_requests_end")))
static volatile uint64_t limine_requests_end_marker[] =
    LIMINE_REQUESTS_END_MARKER;

/* -------------------------------------------------
   Halt CPU
------------------------------------------------- */

static void hcf(void)
{
    for (;;)
    {
        __asm__ volatile ("hlt");
    }
}

/* -------------------------------------------------
   XyrisOS Entry Point
------------------------------------------------- */

void kernel_main(void)
{
    if (!LIMINE_BASE_REVISION_SUPPORTED(limine_base_revision))
    {
        hcf();
    }

    if (framebuffer_request.response == NULL ||
        framebuffer_request.response->framebuffer_count < 1)
    {
        hcf();
    }

    struct limine_framebuffer *framebuffer =
        framebuffer_request.response->framebuffers[0];

    volatile uint32_t *fb = framebuffer->address;

    for (size_t y = 0; y < framebuffer->height; y++)
    {
        for (size_t x = 0; x < framebuffer->width; x++)
        {
            uint32_t blue = x * 255 / framebuffer->width;
            uint32_t green = y * 255 / framebuffer->height;

            fb[y * (framebuffer->pitch / 4) + x] =
                (green << 8) | blue;
        }
    }

    hcf();
}