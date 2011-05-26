#ifndef __X86_64_UACCESS_H
#define __X86_64_UACCESS_H

/*
 * Valid if in +ve half of 48-bit address space, or above Xen-reserved area.
 * This is also valid for range checks (addr, addr+size). As long as the
 * start address is outside the Xen-reserved area then we will access a
 * non-canonical address (and thus fault) before ever reaching VIRT_START.
 */
#define __addr_ok(addr) \
    (((unsigned long)(addr) < (1UL<<48)) || \
     ((unsigned long)(addr) >= HYPERVISOR_VIRT_END))

#define access_ok(addr, size) (__addr_ok(addr))

#define array_access_ok(addr, count, size) (__addr_ok(addr))

#define __put_user_size(x,ptr,size,retval,errret)			\
do {									\
	retval = 0;							\
	switch (size) {							\
	case 1: __put_user_asm(x,ptr,retval,"b","b","iq",errret);break;	\
	case 2: __put_user_asm(x,ptr,retval,"w","w","ir",errret);break; \
	case 4: __put_user_asm(x,ptr,retval,"l","k","ir",errret);break;	\
	case 8: __put_user_asm(x,ptr,retval,"q","","ir",errret);break;	\
	default: __put_user_bad();					\
	}								\
} while (0)

#define __get_user_size(x,ptr,size,retval,errret)			\
do {									\
	retval = 0;							\
	switch (size) {							\
	case 1: __get_user_asm(x,ptr,retval,"b","b","=q",errret);break;	\
	case 2: __get_user_asm(x,ptr,retval,"w","w","=r",errret);break;	\
	case 4: __get_user_asm(x,ptr,retval,"l","k","=r",errret);break;	\
	case 8: __get_user_asm(x,ptr,retval,"q","","=r",errret); break;	\
	default: (x) = __get_user_bad();				\
	}								\
} while (0)

#endif /* __X86_64_UACCESS_H */
