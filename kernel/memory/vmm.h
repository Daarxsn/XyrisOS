#ifndef XYRIS_VMM_H
#define XYRIS_VMM_H

#include <stdint.h>
#include <stddef.h>

void vmm_init(void);

void map_page(uint64_t virtual_addr,
              uint64_t physical_addr,
              uint64_t flags);

void unmap_page(uint64_t virtual_addr);

uint64_t translate_page(uint64_t virtual_addr);

#endif