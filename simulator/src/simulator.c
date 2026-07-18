#include <stdio.h>
#include <string.h>

#include "simulator.h"
#include "cpu.h"
#include "memory.h"
#include "display.h"
#include "system.h"

void simulator_init(void)
{
    printf("Booting XyrisSim...\n\n");

    system_init();

    cpu_init();
    memory_init();
    display_init();

    printf("\nSystem Ready!\n");
}

int simulator_execute(const char *command)
{
    if (strcmp(command, "help") == 0)
    {
        printf("\nAvailable Commands\n");
        printf("------------------\n");
        printf("help\n");
        printf("status\n");
        printf("boot\n");
        printf("clear\n");
        printf("exit\n");
    }

    else if (strcmp(command, "status") == 0)
    {
        printf("\nSystem Status\n");
        printf("------------------\n");

        printf("CPU      : %s\n",
               system_state.cpu_online ? "ONLINE" : "OFFLINE");

        printf("Memory   : %d MB\n",
               system_state.ram_mb);

        printf("Display  : %s\n",
               system_state.display_online ? "READY" : "OFFLINE");

        printf("Kernel   : %s\n",
               system_state.kernel_loaded ? "LOADED" : "NOT LOADED");
    }

    else if (strcmp(command, "boot") == 0)
    {
        printf("\nLoading Kernel...\n");
        printf("Initializing Scheduler...\n");
        printf("Initializing Drivers...\n");
        printf("Initializing Memory Manager...\n");

        system_state.kernel_loaded = 1;

        printf("\nKernel Boot Successful!\n");
    }

    else if (strcmp(command, "clear") == 0)
    {
        for (int i = 0; i < 40; i++)
        {
            printf("\n");
        }
    }

    else if (strcmp(command, "exit") == 0)
    {
        printf("Exiting XyrisSim...\n");
        return 1;
    }

    else
    {
        printf("Unknown command. Type 'help'.\n");
    }

    return 0;
}