#include "cpu.h"

void cpu_halt(void)
{
    __asm__ volatile("hlt");
}

void cpu_halt_forever(void)
{
    __asm__ volatile("cli");

    while (1)
    {
        __asm__ volatile("hlt");
    }
}