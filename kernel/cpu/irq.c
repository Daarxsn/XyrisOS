#include "irq.h"
#include "pic.h"
#include "pit.h"

void irq_dispatch(registers_t *regs)
{
    uint8_t irq = (uint8_t)(regs->vector - 32);

    switch (irq)
    {
        case 0:
            pit_handler();
            break;

        default:
            break;
    }

    pic_send_eoi(irq);
}