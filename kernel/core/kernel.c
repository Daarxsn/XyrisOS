#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "boot/limine.h"

#include "graphics/framebuffer.h"
#include "ui/ui.h"

#include "boot/boot.h"
#include "init/init.h"

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
   Kernel Idle
------------------------------------------------- */

static void kernel_idle(void)
{
    while (1)
    {
        __asm__ volatile("hlt");
    }
}

/* -------------------------------------------------
   Verify Bootloader
------------------------------------------------- */

static struct limine_framebuffer *kernel_verify_bootloader(void)
{
    if (!LIMINE_BASE_REVISION_SUPPORTED(limine_base_revision))
        kernel_idle();

    if (framebuffer_request.response == NULL)
        kernel_idle();

    if (framebuffer_request.response->framebuffer_count == 0)
        kernel_idle();

    return framebuffer_request.response->framebuffers[0];
}

/* -------------------------------------------------
   Graphics Initialization
------------------------------------------------- */

static void kernel_initialize_graphics(
    struct limine_framebuffer *framebuffer)
{
    framebuffer_init(framebuffer);

    framebuffer_clear(0x1E1E2E);

    ui_init();
}

/* -------------------------------------------------
   Kernel Entry
------------------------------------------------- */

void kernel_main(void)
{
    /*
     * Verify Limine Bootloader
     */

    struct limine_framebuffer *framebuffer =
        kernel_verify_bootloader();

    /*
     * Initialize Graphics
     */

    kernel_initialize_graphics(framebuffer);

    /*
     * Boot User Interface
     */

    boot_init();

    boot_header();

    /*
     * Initialize Kernel
     */

    kernel_initialize();

    /*
     * Boot Complete
     */

    boot_success("Kernel Ready");

    /*
     * Enter Idle State
     */

    kernel_idle();
}