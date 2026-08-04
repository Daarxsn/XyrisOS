#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "foundation/resource.h"
#include "boot/limine.h"

#include "foundation/ukom.h"
#include "foundation/capability.h"
#include "foundation/event.h"
#include "foundation/time.h"
#include "foundation/config.h"

#include "graphics/framebuffer.h"
#include "ui/ui.h"

#include "cpu/gdt.h"
#include "cpu/idt.h"
#include "cpu/isr.h"
#include "cpu/pic.h"
#include "cpu/pit.h"

#include "memory/memory_map.h"
#include "memory/hhdm.h"
#include "memory/pmm.h"
#include "memory/heap.h"
#include "memory/vmm.h"

#include "execution/thread.h"
#include "execution/execution.h"
#include "execution/scheduler.h"

#include "boot/boot.h"

#include "image/image.h"
#include "image/logo.h"

#include "../tests/tests.h"

#include "debug/print.h"
#include "debug/hex.h"

#include "debug/print.h"
#include "string.h"


static void thread_a(void* arg);
static void thread_b(void* arg);



/* -------------------------------------------------
   Limine Requests
------------------------------------------------- */

__attribute__((used, section(".limine_requests")))
static volatile uint64_t limine_base_revision[] =
    LIMINE_BASE_REVISION(6);


__attribute__((used, section(".limine_requests")))
static volatile struct limine_framebuffer_request framebuffer_request =
{
    .id = LIMINE_FRAMEBUFFER_REQUEST_ID,
    .revision = 0
};


__attribute__((used, section(".limine_requests_start")))
static volatile uint64_t limine_requests_start_marker[] =
    LIMINE_REQUESTS_START_MARKER;


__attribute__((used, section(".limine_requests_end")))
static volatile uint64_t limine_requests_end_marker[] =
    LIMINE_REQUESTS_END_MARKER;



/* -------------------------------------------------
   Idle Loop
------------------------------------------------- */

static void kernel_idle(void)
{
    while (1)
    {
        __asm__ volatile ("hlt");
    }
}



/* -------------------------------------------------
   Boot Verification
------------------------------------------------- */

static struct limine_framebuffer* kernel_verify_bootloader(void)
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
    struct limine_framebuffer* fb
)
{
    framebuffer_init(fb);

    framebuffer_clear(0x1E1E2E);

    ui_init();

    boot_init();
    boot_header();

    boot_step_ok("Framebuffer Initialized");
    boot_step_ok("Graphics Engine Initialized");
}



/* -------------------------------------------------
   CPU Initialization
------------------------------------------------- */

static void kernel_initialize_cpu(void)
{
    gdt_init();
    boot_step_ok("Global Descriptor Table Loaded");

    idt_init();
    boot_step_ok("Interrupt Descriptor Table Loaded");

    isr_init();
    boot_step_ok("Interrupt Service Routines Loaded");
}



/* -------------------------------------------------
   Interrupt Initialization
------------------------------------------------- */

static void kernel_initialize_interrupts(void)
{
    pic_initialize();

    boot_step_ok(
        "Programmable Interrupt Controller Initialized"
    );


    pit_initialize(100);

    boot_step_ok(
        "Programmable Interval Timer Initialized"
    );


    pic_unmask_irq(0);

    boot_step_ok(
        "Timer IRQ0 Enabled"
    );


    __asm__ volatile ("sti");

    boot_step_ok(
        "CPU Interrupts Enabled"
    );
}



/* -------------------------------------------------
   Memory Initialization
------------------------------------------------- */

static void kernel_initialize_memory(void)
{
    memory_map_init();

    boot_step_ok(
        "Memory Map Initialized"
    );


    hhdm_init();

    boot_step_ok(
        "HHDM Initialized"
    );


    pmm_init();

pmm_stats_t stats = pmm_get_stats();

if (stats.free_pages > 0)
{
    boot_step_ok("PMM Has Free Pages");
}
else
{
    boot_step_fail("PMM Has NO Free Pages");
}

boot_step_ok("Physical Memory Manager Initialized");

/* ------------------------------------
   Memory Statistics
------------------------------------ */

debug_print_line("");
debug_print_line("========== MEMORY STATISTICS ==========");

uint64_t total_mb =
    stats.total_memory / (1024 * 1024);

uint64_t usable_mb =
    stats.usable_memory / (1024 * 1024);

uint64_t reserved_mb =
    stats.reserved_memory / (1024 * 1024);

char buffer[32];

debug_print("Total Memory    : ");
itoa(total_mb, buffer, 10);
debug_print(buffer);
debug_print_line(" MB");

debug_print("Usable Memory   : ");
itoa(usable_mb, buffer, 10);
debug_print(buffer);
debug_print_line(" MB");

debug_print("Reserved Memory : ");
itoa(reserved_mb, buffer, 10);
debug_print(buffer);
debug_print_line(" MB");

debug_print("Total Pages     : ");
itoa(stats.total_pages, buffer, 10);
debug_print_line(buffer);

debug_print("Free Pages      : ");
itoa(stats.free_pages, buffer, 10);
debug_print_line(buffer);

debug_print("Used Pages      : ");
itoa(stats.used_pages, buffer, 10);
debug_print_line(buffer);

debug_print("Reserved Pages  : ");
itoa(stats.reserved_pages, buffer, 10);
debug_print_line(buffer);

debug_print_line("=======================================");
debug_print_line("");

    heap_init();
    void* heap_debug = kmalloc(16);

if (heap_debug)
{
    boot_step_ok("Heap Allocation Working");
}
else
{
    boot_step_fail("Heap Allocation Broken");
}

    boot_step_ok(
        "Kernel Heap Initialized"
    );


    vmm_init();

    boot_step_ok(
        "Virtual Memory Manager Initialized"
    );
}



/* -------------------------------------------------
   Execution Initialization
------------------------------------------------- */

static void kernel_initialize_execution(void)
{
    execution_init();

    boot_step_ok(
        "Execution Manager Initialized"
    );
}



/* -------------------------------------------------
   Test Threads
------------------------------------------------- */

static void thread_a(void* arg)
{
    (void)arg;

    while (1)
    {
        __asm__ volatile ("hlt");
    }
}


static void thread_b(void* arg)
{
    (void)arg;

    while (1)
    {
        __asm__ volatile ("hlt");
    }
}



/* -------------------------------------------------
   Kernel Services
------------------------------------------------- */

static void kernel_initialize_kernel(void)
{
    xkobject_init();

    boot_step_ok(
        "UKOM Initialized"
    );


    xk_capability_init();

    boot_step_ok(
        "Capability Manager Initialized"
    );


    xk_resource_init();

    boot_step_ok(
        "Resource Manager Initialized"
    );


    xk_event_init();

    boot_step_ok(
        "Event Manager Initialized"
    );


    xk_time_init();

    boot_step_ok(
        "Time Manager Initialized"
    );


    xk_config_init();

    boot_step_ok(
        "Configuration Manager Initialized"
    );


    execution_create(
        thread_a,
        NULL
    );


    execution_create(
        thread_b,
        NULL
    );

    /* -------------------------------------------------
   Heap Test
------------------------------------------------- */

void* heap_test1 = kmalloc(64);
void* heap_test2 = kmalloc(128);

if (heap_test1 != NULL && heap_test2 != NULL)
{
    boot_step_ok("Kernel Heap Test Passed");
}
else
{
    boot_step_fail("Kernel Heap Test Failed");
}

kfree(heap_test1);
kfree(heap_test2);
/* ------------------------------
   VMM Test
------------------------------ */

phys_addr_t page = pmm_alloc_page();

if (page != 0)
{
    if (vmm_map_page(
            vmm_kernel_space(),
            0xFFFF900000000000ULL,
            page,
            VMM_WRITABLE))
    {
        boot_step_ok("Virtual Memory Manager Test Passed");
    }
    else
    {
        boot_step_fail("Virtual Memory Manager Test Failed");
    }
}
else
{
    boot_step_fail("PMM Allocation Failed");
}



run_kernel_tests();

    boot_success(
        "Kernel Ready"
    );
}



/* -------------------------------------------------
   Kernel Entry
------------------------------------------------- */

void kernel_main(void)
{
    struct limine_framebuffer* framebuffer =
        kernel_verify_bootloader();


    kernel_initialize_graphics(
        framebuffer
    );


    kernel_initialize_cpu();


    kernel_initialize_memory();


    kernel_initialize_execution();


    kernel_initialize_kernel();


    kernel_initialize_interrupts();


    kernel_idle();
}