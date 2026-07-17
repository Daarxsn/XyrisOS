/* In Phase 1, the kernel assumes an identity-mapped memory model 
(physical addresses are directly usable as virtual addresses).
Later, when we implement the VMM, this assumption will be removed.*/

/*
 * heap.c
 * XyrisOS Kernel
 *
 * Phase 1 Kernel Heap
 * Simple bump allocator
 */

#include "heap.h"
#include "pmm.h"

#include <stdint.h>
#include <stddef.h>

/* --------------------------------------------------
   Heap State
-------------------------------------------------- */

static uint8_t* heap_start = NULL;
static uint8_t* heap_current = NULL;
static uint8_t* heap_end = NULL;

/* --------------------------------------------------
   Alignment
-------------------------------------------------- */

static size_t align8(size_t size)
{
    return (size + 7) & ~7;
}

/* --------------------------------------------------
   Initialize Heap
-------------------------------------------------- */

void heap_init(void)
{
    heap_start = (uint8_t*)pmm_alloc_page();

    if (heap_start == NULL)
        return;

    heap_current = heap_start;
    heap_end = heap_start + PAGE_SIZE;
}

/* --------------------------------------------------
   Allocate Memory
-------------------------------------------------- */

void* kmalloc(size_t size)
{
    if (size == 0)
        return NULL;

    size = align8(size);

    if (heap_current + size > heap_end)
        return NULL;

    void* ptr = heap_current;

    heap_current += size;

    return ptr;
}

/* --------------------------------------------------
   Allocate & Zero Memory
-------------------------------------------------- */

void* kcalloc(size_t count, size_t size)
{
    size_t total = count * size;

    uint8_t* ptr = (uint8_t*)kmalloc(total);

    if (ptr == NULL)
        return NULL;

    for (size_t i = 0; i < total; i++)
        ptr[i] = 0;

    return ptr;
}

/* --------------------------------------------------
   Reallocate Memory
-------------------------------------------------- */

void* krealloc(void* ptr, size_t size)
{
    (void)ptr;

    return kmalloc(size);
}

/* --------------------------------------------------
   Free Memory
-------------------------------------------------- */

void kfree(void* ptr)
{
    (void)ptr;

    /*
     * Phase 1:
     * Bump allocator cannot free memory.
     */
}