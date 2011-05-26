/******************************************************************************
 * xenctrl.h
 *
 * A library for low-level access to the Xen control interfaces.
 *
 * Copyright (c) 2003-2004, K A Fraser.
 */

#ifndef XENCTRL_H
#define XENCTRL_H

/* Tell the Xen public headers we are a user-space tools build. */
#ifndef __XEN_TOOLS__
#define __XEN_TOOLS__ 1
#endif

#include <stddef.h>
#include <stdint.h>
#include <sys/ptrace.h>
#include <xen/xen.h>
#include <xen/domctl.h>
#include <xen/sysctl.h>
#include <xen/version.h>
#include <xen/event_channel.h>
#include <xen/sched.h>
#include <xen/memory.h>
#include <xen/acm.h>
#include <xen/acm_ops.h>

#ifdef __ia64__
#define XC_PAGE_SHIFT           14
#else
#define XC_PAGE_SHIFT           12
#endif
#define XC_PAGE_SIZE            (1UL << XC_PAGE_SHIFT)
#define XC_PAGE_MASK            (~(XC_PAGE_SIZE-1))

/*
 *  DEFINITIONS FOR CPU BARRIERS
 */

#if defined(__i386__)
#define mb()  __asm__ __volatile__ ( "lock; addl $0,0(%%esp)" : : : "memory" )
#define rmb() __asm__ __volatile__ ( "lock; addl $0,0(%%esp)" : : : "memory" )
#define wmb() __asm__ __volatile__ ( "" : : : "memory")
#elif defined(__x86_64__)
#define mb()  __asm__ __volatile__ ( "mfence" : : : "memory")
#define rmb() __asm__ __volatile__ ( "lfence" : : : "memory")
#define wmb() __asm__ __volatile__ ( "" : : : "memory")
#elif defined(__ia64__)
/* FIXME */
#define mb()
#define rmb()
#define wmb()
#elif defined(__powerpc__)
/* XXX loosen these up later */
#define mb()   __asm__ __volatile__ ("sync" : : : "memory")
#define rmb()  __asm__ __volatile__ ("sync" : : : "memory") /* lwsync? */
#define wmb()  __asm__ __volatile__ ("sync" : : : "memory") /* eieio? */
#else
#error "Define barriers"
#endif

/*
 *  INITIALIZATION FUNCTIONS
 */

/**
 * This function opens a handle to the hypervisor interface.  This function can
 * be called multiple times within a single process.  Multiple processes can
 * have an open hypervisor interface at the same time.
 *
 * Each call to this function should have a corresponding call to
 * xc_interface_close().
 *
 * This function can fail if the caller does not have superuser permission or
 * if a Xen-enabled kernel is not currently running.
 *
 * @return a handle to the hypervisor interface or -1 on failure
 */
int xc_interface_open(void);

/**
 * This function closes an open hypervisor interface.
 *
 * This function can fail if the handle does not represent an open interface or
 * if there were problems closing the interface.
 *
 * @parm xc_handle a handle to an open hypervisor interface
 * @return 0 on success, -1 otherwise.
 */
int xc_interface_close(int xc_handle);

/*
 * KERNEL INTERFACES
 */

/*
 * Resolve a kernel device name (e.g., "evtchn", "blktap0") into a kernel
 * device number. Returns -1 on error (and sets errno).
 */
int xc_find_device_number(const char *name);

/*
 * DOMAIN DEBUGGING FUNCTIONS
 */

typedef struct xc_core_header {
    unsigned int xch_magic;
    unsigned int xch_nr_vcpus;
    unsigned int xch_nr_pages;
    unsigned int xch_ctxt_offset;
    unsigned int xch_index_offset;
    unsigned int xch_pages_offset;
} xc_core_header_t;

#define XC_CORE_MAGIC 0xF00FEBED

long xc_ptrace_core(
    int xc_handle,
    enum __ptrace_request request,
    uint32_t domid,
    long addr,
    long data,
    vcpu_guest_context_t *ctxt);
void * map_domain_va_core(
    unsigned long domfd,
    int cpu,
    void *guest_va,
    vcpu_guest_context_t *ctxt);
int xc_waitdomain_core(
    int xc_handle,
    int domain,
    int *status,
    int options,
    vcpu_guest_context_t *ctxt);

/*
 * DOMAIN MANAGEMENT FUNCTIONS
 */

typedef struct {
    uint32_t      domid;
    uint32_t      ssidref;
    unsigned int  dying:1, crashed:1, shutdown:1,
                  paused:1, blocked:1, running:1;
    unsigned int  shutdown_reason; /* only meaningful if shutdown==1 */
    unsigned long nr_pages;
    unsigned long shared_info_frame;
    uint64_t      cpu_time;
    unsigned long max_memkb;
    unsigned int  nr_online_vcpus;
    unsigned int  max_vcpu_id;
    xen_domain_handle_t handle;
} xc_dominfo_t;

typedef xen_domctl_getdomaininfo_t xc_domaininfo_t;
int xc_domain_create(int xc_handle,
                     uint32_t ssidref,
                     xen_domain_handle_t handle,
                     uint32_t *pdomid);


/* Functions to produce a dump of a given domain
 *  xc_domain_dumpcore - produces a dump to a specified file
 *  xc_domain_dumpcore_via_callback - produces a dump, using a specified
 *                                    callback function
 */
int xc_domain_dumpcore(int xc_handle,
                       uint32_t domid,
                       const char *corename);

/* Define the callback function type for xc_domain_dumpcore_via_callback.
 *
 * This function is called by the coredump code for every "write",
 * and passes an opaque object for the use of the function and
 * created by the caller of xc_domain_dumpcore_via_callback.
 */
typedef int (dumpcore_rtn_t)(void *arg, char *buffer, unsigned int length);

int xc_domain_dumpcore_via_callback(int xc_handle,
                                    uint32_t domid,
                                    void *arg,
                                    dumpcore_rtn_t dump_rtn);

/*
 * This function sets the maximum number of vcpus that a domain may create.
 *
 * @parm xc_handle a handle to an open hypervisor interface.
 * @parm domid the domain id in which vcpus are to be created.
 * @parm max the maximum number of vcpus that the domain may create.
 * @return 0 on success, -1 on failure.
 */
int xc_domain_max_vcpus(int xc_handle,
                        uint32_t domid,
                        unsigned int max);

/**
 * This function pauses a domain. A paused domain still exists in memory
 * however it does not receive any timeslices from the hypervisor.
 *
 * @parm xc_handle a handle to an open hypervisor interface
 * @parm domid the domain id to pause
 * @return 0 on success, -1 on failure.
 */
int xc_domain_pause(int xc_handle,
                    uint32_t domid);
/**
 * This function unpauses a domain.  The domain should have been previously
 * paused.
 *
 * @parm xc_handle a handle to an open hypervisor interface
 * @parm domid the domain id to unpause
 * return 0 on success, -1 on failure
 */
int xc_domain_unpause(int xc_handle,
                      uint32_t domid);

/**
 * This function will destroy a domain.  Destroying a domain removes the domain
 * completely from memory.  This function should be called after sending the
 * domain a SHUTDOWN control message to free up the domain resources.
 *
 * @parm xc_handle a handle to an open hypervisor interface
 * @parm domid the domain id to destroy
 * @return 0 on success, -1 on failure
 */
int xc_domain_destroy(int xc_handle,
                      uint32_t domid);

/**
 * This function will shutdown a domain. This is intended for use in
 * fully-virtualized domains where this operation is analogous to the
 * sched_op operations in a paravirtualized domain. The caller is
 * expected to give the reason for the shutdown.
 *
 * @parm xc_handle a handle to an open hypervisor interface
 * @parm domid the domain id to destroy
 * @parm reason is the reason (SHUTDOWN_xxx) for the shutdown
 * @return 0 on success, -1 on failure
 */
int xc_domain_shutdown(int xc_handle,
                       uint32_t domid,
                       int reason);

int xc_vcpu_setaffinity(int xc_handle,
                        uint32_t domid,
                        int vcpu,
                        uint64_t cpumap);
int xc_vcpu_getaffinity(int xc_handle,
                        uint32_t domid,
                        int vcpu,
                        uint64_t *cpumap);

/**
 * This function will return information about one or more domains. It is
 * designed to iterate over the list of domains. If a single domain is
 * requested, this function will return the next domain in the list - if
 * one exists. It is, therefore, important in this case to make sure the
 * domain requested was the one returned.
 *
 * @parm xc_handle a handle to an open hypervisor interface
 * @parm first_domid the first domain to enumerate information from.  Domains
 *                   are currently enumerate in order of creation.
 * @parm max_doms the number of elements in info
 * @parm info an array of max_doms size that will contain the information for
 *            the enumerated domains.
 * @return the number of domains enumerated or -1 on error
 */
int xc_domain_getinfo(int xc_handle,
                      uint32_t first_domid,
                      unsigned int max_doms,
                      xc_dominfo_t *info);


/**
 * This function will set the execution context for the specified vcpu.
 *
 * @parm xc_handle a handle to an open hypervisor interface
 * @parm domid the domain to set the vcpu context for
 * @parm vcpu the vcpu number for the context
 * @parm ctxt pointer to the the cpu context with the values to set
 * @return the number of domains enumerated or -1 on error
 */
int xc_vcpu_setcontext(int xc_handle,
                       uint32_t domid,
                       uint32_t vcpu,
                       vcpu_guest_context_t *ctxt);
/**
 * This function will return information about one or more domains, using a
 * single hypercall.  The domain information will be stored into the supplied
 * array of xc_domaininfo_t structures.
 *
 * @parm xc_handle a handle to an open hypervisor interface
 * @parm first_domain the first domain to enumerate information from.
 *                    Domains are currently enumerate in order of creation.
 * @parm max_domains the number of elements in info
 * @parm info an array of max_doms size that will contain the information for
 *            the enumerated domains.
 * @return the number of domains enumerated or -1 on error
 */
int xc_domain_getinfolist(int xc_handle,
                          uint32_t first_domain,
                          unsigned int max_domains,
                          xc_domaininfo_t *info);

/**
 * This function returns information about the execution context of a
 * particular vcpu of a domain.
 *
 * @parm xc_handle a handle to an open hypervisor interface
 * @parm domid the domain to get information from
 * @parm vcpu the vcpu number
 * @parm ctxt a pointer to a structure to store the execution context of the
 *            domain
 * @return 0 on success, -1 on failure
 */
int xc_vcpu_getcontext(int xc_handle,
                               uint32_t domid,
                               uint32_t vcpu,
                               vcpu_guest_context_t *ctxt);

typedef xen_domctl_getvcpuinfo_t xc_vcpuinfo_t;
int xc_vcpu_getinfo(int xc_handle,
                    uint32_t domid,
                    uint32_t vcpu,
                    xc_vcpuinfo_t *info);

int xc_domain_setcpuweight(int xc_handle,
                           uint32_t domid,
                           float weight);
long long xc_domain_get_cpu_usage(int xc_handle,
                                  domid_t domid,
                                  int vcpu);

int xc_domain_sethandle(int xc_handle, uint32_t domid,
                        xen_domain_handle_t handle);

typedef xen_domctl_shadow_op_stats_t xc_shadow_op_stats_t;
int xc_shadow_control(int xc_handle,
                      uint32_t domid,
                      unsigned int sop,
                      unsigned long *dirty_bitmap,
                      unsigned long pages,
                      unsigned long *mb,
                      uint32_t mode,
                      xc_shadow_op_stats_t *stats);

int xc_sedf_domain_set(int xc_handle,
                       uint32_t domid,
                       uint64_t period, uint64_t slice,
                       uint64_t latency, uint16_t extratime,
                       uint16_t weight);

int xc_sedf_domain_get(int xc_handle,
                       uint32_t domid,
                       uint64_t* period, uint64_t *slice,
                       uint64_t *latency, uint16_t *extratime,
                       uint16_t *weight);

int xc_sched_credit_domain_set(int xc_handle,
                               uint32_t domid,
                               struct xen_domctl_sched_credit *sdom);

int xc_sched_credit_domain_get(int xc_handle,
                               uint32_t domid,
                               struct xen_domctl_sched_credit *sdom);

/*
 * EVENT CHANNEL FUNCTIONS
 */

/**
 * This function allocates an unbound port.  Ports are named endpoints used for
 * interdomain communication.  This function is most useful in opening a
 * well-known port within a domain to receive events on.
 *
 * @parm xc_handle a handle to an open hypervisor interface
 * @parm dom the ID of the local domain (the 'allocatee')
 * @parm remote_dom the ID of the domain who will later bind
 * @return allocated port (in @dom) on success, -1 on failure
 */
int xc_evtchn_alloc_unbound(int xc_handle,
                            uint32_t dom,
                            uint32_t remote_dom);

int xc_physdev_pci_access_modify(int xc_handle,
                                 uint32_t domid,
                                 int bus,
                                 int dev,
                                 int func,
                                 int enable);

int xc_readconsolering(int xc_handle,
                       char **pbuffer,
                       unsigned int *pnr_chars,
                       int clear);

typedef xen_sysctl_physinfo_t xc_physinfo_t;
int xc_physinfo(int xc_handle,
                xc_physinfo_t *info);

int xc_sched_id(int xc_handle,
                int *sched_id);

int xc_domain_setmaxmem(int xc_handle,
                        uint32_t domid,
                        unsigned int max_memkb);

int xc_domain_set_time_offset(int xc_handle,
                              uint32_t domid,
                              int32_t time_offset_seconds);

int xc_domain_memory_increase_reservation(int xc_handle,
                                          uint32_t domid,
                                          unsigned long nr_extents,
                                          unsigned int extent_order,
                                          unsigned int address_bits,
                                          xen_pfn_t *extent_start);

int xc_domain_memory_decrease_reservation(int xc_handle,
                                          uint32_t domid,
                                          unsigned long nr_extents,
                                          unsigned int extent_order,
                                          xen_pfn_t *extent_start);

int xc_domain_memory_populate_physmap(int xc_handle,
                                      uint32_t domid,
                                      unsigned long nr_extents,
                                      unsigned int extent_order,
                                      unsigned int address_bits,
                                      xen_pfn_t *extent_start);

int xc_domain_translate_gpfn_list(int xc_handle,
                                  uint32_t domid,
                                  unsigned long nr_gpfns,
                                  xen_pfn_t *gpfn_list,
                                  xen_pfn_t *mfn_list);

int xc_domain_ioport_permission(int xc_handle,
                                uint32_t domid,
                                uint32_t first_port,
                                uint32_t nr_ports,
                                uint32_t allow_access);

int xc_domain_irq_permission(int xc_handle,
                             uint32_t domid,
                             uint8_t pirq,
                             uint8_t allow_access);

int xc_domain_iomem_permission(int xc_handle,
                               uint32_t domid,
                               unsigned long first_mfn,
                               unsigned long nr_mfns,
                               uint8_t allow_access);

unsigned long xc_make_page_below_4G(int xc_handle, uint32_t domid,
                                    unsigned long mfn);

typedef xen_sysctl_perfc_desc_t xc_perfc_desc_t;
typedef xen_sysctl_perfc_val_t xc_perfc_val_t;
/* IMPORTANT: The caller is responsible for mlock()'ing the @desc and @val
   arrays. */
int xc_perfc_control(int xc_handle,
                     uint32_t op,
                     xc_perfc_desc_t *desc,
                     xc_perfc_val_t *val,
                     int *nbr_desc,
                     int *nbr_val);

/**
 * Memory maps a range within one domain to a local address range.  Mappings
 * should be unmapped with munmap and should follow the same rules as mmap
 * regarding page alignment.  Returns NULL on failure.
 *
 * In Linux, the ring queue for the control channel is accessible by mapping
 * the shared_info_frame (from xc_domain_getinfo()) + 2048.  The structure
 * stored there is of type control_if_t.
 *
 * @parm xc_handle a handle on an open hypervisor interface
 * @parm dom the domain to map memory from
 * @parm size the amount of memory to map (in multiples of page size)
 * @parm prot same flag as in mmap().
 * @parm mfn the frame address to map.
 */
void *xc_map_foreign_range(int xc_handle, uint32_t dom,
                            int size, int prot,
                            unsigned long mfn );

void *xc_map_foreign_batch(int xc_handle, uint32_t dom, int prot,
                           xen_pfn_t *arr, int num );

/**
 * Translates a virtual address in the context of a given domain and
 * vcpu returning the machine page frame number of the associated
 * page.
 *
 * @parm xc_handle a handle on an open hypervisor interface
 * @parm dom the domain to perform the translation in
 * @parm vcpu the vcpu to perform the translation on
 * @parm virt the virtual address to translate
 */
unsigned long xc_translate_foreign_address(int xc_handle, uint32_t dom,
                                           int vcpu, unsigned long long virt);

int xc_get_pfn_list(int xc_handle, uint32_t domid, xen_pfn_t *pfn_buf,
                    unsigned long max_pfns);

int xc_ia64_get_pfn_list(int xc_handle, uint32_t domid,
                         xen_pfn_t *pfn_buf,
                         unsigned int start_page, unsigned int nr_pages);

int xc_copy_to_domain_page(int xc_handle, uint32_t domid,
                           unsigned long dst_pfn, const char *src_page);

int xc_clear_domain_page(int xc_handle, uint32_t domid,
                         unsigned long dst_pfn);

long xc_get_max_pages(int xc_handle, uint32_t domid);

int xc_mmuext_op(int xc_handle, struct mmuext_op *op, unsigned int nr_ops,
                 domid_t dom);

int xc_memory_op(int xc_handle, int cmd, void *arg);

int xc_get_pfn_type_batch(int xc_handle, uint32_t dom,
                          int num, unsigned long *arr);


/* Get current total pages allocated to a domain. */
long xc_get_tot_pages(int xc_handle, uint32_t domid);


/*
 * Trace Buffer Operations
 */

/**
 * xc_tbuf_enable - enable tracing buffers
 *
 * @parm xc_handle a handle to an open hypervisor interface
 * @parm cnt size of tracing buffers to create (in pages)
 * @parm mfn location to store mfn of the trace buffers to
 * @parm size location to store the size (in bytes) of a trace buffer to
 *
 * Gets the machine address of the trace pointer area and the size of the
 * per CPU buffers.
 */
int xc_tbuf_enable(int xc_handle, size_t cnt, unsigned long *mfn,
    unsigned long *size);

/*
 * Disable tracing buffers.
 */
int xc_tbuf_disable(int xc_handle);

/**
 * This function sets the size of the trace buffers. Setting the size
 * is currently a one-shot operation that may be performed either at boot
 * time or via this interface, not both. The buffer size must be set before
 * enabling tracing.
 *
 * @parm xc_handle a handle to an open hypervisor interface
 * @parm size the size in pages per cpu for the trace buffers
 * @return 0 on success, -1 on failure.
 */
int xc_tbuf_set_size(int xc_handle, unsigned long size);

/**
 * This function retrieves the current size of the trace buffers.
 * Note that the size returned is in terms of bytes, not pages.

 * @parm xc_handle a handle to an open hypervisor interface
 * @parm size will contain the size in bytes for the trace buffers
 * @return 0 on success, -1 on failure.
 */
int xc_tbuf_get_size(int xc_handle, unsigned long *size);

int xc_tbuf_set_cpu_mask(int xc_handle, uint32_t mask);

int xc_tbuf_set_evt_mask(int xc_handle, uint32_t mask);

int xc_domctl(int xc_handle, struct xen_domctl *domctl);
int xc_sysctl(int xc_handle, struct xen_sysctl *sysctl);

int xc_version(int xc_handle, int cmd, void *arg);

/*
 * MMU updates.
 */
#define MAX_MMU_UPDATES 1024
struct xc_mmu {
    mmu_update_t updates[MAX_MMU_UPDATES];
    int          idx;
    domid_t      subject;
};
typedef struct xc_mmu xc_mmu_t;
xc_mmu_t *xc_init_mmu_updates(int xc_handle, domid_t dom);
int xc_add_mmu_update(int xc_handle, xc_mmu_t *mmu,
                   unsigned long long ptr, unsigned long long val);
int xc_finish_mmu_updates(int xc_handle, xc_mmu_t *mmu);

int xc_acm_op(int xc_handle, int cmd, void *arg, size_t arg_size);

/*
 * Return a handle to the event channel driver, or -1 on failure, in which case
 * errno will be set appropriately.
 */
int xc_evtchn_open(void);

/*
 * Close a handle previously allocated with xc_evtchn_open().
 */
int xc_evtchn_close(int xce_handle);

/*
 * Return an fd that can be select()ed on for further calls to
 * xc_evtchn_pending().
 */
int xc_evtchn_fd(int xce_handle);

/*
 * Notify the given event channel. Returns -1 on failure, in which case
 * errno will be set appropriately.
 */
int xc_evtchn_notify(int xce_handle, evtchn_port_t port);

/*
 * Returns a new event port bound to the remote port for the given domain ID,
 * or -1 on failure, in which case errno will be set appropriately.
 */
evtchn_port_t xc_evtchn_bind_interdomain(int xce_handle, int domid,
    evtchn_port_t remote_port);

/*
 * Unbind the given event channel. Returns -1 on failure, in which case errno
 * will be set appropriately.
 */
int xc_evtchn_unbind(int xce_handle, evtchn_port_t port);

/*
 * Bind an event channel to the given VIRQ. Returns the event channel bound to
 * the VIRQ, or -1 on failure, in which case errno will be set appropriately.
 */
evtchn_port_t xc_evtchn_bind_virq(int xce_handle, unsigned int virq);

/*
 * Return the next event channel to become pending, or -1 on failure, in which
 * case errno will be set appropriately.  
 */
evtchn_port_t xc_evtchn_pending(int xce_handle);

/*
 * Unmask the given event channel. Returns -1 on failure, in which case errno
 * will be set appropriately.
 */
int xc_evtchn_unmask(int xce_handle, evtchn_port_t port);

#endif
