/******************************************************************************
 * event.h
 * 
 * A nice interface for passing asynchronous events to guest OSes.
 * 
 * Copyright (c) 2002-2006, K A Fraser
 */

#ifndef __XEN_EVENT_H__
#define __XEN_EVENT_H__

#include <xen/config.h>
#include <xen/sched.h>
#include <xen/smp.h>
#include <xen/softirq.h>
#include <asm/bitops.h>
#include <asm/event.h>

void evtchn_set_pending(struct vcpu *v, int port);

/*
 * send_guest_vcpu_virq: Notify guest via a per-VCPU VIRQ.
 *  @v:        VCPU to which virtual IRQ should be sent
 *  @virq:     Virtual IRQ number (VIRQ_*)
 */
void send_guest_vcpu_virq(struct vcpu *v, int virq);

/*
 * send_guest_global_virq: Notify guest via a global VIRQ.
 *  @d:        Domain to which virtual IRQ should be sent
 *  @virq:     Virtual IRQ number (VIRQ_*)
 */
void send_guest_global_virq(struct domain *d, int virq);

/*
 * send_guest_pirq:
 *  @d:        Domain to which physical IRQ should be sent
 *  @pirq:     Physical IRQ number
 */
void send_guest_pirq(struct domain *d, int pirq);

/* Send a notification from a local event-channel port. */
long evtchn_send(unsigned int lport);

/* Bind a local event-channel port to the specified VCPU. */
long evtchn_bind_vcpu(unsigned int port, unsigned int vcpu_id);

/* Allocate/free a Xen-attached event channel port. */
int alloc_unbound_xen_event_channel(
    struct vcpu *local_vcpu, domid_t remote_domid);
void free_xen_event_channel(
    struct vcpu *local_vcpu, int port);

/* Notify remote end of a Xen-attached event channel.*/
void notify_via_xen_event_channel(int lport);

/* Wait on a Xen-attached event channel. */
#define wait_on_xen_event_channel(port, condition)                      \
    do {                                                                \
        if ( condition )                                                \
            break;                                                      \
        set_bit(_VCPUF_blocked_in_xen, &current->vcpu_flags);           \
        mb(); /* set blocked status /then/ re-evaluate condition */     \
        if ( condition )                                                \
        {                                                               \
            clear_bit(_VCPUF_blocked_in_xen, &current->vcpu_flags);     \
            break;                                                      \
        }                                                               \
        raise_softirq(SCHEDULE_SOFTIRQ);                                \
        do_softirq();                                                   \
    } while ( 0 )

#endif /* __XEN_EVENT_H__ */
