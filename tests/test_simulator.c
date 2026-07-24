#include <stdio.h>

#include "../simulator/include/simulator.h"
#include "../simulator/include/system.h"
#include "../simulator/include/cpu.h"
#include "../simulator/include/memory.h"

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

int test_simulator_commands(void)
{
    printf("\nRunning Simulator Command Tests...\n");

    /*
     * Initialize the simulator.
     */
    simulator_init();

    /*
     * Test help command.
     */
    ASSERT(
        simulator_execute("help") == 0,
        "Help command should execute successfully"
    );

    /*
     * Test status command.
     */
    ASSERT(
        simulator_execute("status") == 0,
        "Status command should execute successfully"
    );

    /*
     * Test boot command.
     */
    system_state.kernel_loaded = 0;

    ASSERT(
        simulator_execute("boot") == 0,
        "Boot command should execute successfully"
    );

    ASSERT(
        system_state.kernel_loaded == 1,
        "Boot command should load the kernel"
    );

    /*
     * Test boot command when kernel is already loaded.
     */
    ASSERT(
        simulator_execute("boot") == 0,
        "Boot command should be safe when kernel is already loaded"
    );

    ASSERT(
        system_state.kernel_loaded == 1,
        "Kernel should remain loaded after repeated boot"
    );

    /*
     * Test reset command.
     */
    ASSERT(
        simulator_execute("reset") == 0,
        "Reset command should execute successfully"
    );

    ASSERT(
        system_state.kernel_loaded == 0,
        "Reset should unload the kernel"
    );

    ASSERT(
        system_state.cpu_online == 1,
        "Reset should restore CPU state"
    );

    ASSERT(
        system_state.memory_online == 1,
        "Reset should restore memory state"
    );

    ASSERT(
        system_state.display_online == 1,
        "Reset should restore display state"
    );

    ASSERT(
        system_state.ram_mb == 64,
        "Reset should restore RAM configuration"
    );

    /*
     * Test repeated reset.
     */
    ASSERT(
        simulator_execute("reset") == 0,
        "Repeated reset should execute safely"
    );

    ASSERT(
        system_state.kernel_loaded == 0,
        "Repeated reset should keep kernel unloaded"
    );

    /*
     * Test unknown command handling.
     */
    ASSERT(
        simulator_execute("invalid_command") == 0,
        "Unknown command should be handled safely"
    );

    /*
     * Test empty command handling.
     */
    ASSERT(
        simulator_execute("") == 0,
        "Empty command should be handled safely"
    );

    /*
     * Test exit command.
     */
    ASSERT(
        simulator_execute("exit") == 1,
        "Exit command should return exit status"
    );

    printf("Simulator command tests passed!\n");

    return 0;
}