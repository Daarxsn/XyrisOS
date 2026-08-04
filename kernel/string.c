#include "string.h"

void* memset(void* ptr, int value, size_t num)
{
    unsigned char* buffer = (unsigned char*)ptr;

    for (size_t i = 0; i < num; i++)
    {
        buffer[i] = (unsigned char)value;
    }

    return ptr;
}

char *itoa(
    unsigned long long value,
    char *buffer,
    int base)
{
    if (base < 2 || base > 16)
    {
        buffer[0] = '\0';
        return buffer;
    }

    static const char digits[] = "0123456789ABCDEF";

    char *start = buffer;
    char *ptr = buffer;

    do
    {
        *ptr++ = digits[value % base];
        value /= base;
    }
    while (value);

    *ptr = '\0';

    ptr--;

    while (start < ptr)
    {
        char temp = *start;
        *start = *ptr;
        *ptr = temp;

        start++;
        ptr--;
    }

    return buffer;
}