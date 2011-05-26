/*
 * acm_ops.h: Xen access control module hypervisor commands
 *
 * Reiner Sailer <sailer@watson.ibm.com>
 * Copyright (c) 2005,2006 International Business Machines Corporation.
 */

#ifndef __XEN_PUBLIC_ACM_OPS_H__
#define __XEN_PUBLIC_ACM_OPS_H__

#include "xen.h"
#include "acm.h"

/*
 * Make sure you increment the interface version whenever you modify this file!
 * This makes sure that old versions of acm tools will stop working in a
 * well-defined way (rather than crashing the machine, for instance).
 */
#define ACM_INTERFACE_VERSION   0xAAAA0008

/************************************************************************/

/*
 * Prototype for this hypercall is:
 *  int acm_op(int cmd, void *args)
 * @cmd  == ACMOP_??? (access control module operation).
 * @args == Operation-specific extra arguments (NULL if none).
 */


#define ACMOP_setpolicy         1
struct acm_setpolicy {
    /* IN */
    uint32_t interface_version;
    XEN_GUEST_HANDLE(void) pushcache;
    uint32_t pushcache_size;
};


#define ACMOP_getpolicy         2
struct acm_getpolicy {
    /* IN */
    uint32_t interface_version;
    XEN_GUEST_HANDLE(void) pullcache;
    uint32_t pullcache_size;
};


#define ACMOP_dumpstats         3
struct acm_dumpstats {
    /* IN */
    uint32_t interface_version;
    XEN_GUEST_HANDLE(void) pullcache;
    uint32_t pullcache_size;
};


#define ACMOP_getssid           4
#define ACM_GETBY_ssidref  1
#define ACM_GETBY_domainid 2
struct acm_getssid {
    /* IN */
    uint32_t interface_version;
    uint32_t get_ssid_by; /* ACM_GETBY_* */
    union {
        domaintype_t domainid;
        ssidref_t    ssidref;
    } id;
    XEN_GUEST_HANDLE(void) ssidbuf;
    uint32_t ssidbuf_size;
};

#define ACMOP_getdecision      5
struct acm_getdecision {
    /* IN */
    uint32_t interface_version;
    uint32_t get_decision_by1; /* ACM_GETBY_* */
    uint32_t get_decision_by2; /* ACM_GETBY_* */
    union {
        domaintype_t domainid;
        ssidref_t    ssidref;
    } id1;
    union {
        domaintype_t domainid;
        ssidref_t    ssidref;
    } id2;
    uint32_t hook;
    /* OUT */
    uint32_t acm_decision;
};

#endif /* __XEN_PUBLIC_ACM_OPS_H__ */

/*
 * Local variables:
 * mode: C
 * c-set-style: "BSD"
 * c-basic-offset: 4
 * tab-width: 4
 * indent-tabs-mode: nil
 * End:
 */
