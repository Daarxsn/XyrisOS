/*
 * pmm.c
 * XyrisOS Kernel
 *
 * Physical Memory Manager
 */

#include "pmm.h"
#include "bitmap.h"

#include <stdint.h>
#include <stddef.h>

#define INVALID_PAGE_INDEX ((size_t)-1)

/* --------------------------------------------------
   Internal State
-------------------------------------------------- */

static bitmap_t pmm_bitmap;

/*
 * Temporary bitmap storage.
 * Supports approximately 1 GiB of RAM.
 * Phase 2 will replace this using the
 * bootloader memory map.
 */
static uint8_t bitmap_storage[32768];

static pmm_stats_t stats;

/* --------------------------------------------------
   PMM Initialization
-------------------------------------------------- */

void pmm_init(uint64_t memory_size)
{
    stats.total_memory = memory_size;
    stats.used_memory = 0;
    stats.free_memory = memory_size;

    stats.total_pages = memory_size / PAGE_SIZE;
    stats.used_pages = 0;
    stats.free_pages = stats.total_pages;

    bitmap_init(
        &pmm_bitmap,
        bitmap_storage,
        stats.total_pages
    );
}

/* --------------------------------------------------
   Allocate One Physical Page
-------------------------------------------------- */

void* pmm_alloc_page(void)
{
    size_t page = bitmap_find_free(&pmm_bitmap);

    if (page == INVALID_PAGE_INDEX)
        return INVALID_PAGE;

    bitmap_set(&pmm_bitmap, page);

    stats.used_pages++;
    stats.free_pages--;

    stats.used_memory += PAGE_SIZE;
    stats.free_memory -= PAGE_SIZE;

    /*
     * Phase 1:
     * Physical memory is assumed to be identity-mapped.
     */
    return (void *)((uintptr_t)page * PAGE_SIZE);
}

/* --------------------------------------------------
   Free One Physical Page
-------------------------------------------------- */

void pmm_free_page(void* address)
{
    if (address == NULL)
        return;

    size_t page = ((uintptr_t)address) / PAGE_SIZE;

    if (page >= stats.total_pages)
        return;

    if (!bitmap_test(&pmm_bitmap, page))
        return;

    bitmap_clear(&pmm_bitmap, page);

    stats.used_pages--;
    stats.free_pages++;

    stats.used_memory -= PAGE_SIZE;
    stats.free_memory += PAGE_SIZE;
}

/* --------------------------------------------------
   Statistics
-------------------------------------------------- */

pmm_stats_t pmm_get_stats(void)
{
    return stats;
}