#ifndef __X86_64_ASM_DEFNS_H__
#define __X86_64_ASM_DEFNS_H__

#ifndef NDEBUG
/* Indicate special exception stack frame by inverting the frame pointer. */
#define SETUP_EXCEPTION_FRAME_POINTER           \
        movq  %rsp,%rbp;                        \
        notq  %rbp
#else
#define SETUP_EXCEPTION_FRAME_POINTER
#endif

#define SAVE_ALL                                \
        cld;                                    \
        pushq %rdi;                             \
        pushq %rsi;                             \
        pushq %rdx;                             \
        pushq %rcx;                             \
        pushq %rax;                             \
        pushq %r8;                              \
        pushq %r9;                              \
        pushq %r10;                             \
        pushq %r11;                             \
        pushq %rbx;                             \
        pushq %rbp;                             \
        SETUP_EXCEPTION_FRAME_POINTER;          \
        pushq %r12;                             \
        pushq %r13;                             \
        pushq %r14;                             \
        pushq %r15;

#define RESTORE_ALL                             \
        popq  %r15;                             \
        popq  %r14;                             \
        popq  %r13;                             \
        popq  %r12;                             \
        popq  %rbp;                             \
        popq  %rbx;                             \
        popq  %r11;                             \
        popq  %r10;                             \
        popq  %r9;                              \
        popq  %r8;                              \
        popq  %rax;                             \
        popq  %rcx;                             \
        popq  %rdx;                             \
        popq  %rsi;                             \
        popq  %rdi;

#ifdef PERF_COUNTERS
#define PERFC_INCR(_name,_idx)                  \
    pushq %rdx;                                 \
    leaq perfcounters+_name(%rip),%rdx;         \
    lock incl (%rdx,_idx,4);                    \
    popq %rdx;
#else
#define PERFC_INCR(_name,_idx)
#endif

/* Work around AMD erratum #88 */
#define safe_swapgs                             \
        "mfence; swapgs;"

#define BUILD_SMP_INTERRUPT(x,v) XBUILD_SMP_INTERRUPT(x,v)
#define XBUILD_SMP_INTERRUPT(x,v)               \
asmlinkage void x(void);                        \
__asm__(                                        \
    "\n"__ALIGN_STR"\n"                         \
    ".globl " STR(x) "\n\t"                     \
    STR(x) ":\n\t"                              \
    "pushq $0\n\t"                              \
    "movl $"#v",4(%rsp)\n\t"                    \
    STR(SAVE_ALL)                               \
    "movq %rsp,%rdi\n\t"                        \
    "callq "STR(smp_##x)"\n\t"                  \
    "jmp ret_from_intr\n");

#define BUILD_COMMON_IRQ()                      \
__asm__(                                        \
    "\n" __ALIGN_STR"\n"                        \
    "common_interrupt:\n\t"                     \
    STR(SAVE_ALL)                               \
    "movq %rsp,%rdi\n\t"                        \
    "callq " STR(do_IRQ) "\n\t"                 \
    "jmp ret_from_intr\n");

#define IRQ_NAME2(nr) nr##_interrupt(void)
#define IRQ_NAME(nr) IRQ_NAME2(IRQ##nr)

#define BUILD_IRQ(nr)                           \
asmlinkage void IRQ_NAME(nr);                   \
__asm__(                                        \
"\n"__ALIGN_STR"\n"                             \
STR(IRQ) #nr "_interrupt:\n\t"                  \
    "pushq $0\n\t"                              \
    "movl $"#nr",4(%rsp)\n\t"                   \
    "jmp common_interrupt");

#endif /* __X86_64_ASM_DEFNS_H__ */
