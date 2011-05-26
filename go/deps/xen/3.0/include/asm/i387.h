/*
 * include/asm-i386/i387.h
 *
 * Copyright (C) 1994 Linus Torvalds
 *
 * Pentium III FXSR, SSE support
 * General FPU state handling cleanups
 *	Gareth Hughes <gareth@valinux.com>, May 2000
 */

#ifndef __ASM_I386_I387_H
#define __ASM_I386_I387_H

#include <xen/sched.h>
#include <asm/processor.h>

extern void init_fpu(void);
extern void save_init_fpu(struct vcpu *v);
extern void restore_fpu(struct vcpu *v);

#define unlazy_fpu(v) do {                                      \
    if ( test_bit(_VCPUF_fpu_dirtied, &(v)->vcpu_flags) )       \
        save_init_fpu(v);                                       \
} while ( 0 )

#define load_mxcsr(val) do {                                    \
    unsigned long __mxcsr = ((unsigned long)(val) & 0xffbf);    \
    __asm__ __volatile__ ( "ldmxcsr %0" : : "m" (__mxcsr) );    \
} while ( 0 )

static inline void setup_fpu(struct vcpu *v)
{
    /* Avoid recursion. */
    clts();

    if ( !test_and_set_bit(_VCPUF_fpu_dirtied, &v->vcpu_flags) )
    {
        if ( test_bit(_VCPUF_fpu_initialised, &v->vcpu_flags) )
            restore_fpu(v);
        else
            init_fpu();
    }
}

#endif /* __ASM_I386_I387_H */
