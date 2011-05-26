/*
 * svm.h: SVM Architecture related definitions
 * Copyright (c) 2005, AMD Corporation.
 * Copyright (c) 2004, Intel Corporation.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc., 59 Temple
 * Place - Suite 330, Boston, MA 02111-1307 USA.
 *
 */

#ifndef __ASM_X86_HVM_SVM_H__
#define __ASM_X86_HVM_SVM_H__

#include <xen/sched.h>
#include <asm/types.h>
#include <asm/regs.h>
#include <asm/processor.h>
#include <asm/hvm/svm/vmcb.h>
#include <asm/i387.h>

extern void asidpool_retire(struct vmcb_struct *vmcb, int core);
extern void svm_dump_vmcb(const char *from, struct vmcb_struct *vmcb);
extern void svm_do_launch(struct vcpu *v);
extern void arch_svm_do_resume(struct vcpu *v);

extern u64 root_vmcb_pa[NR_CPUS];

#define SVM_REG_EAX (0) 
#define SVM_REG_ECX (1) 
#define SVM_REG_EDX (2) 
#define SVM_REG_EBX (3) 
#define SVM_REG_ESP (4) 
#define SVM_REG_EBP (5) 
#define SVM_REG_ESI (6) 
#define SVM_REG_EDI (7) 
#define SVM_REG_R8  (8)
#define SVM_REG_R9  (9)
#define SVM_REG_R10 (10)
#define SVM_REG_R11 (11)
#define SVM_REG_R12 (12)
#define SVM_REG_R13 (13)
#define SVM_REG_R14 (14)
#define SVM_REG_R15 (15)

#endif /* __ASM_X86_HVM_SVM_H__ */
