#include "memory_tests.h"

#include <stdint.h>

#include "../memory/pmm.h"
#include "../memory/heap.h"
#include "../memory/vmm.h"
#include "../boot/boot.h"

void run_memory_tests(void)
{
    /* ---------------------------------
       PMM Tests
    ---------------------------------- */

    pmm_stats_t before = pmm_get_stats();

    phys_addr_t page = pmm_alloc_page();

    if (page == 0)
    {
        boot_step_fail("PMM Test: Allocate Page");
        return;
    }

    boot_step_ok("PMM Test: Allocate Page");

    pmm_stats_t after_alloc = pmm_get_stats();

    if (after_alloc.free_pages == before.free_pages - 1 &&
        after_alloc.used_pages == before.used_pages + 1)
    {
        boot_step_ok("PMM Test: Statistics After Allocation");
    }
    else
    {
        boot_step_fail("PMM Test: Statistics After Allocation");
    }

    pmm_free_page(page);

    pmm_stats_t after_free = pmm_get_stats();

    if (after_free.free_pages == before.free_pages &&
        after_free.used_pages == before.used_pages)
    {
        boot_step_ok("PMM Test: Free Page");
    }
    else
    {
        boot_step_fail("PMM Test: Free Page");
    }

    /* ---------------------------------
       PMM Multi-Page Stress Test
    ---------------------------------- */

    phys_addr_t pages[10] = {0};

    int success = 1;

    for (int i = 0; i < 10; i++)
    {
        pages[i] = pmm_alloc_page();

        if (pages[i] == 0)
        {
            success = 0;
            break;
        }
    }

    if (success)
    {
        boot_step_ok("PMM Test: Multi-Page Allocation");
    }
    else
    {
        boot_step_fail("PMM Test: Multi-Page Allocation");
    }

    for (int i = 0; i < 10; i++)
    {
        if (pages[i] != 0)
        {
            pmm_free_page(pages[i]);
        }
    }

    /* ---------------------------------
       Heap Tests
    ---------------------------------- */

    void* ptr1 = kmalloc(1);
    void* ptr2 = kmalloc(64);
    void* ptr3 = kmalloc(4096);

    if (ptr1 && ptr2 && ptr3)
    {
        boot_step_ok("Heap Test: Multiple Allocations");
    }
    else
    {
        boot_step_fail("Heap Test: Multiple Allocations");
    }

    /* ---------------------------------
       Heap Alignment Test
    ---------------------------------- */

    if ((((uintptr_t)ptr2) & 0x7) == 0)
    {
        boot_step_ok("Heap Test: Alignment");
    }
    else
    {
        boot_step_fail("Heap Test: Alignment");
    }

    kfree(ptr1);
    kfree(ptr2);
    kfree(ptr3);

    boot_step_ok("Heap Test: Free");

    /* ---------------------------------
       VMM Tests
    ---------------------------------- */

    phys_addr_t phys = pmm_alloc_page();

    if (phys == 0)
    {
        boot_step_fail("VMM Test: Allocate Physical Page");
        return;
    }

    uintptr_t virt = 0xFFFF900010000000ULL;

    if (vmm_map_page(
            vmm_kernel_space(),
            virt,
            phys,
            VMM_WRITABLE))
    {
        boot_step_ok("VMM Test: Map Page");
    }
    else
    {
        boot_step_fail("VMM Test: Map Page");
        pmm_free_page(phys);
        return;
    }

    phys_addr_t translated =
        vmm_translate(vmm_kernel_space(), virt);

    if (translated == phys)
    {
        boot_step_ok("VMM Test: Translate");
    }
    else
    {
        boot_step_fail("VMM Test: Translate");
    }

    /* ---------------------------------
       VMM Get Flags Test
    ---------------------------------- */

    uint64_t page_flags =
        vmm_get_page_flags(
            vmm_kernel_space(),
            virt);

    if ((page_flags & VMM_PRESENT) &&
        (page_flags & VMM_WRITABLE))
    {
        boot_step_ok("VMM Test: Get Flags");
    }
    else
    {
        boot_step_fail("VMM Test: Get Flags");
    }

    if (vmm_unmap_page(vmm_kernel_space(), virt))
    {
        boot_step_ok("VMM Test: Unmap Page");
    }
    else
    {
        boot_step_fail("VMM Test: Unmap Page");
    }

    pmm_free_page(phys);
}