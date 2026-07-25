#include "isr.h"
#include "exceptions.h"
#include "irq.h"

void isr_handler(registers_t *regs)
{
    if (regs->vector < 32)
    {
        exception_dispatch(regs);
    }
    else
    {
        irq_dispatch(regs);
    }
}

void isr_init(void)
{
}