/*
 * ioreq.h: I/O request definitions for device models
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

#ifndef _IOREQ_H_
#define _IOREQ_H_

#define IOREQ_READ      1
#define IOREQ_WRITE     0

#define STATE_INVALID           0
#define STATE_IOREQ_READY       1
#define STATE_IOREQ_INPROCESS   2
#define STATE_IORESP_READY      3

#define IOREQ_TYPE_PIO          0 /* pio */
#define IOREQ_TYPE_COPY         1 /* mmio ops */
#define IOREQ_TYPE_AND          2
#define IOREQ_TYPE_OR           3
#define IOREQ_TYPE_XOR          4
#define IOREQ_TYPE_XCHG         5

/*
 * VMExit dispatcher should cooperate with instruction decoder to
 * prepare this structure and notify service OS and DM by sending
 * virq
 */
struct ioreq {
    uint64_t addr;          /*  physical address            */
    uint64_t size;          /*  size in bytes               */
    uint64_t count;         /*  for rep prefixes            */
    union {
        uint64_t data;      /*  data                        */
        void    *pdata;     /*  pointer to data             */
    } u;
    uint8_t state:4;
    uint8_t pdata_valid:1;  /* if 1, use pdata above        */
    uint8_t dir:1;          /*  1=read, 0=write             */
    uint8_t df:1;
    uint8_t type;           /* I/O type                     */
    uint64_t io_count;      /* How many IO done on a vcpu   */
};
typedef struct ioreq ioreq_t;

struct global_iodata {
    uint16_t    pic_elcr;
    uint16_t    pic_irr;
    uint16_t    pic_last_irr;
    uint16_t    pic_clear_irr;
};
typedef struct global_iodata global_iodata_t;

struct vcpu_iodata {
    struct ioreq         vp_ioreq;
    /* Event channel port */
    unsigned int    vp_eport;   /* VMX vcpu uses this to notify DM */
};
typedef struct vcpu_iodata vcpu_iodata_t;

struct shared_iopage {
    struct global_iodata sp_global;
    struct vcpu_iodata   vcpu_iodata[1];
};
typedef struct shared_iopage shared_iopage_t;

#define IOREQ_BUFFER_SLOT_NUM     80
struct buffered_iopage {
    unsigned long   read_pointer;
    unsigned long   write_pointer;
    ioreq_t         ioreq[IOREQ_BUFFER_SLOT_NUM];
};            /* sizeof this structure must be in one page */
typedef struct buffered_iopage buffered_iopage_t;

#endif /* _IOREQ_H_ */

/*
 * Local variables:
 * mode: C
 * c-set-style: "BSD"
 * c-basic-offset: 4
 * tab-width: 4
 * indent-tabs-mode: nil
 * End:
 */
