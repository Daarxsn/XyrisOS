/*
 * vmm.c
 * XyrisOS Kernel
 *
 * Phase 1 Virtual Memory Skeleton
 */

#include "vmm.h"

void vmm_init(void)
{
    /* TODO: Initialize virtual memory subsystem */
}

void map_page(uint64_t virtual_addr,
              uint64_t physical_addr,
              uint64_t flags)
{
    (void)virtual_addr;
    (void)physical_addr;
    (void)flags;

    /* TODO: Implement page mapping */
}

void unmap_page(uint64_t virtual_addr)
{
    (void)virtual_addr;

    /* TODO: Implement page unmapping */
}

uint64_t translate_page(uint64_t virtual_addr)
{
    (void)virtual_addr;

    /* TODO: Implement address translation */

    return 0;
}