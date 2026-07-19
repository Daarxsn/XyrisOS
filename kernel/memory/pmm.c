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