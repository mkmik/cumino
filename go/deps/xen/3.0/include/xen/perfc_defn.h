/* This file is legitimately included multiple times. */
/*#ifndef __XEN_PERFC_DEFN_H__*/
/*#define __XEN_PERFC_DEFN_H__*/

#include <asm/perfc_defn.h>

PERFCOUNTER_ARRAY(hypercalls,           "hypercalls", NR_hypercalls)

PERFCOUNTER_CPU(irqs,                   "#interrupts")
PERFCOUNTER_CPU(ipis,                   "#IPIs")

PERFCOUNTER_CPU(sched_irq,              "sched: timer")
PERFCOUNTER_CPU(sched_run,              "sched: runs through scheduler")
PERFCOUNTER_CPU(sched_ctx,              "sched: context switches")

PERFCOUNTER_CPU(need_flush_tlb_flush,   "PG_need_flush tlb flushes")

/*#endif*/ /* __XEN_PERFC_DEFN_H__ */
