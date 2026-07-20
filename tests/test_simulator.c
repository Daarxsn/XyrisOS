#include <stdio.h>

#include "../simulator/include/simulator.h"
#include "../simulator/include/system.h"

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

int test_simulator(void)
{
    printf("\nRunning Simulator Tests...\n");

    simulator_init();

    ASSERT(system_state.kernel_loaded == 0,
           "Kernel should not be loaded after simulator initialization");

    simulator_execute("boot");

    ASSERT(system_state.kernel_loaded == 1,
           "Kernel should be loaded after boot command");

    simulator_execute("reset");

    ASSERT(system_state.kernel_loaded == 0,
           "Kernel should not be loaded after reset");

    ASSERT(system_state.cpu_online == 1,
           "CPU should be online after reset");

    ASSERT(system_state.memory_online == 1,
           "Memory should be online after reset");

    ASSERT(system_state.display_online == 1,
           "Display should be online after reset");

    ASSERT(system_state.ram_mb == 64,
           "RAM should be restored to 64 MB after reset");

    printf("Simulator tests passed!\n");

    return 0;
}