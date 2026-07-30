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

#include "kernel/memory/memory_map.h"
#include "kernel/memory/hhdm.h"
#include "kernel/memory/pmm.h"
#include "kernel/memory/heap.h"
#include "kernel/memory/vmm.h"

#include "kernel/execution/thread.h"
#include "kernel/execution/execution.h"
#include "kernel/execution/scheduler.h"

#include "boot/boot.h"

#include "image/image.h"
#include "image/logo.h"

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

static void kernel_initialize_graphics(struct limine_framebuffer *fb)
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
    boot_step_ok("Programmable Interrupt Controller Initialized");

    pit_initialize(100);
    boot_step_ok("Programmable Interval Timer Initialized");

    pic_unmask_irq(0);
    boot_step_ok("Timer IRQ0 Enabled");

    __asm__ volatile ("sti");

    boot_step_ok("CPU Interrupts Enabled");
}

static void kernel_initialize_memory(void)
{
    memory_map_init();
    boot_step_ok("Memory Map Initialized");

    hhdm_init();
    boot_step_ok("HHDM Initialized");

    pmm_init();
    boot_step_ok("Physical Memory Manager Initialized");

    heap_init();
    boot_step_ok("Kernel Heap Initialized");

    vmm_init();
    boot_step_ok("Virtual Memory Manager Initialized");
}

static void kernel_initialize_execution(void)
{
    execution_init();
    boot_step_ok("Execution Manager Initialized");
}

/* -------------------------------------------------
   Kernel Initialization
------------------------------------------------- */
static void thread_a(void* arg)
{
    (void)arg;
    
    while (1)
    {
        printk("A");
    }
}

static void thread_b(void* arg)
{
    (void)arg;
    
    while (1)
    {
        printk("B");
    }
}

   static void kernel_initialize_kernel(void)
{
    /* Foundation */

    xkobject_init();
    boot_step_ok("UKOM Initialized");

    xk_capability_init();
    boot_step_ok("Capability Manager Initialized");

    xk_resource_init();
    boot_step_ok("Resource Manager Initialized");

    xk_event_init();
    boot_step_ok("Event Manager Initialized");

    xk_time_init();
    boot_step_ok("Time Manager Initialized");

    xk_config_init();
    boot_step_ok("Configuration Manager Initialized");

    /* scheduler_init() only if execution_init() doesn't already do it */

    /*boot_step_ok("Physical Memory Manager Ready");
    boot_step_ok("Debug Console Ready");

    boot_step_warn("ACPI Not Found");
    boot_step_fail("PCI Enumeration Failed");*/

    execution_create(thread_a, NULL);
    execution_create(thread_b, NULL);
    boot_success("Kernel Ready");
}

/* -------------------------------------------------
   Kernel Entry
------------------------------------------------- */

void kernel_main(void)
{
    struct limine_framebuffer *framebuffer =
        kernel_verify_bootloader();

    kernel_initialize_graphics(framebuffer);

    kernel_initialize_cpu();

    kernel_initialize_memory();

    kernel_initialize_execution();

    kernel_initialize_kernel();

    kernel_initialize_interrupts();

    kernel_idle();
}