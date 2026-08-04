#ifndef XYRIS_STRING_H
#define XYRIS_STRING_H

#include <stddef.h>

/*
 * ============================================================
 * XyrisOS Kernel String Library
 * ============================================================
 */

/*
 * Memory operations
 */

void *memset(
    void *destination,
    int value,
    size_t size
);

void *memcpy(
    void *destination,
    const void *source,
    size_t size
);

int memcmp(
    const void *left,
    const void *right,
    size_t size
);

void *memmove(
    void *destination,
    const void *source,
    size_t size
);

/*
 * String operations
 */

size_t strlen(
    const char *string
);

char *strcpy(
    char *destination,
    const char *source
);

char *strncpy(
    char *destination,
    const char *source,
    size_t size
);

#endif