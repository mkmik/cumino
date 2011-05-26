#ifndef __X86_TYPES_H__
#define __X86_TYPES_H__

#ifndef __ASSEMBLY__

#include <xen/config.h>

typedef __signed__ char __s8;
typedef unsigned char __u8;

typedef __signed__ short __s16;
typedef unsigned short __u16;

typedef __signed__ int __s32;
typedef unsigned int __u32;

#if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#if defined(__i386__)
typedef __signed__ long long __s64;
typedef unsigned long long __u64;
#elif defined(__x86_64__)
typedef __signed__ long __s64;
typedef unsigned long __u64;
#endif
#endif

typedef signed char s8;
typedef unsigned char u8;

typedef signed short s16;
typedef unsigned short u16;

typedef signed int s32;
typedef unsigned int u32;

#if defined(__i386__)
typedef signed long long s64;
typedef unsigned long long u64;
#if defined(CONFIG_X86_PAE)
typedef u64 paddr_t;
#define PRIpaddr "016llx"
#else
typedef unsigned long paddr_t;
#define PRIpaddr "08lx"
#endif
#elif defined(__x86_64__)
typedef signed long s64;
typedef unsigned long u64;
typedef unsigned long paddr_t;
#define PRIpaddr "016lx"
#endif

typedef unsigned long size_t;

#endif /* __ASSEMBLY__ */

#if defined(__i386__)
#define BITS_PER_LONG 32
#define BYTES_PER_LONG 4
#define LONG_BYTEORDER 2
#elif defined(__x86_64__)
#define BITS_PER_LONG 64
#define BYTES_PER_LONG 8
#define LONG_BYTEORDER 3
#endif

#endif /* __X86_TYPES_H__ */
