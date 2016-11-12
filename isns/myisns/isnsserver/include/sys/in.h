/*****************************************************************************
 Copyright (c) 2009, Hangzhou H3C Technologies Co., Ltd. All rights reserved.
------------------------------------------------------------------------------
                              in.h
  Project Code: Comware V7
   Module Name: TCP/IP Socket
  Date Created: 2009/8/6
        Author: J03845
   Description: TCP/IP协议栈使用的公共头文件，此文件中定义了用户态和内核公用的
                宏、结构体等定义。
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------
  2009/8/6    J03845           Created for Comware V7
*****************************************************************************/

#ifndef _SYS_IN_H_
#define _SYS_IN_H_

#ifdef __cplusplus
extern "C"{
#endif
#ifndef _SYS_DOMAIN_H_
#include <sys/domain.h>
#endif
#ifndef _SYS_SOCKET_H_
#include <sys/socket.h>
#endif
/******************************************************************************
 *               这里定义用户态和内核态公用的宏以及结构体                     *
 ******************************************************************************/

#ifndef IPVERSION
#define IPVERSION    4
#endif

#ifndef MAXTTL
#define MAXTTL    255
#endif

#ifndef IP_MAXPACKET
#define IP_MAXPACKET    65535        /* maximum packet size */
#endif

#ifndef INET_ADDRSTRLEN
#define INET_ADDRSTRLEN  16        /* for IPv4 dotted_decimal */
#endif

#ifndef INET6_ADDRSTRLEN
#define INET6_ADDRSTRLEN 46        /* for IPv6 hex string */
#endif

#ifndef NS_INADDRSZ
#define NS_INADDRSZ       4         /* sizeof INADDR_S */
#endif

#ifndef NS_IN6ADDRSZ
#define NS_IN6ADDRSZ      16        /* sizeof IN6ADDR_S */
#endif

#ifndef NS_INT16SZ
#define NS_INT16SZ 2                /*size of unsigned short*/
#endif

/*
 *
 * Constants and structures defined by the internet system,
 * Per RFC 790, September 1981, and numerous additions.
 */

/*
 * Protocols (RFC 1700)
 */
/* Protocols common to RFC 1700, POSIX, and X/Open. */
#define IPPROTO_IP          0       /* dummy for IP */
#define IPPROTO_ICMP        1       /* control message protocol */
#define IPPROTO_TCP         6       /* tcp */
#define IPPROTO_UDP         17      /* user datagram protocol */

#define IPPROTO_HOPOPTS     0       /* IP6 hop-by-hop options */
#define IPPROTO_IGMP        2       /* group mgmt protocol */
#define IPPROTO_GGP         3       /* gateway^2 (deprecated) */
#define IPPROTO_IPV4        4       /* IPv4 encapsulation */
#define IPPROTO_IPIP        IPPROTO_IPV4    /* for compatibility */
#define IPPROTO_ST          7       /* Stream protocol II */
#define IPPROTO_EGP         8       /* exterior gateway protocol */
#define IPPROTO_PIGP        9       /* private interior gateway */
#define IPPROTO_RCCMON      10      /* BBN RCC Monitoring */
#define IPPROTO_NVPII       11      /* network voice protocol*/
#define IPPROTO_PUP         12      /* pup */
#define IPPROTO_ARGUS       13      /* Argus */
#define IPPROTO_EMCON       14      /* EMCON */
#define IPPROTO_XNET        15      /* Cross Net Debugger */
#define IPPROTO_CHAOS       16      /* Chaos*/
#define IPPROTO_MUX         18      /* Multiplexing */
#define IPPROTO_MEAS        19      /* DCN Measurement Subsystems */
#define IPPROTO_HMP         20      /* Host Monitoring */
#define IPPROTO_PRM         21      /* Packet Radio Measurement */
#define IPPROTO_IDP         22      /* xns idp */
#define IPPROTO_TRUNK1      23      /* Trunk-1 */
#define IPPROTO_TRUNK2      24      /* Trunk-2 */
#define IPPROTO_LEAF1       25      /* Leaf-1 */
#define IPPROTO_LEAF2       26      /* Leaf-2 */
#define IPPROTO_RDP         27      /* Reliable Data */
#define IPPROTO_IRTP        28      /* Reliable Transaction */
#define IPPROTO_TP          29      /* tp-4 w/ class negotiation */
#define IPPROTO_BLT         30      /* Bulk Data Transfer */
#define IPPROTO_NSP         31      /* Network Services */
#define IPPROTO_INP         32      /* Merit Internodal */
#define IPPROTO_DCCP        33      /* Datagram Congestion Control Protocol*/
#define IPPROTO_3PC         34      /* Third Party Connect */
#define IPPROTO_IDPR        35      /* InterDomain Policy Routing */
#define IPPROTO_XTP         36      /* XTP */
#define IPPROTO_DDP         37      /* Datagram Delivery */
#define IPPROTO_CMTP        38      /* Control Message Transport */
#define IPPROTO_TPXX        39      /* TP++ Transport */
#define IPPROTO_IL          40      /* IL transport protocol */
#define IPPROTO_IPV6        41      /* IP6 header */
#define IPPROTO_SDRP        42      /* Source Demand Routing */
#define IPPROTO_ROUTING     43      /* IP6 routing header */
#define IPPROTO_FRAGMENT    44      /* IP6 fragmentation header */
#define IPPROTO_IDRP        45      /* InterDomain Routing*/
#define IPPROTO_RSVP        46      /* resource reservation */
#define IPPROTO_GRE         47      /* General Routing Encap. */
#define IPPROTO_MHRP        48      /* Mobile Host Routing */
#define IPPROTO_BHA         49      /* BHA */
#define IPPROTO_ESP         50      /* IP6 Encap Sec. Payload */
#define IPPROTO_AH          51      /* IP6 Auth Header */
#define IPPROTO_INLSP       52      /* Integ. Net Layer Security */
#define IPPROTO_SWIPE       53      /* IP with encryption */
#define IPPROTO_NHRP        54      /* Next Hop Resolution */
#define IPPROTO_MOBILE      55      /* IP Mobility */
#define IPPROTO_TLSP        56      /* Transport Layer Security */
#define IPPROTO_SKIP        57      /* SKIP */
#define IPPROTO_ICMPV6      58      /* ICMP6 */
#define IPPROTO_NONE        59      /* IP6 no next header */
#define IPPROTO_DSTOPTS     60      /* IP6 destination option */
#define IPPROTO_AHIP        61      /* any host internal protocol */
#define IPPROTO_CFTP        62      /* CFTP */
#define IPPROTO_HELLO       63      /* "hello" routing protocol */
#define IPPROTO_SATEXPAK    64      /* SATNET/Backroom EXPAK */
#define IPPROTO_KRYPTOLAN   65      /* Kryptolan */
#define IPPROTO_RVD         66      /* Remote Virtual Disk */
#define IPPROTO_IPPC        67      /* Pluribus Packet Core */
#define IPPROTO_ADFS        68      /* Any distributed FS */
#define IPPROTO_SATMON      69      /* Satnet Monitoring */
#define IPPROTO_VISA        70      /* VISA Protocol */
#define IPPROTO_IPCV        71      /* Packet Core Utility */
#define IPPROTO_CPNX        72      /* Comp. Prot. Net. Executive */
#define IPPROTO_CPHB        73      /* Comp. Prot. HeartBeat */
#define IPPROTO_WSN         74      /* Wang Span Network */
#define IPPROTO_PVP         75      /* Packet Video Protocol */
#define IPPROTO_BRSATMON    76      /* BackRoom SATNET Monitoring */
#define IPPROTO_ND          77      /* Sun net disk proto (temp.) */
#define IPPROTO_WBMON       78      /* WIDEBAND Monitoring */
#define IPPROTO_WBEXPAK     79      /* WIDEBAND EXPAK */
#define IPPROTO_EON         80      /* ISO cnlp */
#define IPPROTO_VMTP        81      /* VMTP */
#define IPPROTO_SVMTP       82      /* Secure VMTP */
#define IPPROTO_VINES       83      /* Banyon VINES */
#define IPPROTO_TTP         84      /* TTP */
#define IPPROTO_IGP         85      /* NSFNET-IGP */
#define IPPROTO_DGP         86      /* dissimilar gateway prot. */
#define IPPROTO_TCF         87      /* TCF */
#define IPPROTO_IGRP        88      /* Cisco/GXS IGRP */
#define IPPROTO_OSPFIGP     89      /* OSPFIGP */
#define IPPROTO_SRPC        90      /* Strite RPC protocol */
#define IPPROTO_LARP        91      /* Locus Address Resoloution */
#define IPPROTO_MTP         92      /* Multicast Transport */
#define IPPROTO_AX25        93      /* AX.25 Frames */
#define IPPROTO_IPEIP       94      /* IP encapsulated in IP */
#define IPPROTO_MICP        95      /* Mobile Int.ing control */
#define IPPROTO_SCCSP       96      /* Semaphore Comm. security */
#define IPPROTO_ETHERIP     97      /* Ethernet IP encapsulation */
#define IPPROTO_ENCAP       98      /* encapsulation header */
#define IPPROTO_APES        99      /* any private encr. scheme */
#define IPPROTO_GMTP        100     /* GMTP*/
#define IPPROTO_IPCOMP      108     /* payload compression (IPComp) */
#define IPPROTO_SCTP        132     /* SCTP */
#define IPPROTO_UDPLITE     136
/* 101-254: Partly Unassigned */
#define IPPROTO_PIM         103     /* Protocol Independent Mcast */
#define IPPROTO_VRRP        112     /* VRRP */
#define IPPROTO_PGM         113     /* PGM */
#define IPPROTO_PFSYNC      240     /* PFSYNC */
/* 255: Reserved */
/* BSD Private, local use, namespace incursion, no longer used */
#define IPPROTO_OLD_DIVERT  254     /* OLD divert pseudo-proto */
#define IPPROTO_RAW         255     /* raw socket protocol */
#define IPPROTO_MAX         256

/* last return value of *_input(), meaning "all job for this pkt is done".  */
#define IPPROTO_DONE        257

/* Only used internally, so can be outside the range of valid IP protocols. */
#define IPPROTO_DIVERT      258     /* divert pseudo-protocol */

/*
 * Defined to avoid confusion.  The master value is defined by
 * PROTO_SPACER in sys/protosw.h.
 */
#define IPPROTO_SPACER      32767       /* spacer for loadable protos */

/* Internet address.  */
typedef UINT32 in_addr_t;

struct in_addr
{
    in_addr_t s_addr;
};

typedef struct in_addr INADDR_S;

/* Type to represent a port.  */
typedef UINT16 in_port_t;

/* Socket address, internet style. */
struct sockaddr_in {
    unsigned char sin_len;
    unsigned char sin_family;
    in_port_t   sin_port;
    struct  in_addr sin_addr;
    /* Begin: Added by J03845 for Socket-Dist-Project. */
    VRF_INDEX sin_vrf;
    /* End: Added by J03845 for Socket-Dist-Project. */
    char    sin_zero[sizeof(struct sockaddr) - \
                     (sizeof(unsigned char) + \
                     sizeof(unsigned char) + \
                     sizeof(in_port_t) + \
                     sizeof(struct  in_addr) + \
                     sizeof(VRF_INDEX))];
};


typedef UINT16 n_short;      /* short as received from the net */
typedef UINT32 n_long;       /* long as received from the net */
typedef UINT32 n_time;       /* ms since 00:00 GMT, byte rev */

/* Definitions of the bits in an Internet address integer.

   On subnets, host and network parts are found according to
   the subnet mask, not these masks.  */

#define IN_CLASSA(a)        ((((in_addr_t)(a)) & 0x80000000) == 0)
#define IN_CLASSA_NET       0xff000000
#define IN_CLASSA_NSHIFT    24
#define IN_CLASSA_HOST      (0xffffffff & ~IN_CLASSA_NET)
#define IN_CLASSA_MAX       128

#define IN_CLASSB(a)        ((((in_addr_t)(a)) & 0xc0000000) == 0x80000000)
#define IN_CLASSB_NET       0xffff0000
#define IN_CLASSB_NSHIFT    16
#define IN_CLASSB_HOST      (0xffffffff & ~IN_CLASSB_NET)
#define IN_CLASSB_MAX       65536

#define IN_CLASSC(a)        ((((in_addr_t)(a)) & 0xe0000000) == 0xc0000000)
#define IN_CLASSC_NET       0xffffff00
#define IN_CLASSC_NSHIFT    8
#define IN_CLASSC_HOST      (0xffffffff & ~IN_CLASSC_NET)

#define IN_CLASSD(a)        (((in_addr_t)(a) & 0xf0000000) == 0xe0000000)
#define IN_CLASSD_NET       0xf0000000  /* These ones aren't really */
#define IN_CLASSD_NSHIFT    28      /* net and host fields, but */
#define IN_CLASSD_HOST      0x0fffffff  /* routing needn't know.    */
#define IN_MULTICAST(a)     IN_CLASSD(a)
#define IN_LOOPBACK(a)      (((in_addr_t) (a) & 0xff000000) == 0x7f000000)

#define IN_EXPERIMENTAL(a)  ((((in_addr_t)(a)) & 0xe0000000) == 0xe0000000)
#define IN_BADCLASS(a)      ((((in_addr_t)(a)) & 0xf0000000) == 0xf0000000)


#define IN_LINKLOCAL(a)     (((in_addr_t)(a) & 0xffff0000) == 0xa9fe0000)

#define IN_PRIVATE(a)   ((((in_addr_t)(a) & 0xff000000) == 0x0a000000) || \
                         (((in_addr_t)(a) & 0xfff00000) == 0xac100000) || \
                         (((in_addr_t)(a) & 0xffff0000) == 0xc0a80000))

#define IN_LOCAL_GROUP(a)   (((in_addr_t)(a) & 0xffffff00) == 0xe0000000)
 
#define IN_ANY_LOCAL(a)     (IN_LINKLOCAL(a) || IN_LOCAL_GROUP(a))


/* Address to accept any incoming messages.  */
#define INADDR_ANY      ((in_addr_t) 0x00000000)
/* Address to send to all hosts.  */
#define INADDR_BROADCAST    ((in_addr_t) 0xffffffff)
/* Address indicating an error return.  */
#define INADDR_NONE     ((in_addr_t) 0xffffffff)

/* Network number for local host loopback.  */
#define IN_LOOPBACKNET      127
/* Address to loopback in software to local host.  */
#define INADDR_LOOPBACK    ((in_addr_t) 0x7f000001) /* Inet 127.0.0.1.  */


/* Defines for Multicast INADDR.  */
#define INADDR_UNSPEC_GROUP     ((in_addr_t)0xe0000000)  /* 224.0.0.0 */
#define INADDR_ALLHOSTS_GROUP   ((in_addr_t)0xe0000001)  /* 224.0.0.1 */
#define INADDR_ALLRTRS_GROUP    ((in_addr_t)0xe0000002)  /* 224.0.0.2 */
#define INADDR_ALLRPTS_GROUP    ((in_addr_t)0xe0000016)  /* 224.0.0.22, IGMPv3 */
#define INADDR_CARP_GROUP       ((in_addr_t)0xe0000012)  /* 224.0.0.18 */
#define INADDR_PFSYNC_GROUP     ((in_addr_t)0xe00000f0)  /* 224.0.0.240 */
#define INADDR_ALLMDNS_GROUP    ((in_addr_t)0xe00000fb)  /* 224.0.0.251 */
#define INADDR_MAX_LOCAL_GROUP  ((in_addr_t)0xe00000ff)  /* 224.0.0.255 */

/*****************************************************************************
    Func Name: INADDR_IsValidUCAddr
 Date Created: 2009/12/25
       Author: zhouwenming 02256
  Description: Check unicast address validity.
        Input: uiIPAddr: input address(host order)
       Output: 
       Return: if valid return BOOL_TRUE else return BOOL_FALSE
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
static inline BOOL_T INADDR_IsValidUCAddr(IN UINT32 uiIPAddr)
{
    if ((INADDR_BROADCAST == uiIPAddr) || (INADDR_ANY == uiIPAddr))
    {
        return BOOL_FALSE;
    }

    /* 排除非法IP地址，0.1.1.1等 */
    if (IN_CLASSA(uiIPAddr))
    {   
        if (0 == (uiIPAddr & IN_CLASSA_NET))
        {
            return BOOL_FALSE;
        }
    }

    if (IN_CLASSA(uiIPAddr) || IN_CLASSB(uiIPAddr) || IN_CLASSC(uiIPAddr))
    {
        return BOOL_TRUE;
    }

    return BOOL_FALSE;
}

/*****************************************************************************
    Func Name: INADDR_IsValidMask
 Date Created: 2009/12/25
       Author: zhouwenming 02256
  Description: Check mask validity.
        Input: uiMask: input mask(host order)
       Output: 
       Return: if valid return BOOL_TRUE else return BOOL_FALSE
      Caution: 0.0.0.0 is a valid mask
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
static inline BOOL_T INADDR_IsValidMask(IN UINT32 uiMask)
{
    INT i;
    UINT32 uiMaskTemp = INADDR_NONE;
    
    for (i = 0; i <= 32; i++)
    {
        if (uiMask == uiMaskTemp)
        {
            return BOOL_TRUE;
        }
        uiMaskTemp <<= 1 ;
    }
    return BOOL_FALSE;
}

/*****************************************************************************
    Func Name: INADDR_MaskToLen
 Date Created: 2009/12/25
       Author: zhouwenming 02256
  Description: get the mask length.
        Input: uiMask: input mask(host order)
       Output: 
       Return: mask length
      Caution: the input mask must be valid
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
static inline UINT INADDR_MaskToLen(IN UINT32 uiMask)
{
    UINT i = 0;
    
    while (0 != uiMask)
    {
        uiMask <<= 1;
        i++;
    }
    
    return i;
}

/*****************************************************************************
    Func Name: INADDR_LenToMask
 Date Created: 2009/12/25
       Author: zhouwenming 02256
  Description: get mask by masklen.
        Input: uiLen: mask length
       Output: puiMask: mask(host order)
       Return: void
      Caution: mask length must <= 32
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------
  2010-05-04  g06076           LPTB00126(MIPS CPU不支持左移32位操作)
*****************************************************************************/
static inline VOID INADDR_LenToMask(IN UINT uiLen, OUT UINT32 *puiMask)
{
    if (0 == uiLen)
    {
        *puiMask = 0;
    }
    else
    {
        *puiMask = INADDR_NONE<<(32-uiLen);
    }

    return;
}

#define IN6ADDR_SIZE8    NS_IN6ADDRSZ
#define IN6ADDR_SIZE16   8
#define IN6ADDR_SIZE32   4

struct in6_addr {
    union
    {
        unsigned char   __u6_addr8[IN6ADDR_SIZE8];
        unsigned short  __u6_addr16[IN6ADDR_SIZE16];
        unsigned int    __u6_addr32[IN6ADDR_SIZE32];
    } __u6_addr;            /* 128-bit IP6 address */
    
    #define s6_addr   __u6_addr.__u6_addr8
    #define s6_addr16 __u6_addr.__u6_addr16
    #define s6_addr32 __u6_addr.__u6_addr32
};

typedef struct in6_addr IN6ADDR_S;

/* 表示地址范围类型的宏IN6ADDR_SCOPE_XXX */
#define  IN6ADDR_SCOPE_RESERVED         0x00    /* 保留范围 */
#define  IN6ADDR_SCOPE_INTFACELOCAL     0x01    /* 接口本地范围 */
#define  IN6ADDR_SCOPE_LINKLOCAL        0x02    /* 链路本地范围 */
#define  IN6ADDR_SCOPE_ADMINLOCAL       0x04    /* 管理本地范围 */
#define  IN6ADDR_SCOPE_SITELOCAL        0x05    /* 站点本地范围 */
#define  IN6ADDR_SCOPE_ORGLOCAL         0x08    /* 组织本地范围 */
#define  IN6ADDR_SCOPE_GLOBAL           0x0e    /* 全球范围 */

#define IN6ADDR_UNICAST_UNSPECIFIED     0       /* 未指定地址，即全0 */
#define IN6ADDR_UNICAST_INLOOPBACK      1       /* 环回地址，即::1 */
#define IN6ADDR_UNICAST_LINKLOCAL       2       /* 链路本地地址 */
#define IN6ADDR_UNICAST_V4MAPPED        3       /* 映射到IPv4的IPv6地址 */
#define IN6ADDR_UNICAST_V4COMPAT        4       /* 与IPv4兼容的IPv6地址 */
#define IN6ADDR_UNICAST_GLOBAL          5       /* 普通全球单播地址，即除V4MAPPED外的全球单播地址 */
#define IN6ADDR_UNICAST_INVALID         6       /* 无效地址 */

/* Prefix length for Link local addess and Site local address */
#define IN6ADDR_PRELEN_LLA    10       /* 链路本地地址前缀长度 */
#define IN6ADDR_PRELEN_MAX    128      /* IPv6地址前缀最大长度 */

/* 常用IPv6地址的宏定义 */
/* 未指定地址 */
#define IN6ADDR_ANY_INIT \
        {{{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }}}
/* 环回地址 */
#define IN6ADDR_LOOPBACK_INIT \
        {{{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01 }}}
/* 接口范围内所有节点多播地址 */
#define IN6ADDR_INTFACELOCAL_ALLNODES_INIT \
        {{{ 0xff, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01 }}}
/* 链路范围内所有节点多播地址 */
#define IN6ADDR_LINKLOCAL_ALLNODES_INIT \
        {{{ 0xff, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01 }}}
/* 链路范围内所有路由器多播地址 */
#define IN6ADDR_LINKLOCAL_ALLROUTERS_INIT \
        {{{ 0xff, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02 }}}
/* 链路范围内MLDv2的路由器多播地址 */
#define IN6ADDR_LINKLOCAL_ALLV2ROUTERS_INIT \
        {{{ 0xff, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x16 }}}


/*****************************************************************************
    Func Name: IN6ADDR_IsUnspecified
 Date Created: 2010/9/19
       Author: zhouwenming 02256
  Description: 判断Ipv6地址是不是未指定地址(即全0)
        Input: pstAddr: 待判断的IPv6地址
       Output: 无
       Return: BOOL_TRUE: 是未指定地址
               BOOL_FALSE: 不是未指定地址
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
static inline BOOL_T IN6ADDR_IsUnspecified(IN const IN6ADDR_S *pstAddr)
{
    const UINT *puiAddr;
    
    puiAddr = pstAddr->s6_addr32;
    
    return ((puiAddr[0] == 0) && (puiAddr[1] == 0) && (puiAddr[2] == 0) && (puiAddr[3] == 0));
}

/*****************************************************************************
    Func Name: IN6ADDR_IsLoopback
 Date Created: 2010/9/19
       Author: zhouwenming 02256
  Description: 判断Ipv6地址是不是环回地址(::1)
        Input: pstAddr: 待判断的IPv6地址
       Output: 无
       Return: BOOL_TRUE: 是环回地址
               BOOL_FALSE: 不是环回地址
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
static inline BOOL_T IN6ADDR_IsLoopback(IN const IN6ADDR_S *pstAddr)
{
    const UINT *puiAddr;
    
    puiAddr = pstAddr->s6_addr32;
    
    return ((puiAddr[0] == 0) && (puiAddr[1] == 0) && 
            (puiAddr[2] == 0) && (puiAddr[3] == htonl(1)));
}

/*****************************************************************************
    Func Name: IN6ADDR_IsLinkLocal
 Date Created: 2010/9/19
       Author: zhouwenming 02256
  Description: 判断Ipv6地址是不是链路本地地址
        Input: pstAddr: 待判断的IPv6地址
       Output: 无
       Return: BOOL_TRUE: 是链路本地地址
               BOOL_FALSE: 不是链路本地地址
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
static inline BOOL_T IN6ADDR_IsLinkLocal(IN const IN6ADDR_S *pstAddr)
{
    const UCHAR *pucAddr;

    pucAddr = pstAddr->s6_addr;
    
    return ((pucAddr[0] == 0xfe) && ((pucAddr[1] & 0xc0) == 0x80));
}

/*****************************************************************************
    Func Name: IN6ADDR_IsV4Mapped
 Date Created: 2010/9/19
       Author: zhouwenming 02256
  Description: 判断Ipv6地址是不是IPv4映射的IPv6地址
        Input: pstAddr: 待判断的IPv6地址
       Output: 无
       Return: BOOL_TRUE: 是IPv4映射的IPv6地址
               BOOL_FALSE: 不是IPv4映射的IPv6地址
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
static inline BOOL_T IN6ADDR_IsV4Mapped(IN const IN6ADDR_S *pstAddr)
{
    const UINT *puiAddr;

    puiAddr = pstAddr->s6_addr32;
    return ((puiAddr[0] == 0) && (puiAddr[1] == 0) && (puiAddr[2] == htonl(0x0000ffff)));
}

/*****************************************************************************
    Func Name: IN6ADDR_IsV4Compat
 Date Created: 2010/9/19
       Author: zhouwenming 02256
  Description: 判断Ipv6地址是不是与IPv4兼容的IPv6地址
        Input: pstAddr: 待判断的IPv6地址
       Output: 无
       Return: BOOL_TRUE: 是与IPv4兼容的IPv6地址
               BOOL_FALSE: 不是与IPv4兼容的IPv6地址
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
static inline BOOL_T IN6ADDR_IsV4Compat(IN const IN6ADDR_S *pstAddr)
{
    const UINT *puiAddr;

    puiAddr = pstAddr->s6_addr32;
    return ((puiAddr[0] == 0) && (puiAddr[1] == 0) && 
            (puiAddr[2] == 0) && (puiAddr[3] !=0) && 
            (puiAddr[3] != htonl(1)) );
}

/*****************************************************************************
    Func Name: IN6ADDR_GetPrefix
 Date Created: 2010/9/19
       Author: zhouwenming 02256
  Description: 获取地址对应的前缀
        Input: pstAddr: IPv6地址
               pstMask: 掩码
       Output: pstPrefix: 输出的前缀
       Return: 无
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
static inline VOID IN6ADDR_GetPrefix(IN const IN6ADDR_S *pstAddr, IN const IN6ADDR_S *pstMask, 
                                            OUT IN6ADDR_S *pstPrefix)
{
    UINT *puiPrefix;
    const UINT *puiAddr;
    const UINT *puiMask;
    
    puiPrefix = pstPrefix->s6_addr32;
    puiAddr = pstAddr->s6_addr32;
    puiMask = pstMask->s6_addr32;

    puiPrefix[0] = puiAddr[0] & puiMask[0];
    puiPrefix[1] = puiAddr[1] & puiMask[1];
    puiPrefix[2] = puiAddr[2] & puiMask[2];
    puiPrefix[3] = puiAddr[3] & puiMask[3];
    
    return;
}

/*****************************************************************************
    Func Name: IN6ADDR_IsEqual
 Date Created: 2010/9/19
       Author: zhouwenming 02256
  Description: 判断Ipv6地址是否相等
        Input: pstAddr1: 待比较地址1
               pstAddr2: 待比较地址2
       Output: 无
       Return: BOOL_TRUE: 输入的两地址相等
               BOOL_FALSE: 输入的两地址不相等
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
static inline BOOL_T IN6ADDR_IsEqual(IN const IN6ADDR_S *pstAddr1, IN const IN6ADDR_S *pstAddr2)
{
    const UINT *puiAddr1;
    const UINT *puiAddr2;
    
    puiAddr1 = pstAddr1->s6_addr32;
    puiAddr2 = pstAddr2->s6_addr32;
    
    return ( (puiAddr1[0] == puiAddr2[0]) && 
             (puiAddr1[1] == puiAddr2[1]) && 
             (puiAddr1[2] == puiAddr2[2]) && 
             (puiAddr1[3] == puiAddr2[3]) );
}

/*****************************************************************************
    Func Name: IN6ADDR_Cmp
 Date Created: 2010/9/19
       Author: zhouwenming 02256
  Description: 比较IPv6地址大小
        Input: pstAddr1: 待比较地址1
               pstAddr2: 待比较地址2
       Output: 无
       Return: 大于0: 地址1大于地址2
               小于0: 地址1小于地址2
               等于0: 地址1等于地址2
      Caution: IPV6地址按网络序输入
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
static inline INT IN6ADDR_Cmp(IN const IN6ADDR_S *pstAddr1, IN const IN6ADDR_S *pstAddr2)
{
    UINT i;
    INT iRet;

    for (i = 0; i < NS_IN6ADDRSZ; i++)
    {
        iRet = pstAddr1->s6_addr[i] - pstAddr2->s6_addr[i];
        if (0 != iRet)
        {
            break;
        }
    }
    
    return iRet;
}

/*****************************************************************************
    Func Name: IN6ADDR_Copy
 Date Created: 2010/9/19
       Author: zhouwenming 02256
  Description: 拷贝IPv6地址
        Input: pstSrcAddr: 源地址
       Output: pstDestAddr: 目的地址
       Return: 无
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
static inline VOID IN6ADDR_Copy(OUT IN6ADDR_S *pstDestAddr, IN const IN6ADDR_S *pstSrcAddr)
{
    UINT *puiDestAddr;
    const UINT *puiSrcAddr;
    
    puiDestAddr = pstDestAddr->s6_addr32;
    puiSrcAddr = pstSrcAddr->s6_addr32;

    puiDestAddr[0] = puiSrcAddr[0];
    puiDestAddr[1] = puiSrcAddr[1];
    puiDestAddr[2] = puiSrcAddr[2];
    puiDestAddr[3] = puiSrcAddr[3];

    return;
}

/*****************************************************************************
    Func Name: IN6ADDR_MaskToLen
 Date Created: 2010/9/19
       Author: zhouwenming 02256
  Description: 根据掩码计算得到掩码长度
        Input: pstMask: IPv6地址掩码
       Output: 无
       Return: 掩码长度
      Caution: 由调用者保证掩码的合法性
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
static inline UINT IN6ADDR_MaskToLen(IN const IN6ADDR_S *pstMask)
{
    UINT uiMaskLen = 0;
    UINT uiLoop;
    const UINT *puiMask;
    UINT uiMask;

    puiMask = pstMask->s6_addr32;

    for(uiLoop = 0; uiLoop < IN6ADDR_SIZE32; uiLoop++)
    {
        uiMask = puiMask[uiLoop];
        if(0xffffffff == uiMask)
        {
            uiMaskLen += 32;
        }
        else
        {
            uiMask = ntohl(uiMask);
            if ((uiMask & 0xffff0000) == 0xffff0000)
            {
                uiMaskLen += 16;
                uiMask <<= 16;
            }
            if ((uiMask & 0xff000000) == 0xff000000)
            {
                uiMaskLen += 8;
                uiMask <<= 8;
            }
            if ((uiMask & 0xf0000000) == 0xf0000000)
            {
                uiMaskLen += 4;
                uiMask <<= 4;
            }
            if ((uiMask & 0xc0000000) == 0xc0000000)
            {
                uiMaskLen += 2;
                uiMask <<= 2;
            }
            if ((uiMask & 0x80000000) == 0x80000000)
            {
                uiMaskLen += 1;
            }

            return uiMaskLen;
        }
    }

    return uiMaskLen;
}

/*****************************************************************************
    Func Name: IN6ADDR_LenToMask
 Date Created: 2010/9/19
       Author: zhouwenming 02256
  Description: 根据掩码长度计算得到掩码
        Input: uiLen: IPv6地址掩码长度
       Output: pstMask: 成功时输出的IPv6地址掩码
       Return: 无
      Caution: 由调用者保证掩码的合法性
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
static inline VOID IN6ADDR_LenToMask(IN UINT uiLen, OUT IN6ADDR_S *pstMask)
{
    UINT uiUintLen;
    UINT uiBitLen;
    UINT uiLoop;
    UINT *puiMask;

    puiMask = pstMask->s6_addr32;
    
    puiMask[0] = 0;
    puiMask[1] = 0;
    puiMask[2] = 0;
    puiMask[3] = 0;

    uiUintLen = uiLen >> 5;     /* uiLen除以32 */
    uiBitLen = uiLen & 31;      /* uiLen除以32的余数 */
    for (uiLoop = 0; uiLoop < uiUintLen; uiLoop++)
    {
        puiMask[uiLoop] = 0xffffffff;
    }
    
    if (uiBitLen != 0)
    {
        puiMask[uiUintLen] = htonl(((UINT)0xffffffff) << (32-uiBitLen));
    }

    return;
}

/*****************************************************************************
    Func Name: IN6ADDR_IsMulticast
 Date Created: 2010/9/19
       Author: zhouwenming 02256
  Description: 判断Ipv6地址是不是多播地址
        Input: pstAddr: 待判断的IPv6地址
       Output: 无
       Return: BOOL_TRUE: 是多播地址
               BOOL_FALSE: 不是多播地址
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
static inline BOOL_T IN6ADDR_IsMulticast(IN const IN6ADDR_S *pstAddr)
{
    return (pstAddr->s6_addr[0] == 0xff);
}

/*****************************************************************************
    Func Name: IN6ADDR_GetMCAddrScope
 Date Created: 2010/9/19
       Author: zhouwenming 02256
  Description: 获取多播地址作用范围(scope)
        Input: pstMCAddr: IPv6多播地址
       Output: 无
       Return: scope，比如IN6ADDR_SCOPE_LINKLOCAL
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
static inline UINT IN6ADDR_GetMCAddrScope(IN const IN6ADDR_S *pstMCAddr)
{
    return (pstMCAddr->s6_addr[1] & 0x0f);
}

/*****************************************************************************
    Func Name: IN6ADDR_IsIntfaceLocalMCAddr
 Date Created: 2010/9/19
       Author: zhouwenming 02256
  Description: 判断Ipv6地址是不是接口本地范围多播地址
        Input: pstMCAddr: 待判断的IPv6地址
       Output: 无
       Return: BOOL_TRUE: 是接口本地范围多播地址
               BOOL_FALSE: 不是接口本地范围多播地址
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
static inline BOOL_T IN6ADDR_IsIntfaceLocalMCAddr(IN const IN6ADDR_S *pstAddr)
{
    return ((pstAddr->s6_addr[0] == 0xff) && ((pstAddr->s6_addr[1] & 0x0f) == 0x01));
}

/*****************************************************************************
    Func Name: IN6ADDR_IsLinkLocalMCAddr
 Date Created: 2010/9/19
       Author: zhouwenming 02256
  Description: 判断Ipv6地址是不是链路本地范围多播地址
        Input: pstAddr: 待判断的IPv6地址
       Output: 无
       Return: BOOL_TRUE: 是链路本地范围多播地址
               BOOL_FALSE: 不是链路本地范围多播地址
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
static inline BOOL_T IN6ADDR_IsLinkLocalMCAddr(IN const IN6ADDR_S *pstAddr)
{
    return ((pstAddr->s6_addr[0] == 0xff) && ((pstAddr->s6_addr[1] & 0x0f) == 0x02));
}

/*****************************************************************************
    Func Name: IN6ADDR_IsMCAdminLocalMCAddr
 Date Created: 2010/9/19
       Author: zhouwenming 02256
  Description: 判断Ipv6地址是不是管理本地范围多播地址
        Input: pstAddr: 待判断的IPv6地址
       Output: 无
       Return: BOOL_TRUE: 是管理本地范围多播地址
               BOOL_FALSE: 不是管理本地范围多播地址
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
static inline BOOL_T IN6ADDR_IsAdminLocalMCAddr(IN const IN6ADDR_S *pstAddr)
{
    return ((pstAddr->s6_addr[0] == 0xff) && ((pstAddr->s6_addr[1] & 0x0f) == 0x04));
}

/*****************************************************************************
    Func Name: IN6ADDR_IsSiteLocalMCAddr
 Date Created: 2010/9/19
       Author: zhouwenming 02256
  Description: 判断Ipv6地址是不是站点本地范围多播地址
        Input: pstAddr: 待判断的IPv6地址
       Output: 无
       Return: BOOL_TRUE: 是站点本地范围多播地址
               BOOL_FALSE: 不是站点本地范围多播地址
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
static inline BOOL_T IN6ADDR_IsSiteLocalMCAddr(IN const IN6ADDR_S *pstAddr)
{
    return ((pstAddr->s6_addr[0] == 0xff) && ((pstAddr->s6_addr[1] & 0x0f) == 0x05));
}

/*****************************************************************************
    Func Name: IN6ADDR_IsOrgLocalMCAddr
 Date Created: 2010/9/19
       Author: zhouwenming 02256
  Description: 判断Ipv6地址是不是组织本地范围多播地址
        Input: pstAddr: 待判断的IPv6地址
       Output: 无
       Return: BOOL_TRUE: 是组织本地范围多播地址
               BOOL_FALSE: 不是组织本地范围多播地址
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
static inline BOOL_T IN6ADDR_IsOrgLocalMCAddr(IN const IN6ADDR_S *pstAddr)
{
    return ((pstAddr->s6_addr[0] == 0xff) && ((pstAddr->s6_addr[1] & 0x0f) == 0x08));
}

/*****************************************************************************
    Func Name: IN6ADDR_IsGlobalMCAddr
 Date Created: 2010/9/19
       Author: zhouwenming 02256
  Description: 判断Ipv6地址是不是全球范围多播地址
        Input: pstAddr: 待判断的IPv6地址
       Output: 无
       Return: BOOL_TRUE: 是全球多播地址
               BOOL_FALSE: 不是全球多播地址
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
static inline BOOL_T IN6ADDR_IsGlobalMCAddr(IN const IN6ADDR_S *pstAddr)
{
    return ((pstAddr->s6_addr[0] == 0xff) && ((pstAddr->s6_addr[1] & 0x0f) == 0x0e));
}

/*****************************************************************************
    Func Name: IN6ADDR_IsSolicitedMCAddr
 Date Created: 2010/9/19
       Author: zhouwenming 02256
  Description: 判断Ipv6地址是不是被请求节点多播地址
        Input: pstAddr: 待判断的IPv6地址
       Output: 无
       Return: BOOL_TRUE: 是被请求节点多播地址
               BOOL_FALSE: 不是被请求节点多播地址
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
static inline BOOL_T IN6ADDR_IsSolicitedMCAddr(IN const IN6ADDR_S *pstAddr)
{
    return ((pstAddr->s6_addr16[0] == htons(0xff02)) && (pstAddr->s6_addr32[1] == 0) && 
            (pstAddr->s6_addr32[2] == htonl(1)) && (pstAddr->s6_addr[12] == 0xff));
}

/*****************************************************************************
    Func Name: IN6ADDR_GetSolicitedMCAddr
 Date Created: 2010/9/19
       Author: zhouwenming 02256
  Description: 根据单播地址获取其对应的被请求节点多播地址
        Input: pstAddr: IPv6地址
       Output: pstMCAddr: 被请求节点多播地址
       Return: 无
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
static inline VOID IN6ADDR_GetSolicitedMCAddr(IN const IN6ADDR_S *pstAddr, OUT IN6ADDR_S *pstMCAddr)
{
    UINT *puiMCAddr;

    puiMCAddr = pstMCAddr->s6_addr32;
    
    puiMCAddr[0] = htonl(0xFF020000);
    puiMCAddr[1] = 0;
    puiMCAddr[2] = htonl(1);
    puiMCAddr[3] = pstAddr->s6_addr32[3];
    
    pstMCAddr->s6_addr[12] = 0xff;
    
    return;
}

/*****************************************************************************
    Func Name: IN6ADDR_GetUCAddrType
 Date Created: 2010/9/19
       Author: zhouwenming 02256
  Description: 根据单播地址类型
        Input: pstAddr: IPv6单播地址
       Output: 无
       Return: 单播地址类型，比如IN6ADDR_UNICAST_UNSPECIFIED
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
static inline UINT IN6ADDR_GetUCAddrType(IN const IN6ADDR_S *pstAddr)
{
    UINT uiAddrType;
    
    if( BOOL_TRUE == IN6ADDR_IsMulticast(pstAddr) )        /* 地址为多播地址, 无效 */
    {
        uiAddrType = IN6ADDR_UNICAST_INVALID;
    }
    else if( BOOL_TRUE == IN6ADDR_IsUnspecified(pstAddr) ) /* 地址为未指定全零地址 */
    {
        uiAddrType = IN6ADDR_UNICAST_UNSPECIFIED;
    }
    else if( BOOL_TRUE == IN6ADDR_IsLoopback(pstAddr) )    /* 地址为环回地址 */
    {
        uiAddrType = IN6ADDR_UNICAST_INLOOPBACK;
    }
    else if( BOOL_TRUE == IN6ADDR_IsLinkLocal(pstAddr) )   /* 地址为链路本地地址 */
    {
        uiAddrType = IN6ADDR_UNICAST_LINKLOCAL;
    }
    else if( BOOL_TRUE == IN6ADDR_IsV4Mapped(pstAddr) )    /* 地址为IPv4映射的IPv6地址 */
    {
        uiAddrType = IN6ADDR_UNICAST_V4MAPPED;
    }
    else if( BOOL_TRUE == IN6ADDR_IsV4Compat(pstAddr) )
    {
        uiAddrType = IN6ADDR_UNICAST_V4COMPAT;
    }
    else
    {
        uiAddrType = IN6ADDR_UNICAST_GLOBAL;
    }

    return uiAddrType;
}


/*****************************************************************************
    Func Name: IN6ADDR_GetUCAddrScope
 Date Created: 2010/9/19
       Author: zhouwenming 02256
  Description: 根据单播地址作用范围(scope)
        Input: pstAddr: IPv6单播地址
       Output: 无
       Return: scope，比如IN6ADDR_SCOPE_LINKLOCAL
      Caution: 调用者保证输入地址为单播地址
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
static inline UINT IN6ADDR_GetUCAddrScope(IN const IN6ADDR_S *pstAddr)
{
    UINT uiAddrScope;
    
    if( BOOL_TRUE == IN6ADDR_IsUnspecified(pstAddr) ) /* 地址为未指定全零地址，认为是保留范围类型 */
    {
        uiAddrScope = IN6ADDR_SCOPE_RESERVED;
    }
    else if( BOOL_TRUE == IN6ADDR_IsLoopback(pstAddr) )
    {
        uiAddrScope = IN6ADDR_SCOPE_INTFACELOCAL;
    }
    else if( BOOL_TRUE == IN6ADDR_IsLinkLocal(pstAddr) )
    {
        uiAddrScope = IN6ADDR_SCOPE_LINKLOCAL;
    }
    else
    {
        uiAddrScope = IN6ADDR_SCOPE_GLOBAL;
    }
    
    return uiAddrScope;
}


/* 描述了64bit接口ID的具体内容，注意接口ID是以网络字节序保存的 */
typedef struct tagIP6InterfaceId
{
    union
    {
        UCHAR    u6_InterfaceId8[8];
        USHORT   u6_InterfaceId16[4];
        UINT     u6_InterfaceId32[2];
    } u6_InterfaceId;
#define  ip6_ifid8    u6_InterfaceId.u6_InterfaceId8
#define  ip6_ifid16   u6_InterfaceId.u6_InterfaceId16
#define  ip6_ifid32   u6_InterfaceId.u6_InterfaceId32
}IP6_INTERFACEID_S;



/* 指定MAC地址结构，指定的源MAC和目的MAC都将使用这个结构。 */
struct snd_mac {
    u_char sm_mac[MAC_ADDR_LEN];   /* mac address */
};

/*
 * 设置发送VLAN ID、接收报文中的VLAN ID都使用下面这个结构来携带。
 * 结构中可以携带两个VLAN ID，第一层VLAN ID和第二层VLAN ID。如果某个VLAN
 * 没有对应的值，则使用无效值填充:VLAN_INVALID_VALUE。
 */
struct pkt_vlans {
    u_short pv_firstvlan;
    u_short pv_secondvlan;
};

struct snd_if {
    IF_INDEX si_if;             /* output if index */
    struct in_addr si_nxthop;   /* nexthop address */
    struct in_addr si_lcladdr;  /* local address */
};

struct ip_macaddr {
    u_char im_dstmac[MAC_ADDR_LEN];   /* dst mac address */
    u_char im_srcmac[MAC_ADDR_LEN];   /* src mac address */
};

struct pkt_pwid {
    UINT32 uiVsiIndex;
    UINT32 uiLinkID;
};

#define IP_L4MAXFILTERLEN  8
typedef struct snd_originpktfilter {
    u_char ucProto; /* IPPROTO_ICMP/IPPROTO_TCP/IPPROTO_UDP */
    u_char ucLen;   /* 过滤字段长度,最大为8字节。过滤位置从原始报文的ICMP/TCP/UDP头部开始 */
    u_char aucValue[IP_L4MAXFILTERLEN];  /* 过滤字段内容 */
    u_char aucMask[IP_L4MAXFILTERLEN];  /* 过滤字段内容 */
}ICMP_ORIGINPKT_FILTER_S;

typedef struct ip_filter {
    u_char ucOffset; /* 过滤字段距离四层头的偏移量 */
    u_char ucLen;    /* 过滤字段长度,最大为8字节 */
    u_char aucValue[IP_L4MAXFILTERLEN];  /* 过滤字段内容 */
    u_char aucMask[IP_L4MAXFILTERLEN];   /* 过滤字段掩码 */
}IP_FILTER_S;

#define ICMP_FILTER_SIZE       8
typedef struct icmp_filter {
    u_int32_t auiicmp_filt[ICMP_FILTER_SIZE];
}ICMP_FILTER_S;

#ifdef __KERNEL__
#define    ICMP_FILTER_SETPASSALL(filterp) \
do {  \
    int iIndex; u_char *pcbuf;  \
    pcbuf = (u_char *)filterp;  \
    for (iIndex = 0; iIndex < (int)(long)sizeof(struct icmp_filter); iIndex++) {  \
        pcbuf[iIndex] = 0xff;  \
    }  \
} while (BOOL_FALSE)

#define    ICMP_FILTER_SETBLOCKALL(filterp) \
    bzero(filterp, sizeof(struct icmp_filter))
#else /* _KERNEL */
#define    ICMP_FILTER_SETPASSALL(filterp) \
    memset(filterp, 0xff, sizeof(struct icmp_filter))
#define    ICMP_FILTER_SETBLOCKALL(filterp) \
    memset(filterp, 0x00, sizeof(struct icmp_filter))
#endif /* _KERNEL */

#define    ICMP_FILTER_SETPASS(type, filterp) \
    (((filterp)->auiicmp_filt[(type) >> 5]) |= ((u_int32_t)1 << ((type) & 31)))
#define    ICMP_FILTER_SETBLOCK(type, filterp) \
    (((filterp)->auiicmp_filt[(type) >> 5]) &= ~((u_int32_t)1 << ((type) & 31)))
#define    ICMP_FILTER_WILLPASS(type, filterp) \
    ((((filterp)->auiicmp_filt[(type) >> 5]) & ((u_int32_t)1 << ((type) & 31))) != 0)
#define    ICMP_FILTER_WILLBLOCK(type, filterp) \
    ((((filterp)->auiicmp_filt[(type) >> 5]) & ((u_int32_t)1 << ((type) & 31))) == 0)

/*
 * Defaults and limits for options
 */
#define IP_DEFAULT_MULTICAST_TTL        1
#define IP_DEFAULT_MULTICAST_LOOP       1

/* Structure used to describe IP options for IP_OPTIONS and IP_RETOPTS.
   The `ip_dst' field is used for the first-hop gateway when using a
   source route (this gets put into the header proper).  */
#ifndef    MAX_IPOPTLEN
#define MAX_IPOPTLEN    40
#endif

struct ip_opts
{
    struct in_addr ip_dst;  /* First hop; zero without source route.  */
    char ip_opts[MAX_IPOPTLEN];       /* Actually variable in size.  */
};

/*
 * Argument structure for IP_ADD_MEMBERSHIP and IP_DROP_MEMBERSHIP.
 */
struct ip_mreq {
    struct in_addr imr_multiaddr;  /* IP multicast address of group */
    IF_INDEX imr_interface;  /* J03845: interface index */
};

/*
 * Modified argument structure for IP_MULTICAST_IF, obtained from Linux.
 * This is used to specify an interface index for multicast sends, as
 * the IPv4 legacy APIs do not support this (unless IP_SENDIF is available).
 */
struct ip_mreqn {
    struct in_addr imr_multiaddr;  /* IP multicast address of group */
    struct in_addr imr_address;    /* local IP address of interface */
    IF_INDEX imr_ifindex;    /* Interface index; cast to uint32_t */
};

/*
 * Argument structure for IPv4 Multicast Source Filter APIs. [RFC3678]
 */
struct ip_mreq_source {
    struct  in_addr imr_multiaddr;  /* IP multicast address of group */
    struct  in_addr imr_sourceaddr; /* IP address of source */
    IF_INDEX imr_interface;  /* J03845: interface index */
};

/*
 * Argument structures for Protocol-Independent Multicast Source
 * Filter APIs. [RFC3678]
 */
struct group_req {
    IF_INDEX        gr_interface;   /* interface index */
    struct sockaddr_storage gr_group;   /* group address */
};

struct group_source_req {
    IF_INDEX        gsr_interface;  /* interface index */
    struct sockaddr_storage gsr_group;  /* group address */
    struct sockaddr_storage gsr_source; /* source address */
};


/*
 * IPv4 报文头数据结构定义,本结构只在INET协议栈内部使用。
 * 移植BSD协议栈时,在转发和协议栈两个模块中，使用不同的定义。
 * 与ipfw_pub.h中定义的结构中字段名字不一样。
 * 不同之处在于各个字段的名字,考虑到实际使用的时候都是传递结构地址。
 * 需要注意：在两个模块间的接口处，一旦出现了这个结构，需要显式的做强制类型转换！
 */
struct ip {
#if defined(__LITTLE_ENDIAN_BITFIELD)
    u_int8_t    ip_hl:4,           /* header length */
                ip_v:4;            /* version */
#elif defined(__BIG_ENDIAN_BITFIELD)
    u_int8_t    ip_v:4,            /* version */
                ip_hl:4;           /* header length */
#else
#error "Adjust your product defines"
#endif	
    u_char      ip_tos;            /* type of service */
    u_short     ip_len;            /* total length */
    u_short     ip_id;             /* identification */
    u_short     ip_off;            /* fragment offset field */
    u_char      ip_ttl;            /* time to live */
    u_char    ip_p;            /* protocol */
    u_short    ip_sum;            /* checksum */
    struct    in_addr ip_src,ip_dst;    /* source and dest address */
};

#ifndef    MAX_IPHEADLEN
#define MAX_IPHEADLEN    60
#endif

/*
 * Overlay for ip header used by other protocols (tcp, udp).
 */
struct ipovly {
    u_char    ih_x1[9];        /* (unused) */
    u_char    ih_pr;            /* protocol */
    u_short    ih_len;            /* protocol length */
    struct    in_addr ih_src;        /* source internet address */
    struct    in_addr ih_dst;        /* destination internet address */
};

/*
 * This is the real IPv4 pseudo header, used for computing the TCP and UDP
 * checksums. For the Internet checksum, struct ipovly can be used instead.
 * For stronger checksums, the real thing must be used.
 */
struct ippseudo {
    struct  in_addr ippseudo_src;   /* source internet address */
    struct  in_addr ippseudo_dst;   /* destination internet address */
    u_char      ippseudo_pad;   /* pad, must be zero */
    u_char      ippseudo_p; /* protocol */
    u_short     ippseudo_len;   /* protocol length */
};

/*
 * 接收到报文的接口信息。用户设置IP_RECVIF选项后，在接收报文时，会将报文的
 * 入接口和端口索引以控制信息的方式携带给用户。UDP/RAWIP支持。
 */
struct rcvifinfo
{
    IF_INDEX rif_ifindex;    /* 报文入接口索引 */
    IF_INDEX rif_portindex;  /* 报文入端口索引，如果不是从二层端口上送的，设置为0 */
};

/* INET消息类型定义 */
enum inet_msgtype
{
    INET_INPCB_EVENT = 0,  /* INPCB数据的事件消息 */
    INET_INPCB_MSG,        /* INPCB数据的处理消息 */
    INET_PORT_EVENT,       /* 端口数据的事件消息 */
    INET_PORT_MSG,         /* 端口数据的处理消息 */
    INET_CFG_MSG,          /* 命令行配置消息 */
    INET_MCAST_MSG,        /* G03597: 组播消息 */
    INET_NSR_MSG,          /* NSR消息 */
    INET_MSG_BUTT
};

/*
 * Structures.
 */
/* 通信结构,保持1字节对齐 */
#pragma pack(1)
ISSU typedef struct inet_msghead
{
    u_short maintype;    /* 主消息类型, TCP\UDP\RAWIP模块分发到三级子模块 */
    u_short subtype;     /* 子消息类型 */
    u_int   length;      /* 消息长度, 包含头长度 */
}INET_MSGHEAD_S;
#pragma pack()

#define IN_MSGHEAD_LEN  (sizeof(struct inet_msghead))

/* 封装INET通信头信息的宏定义 */
#define IN_ENCAP_HEAD(msg_M, mtype_M, stype_M, len_M) \
    (msg_M)->maintype = htons(mtype_M); \
    (msg_M)->subtype = htons(stype_M); \
    (msg_M)->length = htonl(len_M);

/* 封装INET通信头信息的宏定义 */
#define IN_DECAP_HEAD(msg_M, mtype_M, stype_M, len_M) \
    mtype_M = ntohs((msg_M)->maintype); \
    stype_M = ntohs((msg_M)->subtype); \
    len_M = ntohl((msg_M)->length);

/* INPCB 索引 */
ISSU typedef struct {
    u_char index[8];
} INPCB_INDEX;

#define IN_PCBINDEX_STRLEN 16 /* "FFFFFFFFFFFFFFFF" */

/*
 * PCB with AF_INET6 null bind'ed laddr can receive AF_INET input packet.
 * So, AF_INET6 null laddr is also used as AF_INET null laddr, by utilizing
 * the following structure.
 */
struct in_addr_4in6 {
    u_int32_t    ia46_pad32[3];
    struct    in_addr    ia46_addr4;
};

/* 定义每个协议控制块显示摘要信息,应该8字节对齐 */
#define IN_BRIEF_NUM   30

ISSU typedef struct tagIN_ShowBrf
{
    union {
        /* foreign host table entry */
        struct    in_addr_4in6  stAddr4;
        struct    in6_addr stAddr6;
    } unLocal;
#define ip6_laddr unLocal.stAddr6
#define ip_laddr  unLocal.stAddr4.ia46_addr4
    union {
        /* foreign host table entry */
        struct    in_addr_4in6  stAddr4;
        struct    in6_addr stAddr6;
    } unRemote;
#define ip6_faddr unRemote.stAddr6
#define ip_faddr  unRemote.stAddr4.ia46_addr4

    INPCB_INDEX stPcbIndex;/* inpcb index */

    /* Prototol specific field */
    USHORT usProtocol;   /* protocol, RAWIP only */
    USHORT usAuthentication;        /* 使能MD5或keychain校验? 1 Yes 0 No */
    USHORT usLocalPort;  /* local port number, UDP/TCP only */
    USHORT usRemotePort; /* foreign port, UDP/TCP only */

    UINT uiState;        /* tcp connection state */
    UINT uiBoard;        /* logical board id */

    union {
        struct {
            VRF_INDEX usSndVrf; /* send vpn index */
            VRF_INDEX usRcvVrf;  /* receive vpn index */
        } stVrf;

        UCHAR aucReserved[8];
    } unVrf;
    #define SndVrf unVrf.stVrf.usSndVrf
    #define RcvVrf unVrf.stVrf.usRcvVrf
} IN_SHOWBRF_S;

/* 定义每个socket的详细消息,应保证8字节对齐 */
#define IN_VERBOSE_NUM 10
ISSU typedef struct tagIN_ShowVbs
{
    IN_SHOWBRF_S  stBriefInfo;

    UINT     uiInp_Flags;               /* generic IP/datagram flags */
    UCHAR    ucInp_Proto;               /* protocol number */
    UCHAR    ucInp_Vflag;               /* IP version flag (v4/v6) */
    UCHAR    ucInp_Ttl;                 /* time to live proto */
    UCHAR    ucInp_Minttl;              /* minimum TTL or drop */

    SO_SHOWINFO_S stSoShowInfo;         /* socket详细信息 */
    UINT     uiInp_ExtFlags;            /* generic IP/datagram extern flags */
    UINT     uiStandbyBoard;            /* standby board id */
    UINT     uiProFlags;                /* protocol options */
    USHORT   usInp_NSRState;            /* NSR state*/
    UCHAR    ucInp_NSRRole;             /* NSR role*/
} IN_SHOWVBS_S;

/* 定义内核的显示应答消息。对于显示摘要信息命令，
   后面跟随IN_SHOWBRF_S结构；对于显示详细信息命令，
   后面跟随IN_SHOWVBS_S结构。 */
typedef struct tagINShowInfo
{
    UINT uiEnd;                         /* 是否结束,0 未结束，1 已结束 */
    UINT uiShowNum;                     /* 本次显示的socket数目 */
} IN_SHOWINFO_S;

/* 定义获取INPCB表项数目消息 */
ISSU typedef struct tagINShowNum
{
    UINT uiInpcbNum;                    /* 系统的inpcb数目 */
    UINT uiPpcbNum;                     /* tcpcb的数目 */
} IN_SHOWNUM_S;

/* 共享库发给socket内核线程的请求消息 */
ISSU typedef struct tagIN_ShowReq
{
    INPCB_INDEX  stPcbIndex;  /* pcb索引 */
    
    UINT  uiFirst;            /* 是否第一次遍历 */
    UINT  uiReserved;         /* 保留字段,64位对齐 */
} IN_SHOWREQ_S;

/* 
 * 每个端口段资源里面的端口数(INPORT_UINTS_ONERES*8*4), uint存储 
 */
#define INPORT_PORT_PERSEG   64
#define INPORT_PORT_PERUINT  ((u_int)sizeof(u_int) * 8)
#define INPORT_UINTS_ONERES  (INPORT_PORT_PERSEG / INPORT_PORT_PERUINT)
#define INPORT_RES_NUM       ((u_int)(64 * 1024) / (INPORT_PORT_PERSEG))
#define INPORT_PORT_START    1024
#define INPORT_PORT_STOP     65535
#define INPORT_RES_START     (INPORT_PORT_START / INPORT_PORT_PERSEG)

/* 端口资源管理数据结构 */
struct inport_resource
{
    u_int ipr_state;        /* 0, 未分配; 1, 已分配; 2, 待回收 */
    u_int ipr_slot;         /* 只有主用板记录有用 */
    u_int ipr_resource[INPORT_UINTS_ONERES];  /* 对应4*8*INPORT_UINTS_ONERES个端口 */
};

/* 单一板端口资源数据结构 */
struct inport_info
{
    u_int inport_resindex;
    u_int lastint;
    u_int lastbit;
    u_int ipr_resource[INPORT_UINTS_ONERES];
};

/*
 * Ports < IPPORT_RESERVED are reserved for
 * privileged processes (e.g. root).         (IP_PORTRANGE_LOW)
 */
#define IPPORT_RESERVED     1024

/*
 * Options for use with [gs]etsockopt at the IP level.
 * First word of comment is data type; bool is stored in int.
 */
#define IP_OPTIONS          1    /* buf/ip_opts; set/get IP options */
#define IP_HDRINCL          2    /* int; header is included with data */
#define IP_TOS              3    /* int; IP type of service and preced. */
#define IP_TTL              4    /* int; IP time to live */
#define IP_RECVOPTS         5    /* bool; receive all IP opts w/dgram */
#define IP_RECVRETOPTS      6    /* bool; receive IP opts for response */
#define IP_RECVDSTADDR      7    /* bool; receive IP dst addr w/dgram */
#define IP_SENDSRCADDR      IP_RECVDSTADDR /* cmsg_type to set src addr */
#define IP_RETOPTS          8    /* ip_opts; set/get IP options */
#define IP_MULTICAST_IF     9    /* struct in_addr *or* struct ip_mreqn;
                      * set/get IP multicast i/f  */
#define IP_MULTICAST_TTL    10   /* u_char; set/get IP multicast ttl */
#define IP_MULTICAST_LOOP   11   /* u_char; set/get IP multicast loopback */
#define IP_ADD_MEMBERSHIP   12   /* ip_mreq; add an IP group membership */
#define IP_DROP_MEMBERSHIP  13   /* ip_mreq; drop an IP group membership */
#define IP_MULTICAST_VIF    14   /* set/get IP mcast virt. iface */
#define IP_RSVP_ON          15   /* enable RSVP in kernel */
#define IP_RSVP_OFF         16   /* disable RSVP in kernel */
#define IP_RSVP_VIF_ON      17   /* set RSVP per-vif socket */
#define IP_RSVP_VIF_OFF     18   /* unset RSVP per-vif socket */
#define IP_PORTRANGE        19   /* int; range to choose for unspec port */
#define IP_RECVIF           20   /* bool; receive reception if w/dgram */
/* for IPSEC */
#define IP_IPSEC_POLICY     21   /* int; set/get security policy */
#define IP_FAITH            22   /* bool; accept FAITH'ed connections */

#define IP_ONESBCAST        23   /* bool: send all-ones broadcast */
#define IP_SNDBYICMPSRC     24   /* int: specify source address of send icmp packets*/

#define IP_FW_TABLE_ADD     40   /* add entry */
#define IP_FW_TABLE_DEL     41   /* delete entry */
#define IP_FW_TABLE_FLUSH   42   /* flush table */
#define IP_FW_TABLE_GETSIZE 43   /* get table size */
#define IP_FW_TABLE_LIST    44   /* list table contents */

#define IP_FW_ADD           50   /* add a firewall rule to chain */
#define IP_FW_DEL           51   /* delete a firewall rule from chain */
#define IP_FW_FLUSH         52   /* flush firewall rule chain */
#define IP_FW_ZERO          53   /* clear single/all firewall counter(s) */
#define IP_FW_GET           54   /* get entire firewall rule chain */
#define IP_FW_RESETLOG      55   /* reset logging counters */

#define IP_FW_NAT_CFG           56   /* add/config a nat rule */
#define IP_FW_NAT_DEL           57   /* delete a nat rule */
#define IP_FW_NAT_GET_CONFIG    58   /* get configuration of a nat rule */
#define IP_FW_NAT_GET_LOG       59   /* get log of a nat rule */

#define IP_DUMMYNET_CONFIGURE   60   /* add/configure a dummynet pipe */
#define IP_DUMMYNET_DEL     61   /* delete a dummynet pipe from chain */
#define IP_DUMMYNET_FLUSH   62   /* flush dummynet */
#define IP_DUMMYNET_GET     64   /* get entire dummynet pipes */

#define IP_RECVTTL          65   /* bool; receive IP TTL w/dgram */
#define IP_MINTTL           66   /* minimum TTL for packet or drop */
#define IP_DONTFRAG         67   /* don't fragment packet */

#define IP_RECVTOS          68   /* receive IP TOS */

/* IPv4 Source Filter Multicast API [RFC3678] */
#define IP_ADD_SOURCE_MEMBERSHIP    70   /* join a source-specific group */
#define IP_DROP_SOURCE_MEMBERSHIP   71   /* drop a single source */
#define IP_BLOCK_SOURCE         72   /* block a source */
#define IP_UNBLOCK_SOURCE       73   /* unblock a source */

/* The following option is private; do not use it from user applications. */
#define IP_MSFILTER         74   /* set/get filter list */

/* Protocol Independent Multicast API [RFC3678] */
#define MCAST_JOIN_GROUP        80   /* join an any-source group */
#define MCAST_LEAVE_GROUP       81   /* leave all sources for group */
#define MCAST_JOIN_SOURCE_GROUP     82   /* join a source-specific group */
#define MCAST_LEAVE_SOURCE_GROUP    83   /* leave a single source */
#define MCAST_BLOCK_SOURCE      84   /* block a source */
#define MCAST_UNBLOCK_SOURCE        85   /* unblock a source */

/* Begin: Added by J03845 for Socket-Dist-Project on 2009.6.22 */
#define IP_SNDBYDSTMAC      86  /* specify destnation MAC address of send packets */
#define IP_SNDDATAIF        87  /* specify output interface */
#define IP_ROUTER_ALERT     88  /* recieve router alert packet */
#define IP_PROTOCOL_PACKET  89  /* protocol packet */
/* End: Added by J03845 for Socket-Dist-Project on 2009.6.22 */
/*
 * J03845: Added for LPD02653 on 2010-7-21.
 * VRRP和DHCP发送报文时可能需要指定报文源、目的MAC地址，同时还指定发送报文VLAN
 * ID。这种设置都是每报文的，所以通过control信息方式携带并最终设置到MBUF上。
 * 另外，DHCP在接收报文时需要获取报文中的VLAN ID，这个也将通过控制信息携带给
 * 用户态。
 * 注意: 因为现在对发送报文MAC、VLAN ID的设置都是每报文的，所以暂时不支持
 * 通过setsockopt方式设置源MAC和VLAN ID。后续如果有需求再增加。
 */
#define IP_SNDBYSRCMAC      90  /* specify source MAC address of send packets */
#define IP_PKTVLANID        91  /* send packet's vlan ids */
#define IP_RCVVLANID        92  /* recv packet's vlan ids */
#define IP_ACLFILTER        93  /* acl filter */
#define IP_RCVMACADDR       94  /* recv packet's mac address info */
#define IP_SNDBYLSPV        95  /* send by lspv */
#define IP_MPLSFLAG         96  /* mpls flag */
#define IP_ACLFILTER_MAC     97  /* mac acl filter */
#define IP_SYNCPCB          98  /* wait until pcb synced */
/* 提供给组播使用, 通过获取入报文的PVC索引下发组播规则和控制发送报文的出PVC */
#define IP_SNDPVCIDX        99   /* send packet's pvc idx */
#define IP_RCVPVCIDX        100  /* recv packet's pvc idx */
#define IP_SNDPWID          101  /* send packet's pw id */
#define IP_RCVPWID          102  /* recv packet's pw id */
#define IP_SELECTMATCHSRCBYFIB 103  /* only when dst is reachable, select matched source address */
/* for ping/tracert, recv icmp error packet to know about errors in handling ping request  */
#define IP_RCVICMPERR       104
#define IP_ICMPFILTER       105
#define IP_FILTER           106
/* if acl not match, don't drop connect */
#define IP_DONTDROP         107

#define IP_DSCP2TOS(a)              (((a) & 0x3F) << 2)
#define IP_TOS2DSCP(a)              (((a) >> 2) & 0x3F)
#define IP_PRECEDENCE2TOS(a)        (((a) & 0x07) << 5)
#define IP_TOS2PRECEDENCE(a)        (((a) >> 5) & 0x07)

#ifdef __KERNEL__

/******************************************************************************
 *                    这里定义内核使用的宏和结构体                            *
 ******************************************************************************/

/*
 * Filter modes; also used to represent per-socket filter mode internally.
 */
#define MCAST_UNDEFINED 0   /* fmode: not yet defined */
#define MCAST_INCLUDE   1   /* fmode: include these source(s) */
#define MCAST_EXCLUDE   2   /* fmode: exclude these source(s) */

/*
 * The imo_membership vector for each socket is now dynamically allocated at
 * run-time, bounded by USHRT_MAX, and is reallocated when needed, sized
 * according to a power-of-two increment.
 */
#define IP_MIN_MEMBERSHIPS  31
#define IP_MAX_MEMBERSHIPS  (1024 * 1024 - 1)
#define IP_MAX_SOURCE_FILTER    1024    /* # of filters per socket, per group */

int inet_pton(int af,const char *src,void *dst);
const char * inet_ntop(int af,const void *src,char *dst,socklen_t len);
char * inet_ntoa(struct in_addr ina);
char * inet_ntoa_r(struct in_addr ina, char *buf);
int ip_ctloutput(struct socket *so, struct sockopt *sopt);
void ip_init(void);

extern    u_char    inetctlerrmap[];
extern struct proc_dir_entry *inet_procroot;
extern struct domain inetdomain;


n_time   iptime(void);


/*
 * Default local port range, used by both IP_PORTRANGE_DEFAULT
 * and IP_PORTRANGE_HIGH.
 */
#define IPPORT_HIFIRSTAUTO  49152
#define IPPORT_HILASTAUTO   65535

/*
 * Scanning for a free reserved port return a value below IPPORT_RESERVED,
 * but higher than IPPORT_RESERVEDSTART.  Traditionally the start value was
 * 512, but that conflicts with some well-known-services that firewalls may
 * have a fit if we use.
 */
#define IPPORT_RESERVEDSTART    600

#define IPPORT_MAX      65535
/*
 * Argument for IP_PORTRANGE:
 * - which range to search when port is unspecified at bind() or connect()
 */
#define IP_PORTRANGE_DEFAULT    0   /* default range */
#define IP_PORTRANGE_HIGH   1   /* "high" - request firewall bypass */
#define IP_PORTRANGE_LOW    2   /* "low" - vouchsafe security */

void ip_protoinput(u_char proto, struct mbuf *m, int off0);

void ip_protoctlinput(u_char proto, int cmd, struct sockaddr *sa, void *ctlparam);

void ip_l4mbuftrsend(struct mbuf *m, u_int dstslot);
u_long inet_autorun(void *dummy);
#else
/******************************************************************************
 *                这里定义用户态使用的宏、结构体和接口                        *
 ******************************************************************************/

/* 用户态使用的接口 */
/* Convert Internet host address from numbers-and-dots notation in CP
   into binary data in network byte order. */
extern in_addr_t inet_addr (__const char *__cp);

/* Return the local host address part of the Internet address in IN. */
extern in_addr_t inet_lnaof (struct in_addr __in);

/* Make Internet host address in network byte order by combining the
   network number NET with the local address HOST. */
extern struct in_addr inet_makeaddr (in_addr_t __net, in_addr_t __host);

/* Return network number part of the Internet address IN. */
extern in_addr_t inet_netof (struct in_addr __in);

/* Extract the network number in network byte order from the address
   in numbers-and-dots natation starting at CP. */
extern in_addr_t inet_network (__const char *__cp);  

/* Convert from presentation format of an Internet number in buffer
   starting at CP to the binary network format and store result for
   interface type AF in buffer starting at BUF.  */
extern int inet_pton (int __af, __const char *__restrict __cp,
              void *__restrict __buf);

/* Convert a Internet address in binary network format for interface
   type AF in buffer starting at CP to presentation form and place
   result in buffer of length LEN astarting at BUF.  */
extern __const char *inet_ntop (int __af, __const void *__restrict __cp,
                char *__restrict __buf, socklen_t __len);


/* Convert Internet number in IN to ASCII representation.  The return value
   is a pointer to an internal array containing the string.  */
extern char *inet_ntoa (struct in_addr __in);

/* Convert Internet host address from numbers-and-dots notation in CP
   into binary data and store the result in the structure INP.  */
extern int inet_aton (__const char *__cp, struct in_addr *__inp);

#endif /* __KERNEL__ */


/* 地址通用结构中存储的地址转成字符串的最大长度，包括'\0' */
#define INET_ADDR_STR_LEN      INET6_ADDRSTRLEN

/* IPv4和IPv6使用的通用IP地址结构. 其中的地址建议存储为网络序 */
typedef struct tagINET_ADDR
{
    USHORT   usFamily;             /* 地址协议族(AF_INET/AF_INET6) */
    USHORT   usReserved;           /* 保留字段 */
    union
    {
        IN6ADDR_S stIP6Addr;
        INADDR_S  stIP4Addr;
    } un_addr;                      /* IP地址字段 */

    #define uIP6_Addr       un_addr.stIP6Addr
    #define uIP4_Addr       un_addr.stIP4Addr
}INET_ADDR_S;

/* 地址通用结构成员相关宏 */
#define INET_ADDR_FAMILY(pstAddrM)      ((pstAddrM)->usFamily)
#define INET_ADDR_IP4ADDR(pstAddrM)     ((pstAddrM)->uIP4_Addr)
#define INET_ADDR_IP4ADDRUINT(pstAddrM) INET_ADDR_IP4ADDR(pstAddrM).s_addr
#define INET_ADDR_IP6ADDR(pstAddrM)     ((pstAddrM)->uIP6_Addr)
#define INET_ADDR_IP6ADDRUINT(pstAddrM) INET_ADDR_IP6ADDR(pstAddrM).s6_addr32

/* 地址通用结构相关编程接口 */
/*****************************************************************************
  函 数 名: INET_ADDR_Copy
  创建日期: 2010年9月13日 
  作    者: z02256
  函数描述: 拷贝源地址到指定目的地址
  输入参数: IN INET_ADDR_S *pstSrcAddr : 源地址
  输出参数: OUT INET_ADDR_S *pstDestAddr : 目的地址
    返回值: 无
    注意点: 无
------------------------------------------------------------------------------
    修改历史                                                                  
    日期        姓名             描述                                         
  --------------------------------------------------------------------------
                                                                              
*****************************************************************************/
static inline VOID INET_ADDR_Copy(OUT INET_ADDR_S *pstDestAddr, IN const INET_ADDR_S *pstSrcAddr)
{
    UINT *puiDestAddr;
    const UINT *puiSrcAddr;
    
    INET_ADDR_FAMILY(pstDestAddr) = INET_ADDR_FAMILY(pstSrcAddr);

    if(AF_INET == INET_ADDR_FAMILY(pstSrcAddr))
    {
        INET_ADDR_IP4ADDR(pstDestAddr) = INET_ADDR_IP4ADDR(pstSrcAddr);
    }
    else
    {
        puiDestAddr = INET_ADDR_IP6ADDR(pstDestAddr).s6_addr32;
        puiSrcAddr = INET_ADDR_IP6ADDR(pstSrcAddr).s6_addr32;
        
        puiDestAddr[0] = puiSrcAddr[0];
        puiDestAddr[1] = puiSrcAddr[1];
        puiDestAddr[2] = puiSrcAddr[2];
        puiDestAddr[3] = puiSrcAddr[3];
    }

    return;
}

/*****************************************************************************
  函 数 名: INET_ADDR_IP2Str
  创建日期: 2010年9月13日 
  作    者: z02256
  函数描述: IP地址结构转换为字符串
  输入参数: IN INET_ADDR_S *pstAddr : 需要转换为字符串的通用IP地址结构
            IN UINT uiStrLen : 字符串缓冲区长度，包括'\0'；一般输入INET_ADDR_STR_LEN
  输出参数: 拼装后的字符串
    返回值: ERROR_SUCCESS: 成功
            ERROR_FAILED: 失败
    注意点: 本函数只支持网络序地址的转换
------------------------------------------------------------------------------
    修改历史                                                                  
    日期        姓名             描述                                         
  --------------------------------------------------------------------------
                                                                              
*****************************************************************************/
static inline ULONG INET_ADDR_IP2Str(IN const INET_ADDR_S *pstAddr, IN UINT uiStrLen, OUT CHAR *pcStr)
{
    if(NULL != inet_ntop(INET_ADDR_FAMILY(pstAddr), (VOID *)&(pstAddr->un_addr), pcStr, uiStrLen))
    {
        return ERROR_SUCCESS;
    }
    else
    {
        return ERROR_FAILED;
    }
}

/*****************************************************************************
  函 数 名: INET_ADDR_Str2IP
  创建日期: 2010年8月31日 
  作    者: h03198
  函数描述: 将字符格式转换为IP地址通用结构
  输入参数: IN USHORT usFamily : 地址族(AF_INET/AF_INET6)
            IN CHAR *pcStr : 需要转换的字符串指针，以'\0'结尾
  输出参数: OUT INET_ADDR_S *pstAddr : 构造的IP地址
    返回值: ERROR_SUCCESS: 成功
            ERROR_FAILED: 失败
    注意点: 1、IP地址通用结构不负责申请内存，保存IP地址通用结构的内存有使用者负责
            2、本函数只支持转换为网络序地址
------------------------------------------------------------------------------
    修改历史                                                                  
    日期        姓名             描述                                         
  --------------------------------------------------------------------------
                                                                              
*****************************************************************************/
static inline ULONG INET_ADDR_Str2IP(IN USHORT usFamily, IN const CHAR *pcStr, OUT INET_ADDR_S *pstAddr)
{
    if( inet_pton(usFamily, pcStr, (VOID *)&(pstAddr->un_addr)) > 0 )
    {
        INET_ADDR_FAMILY(pstAddr) = usFamily;
        return ERROR_SUCCESS;
    }
    else
    {
        return ERROR_FAILED;
    }
}

/*****************************************************************************
  函 数 名: INET_ADDR_Cmp
  创建日期: 2010年9月13日 
  作    者: z02256
  函数描述: 比较IP地址通用结构中包含的地址字段的大小(网络序)
  输入参数: IN INET_ADDR_S *pstAddr1 : 待比较地址
            IN INET_ADDR_S *pstAddr2 : 待比较地址
  输出参数: 无
    返回值: 大于0: 地址1大于地址2
            小于0: 地址1小于地址2
            等于0: 地址1等于地址2
    注意点: 只比较相同地址族的地址通用结构
------------------------------------------------------------------------------
    修改历史                                                                  
    日期        姓名             描述                                         
  --------------------------------------------------------------------------
                                                                              
*****************************************************************************/
static inline INT INET_ADDR_Cmp(IN const INET_ADDR_S *pstAddr1, IN const INET_ADDR_S *pstAddr2)
{
    INT iRet = 0;
    
    if(AF_INET == INET_ADDR_FAMILY(pstAddr1))
    {
        UINT32 uiIP4Addr1 = ntohl(INET_ADDR_IP4ADDR(pstAddr1).s_addr);
        UINT32 uiIP4Addr2 = ntohl(INET_ADDR_IP4ADDR(pstAddr2).s_addr);

        if (uiIP4Addr1 > uiIP4Addr2)
        {
            iRet = 1;
        }
        else if(uiIP4Addr1 < uiIP4Addr2)
        {
            iRet = -1;
        }
        else
        {
            iRet = 0;
        }
    }
    else
    {
        iRet = IN6ADDR_Cmp(&(INET_ADDR_IP6ADDR(pstAddr1)), &(INET_ADDR_IP6ADDR(pstAddr2)));
    }

    return iRet;
}


/***************************************************************************/
/*                        IPv4协议使能宏和数据结构定义                     */
/***************************************************************************/

/* 使能上送CPU模块宏 */
#define PROTOCOL_ENABLE_GLOBAL      0x0           /* 协议全局使能 */
#define PROTOCOL_ENABLE_INTERFACE   0x1           /* 协议接口使能 */

#define PROTOCOL_FUNC_MINTTL    0x1
#define PROTOCOL_INVALID_VALUE      0xFFFFFFFF    /* 无效值 */

typedef struct tagIP_PROTOCOLPACKETLIST
{
    UINT uiTag;
    UINT uiGlobal;
    UINT uiValue;
    UINT uiProtocolType;
    UINT uiDstIP;
    UINT uiMaskLen;
    UINT uiPara1;
    UINT uiPara2;
    IF_INDEX ifIndex;
}IP_PROTOCOL_PACKET_LIST_S;

typedef struct tagIP_ENABLE_PROTOCOL
{
    UINT uiTag;
    UINT uiGlobal;
    UINT uiAction;
    UINT uiProtocolType;
    UINT uiFunction;
    UINT uiDstIP;
    UINT uiMaskLen;
    UINT uiPara1;
    UINT uiPara2;
    UINT uiPara3;
    IF_INDEX ifIndex;
   VRF_INDEX vrfIndex;
}IP_ENABLE_PROTOCOL_S;
#ifdef __KERNEL__

/***************************************************************************/
/*                         内核对外接口定义                                */
/***************************************************************************/
ULONG KPROEN_AutoRun(INOUT LPVOID pStartContext);
ULONG KPROEN_MdcCallBack(IN UINT uiEvent, IN VOID *pData);

#else

/***************************************************************************/
/*                        用户态对外接口定义                               */
/***************************************************************************/
ULONG IP_EnableProtocolToCPU(IN UINT uiTag, 
                            IN UINT uiGlobal,
                            IN UINT uiValue, 
                            IN UINT uiProtocolType,
                            IN UINT uiDstIP, 
                            IN UINT uiMaskLen,
                            IN UINT uiPara1, 
                            IN UINT uiPara2,
                            IN IF_INDEX ifIndex);

ULONG IP_EnableProtocolPacketListToCPU(IN UINT32 uiBatchNum, 
                                  INOUT IP_PROTOCOL_PACKET_LIST_S *pstBatchData);

ULONG IP_EnableProtocolToCpuEx(IN UINT32 uiNum, IN const IP_ENABLE_PROTOCOL_S *pstData, IN UINT uiKeyLen);
#endif

#ifdef __cplusplus
}
#endif  /* end of __cplusplus */

#endif /* _SYS_IN_H_ */
