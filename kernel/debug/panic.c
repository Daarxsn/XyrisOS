#include "panic.h"

void kernel_panic(registers_t *regs, const char *reason)
{
    (void)regs;
    (void)reason;

    for (;;)
    {
        __asm__ volatile ("hlt");
    }
}