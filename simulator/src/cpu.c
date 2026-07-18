#include <stdio.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#include "cpu.h"

static void pause_ms(int ms)
{
#ifdef _WIN32
    Sleep(ms);
#else
    usleep(ms * 1000);
#endif
}

void cpu_init(void)
{
    printf("[CPU] Detecting Processor...\n");
    pause_ms(350);

    printf("[CPU] Initializing Registers...\n");
    pause_ms(350);

    printf("[CPU] CPU Ready.\n");
}