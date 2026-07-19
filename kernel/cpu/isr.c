#include "isr.h"
#include "exceptions.h"

void isr_handler(registers_t *regs)
{
    exception_dispatch(regs);
}

void isr_init(void)
{
}