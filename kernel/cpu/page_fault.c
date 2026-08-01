#include "page_fault.h"

#include "cpu.h"

#include "../logger/logger.h"
#include "../debug/panic.h"
#include "../debug/print.h"
#include "../debug/hex.h"

void page_fault_handler(registers_t *regs)
{
    uintptr_t fault_address = cpu_read_cr2();

    klog_fatal("PAGE FAULT");

    debug_print_line("");
    debug_print_line("========== PAGE FAULT ==========");

    debug_print("Fault Address : ");
    debug_print_hex64((uint64_t)fault_address);

    debug_print("Access Type   : ");
    if (regs->error_code & (1ULL << 1))
        debug_print_line("Write");
    else
        debug_print_line("Read");

    debug_print("Privilege     : ");
    if (regs->error_code & (1ULL << 2))
        debug_print_line("User");
    else
        debug_print_line("Kernel");

    debug_print("Reason        : ");
    if (regs->error_code & 1ULL)
        debug_print_line("Protection Violation");
    else
        debug_print_line("Non-Present Page");

    debug_print_line("===============================");
    debug_print_line("");

    kernel_panic(regs, "Page Fault");
}