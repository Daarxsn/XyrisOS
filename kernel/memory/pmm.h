#ifndef PMM_H
#define PMM_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define PAGE_SIZE 4096

typedef struct {
    uint64_t total_memory;
    uint64_t free_memory;
    uint64_t used_memory;

    size_t total_pages;
    size_t free_pages;
    size_t used_pages;
} pmm_stats_t;

void pmm_init(uint64_t memory_size);

void* pmm_alloc_page(void);

void pmm_free_page(void* page);

pmm_stats_t pmm_get_stats(void);

#endif