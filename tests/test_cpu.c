#include <stdio.h>

#include "../simulator/include/cpu.h"

#define ASSERT(condition, message)        \
    do                                    \
    {                                     \
        if (!(condition))                 \
        {                                 \
            printf("[FAIL] %s\n", message); \
            return 1;                     \
        }                                 \
        printf("[PASS] %s\n", message);   \
    } while (0)

int test_cpu(void)
{
    printf("\nRunning CPU Tests...\n");

    cpu_ready = 0;

    ASSERT(cpu_ready == 0,
           "CPU should not be ready before initialization");

    cpu_init();

    ASSERT(cpu_ready == 1,
           "CPU should be ready after initialization");

    printf("CPU tests passed!\n");

    return 0;
}