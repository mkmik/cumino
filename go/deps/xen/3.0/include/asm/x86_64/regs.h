#ifndef _X86_64_REGS_H
#define _X86_64_REGS_H

#include <xen/types.h>
#include <public/xen.h>

#define vm86_mode(r) (0) /* No VM86 support in long mode. */
#define ring_0(r)    (((r)->cs & 3) == 0)
#define ring_1(r)    (((r)->cs & 3) == 1)
#define ring_2(r)    (((r)->cs & 3) == 2)
#define ring_3(r)    (((r)->cs & 3) == 3)

#define guest_kernel_mode(v, r)   \
    (ring_3(r) && ((v)->arch.flags & TF_kernel_mode))

#define permit_softint(dpl, v, r) \
    ((dpl) >= (guest_kernel_mode(v, r) ? 1 : 3))

/* Number of bytes of on-stack execution state to be context-switched. */
/* NB. Segment registers and bases are not saved/restored on x86/64 stack. */
#define CTXT_SWITCH_STACK_BYTES (offsetof(struct cpu_user_regs, es))

#endif
