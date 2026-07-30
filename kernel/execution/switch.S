.global switch_context

.text

switch_context:
    /* rdi = old context
       rsi = new context */

    /* Save callee-saved registers */
    movq %r15, 0(%rdi)
    movq %r14, 8(%rdi)
    movq %r13, 16(%rdi)
    movq %r12, 24(%rdi)
    movq %rbx, 32(%rdi)
    movq %rbp, 40(%rdi)

    /* Save current stack pointer */
    movq %rsp, 48(%rdi)

    /* Save return address */
    movq (%rsp), %rax
    movq %rax, 56(%rdi)

    /* Restore callee-saved registers */
    movq 0(%rsi), %r15
    movq 8(%rsi), %r14
    movq 16(%rsi), %r13
    movq 24(%rsi), %r12
    movq 32(%rsi), %rbx
    movq 40(%rsi), %rbp

    /* Restore stack pointer */
    movq 48(%rsi), %rsp

    /* Jump to next RIP */
    movq 56(%rsi), %rax
    jmp *%rax