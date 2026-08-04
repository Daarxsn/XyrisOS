#ifndef XYRIS_CONTEXT_H
#define XYRIS_CONTEXT_H

#include <stdint.h>

/*
 * ============================================================
 * XyrisOS Context Manager
 * ------------------------------------------------------------
 * Architecture-independent CPU context interface.
 * ============================================================
 */

/*
 * ------------------------------------------------------------
 * CPU Context
 * ------------------------------------------------------------
 *
 * NOTE:
 * This is intentionally minimal.
 * The real register save/restore implementation
 * will be added during the assembly context switch
 * phase.
 */

typedef struct context
{
    uint64_t rsp;

    uint64_t rip;

    uint64_t rflags;

} context_t;

/*
 * ------------------------------------------------------------
 * Context API
 * ------------------------------------------------------------
 */

/*
 * Initialize context manager.
 */
void context_initialize(void);

/*
 * Save current CPU context.
 */
void context_save(context_t *context);

/*
 * Restore CPU context.
 */
void context_restore(context_t *context);

/*
 * Switch execution from one context to another.
 */
void context_switch(
    context_t *current,
    context_t *next
);

#endif