/*******************************************************************************
 Copyright (c) 2010, Hangzhou H3C Technologies Co., Ltd. All rights reserved.
--------------------------------------------------------------------------------
                              ip6addr.h
  Project Code: Comware V7
   Module Name: IP6Addr
  Date Created: 2010-7-31
        Author: zhouwenming 02256
   Description: Header file for IP6Addr module

--------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
--------------------------------------------------------------------------------
  YYYY-MM-DD

*******************************************************************************/

#ifndef _SYS_IP6ADDR_H_
#define _SYS_IP6ADDR_H_

#ifdef  __cplusplus
extern "C"{
#endif




#define IP6ADDR_FEATURENAME ("ip6addr")     /* 地址管理模块特性名称 */
#define IP6ADDR_DBM_EFFECTED_CFG ("eIP6ADDR")       /* 生效数据库名 */
#define IP6ADDR_DBM_PENDING_CFG ("pIP6ADDR")       /* 未生效数据库名 */

/*********************************************************************************************************************
                         地址来源模块需要关心的定义(BEGIN)
*********************************************************************************************************************/
/* IPv6地址来源类型定义 */
#define IP6ADDR_ORIGIN_TYPE_NONE          (1<<0)      /* 来源无效值 */
#define IP6ADDR_ORIGIN_TYPE_INLOOPBACK    (1<<1)      /* 内部环回地址，::1 */
#define IP6ADDR_ORIGIN_TYPE_MANUAL        (1<<2)      /* 手工配置地址 */
#define IP6ADDR_ORIGIN_TYPE_EUI64         (1<<3)      /* 手工配置地址 */
#define IP6ADDR_ORIGIN_TYPE_DHCP          (1<<4)      /* DHCP分配地址 */
#define IP6ADDR_ORIGIN_TYPE_VRRP          (1<<5)      /* VRRP地址 */
#define IP6ADDR_ORIGIN_TYPE_LINKLAYER     (1<<6)      /* 无状态自动配置地址 */
#define IP6ADDR_ORIGIN_TYPE_RANDOM        (1<<7)      /* 随机地址，比如RFC3041 privacy address */
#define IP6ADDR_ORIGIN_TYPE_VSRP          (1<<8)      /* 手工配置VSRP虚地址 */
#define IP6ADDR_ORIGIN_TYPE_TRILL_VR      (1<<9)      /* TRILL VR地址 */
#define IP6ADDR_ORIGIN_TYPE_AFT           (1<<10)     /* AFT地址 */
#define IP6ADDR_ORIGIN_TYPE_PROXY         (1<<11)     /* 通过关联动态前缀生成的地址 */
#define IP6ADDR_ORIGIN_TYPE_MTUNNEL       (1<<12)     /* MTUNNEL地址 */
#define IP6ADDR_ORIGIN_TYPE_ALL           0xFFFFFFFF  /* 所有来源类型合集 */

/* 地址来源设置的属性宏定义 */
#define IP6ADDR_ORIGIN_ATTR_DEPENDENT     (1<<0)      /* 地址无法使能接口ipv6功能 */
#define IP6ADDR_ORIGIN_ATTR_NODAD         (1<<1)      /* 地址无需做DAD，比如VRRP地址 */
#define IP6ADDR_ORIGIN_ATTR_EUI64         (1<<2)      /* EUI-64地址，地址管理需要加上接口ID生成完整的地址 */
#define IP6ADDR_ORIGIN_ATTR_ANYCAST       (1<<3)      /* 任播地址 */

/* 虚地址类型集合 */
#define IP6ADDR_ORIGIN_TYPE_VR (IP6ADDR_ORIGIN_TYPE_VRRP | IP6ADDR_ORIGIN_TYPE_TRILL_VR)

/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
                         地址来源模块需要关心的定义(END)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/


/*********************************************************************************************************************
                         地址使用模块需要关心的定义(BEGIN)
*********************************************************************************************************************/
/* 地址状态宏定义 */
#define IP6ADDR_STATUS_PREFERRED     (1<<1)     /* preferred状态，地址可用于源目的地址 */
#define IP6ADDR_STATUS_DEPRECATED    (1<<2)     /* deprecated状态，地址可用于目的地址，不建议用于源地址 */
#define IP6ADDR_STATUS_INVALID       (1<<3)     /* 无效状态，不能用于通信 */
#define IP6ADDR_STATUS_TENTATIVE     (1<<6)     /* 未做地址重复检查的状态，不能用于通信 */
#define IP6ADDR_STATUS_DUPLICATE     (1<<7)     /* 重复状态，不能用于通信 */

#define IP6ADDR_STATUS_ACTIVE        (IP6ADDR_STATUS_PREFERRED|IP6ADDR_STATUS_DEPRECATED)
#define IP6ADDR_STATUS_ALL           (0xFFFFFFFF)

/* 地址属性宏定义 */
#define IP6ADDR_PROPERTY_ANYCAST     (1<<0)     /* 任播地址 */
#define IP6ADDR_PROPERTY_LLAMAIN     (1<<1)     /* LINKLOCAL主地址，决定网络层UP/DOWN，接口使能IPV6时必须要有此地址 */
#define IP6ADDR_PROPERTY_GATEWAY     (1<<2)     /* 可作RA网关 */

/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
                         地址使用模块需要关心的定义(END)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/

#define IP6ADDR_DAD_OK           1    /* DAD通过，无重复地址 */
#define IP6ADDR_DAD_DUPLICATE    2    /* DAD检测到重复地址 */

/*******************************ipv6地址配置类型定义**********************************/
#define IP6CFG_CFG_TYPE_LINKLOCAL    (1<<1)      /* 链路本地地址配置 */
#define IP6CFG_CFG_TYPE_GLOBAL       (1<<2)      /* 全球单播地址配置 */
#define IP6CFG_CFG_TYPE_EUI64        (1<<3)      /* EUI64地址配置 */
#define IP6CFG_CFG_TYPE_ANYCAST      (1<<4)      /* 任播地址配置 */


/***************************************** 错误码定义 *****************************************/
/* Error facility */
#define ERROR_IP6ADDR_FACILITY       ERROR_GET_MODULEFACILITY(MID_IP6ADDR)

/* Error code of success level: 0x0~0x20 */

/* Error code of information level: 0x21~0x40 */
/* 地址冲突 */
#define ERROR_IP6ADDR_CONFLICT       ERROR_DEFINE(ERROR_SEVERITY_INFO, ERROR_IP6ADDR_FACILITY, 0x21UL)
#define ERROR_IP6PREFIX_EXCEED       ERROR_DEFINE(ERROR_SEVERITY_INFO, ERROR_IP6ADDR_FACILITY, 0xAAUL)

/* Error code of warning level: 0x41~0x60 */

/* Error code of error level: 0x61~0x80 */
/* 地址或者掩码非法 */
#define ERROR_IP6ADDR_INVALIDADDR    ERROR_DEFINE(ERROR_SEVERITY_ERROR, ERROR_IP6ADDR_FACILITY, 0x61UL)
/* 相关操作无法执行 */
#define ERROR_IP6ADDR_ERRCONNECTION  ERROR_DEFINE(ERROR_SEVERITY_ERROR, ERROR_IP6ADDR_FACILITY, 0x62UL)

/* 地址信息结构 */
ISSU typedef struct tagIP6ADDR_Key
{
    VRF_INDEX vrfIndex;
    IF_INDEX  ifIndex;
    IN6ADDR_S stAddr;
    UINT      uiPrefixLen;
    UINT      uiOriginType;      /* 地址来源类型，见地址来源类型 */
}IP6ADDR_KEY_S;

#define IP6ADDR_ISSAMEADDR(_pstKeySrcM, _pstKeyDstM) \
        ( ((_pstKeySrcM)->vrfIndex == (_pstKeyDstM)->vrfIndex) && \
          ((_pstKeySrcM)->ifIndex == (_pstKeyDstM)->ifIndex) && \
          (0 == IN6ADDR_Cmp(&(_pstKeySrcM)->stAddr, &(_pstKeyDstM)->stAddr)) && \
          ((_pstKeySrcM)->uiPrefixLen == (_pstKeyDstM)->uiPrefixLen) && \
          ((_pstKeySrcM)->uiOriginType == (_pstKeyDstM)->uiOriginType) )

/* 地址信息结构 */
ISSU typedef struct tagIP6ADDR_Info
{
    IP6ADDR_KEY_S stAddrKey;
    UINT          uiProperty;     /* 地址属性，见地址属性宏定义 */
    UINT          uiStatus;       /* 地址状态，见地址状态宏定义 */
}IP6ADDR_INFO_S;

#define IP6ADDR_LIFETIME_INFINITE    0xFFFFFFFF

ISSU typedef struct tagIP6ADDR_LifeTime
{
    UINT uiValidTime;      /* 生存时间，等于prefer状态时间+deprecated状态时间 */
    UINT uiPreferrdTime;   /* prefer状态的时间 */
}IP6ADDR_LIFETIME_S;

ISSU typedef struct tagIP6ADDR_OriginInfo
{
    IP6ADDR_KEY_S      stAddrKey;
    UINT               uiOriginAttr;    /* 地址来源属性 */
    IP6ADDR_LIFETIME_S stLifeTime;      /* 地址生存时间 */
}IP6ADDR_ORIGIN_INFO_S;

/* 完整的地址信息 */
#pragma pack(4)
ISSU typedef struct tagIP6ADDR_FullInfo
{
    IP6ADDR_INFO_S stIP6Addr;
    UINT uiOriginSubId;      /* 来源可能分布于不同板，此subID标识位置，由来源指定 */
    UINT uiOriginAttr;
    UINT uiFlag;             /* 废弃字段 */
    IP6ADDR_LIFETIME_S stLifeTime;
    UINT uiCreateTime;
    UINT uiOriginalCreateTime;
    UINT uiLastChangedTime;
    UINT uiEpoch;
    UINT64 uiContext;
}IP6ADDR_FULLINFO_S;
#pragma pack()

#define IP6ADDR_PREFIX_ORIGIN_NDCFG     (1<<0) /* ND手工配置 */
#define IP6ADDR_PREFIX_ORIGIN_MANUAL    (1<<1) /*手工配置，表示手工配置的地址生成*/
#define IP6ADDR_PREFIX_ORIGIN_WELLKNOWN (1<<2) /* 知名前缀 */
#define IP6ADDR_PREFIX_ORIGIN_DHCP      (1<<3) /* DHCP得到的前缀 */
#define IP6ADDR_PREFIX_ORIGIN_ROUTERADV (1<<4) /* RA公告生成的前缀 */
#define IP6ADDR_PREFIX_ORIGIN_OTHER     (1<<5) /* 除以上明确定义的来源外的其他来源 */
#define IP6ADDR_PREFIX_ORIGIN_PREFIXCFG (1<<6) /* IPv6前缀管理静态配置 */
#define IP6ADDR_PREFIX_ORIGIN_DHCPPREFIXALLOC (1<<7) /* DHCP前缀申请得到的前缀*/
#define IP6ADDR_PREFIX_ORIGIN_PROXY (1<<8) /* 用于前缀代理的前缀 */
#define IP6ADDR_PREFIX_ORIGIN_ALL       0xFFFFFFFF /* 所有来源 */

/**************Begin:涉及与DHCPv6 Client通信相关数据结果定义******************/
#define IP6ADDR_CFG_DHCPALLOC       (1<<0)    /* DHCPv6 Client 地址申请配置 */
#define IP6ADDR_CFG_DHCPINFO        (1<<1)    /* DHCPv6 Client 信息申请配置*/

ISSU typedef struct tagIP6AddrDhcpcCfg
{ 
    UINT   uiServType;         /* 服务类型 */
    UINT   uiOptionGrp;        /* 选项组号，如果没有设置选项组，则为全F*/
    UINT   uiRapidFlag;        /* Rapid 使能标记，ENABLE或者DISABLE */
}IP6ADDR_DHCPCCFG_S;
/**************Begin:涉及与DHCPv6 Client通信相关数据结构定义******************/

ISSU typedef struct tagIP6ADDR_PrefixKey 
{
    VRF_INDEX vrfIndex;         /* vrf索引 */
    IF_INDEX ifIndex;           /* 接口索引 */
    IN6ADDR_S stPrefixAddr;     /* 前缀地址 */
    UINT uiPrefixLength;        /* 前缀长度 */
    UINT uiPrefixOrigin;        /* 前缀来源，见前缀来源类型宏定义*/
}IP6ADDR_PREFIX_KEY_S;

ISSU typedef struct tagIP6ADDR_Prefix
{
    IP6ADDR_PREFIX_KEY_S   stPrefixKey;     /* prefix关键信息 */
    IP6ADDR_LIFETIME_S     stLifeTime;      /* 有效时间 */
    UCHAR                  ucFlags;         /* 前缀标志 */
}IP6ADDR_PREFIX_S;

/* 完整的前缀信息 */
ISSU typedef struct tagIP6ADDR_PREFIX_FullInfo
{
    IP6ADDR_PREFIX_S    stPrefix;
    UINT                uiCreateTime;
    UINT                uiEpoch;
}IP6ADDR_PREFIX_FULLINFO_S;

ISSU typedef struct tagIP6ADDR_GatewayKey 
{
    IF_INDEX ifIndex;            /* 接口索引 */
    IN6ADDR_S stGatewayAddr;     /* 网关地址 */
    VRF_INDEX vrfIndex;
}IP6ADDR_GATEWAY_KEY_S;

ISSU typedef struct tagIP6ADDR_GATEWAY
{
    IP6ADDR_GATEWAY_KEY_S   stGatewayKey;
    UCHAR                   ucFlag;             /* RA报文中的标记信息 */
    CHAR                    cPriority;
    UINT                    uiRouterLifetime;
    UINT                    uiCreateTime;
    UINT                    uiEpoch;
}IP6ADDR_GATEWAY_S;

/* IP6ADDR前缀配置结构 */
ISSU typedef struct tagIP6ADDR_PREFIXCONFIG
{
    IF_INDEX ifIndex;
    VRF_INDEX vrfIndex;             /* VRF索引 */
    IN6ADDR_S stPrefix;             /* 前缀地址 */ 
    UINT uiValidTime;               /* 前缀有效生命周期 */
    UINT uiPreferredTime;           /* 前缀首选生命周期 */   
    USHORT usPrefixLen;             /* 前缀掩码长度 */
    UCHAR ucLAFlag;                 /* OnLink和auto标志 */
    BOOL_T bIsUndo;                 /* UNDO标识，有undo:1  无undo:0 */
    BOOL_T bIsDefaultPara;          /*标识是否携带参数，无参数:1，带参数:0*/
}IP6ADDR_PREFIXCONFIG_S;

#ifndef __KERNEL__
extern INT IP6Addr_Open(VOID);
extern VOID IP6Addr_Close(IN INT iHandle);

extern ULONG IP6Addr_AddAddr(IN INT iHandle, IN const IP6ADDR_ORIGIN_INFO_S *pstIP6AddrOrigin, IN UINT uiFlags);
extern ULONG IP6Addr_DelAddr(IN INT iHandle, IN const IP6ADDR_ORIGIN_INFO_S *pstIP6AddrOrigin, IN UINT uiFlags);

extern ULONG IP6Addr_SmoothStart(IN INT iHandle, IN UINT uiOriginType);
extern ULONG IP6Addr_SmoothEnd(IN INT iHandle, IN UINT uiOriginType);

/**************Begin:IPv6前缀管理相关函数*********************/
extern ULONG IP6Addr_GetPrefixByIndex(IN INT iHandle, IN UINT uiPrefixIndex, OUT IP6ADDR_PREFIX_S *pstInfo);
extern ULONG IP6Addr_AddPrefixByIndex(IN INT iHandle,
                                      IN UINT uiPrefixIndex,
                                      IN const IP6ADDR_PREFIX_S *pstInfo,
                                      IN UINT uiFlags);
extern ULONG IP6Addr_DelPrefixByIndex(IN INT iHandle,
                                      IN UINT uiPrefixIndex,
                                      IN UINT uiPrefixOrigin,
                                      IN UINT uiFlags );
extern ULONG IP6Addr_PrefixSmoothStart(IN INT iHandle, IN UINT uiOriginType);
extern ULONG IP6Addr_PrefixSmoothEnd(IN INT iHandle, IN UINT uiOriginType);
/**************End:IPv6前缀管理相关函数*********************/

extern ULONG IP6Cfg_AddManualAddr(IN IF_INDEX ifIndex, IN const IN6ADDR_S *pstAddr, IN UINT uiPrefixLen, IN UINT uiCfgType);
extern ULONG IP6Cfg_DelManualAddr(IN IF_INDEX ifIndex, IN const IN6ADDR_S *pstAddr, IN UINT uiPrefixLen, IN UINT uiCfgType);

#endif

extern ULONG IP6Addr_GetLinkLocalAddr(IN IF_INDEX ifIndex, OUT IP6ADDR_INFO_S *pstIP6Addr);

extern ULONG IP6Addr_MatchBestNet(IN IF_INDEX ifIndex, 
                                  IN const IN6ADDR_S *pstIP6AddrIn, 
                                  IN UINT uiStatus, 
                                  OUT IP6ADDR_INFO_S *pstIP6Addr);

extern ULONG IP6Addr_GetAddrInfoInVrf(IN VRF_INDEX vrfIndex, 
                                      IN const IN6ADDR_S *pstIP6AddrIn, 
                                      IN UINT uiOriginType, 
                                      IN UINT uiStatus, 
                                      OUT IP6ADDR_INFO_S *pstIP6Addr);
extern ULONG IP6Addr_GetAddrInfo(IN IF_INDEX ifIndex, 
                                 IN const IN6ADDR_S *pstIP6AddrIn, 
                                 IN UINT uiOriginType, 
                                 IN UINT uiStatus, 
                                 OUT IP6ADDR_INFO_S *pstIP6Addr);

extern ULONG IP6Addr_GetFirstAddr(IN IF_INDEX ifIndex, IN UINT uiScope, OUT IP6ADDR_INFO_S *pstIP6Addr);
extern ULONG IP6Addr_GetNextAddr(IN IF_INDEX ifIndex, IN UINT uiScope, INOUT IP6ADDR_INFO_S *pstIP6Addr);

extern ULONG IP6Addr_SelectSrcAddr(IN const IN6ADDR_S *pstDstAddr,
                                   IN VRF_INDEX vrfIndex, 
                                   IN IF_INDEX ifOutIndex,
                                   OUT IN6ADDR_S *pstSrcAddr);

extern ULONG IP6Addr_SelectSrcAddrOnIf(IN const IN6ADDR_S *pstDstAddr,
                                       IN IF_INDEX ifSrcIndex,
                                       OUT IN6ADDR_S *pstSrcAddr);
ULONG IP6CFG_DhcpAlloc(IN UINT uiRapidFlag,
                       IN UINT uiOptionGrp,
                       IN BOOL_T bIsUndo,
                       IN IF_INDEX ifIndex);
ULONG IP6CFG_GetFirstFullPrefix(IN IF_INDEX ifIndex, 
                                IN UINT uiPrefixOrigin,
                                OUT IP6ADDR_PREFIX_FULLINFO_S *pstFullPrefix );
ULONG IP6CFG_GetNextFullPrefix(IN IF_INDEX ifIndex, 
                               IN UINT uiPrefixOrigin,
                               INOUT IP6ADDR_PREFIX_FULLINFO_S *pstFullPrefix );
ULONG IP6CFG_SetRAPrefix(IN IP6ADDR_PREFIXCONFIG_S *pstPrefix );

/* =============================简单命令行summary命令使能============================= */
ULONG IP6Addr_GetIp6Summary(IN ULONG ulBufLen, OUT CHAR *pcString);

#ifdef __KERNEL__
extern VOID IP6Addr_NotifyDADResult( IN IF_INDEX ifIndex, IN const IN6ADDR_S *pstIP6Addr, IN UINT uiDADResult );

/* 接口上的LinkLocal地址设置为不允许作为RA源地址，即不作为下挂主机网关 */
extern ULONG IP6Addr_LinkLocalAddrRAHalt(IN IF_INDEX ifIndex);
/* 接口上的LinkLocal地址设置为允许作为RA源地址(默认情况)，即可以作为下挂主机网关 */
extern ULONG IP6Addr_LinkLocalAddrRANoHalt(IN IF_INDEX ifIndex);

extern BOOL_T IP6Addr_IsMCAddrOnIf(IN IF_INDEX ifIndex, IN const IN6ADDR_S *pstIP6MCAddr);
extern ULONG IP6Addr_AddMCAddr(IN IF_INDEX ifIndex, IN const IN6ADDR_S *pstIP6MCAddr);
extern ULONG IP6Addr_DelMCAddr(IN IF_INDEX ifIndex, IN const IN6ADDR_S *pstIP6MCAddr);

/* 前缀相关接口 */
extern ULONG IP6Addr_GetPrefixInfo( IN IF_INDEX ifIndex,
                                    IN const IN6ADDR_S *pstPrefixAddr,
                                    IN UINT uiPrefixLength,
                                    IN UINT uiPrefixOrigin,
                                    OUT IP6ADDR_PREFIX_S *pstPrefix);
extern ULONG IP6Addr_GetFirstPrefix(IN IF_INDEX ifIndex, 
                                    IN UINT uiPrefixOrigin,
                                    OUT IP6ADDR_PREFIX_S *pstPrefix );
extern ULONG IP6Addr_GetNextPrefix(IN IF_INDEX ifIndex, 
                                   IN UINT uiPrefixOrigin,
                                   INOUT IP6ADDR_PREFIX_S *pstPrefix );


/* 无状态自动配置功能接口 */                                    
extern BOOL_T IP6Addr_IsStatelessAutoCfg( IN IF_INDEX ifIndex );                                    
extern ULONG IP6Addr_RefreshRAPrefix( IN const IP6ADDR_PREFIX_S *pstPrefix );
extern ULONG IP6Addr_RefreshDefaultRouter( IN IF_INDEX ifIndex, 
                                           IN const IN6ADDR_S *pstRouterAddr, 
                                           IN UCHAR ucFlags, 
                                           IN UINT uiLifeTime );

#endif


/**************************************************************************************************************
                                地址事件注册接口(BEGIN)
**************************************************************************************************************/
#define IP6ADDR_EVENT_ADDR            ((1<<0)<<16)   /* 地址事件 */
#define IP6ADDR_EVENT_PREFIX          ((1<<1)<<16)   /* 前缀事件 */
#define IP6ADDR_EVENT_GATEWAY         ((1<<2)<<16)   /* 网关事件事件 */
#define IP6ADDR_EVENT_PREFIXM         ((1<<3)<<16)   /* 前缀管理事件发布 */


#define IP6ADDR_EVENT_ADDR_ADD         (IP6ADDR_EVENT_ADDR|(1<<0))   /* 地址添加后事件 */
#define IP6ADDR_EVENT_ADDR_UPDATE      (IP6ADDR_EVENT_ADDR|(1<<1))   /* 地址状态改变后事件 */
#define IP6ADDR_EVENT_ADDR_PRE_DELETE  (IP6ADDR_EVENT_ADDR|(1<<2))   /* 地址删除前事件 */
#define IP6ADDR_EVENT_ADDR_DELETE      (IP6ADDR_EVENT_ADDR|(1<<3))   /* 地址删除后事件 */

#define IP6ADDR_EVENT_PREFIX_ADD       (IP6ADDR_EVENT_PREFIX|(1<<0))   /* 前缀添加后事件 */
#define IP6ADDR_EVENT_PREFIX_DELETE    (IP6ADDR_EVENT_PREFIX|(1<<1))   /* 前缀删除后事件 */

#define IP6ADDR_EVENT_GATEWAY_ADD     (IP6ADDR_EVENT_GATEWAY|(1<<0))   /* 网关添加后事件 */
#define IP6ADDR_EVENT_GATEWAY_DELETE  (IP6ADDR_EVENT_GATEWAY|(1<<1))   /* 网关删除后事件 */

#define IP6ADDR_EVENT_PREFIXM_ADD      (IP6ADDR_EVENT_PREFIXM|(1<<0))   /* 前缀管理添加后事件 */
#define IP6ADDR_EVENT_PREFIXM_DELETE   (IP6ADDR_EVENT_PREFIXM|(1<<1))   /* 前缀管理删除后事件 */


typedef VOID ( *IP6Addr_ProcAddrEvent_PF ) ( IN UINT uiAddrEvent, IN const IP6ADDR_INFO_S *pstIP6Addr );

typedef VOID ( *IP6Addr_ProcPrefixEvent_PF ) ( IN UINT uiPrefixEvent, IN const IP6ADDR_PREFIX_S *pstPrefix );

typedef VOID ( *IP6Addr_ProcGatewayEvent_PF ) ( IN VRF_INDEX vrfIndex, 
                                                IN UINT uiGatewayEvent, 
                                                IN IF_INDEX ifIndexOut, 
                                                IN const IN6ADDR_S *pstGateway );

#ifdef __KERNEL__

typedef ULONG IP6ADDR_EVENT_HANDLE;
#define IP6ADDR_EVENT_INVALID_HANDLE (IP6ADDR_EVENT_HANDLE)0

extern IP6ADDR_EVENT_HANDLE IP6Addr_KRegisterAddrEvent( IN UINT uiMID,
                                                        IN UINT uiEventType,
                                                        IN UINT uiOriginType,
                                                        IN UINT uiStatus, 
                                                        IN IP6Addr_ProcAddrEvent_PF pfEventProc );
extern VOID IP6Addr_KDeregisterAddrEvent( IN IP6ADDR_EVENT_HANDLE hIP6AddrEvent );

#else
INT IP6Addr_Event_Init(VOID);
VOID IP6Addr_Event_Fini(IN INT iFd);
extern ULONG IP6Addr_RegisterAddrEvent(IN UINT uiMID,
                                       IN UINT uiAddrEvent,
                                       IN UINT uiAddrOrigin,
                                       IN UINT uiStatus, 
                                       IN IP6Addr_ProcAddrEvent_PF pfProcAddrEvent);

extern ULONG IP6Addr_RegisterPrefixEvent(IN UINT uiMID,
                                         IN UINT uiPrefixEvent,
                                         IN UINT uiPrefixOrigin,
                                         IN IP6Addr_ProcPrefixEvent_PF pfProcPrefixEvent );

extern ULONG IP6Addr_RegisterGatewayEvent(IN UINT uiMID,
                                          IN UINT uiGatewayEvent,
                                          IN IP6Addr_ProcGatewayEvent_PF pfProcGatewayEvent );

extern VOID IP6Addr_DeregisterEvent( UINT uiEvent );
extern ULONG IP6Addr_HandleEvent( IN INT iHandle );
extern ULONG IP6Addr_RequestBatchEvent( IN INT iHandle );

/***********Begin:IPv6前缀管理前缀事件相关函数***********/
typedef VOID ( *IP6ADDR_PREFIXMNGCHANGE_PROC_PF ) (IN UINT uiPrefixEvent, 
                                                IN UINT uiPrefixIndex,
                                                IN const IP6ADDR_PREFIX_S *pstPrefix );
extern ULONG IP6Addr_RegisterPrefixMngEvent(IN UINT uiMID,
                                            IN UINT uiPrefixEvent,
                                            IN IP6ADDR_PREFIXMNGCHANGE_PROC_PF pfProcPrefixEvent );
/***********End:IPv6前缀管理前缀事件相关函数***********/

/***********Begin:配置变化事件相关定义***********/
typedef ULONG ( *IP6ADDR_CONFIG_CHANGE_PROC_PF) (IN UINT uiIfCfgOper, 
                                                 IN IF_INDEX ifIndex, 
                                                 IN const VOID *pstCfgData ); 
extern ULONG IP6Addr_RegisterConfigChangeEvent(IN INT iHandle,
                                               IN UINT uiCfgType,
                                               IN IP6ADDR_CONFIG_CHANGE_PROC_PF pfCfgProc);
extern VOID IP6Addr_DeregisterConfigChangeEvent (IN INT iHandle, IN UINT uiCfgType);
extern ULONG IP6Addr_HandleConfigChangeEvent (IN INT iHandle);
extern ULONG IP6Addr_RequestConfig (IN INT iHandle);
/***********End:配置变化事件相关定义***********/

#endif


/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
                                地址事件注册接口(END)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/


/**************************************************************************************************************
                                根据IP获取MAC的接口(BEGIN)
**************************************************************************************************************/
#ifdef __KERNEL__

typedef enum tagIP6ADDR_GetMacPri
{
    IP6ADDR_GETMAC_PRI_DEFAULT = 0,
    IP6ADDR_GETMAC_PRI_AFT = 4,
    IP6ADDR_GETMAC_PRI_VRRP = 7,
    IP6ADDR_GETMAC_PRI_VSRPADDR = 8,
    IP6ADDR_GETMAC_PRI_TRILLVR = 9,
    IP6ADDR_GETMAC_PRI_BUTT = 10
}IP6ADDR_GETMAC_PRI_E;

#define IP6ADDR_GETMAC_FLAG_PROXY   (1<<0)   /* 此MAC非本机MAC，只是做代理回应的 */
#define IP6ADDR_GETMAC_FLAG_MACLB   (1<<1)   /* 此IP对应多个MAC进行负载分担，不能发送广播公告 */

typedef struct tagIP6ADDR_GetMacAttachInfo
{
    IN6ADDR_S stRequesterIP;                  /* 请求者的IP地址 */
    UCHAR     aucRequesterMac[MAC_ADDR_LEN];  /* 请求者的MAC地址 */
}IP6ADDR_GETMAC_ATTACHINFO_S;

typedef ULONG (*IP6ADDR_GETMAC_PF)( IN VRF_INDEX vrfIndex,   /* Vrf索引 */
                                    IN IF_INDEX ifIndex,     /* 接口索引 */
                                    IN const IN6ADDR_S *pstIPAddr, /* 指定的本机IP地址  */
                                    IN const IP6ADDR_GETMAC_ATTACHINFO_S *pstAttachInfo, /* 附加信息，该信息提供给VRRPE使用 */
                                    OUT UCHAR *pucLocalMac,  /* 指定IP地址对应的MAC地址 */
                                    OUT UINT *puiFlag);      /* 特殊用途标记，IP6ADDR_GETMAC_FLAG_xxx */
/* 返回值：*/
#define IP6ADDR_GETMAC_OK         0        /* 获取MAC成功 */
#define IP6ADDR_GETMAC_NOMAC      1        /* 本类型的地址不需要回应 */
#define IP6ADDR_GETMAC_NOTSPEC    2        /* 需要回应但是不关心MAC */
#define IP6ADDR_GETMAC_NOTSEND    3        /* 禁止回应，即使有其他类型也不能回应 */

extern VOID IP6Addr_RegGetMacCallback(IN UINT uiOriginType, 
                                      IN IP6ADDR_GETMAC_PRI_E enGetMacPrior, 
                                      IN IP6ADDR_GETMAC_PF pfGetMacFunc);
extern VOID IP6Addr_DeregGetMacCallback(IN UINT uiOriginType);

extern ULONG IP6Addr_GetMac(IN IF_INDEX ifIndex, 
                            IN VRF_INDEX vrfIndex,
                            IN const IN6ADDR_S *pstIPAddr, 
                            IN const IP6ADDR_GETMAC_ATTACHINFO_S *pstAttachInfo, 
                            OUT UCHAR *pucMac, 
                            OUT UINT *puiFlag);       /* 特殊用途标记，IP6ADDR_GETMAC_FLAG_xxx */

#endif
/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
                                根据IP获取MAC的接口(END)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/


/* =======================内核初始化启动函数声明============================================= */
#ifdef __KERNEL__
extern ULONG IP6Addr_StartInit(INOUT LPVOID pStartContext);
extern ULONG IP6Addr_StartRun(INOUT LPVOID pStartContext);

extern ULONG IP6Addr_MDCEventProc(IN UINT uiEvent, IN VOID *pData);
#endif
/**************************************************************************************************************
                                IPv6地址管理用户态内核态通信定义(BEGIN)
**************************************************************************************************************/
/* DUMP 缓存最大值 */
#define IP6ADDR_DUMP_BUFSIZE       2048UL

/* 获取内核事件变化消息的缓存大小 */
#define IP6ADDR_KEVENT_BUFSIZE     2048

/* 最大来源数 */
#define IP6ADDR_ORIGINTYPE_MAX (sizeof(UINT)*8)

/* 获取FULLINFO中地址关键信息的宏 */
#define IP6ADDR_ADDRKEY_INFULLINFO(pstAddrFullInfo)  (&((pstAddrFullInfo)->stIP6Addr.stAddrKey))

/* 将地址关键信息设为无效 */
#define IP6ADDR_SET_INVALIDADDRKEY(_pstIP6AddrKey) \
{\
    memset(_pstIP6AddrKey, 0, sizeof(IP6ADDR_KEY_S));\
    (_pstIP6AddrKey)->vrfIndex = (USHORT)L3VPN_INVALID_VRF_INDEX;\
}
/* 将前缀关键信息设为无效 */
#define IP6ADDR_SET_INVALIDPREKEY(_pstIP6PreKey) \
{\
    memset(_pstIP6PreKey, 0, sizeof(IP6ADDR_PREFIX_KEY_S));\
}

/* 判断地址关键信息是否为无效的宏 */
#define IP6ADDR_ISINVALIDADDRKEY(pstIP6AddrKey) ((USHORT)L3VPN_INVALID_VRF_INDEX == (pstIP6AddrKey)->vrfIndex)


typedef struct tagIP6ADDR_MulticastInfo
{
    IN6ADDR_S stIP6MutiAddr;                    /* 组播地址 */
    UINT uiRefCount;                            /* 组播地址的引用计数 */
}IP6ADDR_MULTICAST_S;

/* 内核地址管理提供的通用过滤结构 */
typedef struct tagIP6ADDR_Slt
{
    UINT uiScope;
    UINT uiFlag;             /* 标记结构中对应域是否有效。按位定义的成员是否有效无需定义 */
    VRF_INDEX vrfIndex;
    IF_INDEX ifIndex;
    IF_INDEX ifExcludeIndex;
    IN6ADDR_S stSearchAddr;
    UINT uiPrefixLen;
    UINT uiProperty;
    UINT uiExcludeProperty;
    UINT uiStatus;
    UINT uiOriginType;
    UINT uiOriginAttr;
    UINT uiExcludeOriginAttr;
}IP6ADDR_SLT_S;
/* 如下宏用于设置IP6ADDR_SLT_S对应位 */
#define IP6ADDR_SLT_IFINDEX             (1<<0)
#define IP6ADDR_SLT_EXCLUDEIFINDEX      (1<<1)
#define IP6ADDR_SLT_VRFINDEX            (1<<2)
#define IP6ADDR_SLT_SEARCHADDR          (1<<3)
#define IP6ADDR_SLT_PREFIXLEN           (1<<4)
#define IP6ADDR_SLT_PROPERTY            (1<<5)
#define IP6ADDR_SLT_EXCLUDEPROPERTY     (1<<6)
#define IP6ADDR_SLT_STATUS              (1<<7)
#define IP6ADDR_SLT_ORIGINTYPE          (1<<8)
#define IP6ADDR_SLT_ORIGINATTR          (1<<9)
#define IP6ADDR_SLT_EXCLUDEORIGINATTR   (1<<10)
#define IP6ADDR_SLT_SCOPE               (1<<11)

#define IP6ADDR_SLT_SETSCOPE(_pstSLT, _uiScope) \
{ \
    (_pstSLT)->uiScope = _uiScope; \
    (_pstSLT)->uiFlag |= IP6ADDR_SLT_SCOPE; \
}
#define IP6ADDR_SLT_SETIFINDEX(_pstSLT, _ifIndex) \
{ \
    (_pstSLT)->ifIndex = _ifIndex; \
    (_pstSLT)->uiFlag |= IP6ADDR_SLT_IFINDEX; \
}
#define IP6ADDR_SLT_SETEXCLUDEIFINDEX(_pstSLT, _ifIndex) \
{ \
    (_pstSLT)->ifExcludeIndex = _ifIndex; \
    (_pstSLT)->uiFlag |= IP6ADDR_SLT_EXCLUDEIFINDEX; \
}
#define IP6ADDR_SLT_SETVRFINDEX(_pstSLT, _vrfIndex) \
{ \
    (_pstSLT)->vrfIndex = _vrfIndex; \
    (_pstSLT)->uiFlag |= IP6ADDR_SLT_VRFINDEX; \
}
#define IP6ADDR_SLT_SETSEARCHADDR(_pstSLT, _pstSearchAddr) \
{ \
    memcpy(&(_pstSLT)->stSearchAddr, _pstSearchAddr, sizeof(IN6ADDR_S)); \
    (_pstSLT)->uiFlag |= IP6ADDR_SLT_SEARCHADDR; \
    if (BOOL_TRUE == IN6ADDR_IsMulticast(_pstSearchAddr))\
    {\
        (_pstSLT)->uiScope = (UINT)IN6ADDR_GetMCAddrScope(_pstSearchAddr);\
    }\
    else\
    {\
        (_pstSLT)->uiScope = (UINT)IN6ADDR_GetUCAddrScope(_pstSearchAddr);\
    }\
    (_pstSLT)->uiFlag |= IP6ADDR_SLT_SCOPE; \
}
#define IP6ADDR_SLT_SETPREFIXLEN(_pstSLT, _uiPrefixLen) \
{ \
    (_pstSLT)->uiPrefixLen = _uiPrefixLen; \
    (_pstSLT)->uiFlag |= IP6ADDR_SLT_PREFIXLEN; \
}
#define IP6ADDR_SLT_SETPROPERTY(_pstSLT, _uiProperty) \
{ \
    (_pstSLT)->uiProperty |= _uiProperty; \
    (_pstSLT)->uiFlag |= IP6ADDR_SLT_PROPERTY; \
}
#define IP6ADDR_SLT_SETEXCLUDEPROPERTY(_pstSLT, _uiExcludeProperty) \
{ \
    (_pstSLT)->uiExcludeProperty |= _uiExcludeProperty; \
    (_pstSLT)->uiFlag |= IP6ADDR_SLT_EXCLUDEPROPERTY; \
}
#define IP6ADDR_SLT_SETSTATUS(_pstSLT, _uiStatus) \
{ \
    (_pstSLT)->uiStatus |= _uiStatus; \
    (_pstSLT)->uiFlag |= IP6ADDR_SLT_STATUS; \
}
#define IP6ADDR_SLT_SETORIGINTYPE(_pstSLT, _uiOriginType) \
{ \
    (_pstSLT)->uiOriginType |= _uiOriginType; \
    (_pstSLT)->uiFlag |= IP6ADDR_SLT_ORIGINTYPE; \
}
#define IP6ADDR_SLT_SETORIGINATTR(_pstSLT, _uiOriginAttr) \
{ \
    (_pstSLT)->uiOriginAttr |= _uiOriginAttr; \
    (_pstSLT)->uiFlag |= IP6ADDR_SLT_ORIGINATTR; \
}
#define IP6ADDR_SLT_SETEXCLUDEORIGINATTR(_pstSLT, _uiExcludeOriginAttr) \
{ \
    (_pstSLT)->uiExcludeOriginAttr |= _uiExcludeOriginAttr; \
    (_pstSLT)->uiFlag |= IP6ADDR_SLT_EXCLUDEORIGINATTR; \
}

/* 过滤结构初始化宏 */
#define IP6ADDR_SLT_INIT(_pstSLT)\
{\
    memset((_pstSLT), 0, sizeof(IP6ADDR_SLT_S));\
}

#define IP6ADDR_SLT_CLEARFLAG(_pstSLT, _uiFlag) ((_pstSLT)->uiFlag &= ~(_uiFlag))
#define IP6ADDR_SLT_TESTFLAG(_pstSLT, _uiFlag) ((_pstSLT)->uiFlag & (_uiFlag))

/* LIPC端口定义 */
/* 公用全局知名端口 */


/* 事件发布的本地知名端口 */


/* 实时同步组播全局知名端口 */


/* 批量同步全局知名端口 */



/* CIOCTL相关数据结构定义 */

#define IP6ADDR_CIOCTL_CMD_DRVK    CIOCTL_CMD_DEF(CIOCTL_CMDMOD_IP6ADDR, 1)


#define IP6ADDR_DUMP_CMD_SHOW      DUMP_CMD_DEF(DUMP_CMDMOD_IP6ADDR, 1)

#define KEVENT_FAMILY_IP6ADDR  1
#define IP6ADDR_KEVENT_ID_NOTIFY   KEVENT_ID_DEF(KEVENT_FAMILY_IP6ADDR, 1)
#define IP6ADDR_KEVENT_ID_AUTO     KEVENT_ID_DEF(KEVENT_FAMILY_IP6ADDR, 2)

/* CIOCTL 消息类型 */
ISSU typedef enum tagIP6ADDR_CioctlMsgType
{
    IP6ADDR_CIOCTL_ADDADDR = 0,          /* 添加地址 */
    IP6ADDR_CIOCTL_DELADDR,              /* 删除地址 */
    IP6ADDR_CIOCTL_MODIFYADDR,           /* 修改地址状态 */
    IP6ADDR_CIOCTL_STARTDAD,             /* 发起DAD */
    IP6ADDR_CIOCTL_STOPDAD,              /* 结束DAD */
    IP6ADDR_CIOCTL_GETNEXTIF,            /* 获取下一个接口索引 */
    IP6ADDR_CIOCTL_GETNEXTBATCH,         /* 获取下一批地址 */
    IP6ADDR_CIOCTL_OVERLAPCHECK,         /* 地址overlap冲突检查 */
    IP6ADDR_CIOCTL_LOOKUPADDRESS,        /* 精确查找 */
    IP6ADDR_CIOCTL_SELECTSRCADDR,        /* 选择源地址 */
    IP6ADDR_CIOCTL_SELECTSRCADDRNON,     /* 非全球单播地址选择源地址 */
    IP6ADDR_CIOCTL_SELECTSRCADDRONIF,    /* 在指定接口上选择源地址 */
    IP6ADDR_CIOCTL_BESTMATCHONIF,        /* 接口上选择最优的地址 */
    IP6ADDR_CIOCTL_GETADDREPOCH,         /* 获取地址时戳 */
    IP6ADDR_CIOCTL_INCADDREPOCH,         /* 更新地址时戳 */
    IP6ADDR_CIOCTL_GETIFID,              /* 获取接口ID */
    IP6ADDR_CIOCTL_GETLINKSTATE,         /* 获取链路层状态 */
    IP6ADDR_CIOCTL_GETKEVENT,            /* 获取内核变化事件 */
    IP6ADDR_CIOCTL_CLEARKEVENT,          /* 清除本板接口内核事件平滑 */
    IP6ADDR_CIOCTL_PREPAREIFKEVENT,      /* 准备本板接口内核事件平滑 */
    IP6ADDR_CIOCTL_GETIFCTLBLKINFO,      /* 获取接口控制块上的信息 */
    /* 前缀消息类型 */
    IP6ADDR_CIOCTL_ADDPREFIX,            /* 添加前缀 */  
    IP6ADDR_CIOCTL_DELPREFIX,            /* 删除前缀 */ 
    IP6ADDR_CIOCTL_LOOKUPPREFIX,         /* 精确查找前缀 */ 
    IP6ADDR_CIOCTL_GETNEXTBATCHPREFIX,   /* 获取下一批前缀 */
    IP6ADDR_CIOCTL_GETPREFIXEPOCH,       /* 获取前缀时间戳 */ 
    IP6ADDR_CIOCTL_INCPREFIXEPOCH,       /* 更新前缀时间戳 */
    IP6ADDR_CIOCTL_GETPUBADDRBYPRE,      /* 根据指定前缀获取公有地址 */
    /* 网关消息类型 */
    IP6ADDR_CIOCTL_ADDGATEWAY,           /* 添加网关 */   
    IP6ADDR_CIOCTL_DELGATEWAY,           /* 删除网关 */ 
    IP6ADDR_CIOCTL_DELIFGATEWAY,         /* 删除接口上的网关信息 */
    IP6ADDR_CIOCTL_GETNEXTBATCHGATEWAY,  /* 获取下一批网关 */
    IP6ADDR_CIOCTL_GETGATEWAYEPOCH,      /* 获取网关时间戳 */ 
    IP6ADDR_CIOCTL_INCGATEWAYEPOCH,      /* 更新网关时间戳 */ 
    IP6ADDR_CIOCTL_GETGWMINTIME,         /* 获取网关最短老化时间 */
    IP6ADDR_CIOCTL_SETGWREACHABILITY,    /* 刷新网关可达性 */
    IP6ADDR_CIOCTL_SELECTGATEWAY,        /* 优选网关 */
    IP6ADDR_CIOCTL_GETNEXTBATCHPBGW,     /* 获取下一批已发布的网关 */
    IP6ADDR_CIOCTL_REFRESHPUBGATEWAY,    /* 更新私网内已经发布的网关信息 */
    /* AUTO CONFIG消息类型 */
    IP6ADDR_CIOCTL_SETAUTOENABLE,        /* 使能Auto配置/Auto linklocal配置 */
    IP6ADDR_CIOCTL_GETAUTOINFO,          /* 获取无状态自动配置 */
    IP6ADDR_CIOCTL_GETBATCHAUTO,         /* 批量获取无状态自动配置 */
    IP6ADDR_CIOCTL_RSSTART,              /* RS报文发送 */
    IP6ADDR_CIOCTL_GETAUTOMINTIME,       /* 获取auto最短老化事件 */
    IP6ADDR_CIOCTL_GETAUTOEPOCH,         /* 获取无状态使能时间戳 */
    IP6ADDR_CIOCTL_INCAUTOEPOCH,         /* 更新无状态使能时间戳 */
    IP6ADDR_CIOCTL_GETSTAT,              /* 获取统计信息 */
    IP6ADDR_CIOCTL_GETNEXTADDRINVRF,     /* 获取vrf中的下一个地址信息 */
    IP6ADDR_CIOCTL_GETNEXTGWINVRF,       /* 获取下一个网关ON vrf */
    IP6ADDR_CIOCTL_GETGWBYKEY,           /* 根据KEY获取网关 */
    IP6ADDR_CIOCTL_SETPREFERTEMPADDR,    /* 设置优选临时地址消息 */
    IP6ADDR_CIOCTL_GETPREFERTEMPADDR,    /* 获取优选临时地址标记 */
    IP6ADDR_CIOCTL_GETPREFIX,            /* 根据key查找接口上的前缀 */    
    IP6ADDR_CIOCTL_GETBESTMATCHPRELEN,   /* 查找和地址最短匹配的前缀长度 */
    IP6ADDR_CIOCTL_BUTT
}IP6ADDR_CIOCTL_MSGTYPE_E;

/* DUMP 消息类型 */
typedef enum tagIP6ADDR_DumpMsgType
{
    IP6ADDR_DUMP_GETSTATISTIC=0,   /* 获取统计信息 */
    IP6ADDR_DUMP_BATCHGETUC,       /* 批量获取单播地址 */
    IP6ADDR_DUMP_BATCHGETMCONIF,   /* 批量获取指定接口上组播地址 */
    IP6ADDR_DUMP_BATCHGETPREONIF,  /* 批量获取接口上前缀 */
    IP6ADDR_DUMP_BATCHGETGWONIF,   /* 批量获取接口上网关 */
    IP6ADDR_DUMP_BUTT
}IP6ADDR_DUMP_MSGTYPE_E;

/* 统计信息类型 */
typedef enum tagIp6AddrStatType
{
    IP6ADDR_STAT_ADDR_ADD,
    IP6ADDR_STAT_ADDR_DEL,
    IP6ADDR_STAT_PREFIX_ADD,
    IP6ADDR_STAT_PREFIX_DEL,
    IP6ADDR_STAT_GATEWAY_ADD,
    IP6ADDR_STAT_GATEWAY_DEL,
    IP6ADDR_STAT_CIOCTL_NUM, 
    IP6ADDR_STAT_DUMP_NUM, 
    IP6ADDR_STAT_IF_ADD,
    IP6ADDR_STAT_IF_DEL,
    IP6ADDR_STAT_RA_PREFIX_NUM,
    IP6ADDR_STAT_RA_GATEWAY_NUM,
    IP6ADDR_STAT_BUTT
}IP6ADDR_STAT_TYPE_E;

/* DAD检测结果上送结构 */
ISSU typedef struct tagIP6ADDR_DADMsg
{
    IN6ADDR_S   stIP6Addr;   /* 地址 */
    IF_INDEX    ifIndex;     /* 接口索引 */
    UINT        uiDADState;  /* DAD检测结果 */
}IP6ADDR_DADMSG_S;

/* 接口ID变化消息结构 */
ISSU typedef struct tagIP6ADDR_IIDMsg
{
    IF_INDEX            ifIndex;         /* 变化的接口索引 */
    IP6_INTERFACEID_S   stInterfaceID;   /* 变化后接口ID */
}IP6ADDR_IIDMSG_S;

/* 接口Nego状态变化消息结构 */
ISSU typedef struct tagIP6ADDR_NegotiateMsg
{
    IF_INDEX            ifIndex;         /* 变化的接口索引 */
    UINT                uiNegoStatus;    /* Nego状态 */
    BOOL_T              bDownBefore;     /* 曾经down过 */
    USHORT              usReserve;
}IP6ADDR_NEGOTIATEMSG_S;

/* 消息封装相关宏及数据结构定义 */
ISSU typedef struct tagIP6ADDR_MsgHead
{
    UINT uiMsgType;    /* 消息类型 */
    UINT uiInfoLen;    /* 负载长度 */
}IP6ADDR_MSGHEAD_S;

/* 获取指向V的指针 */
#define IP6ADDR_TLV_GETVP(buf) ((VOID *)((CHAR *)(buf) + sizeof(IP6ADDR_MSGHEAD_S)))
/* 解封装时获取type */
#define IP6ADDR_TLV_GETT(buf) (ntohl(((IP6ADDR_MSGHEAD_S *)(buf))->uiMsgType))
/* 解封装时获取len */
#define IP6ADDR_TLV_GETL(buf) (ntohl(((IP6ADDR_MSGHEAD_S *)(buf))->uiInfoLen))
/* 获取消息长度 */
#define IP6ADDR_TLV_GETHEADLEN (sizeof(IP6ADDR_MSGHEAD_S))

/* 封装T & L */
#define IP6ADDR_TLV_ENCAPTL(type, len, buf) \
{ \
    ((IP6ADDR_MSGHEAD_S *)(buf))->uiMsgType = htonl(type); \
    ((IP6ADDR_MSGHEAD_S *)(buf))->uiInfoLen = htonl(len); \
}

/* 一级消息类型定义 */
ISSU typedef enum tagIP6Addr_MsgType
{
    IP6ADDR_MSGTYPE_ADDR,          /* 地址消息 */
    IP6ADDR_MSGTYPE_PREFIX,        /* 前缀消息 */
    IP6ADDR_MSGTYPE_GATEWAY,       /* 网关消息 */
    IP6ADDR_MSGTYPE_SMOOTH,        /* 平滑消息 */  
    IP6ADDR_MSGTYPE_IP6CFG,        /* 地址配置消息 */
    IP6ADDR_MSGTYPE_KEVENTNOTIFY,  /* 内核事件通知 */ 
    IP6ADDR_MSGTYPE_NET,           /* 网络层消息 */
    IP6ADDR_MSGTYPE_DAD,           /* 网络层消息 */
    IP6ADDR_MSGTYPE_AUTO,          /* 无状态自动配置消息 */
    IP6ADDR_MSGTYPE_RRSEQ,         /* RR序列号同步消息 */
    IP6ADDR_MSGTYPE_CFGCHANGE,     /* 注册响应配置变化消息 */
    IP6ADDR_MSGTYPE_NOTIFY,        /* 通知地址管理再发一个有配置消息 */
    IP6ADDR_MSGTYPE_BUTT
} IP6ADDR_MSGTYPE_E;

/* 内核网络层二级消息类型 */
ISSU typedef enum tagIP6ADDR_NETTYPE
{
    IP6ADDR_NET_NEGOTIATESTATUS = 0,     /* 允许网络层UP/DOWN消息 */
    IP6ADDR_NET_INTERFACEID,             /* 接口ID变化 */
    IP6ADDR_NET_BUTT
}IP6ADDR_NETTYPE_E;

/* 内核AUTO二级消息类型 */
ISSU typedef enum tagIP6ADDR_AUTOTYPE
{
    IP6ADDR_AUTO_PREFIX = 0,       /* 刷新前缀信息消息 */
    IP6ADDR_AUTO_GATEWAY,          /* 刷新网关信息消息 */
    IP6ADDR_AUTO_BUTT
}IP6ADDR_AUTOTYPE_E;


ISSU typedef enum tagIP6ADDR_ENABLETYPE
{
    IP6ADDR_ENABLE_AUTO = 0,       /* 接口使能无状态自动配置功能 */
    IP6ADDR_ENABLE_AUTOLL,         /* 接口使能自动生成linklocal地址功能 */
    IP6ADDR_ENABLE_BUTT
}IP6ADDR_ENABLETYPE_E;

#define IP6ADDR_ENABLE_AUTO_BIT           (1<<IP6ADDR_ENABLE_AUTO)     /* 使能无状态自动配置 */
#define IP6ADDR_ENABLE_AUTOLL_BIT         (1<<IP6ADDR_ENABLE_AUTOLL)   /* 使能auto link-local配置 */

/* 自动配置信息结构体 */
ISSU typedef struct tagIP6ADDR_AutoCfg
{
    UINT uiEpoch[IP6ADDR_ENABLE_BUTT];
    UINT uiRAPrefixCount;
    UINT uiAutoAddrCount;
    UINT uiEnableFlag;
}IP6ADDR_AUTOCFG_S;

/* 接口控制块属性结构 */
ISSU typedef struct tagIP6ADDR_IFCtlBlkInfo
{
    UINT uiFlag;
    UINT uiIFEvFlag;
    IP6ADDR_AUTOCFG_S stAutoCfg;
}IP6ADDR_IFCTLBLKINFO_S;

/* 使能同步消息 */
ISSU typedef struct tagIP6ADDR_EnableInfo
{
    IF_INDEX ifIndex;
    UINT uiEnableFlag;
}IP6ADDR_ENABLEINFO_S;

/*****************************************************************************
    Func Name: IP6ADDR_LifeTime_HTON
 Date Created: 2010/12/22 
       Author: l06132
  Description: IP6ADDR_LIFETIME_S主机序转网络序
        Input: IP6ADDR_LIFETIME_S *pstLifeTime, 待转换对象
       Output: IP6ADDR_LIFETIME_S *pstLifeTime, 转换后对象
       Return: 无
      Caution:
Enter context:
 Exit context:
  Locking seq:
------------------------------------------------------------------------------
  Modification History                                                      
  DATE        NAME             DESCRIPTION                                  
  --------------------------------------------------------------------------
  2010/12/22                                                                          
*****************************************************************************/
static inline VOID IP6ADDR_LifeTime_HTON(INOUT IP6ADDR_LIFETIME_S *pstLifeTime)
{
    pstLifeTime->uiValidTime = htonl(pstLifeTime->uiValidTime);
    pstLifeTime->uiPreferrdTime = htonl(pstLifeTime->uiPreferrdTime);

    return;
}
#define IP6ADDR_LifeTime_NTOH IP6ADDR_LifeTime_HTON

/*****************************************************************************
    Func Name: IP6ADDR_Key_HTON
 Date Created: 2010/12/22 
       Author: l06132
  Description: IP6ADDR_KEY_S主机序转网络序
        Input: IP6ADDR_KEY_S *pstAddrKey, 待转换对象
       Output: IP6ADDR_KEY_S *pstAddrKey, 转换后对象
       Return: 无
      Caution:
Enter context:
 Exit context:
  Locking seq:
------------------------------------------------------------------------------
  Modification History                                                      
  DATE        NAME             DESCRIPTION                                  
  --------------------------------------------------------------------------
  2010/12/22                                                                          
*****************************************************************************/
static inline VOID IP6ADDR_Key_HTON(INOUT IP6ADDR_KEY_S *pstAddrKey)
{
    pstAddrKey->vrfIndex = htons(pstAddrKey->vrfIndex);
    pstAddrKey->ifIndex = htonl(pstAddrKey->ifIndex);
    pstAddrKey->uiPrefixLen = htonl(pstAddrKey->uiPrefixLen);
    pstAddrKey->uiOriginType = htonl(pstAddrKey->uiOriginType);
    
    return;
}
#define IP6ADDR_Key_NTOH IP6ADDR_Key_HTON

/*****************************************************************************
    Func Name: IP6ADDR_AddrInfo_HTON
 Date Created: 2010/12/22 
       Author: l06132
  Description: IP6ADDR_INFO_S主机序转网络序
        Input: IP6ADDR_INFO_S *pstAddrInfo, 待转换对象
       Output: IP6ADDR_INFO_S *pstAddrInfo, 转换后对象
       Return: 无
      Caution:
Enter context:
 Exit context:
  Locking seq:
------------------------------------------------------------------------------
  Modification History                                                      
  DATE        NAME             DESCRIPTION                                  
  --------------------------------------------------------------------------
  2010/12/22                                                                          
*****************************************************************************/
static inline VOID IP6ADDR_AddrInfo_HTON(INOUT IP6ADDR_INFO_S *pstAddrInfo)
{
    pstAddrInfo->uiProperty = htonl(pstAddrInfo->uiProperty);
    pstAddrInfo->uiStatus = htonl(pstAddrInfo->uiStatus);

    IP6ADDR_Key_HTON(&(pstAddrInfo->stAddrKey));
    
    return;
}
#define IP6ADDR_AddrInfo_NTOH IP6ADDR_AddrInfo_HTON

/*****************************************************************************
    Func Name: IP6ADDR_PrefixKey_HTON
 Date Created: 2011/04/06 
       Author: wkf2101
  Description: IP6ADDR_PREFIX_KEY_S主机序转网络序
        Input: IP6ADDR_PREFIX_KEY_S *pstPrefixKey, 待转换对象
       Output: IP6ADDR_PREFIX_KEY_S *pstPrefixKey, 转换后对象
       Return: 无
      Caution:
Enter context:
 Exit context:
  Locking seq:
------------------------------------------------------------------------------
  Modification History                                                      
  DATE        NAME             DESCRIPTION                                  
  --------------------------------------------------------------------------
  2011/04/06                                                                          
*****************************************************************************/
static inline VOID IP6ADDR_PrefixKey_HTON(INOUT IP6ADDR_PREFIX_KEY_S *pstPrefixKey)
{
    pstPrefixKey->ifIndex = htonl(pstPrefixKey->ifIndex);
    pstPrefixKey->vrfIndex = htons(pstPrefixKey->vrfIndex);
    pstPrefixKey->uiPrefixLength = htonl(pstPrefixKey->uiPrefixLength);
    pstPrefixKey->uiPrefixOrigin = htonl(pstPrefixKey->uiPrefixOrigin);    
    return;
}
#define IP6ADDR_PrefixKey_NTOH IP6ADDR_PrefixKey_HTON

/*****************************************************************************
    Func Name: IP6ADDR_Prefix_HTON
 Date Created: 2010/12/22 
       Author: l06132
  Description: IP6ADDR_PREFIX_S主机序转网络序
        Input: IP6ADDR_PREFIX_S *pstPrefixInfo, 待转换对象
       Output: IP6ADDR_PREFIX_S *pstPrefixInfo, 转换后对象
       Return: 无
      Caution:
Enter context:
 Exit context:
  Locking seq:
------------------------------------------------------------------------------
  Modification History                                                      
  DATE        NAME             DESCRIPTION                                  
  --------------------------------------------------------------------------
  2010/12/22                                                                          
*****************************************************************************/
static inline VOID IP6ADDR_Prefix_HTON(INOUT IP6ADDR_PREFIX_S *pstPrefixInfo)
{
    IP6ADDR_PrefixKey_HTON(&(pstPrefixInfo->stPrefixKey));
    IP6ADDR_LifeTime_HTON(&(pstPrefixInfo->stLifeTime));
    
    return;
}
#define IP6ADDR_Prefix_NTOH IP6ADDR_Prefix_HTON

/*****************************************************************************
    Func Name: IP6ADDR_PrefixFull_HTON
 Date Created: 2010/12/22 
       Author: l06132
  Description: IP6ADDR_PREFIX_FULLINFO_S主机序转网络序
        Input: IP6ADDR_PREFIX_FULLINFO_S *pstPrefixFullInfo, 待转换对象
       Output: IP6ADDR_PREFIX_FULLINFO_S *pstPrefixFullInfo, 转换后对象
       Return: 无
      Caution:
Enter context:
 Exit context:
  Locking seq:
------------------------------------------------------------------------------
  Modification History                                                      
  DATE        NAME             DESCRIPTION                                  
  --------------------------------------------------------------------------
  2010/12/22                                                                          
*****************************************************************************/
static inline VOID IP6ADDR_PrefixFull_HTON(INOUT IP6ADDR_PREFIX_FULLINFO_S *pstPrefixFullInfo)
{
    pstPrefixFullInfo->uiEpoch = htonl(pstPrefixFullInfo->uiEpoch);
    pstPrefixFullInfo->uiCreateTime = htonl(pstPrefixFullInfo->uiCreateTime);
    
    IP6ADDR_Prefix_HTON(&(pstPrefixFullInfo->stPrefix));
       
    return;
}
#define IP6ADDR_PrefixFull_NTOH IP6ADDR_PrefixFull_HTON

/*****************************************************************************
    Func Name: IP6ADDR_GatewayKey_HTON
 Date Created: 2011/04/06 
       Author: wkf2101
  Description: IP6ADDR_GATEWAY_KEY_S主机序转网络序
        Input: IP6ADDR_GATEWAY_KEY_S *pstGatewayKey, 待转换对象
       Output: IP6ADDR_GATEWAY_KEY_S *pstGatewayKey, 转换后对象
       Return: 无
      Caution:
Enter context:
 Exit context:
  Locking seq:
------------------------------------------------------------------------------
  Modification History                                                      
  DATE        NAME             DESCRIPTION                                  
  --------------------------------------------------------------------------
  2011/04/06                                                                          
*****************************************************************************/
static inline VOID IP6ADDR_GatewayKey_HTON(INOUT IP6ADDR_GATEWAY_KEY_S *pstGatewayKey)
{
    pstGatewayKey->ifIndex = htonl(pstGatewayKey->ifIndex);
    pstGatewayKey->vrfIndex = htons(pstGatewayKey->vrfIndex);
    return;
}
#define IP6ADDR_GatewayKey_NTOH IP6ADDR_GatewayKey_HTON

/*****************************************************************************
    Func Name: IP6ADDR_Gateway_HTON
 Date Created: 2011/04/06 
       Author: wkf2101
  Description: IP6ADDR_GATEWAY_S主机序转网络序
        Input: IP6ADDR_GATEWAY_S *pstGateway, 待转换对象
       Output: IP6ADDR_GATEWAY_S *pstGateway, 转换后对象
       Return: 无
      Caution:
Enter context:
 Exit context:
  Locking seq:
------------------------------------------------------------------------------
  Modification History                                                      
  DATE        NAME             DESCRIPTION                                  
  --------------------------------------------------------------------------
  2011/04/06                                                                          
*****************************************************************************/
static inline VOID IP6ADDR_Gateway_HTON(INOUT IP6ADDR_GATEWAY_S *pstGateway)
{
    pstGateway->uiCreateTime = htonl(pstGateway->uiCreateTime);
    pstGateway->uiRouterLifetime = htonl(pstGateway->uiRouterLifetime);
    pstGateway->uiEpoch = htonl(pstGateway->uiEpoch);
    
    IP6ADDR_GatewayKey_HTON(&pstGateway->stGatewayKey);
    return;
}
#define IP6ADDR_Gateway_NTOH IP6ADDR_Gateway_HTON

/*****************************************************************************
    Func Name: IP6ADDR_Multicast_HTON
 Date Created: 2010/12/22 
       Author: l06132
  Description: IP6ADDR_MULTICAST_S主机序转网络序
        Input: IP6ADDR_MULTICAST_S *pstMCInfo, 待转换对象
       Output: IP6ADDR_MULTICAST_S *pstMCInfo, 转换后对象
       Return: 无
      Caution:
Enter context:
 Exit context:
  Locking seq:
------------------------------------------------------------------------------
  Modification History                                                      
  DATE        NAME             DESCRIPTION                                  
  --------------------------------------------------------------------------
  2010/12/22                                                                          
*****************************************************************************/
static inline VOID IP6ADDR_Multicast_HTON(INOUT IP6ADDR_MULTICAST_S *pstMCInfo)
{
    pstMCInfo->uiRefCount = htonl(pstMCInfo->uiRefCount);
    
    return;
}
#define IP6ADDR_Multicast_NTOH IP6ADDR_Multicast_HTON

/*****************************************************************************
    Func Name: IP6ADDR_FullInfo_HTON
 Date Created: 2010/12/22 
       Author: l06132
  Description: IP6ADDR_FULLINFO_S主机序转网络序
        Input: IP6ADDR_FULLINFO_S *pstAddrFullInfo, 待转换对象
       Output: IP6ADDR_FULLINFO_S *pstAddrFullInfo, 转换后对象
       Return: 无
      Caution:
Enter context:
 Exit context:
  Locking seq:
------------------------------------------------------------------------------
  Modification History                                                      
  DATE        NAME             DESCRIPTION                                  
  --------------------------------------------------------------------------
  2010/12/22                                                                          
*****************************************************************************/
static inline VOID IP6ADDR_FullInfo_HTON(INOUT IP6ADDR_FULLINFO_S *pstAddrFullInfo)
{
    pstAddrFullInfo->uiOriginAttr = htonl(pstAddrFullInfo->uiOriginAttr);
    pstAddrFullInfo->uiFlag = htonl(pstAddrFullInfo->uiFlag);
    pstAddrFullInfo->uiEpoch = htonl(pstAddrFullInfo->uiEpoch);
    pstAddrFullInfo->uiCreateTime = htonl(pstAddrFullInfo->uiCreateTime);
    pstAddrFullInfo->uiLastChangedTime = htonl(pstAddrFullInfo->uiLastChangedTime);
    pstAddrFullInfo->uiOriginalCreateTime = htonl(pstAddrFullInfo->uiOriginalCreateTime);
    
    IP6ADDR_AddrInfo_HTON(&(pstAddrFullInfo->stIP6Addr));
    IP6ADDR_LifeTime_HTON(&(pstAddrFullInfo->stLifeTime));

    return;
}
#define IP6ADDR_FullInfo_NTOH IP6ADDR_FullInfo_HTON

/*****************************************************************************
    Func Name: IP6ADDR_SLT_HTON
 Date Created: 2010/12/22 
       Author: l06132
  Description: IP6ADDR_SLT_S主机序转网络序
        Input: IP6ADDR_SLT_S *pstSLT, 待转换对象
       Output: IP6ADDR_SLT_S *pstSLT, 转换后对象
       Return: 无
      Caution:
Enter context:
 Exit context:
  Locking seq:
------------------------------------------------------------------------------
  Modification History                                                      
  DATE        NAME             DESCRIPTION                                  
  --------------------------------------------------------------------------
  2010/12/22                                                                          
*****************************************************************************/
static inline VOID IP6ADDR_SLT_HTON(INOUT IP6ADDR_SLT_S *pstSLT)
{
    pstSLT->uiScope = htonl(pstSLT->uiScope);
    pstSLT->uiFlag = htonl(pstSLT->uiFlag);
    pstSLT->vrfIndex = htons(pstSLT->vrfIndex);
    pstSLT->ifIndex = htonl(pstSLT->ifIndex);
    pstSLT->ifExcludeIndex = htonl(pstSLT->ifExcludeIndex);
    pstSLT->uiPrefixLen = htonl(pstSLT->uiPrefixLen);
    pstSLT->uiProperty = htonl((pstSLT)->uiProperty);
    pstSLT->uiExcludeProperty = htonl(pstSLT->uiExcludeProperty);
    pstSLT->uiStatus = htonl(pstSLT->uiStatus);
    pstSLT->uiOriginAttr = htonl(pstSLT->uiOriginAttr);
    pstSLT->uiExcludeOriginAttr = htonl(pstSLT->uiExcludeOriginAttr);
    pstSLT->uiOriginType = htonl(pstSLT->uiOriginType);

    return;
}
#define IP6ADDR_SLT_NTOH IP6ADDR_SLT_HTON

/*****************************************************************************
    Func Name: IP6ADDR_DadMsg_HTON
 Date Created: 2010/12/22 
       Author: l06132
  Description: IP6ADDR_DADMSG_S主机序转网络序
        Input: IP6ADDR_DADMSG_S *pstDadMsg, 待转换对象
       Output: IP6ADDR_DADMSG_S *pstDadMsg, 转换后对象
       Return: 无
      Caution:
Enter context:
 Exit context:
  Locking seq:
------------------------------------------------------------------------------
  Modification History                                                      
  DATE        NAME             DESCRIPTION                                  
  --------------------------------------------------------------------------
  2010/12/22                                                                          
*****************************************************************************/
static inline VOID IP6ADDR_DadMsg_HTON(INOUT IP6ADDR_DADMSG_S *pstDadMsg)
{
    pstDadMsg->ifIndex = htonl(pstDadMsg->ifIndex);
    pstDadMsg->uiDADState = htonl(pstDadMsg->uiDADState);

    return;
}
#define IP6ADDR_DadMsg_NTOH IP6ADDR_DadMsg_HTON

/*****************************************************************************
    Func Name: IP6ADDR_IIDMsg_HTON
 Date Created: 2010/12/22 
       Author: l06132
  Description: IP6ADDR_IIDMSG_S主机序转网络序
        Input: IP6ADDR_IIDMSG_S *pstIIDMsg, 待转换对象
       Output: IP6ADDR_IIDMSG_S *pstIIDMsg, 转换后对象
       Return: 无
      Caution:
Enter context:
 Exit context:
  Locking seq:
------------------------------------------------------------------------------
  Modification History                                                      
  DATE        NAME             DESCRIPTION                                  
  --------------------------------------------------------------------------
  2010/12/22                                                                          
*****************************************************************************/
static inline VOID IP6ADDR_IIDMsg_HTON(INOUT IP6ADDR_IIDMSG_S *pstIIDMsg)
{
    pstIIDMsg->ifIndex = htonl(pstIIDMsg->ifIndex);

    return;
}
#define IP6ADDR_IIDMsg_NTOH IP6ADDR_IIDMsg_HTON

/*****************************************************************************
    Func Name: IP6ADDR_NegotiateMsg_HTON
 Date Created: 2010/12/22 
       Author: l06132
  Description: IP6ADDR_NEGOTIATEMSG_S主机序转网络序
        Input: IP6ADDR_NEGOTIATEMSG_S *pstNegotiateMsg, 待转换对象
       Output: IP6ADDR_NEGOTIATEMSG_S *pstNegotiateMsg, 转换后对象
       Return: 无
      Caution:
Enter context:
 Exit context:
  Locking seq:
------------------------------------------------------------------------------
  Modification History                                                      
  DATE        NAME             DESCRIPTION                                  
  --------------------------------------------------------------------------
  2010/12/22                                                                          
*****************************************************************************/
static inline VOID IP6ADDR_NegotiateMsg_HTON(INOUT IP6ADDR_NEGOTIATEMSG_S *pstNegotiateMsg)
{
    pstNegotiateMsg->ifIndex = htonl(pstNegotiateMsg->ifIndex);
    pstNegotiateMsg->uiNegoStatus = htonl(pstNegotiateMsg->uiNegoStatus);
    pstNegotiateMsg->bDownBefore = (BOOL_T)htons((USHORT)pstNegotiateMsg->bDownBefore);

    return;
}
#define IP6ADDR_NegotiateMsg_NTOH IP6ADDR_NegotiateMsg_HTON

/*****************************************************************************
    Func Name: IP6ADDR_CmpUINT32
 Date Created: 2010/11/20 
       Author: l06132
  Description: 比较两个UINT无符号数的大小
        Input: uiLeft  比较左值
               uiRight 比较右值   
       Output: 
       Return: 1   左值>右值
               0   左值=右值
               -1  左值<右值
      Caution: 不能比较大于32bit的无符号数
Enter context:
 Exit context:
  Locking seq:
------------------------------------------------------------------------------
  Modification History                                                      
  DATE        NAME             DESCRIPTION                                  
  --------------------------------------------------------------------------
  2010/11/20                                                                          
*****************************************************************************/
static inline INT IP6ADDR_CmpUINT32(IN UINT uiLeft, IN UINT uiRight)
{
    INT iRet;
    
    if (uiLeft > uiRight)
    {
        iRet = 1;
    }
    else if (uiLeft < uiRight)
    {
        iRet = -1;
    }
    else
    {
        iRet = 0;
    }
    
    return iRet;
}
/*****************************************************************************
    Func Name: IP6ADDR_AutoCfg_HTON
 Date Created: 2011/4/26 
       Author: ykf2060
  Description: 自动配置信息主机序转网络序
        Input: IP6ADDR_AUTOCFG_S *pstAutoCfg    指向自动配置信息的指针
       Output: IP6ADDR_AUTOCFG_S *pstAutoCfg    指向自动配置信息的指针
       Return: 
      Caution: 
Enter context: 
 Exit context: 
  Locking seq: 
------------------------------------------------------------------------------
  Modification History                                                      
  DATE        NAME             DESCRIPTION                                  
  --------------------------------------------------------------------------
  2011/4/26                                                                          
*****************************************************************************/
static inline VOID IP6ADDR_AutoCfg_HTON(INOUT IP6ADDR_AUTOCFG_S *pstAutoCfg)
{
    UINT i;
    for (i = 0; i < IP6ADDR_ENABLE_BUTT; i++)
    {
        pstAutoCfg->uiEpoch[i] = htonl(pstAutoCfg->uiEpoch[i]);
    }
    pstAutoCfg->uiEnableFlag = htonl(pstAutoCfg->uiEnableFlag);
    pstAutoCfg->uiAutoAddrCount = htonl(pstAutoCfg->uiAutoAddrCount);
    pstAutoCfg->uiRAPrefixCount = htonl(pstAutoCfg->uiRAPrefixCount);
    return;
}
#define IP6ADDR_AutoCfg_NTOH IP6ADDR_AutoCfg_HTON

/*****************************************************************************
    Func Name: IP6ADDR_IfCtlBlkInfo_HTON
 Date Created: 2010/12/22 
       Author: l06132
  Description: IP6ADDR_IFCTLBLKINFO_S主机序转网络序
        Input: IP6ADDR_IFCTLBLKINFO_S *pstIFCtlBlkInfo, 待转换对象
       Output: IP6ADDR_IFCTLBLKINFO_S *pstIFCtlBlkInfo, 转换后对象
       Return: 无
      Caution:
Enter context:
 Exit context:
  Locking seq:
------------------------------------------------------------------------------
  Modification History                                                      
  DATE        NAME             DESCRIPTION                                  
  --------------------------------------------------------------------------
  2010/12/22                                                                          
*****************************************************************************/
static inline VOID IP6ADDR_IfCtlBlkInfo_HTON(INOUT IP6ADDR_IFCTLBLKINFO_S *pstIFCtlBlkInfo)
{
    pstIFCtlBlkInfo->uiFlag = htonl(pstIFCtlBlkInfo->uiFlag);
    pstIFCtlBlkInfo->uiIFEvFlag = htonl(pstIFCtlBlkInfo->uiIFEvFlag);
    IP6ADDR_AutoCfg_HTON(&pstIFCtlBlkInfo->stAutoCfg);

    return;
}
#define IP6ADDR_IfCtlBlkInfo_NTOH IP6ADDR_IfCtlBlkInfo_HTON

/*****************************************************************************
    Func Name: IP6ADDR_EnableInfo_HTON
 Date Created: 2010/12/22 
       Author: l06132
  Description: IP6ADDR_ENABLEINFO_S主机序转网络序
        Input: IP6ADDR_ENABLEINFO_S *pstEnableInfo, 待转换对象
       Output: IP6ADDR_ENABLEINFO_S *pstEnableInfo, 转换后对象
       Return: 无
      Caution:
Enter context:
 Exit context:
  Locking seq:
------------------------------------------------------------------------------
  Modification History                                                      
  DATE        NAME             DESCRIPTION                                  
  --------------------------------------------------------------------------
  2010/12/22                                                                          
*****************************************************************************/
static inline VOID IP6ADDR_EnableInfo_HTON(INOUT IP6ADDR_ENABLEINFO_S *pstEnableInfo)
{
    pstEnableInfo->ifIndex= htonl(pstEnableInfo->ifIndex);
    pstEnableInfo->uiEnableFlag= htonl(pstEnableInfo->uiEnableFlag);

    return;
}
#define IP6ADDR_EnableInfo_NTOH IP6ADDR_EnableInfo_HTON

#ifdef __cplusplus
}
#endif

#endif /* _SYS_IPADDR_H_ */

