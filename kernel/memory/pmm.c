/*
 * pmm.c
 * XyrisOS Kernel
 *
 * Physical Memory Manager
 */

#include "pmm.h"
#include "bitmap.h"
#include "memory_map.h"
#include "hhdm.h"

#include <stdint.h>
#include <stddef.h>

#define PMM_MAX_USABLE_REGIONS 64
#define INVALID_PAGE_INDEX ((size_t)-1)

/* --------------------------------------------------
   Internal State
-------------------------------------------------- */

static bitmap_t frame_bitmap;

/*
 * Virtual address of the bitmap.
 */
static uint8_t *bitmap_buffer = NULL;

/*
 * Physical memory regions available
 * for allocation.
 */
static const memory_region_t*
    usable_regions[PMM_MAX_USABLE_REGIONS];

static size_t usable_region_count = 0;

/*
 * PMM statistics.
 */
static pmm_stats_t stats;

static phys_addr_t pmm_page_address(size_t page)
{
    return (phys_addr_t)page * PAGE_SIZE;
}

static size_t pmm_page_index(phys_addr_t address)
{
    return address / PAGE_SIZE;
}

static void pmm_discover_regions(void)
{
    usable_region_count = 0;

    size_t count = memory_map_region_count();

    for (size_t i = 0; i < count; i++)
    {
        const memory_region_t *region =
            memory_map_region(i);

        if (region->type != MEMORY_USABLE)
            continue;

        if (usable_region_count >= PMM_MAX_USABLE_REGIONS)
            break;

        usable_regions[usable_region_count++] = region;
    }
}

static const memory_region_t*
pmm_largest_region(void)
{
    if (usable_region_count == 0)
        return NULL;

    const memory_region_t *largest =
        usable_regions[0];

    for (size_t i = 1;
         i < usable_region_count;
         i++)
    {
        if (usable_regions[i]->length >
            largest->length)
        {
            largest = usable_regions[i];
        }
    }

    return largest;
}

static size_t pmm_bitmap_bytes(void)
{
    return (stats.total_pages + 7) / 8;
}

static size_t pmm_bitmap_pages(void)
{
    return
        (pmm_bitmap_bytes() + PAGE_SIZE - 1)
        / PAGE_SIZE;
}

static void pmm_initialize_bitmap(const memory_region_t *region)
{
    size_t bitmap_pages = pmm_bitmap_pages();

    /*
     * Place the bitmap at the beginning
     * of the largest usable region.
     */
    phys_addr_t bitmap_phys = region->base;

    bitmap_buffer = phys_to_virt(bitmap_phys);

    bitmap_init(
        &frame_bitmap,
        bitmap_buffer,
        stats.total_pages
    );

    /*
     * Mark everything free initially.
     */
    bitmap_clear_all(&frame_bitmap);

    /*
     * Reserve bitmap pages.
     */
    size_t first =
    pmm_page_index(bitmap_phys);

    pmm_reserve(bitmap_phys, bitmap_pages);
}

static void pmm_reserve_region(
    phys_addr_t base,
    uint64_t length)
{
    size_t pages =
        (length + PAGE_SIZE - 1) / PAGE_SIZE;

    pmm_reserve(base, pages);
}

static void pmm_reserve_non_usable(void)
{
    size_t count = memory_map_region_count();

    for (size_t i = 0; i < count; i++)
    {
        const memory_region_t* region =
            memory_map_region(i);

        if (region->type == MEMORY_USABLE)
            continue;

        pmm_reserve_region(
            region->base,
            region->length
        );
    }
}

void pmm_init(void)
{
    memory_map_info_t map = memory_map_info();

    stats.total_memory = map.total_memory;
    stats.usable_memory = map.usable_memory;
    stats.reserved_memory = map.reserved_memory;

    stats.total_pages =
        map.total_memory / PAGE_SIZE;

    stats.free_pages =
        map.usable_memory / PAGE_SIZE;

    stats.used_pages = 0;

    stats.reserved_pages =
        map.reserved_memory / PAGE_SIZE;

    pmm_discover_regions();

    const memory_region_t *region =
        pmm_largest_region();

    if (region == NULL)
    {
        for (;;)
            __asm__ volatile ("hlt");
    }

    pmm_initialize_bitmap(region);
    pmm_reserve_non_usable();
}

/* --------------------------------------------------
   Allocate One Physical Page
-------------------------------------------------- */

phys_addr_t pmm_alloc_page(void)
{
    size_t page = bitmap_find_free(&frame_bitmap);

    if (page == INVALID_PAGE_INDEX)
        return 0;

    bitmap_set(&frame_bitmap, page);

    stats.used_pages++;
    stats.free_pages--;

    return pmm_page_address(page);
}

/* --------------------------------------------------
   Allocate Multiple Physical Pages
-------------------------------------------------- */

phys_addr_t pmm_alloc_pages(size_t count)
{
    if (count == 0)
        return 0;

    size_t consecutive = 0;
    size_t first = INVALID_PAGE_INDEX;

    for (size_t i = 0; i < stats.total_pages; i++)
    {
        if (!bitmap_test(&frame_bitmap, i))
        {
            if (consecutive == 0)
                first = i;

            consecutive++;

            if (consecutive == count)
            {
                for (size_t j = 0; j < count; j++)
                    bitmap_set(&frame_bitmap, first + j);

                stats.used_pages += count;
                stats.free_pages -= count;

                return pmm_page_address(first);
            }
        }
        else
        {
            consecutive = 0;
        }
    }

    return 0;
}

/* --------------------------------------------------
   Free One Physical Page
-------------------------------------------------- */

void pmm_free_page(phys_addr_t page)
{
    size_t index = pmm_page_index(page);

    if (index >= stats.total_pages)
        return;

    if (!bitmap_test(&frame_bitmap, index))
        return;

    bitmap_clear(&frame_bitmap, index);

    stats.used_pages--;
    stats.free_pages++;
}

/* --------------------------------------------------
   Free Multiple Physical Pages
-------------------------------------------------- */

void pmm_free_pages(
    phys_addr_t page,
    size_t count)
{
    size_t first = pmm_page_index(page);

    for (size_t i = 0; i < count; i++)
    {
        size_t current = first + i;

        if (current >= stats.total_pages)
            break;

        if (!bitmap_test(&frame_bitmap, current))
            continue;

        bitmap_clear(&frame_bitmap, current);

        stats.used_pages--;
        stats.free_pages++;
    }
}

/* --------------------------------------------------
   Reserve Pages
-------------------------------------------------- */

void pmm_reserve(phys_addr_t address, size_t pages)
{
    size_t first = pmm_page_index(address);

    for (size_t i = 0; i < pages; i++)
    {
        if (!bitmap_test(&frame_bitmap, first + i))
        {
            bitmap_set(&frame_bitmap, first + i);

            stats.reserved_pages++;
            stats.free_pages--;
        }
    }
}

/* --------------------------------------------------
   Release Reserved Pages
-------------------------------------------------- */

void pmm_unreserve(phys_addr_t address, size_t pages)
{
    size_t first = pmm_page_index(address);

    for (size_t i = 0; i < pages; i++)
    {
        if (bitmap_test(&frame_bitmap, first + i))
        {
            bitmap_clear(&frame_bitmap, first + i);

            stats.reserved_pages--;
            stats.free_pages++;
        }
    }
}

/* --------------------------------------------------
   Statistics
-------------------------------------------------- */

pmm_stats_t pmm_get_stats(void)
{
    return stats; 
}
