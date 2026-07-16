#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "boot/limine.h"

#include "graphics/framebuffer.h"
#include "graphics/graphics.h"
#include "graphics/font.h"

#include "ui/ui.h"

#include "image/image.h"
#include "image/logo.h"

/* -------------------------------------------------
   Limine Base Revision
------------------------------------------------- */

__attribute__((used, section(".limine_requests")))
static volatile uint64_t limine_base_revision[] =
    LIMINE_BASE_REVISION(6);

/* -------------------------------------------------
   Framebuffer Request
------------------------------------------------- */

__attribute__((used, section(".limine_requests")))
static volatile struct limine_framebuffer_request framebuffer_request =
{
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
   Kernel Entry
------------------------------------------------- */

void kernel_main(void)
{
    /* Verify Limine revision */

    if (!LIMINE_BASE_REVISION_SUPPORTED(limine_base_revision))
    {
        hcf();
    }

    /* Verify framebuffer */

    if (framebuffer_request.response == NULL ||
        framebuffer_request.response->framebuffer_count < 1)
    {
        hcf();
    }

    /* Get framebuffer */

    struct limine_framebuffer *framebuffer =
        framebuffer_request.response->framebuffers[0];

    /* Initialize framebuffer */

    framebuffer_init(framebuffer);

    /* Initialize UI */

    ui_init();

    /* -------------------------------------------------
       Background
    ------------------------------------------------- */

    clear_screen(0x101820);

    /* -------------------------------------------------
       XyrisOS Logo
    ------------------------------------------------- */

draw_image_scaled(
    center_x(256),
    40,
    &xyris_logo,
    256,
    256
);

    /* -------------------------------------------------
       Title
    ------------------------------------------------- */

    draw_string(
        center_x(7 * 8),
        570,
        "XyrisOS",
        0xFFFFFF
    );

    /* -------------------------------------------------
       Subtitle
    ------------------------------------------------- */

    draw_string(
        center_x(22 * 8),
        595,
        "The Next Generation OS",
        0x4FC3F7
    );

    /* -------------------------------------------------
       Status
    ------------------------------------------------- */

    draw_string(
        center_x(19 * 8),
        620,
        "Booting Kernel...",
        0x00FF88
    );

    /* -------------------------------------------------
       Halt
    ------------------------------------------------- */

    hcf();
}