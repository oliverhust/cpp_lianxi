/*****************************************************************************
 Copyright (c) 2010, Hangzhou H3C Technologies Co., Ltd. All rights reserved.
------------------------------------------------------------------------------
                              in6.h
  Project Code: Comware V7
   Module Name: TCP6/IP6
  Date Created: 2010-9-10
        Author: 
   Description: TCP6/IP6协议栈使用的公共头文件，此文件中定义了用户态和内核公用的
                宏、结构体等定义。
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
#ifndef _SYS_IN6_H_
#define _SYS_IN6_H_

#ifdef  __cplusplus
extern "C"{
#endif

#ifndef _SYS_IN_H_
#error ("you must include in.h first!" )
#endif


/******************************************************************************
 *               这里定义用户态和内核态公用的宏以及结构体                     *
 ******************************************************************************/
/*
 * Options for use with [gs]etsockopt at the IPV6 level.
 * First word of comment is data type; bool is stored in int.
 */
/* no hdrincl */
#if 0 /* the followings are relic in IPv4 and hence are disabled */
#define IPV6_OPTIONS        1  /* buf/ip6_opts; set/get IP6 options */
#define IPV6_RECVOPTS        5  /* bool; receive all IP6 opts w/dgram */
#define IPV6_RECVRETOPTS    6  /* bool; receive IP6 opts for response */
#define IPV6_RECVDSTADDR    7  /* bool; receive IP6 dst addr w/dgram */
#define IPV6_RETOPTS        8  /* ip6_opts; set/get IP6 options */
#endif
#define IPV6_SOCKOPT_RESERVED1    3  /* reserved for future use */
#define IPV6_UNICAST_HOPS    4  /* int; IP6 hops */
#define IPV6_MULTICAST_IF    9  /* u_int; set/get IP6 multicast i/f  */
#define IPV6_MULTICAST_HOPS    10 /* int; set/get IP6 multicast hops */
#define IPV6_MULTICAST_LOOP    11 /* u_int; set/get IP6 multicast loopback */
#define IPV6_JOIN_GROUP        12 /* ip6_mreq; join a group membership */
#define IPV6_LEAVE_GROUP    13 /* ip6_mreq; leave a group membership */
#define IPV6_PORTRANGE        14 /* int; range to choose for unspec port */
#define ICMP6_FILTER        18 /* icmp6_filter; icmp6 filter */
/* RFC2292 options */
#define IPV6_2292PKTINFO    19 /* bool; send/recv if, src/dst addr */
#define IPV6_2292HOPLIMIT    20 /* bool; hop limit */
#define IPV6_2292NEXTHOP    21 /* bool; next hop addr */
#define IPV6_2292HOPOPTS    22 /* bool; hop-by-hop option */
#define IPV6_2292DSTOPTS    23 /* bool; destinaion option */
#define IPV6_2292RTHDR        24 /* bool; routing header */
#define IPV6_2292PKTOPTIONS    25 /* buf/cmsghdr; set/get IPv6 options */

#define IPV6_CHECKSUM        26 /* int; checksum offset for raw socket */
#define IPV6_V6ONLY        27 /* bool; make AF_INET6 sockets v6 only */
#ifndef _KERNEL
#define IPV6_BINDV6ONLY        IPV6_V6ONLY
#endif

#if 1 /* IPSEC */
#define IPV6_IPSEC_POLICY    28 /* struct; get/set security policy */
#endif /* IPSEC */

#define IPV6_FAITH        29 /* bool; accept FAITH'ed connections */

#if 1 /* IPV6FIREWALL */
#define IPV6_FW_ADD        30 /* add a firewall rule to chain */
#define IPV6_FW_DEL        31 /* delete a firewall rule from chain */
#define IPV6_FW_FLUSH        32 /* flush firewall rule chain */
#define IPV6_FW_ZERO        33 /* clear single/all firewall counter(s) */
#define IPV6_FW_GET        34 /* get entire firewall rule chain */
#endif

/* new socket options introduced in RFC3542 */
#define IPV6_RTHDRDSTOPTS    35 /* ip6_dest; send dst option before rthdr */

#define IPV6_RECVPKTINFO    36 /* bool; recv if, dst addr */
#define IPV6_RECVHOPLIMIT    37 /* bool; recv hop limit */
#define IPV6_RECVRTHDR        38 /* bool; recv routing header */
#define IPV6_RECVHOPOPTS    39 /* bool; recv hop-by-hop option */
#define IPV6_RECVDSTOPTS    40 /* bool; recv dst option after rthdr */
#define IPV6_RECVRTHDRDSTOPTS    41 /* bool; recv dst option before rthdr */

#define IPV6_USE_MIN_MTU    42 /* bool; send packets at the minimum MTU */
#define IPV6_RECVPATHMTU    43 /* bool; notify an according MTU */

#define IPV6_PATHMTU        44 /* mtuinfo; get the current path MTU (sopt),
                      4 bytes int; MTU notification (cmsg) */
#if 0 /*obsoleted during 2292bis -> 3542*/
#define IPV6_REACHCONF        45 /* no data; ND reachability confirm
                      (cmsg only/not in of RFC3542) */
#endif

/* more new socket options introduced in RFC3542 */
#define IPV6_PKTINFO        46 /* in6_pktinfo; send if, src addr */
#define IPV6_HOPLIMIT        47 /* int; send hop limit */
#define IPV6_NEXTHOP        48 /* sockaddr; next hop addr */
#define IPV6_HOPOPTS        49 /* ip6_hbh; send hop-by-hop option */
#define IPV6_DSTOPTS        50 /* ip6_dest; send dst option befor rthdr */
#define IPV6_RTHDR        51 /* ip6_rthdr; send routing header */
#if 0
#define IPV6_PKTOPTIONS        52 /* buf/cmsghdr; set/get IPv6 options */
                   /* obsoleted by RFC3542 */
#endif

#define IPV6_RECVTCLASS        57 /* bool; recv traffic class values */

#define IPV6_AUTOFLOWLABEL    59 /* bool; attach flowlabel automagically */

#define IPV6_TCLASS        61 /* int; send traffic class value */
#define IPV6_DONTFRAG        62 /* bool; disable IPv6 fragmentation */

#define IPV6_PREFER_TEMPADDR    63 /* int; prefer temporary addresses as
                    * the source address.
                    */

#define IPV6_IPSEC_PROFILE      64 /* char[IPSEC_PROFILE_NAME_LEN + 1]; set/recvmsg profile name */

#define IPV6_SNDBYICMPSRC       65 /* int: specify source address of send  icmpv6 packets */

/*
 * The following option is private; do not use it from user applications.
 * It is deliberately defined to the same value as IP_MSFILTER.
 */
#define    IPV6_MSFILTER        74 /* struct __msfilterreq;
                    * set/get multicast source filter list.
                    */
                    
#define IPV6_RCVVLANID          100
#define IPV6_RECVIPSECINFO      101
#define IPV6_PROTOCOL_PACKET    102

#define IPV6_SNDBYDSTMAC        103
#define IPV6_SNDBYSRCMAC        104
#define IPV6_PKTVLANID          105

#define IPV6_ACLFILTER          106

#define IPV6_SNDBYSRCADDR       107
#define IPV6_RCVMACADDR         108

#define IPV6_ACLFILTER_MAC      109

#define IPV6_SYNCPCB            110  /* wait until pcb synced */

/* 提供给组播使用, 通过获取入报文的PVC索引下发组播规则和控制发送报文的出PVC */
#define IPV6_SNDPVCIDX          111   /* send packet's pvc idx */
#define IPV6_RCVPVCIDX          112   /* recv packet's pvc idx */
#define IPV6_SNDPWID            113   /* send packet's pw id */
#define IPV6_RCVPWID            114   /* recv packet's pw id */
#define IPV6_RECVIF             115   /* bool; receive reception if w/dgram */
#define IPV6_SELECTMATCHSRCBYFIB 116  /* only when dst is reachable, select matched source address */
/* for ping6/tracert6, recv icmpv6 error packet to know about errors in handling ping6 request  */
#define ICMP6_RCVERROR          117
#define IPV6_FILTER             118
/* if acl not match, don't drop connect */
#define IPV6_DONTDROP           119

/* to define items, should talk with KAME guys first, for *BSD compatibility */

#define IPV6_RTHDR_LOOSE     0 /* this hop need not be a neighbor. XXX old spec */
#define IPV6_RTHDR_STRICT    1 /* this hop must be a neighbor. XXX old spec */
#define IPV6_RTHDR_TYPE_0    0 /* IPv6 routing header type 0 */

/*
 * Defaults and limits for options
 */
#define IPV6_DEFAULT_MULTICAST_HOPS 1    /* normally limit m'casts to 1 hop */
#define IPV6_DEFAULT_MULTICAST_LOOP 1    /* normally hear sends if a member */

/*
 * The im6o_membership vector for each socket is now dynamically allocated at
 * run-time, bounded by USHRT_MAX, and is reallocated when needed, sized
 * according to a power-of-two increment.
 */
#define    IPV6_MIN_MEMBERSHIPS    31
#define    IPV6_MAX_MEMBERSHIPS    (1024 * 1024 - 1)

/*
 * Default resource limits for IPv6 multicast source filtering.
 * These may be modified by sysctl.
 */
#define    IPV6_MAX_GROUP_SRC_FILTER    512    /* sources per group */
#define    IPV6_MAX_SOCK_SRC_FILTER    128    /* sources per socket/group */

/*
 * Argument for IPV6_PORTRANGE:
 * - which range to search when port is unspecified at bind() or connect()
 */
#define    IPV6_PORTRANGE_DEFAULT    0    /* default range */
#define    IPV6_PORTRANGE_HIGH    1    /* "high" - request firewall bypass */
#define    IPV6_PORTRANGE_LOW    2    /* "low" - vouchsafe security */

/*
 * G03597: used by setsockopt syscall (IPV6_USE_MIN_MTU)
 */
#define IP6PO_MINMTU_MCASTONLY    -1 /* default; send at min MTU for multicast*/
#define IP6PO_MINMTU_DISABLE     0 /* always perform pmtu disc */
#define IP6PO_MINMTU_ALL     1 /* always send at min MTU */

/* Structure describing an Internet6 socket address.  */
typedef struct sockaddr_in6
{
    unsigned char   sin6_len;       /* 地址结构的长度 */
    unsigned char   sin6_family;    /* 协议族，等于AF_INET6 */
    in_port_t       sin6_port;      /* 端口号 */
    UINT32          sin6_flowinfo;  /* IPv6流信息 */
    struct in6_addr sin6_addr;      /* IPv6地址 */
    UINT32          sin6_scope_id;  /* 范围索引 */
    VRF_INDEX       sin6_vrf;       /* vrf索引 */
} sockaddr_in6;

#ifdef __KERNEL__
/******************************************************************************
 *                    这里定义内核使用的宏和结构体                            *
 ******************************************************************************/
void ip6_l4mbuftrsend(struct mbuf *m, u_int dstslot);
u_long inet6_autorun(void *dummy);
void ip6_protoinput(u_char proto, struct mbuf *m, int off0);
void ip6_protoctlinput(u_char proto, int cmd, struct sockaddr *sa, void *ctlparam);
void ip6_pmtunotify(IN struct socket *pstSock, IN VRF_INDEX vrfIndex, IN IN6ADDR_S *pstAddr, IN u_int uiPmtu);
void ip6_pmtudel(IN struct socket * pstSock);
#endif

/*
 * Argument structure for IPV6_JOIN_GROUP and IPV6_LEAVE_GROUP.
 */
struct ipv6_mreq {
    struct in6_addr    ipv6mr_multiaddr;
    IF_INDEX ipv6mr_ifindex;  /* interface index */
};

/*
 * IPV6_PKTINFO: Packet information(RFC2292 sec 5)
 */
struct in6_pktinfo {
    struct in6_addr    ipi6_addr;    /* src/dst IPv6 address */
    IF_INDEX    ipi6_ifindex;    /* send/recv interface index */
};

/*
 * Control structure for IPV6_RECVPATHMTU socket option.
 */
struct ip6_mtuinfo {
    struct sockaddr_in6 ip6m_addr;    /* or sockaddr_storage? */
    UINT32 ip6m_mtu;
};

/*
 * Extension Headers
 */

struct    ip6_ext {
    UINT8 ip6e_nxt;
    UINT8 ip6e_len;
} ;

/* Hop-by-Hop options header */
/* XXX should we pad it to force alignment on an 8-byte boundary? */
struct ip6_hbh {
    UINT8 ip6h_nxt;    /* next header */
    UINT8 ip6h_len;    /* length in units of 8 octets */
    /* followed by options */
};

/* Destination options header */
/* XXX should we pad it to force alignment on an 8-byte boundary? */
struct ip6_dest {
    UINT8 ip6d_nxt;    /* next header */
    UINT8 ip6d_len;    /* length in units of 8 octets */
    /* followed by options */
};

/* Routing header */
struct ip6_rthdr {
    UINT8  ip6r_nxt;    /* next header */
    UINT8  ip6r_len;    /* length in units of 8 octets */
    UINT8  ip6r_type;    /* routing type */
    UINT8  ip6r_segleft;    /* segments left */
    /* followed by routing type specific data */
};

/* Type 0 Routing header, deprecated by RFC 5095. */
struct ip6_rthdr0 {
    UINT8   ip6r0_nxt;        /* next header */
    UINT8   ip6r0_len;        /* length in units of 8 octets */
    UINT8   ip6r0_type;       /* always zero */
    UINT8   ip6r0_segleft;    /* segments left */
    UINT32  ip6r0_reserved;  /* reserved field */
    /* followed by up to 127 struct in6_addr */
} ;

/* Fragment header */
struct ip6_frag {
    UINT8  ip6f_nxt;         /* next header */
    UINT8  ip6f_reserved;    /* reserved field */
    UINT16 ip6f_offlg;       /* offset, reserved, and flag */
    UINT32 ip6f_ident;       /* identification */
} ;

/*
 * Control options for outgoing packets
 */

/* Routing header related info */
struct    ip6po_rhinfo {
    struct    ip6_rthdr *ip6po_rhi_rthdr; /* Routing header */
};
#define ip6po_rthdr    ip6po_rhinfo.ip6po_rhi_rthdr

/* Nexthop related info */
struct    ip6po_nhinfo {
    struct    sockaddr *ip6po_nhi_nexthop;
};
#define ip6po_nexthop    ip6po_nhinfo.ip6po_nhi_nexthop


#ifdef __KERNEL__

struct    ip6_pktopts {
    struct    mbuf *ip6po_m;    /* Pointer to mbuf storing the data */
    int    ip6po_hlim;    /* Hoplimit for outgoing packets */

    /* Outgoing IF/address information */
    struct    in6_pktinfo *ip6po_pktinfo;

    /* Next-hop address information */
    struct    ip6po_nhinfo ip6po_nhinfo;

    struct    ip6_hbh *ip6po_hbh; /* Hop-by-Hop options header */

    /* Destination options header (before a routing header) */
    struct    ip6_dest *ip6po_dest1;

    /* Routing header related info. */
    struct    ip6po_rhinfo ip6po_rhinfo;

    /* Destination options header (after a routing header) */
    struct    ip6_dest *ip6po_dest2;
    
    struct in6_addr ip6po_src;

    int    ip6po_tclass;    /* traffic class */

    int    ip6po_minmtu;  /* fragment vs PMTU discovery policy */

    int    ip6po_prefer_tempaddr;  /* whether temporary addresses are
                       preferred as source address */
    int ip6po_flags;
};

/* ip6po_prefer_tempaddr的取值 */
#define IP6PO_TEMPADDR_SYSTEM    -1 /* follow the system default */
#define IP6PO_TEMPADDR_NOTPREFER 0 /* not prefer temporary address */
#define IP6PO_TEMPADDR_PREFER     1 /* prefer temporary address */

/* ip6po_flags的可用标识 */
#if 0    /* parameters in this block is obsolete. do not reuse the values. */
#define IP6PO_REACHCONF    0x01    /* upper-layer reachability confirmation. */
#define IP6PO_MINMTU    0x02    /* use minimum MTU (IPV6_USE_MIN_MTU) */
#endif
#define IP6PO_DONTFRAG    0x04    /* disable fragmentation (IPV6_DONTFRAG) */
#define IP6PO_USECOA    0x08    /* use care of address */
#define IP6PO_USEICMPSRC  0x10    /* use icmp source address */
#define IP6PO_SELECTMATCHSRCBYFIB  0x20    /* only when dst is reachable, 
                                              select matched source address */

extern struct domain inet6domain;
int  ip6_ctloutput(struct socket *, struct sockopt *);
void ip6_init(void);

#endif


/***************************************************************************/
/*                        IPv6协议使能宏和数据结构定义                     */
/***************************************************************************/
typedef struct tagIP6_PROTOCOLPACKETLIST
{
    UINT uiTag;
    UINT uiGlobal;
    UINT uiValue;
    UINT uiProtocolType;
    IN6ADDR_S stDstIn6Addr;
    UINT uiMaskLen;
    UINT uiPara1;
    UINT uiPara2;
    IF_INDEX ifIndex;
}IP6_PROTOCOL_PACKET_LIST_S;

typedef struct tagIP6_ENABLE_PROTOCOL
{
    UINT uiTag;
    UINT uiGlobal;
    UINT uiAction;
    UINT uiProtocolType;
    UINT uiFunction;
    IN6ADDR_S stDstIn6Addr;
    UINT uiMaskLen;
    UINT uiPara1;
    UINT uiPara2;
    UINT uiPara3;
    IF_INDEX ifIndex;
    VRF_INDEX vrfIndex;
}IP6_ENABLE_PROTOCOL_S;

/***************************************************************************/
/*                        用户态对外接口定义                               */
/***************************************************************************/
ULONG IP6_EnableProtocolToCPU(IN UINT uiTag, 
                                    IN UINT uiGlobal,
                                    IN UINT uiValue, 
                                    IN UINT uiProtocolType,
                                    IN IN6ADDR_S stDstIn6Addr, 
                                    IN UINT uiMaskLen,
                                    IN UINT uiPara1, 
                                    IN UINT uiPara2,
                                    IN IF_INDEX ifIndex);

ULONG IP6_EnableProtocolPacketListToCPU( IN UINT32 uiBatchNum,
                                           IN const IP6_PROTOCOL_PACKET_LIST_S *pstBatchData);

ULONG IP6_EnableProtocolToCpuEx(IN UINT32 uiNum,IN const IP6_ENABLE_PROTOCOL_S * pstData, IN UINT uiKeyLen);
#ifdef  __cplusplus
}
#endif  /* end of __cplusplus */

#endif  /* end of _SYS_IN6_H_ */
