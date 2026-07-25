#ifndef XYRIS_IRQ_H
#define XYRIS_IRQ_H

#include "isr.h"

void irq_dispatch(registers_t *regs);

#endif