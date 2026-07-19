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

static memory_type_t translate_type(uint64_t type)
{
    switch (type)
    {
        case LIMINE_MEMMAP_USABLE:
            return MEMORY_USABLE;

        case LIMINE_MEMMAP_RESERVED:
        case LIMINE_MEMMAP_RESERVED_MAPPED:
            return MEMORY_RESERVED;

        case LIMINE_MEMMAP_ACPI_RECLAIMABLE:
            return MEMORY_ACPI_RECLAIMABLE;

        case LIMINE_MEMMAP_ACPI_NVS:
            return MEMORY_ACPI_NVS;

        case LIMINE_MEMMAP_BAD_MEMORY:
            return MEMORY_BAD_MEMORY;

        case LIMINE_MEMMAP_BOOTLOADER_RECLAIMABLE:
            return MEMORY_BOOTLOADER;

        case LIMINE_MEMMAP_EXECUTABLE_AND_MODULES:
            return MEMORY_KERNEL;

        case LIMINE_MEMMAP_FRAMEBUFFER:
            return MEMORY_FRAMEBUFFER;

        default:
            return MEMORY_UNKNOWN;
    }
}
/*
 * Initialize memory map.
 */

 void memory_map_dump(void){
   for (size_t i = 0; i < memory_map_region_count(); i++)
    {
        const memory_region_t *region = memory_map_region(i);

        printf("Memory Map\n");
        printf("Regions : %llu\n", info.region_count);
        printf("Total   : %llu MB\n", info.total_memory / 1024 / 1024);
        printf("Usable  : %llu MB\n", info.usable_memory / 1024 / 1024);
        printf("Reserved: %llu MB\n", info.reserved_memory / 1024 / 1024);

        /* print:
        Region number
        Base address
        Length
        Type
        */
    }
}
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

      uint64_t count = memmap_request.response->entry_count;

      if (count > MAX_MEMORY_REGIONS)
          count = MAX_MEMORY_REGIONS;

      for (uint64_t i = 0; i < count; i++)
        {
          struct limine_memmap_entry *entry =
          memmap_request.response->entries[i];

          regions[i].base = entry->base;
          regions[i].length = entry->length;
          regions[i].type = translate_type(entry->type);

          info.total_memory += entry->length;

          if (regions[i].type == MEMORY_USABLE)
            info.usable_memory += entry->length;
          else
            info.reserved_memory += entry->length;
        }

      info.region_count = count;
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