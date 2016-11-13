/*****************************************************************************
 Copyright (c) 2008, Hangzhou H3C Technologies Co., Ltd. All rights reserved.
------------------------------------------------------------------------------
                              ip.h
  Project Code: Comware V7
   Module Name: IPv4/IPv6
  Date Created: 2008-07-02
        Author: stid 04370
   Description: Common driver macros/data types/interfaces between platform and driver
                for IPv4/IPv6
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------
  YYYY-MM-DD

*****************************************************************************/

#ifndef _DRV_IP_H_
#define _DRV_IP_H_

#ifdef __cplusplus
extern "C"{
#endif


/****************************************************
 *             Struct definition                    *
 ****************************************************/
/**********************BPA标识位***********************/
#define BPA_MARK_TYPE_NONE        0x0       /* 流量统计缺省信息 */
#define BPA_MARK_TYPE_SRC         0x01      /* 设置基于源的流量统计 */
#define BPA_MARK_TYPE_DST         0x02      /* 设置基于目的的流量统计 */

/* BPA统计信息结构 */
typedef struct tagIP_BPAStat
{
    UINT64 uiInPktCnt;            /* 入方向的报文数 */
    UINT64 uiInPktByteCnt;        /* 入方向的字节数 */
    UINT64 uiOutPktCnt;           /* 出方向的报文数 */
    UINT64 uiOutPktByteCnt;       /* 出方向的字节数 */
    UINT64 uiFlowIndex;           /* 流量索引值 */
} IP_BPA_STAT_S;

/* FIB4 硬件表项规格定制 */
typedef struct tagFIB4_SpecConf
{
    UINT uiMaxECMP;     /* FIB4 最大等价路由数 */
    UINT uiMaxPublic;   /* FIB4 公网表项规格 */
    UINT uiMaxVrf;      /* FIB4 私网表项规格 */
    UINT uiECMPMode;    /* FIB4 等价路由模式，用于下驱动*/
} FIB4_SPECCONF_S;

/* FIB6 硬件表项规格定制 */
typedef struct tagFIB6_SpecConf
{
    UINT uiMaxECMP6;    /* FIB6 最大等价路由数 */
    UINT uiMaxPublic6;  /* FIB6 公网表项规格 */
    UINT uiMaxVrf6;     /* FIB6 私网表项规格 */
} FIB6_SPECCONF_S;


/**********************FIB前缀和VN下发时回填的DrvContext数量***********************/
#define FIB_DRVCONTEXT_LEN_MAX     8
#define VN_DRVCONTEXT_LEN          16
#define VN_NEXTHOP_DRVCONTEXT_LEN  4

/***************************IP负载分担方式定义*********************************/
typedef enum tagIP_LoadSharingModeEnum
{
    IP_LOADSHARING_MODE_DEFAULT = 0,        /* 缺省的负载分担，各产品可能缺省不一样*/
    IP_LOADSHARING_MODE_PACKET,             /* 逐包负载分担 */
    IP_LOADSHARING_MODE_FLOW,               /* 逐流负载分担 */
    IP_LOADSHARING_MODE_FLOW_ALGORITHM,     /* 逐流负载分担算法 */
    IP_LOADSHARING_MODE_FLOW_TUNNEL,        /* tunnel hash模式 */
    IP_LOADSHARING_MODE_BUTT
}IP_LOADSHARING_MODE_E;

/*逐流负载分担方式位图定义*/
#define IP_LOADSHARING_OPTION_DEFAULT       0x00
#define IP_LOADSHARING_OPTION_SRCIP         0x01
#define IP_LOADSHARING_OPTION_DSTIP         0x02
#define IP_LOADSHARING_OPTION_PROTOCOL      0x04
#define IP_LOADSHARING_OPTION_SRCPORT       0x08
#define IP_LOADSHARING_OPTION_DSTPORT       0x10
#define IP_LOADSHARING_OPTION_INGRESSPORT   0x20

#define IP_LOADSHARING_LOCALFIRST_SLOT      1  /* 支持负载分担本板优先 */
#define IP_LOADSHARING_LOCALFIRST_CHASSIS   2  /*支持负载分担本框优先*/

/**********************QPPB标识位***********************/
#define QPPB_MARK_TYPE_NONE        0x0          /* QPPB缺省信息 */
#define QPPB_MARK_TYPE_IPPRE       0x01         /* 设置ip-precedence信息 */
#define QPPB_MARK_TYPE_LOCALID     0x02         /* 设置qos-local-id信息 */
/* tunnel hash方式定义 */
#define IP_LOADSHARING_OPTION_TUNNEL_DEFAULT 0x00
#define IP_LOADSHARING_OPTION_TUNNEL_OUT     0x01
#define IP_LOADSHARING_OPTION_TUNNEL_IN      0x02
#define IP_LOADSHARING_OPTION_TUNNEL_ALL     0x04
/*用于计算IPV4/6负载分担模式下等价路由选路信息-报文类型*/
#define IP_LOADSHARING_PATH_PKTTYPE_IPV4OE 1
#define IP_LOADSHARING_PATH_PKTTYPE_IPV6OE 2

/*用于计算IPV4/V6负载分担模式下等价路由选路信息-参数*/
typedef struct tagIP_LoadSharingPathPara
{
    IF_INDEX    ifIndexIngress;/*入接口索引*/
    UINT        uiPktType;     /*报文封装类型*/
    INET_ADDR_S stDstIp;       /*目的IP*/
    INET_ADDR_S stSrcIp;       /*源IP*/
    UINT        uiProID;       /*Protocol ID*/
    USHORT      usDstPort;     /*目的L4端口*/
    USHORT      usSrcPort;     /*源L4端口*/
    VRF_INDEX   vrfIndex;      /*L3VPN的VPN实例索引*/
}IP_LOADSHARING_PATH_PARA_S;

/*用于计算IPV4/V6负载分担模式下等价路由选路信息-结果*/
typedef struct tagIP_LoadSharingPathResult
{
    INET_ADDR_S stNextHop;     /*驱动返回选中路由下一跳*/
    IF_INDEX    ifIndexEgress; /*驱动返回选中路由出接口*/
}IP_LOADSHARING_PATH_RESULT_S;

#ifdef __KERNEL__

#ifndef _DRV_ADJ_H_
#include <drv/adj.h>
#endif

#ifndef _MPLS_NHLFE_H_
#include <drv/mpls_nhlfe.h>
#endif

/*******************驱动返回的标志******************************/
/* VN重刷后是否需要同步宏定义 */
#define L3_VN_RELOAD_NOSYN   0      /* 不需要同步 */
#define L3_VN_RELOAD_SYN     1      /* 需要同步 */

/* 反刷前缀后是否需要同步宏定义 */
#define L3_ROUTE_RELOAD_NOSYN  0    /* 不需要同步 */
#define L3_ROUTE_RELOAD_SYN    1    /* 需要同步 */


/**************************路由驱动接口属性***************************/
/*****************************路由属性定义***************************/
/* 出接口相关属性，低16位 */
#define L3_ROUTE_BLACKHOLE   0x00000001   /* 黑洞路由，丢弃报文，不做其他处理 */
#define L3_ROUTE_CPU         0x00000002   /* 将报文交给软件处理 */
#define L3_ROUTE_LOOPBACK    0x00000004   /* 上送本机协议栈处理 */
#define L3_ROUTE_TUNNEL      0x00000008   /* 出接口为TUNNEL接口 */
#define L3_ROUTE_NORMAL      0x00008000   /* 除以上属性以外的正常转发路由，仅驱动返回值用 */

/* 前缀相关属性，高16位*/
#define L3_ROUTE_DIRECT      0x00010000   /* 直连路由 */
#define L3_ROUTE_INDIRECT    0x00020000   /* 非直连路由 */
#define L3_ROUTE_ECMP        0x00040000   /* ECMP路由 */
#define L3_ROUTE_FTN         0x00080000   /* FTN，即前缀中有标签 */
#define L3_ROUTE_BGP         0x00100000   /* BGP路由 */
#define L3_ROUTE_TUNNEL_END  0x00200000   /* TUNNEL隧道终结 */
#define L3_ROUTE_NAT         0x00400000   /* NAT生成的路由 */
#define L3_ROUTE_ARP         0x00800000   /* ARP主机路由 */
#define L3_ROUTE_ND          L3_ROUTE_ARP /* ND主机路由 */
#define L3_ROUTE_BROADCAST   0x01000000   /* 广播路由 */
#define L3_ROUTE_FTNSTAT     0x02000000   /* FTN统计使能 */
#define L3_ROUTE_ALL         0x1FFFF000   /*所有FIB前缀属性，仅用于驱动返回值*/

/* 前缀MPLS相关属性，低4位 */
#define L3_ROUTE_MPLS_NORMAL 0x00000001   /* 普通Transit LSP表项，除CRLSP之外的BGP LSP、LDP LSP、静态LSP */
#define L3_ROUTE_MPLS_CRLSP  0x00000002   /* Transit CRLSP表项 */
#define L3_ROUTE_MPLS_VLL    0x00000004   /* VPWS PW表项 */
#define L3_ROUTE_MPLS_VPLS   0x00000008   /* VPLS PW表项 */
#define L3_ROUTE_MPLS_ALL    0x0000000F   /* 所有MPLS表项，仅用于驱动返回值*/

/************************************************************
  关联模块定义, 与路由属性配合指定需要重刷的路由
************************************************************/
#define L3_ROUTE_MIDROUTE    0x40000000  /*FIB或VRFFIB模块*/
#define L3_ROUTE_MIDMPLS     0x80000000  /*MPLS模块*/
#define L3_ROUTE_MIDTUNNEL   0x20000000  /*隧道模块*/

/*****************************下一跳属性******************************************/
#define L3_NEXTHOP_RELAY       0x0100
#define L3_NEXTHOP_FRR_MAIN    0x0200
#define L3_NEXTHOP_FRR_BACKUP  0x0400
#define L3_NEXTHOP_IP          0x0001
#define L3_NEXTHOP_MPLS        0x0002
#define L3_NEXTHOP_L2VPN       0x0004
#define L3_NEXTHOP_LSM         0x0008
#define L3_NEXTHOP_ROUTE       0x0010
#define L3_NEXTHOP_LISP        0x0020
#define L3_NEXTHOP_P2MP_LSM    0x0080
#define L3_NEXTHOP_DUMMY       0x8000

/*****************************下一跳的状态******************************************/
#define L3_NEXTHOP_STATUS_ACTIVE      0x01

#define IP_INVALID_PRECE     0xFFFF                /* 无效的ip优先级 */
#define IP_INVALID_LOCALID   0xFFFF                /* 无效的QOS local id */
#define IP_INVALID_FLOWINDEX ((UINT64)-1)          /* 无效的BGP流量索引 */
/******************************接口协议mtu下驱动控制字******************************/
#define IOCTL_COMMON_SET_IP_MTU                  IF_IOCTL_CMD_DEF(MID_IPFW, IF_COMMON_TYPE, 0)
#define IOCTL_COMMON_SET_IP6_MTU                 IF_IOCTL_CMD_DEF(MID_IP6FW, IF_COMMON_TYPE, 0)

/******************************QPPB下驱动控制字******************************/
#define IOCTL_COMMON_SET_IP_QPPB                 IF_IOCTL_CMD_DEF(MID_IPFW, IF_COMMON_TYPE, 1)

typedef struct tagQPPBPolicy
{
  USHORT usMarkSIPType;
  USHORT usMarkDIPType;
} QPPB_POLICY_S;

#define IP_PROTOENDRVCONTEXTLEN 2

/******************************BPA下驱动控制字******************************/
#define IOCTL_COMMON_SET_IP_BPA              IF_IOCTL_CMD_DEF(MID_IPFW, IF_COMMON_TYPE, 2)

/* 配置下驱动结构 */
typedef struct tagIPBPA
{
  USHORT usMarkInputType;           /* 入方向统计 */
  USHORT usMarkOutputType;          /* 出方向统计 */
} IP_BPA_S;

/********************************VN下发驱动操作类型*********************************/
typedef enum tagL3VnOpType
{
    L3_VN_ADD,        /* 增加一条表项 */
    L3_VN_MOD,        /* 修改一条表项 */
    L3_VN_DEL,        /* 删除一条表项 */
    L3_VN_BUTT
} L3_VN_OPTYPE_E;

/**************************路由下发驱动操作类型***************************/
typedef enum tagL3RouteOpType
{
    L3_ROUTE_ADD,        /* 增加一条表项 */
    L3_ROUTE_MODIFY,     /* 修改一条表项 */
    L3_ROUTE_DEL,        /* 删除一条表项 */
    L3_ROUTE_BUTT
 } L3_ROUTE_OPTYPE_E;

/**************************转发特定类型报文枚举定义***************************/
typedef enum tagIPFORWARD_OPTION
{
    IPFORWARD_UNREACHABLE,                 /*网络地址不可达报文*/
    IPFORWARD_TTLEXPIRE,                   /*TTL为1的报文*/
    IPFORWARD_ICMPREDIRECT,                /*ICMP重定向报文*/
    IPFORWARD_ICMP_FRAG_FORWARDING,        /*设置是否转发ICMP分片报文的功能*/
    IPFORWARD_LOADSHARING_MODE,            /*设置产品硬件三层转发的负载分担方式*/
    IPFORWARD_DIRECTBROADCAST,             /*直连广播报文*/
    IPFORWARD_IF_DIRECTBROADCAST,          /*基于接口的直连广播报文*/
    IPFORWARD_ARPSRCMAC_CHECK,             /* 设置ARP报文源MAC一致性检查 */
    IPFORWARD_COMPATIBLE_IPV4_FORWARDING,  /* 设置是否转发IPv4兼容IPv6地址格式的报文 */
    IPFORWARD_MC_NONRPF,                   /* 设置对RPF检查失败的三层组播报文的处理方式 */
    IPFORWARD_SETMAXECMP,                  /*设置最大ECMP个数*/
    IPFORWARD_SET_ECMP_MODE,               /* 设置ECMP 模式 */
    IPFORWARD_SET_OPTION_DROP,             /* 设置ipv6 option drop使能标记 */  
    IPFORWARD_SET_LOCALFIRST,               /* 设置本地优先 */
    IPFORWARD_OPTION_BUTT
} IPFORWARD_OPTION_E;

/**************************转发报文处理类型宏定义***************************/
#define IPFORWARD_TOCPU         1   /* 上送CPU */
#define IPFORWARD_NOTTOCPU      2   /* 不上送CPU */
#define IPFORWARD_BROADCAST     3   /* 广播 */
#define IPFORWARD_NOTBROADCAST  4   /* 不广播 */
#define IPFORWARD_BRIDGING      5   /* 进行二层转发 */
#define IPFORWARD_NOTBRIDGING   6   /* 不进行二层转发 */

typedef struct tagIPLoadSharingConf
{
    UINT uiLocalFirstSupport;       /* 是否支持负载分担本地优先命令,ENABLE表示支持,DISABLE表示不支持 */
    UINT uiLocalFirstType;          /* 负载分担本地优先类型,本板or本框优先 */
    UINT uiLocalFirstDefault;       /*负载分担本地优先缺省值，ENABLE表示开启,DISABLE表示关闭 */
}IP_LOADSHARING_CONF_S;

/* 设定RPF检查失败的三层组播报文的处理方式，如是否上送CPU或在入VLAN内广播等 */
typedef struct  tagIPMCNONRPF_PARAM
{
    IF_INDEX ifIndex;     /* 接口索引 */
    USHORT usType;        /* 处理方式 */
    USHORT usVlanID;      /* ifIndex对应VLAN虚接口的VLAN索引。*/
}IPMCNONRPF_PARAM_S;

/*用于设置负载分担模式*/
typedef struct tagIP_LoadSharingMode
{
    ULONG  ulType;            /*逐包还是逐流*/
    UINT64 uiOptionBitMap;   /*逐流时,哪些报文特征参与负载分担的决策*/
}IP_LOADSHARING_MODE_S;

/*****************************转发表项下发相关定义********************************/
/* VN下一跳信息 */
typedef struct tagL3Nexthop
{
    UCHAR        ucPrefixIndex;               /* 此下一跳路径对的应前缀表项的编号 */
    UCHAR        ucStatus;                    /* 下一跳状态 */
    USHORT       usFlag;                      /* 下一跳属性 */
    IF_INDEX     ifIndex;                     /* 出接口索引 */
    INET_ADDR_S  stNexthopIp;                 /* 下一跳地址 */
    UINT32       uiNhRouteStatus;             /* 下一跳地址转发属性 */
    INET_ADDR_S  stLastAddr;                  /* BGP邻接域下一跳 */
    UINT32       uiDataMTU;
    UINT32       uiWeight;                    /* 权重 */
    UINT64       auiTunnelInfo[2];
    union                                   /* 根据uiFlag中是否有L3_VN_MPLS决定封装类型 */
    {
        L3_ADJACENT_LINK_S   stL3ADJLink;   /* 链路层封装信息 */
        MPLS_NHLFE_FWDINFO_S stNhlfeInfo;   /* MPLS封装信息 */
    }unNexthop;
    #define nh_stL3ADJLink unNexthop.stL3ADJLink
    #define nh_stNhlfeInfo unNexthop.stNhlfeInfo
    UINT32     auiContext[VN_NEXTHOP_DRVCONTEXT_LEN];  /* 驱动返回信息 */
} L3_NEXTHOP_S;

/* VN下驱动结构 */
typedef struct tagL3Vn
{
    USHORT usVnFlag;                            /* Vn属性，各个下一跳的属性合集 */
    USHORT usVnNexthopNum;                      /* 下一跳数量 */
    UINT32 auiVnDrvContext[VN_DRVCONTEXT_LEN];  /* VN DrvContext */
    L3_NEXTHOP_S astL3Nexthop[1];               /* 下一跳信息 */
} L3_VN_S;


/* 向驱动下发IPv4 ROUTE表项的数据结构 */
typedef struct tagL3Route
{
    INADDR_S stDstIp;
    INADDR_S stDstMask;
    VRF_INDEX vrfIndex;
    BOOL_T bIsVnDownload;
    USHORT usIpPrecedence;         /* QPPB策略设置的IP优先级 */
    USHORT usQosLocalId;           /* QPPB策略设置的Qos-local-id值 */
    USHORT usFlowIndex;            /* BGP的流量索引 */
    ULONG  ulVnHandle;
    UINT32 uiRouteStatus;
    UINT32 uiOriginAs;
    UINT32 uiLastAs;               /* BGP邻接自治域 */
    UINT32 uiLabel;                /* 标签  */
    UINT32 uiLabelBackup;          /* 备份标签  */
    L3_NEXTHOP_S stNexthop;
    UINT32 auiVnDrvContext[VN_DRVCONTEXT_LEN];   /* VN独立下发后回填的VnDrvContext */
    UINT32 auiContext[FIB_DRVCONTEXT_LEN_MAX];   /*用于回填*/
    UINT32 uiFIBSeq;               /* FPGA: FIB序号 */
}L3_ROUTE_S;

/* 向驱动下发IPv6 ROUTE表项的数据结构 */
typedef struct tagL3Route6
{
    IN6ADDR_S stDstIp;
    UCHAR     ucDstPrefixLen;
    VRF_INDEX vrfIndex;
    BOOL_T bIsVnDownload;
    USHORT usIpPrecedence;         /* QPPB策略设置的IP优先级 */
    USHORT usQosLocalId;           /* QPPB策略设置的Qos-local-id值 */
    USHORT usFlowIndex;            /* BGP的流量索引 */
    ULONG  ulVnHandle;
    UINT32 uiRouteStatus;
    UINT32 uiOriginAs;
    UINT32 uiLastAs;               /* BGP邻接自治域 */
    UINT32 uiLabel;                /* 标签  */
    UINT32 uiLabelBackup;          /* 备份标签  */
    L3_NEXTHOP_S stNexthop;
    UINT32 auiVnDrvContext[VN_DRVCONTEXT_LEN];   /* VN独立下发后回填的VnDrvContext */
    UINT32 auiContext[FIB_DRVCONTEXT_LEN_MAX];   /*用于回填*/
    UINT32 uiFIBSeq;               /* FPGA: FIB序号 */
}L3_ROUTE6_S;

typedef struct tagIPDrvIntf
{
    ULONG (*pfDRV_ADJ_HandleLinkChange)(IN ULONG ulOperation,
                                        IN L3_ADJACENT_LINK_S *pstOldADJLink,
                                        INOUT L3_ADJACENT_LINK_S *pstADJLink,
                                        OUT UINT *puiNotifyRefFlag,
                                        OUT ULONG *pulSynFlag);
    ULONG (*pfDRV_VN_HandleVnChange)(IN ULONG ulOperation,
                                     IN L3_VN_S *pstOldVn,
                                     INOUT L3_VN_S *pstVn,
                                     OUT UINT *puiNotifyRefFlag,
                                     OUT ULONG *pulSynFlag);
    ULONG (*pfDRV_IPv4_HandleRouteChange)(IN ULONG ulOperation,
                                          IN ULONG ulOldECMPNum,
                                          IN L3_ROUTE_S *pstOldRoute,
                                          IN ULONG ulECMPNum,
                                          INOUT L3_ROUTE_S *pstRoute,
                                          OUT ULONG *pulSynFlag);
    ULONG (*pfDRV_IPv4_SetForwardingOption)(IN UINT uiPacketType,
                                            IN VOID * pParam);
    ULONG (*pfDRV_IPv4_EnableProtocolToCPU)(IN UINT uiGlobal,
                                            IN UINT uiValue,
                                            IN UINT uiProtocolType,
                                            IN INADDR_S stDstIP,
                                            IN UINT uiMaskLen,
                                            IN UINT uiPara1,
                                            IN UINT uiPara2,
                                            IN UINT uiVlanID,
                                            IN IF_INDEX ifIndex,
                                            INOUT UINT64 auiDrvContext[IP_PROTOENDRVCONTEXTLEN]);
    ULONG (*pfDRV_IPv6_HandleRouteChange)(IN ULONG ulOperation,
                                          IN ULONG ulOldECMPNum,
                                          IN L3_ROUTE6_S *pstOldRoute,
                                          IN ULONG ulECMPNum,
                                          INOUT L3_ROUTE6_S *pstRoute,
                                          OUT ULONG *pulSynFlag);
    ULONG (*pfDRV_IPv6_SetForwardingOption)(IN UINT uiPacketType,
                                           IN VOID* pParam);
    ULONG (*pfDRV_IPv6_InterfaceEnable)(IN IF_INDEX ifIndex,
                                        IN ULONG ulEnable);
    ULONG (*pfDRV_IPv6_EnableProtocolToCPU) (IN UINT uiGlobal,
                                             IN UINT uiValue,
                                             IN UINT uiProtocolType,
                                             IN IN6ADDR_S stDstIP,
                                             IN UINT uiMaskLen,
                                             IN UINT uiPara1,
                                             IN UINT uiPara2,
                                             IN UINT uiVlanID,
                                             IN IF_INDEX ifIndex,
                                             INOUT UINT64 auiDrvContext[IP_PROTOENDRVCONTEXTLEN]);
    ULONG (*pfDRV_IP_GetLoadSharingPath)(IN IP_LOADSHARING_PATH_PARA_S *pstPara,
                                         OUT IP_LOADSHARING_PATH_RESULT_S *pstResult);
}IP_DRV_INTF_S, *PIP_DRV_INTF_S;

/* 获取/清除IP统计信息驱动接口，芯片只支持全局统计信息，暂时提供ipv4统计 */
typedef struct tagIPStatDrvIntf
{
    VOID (*pfDRV_IPv4_GetStat)(IN BOOL_T bGlobalStat, IN IF_INDEX ifIndex, OUT VOID* pParam);
    VOID (*pfDRV_IPv4_ResetStat)(IN BOOL_T bGlobalStat, IN IF_INDEX ifIndex);
}IP_STAT_DRV_INTF_S, *PIP_STAT_DRV_INTF_S;

/* 获取BPA统计信息驱动接口 */
typedef struct tagBPAStatDrvIntf
{
    VOID (*pfDRV_IP_GetBpaStat)(IN BOOL_T bGlobalStat, IN IF_INDEX ifIndex, IN USHORT usMaxStatNum, 
                                OUT USHORT *pusStatNum, OUT IP_BPA_STAT_S *pstStat);
    VOID (*pfDRV_IP6_GetBpaStat)(IN BOOL_T bGlobalStat, IN IF_INDEX ifIndex, IN USHORT usMaxStatNum, 
                                 OUT USHORT *pusStatNum, OUT IP_BPA_STAT_S *pstStat);
}IP_BPASTAT_DRV_INTF_S, *PIP_BPASTAT_DRV_INTF_S;

extern ULONG IP_Register_Driver (IN ULONG ulVersion, IN IP_DRV_INTF_S *pstDriverIPIntf);
extern ULONG IP_Stat_Register_Driver (IN ULONG ulVersion, IN IP_STAT_DRV_INTF_S *pstIPStatDrvIntf);
extern ULONG IP_BPAStat_Register_Driver (IN ULONG ulVersion, IN const IP_BPASTAT_DRV_INTF_S *pstBpaStatDrvIntf);
extern ULONG IP_LoadSharing_InitConf(IN const IP_LOADSHARING_CONF_S *pstLoadSharingConf);

/* IP驱动接口注册 */
extern ULONG IP_Install_Drv(IN LPVOID pStartContext);



/*****************************产品定制及初始化******************************************/
/*******************************************************************/
/*                            宏定义                               */
/*******************************************************************/
#define FIB4_HASH_LEVELNUM_MAX         7    /* 定制FIB4 HASH的最大级数 */


/*******************************************************************/
/*                            数据结构                             */
/*******************************************************************/

/* FIB存储方式定义 */
typedef enum tagFIB4_StorageEnum
{
    FIB4_STORAGE_HASH_1,   /* Hash方式20-4-4-4(4级)，占用内存相对多一点，查询效率相对高一些 */
    FIB4_STORAGE_HASH_2,   /* Hash方式16-8-8(3级)，占用内存较少，查询效率相对低一些(介于HASH_1和RADIX之间) */
    FIB4_STORAGE_RADIX,    /* Radix方式，占用内存少，查询效率低于Hash方式 */
    FIB4_STORAGE_BUTT
}FIB4_STORAGE_E;

typedef struct tagFIB4_Conf
{
    FIB4_STORAGE_E enPublic;    /* 公网FIB存储方式定制 */
    UINT uiDrvContextLen;       /* FIB4前缀DrvContext数量，与L3_ROUTE_S中的DrvContext对应，无默认值；无需下发驱动可以定制为0 */
    UINT uiVNDrvContextLen;     /* VN DrvContext数量，与L3_VN_S中的DrvContext对应，无默认值；无需下发驱动可以定制为0 */
    FIB4_SPECCONF_S stSpecConf; /* FIB4 硬件表项规格定制 */
} FIB4_CONF_S;

typedef struct tagFIB6_Conf
{
    UINT uiDrvContextLen;       /* FIB6前缀DrvContext数量，与L3_ROUTE6_S中的DrvContext对应，无默认值；无需下发驱动可以定制为0 */
    FIB6_SPECCONF_S stSpecConf; /* FIB6 硬件表项规格定制 */
} FIB6_CONF_S;

/*
 * J03845: 为保证驱动接口封装性，并支持可扩展，IPv4相关的驱动接口封装成EOM形式，
 * 各模块在使用驱动接口时，从EOM获取对应的接口，并判断接口是否存在；而不使用原有
 * 获取全局变量的方式来进行。
 */
#define EOM_ID_IP                  ((ULONG)EOM_OID_DEFINE(MID_WAN_BASE, 0x01))
#define EOM_ID_IPSTAT              ((ULONG)EOM_OID_DEFINE(MID_WAN_BASE, 0x11))
#define EOM_ID_IPBPASTAT           ((ULONG)EOM_OID_DEFINE(MID_WAN_BASE, 0x16))

/*******************************************************************/
/*                            函数原型                             */
/*******************************************************************/
/* FIB相关接口 */
/* 定制相关 */
extern ULONG FIB_Init(IN FIB4_CONF_S *pstConf);
extern ULONG FIB_Run(IN LPVOID pStartContext);

extern ULONG FIB6_Init(IN FIB6_CONF_S *pstConf);
extern ULONG FIB6_Run(IN LPVOID pStartContext);

extern ULONG USR_FIB_Run(IN LPVOID pStartContext);
extern ULONG USR_FIB_Init(IN LPVOID pStartContext);
extern ULONG USR_FIB6_Run(IN LPVOID pStartContext);
extern ULONG USR_FIB6_Init(IN LPVOID pStartContext);

extern ULONG FIB_MDCEventNotify(IN UINT uiEvent, IN VOID *pData);
extern ULONG FIB_MDCStopNotify(IN UINT uiEvent, IN VOID *pData);

/*******************************************************************/
/*                      产品定制快转规格结构定义                   */
/*******************************************************************/
/* IP 快转定制 */
typedef struct tagIPFS_Conf
{
    ULONG ulHashSize;     /* 快转表总容量 */
    UINT  uiCachePerCPU;  /* 每CPU缓存的快转表项 */
    UINT8 uiIsCRCHash;    /* 是否使用快速CRC算法 */
} IPFS_CONF_S;


/*******************************************************************/
/*                      快转函数原型                               */
/*******************************************************************/
/*快转规格注册函数*/
extern VOID IPFS_Init_Conf(IN IPFS_CONF_S *pstConfData);

extern ULONG USR_IPFS_Init( IN LPVOID pStartContext );

/*******************************************************************/
/*                    产品定制IPv6快转规格结构定义                 */
/*******************************************************************/
typedef struct tagIP6FS_Conf
{
    ULONG ulHashSize;     /* 快转表总容量 */
    UINT  uiCachePerCPU;  /* 每CPU缓存的快转表项 */
    UINT8 uiIsCRCHash;    /* 是否使用快速CRC算法 */
} IP6FS_CONF_S;

/*******************************************************************/
/*                      IPv6快转函数原型                           */
/*******************************************************************/
extern VOID IP6FS_Init_Conf(IN IP6FS_CONF_S *pstConfData);

extern ULONG USR_IP6FS_Init(IN LPVOID pStartContext);

#endif /* __KERNEL__ */


#ifdef __cplusplus
}
#endif /* end of __cplusplus */

#endif /* end of _DRV_IP_H_ */


