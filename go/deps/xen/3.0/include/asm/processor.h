
/* Portions are: Copyright (c) 1994 Linus Torvalds */

#ifndef __ASM_X86_PROCESSOR_H
#define __ASM_X86_PROCESSOR_H

#ifndef __ASSEMBLY__
#include <xen/config.h>
#include <xen/cache.h>
#include <xen/types.h>
#include <public/xen.h>
#include <asm/types.h>
#include <asm/cpufeature.h>
#include <asm/desc.h>
#endif

/*
 * CPU vendor IDs
 */
#define X86_VENDOR_INTEL 0
#define X86_VENDOR_CYRIX 1
#define X86_VENDOR_AMD 2
#define X86_VENDOR_UMC 3
#define X86_VENDOR_NEXGEN 4
#define X86_VENDOR_CENTAUR 5
#define X86_VENDOR_RISE 6
#define X86_VENDOR_TRANSMETA 7
#define X86_VENDOR_NSC 8
#define X86_VENDOR_NUM 9
#define X86_VENDOR_UNKNOWN 0xff

/*
 * EFLAGS bits
 */
#define X86_EFLAGS_CF	0x00000001 /* Carry Flag */
#define X86_EFLAGS_PF	0x00000004 /* Parity Flag */
#define X86_EFLAGS_AF	0x00000010 /* Auxillary carry Flag */
#define X86_EFLAGS_ZF	0x00000040 /* Zero Flag */
#define X86_EFLAGS_SF	0x00000080 /* Sign Flag */
#define X86_EFLAGS_TF	0x00000100 /* Trap Flag */
#define X86_EFLAGS_IF	0x00000200 /* Interrupt Flag */
#define X86_EFLAGS_DF	0x00000400 /* Direction Flag */
#define X86_EFLAGS_OF	0x00000800 /* Overflow Flag */
#define X86_EFLAGS_IOPL	0x00003000 /* IOPL mask */
#define X86_EFLAGS_NT	0x00004000 /* Nested Task */
#define X86_EFLAGS_RF	0x00010000 /* Resume Flag */
#define X86_EFLAGS_VM	0x00020000 /* Virtual Mode */
#define X86_EFLAGS_AC	0x00040000 /* Alignment Check */
#define X86_EFLAGS_VIF	0x00080000 /* Virtual Interrupt Flag */
#define X86_EFLAGS_VIP	0x00100000 /* Virtual Interrupt Pending */
#define X86_EFLAGS_ID	0x00200000 /* CPUID detection flag */

/*
 * Intel CPU flags in CR0
 */
#define X86_CR0_PE              0x00000001 /* Enable Protected Mode    (RW) */
#define X86_CR0_MP              0x00000002 /* Monitor Coprocessor      (RW) */
#define X86_CR0_EM              0x00000004 /* Require FPU Emulation    (RO) */
#define X86_CR0_TS              0x00000008 /* Task Switched            (RW) */
#define X86_CR0_ET              0x00000010 /* Extension type           (RO) */
#define X86_CR0_NE              0x00000020 /* Numeric Error Reporting  (RW) */
#define X86_CR0_WP              0x00010000 /* Supervisor Write Protect (RW) */
#define X86_CR0_AM              0x00040000 /* Alignment Checking       (RW) */
#define X86_CR0_NW              0x20000000 /* Not Write-Through        (RW) */
#define X86_CR0_CD              0x40000000 /* Cache Disable            (RW) */
#define X86_CR0_PG              0x80000000 /* Paging                   (RW) */

/*
 * Intel CPU features in CR4
 */
#define X86_CR4_VME		0x0001	/* enable vm86 extensions */
#define X86_CR4_PVI		0x0002	/* virtual interrupts flag enable */
#define X86_CR4_TSD		0x0004	/* disable time stamp at ipl 3 */
#define X86_CR4_DE		0x0008	/* enable debugging extensions */
#define X86_CR4_PSE		0x0010	/* enable page size extensions */
#define X86_CR4_PAE		0x0020	/* enable physical address extensions */
#define X86_CR4_MCE		0x0040	/* Machine check enable */
#define X86_CR4_PGE		0x0080	/* enable global pages */
#define X86_CR4_PCE		0x0100	/* enable performance counters at ipl 3 */
#define X86_CR4_OSFXSR		0x0200	/* enable fast FPU save and restore */
#define X86_CR4_OSXMMEXCPT	0x0400	/* enable unmasked SSE exceptions */
#define X86_CR4_VMXE		0x2000  /* enable VMX */

/*
 * Trap/fault mnemonics.
 */
#define TRAP_divide_error      0
#define TRAP_debug             1
#define TRAP_nmi               2
#define TRAP_int3              3
#define TRAP_overflow          4
#define TRAP_bounds            5
#define TRAP_invalid_op        6
#define TRAP_no_device         7
#define TRAP_double_fault      8
#define TRAP_copro_seg         9
#define TRAP_invalid_tss      10
#define TRAP_no_segment       11
#define TRAP_stack_error      12
#define TRAP_gp_fault         13
#define TRAP_page_fault       14
#define TRAP_spurious_int     15
#define TRAP_copro_error      16
#define TRAP_alignment_check  17
#define TRAP_machine_check    18
#define TRAP_simd_error       19
#define TRAP_deferred_nmi     31

/* Set for entry via SYSCALL. Informs return code to use SYSRETQ not IRETQ. */
/* NB. Same as VGCF_IN_SYSCALL. No bits in common with any other TRAP_ defn. */
#define TRAP_syscall         256

/*
 * Non-fatal fault/trap handlers return an error code to the caller. If the
 * code is non-zero, it means that either the exception was not due to a fault
 * (i.e., it was a trap) or that the fault has been fixed up so the instruction
 * replay ought to succeed.
 */
#define EXCRET_not_a_fault 1 /* It was a trap. No instruction replay needed. */
#define EXCRET_fault_fixed 1 /* It was fault that we fixed: try a replay. */

/* 'trap_bounce' flags values */
#define TBF_EXCEPTION          1
#define TBF_EXCEPTION_ERRCODE  2
#define TBF_INTERRUPT          8
#define TBF_FAILSAFE          16

/* 'arch_vcpu' flags values */
#define _TF_kernel_mode        0
#define TF_kernel_mode         (1<<_TF_kernel_mode)

/* #PF error code values. */
#define PFEC_page_present   (1U<<0)
#define PFEC_write_access   (1U<<1)
#define PFEC_user_mode      (1U<<2)
#define PFEC_reserved_bit   (1U<<3)
#define PFEC_insn_fetch     (1U<<4)

#ifndef __ASSEMBLY__

struct domain;
struct vcpu;

/*
 * Default implementation of macro that returns current
 * instruction pointer ("program counter").
 */
#ifdef __x86_64__
#define current_text_addr() ({ void *pc; asm volatile("leaq 1f(%%rip),%0\n1:":"=r"(pc)); pc; })
#else
#define current_text_addr() \
  ({ void *pc; __asm__("movl $1f,%0\n1:":"=g" (pc)); pc; })
#endif

struct cpuinfo_x86 {
    __u8 x86;		/* CPU family */
    __u8 x86_vendor;	/* CPU vendor */
    __u8 x86_model;
    __u8 x86_mask;
    char wp_works_ok;	/* It doesn't on 386's */
    char hlt_works_ok;	/* Problems on some 486Dx4's and old 386's */
    char hard_math;
    char rfu;
    int  cpuid_level;	/* Maximum supported CPUID level, -1=no CPUID */
    unsigned int x86_capability[NCAPINTS];
    char x86_vendor_id[16];
    char x86_model_id[64];
    int  x86_cache_size;  /* in KB - valid for CPUS which support this call  */
    int  x86_cache_alignment;	/* In bytes */
    char fdiv_bug;
    char f00f_bug;
    char coma_bug;
    char pad0;
    int  x86_power;
    unsigned char x86_max_cores; /* cpuid returned max cores value */
    unsigned char booted_cores; /* number of cores as seen by OS */
    unsigned char apicid;
} __cacheline_aligned;

/*
 * capabilities of CPUs
 */

extern struct cpuinfo_x86 boot_cpu_data;

#ifdef CONFIG_SMP
extern struct cpuinfo_x86 cpu_data[];
#define current_cpu_data cpu_data[smp_processor_id()]
#else
#define cpu_data (&boot_cpu_data)
#define current_cpu_data boot_cpu_data
#endif

extern int phys_proc_id[NR_CPUS];
extern int cpu_core_id[NR_CPUS];

extern void identify_cpu(struct cpuinfo_x86 *);
extern void print_cpu_info(struct cpuinfo_x86 *);
extern unsigned int init_intel_cacheinfo(struct cpuinfo_x86 *c);
extern void dodgy_tsc(void);

#ifdef CONFIG_X86_HT
extern void detect_ht(struct cpuinfo_x86 *c);
#else
static always_inline void detect_ht(struct cpuinfo_x86 *c) {}
#endif

/*
 * Generic CPUID function
 * clear %ecx since some cpus (Cyrix MII) do not set or clear %ecx
 * resulting in stale register contents being returned.
 */
#define cpuid(_op,_eax,_ebx,_ecx,_edx)          \
    __asm__("cpuid"                             \
            : "=a" (*(int *)(_eax)),            \
              "=b" (*(int *)(_ebx)),            \
              "=c" (*(int *)(_ecx)),            \
              "=d" (*(int *)(_edx))             \
            : "0" (_op), "2" (0))

/* Some CPUID calls want 'count' to be placed in ecx */
static inline void cpuid_count(
    int op,
    int count,
    unsigned int *eax,
    unsigned int *ebx,
    unsigned int *ecx,
    unsigned int *edx)
{
    __asm__("cpuid"
            : "=a" (*eax), "=b" (*ebx), "=c" (*ecx), "=d" (*edx)
            : "0" (op), "c" (count));
}

/*
 * CPUID functions returning a single datum
 */
static always_inline unsigned int cpuid_eax(unsigned int op)
{
    unsigned int eax;

    __asm__("cpuid"
            : "=a" (eax)
            : "0" (op)
            : "bx", "cx", "dx");
    return eax;
}
static always_inline unsigned int cpuid_ebx(unsigned int op)
{
    unsigned int eax, ebx;

    __asm__("cpuid"
            : "=a" (eax), "=b" (ebx)
            : "0" (op)
            : "cx", "dx" );
    return ebx;
}
static always_inline unsigned int cpuid_ecx(unsigned int op)
{
    unsigned int eax, ecx;

    __asm__("cpuid"
            : "=a" (eax), "=c" (ecx)
            : "0" (op)
            : "bx", "dx" );
    return ecx;
}
static always_inline unsigned int cpuid_edx(unsigned int op)
{
    unsigned int eax, edx;

    __asm__("cpuid"
            : "=a" (eax), "=d" (edx)
            : "0" (op)
            : "bx", "cx");
    return edx;
}



static inline unsigned long read_cr0(void)
{
    unsigned long __cr0;
    __asm__("mov %%cr0,%0\n\t" :"=r" (__cr0));
    return __cr0;
} 

static inline void write_cr0(unsigned long val)
{
	__asm__("mov %0,%%cr0": :"r" ((unsigned long)val));
}

static inline unsigned long read_cr2(void)
{
    unsigned long __cr2;
    __asm__("mov %%cr2,%0\n\t" :"=r" (__cr2));
    return __cr2;
}

static inline unsigned long read_cr4(void)
{
    unsigned long __cr4;
    __asm__("mov %%cr4,%0\n\t" :"=r" (__cr4));
    return __cr4;
} 
    
static inline void write_cr4(unsigned long val)
{
	__asm__("mov %0,%%cr4": :"r" ((unsigned long)val));
}


/* Clear and set 'TS' bit respectively */
static inline void clts(void) 
{
    __asm__ __volatile__ ("clts");
}

static inline void stts(void) 
{
    write_cr0(X86_CR0_TS|read_cr0());
}


/*
 * Save the cr4 feature set we're using (ie
 * Pentium 4MB enable and PPro Global page
 * enable), so that any CPU's that boot up
 * after us can get the correct flags.
 */
extern unsigned long mmu_cr4_features;

static always_inline void set_in_cr4 (unsigned long mask)
{
    unsigned long dummy;
    mmu_cr4_features |= mask;
    __asm__ __volatile__ (
        "mov %%cr4,%0\n\t"
        "or %1,%0\n\t"
        "mov %0,%%cr4\n"
        : "=&r" (dummy) : "irg" (mask) );
}

static always_inline void clear_in_cr4 (unsigned long mask)
{
    unsigned long dummy;
    mmu_cr4_features &= ~mask;
    __asm__ __volatile__ (
        "mov %%cr4,%0\n\t"
        "and %1,%0\n\t"
        "mov %0,%%cr4\n"
        : "=&r" (dummy) : "irg" (~mask) );
}

/*
 *      NSC/Cyrix CPU configuration register indexes
 */

#define CX86_PCR0 0x20
#define CX86_GCR  0xb8
#define CX86_CCR0 0xc0
#define CX86_CCR1 0xc1
#define CX86_CCR2 0xc2
#define CX86_CCR3 0xc3
#define CX86_CCR4 0xe8
#define CX86_CCR5 0xe9
#define CX86_CCR6 0xea
#define CX86_CCR7 0xeb
#define CX86_PCR1 0xf0
#define CX86_DIR0 0xfe
#define CX86_DIR1 0xff
#define CX86_ARR_BASE 0xc4
#define CX86_RCR_BASE 0xdc

/*
 *      NSC/Cyrix CPU indexed register access macros
 */

#define getCx86(reg) ({ outb((reg), 0x22); inb(0x23); })

#define setCx86(reg, data) do { \
	outb((reg), 0x22); \
	outb((data), 0x23); \
} while (0)

/* Stop speculative execution */
static inline void sync_core(void)
{
    int tmp;
    asm volatile("cpuid" : "=a" (tmp) : "0" (1) : "ebx","ecx","edx","memory");
}

static always_inline void __monitor(const void *eax, unsigned long ecx,
		unsigned long edx)
{
	/* "monitor %eax,%ecx,%edx;" */
	asm volatile(
		".byte 0x0f,0x01,0xc8;"
		: :"a" (eax), "c" (ecx), "d"(edx));
}

static always_inline void __mwait(unsigned long eax, unsigned long ecx)
{
	/* "mwait %eax,%ecx;" */
	asm volatile(
		".byte 0x0f,0x01,0xc9;"
		: :"a" (eax), "c" (ecx));
}

#define IOBMP_BYTES             8192
#define IOBMP_INVALID_OFFSET    0x8000

struct tss_struct {
    unsigned short	back_link,__blh;
#ifdef __x86_64__
    u64 rsp0;
    u64 rsp1;
    u64 rsp2;
    u64 reserved1;
    u64 ist[7];
    u64 reserved2;
    u16 reserved3;
#else
    u32 esp0;
    u16 ss0,__ss0h;
    u32 esp1;
    u16 ss1,__ss1h;
    u32 esp2;
    u16 ss2,__ss2h;
    u32 __cr3;
    u32 eip;
    u32 eflags;
    u32 eax,ecx,edx,ebx;
    u32 esp;
    u32 ebp;
    u32 esi;
    u32 edi;
    u16 es, __esh;
    u16 cs, __csh;
    u16 ss, __ssh;
    u16 ds, __dsh;
    u16 fs, __fsh;
    u16 gs, __gsh;
    u16 ldt, __ldth;
    u16 trace;
#endif
    u16 bitmap;
    /* Pads the TSS to be cacheline-aligned (total size is 0x80). */
    u8 __cacheline_filler[24];
} __cacheline_aligned __attribute__((packed));

#define IDT_ENTRIES 256
extern idt_entry_t idt_table[];
extern idt_entry_t *idt_tables[];

extern struct tss_struct init_tss[NR_CPUS];

#ifdef CONFIG_X86_32

extern void init_int80_direct_trap(struct vcpu *v);
#define set_int80_direct_trap(_ed)                  \
    (memcpy(idt_tables[(_ed)->processor] + 0x80,    \
            &((_ed)->arch.int80_desc), 8))

#else

#define init_int80_direct_trap(_ed) ((void)0)
#define set_int80_direct_trap(_ed)  ((void)0)

#endif

extern int gpf_emulate_4gb(struct cpu_user_regs *regs);

extern void write_ptbase(struct vcpu *v);

void destroy_gdt(struct vcpu *d);
long set_gdt(struct vcpu *d, 
             unsigned long *frames, 
             unsigned int entries);

long set_debugreg(struct vcpu *p, int reg, unsigned long value);

struct microcode_header {
    unsigned int hdrver;
    unsigned int rev;
    unsigned int date;
    unsigned int sig;
    unsigned int cksum;
    unsigned int ldrver;
    unsigned int pf;
    unsigned int datasize;
    unsigned int totalsize;
    unsigned int reserved[3];
};

struct microcode {
    struct microcode_header hdr;
    unsigned int bits[0];
};

typedef struct microcode microcode_t;
typedef struct microcode_header microcode_header_t;

/* microcode format is extended from prescott processors */
struct extended_signature {
    unsigned int sig;
    unsigned int pf;
    unsigned int cksum;
};

struct extended_sigtable {
    unsigned int count;
    unsigned int cksum;
    unsigned int reserved[3];
    struct extended_signature sigs[0];
};

/* REP NOP (PAUSE) is a good thing to insert into busy-wait loops. */
static always_inline void rep_nop(void)
{
    __asm__ __volatile__ ( "rep;nop" : : : "memory" );
}

#define cpu_relax() rep_nop()

/* Prefetch instructions for Pentium III and AMD Athlon */
#ifdef 	CONFIG_MPENTIUMIII

#define ARCH_HAS_PREFETCH
extern always_inline void prefetch(const void *x)
{
    __asm__ __volatile__ ("prefetchnta (%0)" : : "r"(x));
}

#elif CONFIG_X86_USE_3DNOW

#define ARCH_HAS_PREFETCH
#define ARCH_HAS_PREFETCHW
#define ARCH_HAS_SPINLOCK_PREFETCH

extern always_inline void prefetch(const void *x)
{
    __asm__ __volatile__ ("prefetch (%0)" : : "r"(x));
}

extern always_inline void prefetchw(const void *x)
{
    __asm__ __volatile__ ("prefetchw (%0)" : : "r"(x));
}
#define spin_lock_prefetch(x)	prefetchw(x)

#endif

void show_stack(struct cpu_user_regs *regs);
void show_xen_trace(void);
void show_stack_overflow(unsigned long esp);
void show_registers(struct cpu_user_regs *regs);
void show_execution_state(struct cpu_user_regs *regs);
void show_page_walk(unsigned long addr);
asmlinkage void fatal_trap(int trapnr, struct cpu_user_regs *regs);

/* Dumps current register and stack state. */
#define dump_execution_state()                                              \
    /* NB. Needs interrupts enabled else we end up in fatal_trap(). */      \
    __asm__ __volatile__ ( "pushf ; sti ; ud2 ; .ascii \"dbg\" ; popf" )

extern void mtrr_ap_init(void);
extern void mtrr_bp_init(void);

extern void mcheck_init(struct cpuinfo_x86 *c);

int cpuid_hypervisor_leaves(
    uint32_t idx, uint32_t *eax, uint32_t *ebx, uint32_t *ecx, uint32_t *edx);
int rdmsr_hypervisor_regs(
    uint32_t idx, uint32_t *eax, uint32_t *edx);
int wrmsr_hypervisor_regs(
    uint32_t idx, uint32_t eax, uint32_t edx);

#endif /* !__ASSEMBLY__ */

#endif /* __ASM_X86_PROCESSOR_H */

/*
 * Local variables:
 * mode: C
 * c-set-style: "BSD"
 * c-basic-offset: 4
 * tab-width: 4
 * indent-tabs-mode: nil
 * End:
 */
