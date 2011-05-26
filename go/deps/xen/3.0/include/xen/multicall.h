/******************************************************************************
 * multicall.h
 */

#ifndef __XEN_MULTICALL_H__
#define __XEN_MULTICALL_H__

#include <xen/percpu.h>
#include <asm/multicall.h>

#define _MCSF_in_multicall   0
#define _MCSF_call_preempted 1
#define MCSF_in_multicall    (1<<_MCSF_in_multicall)
#define MCSF_call_preempted  (1<<_MCSF_call_preempted)
struct mc_state {
    unsigned long flags;
    struct multicall_entry call;
};

DECLARE_PER_CPU(struct mc_state, mc_state);

#endif /* __XEN_MULTICALL_H__ */
