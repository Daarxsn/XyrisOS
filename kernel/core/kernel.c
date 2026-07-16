#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "boot/limine.h"
#include "graphics/framebuffer.h"
#include "graphics/graphics.h"

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
   XyrisOS Kernel Entry Point
------------------------------------------------- */

void kernel_main(void)
{
    /* Verify Limine protocol revision */
    if (!LIMINE_BASE_REVISION_SUPPORTED(limine_base_revision))
    {
        hcf();
    }

    /* Verify framebuffer availability */
    if (framebuffer_request.response == NULL ||
        framebuffer_request.response->framebuffer_count < 1)
    {
        hcf();
    }

    /* Get framebuffer supplied by Limine */
    struct limine_framebuffer *framebuffer =
        framebuffer_request.response->framebuffers[0];

    /* Initialize framebuffer manager */
    framebuffer_init(framebuffer);

clear_screen(0x101820);

/* Filled blue rectangle */
fill_rect(
    80,
    80,
    300,
    150,
    0x1E88E5
);

/* White border */
draw_rect(
    80,
    80,
    300,
    150,
    0xFFFFFF
);

hcf();
}