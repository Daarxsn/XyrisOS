#include "drivers/keyboard.h"

#include "cpu/io.h"
#include "cpu/irq.h"

static bool keyboard_initialized = false;

/* ------------------------------------------------------------
 * Driver Initialization
 * ------------------------------------------------------------ */

bool xk_keyboard_initialize(void)
{
    keyboard_initialized = true;

    xk_irq_register(
        1,
        xk_keyboard_irq_handler
    );

    return true;
}

/* ------------------------------------------------------------
 * Driver Shutdown
 * ------------------------------------------------------------ */

void xk_keyboard_shutdown(void)
{
    xk_irq_unregister(1);

    keyboard_initialized = false;
}


/* ------------------------------------------------------------
 * Status Check
 * ------------------------------------------------------------ */

bool xk_keyboard_has_data(void)
{
    return (inb(XK_PS2_STATUS_PORT) & 1);
}

/* ------------------------------------------------------------
 * Read Scan Code
 * ------------------------------------------------------------ */

uint8_t xk_keyboard_read_scancode(void)
{
    return inb(XK_PS2_DATA_PORT);
}

/* ------------------------------------------------------------
 * IRQ1 Handler
 * ------------------------------------------------------------ */

void xk_keyboard_irq_handler(
    registers_t *regs)
{
    (void)regs;

    if (!keyboard_initialized)
    {
        return;
    }

    if (xk_keyboard_has_data())
    {
        uint8_t scancode =
            xk_keyboard_read_scancode();

        (void)scancode;

        /*
         * Next phase:
         * Translate scancode into
         * key events.
         */
    }
}


/* ------------------------------------------------------------
 * Driver Object
 * ------------------------------------------------------------ */

XKDriver xk_keyboard_driver =
{
    .name = "PS/2 Keyboard",
    .type = XK_DRIVER_KEYBOARD,
    .state = XK_DRIVER_UNINITIALIZED,
    .initialize = xk_keyboard_initialize,
    .shutdown = xk_keyboard_shutdown
};