/*
 * heap.c
 * XyrisOS Kernel
 *
 * Simple kernel heap allocator
 */

#include "heap.h"
#include "pmm.h"
#include "hhdm.h"

#include <stdint.h>
#include <stddef.h>


/* --------------------------------------------------
   Heap State
-------------------------------------------------- */

static uint8_t* heap_start = NULL;
static uint8_t* heap_current = NULL;
static uint8_t* heap_end = NULL;


/*
 * Number of pages reserved for initial heap.
 *
 * This can be increased later when
 * a real allocator is implemented.
 */
#define HEAP_INITIAL_PAGES 16



/* --------------------------------------------------
   Alignment
-------------------------------------------------- */

static size_t align16(size_t size)
{
    return (size + 15) & ~15;
}



/* --------------------------------------------------
   Initialize Heap
-------------------------------------------------- */

void heap_init(void)
{
    phys_addr_t first_page =
        pmm_alloc_pages(HEAP_INITIAL_PAGES);


    if(first_page == 0)
    {
        heap_start = NULL;
        heap_current = NULL;
        heap_end = NULL;
        return;
    }


    heap_start =
        (uint8_t*)phys_to_virt(first_page);


    if(heap_start == NULL)
    {
        return;
    }


    heap_current = heap_start;


    heap_end =
        heap_start +
        (HEAP_INITIAL_PAGES * PAGE_SIZE);
}



/* --------------------------------------------------
   Allocate
-------------------------------------------------- */

void* kmalloc(size_t size)
{
    if(size == 0)
        return NULL;


    if(heap_current == NULL)
        return NULL;


    size = align16(size);


    if(heap_current + size > heap_end)
        return NULL;


    void* ptr =
        heap_current;


    heap_current += size;


    return ptr;
}



/* --------------------------------------------------
   Allocate zeroed
-------------------------------------------------- */

void* kcalloc(
    size_t count,
    size_t size)
{
    size_t total =
        count * size;


    uint8_t* ptr =
        kmalloc(total);


    if(ptr == NULL)
        return NULL;


    for(size_t i = 0; i < total; i++)
        ptr[i] = 0;


    return ptr;
}



/* --------------------------------------------------
   Reallocate
-------------------------------------------------- */

void* krealloc(
    void* ptr,
    size_t size)
{
    (void)ptr;


    return kmalloc(size);
}



/* --------------------------------------------------
   Free
-------------------------------------------------- */

void kfree(void* ptr)
{
    /*
     * Phase 1 bump allocator:
     * freeing is not supported.
     */

    (void)ptr;
}