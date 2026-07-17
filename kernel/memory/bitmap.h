#ifndef BITMAP_H
#define BITMAP_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

void bitmap_init(uint8_t* bitmap, size_t bits);

bool bitmap_test(size_t bit);

void bitmap_set(size_t bit);

void bitmap_clear(size_t bit);

int bitmap_find_free(void);

#endif