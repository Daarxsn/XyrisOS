#ifndef XYRIS_PROCESS_H
#define XYRIS_PROCESS_H

#include <stdint.h>
#include <stdbool.h>

/*
 * ============================================================
 * XyrisOS Process Manager
 * ------------------------------------------------------------
 * Defines the Process Control Block (PCB) and public API.
 * ============================================================
 */

/*
 * ------------------------------------------------------------
 * Configuration
 * ------------------------------------------------------------
 */

#define PROCESS_NAME_LENGTH    32
#define PROCESS_MAX_PROCESSES  256

/*
 * ------------------------------------------------------------
 * Process Identifier
 * ------------------------------------------------------------
 */

typedef uint32_t process_id_t;

/*
 * ------------------------------------------------------------
 * Process State
 * ------------------------------------------------------------
 */

typedef enum
{
    PROCESS_CREATED = 0,

    PROCESS_READY,

    PROCESS_RUNNING,

    PROCESS_BLOCKED,

    PROCESS_SLEEPING,

    PROCESS_TERMINATED

} process_state_t;

/*
 * Forward declaration.
 * The complete definition exists in thread.h.
 */

struct thread;

/*
 * ------------------------------------------------------------
 * Process Control Block (PCB)
 * ------------------------------------------------------------
 */

typedef struct process
{
    /*
     * Identification
     */
    process_id_t pid;

    char name[PROCESS_NAME_LENGTH];

    /*
     * Current process state
     */
    process_state_t state;

    /*
     * Main thread
     */
    struct thread *main_thread;

    /*
     * Memory information
     * (Used later by Member 6)
     */
    void *address_space;

    /*
     * CPU accounting
     */
    uint64_t cpu_time;

    uint64_t wakeup_tick;

    /*
     * Kernel or user process
     */
    bool kernel_process;

} process_t;

/*
 * ------------------------------------------------------------
 * Process Manager API
 * ------------------------------------------------------------
 */

void process_manager_initialize(void);

process_t *process_create(
    const char *name,
    bool kernel_process
);

void process_destroy(
    process_t *process
);

process_t *process_lookup(
    process_id_t pid
);

process_t *process_current(void);

void process_set_state(
    process_t *process,
    process_state_t state
);

process_state_t process_get_state(
    const process_t *process
);

#endif