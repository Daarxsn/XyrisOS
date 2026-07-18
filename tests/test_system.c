#include <stdio.h>
#include <stdlib.h>

#include "../simulator/include/system.h"

#define ASSERT(condition, message)        \
    do                                    \
    {                                     \
        if (!(condition))                 \
        {                                 \
            printf("[FAIL] %s\n", message);\
            return 1;                     \
        }                                 \
        printf("[PASS] %s\n", message);   \
    } while (0)

int main(void)
{
    printf("Running XyrisOS System Tests...\n\n");

    system_init();

    ASSERT(system_state.cpu_online == 1,
           "CPU should be online after system initialization");

    ASSERT(system_state.memory_online == 1,
           "Memory should be online after system initialization");

    ASSERT(system_state.display_online == 1,
           "Display should be online after system initialization");

    ASSERT(system_state.kernel_loaded == 0,
           "Kernel should not be loaded initially");

    ASSERT(system_state.ram_mb == 64,
           "System should initialize with 64 MB RAM");

    printf("\nAll system tests passed!\n");

    return 0;
}