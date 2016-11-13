/*****************************************************************************
 Copyright (c) 2010 Hangzhou H3C Technologies Co., Ltd. All rights reserved.
------------------------------------------------------------------------------
                              adj.h
  Project Code: Comware
   Module Name: ADJ
  Date Created: 2010-07-01
        Author: z02256
   Description: Adj common driver macros/data types/interfaces between platform and driver
                for IPv4/IPv6
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------
  YYYY-MM-DD

*****************************************************************************/

#ifndef _DRV_ADJ_H_
#define _DRV_ADJ_H_

#ifdef __cplusplus
extern "C"{
#endif

/* 对于NickName使用单独的类型用于扩充 */
#ifndef TRILL_NICKNAME
#define TRILL_NICKNAME USHORT
#endif

/* 邻接表重刷驱动后，是否需要同步 */
#define L3_ADJACENT_RELOAD_NOSYN   0      /* 不需要同步 */
#define L3_ADJACENT_RELOAD_SYN     1      /* 需要同步 */

#define ADJ_NEXTHOP_CONTEXT_LEN 2     /* nexthop资源长度 */
#define ADJ_DRV_CONTEXT_LEN     6     /* 邻接表资源长度 */
#define MAX_LINKHEAD_SIZE       52UL  /* 最大链路层头长度 */

/* ADJ表项属性，主要用于在ARP和ND表项下发给驱动时，
  告知驱动下发的是否是静态长配置表项、以及表项出端口是否处于UP状态。
  驱动模块根据此属性做相应的处理。 */

#define ADJ_ENTRY_ATTRIBUTE_LONGSTATIC   (1<<0)  /* 静态长配置ARP或ND表项 */
#define ADJ_ENTRY_ATTRIBUTE_DOWN         (1<<1)  /* 表项所在端口down */
#define ADJ_ENTRY_ATTRIBUTE_MULTIPORT    (1<<2)  /* 多端口ARP或ND表项 */
#define ADJ_ENTRY_ATTRIBUTE_IPOEDHCP     (1<<3)  /* ipoe dhcp生成的表项 */
#define ADJ_ENTRY_ATTRIBUTE_TRILLREMOTE  (1<<4)  /* TRILL远端表项 */

/*****************关联模块定义**********************/
/* 通知驱动邻接表操作类型 */
typedef enum tagL3ADJ_LinkOpType
{
    L3_ADJLINK_ADD = 0,        /* 增加一条表项 */
    L3_ADJLINK_MOD,            /* 修改一条表项 */
    L3_ADJLINK_DEL,            /* 删除一条表项 */
    L3_ADJLINK_BUTT
} L3_ADJLINK_OPTYPE_E;

/* ADJ表项链路层业务类型 */
typedef enum tagADJ_ServiceType
{
    ADJ_SERVICE_UNKNOWN = 0,  /* 未知业务类型 */
    ADJ_SERVICE_ETH,          /* ETH业务类型 */
    ADJ_SERVICE_ATM,          /* ATM业务类型 */
    ADJ_SERVICE_PPP,          /* PPP业务类型 */
    ADJ_SERVICE_HDLC,         /* HDLC业务类型 */
    ADJ_SERVICE_FR,           /* FR业务类型*/
    ADJ_SERVICE_TUNNEL,       /* Tunnel业务类型*/
    ADJ_SERVICE_MTUNNEL,      /* MTunnel业务类型 */
    ADJ_SERVICE_TUNNELBDL,    /* Tunnel Bundlel业务类型 */
    ADJ_SERVICE_SSLVPNAC,     /* SSLVPN access业务类型 */
    ADJ_SERVICE_CMTUNNEL, /* CMTUNNEL业务类型 */    
    ADJ_SERVICE_BUTT       
} ADJ_SERVICE_TYPE_E;

/* ADJ表项链路层结构类型 */
typedef enum tagADJ_LinkMedia
{
    ADJ_LINKMEDIA_UNKNOWN = 0,   /* Unknown link */
    ADJ_LINKMEDIA_BROADCAST,     /* Broadcast link */
    ADJ_LINKMEDIA_P2P,           /* P2P link */
    ADJ_LINKMEDIA_NBMA,          /* NBMA link */
    ADJ_LINKMEDIA_BUTT           /* Butt */
} ADJ_LINKMEDIA_TYPE_E;

/* ADJ表项动作类型 */
typedef enum tagADJ_ActionType
{
    ADJ_ACTION_UNKNOWN = 0,   /* Unknown action */
    ADJ_ACTION_GLEAN,         /* Glean action */
    ADJ_ACTION_FORWARD,       /* Forward action */
    ADJ_ACTION_DROP,          /* Drop action */
    ADJ_ACTION_BLACKHOLE,     /* Black hole action */
    ADJ_ACTION_INLOOPBACK,    /* Loopback action */ 
    ADJ_ACTION_BUTT           /* Butt */
} ADJ_ACTION_TYPE_E;

/* 邻接表下发驱动结构 */
typedef struct tagL3_AdjacentLink
{
    UCHAR ucServiceType;                           /* Link layer service type */
    UCHAR ucLinkType;                              /* Link media type */
    UCHAR ucActionType;                            /* Action type */
    UCHAR ucAdjEntryAttribute;                     /* adj entry attribute */
    VRF_INDEX vrfIndex;                            /* VRF index */  
    INET_ADDR_S stIpAddr;                          /* IP address */
    IF_INDEX ifIndex;                              /* L3 interface index */
    IF_INDEX ifPortIfIndex;                        /* L2 Port ifindex */
    USHORT usSlot;                                 /* Slot ID */
    USHORT usMtu;                                  /* MTU */
    USHORT usVlanID;                               /* VLAN ID */
    USHORT usSecondVlanID;                         /* Second(Inner) VLAN ID */
    IF_INDEX ifPhyIfIndex;                         /* Physical index */
    IF_INDEX ifVaIfIndex;                          /* VA index */
    UINT uiUserId;                                 /* IPoE/portal User ID */
    UINT  uiVsiIndex;                              /* VSI Index */
    UINT  uiVsiLinkID;                             /* Link ID */
    UCHAR aucMac[MAC_ADDR_LEN];                    /* MAC address */
    UCHAR ucIpLhLen;                               /* IP link header length */
    UCHAR ucMplsLhLen;                             /* MPLS link header length */
    UCHAR aucIpLh[MAX_LINKHEAD_SIZE];              /* IP link header */
    UCHAR aucMplsLh[MAX_LINKHEAD_SIZE];            /* MPLS link header */
    UINT32 auiNexthopDrv[ADJ_NEXTHOP_CONTEXT_LEN]; /* Nexthop driver context */ 
    UINT32 auiDrvReserved[ADJ_DRV_CONTEXT_LEN];    /* ADJ driver context */
    UINT64 uiAdjDrvContext;                        /* ADJ index */
    UINT64 auiTrillVnDrvContext[2];                /* 远端ARP引用的VN的drvcontext  */
} L3_ADJACENT_LINK_S;

/*******************************************************************/
/*                      产品定制邻接表规格结构定义                 */
/*******************************************************************/
/* ARP模块定制结构 */
typedef struct tagARP_Conf
{
    ULONG ulMaxARPNum;           /* ARP表项规格定制 */
    ULONG ulMaxMultiportNum;     /* 多端口ARP最大规格 */
    ULONG ulMaxBlackRouteNum;    /* ARP黑洞路由定制 */     
    ULONG ulDefaultRateLimit;    /* ARP限速定制 */
    ULONG ulMaxARPSuppNum;       /* evi ARP suppression表项规格定制 */
    BOOL_T bSupportMFF;          /* 是否支持MFF */
    BOOL_T bSupportFastReply;    /* 是否支持代答 */
    BOOL_T bSupportDetection;    /* 是否支持Detection */
    BOOL_T bSupportSnooping;     /* 是否支持Snooping */
    BOOL_T bSupportPortFilter;   /* 是否支持PortFilter */
    BOOL_T bSupportEviFastReply; /* 是否支持Evi快速应答 */
    BOOL_T bSupportOverlaySup;   /* 是否支持Overlay ARP suppression */    
    BOOL_T bSupportVpwsSup;      /* 是否支持Vpws ARP suppression */    
    UINT uiMaxSnoopingNum;       /* ARP Snooping表项最大数目 */
    UINT uiMaxOverlaySupNum;     /* Overlay ARP suppression表项最大数目 */
    UINT uiMaxVpwsSupNum;        /* Vpws ARP suppression表项最大数目 */
} ARP_CONF_S;

/* WADJ4模块定制结构 */
typedef struct tagWADJ4_Conf
{
    ULONG ulMaxWADJNum;   /* WADJ4表项规格定制 */
} WADJ4_CONF_S;

/* WADJ6模块定制结构 */
typedef struct tagWADJ6_Conf
{
    ULONG ulMaxWADJ6Num;   /* WADJ6表项规格定制 */
} WADJ6_CONF_S;

/* ND模块定制结构 */
typedef struct tagND_Conf
{
    ULONG ulMaxNDNum;                    /* ND表项规格定制 */
    BOOL_T bSupportVpwsSup;              /* 是否支持ND vpws suppression */
    UINT uiMaxVpwsSupNum;                /* ND vpws suppression表项最大数目 */
    BOOL_T bSupportSnooping;   /* 是否支持Snooping */
    BOOL_T bSupportDetection;  /* 是否支持Detection */
    UINT uiMaxSnoopingNum;     /* ND Snooping最大表项个数 */
    BOOL_T bSupportRaguard;    /* 是否支持Ra guard */
    UINT uiMaxRaguardPolicyNum;  /* Ra guard最大策略个数 */
}ND_CONF_S;

/* IPv4邻接表刷新接口 */
extern ULONG ADJ4_ReloadByIfIp(IN IF_INDEX ifIndex, IN INADDR_S stIpAddr);
extern ULONG ADJ4_ReloadByIf(IN IF_INDEX ifIndex, IN BOOL_T bRefreshSubIf);
extern ULONG ADJ4_ReloadByPW(IN UINT uiVsiIndex, IN UINT uiLpwId);

extern ULONG ADJ4_ReloadNoResource(VOID);

/* MAC变化通知接口*/
extern ULONG ARP_MacNotify(IN const UCHAR aucMacAddr[MAC_ADDR_LEN], 
                           IN USHORT usVlanId, 
                           IN IF_INDEX ifIndexPort);
extern ULONG ARP_TrillMacNotify(IN const UCHAR aucMacAddr[MAC_ADDR_LEN],
                                IN USHORT usVlanId,
                                IN TRILL_NICKNAME nickNameRb);
/* IPv6邻接表刷新接口 */
extern ULONG ADJ6_ReloadByIfIp(IN IF_INDEX ifIndex, IN IN6ADDR_S stIpv6Addr);
extern ULONG ADJ6_ReloadByIf(IN IF_INDEX ifIndex, IN BOOL_T bRefreshSubIf);
extern ULONG ADJ6_ReloadNoResource(VOID);
extern ULONG ADJ6_ReloadByPW(IN UINT uiVsiIndex, IN UINT uiLpwId);

/* MAC变化通知接口*/
extern ULONG ND_MacNotify(IN const UCHAR aucMacAddr[MAC_ADDR_LEN],
                          IN USHORT usVlanId,
                          IN IF_INDEX ifIndexPort);
extern ULONG ND_TrillMacNotify(IN const UCHAR aucMacAddr[MAC_ADDR_LEN],
                               IN USHORT usVlanId,
                               IN TRILL_NICKNAME nickNameRb);

/* ARP模块初始化相关函数 */
extern ULONG ARP_Init_Conf(IN const ARP_CONF_S *pstConfData);
extern ULONG ARP_ModInit(INOUT LPVOID pStartContext);
extern ULONG ARP_ModRun(INOUT LPVOID pStartContext);
extern ULONG USR_ARP_Init (INOUT LPVOID pStartContext);
extern ULONG USR_ARP_Run(INOUT LPVOID pStartContext);

/* ARP模块MDC回调函数 */
extern ULONG ARP_MdcCallBack(IN UINT uiEvent, IN VOID *pData);

/* WADJ4模块初始化相关函数 */
extern ULONG WADJ4_Init_Conf(IN const WADJ4_CONF_S *pstConfData);
extern ULONG WADJ4_ModInit(INOUT LPVOID pStartContext);
extern ULONG WADJ4_ModRun(INOUT LPVOID pStartContext);
extern ULONG USR_WADJ4_Init (INOUT LPVOID pStartContext);
extern ULONG USR_WADJ4_Run(INOUT LPVOID pStartContext);

/* WADJ4模块MDC回调函数 */
extern ULONG WADJ4_MdcCallBack(IN UINT uiEvent, IN VOID *pData);

/* ND模块初始化相关函数 */
extern ULONG ND_Init_Conf(IN const ND_CONF_S *pstConfData);
extern ULONG ND_ModInit(INOUT LPVOID pStartContext);
extern ULONG ND_ModRun(INOUT LPVOID pStartContext);
extern ULONG USR_ND_Init (INOUT LPVOID pStartContext);
extern ULONG USR_ND_Run(INOUT LPVOID pStartContext);

/* WADJ6模块初始化相关函数 */
extern ULONG WADJ6_Init_Conf(IN const WADJ6_CONF_S *pstConfData);
extern ULONG WADJ6_ModInit(INOUT LPVOID pStartContext);
extern ULONG WADJ6_ModRun(INOUT LPVOID pStartContext);
extern ULONG USR_WADJ6_Init (INOUT LPVOID pStartContext);
extern ULONG USR_WADJ6_Run(INOUT LPVOID pStartContext);

/* WADJ6模块MDC回调函数 */
extern ULONG WADJ6_MdcCallBack(IN UINT uiEvent, IN VOID *pData);

/* ND模块MDC回调函数 */
extern ULONG ND_MdcCallBack(IN UINT uiEvent, IN VOID *pData);

/* ADJ模块初始化相关函数 */
extern ULONG ADJ_ModInit(INOUT LPVOID pStartContext);
extern ULONG ADJ_ModRun(INOUT LPVOID pStartContext);

/* ADJ模块MDC处理函数 */
extern ULONG ADJ_MdcCallBack(IN UINT uiEvent, IN VOID *pData);

#ifdef __KERNEL__ /* used only in kernel space */

#ifndef _SYS_FSBUF_H_
#include <sys/fsbuf.h>
#endif

/* 驱动特殊发送函数 */
typedef ULONG (*ADJ_DriverSpecialOut_PF)(IN const UINT32 *puiNexthopDrv,
                                         IN const UINT32 *puiDrvContext, 
                                         INOUT MBUF_S *pMbuf);

typedef ULONG (*ADJ_DriverRawSpecialOut_PF)(IN const IF_INDEX ifIndex,
                                            IN const UINT32 *puiNexthopDrv,
                                            IN const UINT32 *puiDrvContext,
                                            INOUT FSBUF_PKTINFO_S *pstPktInfo,
                                            INOUT FSBUF_BLOCKINFO_S *pstBlockInfo);

/* 注册特殊发送函数的获取函数 */
extern ULONG ADJ_RegDriverSpecialOut(IN ADJ_DriverSpecialOut_PF pfDriverSpecialout,
                                     IN ADJ_DriverRawSpecialOut_PF pfDriverRawSpecialout);

#endif

#ifdef __cplusplus
}
#endif /* end of __cplusplus */

#endif /* end of _DRV_ADJ_H_ */

