/******************************************************************************
 * xenoprof.h
 * 
 * Interface for enabling system wide profiling based on hardware performance
 * counters
 * 
 * Copyright (C) 2005 Hewlett-Packard Co.
 * Written by Aravind Menon & Jose Renato Santos
 */

#ifndef __XEN_PUBLIC_XENOPROF_H__
#define __XEN_PUBLIC_XENOPROF_H__

/*
 * Commands to HYPERVISOR_xenoprof_op().
 */
#define XENOPROF_init                0
#define XENOPROF_reset_active_list   1
#define XENOPROF_reset_passive_list  2
#define XENOPROF_set_active          3
#define XENOPROF_set_passive         4
#define XENOPROF_reserve_counters    5
#define XENOPROF_counter             6
#define XENOPROF_setup_events        7
#define XENOPROF_enable_virq         8
#define XENOPROF_start               9
#define XENOPROF_stop               10
#define XENOPROF_disable_virq       11
#define XENOPROF_release_counters   12
#define XENOPROF_shutdown           13
#define XENOPROF_get_buffer         14
#define XENOPROF_last_op            14

#define MAX_OPROF_EVENTS    32
#define MAX_OPROF_DOMAINS   25	
#define XENOPROF_CPU_TYPE_SIZE 64

/* Xenoprof performance events (not Xen events) */
struct event_log {
    uint64_t eip;
    uint8_t mode;
    uint8_t event;
};

/* Xenoprof buffer shared between Xen and domain - 1 per VCPU */
struct xenoprof_buf {
    uint32_t event_head;
    uint32_t event_tail;
    uint32_t event_size;
    uint32_t vcpu_id;
    uint64_t xen_samples;
    uint64_t kernel_samples;
    uint64_t user_samples;
    uint64_t lost_samples;
    struct event_log event_log[1];
};
typedef struct xenoprof_buf xenoprof_buf_t;
DEFINE_XEN_GUEST_HANDLE(xenoprof_buf_t);

struct xenoprof_init {
    int32_t  num_events;
    int32_t  is_primary;
    char cpu_type[XENOPROF_CPU_TYPE_SIZE];
};
typedef struct xenoprof_init xenoprof_init_t;
DEFINE_XEN_GUEST_HANDLE(xenoprof_init_t);

struct xenoprof_get_buffer {
    int32_t  max_samples;
    int32_t  nbuf;
    int32_t  bufsize;
    uint64_t buf_maddr;
};
typedef struct xenoprof_get_buffer xenoprof_get_buffer_t;
DEFINE_XEN_GUEST_HANDLE(xenoprof_get_buffer_t);

struct xenoprof_counter {
    uint32_t ind;
    uint64_t count;
    uint32_t enabled;
    uint32_t event;
    uint32_t hypervisor;
    uint32_t kernel;
    uint32_t user;
    uint64_t unit_mask;
};
typedef struct xenoprof_counter xenoprof_counter_t;
DEFINE_XEN_GUEST_HANDLE(xenoprof_counter_t);

typedef struct xenoprof_passive {
    uint16_t domain_id;
    int32_t  max_samples;
    int32_t  nbuf;
    int32_t  bufsize;
    uint64_t buf_maddr;
} xenoprof_passive_t;
DEFINE_XEN_GUEST_HANDLE(xenoprof_passive_t);


#endif /* __XEN_PUBLIC_XENOPROF_H__ */

/*
 * Local variables:
 * mode: C
 * c-set-style: "BSD"
 * c-basic-offset: 4
 * tab-width: 4
 * indent-tabs-mode: nil
 * End:
 */
