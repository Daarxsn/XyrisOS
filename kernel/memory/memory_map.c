/*
 * memory_map.c
 *
 * XyrisOS
 *
 * Bootloader memory map interface.
 */

#include "memory_map.h"

#include <stddef.h>
#include <stdint.h>
#include "../boot/limine.h"

/*
 * Phase 2
 *
 * Temporary storage.
 *
 * Later these values will come directly
 * from the Limine bootloader response.
 */

#define MAX_MEMORY_REGIONS 128

static memory_region_t regions[MAX_MEMORY_REGIONS];

static memory_map_info_t info;

static const struct limine_memmap_response* get_memmap(void)
{
    return memmap_request.response;
}

__attribute__((used, section(".limine_requests")))
static volatile struct limine_memmap_request memmap_request = {
    .id = LIMINE_MEMMAP_REQUEST_ID,
    .revision = 0,
    .response = NULL
};

/*
 * Initialize memory map.
 */
void memory_map_init(void)
{
    if (memmap_request.response == NULL)
    {
        for (;;)
            __asm__ volatile ("hlt");
    }

    info.total_memory = 0;
    info.usable_memory = 0;
    info.reserved_memory = 0;
    info.region_count = 0;
}
/*
 * Region count.
 */
size_t memory_map_region_count(void)
{
    return info.region_count;
}

/*
 * Get one region.
 */
const memory_region_t*
memory_map_region(size_t index)
{
    if (index >= info.region_count)
        return NULL;

    return &regions[index];
}

/*
 * Memory statistics.
 */
memory_map_info_t
memory_map_info(void)
{
    return info;
}