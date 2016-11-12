/*****************************************************************************
 Copyright (c) 2010, Hangzhou H3C Technologies Co., Ltd. All rights reserved.
------------------------------------------------------------------------------
                            ip6fw.h
  Project Code: Comware V7
   Module Name: IP6FW
  Date Created: 2010-8-3 
        Author: z03132
   Description: Header file for IP6FW module

------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------
                                                                             
*****************************************************************************/

#ifndef _SYS_IP6FW_H_
#define _SYS_IP6FW_H_

#ifdef  __cplusplus
extern "C"{
#endif

/* 
 * For user and kernel both
 */

#ifndef _DRV_IP_H_
#include <drv/ip.h>
#endif

#ifndef _SYS_FIB_H_
#include <sys/fib.h>
#endif

#ifndef _SYS_IN_H_
#include <sys/in.h>
#endif

#ifndef _SYS_IN6_H_
#include <sys/in6.h>
#endif

#ifndef _SYS_ICMP6_H_
#include <sys/icmp6.h>
#endif

#ifndef _SYS_RAWIP6_H_
#include <sys/rawip6.h>
#endif

/****************************************************
 *             宏定义                               *
 ****************************************************/
/* 定义MID */


/* 定义IP6转发模块使用的全局知名端口 */





/* CIOCTL定义 */

#define IP6FW_CIOCTL_CMD_PROC     CIOCTL_CMD_DEF(CIOCTL_CMDMOD_IP6FW, 1)
#define IP6FW_CIOCTL_CMD_SMOOTH   CIOCTL_CMD_DEF(CIOCTL_CMDMOD_IP6FW, 2)
#define IP6FW_CIOCTL_CMD_BATCHIF  CIOCTL_CMD_DEF(CIOCTL_CMDMOD_IP6FW, 3)
#define IP6FW_CIOCTL_CMD_BATCHGLBBASE   CIOCTL_CMD_DEF(CIOCTL_CMDMOD_IP6FW, 4)
#define IP6FW_CIOCTL_CMD_BATCHGLBALL    CIOCTL_CMD_DEF(CIOCTL_CMDMOD_IP6FW, 5)


/* DUMP下内核命令字 */

#define IP6FW_DUMP_CMD_SHOW       DUMP_CMD_DEF(DUMP_CMDMOD_IP6FW, 1)

/* MBUF透传通道定义 */
#define MBUF_RELAY_TYPE_IP6_DELIVERTOUP  10
#define MBUF_RELAY_TYPE_IP6_IFOUTPUT     11

/* 流量类型和流标记掩码 */
#define IP6_FLOWINFO_MASK       0x0fffffff  /* flow info (28 bits) */
#define IP6_FLOWLABEL_MASK      0x000fffff  /* flow label (20 bits) */

/* IPv6报文版本 */
#define IPV6_VERSION         0x06

/* 接口上缺省的IPv6 MTU的值, 目前所有接口上缺省MTU是相同的 */
#define IP6_DEFAULT_MTU      1500
/* 接口上可配置的最小IPv6 MTU */
#define IP6_MIN_MTU          1280
/* 接口上可配置的最大IPv6 MTU */  
#define IP6_MAX_MTU          10240


/* IPv6报文载荷的最大长度 */
#define IPV6_MAXPACKET      65535

#define IP6F_OFF_MASK       0xfff8  /* mask out offset from _offlg */
#define IP6F_RESERVED_MASK  0x0006  /* reserved bits in ip6f_offlg */
#define IP6F_MORE_FRAG      0x0001  /* more-fragments flag */

#define IP6_REASS_MODE_GLOBAL    0
#define IP6_REASS_MODE_LOCAL     1

/****************************************************
 *             报文结构                             *
 ****************************************************/
/* IPv6 报文头 */
typedef struct tagIP6_S 
{
    union 
    {
        struct tagIP6_HDRCTL 
        {
            UINT32 uiIp6TclassFlow;   /* 4 bits unused, 8 bits traffic class and 20 bits of flow-ID */
            UINT16 usIp6PLen;         /* payload length */
            UINT8 ucIp6Nxt;           /* next header */
            UINT8 ucIp6HLim;          /* hop limit */
        } stIp6Ctl1;

        struct tagIP6_HDRCTL1 
        {
            #if defined(__LITTLE_ENDIAN_BITFIELD)
            UINT8 ucIp6Unused:4;      /* traffic class */
            UINT8 ucIp6Ver:4;         /* version */
            #elif defined(__BIG_ENDIAN_BITFIELD)
            UINT8 ucIp6Ver:4;         /* version */
            UINT8 ucIp6Unused:4;      /* traffic class */
            #else
            #error "Adjust your <asm/byteorder.h> defines"
            #endif
        } stIp6Ctl2;
    }unIp6Ctl;

    IN6ADDR_S stIp6Src;               /* source address */
    IN6ADDR_S stIp6Dst;               /* destination address */

}IP6_S;

#define ip6_ucVer       unIp6Ctl.stIp6Ctl2.ucIp6Ver
#define ip6_uiFlow      unIp6Ctl.stIp6Ctl1.uiIp6TclassFlow
#define ip6_usPLen      unIp6Ctl.stIp6Ctl1.usIp6PLen
#define ip6_ucNxtHdr    unIp6Ctl.stIp6Ctl1.ucIp6Nxt
#define ip6_ucHLim      unIp6Ctl.stIp6Ctl1.ucIp6HLim

/****************************************************
 *             配置相关数据定义                     *
 ****************************************************/
typedef enum tagIP6_Debug
{
    IP6_DEBUG_PKT,
    IP6_DEBUG_ERR,
    IP6_DEBUG_ALL
}IP6_DEBUG_E;

/****************************************************
 *             Enum definition                      *
 ****************************************************/

/* 转发消息类型,用于用户态和内核态交互,及用户态板间数据同步 */ 
ISSU typedef enum tagIP6_MsgType
{
    IP6_MSGTYPE_CFGMSG,                 /* 配置数据消息 */
    IP6_MSGTYPE_BATCHIFCFG,             /* 批量接口数据消息 */
    IP6_MSGTYPE_BATCHGLBCFG,            /* 批量全局数据消息 */
    IP6_MSGTYPE_SMOOTH_START,           /* 平滑开始消息 */
    IP6_MSGTYPE_SMOOTH_END,             /* 平滑结束消息 */    
    IP6_MSGTYPE_BATCHGLBCFGALL,
    IP6_MSGTYPE_BUTT
} IP6_MSG_TYPE_E;

/* 转发配置消息类型 */
ISSU typedef enum tagIP6_CfgMsgType
{
    IP6_CFGMSG_SHOWSTAT,        /* IPv6统计信息显示 */
    IP6_CFGMSG_CLEARSTAT,       /* 清空IPv6统计信息 */    
    IP6_CFGMSG_SETDEBUG,        /* 设置IPv6调试信息开关 */
    IP6_CFGMSG_GETDEBUG,        /* 获取IPv6调试信息开关 */
    IP6_CFGMSG_SHOWIFSTAT,      /* IPv6接口统计信息显示 */  
    IP6_CFGMSG_SETIP6MTU,       /* 设置接口ipv6 mtu */
    IP6_CFGMSG_GETIP6MTU,       /* 获取接口ipv6 mtu */
    IP6_CFGMSG_SETIP6ECMP,      /* 设置ipv6 ecmp */
    IP6_CFGMSG_GETIP6ECMP,      /* 获取ipv6 ecmp */
    IP6_CFGMSG_GETIFICMP6STAT,  /* 获取ICMP6接口统计信息 */
    IP6_CFGMSG_GETMIBIFSTAT,    /* 获取MIB IPv6接口统计信息显示 */  
    IP6_CFGMSG_GETMIBSTAT,      /* 获取MIB IPv6全局统计信息显示 */ 
    IP6_CFGMSG_SETOPTIONDROP,   /* 设置ipv6 option drop 功能 */
    IP6_CFGMSG_SETREASSMODE,    /* 设置重组模式 */
    IP6_CFGMSG_GETBPASTAT,      /* 获取BPA统计信息 */
    IP6_CFGMSG_BUTT
} IP6_CFGMSG_TYPE_E;

/* 需要保存数据的应用模块类型 */
typedef enum tagIp6fw_Mod_ID
{
    IP6FW_MOD_BPA,             /* BPA模块ID*/
    IP6FW_MOD_SMA,             /* SMA模块ID*/
    IP6FW_MOD_BUTT            
} IP6FW_MODID_E;

/* 转发配置消息结构 */
ISSU typedef struct tagIP6_MsgHead
{
    USHORT      usType;         /* 转发配置消息类型 */
    USHORT      usMsgLen;       /* 转发配置消息长度,长度包含头域 */
} IP6_MSG_HEAD_S;

ISSU typedef struct tagIP6_CFG_DEBUG
{
    UINT        uiType;
    UINT        uiEnable;
    UINT        uiAcl;
}IP6_CFG_DEBUG_S;

ISSU typedef struct tagIP6_CFG_MTU
{
    IF_INDEX    ifIndex;          /* 接口索引 */
    UINT        uiIP6MTU;         /* 配置的接口协议MTU */
} IP6_CFG_MTU_S;

ISSU typedef struct tagIP6_CFG_ECMP
{
    UINT        uiCurrEcmpNum;   /* 当前ECMP值 */
    UINT        uiNextEcmpNum;   /* 下次重启后ECMP值 */
} IP6_CFG_ECMP_S;

ISSU typedef struct tagIP6_CFG_REASS
{
    UINT        uiIP6ReassMode;
} IP6_CFG_REASSMODE_S;

typedef struct tagIP6_CFG_OPTION
{
    UINT        uiIP6OptionDropFlag;    /* ipv6 option drop 标记位 */
} IP6_CFG_OPTION_S;

/* 显示 DEBUG 开关状态 */
ISSU typedef struct tagIP6_DebugInfo
{
    UINT        uiIP6DbgPktACL; 
    UCHAR       ucIP6PktDebugFlag;
    UCHAR       ucIP6ErrDebugFlag;         
} IP6_DEBUG_INFO_S;

/* 用途: 存储DBM的接口数据结构,同步批量接口消息结构,批量接口下内核结构 */
ISSU typedef struct tagIP6_IFSYNDATA
{
    CHAR        szIfName[IF_MAX_NAME_LEN + 1];   /* 仅DBM中使用 */
    IF_INDEX    ifIndex;                         /* 接口索引, 同步批量数据使用 */
    UINT        uiIP6MTU;                        /* 接口协议MTU */
} IP6_IFSYNDATA_S;

ISSU typedef struct tagIP6_GLBBASEDATA
{
    UINT        uiIP6ECMP;                      /* Ipv6最大ECMP数 */
} IP6_GLBBASEDATA_S;

ISSU typedef struct tagIP6_GLBSYNDATA
{
    UINT        uiIP6ECMP;                       /* Ipv6最大ECMP数 */
    UINT        uiIP6OptionDropFlag;             /* ipv6 option drop 标记位*/
    UINT        uiIsGlbDBMRecover;               /* 判断是否是二进制恢复导致的平滑 */
} IP6_GLBSYNDATA_S;

/* 统计信息，接口统计全局统计共用 */
ISSU typedef struct tagIP6_Stat
{
    UINT64  uiTotalReceived;    /* 收到的IPv6报文总数 */
    UINT64  uiInForwDatagrams;  /* 收到的目的地址不是本机IPv6地址的IPv6报文 */
    UINT64  uiForward;          /* 成功转发的IPv6报文 */
    UINT64  uiOutTransmits;     /* 发送提交给链路层的IPv6报文数 */
    UINT  uiTooShort;           /* 收到的IPv6报文长度不足40字节个数 */
    UINT  uiTooSmall;           /* 收到的截断的IPv6报文个数 */
    UINT  uiHopLimitExceed;     /* 收到的跳数超过限制IPv6报文数 */
    UINT  uiBadHeader;          /* 收到的头部错误IPv6报文数 */
    UINT  uiBadOption;          /* 收到的选项错误IPv6报文数 */
    UINT  uiFragments;          /* 收到IPv6分片报文数 */
    UINT  uiReassembled;        /* IPv6重组成功报文数 */
    UINT  uiFragDropped;        /* 丢弃的IPv6分片报文数 */
    UINT  uiFragTimeout;        /* 超时导致重组失败次数 */
    UINT  uiNoProto;            /* 上层协议不识别报文数 */
    UINT  uiDelivered;          /* 上送本机IPv6报文数 */
    UINT  uiNoRoute;            /* 转发中查FIB6失败报IPv6文数 */
    UINT  uiDstAddrErr;         /* 收到的目的地址非法IPv6报文数 */
    UINT  uiOFailedFragments;   /* 发送时丢弃的分片报文数 */
    UINT  uiLocalOut;           /* 本机成功发送的非分片IPv6报文数 */
    UINT  uiTooBig;             /* 发送时需要分片但是不允许分片而丢弃IPv6报文数 */
    UINT  uiFragmented;         /* 成功被分片的IPv6报文数 */
    UINT  uiOFragments;         /* 成功发送IPv6分片报文数 */
    UINT  uiNotMember;          /* 收到的非入接口所在组播组的IPv6报文数 */
    UINT  uiInMulticasts;       /* 收到组播IPv6报文数 */
    UINT  uiOutMulticasts;      /* 发送IPv6组播报文数 */
    UINT  uiInDiscards;         /* 输入因其他原因丢弃报文数 */
    UINT  uiOutDiscards;        /* 输出因其他原因丢弃报文数 */
    UINT  uiOutNoRoutes;        /* IPv6本机出查找路由表失败的报文数 */
    UINT  uiOutFragReqds;       /* IPv6发送报文时需要分片的报文数 */
    UINT  uiOutRequests;        /* IPv6传输侧提交给网络层的报文数 */
}IP6_STAT_S;

ISSU typedef struct tagIP6_StatShow
{
    IP6_STAT_S   stIP6Stat;
    ICMP6_STAT_S stICMP6Stat;
    RIP6_STAT_S  stRIP6Stat;
} IP6_STAT_SHOW_S;

/* IPv6伪头结构定义 */
typedef struct tagIP6_PseudoHeader
{
    IN6ADDR_S stIPv6SrcAddr;               /* source address */
    IN6ADDR_S stIPv6DstAddr;               /* destination address */
    UINT      uiDataLen;                   /* for UDP, UdpHeaderLen+UserDataLen */
    UCHAR     aucZero[3];                  /* for align */
    UCHAR     ucNextPro;                   /* upper-layer protocol, TCP/UDP/ICMP6 etc  */
}IP6_PSEUDO_HEADER_S;

/* 
 * For kernel only
 */
#ifdef __KERNEL__
/****************************************************
 *             宏定义                               *
 ****************************************************/
/* 下面的宏用于调用函数IP6_Output时设置参数lFlag */
#define IP6_UNSPECSRC              0x00000001   /* 表示发出的报文源地址为0 */
#define IP6_USEMINMTU              0x00000002   /* 表示用最小的1280决定分片处理 */
#define IP6_DONT_FRAG              0x00000004   /* 表示不允许分片 */
#define IP6_NEXTHOP_SPECIFIED      0x00000008   /* 表示指定下一跳发送 */
#define IP6_ISINTERMEDIATE         0x00000010   /* 表示NAT-PT报文 */
#define IP6_PROXYOUTPUT            0x00000020   /* 表示报文在本板发送 */

/* 指定转发流程缺省处理的第一个业务 */
#define IP6_DEFAULT_SERVICEBITMAP  ((ULONG)-1)

/* 业务标志位 Bitmap 赋值操作宏 */
#define IP6_SERVICE_SETBITMAP(pulBitmapM, uiServiceIDM) \
    __set_bit((BITS_PER_LONG - 1) - uiServiceIDM, pulBitmapM);

#define IP6_SERVICE_CLEARBITMAP(pulBitmapM, uiServiceIDM) \
    __clear_bit((BITS_PER_LONG - 1) - uiServiceIDM, pulBitmapM);


/****************************************************
 *             枚举定义                             *
 ****************************************************/

/* IPv6转发业务接收报文入口阶段 */
typedef enum tagIP6FW_PreAll
{
    IP6FW_PRE_ALL_ATKPROXY, 
    IP6FW_PRE_ALL_BUTT
} IP6FW_PRE_ALL_SERVICE_ID_E;

/* IPv6转发业务处理入方向业务ID */
typedef enum tagIP6FW_PreRoutingServiceId
{
    IP6FW_PRE_ROUTING_TCPREASSEMBLE, /* TCP重组 */
    IP6FW_PRE_ROUTING_NETSTREAM,
    IP6FW_PRE_ROUTING_MGROUP,           /* Mirror Group业务 */
    IP6FW_PRE_ROUTING_IPSEC,            /* IPsec 预处理 */
    IP6FW_PRE_ROUTING_FLOWMGR,
    IP6FW_PRE_ROUTING_INTERZONE,     /*域间处理*/
    IP6FW_PRE_ROUTING_SMA,           /* SMA业务 */
    IP6FW_PRE_ROUTING_ATK,
    IP6FW_PRE_ROUTING_LBPRE,
    IP6FW_PRE_ROUTING_IPOE,
    IP6FW_PRE_ROUTING_WEBREDIRECT,
    IP6FW_PRE_ROUTING_PORTAL,
    IP6FW_PRE_ROUTING_TWAMP,            /* TWAMP业务 */    
    IP6FW_PRE_ROUTING_APR,           /* 协议识别 */
    IP6FW_PRE_ROUTING_APPSTAT,       /* 流量统计 */
    IP6FW_PRE_ROUTING_CONNLMT,       /* 连接数限制 */
    IP6FW_PRE_ROUTING_BPASTAT,          /* 基于BGP策略的流量统计 */ 
    IP6FW_PRE_ROUTING_AUTHORIZE,     /* 授权检查处理阶段 */
    IP6FW_PRE_ROUTING_FILTER,           /* PFLT 的处理阶段 */
    IP6FW_PRE_ROUTING_LB,               /* LB 负载均衡处理阶段 */
    IP6FW_PRE_ROUTING_QPPB,             /* QPPB */
    IP6FW_PRE_ROUTING_PRIMAP,
    IP6FW_PRE_ROUTING_QOS,              /* QoS */
    IP6FW_PRE_ROUTING_AFT,           /* 地址转换 */
    IP6FW_PRE_ROUTING_BUTT
}IP6FW_PRE_ROUTING_SERVICE_ID_E;

/* 接收报文查询路由后处理阶段 */
typedef enum tagIp6fw_PostRouting
{
    IP6FW_POST_ROUTING_PBR,
    IP6FW_POST_ROUTING_LB,
    IP6FW_POST_ROUTING_LISP,
    IP6FW_POST_ROUTING_OFP,
    IP6FW_POST_ROUTING_WAAS,
    IP6FW_POST_ROUTING_BUTT
} IP6FW_POST_ROUTING_SERVICEID_E;

/* IPv6处理上送本机重组前业务ID */
typedef enum tagIP6FW_LocalInBeforeDefragServiceId
{
    IP6FW_LOCAL_IN_INTERZONE, /* 域间处理阶段 */
    IP6FW_LOCAL_IN_ATK,
    IP6FW_LOCAL_IN_CONNLMT,          /* 连接数限制 */
    IP6FW_LOCAL_IN_APPL7,
    IP6FW_LOCAL_IN_BEFOREDEFRAG_BUTT
}IP6FW_LOACL_IN_BEFOREDEFRAG_SERVICE_ID_E;

/* IPv6转发业务处理上送本机业务ID */
typedef enum tagIP6FW_LocalInServiceId
{
    IP6FW_LOCAL_IN_GTSM,
    IP6FW_LOCAL_IN_BFD,
    IP6FW_LOCAL_IN_TCP_PROXY,
    IP6FW_LOCAL_IN_BUTT
}IP6FW_LOACL_IN_SERVICE_ID_E;

/* IPv6转发业务处理本本机发送业务ID */
typedef enum tagIP6FW_LocalOutServiceId
{
    IP6FW_LOCAL_OUT_INTERZONE,       /* 域间处理阶段 */
    IP6FW_LOCAL_OUT_WAAS,
    IP6FW_LOCAL_OUT_BUTT
}IP6FW_LOCAL_OUT_SERVICE_ID_E;

/* 本机发送报文查询路由后处理阶段 */
typedef enum tagIp6fw_LocalOutPostRouting
{
    IP6FW_LOCAL_OUT_POST_ROUTING_PBR,
    IP6FW_LOCAL_OUT_POST_ROUTING_LISP,
    IP6FW_LOCAL_OUT_POST_ROUTING_OFP,
    IP6FW_LOCAL_OUT_POST_ROUTING_BUTT
} IP6FW_LOCAL_OUT_POST_ROUTING_SERVICEID_E;

/* IPv6转发业务处理出接口发送业务ID */
typedef enum tagIP6FW_PostRoutingServiceId
{
    IP6FW_POST_ROUTING_BEFOREFRAG_FLOWMGR,      /* 二次引流业务 */
    IP6FW_POST_ROUTING_BEFOREFRAG_FILLTAG,      /* 报文数据域填充 */
    IP6FW_POST_ROUTING_BEFOREFRAG_INTERZONE,     /* 域间处理阶段 */
    IP6FW_POST_ROUTING_BEFOREFRAG_LB,           /* LB 负载均衡处理阶段 */
    IP6FW_POST_ROUTING_BEFOREFRAG_ATK,           /* ATK */
    IP6FW_POST_ROUTING_BEFOREFRAG_SMA,          /* SMA业务 */
    IP6FW_POST_ROUTING_BEFOREFRAG_FILTER,       /* PFLT 的处理阶段 */
    IP6FW_POST_ROUTING_BEFOREFRAG_AUTHORIZE,     /* 授权检查处理阶段 */
    IP6FW_POST_ROUTING_BEFOREFRAG_CONNLMT,       /* 连接数限制 */
    IP6FW_POST_ROUTING_BEFOREFRAG_APR,           /* 协议识别 */
    IP6FW_POST_ROUTING_BEFOREFRAG_APPSTAT,       /* 流量统计 */
    IP6FW_POST_ROUTING_BEFOREFRAG_APPL7,
    IP6FW_POST_ROUTING_BEFOREFRAG_BPASTAT,      /* 基于BGP策略的流量统计 */
    IP6FW_POST_ROUTING_BEFOREFRAG_PORTAL,
    IP6FW_POST_ROUTING_BEFOREFRAG_IPOE,
    IP6FW_POST_ROUTING_BEFOREFRAG_PREQOS,       /* QoS预处理 */
    IP6FW_POST_ROUTING_BEFOREFRAG_TWAMP,    
    IP6FW_POST_ROUTING_BEFOREFRAG_IPSEC,        /* IPsec 业务点 */
    IP6FW_POST_ROUTING_BEFOREFRAG_NETSTREAM,
    IP6FW_POST_ROUTING_BEFOREFRAG_MGROUP,       /* Mirror Group */
    IP6FW_POST_ROUTING_BEFOREFRAG_WAAS,
    IP6FW_POST_ROUTING_BEFOREFRAG_OFP,          /* OpenFlow */
    IP6FW_POST_ROUTING_BEFOREFRAG_BUTT
}IP6FW_POST_ROUTING_SERVICE_ID_E;

/* 报文透传前阶段 */
typedef enum tagIP6FW_PreRelayServiceId
{
    IP6FW_PRE_RELAY_INTERZONE,  /* 域间处理阶段 */
    IP6FW_PRE_RELAY_CONNLMT, /* 连接数限制 */
    IP6FW_PRE_RELAY_APPL7,
    IP6FW_PRE_RELAY_BUTT
} IP6FW_PRE_RELAY_SERVICEID_E;

/* 报文透传接收阶段 */
typedef enum tagIP6FW_RelayReceiveServiceId
{
    IP6FW_RELAY_RECEIVE_INTERZONE, /* 域间处理阶段 */
    IP6FW_RELAY_RECEIVE_BUTT
} IP6FW_RELAY_RECEIVE_SERVICEID_E;

/* 组播选项，本机报文发送时使用 */
typedef struct tagIP6_Moptions
{
    IF_INDEX ifIP6MIndex;            
    UCHAR ucIP6MHlim;              
    UCHAR ucIP6MLoop;                  
}IP6_MOPTIONS_S;

/* IP6 转发统计类型定义 */
typedef enum tagIp6StatType
{
    IP6_STAT_TOTALRCV,
    IP6_STAT_INFORWDATA,
    IP6_STAT_FORWARD,
    IP6_STAT_OUTTRANSMIT,
    IP6_STAT_TOOSHORT,
    IP6_STAT_TOOSMALL,
    IP6_STAT_TTLEXCEED,
    IP6_STAT_BADHEADER,
    IP6_STAT_BADOPTION,
    IP6_STAT_FRAGMENTS,
    IP6_STAT_REASSEMBLED,
    IP6_STAT_FRAGDROP,
    IP6_STAT_FRAGTIMEOUT,
    IP6_STAT_NOPROTO,
    IP6_STAT_DELIVER,
    IP6_STAT_NOROUTE,
    IP6_STAT_DSTADDRERR,
    IP6_STAT_OUTFAILEDFRAG,
    IP6_STAT_LOCALOUT,
    IP6_STAT_TOOBIG,
    IP6_STAT_FRAGMENTED,
    IP6_STAT_OUTFRAGMENTES,
    IP6_STAT_NOTMEMBER,
    IP6_STAT_INMULTICASTS,
    IP6_STAT_OUTMULTICASTS,
    IP6_STAT_INDISCARDS,
    IP6_STAT_OUTDISCARDS,
    IP6_STAT_OUTNOROUTES,
    IP6_STAT_OUTFRAGREQDS,
    IP6_STAT_OutREQUESTS,
    IP6_STAT_MAX
}IP6STAT_TYPE_E;
/****************************************************
 *             函数原型                             *
 ****************************************************/
typedef ULONG (*IP6_SERVICEPROC_PF)(IN IF_INDEX ifIndex, INOUT FIB6_FWDINFO_S *pstFIB6FWDInfo, INOUT MBUF_S *pstMBuf);
extern VOID  IP6_RegisterService(IN IPFW_PHASE_E enPhase, 
                              IN UINT uiServiceID, 
                              IN IP6_SERVICEPROC_PF pfServiceProcFunc);
extern VOID  IP6_DeregisterService(IN IPFW_PHASE_E enPhase, IN UINT uiServiceID);
extern ULONG IP6_TCPService_Handle(IN UINT uiOffset, INOUT MBUF_S *pstMBuf);

extern ULONG IP6_MultiService_Handle(IN IF_INDEX ifIndex, IN UINT uiPhase, INOUT MBUF_S *pstMBuf);
extern ULONG IP6_SetIfServiceSwitch(IN IF_INDEX ifIndex, 
                                    IN IPFW_PHASE_E enPhase, 
                                    IN UINT uiServiceID, 
                                    IN UINT uiValue);
extern ULONG IP6_SetGlobalServiceSwitch(IN IPFW_PHASE_E enPhase,IN UINT uiServiceID,IN UINT uiValue);

extern ULONG UDP6_RegisterService(IN USHORT usDstPort, IN UDP_SERVICE_PF pfServiceProcFunc);
extern VOID UDP6_DeregisterService(IN USHORT usDstPort);

extern ULONG TCP6_RegisterService(IN TCP_SERVICE_PF pfServiceProcFunc);
extern VOID TCP6_DeregisterService(VOID);

extern ULONG IP6_Input(IN MBUF_S *pstMBuf);
extern ULONG IP6_InputOptional(IN MBUF_S * pstMBuf, IN UCHAR ucOptionalFlag, IN ULONG ulServiceBitMap);
extern ULONG IP6_Output(IN MBUF_S *pstMBuf, 
                      IN struct ip6_pktopts *pstGenOpt, 
                      IN ULONG ulFlags, 
                      IN const IP6_MOPTIONS_S *pstIp6Mo, 
                      IN ULONG ulServiceBitMap );
extern ULONG IP6_Output2(IN MBUF_S *pstMBuf, 
                         IN struct ip6_pktopts *pstGenOpt, 
                         IN ULONG ulFlags, 
                         IN const IP6_MOPTIONS_S *pstIp6Mo, 
                         IN ULONG ulServiceBitMap );
extern ULONG IP6_LocalOutput(IN MBUF_S *pstMBuf, IN ULONG ulServiceBitMap);
extern ULONG IP6_IfOutput(IN MBUF_S *pstMBuf, IN ULONG ulServiceBitMap);
extern ULONG IP6_Distribute(IN MBUF_S *pstMBuf);

extern USHORT IP6_ChkSum(IN UCHAR ucNext,IN UINT uiOff,IN UINT uiLen,IN MBUF_S *pstMBuf);
extern ULONG  IP6_GetLastHdr(IN MBUF_S *pstMBuf,INOUT UINT *puiOff,INOUT UCHAR *pucProto);
extern ULONG IP6_GetBufLastHdr(IN const FSBUF_BLOCKINFO_S *pstBlockInfo, 
                               INOUT UINT *puiOff, 
                               INOUT UCHAR *pucProto);

extern VOID IP6_FillMBufQoSTag(INOUT MBUF_S *pstMBuf);
extern ULONG IP6_Reass(IN UINT uiOff, INOUT MBUF_S **ppstMBuf);
extern ULONG IP6_Frag(IN UINT uiMTU, INOUT MBUF_S *pstMBUFPkt);
extern ULONG IP6_FragByPMTU(IN UINT uiLinkMTU, INOUT MBUF_S *pstMBuf);
/* FPGA: 支持转发项目增加 */
extern UINT  IP6_GetAndIncrIfSeqNo(IN UINT uiPhase, IN IF_INDEX ifIndex);
extern UINT IP6_GetIfCfgSeqNo(IN UINT uiPhase, IN IF_INDEX ifIndex);

extern VOID IP6_IfStat_Inc(IN IF_INDEX ifIndex, IN IP6STAT_TYPE_E enIp6StatType);

extern VOID IP6_Stat_Inc (IN IP6STAT_TYPE_E enIp6StatType);

extern VOID IP6_Stat_Dec (IN IP6STAT_TYPE_E enIp6StatType);

extern VOID IP6_FirstFragInOrder_Enable(UCHAR ucServiceBit);
extern VOID IP6_FirstFragInOrder_Disable(UCHAR ucServiceBit);
extern UCHAR IP6_FirstFragInOrder_GetStatus(VOID);

/* 根据扩展头相对于IPv6头的偏移和扩展头长度，获取扩展头指针.
   uiOff, 扩展头相对于IPv6头的偏移;
   uiLen, 扩展头需要进行连续的长度
   uiMid, 模块ID */
static inline VOID* IP6_GetExtHdr(IN MBUF_S *pstMBuf,IN UINT uiOff, IN UINT uiLen, IN UINT uiMid)
{
    ULONG ulRet;
    VOID *pstExt = NULL;
    
    ulRet = MBUF_PULLUP(pstMBuf, uiOff + uiLen, uiMid, GFP_ATOMIC);
    if (ERROR_SUCCESS == ulRet)
    {
        pstExt = (VOID *)(MBUF_BTOD(pstMBuf, UCHAR *) + uiOff);
    }
    return pstExt;
}

static inline VOID* IP6_GetBufExtHdr(IN const FSBUF_BLOCKINFO_S *pstBlockInfo, IN UINT uiOff)
{
    VOID *pstExt = NULL;

    pstExt = (VOID *)(FSBUF_BTOD_OFFSET(pstBlockInfo, 0U, UCHAR *) + uiOff);

    return pstExt;
}


extern VOID IP6_UpdateStat(IF_INDEX ifIndex, ULONG ulOffset, ULONG ulSize, LONG lFlag);
/* 提供给外部模块的宏，同时对接口统计和全局统计加1 */
#define IP6_INCREASE_STAT(ifIndexM, FieldM) \
{\
    IP6_STAT_S *pt = (IP6_STAT_S *)0x1UL;\
    IP6_UpdateStat((ifIndexM), (ULONG)((UCHAR *)&pt->FieldM-(UCHAR *)pt), sizeof(pt->FieldM), 1);\
}

/* 提供给外部模块的宏，同时对接口统计和全局统计减1 */
#define IP6_DECREASE_STAT(ifIndexM, FieldM) \
{\
    IP6_STAT_S *pt = (IP6_STAT_S *)0x1UL;\
    IP6_UpdateStat((ifIndexM), (ULONG)((UCHAR *)&pt->FieldM-(UCHAR *)pt), sizeof(pt->FieldM), -1);\
}

extern VOID IP6_SlowTimo(VOID);
extern VOID IP6_Drain(VOID);

extern ULONG IP6FW_Init(IN LPVOID pStartContext);
extern ULONG IP6FW_Run(IN LPVOID pStartContext);

extern ULONG IP6_MDCEventCallback(IN UINT uiEvent, IN VOID *pData);

extern VOID IP6_VrfNotify(IN IF_INDEX ifIndex, IN VRF_INDEX vrfIndex);

/* 接口转发数据查询 */
extern ULONG IP6_GetFWDData(IN IF_INDEX ifIndex,
                            IN IP6FW_MODID_E enFWDType,
                            OUT ULONG *pulRetValue);

/* 接口转发数据设置 */
extern ULONG IP6_SetFWDData(IN IF_INDEX ifIndex,
                            IN IP6FW_MODID_E enFWDType,
                            IN ULONG ulSetValue);

extern VOID IP6_GetServiceNodeAndIFType(IN IF_INDEX ifIndex, OUT LIP_ADDR *pstLipNode, OUT UINT *puiIfType);

extern ULONG IP6_SetBPA(IN IF_INDEX ifIndex, IN const IP_BPA_BASE_S *pstBpaBase);
extern VOID IP6_SetLoadSharing(IN UINT uiLoadShareMode, IN UINT uiOptionBitMap);
extern ULONG IP6_GetFwdIndex(IN const MBUF_S *pstMBuf, OUT UINT32 *puiFwdIndex);
extern UINT32 IP6_GetFwdMode(VOID);
extern VOID IP6_SetLocalFirst(UINT uiLocalFirstCfg);

#else /* 仅在用户态提供 */

extern ULONG IP6_GetBpaStat(IN IF_INDEX ifIndex, OUT IP_BPA_STAT_S *pstStat);

extern ULONG IP6_GetMibStat (IN LIP_ADDR stLipNode, OUT IP6_STAT_S *pstIp6MibStat);

/* 显示接口信息时从转发模块获取接口转发统计 */
extern ULONG IP6_GetIfStat(IN IF_INDEX ifIndex, IN UINT uiCmd, OUT IP6_STAT_S *pstIP6Stat)      ;

extern ULONG IP6_GetIcmp6IfStat(IN IF_INDEX ifIndex, OUT ICMP6_IFSTAT_S *pstIcmp6IfStat);

/*****************************************************************************
    Func Name: IP6_GeneralCheckSum
 Date Created: 2013-01-06
       Author: j09070
        Input: uiSize    缓存长度
               pBuffer   缓存
       Output: 无
       Return: 校验和
      Caution: 返回结果是网络序
Enter context: 进程上下文
 Exit context: 无
  Locking seq: 无
  Description: 对数据计算校验和
------------------------------------------------------------------------------
 Modification History
 DATE        NAME             DESCRIPTION
 --------------------------------------------------------------------------

*****************************************************************************/
static inline USHORT IP6_GeneralCheckSum(IN UINT uiSize, IN VOID *pBuffer)
{
    UINT uiSum = 0;
    USHORT *pusBuffer = (USHORT *)pBuffer;
    USHORT usTemp = 0;

    while (1 < uiSize)
    {
        /*  This is the inner loop */
        uiSum += *pusBuffer;
        pusBuffer++;
        uiSize -= 2;
    }

    /*  Add left-over byte, if any */
    if (0 < uiSize)
    {
        *(UCHAR *)(&usTemp) = *(UCHAR *)pusBuffer;
    }

    uiSum += usTemp;
    
    /*  Fold 32-bit sum to 16 bits */
    while (0xffff < uiSum)
    {
        uiSum = (uiSum & 0xffff) + (uiSum >> 16);
    }

    uiSum = ~uiSum;

    return (USHORT)uiSum;
}

/*****************************************************************************
    Func Name: IP6_ChkSum
 Date Created: 2013-01-06
       Author: j09070
        Input: ucNextPro      上层协议(TCP、UDP等)
               uiOffset       上层协议头在缓存中的偏移
               uiBufferLen    缓存数据长度
               pBuffer        IPv6报文缓存，指向IPV6头
       Output: 无
       Return: 校验和
      Caution: 返回结果是网络序
Enter context: 进程上下文
 Exit context: 无
  Locking seq: 无
  Description: 用户态计算校验和的函数
------------------------------------------------------------------------------
 Modification History
 DATE        NAME             DESCRIPTION
 --------------------------------------------------------------------------

*****************************************************************************/
static inline USHORT IP6_ChkSum(IN UCHAR ucNextPro,
                                IN UINT uiOffset,
                                IN UINT uiBufferLen,
                                IN VOID *pBuffer)
{
    IP6_PSEUDO_HEADER_S stPseudoHeader;
    UCHAR *pucData = NULL;
    IP6_S *pstIp6 = NULL;
    USHORT usPesuChksum = 0;
    USHORT usDataChksum = 0;
    UINT uiChkSum = 0;
    UINT uiDataLen;

    memset(&stPseudoHeader, 0, sizeof(stPseudoHeader));
    /* 伪头 */
    pstIp6 = (IP6_S *)(VOID *)pBuffer;
    uiDataLen = uiBufferLen - uiOffset;
    stPseudoHeader.stIPv6SrcAddr = pstIp6->stIp6Src;
    stPseudoHeader.stIPv6DstAddr = pstIp6->stIp6Dst;
    stPseudoHeader.uiDataLen = htonl(uiDataLen);
    stPseudoHeader.ucNextPro = ucNextPro;

    /* 计算伪头的校验码 */
    usPesuChksum = IP6_GeneralCheckSum((UINT)sizeof(stPseudoHeader), (VOID *)&stPseudoHeader);

    /* 计算数据的校验码 */
    pucData = (UCHAR *)(VOID *)pBuffer + uiOffset;
    usDataChksum = IP6_GeneralCheckSum(uiDataLen, (VOID *)pucData);

    /* 对伪头的校验码和数据报文的校验码进行处理 */
    usPesuChksum = ~(UINT)usPesuChksum;
    usDataChksum = ~(UINT)usDataChksum;
    uiChkSum += usPesuChksum;
    uiChkSum += usDataChksum;
    if (0xffff < uiChkSum)
    {
        uiChkSum = (uiChkSum & 0xffff) + (uiChkSum >> 16);
    }

    uiChkSum = ~uiChkSum;

    /* 如果为UDP类型，当校验和为0 时，返回0xffff */
    if ((IPPROTO_UDP == ucNextPro) && (0 == uiChkSum))
    {
        uiChkSum = 0xffff;
    }

    return (USHORT)uiChkSum;
}
#endif /* end of __KERNEL__ */

extern ULONG IP6_GetIfMTU(IN IF_INDEX ifIndex, OUT UINT *puiMTU);

#ifdef  __cplusplus
}
#endif  /* end of __cplusplus */

#endif  /* end of _IP6FW_H_ */
