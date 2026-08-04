#include "string.h"

/*
 * ============================================================
 * XyrisOS Kernel String Library
 * ============================================================
 */

void *memset(
    void *destination,
    int value,
    size_t size)
{
    unsigned char *ptr = (unsigned char *)destination;

    while (size--)
    {
        *ptr++ = (unsigned char)value;
    }

    return destination;
}

void *memcpy(
    void *destination,
    const void *source,
    size_t size)
{
    unsigned char *dst = (unsigned char *)destination;

    const unsigned char *src =
        (const unsigned char *)source;

    while (size--)
    {
        *dst++ = *src++;
    }

    return destination;
}

int memcmp(
    const void *left,
    const void *right,
    size_t size)
{
    const unsigned char *a = left;
    const unsigned char *b = right;

    while (size--)
    {
        if (*a != *b)
        {
            return *a - *b;
        }

        a++;
        b++;
    }

    return 0;
}

void *memmove(
    void *destination,
    const void *source,
    size_t size)
{
    unsigned char *dst =
        (unsigned char *)destination;

    const unsigned char *src =
        (const unsigned char *)source;

    if (dst < src)
    {
        while (size--)
        {
            *dst++ = *src++;
        }
    }
    else
    {
        dst += size;
        src += size;

        while (size--)
        {
            *--dst = *--src;
        }
    }

    return destination;
}

size_t strlen(
    const char *string)
{
    size_t length = 0;

    while (*string++)
    {
        length++;
    }

    return length;
}

char *strcpy(
    char *destination,
    const char *source)
{
    char *start = destination;

    while ((*destination++ = *source++))
    {
    }

    return start;
}

char *strncpy(
    char *destination,
    const char *source,
    size_t size)
{
    char *start = destination;

    while (size && *source)
    {
        *destination++ = *source++;
        size--;
    }

    while (size--)
    {
        *destination++ = '\0';
    }

    return start;
}