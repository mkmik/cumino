#ifndef _I386_REGS_H
#define _I386_REGS_H

#include <xen/types.h>
#include <public/xen.h>

#define vm86_mode(r) ((r)->eflags & EF_VM)
#define ring_0(r)    (((r)->cs & 3) == 0)
#define ring_1(r)    (((r)->cs & 3) == 1)
#define ring_2(r)    (((r)->cs & 3) == 2)
#define ring_3(r)    (((r)->cs & 3) == 3)

#define guest_kernel_mode(v, r)   \
    (!vm86_mode(r) && ring_1(r))

#define permit_softint(dpl, v, r) \
    ((dpl) >= (vm86_mode(r) ? 3 : ((r)->cs & 3)))

/* Number of bytes of on-stack execution state to be context-switched. */
#define CTXT_SWITCH_STACK_BYTES (sizeof(struct cpu_user_regs))

#endif
