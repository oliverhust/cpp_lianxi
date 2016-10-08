/*****************************************************************************
 Copyright (c) 2008, Hangzhou H3C Technologies Co., Ltd. All rights reserved.
------------------------------------------------------------------------------
                            basetype.h
  Project Code: Comware Leopard
   Module Name:
  Date Created: 2008-4-2 
        Author: c02254
   Description: This file defines all basic data type used in comware

------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------
                                                                             
*****************************************************************************/

#ifndef _SYS_BASETYPE_H_
#define _SYS_BASETYPE_H_

#ifdef  __cplusplus
extern "C" {
#endif


#ifdef _UT_
#ifdef STATIC
#undef STATIC
#endif
#define STATIC
#else
#ifndef STATIC
#define STATIC    static
#endif
#endif

#ifndef INLINE
#define INLINE    inline
#endif

#ifndef VOID
#define VOID    void
#endif

#ifndef CHAR
#define CHAR    char
#endif

#ifndef UCHAR
#define UCHAR   unsigned char
#endif

#ifndef SHORT
#define SHORT   short
#endif

#ifndef USHORT
#define USHORT  unsigned short
#endif

#ifndef LONG
#define LONG    long
#endif

#ifndef ULONG
#define ULONG   unsigned long
#endif

#ifndef UINT
#define UINT    unsigned int
#endif

#ifndef IF_INDEX
#define IF_INDEX unsigned int
#endif

#ifndef INT
#define INT     int
#endif

#ifndef BOOL_T
typedef USHORT  BOOL_T;
#endif

#ifndef FLOAT
#define FLOAT   float
#endif

#ifndef DOUBLE
#define DOUBLE  double
#endif


#ifndef INT8
#define INT8    CHAR
#endif

#ifndef UINT8
#define UINT8   UCHAR
#endif

#ifndef INT16
#define INT16   SHORT
#endif

#ifndef UINT16
#define UINT16  USHORT
#endif

#ifndef INT32
#define INT32   INT
#endif

#ifndef UINT32
#define UINT32  UINT
#endif

#ifndef VRF_INDEX
#define VRF_INDEX USHORT
#endif

#ifndef MT_INDEX
#define MT_INDEX USHORT
#endif

#ifndef VRF_ANY
#define VRF_ANY (VRF_INDEX)0xffff
#endif

#ifndef LIP_ADDR
#define LIP_ADDR USHORT
#endif

#ifndef ZONE_ID
#define ZONE_ID USHORT
#endif

typedef void * LPVOID;

typedef unsigned long long __attribute__((aligned(8)))  UINT64;
typedef long long __attribute__((aligned(8)))           INT64;


#define BOOL_TRUE   ((BOOL_T)1)
#define BOOL_FALSE  ((BOOL_T)0)

#define ENABLE    1
#define DISABLE   2

#undef NULL
#ifdef _UT_
#define NULL    ((ULONG)0)
#else
#define NULL    ((void *)0)
#endif

#ifndef IN
#define IN
#endif

#ifndef OUT
#define OUT
#endif

#ifndef INOUT
#define INOUT
#endif

#define COMWARE_LEOPARD_VERSION 0x070000UL

#define MAC_ADDR_LEN 6

#define  IGNORE_PARAM(x)     ((x) = (x))

#ifndef __KERNEL__
#define likely(x)    __builtin_expect(!!(x), 1)
#define unlikely(x)  __builtin_expect(!!(x), 0)
#endif

#ifndef __KERNEL__
#define MODULE_CONSTRUCT  __attribute__ ((constructor))
#define MODULE_DESTRUCT   __attribute__ ((destructor))
#endif

#ifndef _lint
#define __innerSASSERTCORE2(expr,lineno)   if (__builtin_constant_p(expr) &&(!(expr))) \
{\
    void __attribute__((warning(""#expr""))) __attribute__((unused)) __attribute__((visibility ("hidden"))) _StaticASSERT_##lineno(void); \
    _StaticASSERT_##lineno() ; \
}    
#define __innerSASSERTCORE(expr,num)    __innerSASSERTCORE2(expr,num) 
#define ISSUASSERT(expr)                __innerSASSERTCORE(expr,__LINE__) 
#define STATICASSERT(expr)              __innerSASSERTCORE(expr,__LINE__)     
#else
#define ISSUASSERT(expr)        LINT_ASSERT(expr)
#define STATICASSERT(expr)      LINT_ASSERT(expr)
#endif
#define ISSU
#define FROZEN_IMPL 


#ifndef __KERNEL__

#undef offsetof
#ifdef __compiler_offsetof
#define offsetof(TYPE,MEMBER) __compiler_offsetof(TYPE,MEMBER)
#else
#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
#endif

#define container_of(ptr, type, member) ({  \
    const typeof( ((type *)0)->member ) *__mptr = (ptr);    \
    (type *)( (char *)__mptr - offsetof(type,member) );})
#endif /* __KERNEL__ */

/*
1. 需要转换字节序的场合(需要同时考虑32/64位可移植性):
   跨板，跨CPU 通信，写入文件等。
2. 需要转换字节序的数据类型，必须是固定长度数据类型，所以不能对ULONG类型转换。
3. ntohl/htonl 是针对32位的数据，因前面32位时代有大量这种用法
   为兼容起见，没有命名为ntoh32,hton32
*/
#undef ntohl
#undef ntohs
#undef htonl
#undef htons
#undef ntoh64
#undef hton64

#ifdef __KERNEL__
#ifndef _ASM_BYTEORDER_H
#include <asm/byteorder.h>
#endif
#define htonl(x) ___htonl(x)
#define ntohl(x) ___ntohl(x)
#define htons(x) ___htons(x)
#define ntohs(x) ___ntohs(x)

#define ntoh64(x) __be64_to_cpu(x)    /* 内核根据CPU实现 */
#define hton64(x) __cpu_to_be64(x)

#else
#ifndef _ENDIAN_H
#include <endian.h>
#endif
#ifndef _BYTESWAP_H
#include <byteswap.h>                  /* LIBC 提供 */
#endif
#if (__BYTE_ORDER == __LITTLE_ENDIAN)
#define ntohs(x)  __bswap_16 (x)
#define htons(x)  __bswap_16 (x)
#define ntohl(x)  __bswap_32 (x)
#define htonl(x)  __bswap_32 (x)
#define ntoh64(x) __bswap_64 (x)
#define hton64(x) __bswap_64 (x)
#else
#define ntohs(x)   (x)
#define htons(x)   (x)
#define ntohl(x)   (x)
#define htonl(x)   (x)
#define ntoh64(x)  (x)
#define hton64(x)  (x)
#endif
#endif

typedef enum tagPRIORITY_E
{
    PRIORITY_LOWEST =       0,      /* debug message */
    PRIORITY_LOWER =        1,      /* forwarding packet */
    PRIORITY_LOW =          2,      /* trap message & log message */
    PRIORITY_NORMAL =       3,      /* common control message */
    PRIORITY_NORMAL_PLUS =  4,      /* local packet */
    PRIORITY_HIGH =         5,      /* fast detection protocol message */
    PRIORITY_HIGHER =       6,      /* topo related protocol message */
    PRIORITY_HIGHEST =      7       /* reserved */
}PRIORITY_E;

typedef struct tagDate 
{                                                 
    UCHAR   ucDate;        /* 1-31  */
    UCHAR   ucMonth;       /* 1-12  */
    USHORT  usYear;        /* 1970- */
}DATE_S;

typedef struct tagTime 
{
    UCHAR   ucSecond;      /* 0-59  */
    UCHAR   ucMinute;      /* 0-59  */
    USHORT  usHour;        /* 0-23  */
}TIME_S;

/* 应用方向 */
typedef enum tagDirection
{
    INBOUND,   /* 入方向 */
    OUTBOUND,  /* 出方向 */
    BOTH       /* 双向 */
}DIRECTION_E;

static inline VOID ZONE_NTOH_ZONEID(ZONE_ID *pzoneID)
{
    *pzoneID = ntohs(*pzoneID);
    return;
}

#define ZONE_HTON_ZONEID(x)  ZONE_NTOH_ZONEID(x)

/* 域名称最大长度 */
#define ZONE_MAX_NAME_LEN                       31U

FROZEN_IMPL typedef struct tagZonePair
{
    ZONE_ID zoneIDSrcID;         /* 源域ID */
    ZONE_ID zoneIDDestID;        /* 目的域ID */
} ZONE_PAIRID_S;

static inline VOID ZONE_NTOH_ZONEPAIR(INOUT ZONE_PAIRID_S *pstZonePair)
{
    pstZonePair->zoneIDSrcID = ntohs(pstZonePair->zoneIDSrcID);
    pstZonePair->zoneIDDestID = ntohs(pstZonePair->zoneIDDestID);
    return;
}

#define ZONE_HTON_ZONEPAIR(x) ZONE_NTOH_ZONEPAIR(x)

ISSU typedef struct tagZonePairName
{
    CHAR szSrcZoneName[ZONE_MAX_NAME_LEN + 1];
    CHAR szDestZoneName[ZONE_MAX_NAME_LEN + 1];
} ZONE_PAIRNAME_S;

#pragma pack(1)
typedef struct tagMACAddr
{
    UCHAR aucOctets[MAC_ADDR_LEN];
}MACADDR_S;
#pragma pack()

#ifndef BIT_TEST
#define BIT_TEST(f, b)  (0 != ((f) & (b)))
#endif

#ifndef BIT_SET
#define BIT_SET(f, b)   ((f) |= (b))
#endif

#ifndef BIT_RESET
#define BIT_RESET(f, b) ((f) &= ~(b))
#endif

#ifndef BIT_MATCH
#define BIT_MATCH(f,b)          (((f) & (b)) == (b))
#endif

#ifndef BIT_COMPARE
#define BIT_COMPARE(f,b1,b2)    (((f) & (b1)) == (b2))
#endif

#ifndef __KERNEL__
#ifndef ARRAY_SIZE
#define ARRAY_SIZE(a) (sizeof (a) / sizeof ((a)[0]))
#endif
#endif

#ifdef __KERNEL__
typedef ULONG SESSION_HANDLE;
#endif

#ifdef  __cplusplus
}
#endif

#endif /* _SYS_BASETYPE_H_ */

