/*****************************************************************************
 Copyright (c) 2009, Hangzhou H3C Technologies Co., Ltd. All rights reserved.
------------------------------------------------------------------------------
                            ipmc.h
  Project Code: Comware
   Module Name: IP Multicast
  Date Created: 2009-9-18 
        Author: dahu
   Description: This file defines  multicast driver interface between driver and platform 

------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------
                                                                             
*****************************************************************************/

#ifndef _DRV_IPMC_H_
#define _DRV_IPMC_H_

#ifdef __cplusplus
extern "C" {
#endif

/*#ifdef __KERNEL__*/

/* 转发表操作类型 */
typedef enum tagIPMC_TYPE_E
{
    IPMC_L3_ADD_IPM,          /* 添加三层组播表项 */ 
    IPMC_L3_DEL_IPM,          /* 删除三层组播表项 */
    IPMC_L3_ADD_OIF,          /* 添加三层表项出接口 */
    IPMC_L3_DEL_OIF,          /* 删除三层表项出接口 */
    IPMC_L3_SET_IIF,          /* 更改入接口 */
    IPMC_L3_ADD_PORT,         /* 添加三层表项出端口 */
    IPMC_L3_DEL_PORT,         /* 删除三层表项出端口 */
    IPMC_L3_ADD_SLOT,         /* 添加三层表项分布式转发的出接口板信息 */
    IPMC_L3_DEL_SLOT,         /* 删除三层表项分布式转发的出接口板信息 */
    IPMC_L3_SET_DATA_GROUP,   /* 设置三层表项对应的Data-Group地址 */
    IPMC_L2_ADD_IP,           /* 添加二层组播IP表项 */
    IPMC_L2_DEL_IP,           /* 删除二层组播IP表项 */
    IPMC_L2_ADD_IP_PORT,      /* 添加二层组播IP表项出端口 */
    IPMC_L2_DEL_IP_PORT,      /* 删除二层组播IP表项出端口 */
    IPMC_L2_ADD_IP_SLOT,      /* 添加二层组播IP表项分布式转发的出接口板信息 */
    IPMC_L2_DEL_IP_SLOT,      /* 删除二层组播IP表项分布式转发的出接口板信息 */
    IPMC_L2_ADD_MAC,          /* 添加二层组播MAC表项 */
    IPMC_L2_DEL_MAC,          /* 删除二层组播MAC表项 */
    IPMC_L2_ADD_MAC_PORT,     /* 添加二层组播MAC表项出端口 */
    IPMC_L2_DEL_MAC_PORT,     /* 删除二层组播MAC表项出端口 */
    IPMC_L2_ADD_MAC_SLOT,     /* 添加二层组播MAC表项分布式转发的出接口板信息 */
    IPMC_L2_DEL_MAC_SLOT,     /* 删除二层组播MAC表项分布式转发的出接口板信息 */
    IPMC_L3_SET_ENTRY_RP,     /* 更改表项的RP信息 */
    IPMC_L3_ADD_ENTRY_DFIF,   /* 添加表项的DF接口信息 */
    IPMC_L3_DEL_ENTRY_DFIF,   /* 删除表项的DF接口信息 */
    IPMC_L3_ADD_RP,           /* 添加RP表项和DF接口信息 */
    IPMC_L3_DEL_RP,           /* 删除RP表项和DF接口信息 */
    IPMC_L3_ADD_RP_DFIF,      /* 添加RP的DF接口信息 */
    IPMC_L3_DEL_RP_DFIF,      /* 删除RP的DF接口信息 */
    IPMC_L3_SET_RP_IIF,       /* 更改RP表项的入接口 */
    IPMC_TYPE_BUTT
}IPMC_TYPE_E;


/* 组播转发表项出接口的位图属性 */
#define  L3_MROUTE_TUNNEL   0x1      /* 出接口为TUNNEL接口 */
#define  L3_MROUTE_VLANIF   0x2      /* 出接口为VLAN虚接口 */
#define  L3_MROUTE_ROUTERIF 0x4      /* 出接口为路由器接口 */
#define  L3_MROUTE_INIF     0x8      /* 出接口为入接口 */
#define  L3_MROUTE_MTUNNEL  0x10     /* 出接口为MTUNNEL接口 */
#define  L3_MROUTE_PRIVATE  0x20     /* 出接口为MTUNNEL接口时，如果同时携带了本标志，
                                        表明是私网MTUNNEL出接口，需要加封装；
                                        没有本标志则表明是公网MTUNNEL出接口，需要解封装 */
#define  L3_MROUTE_SUBVLAN  0x40     /* 出接口为SUBVLAN接口 */

#define  L3_MROUTE_NBMALINK 0x80     /* 出接口为NBMA类型接口 */

#define IPMC_PORT_DRV_LEN    2

typedef struct tagIPMC_PORT_S
{
    IF_INDEX  ifIndex;               /* 出端口的接口索引 */
    UINT32    uiIfType;              /* 出端口的类型，与接口管理提供的类型相同 */
    UINT32 auiDrvContext[IPMC_PORT_DRV_LEN];           /* 驱动维护的产品数据信息 */
}IPMC_PORT_S;

#define IPMC_IF_DRV_LEN       2
#define IPMC_IF_TUN_DRV_LEN   2

/* 出接口单项链表结构 */
typedef struct tagIPMC_OIF_S
{
    struct tagIPMC_OIF_S *pstNext;   /* 单向链表结构下一个节点的指针 */
    IF_INDEX ifIndex;                /* 出接口的接口索引 */
    UINT32 uiVlanID;                 /* VLAN出接口的VLAN ID */
    UINT32 uiIfType;                 /* 出接口的类型，与接口管理提供的类型值相同 */
    UINT32 uiFlag;                   /* 出接口属性 */
    UINT32 uiPortNum;                /* 出端口数组中的端口数目 */
    IPMC_PORT_S *pstPortList;        /* 端口数组指针 */
    UINT32 auiDrvContext[IPMC_IF_DRV_LEN];           /* 驱动维护的产品数据信息 */
    UINT32 auiTunnelDrvContext[IPMC_IF_TUN_DRV_LEN]; /* 隧道出接口的产品数据信息 */
    UINT32 uiDataGrpIp;              /* MTunnel出接口的DataGroup IP地址，网络序 */
    UINT32   uiSecondVlanID;         /* VLAN ID      */
    UINT32   uiUserID;               /* User ID      */ 
    UINT32   uiPVC;                  /* PVC ID      */ 
    UINT32 auiRemoteLinkAddr[4];     /* NBMA口远端链接地址 */
}IPMC_OIF_S;


/* 组播转发表项的位图属性 */
#define  L3_MROUTE_FWD              0x1      /* 表项正常转发 */
#define  L3_MROUTE_DROPPKT          0x2      /* 丢弃报文 */
#define  L3_MROUTE_TOCPU            0x4      /* 报文送CPU处理，不转发 */
#define  L3_MROUTE_FWDANDTOCPU      0x8      /* 正常转发，同时复制一份送CPU处理 */
#define  L3_MROUTE_RPFCHECKUNICAST  0x10     /* RPF检查方式，检查硬件单播路由表 */
#define  L3_MROUTE_RPFCHECKDESIGNED 0x20     /* RPF检查方式，检查组播表项入接口 */
#define  L3_MROUTE_DUMMY            0x40     /* 三层组播空转发项 */
#define  L3_MROUTE_MVLAN            0x80     /* 组播VLAN表项 */
#define  L3_MROUTE_IIFMTUNNEL       0x100    /* 表项入接口为组播TUNNEL接口 */
#define  L2_MROUTE_FWD              0x200    /* 表项正常转发 */
#define  L2_MROUTE_DROPPKT          0x400    /* 丢弃报文 */
#define  L2_MROUTE_TOCPU            0x800    /* 报文送CPU处理，不转发 */
#define  L2_MROUTE_FWDANDTOCPU      0x1000   /* 正常转发，同时复制一份送CPU处理 */
/* 本属性与其它上CPU属性（L2_MROUTE_TOCPU、L2_MROUTE_FWDANDTOCPU、L3_MROUTE_TOCPU、L3_MROUTE_FWDANDTOCPU）
 * 是位与的关系，位与上本标志位表明报文上送的CPU是辅助CPU，否则上送主CPU */
#define  L2_MROUTE_CFDPROXYCPU      0x2000   /* 上送辅助CPU的属性 */
#define  L3_MROUTE_BIDIR            0x4000   /* 双向PIM表项 */
#define  L2_MROUTE_VSI              0x8000   /* VSI表项 */
#define  L3_MROUTE_MTI_NEXTHOP      0x10000  /* 组播tunnel的下一跳信息 */
#define  L2_MROUTE_TRILLGATEWAY     0x20000  /* TRILL卸载模式中，在网关下发的MAC表项 */
#define  L2_MROUTE_TRILLACCESS      0x40000  /* TRILL卸载模式中，在非网关下发的MAC表项 */

#define  L3_MROUTE_IIFNBMA          0x80000  /* 表项入接口为NBMA接口 */

#define IPMC_ENTRY_DRV_LEN    4

/* 下发IPv4二、三层组播表项的数据结构 */
typedef struct tagIPMCINFO_S
{
    /* 基本信息 */ 
    UINT32 uiSrcIp;            /* 组播源IP地址，网络序 */
    UINT32 uiGrpIp;            /* 组播组IP地址，网络序 */
    UCHAR  aucGrpMACAddr[MAC_ADDR_LEN];   /* 组播组MAC地址 */
    UINT32 uiCmdType;          /* 控制命令 */
    UINT32 uiFlag;             /* 表项属性 */
    VRF_INDEX vrfIndex;        /* VRF索引 */
    UINT32 auiDrvContext[IPMC_ENTRY_DRV_LEN];    /* 驱动维护的产品数据信息 */
    
    /* 入接口信息 */
    IF_INDEX ifInIfIndex;
    UINT32 uiInVlanID;
    UINT32 uiInIfType;

    /* 原入接口信息 */
    IF_INDEX ifInIfIndexOld;
    UINT32 uiInVlanIDOld;
    UINT32 uiInIfTypeOld;

    /* L2的出端口信息 */
    UINT32 uiPortNum;                /* 出端口数组中的端口数目 */
    IPMC_PORT_S *pstPortList;        /* 端口数组指针 */

    /* L3出接口/出端口信息 */
    UINT32 uiOIfSize;
    IPMC_OIF_S *pstOIfList;
    
    /* 表项对应的RP信息 */
    UINT32 uiRPAddress;

    /* 分布式接口板信息，L2/L3通用 */
    BITMAP_S *pstSlotBitmap;

    UINT32 uiSeq; /* 组播转发表项序号 */

    UINT32 uiRemoteIp;         /* NBMA入接口远端IP地址，网络序 */
    
}IPMCINFO_S;


/* 获取三层组播表项报文统计信息的数据结构 */
typedef struct tagIPMC_COUNTERS_S
{
    VRF_INDEX vrfIndex;          /* VRF索引 */
    UINT32 uiSrcIp;              /* 三层组播表项源地址，网络序 */
    UINT32 uiDestIp;             /* 三层组播表项组地址，网络序 */
    UINT32 uiFlag;               /* 表项属性 */
    IF_INDEX ifInIfIndex;        /* 三层表项入接口索引 */
    UINT32 uiInVlanID;           /* 三层表项入接口VLAN ID */
    UINT32 uiMatchedPacketNum;   /* 转发表项匹配到的报文数目 */
    UINT32 uiMatchedPacketByte;  /* 转发表项匹配到的报文的字节数目 */
    UINT32 uiForwardPacketNum;   /* 匹配转发表项后，往出接口转发报文的数目 */
    UINT32 uiForwardPacketByte;  /* 匹配转发表项后，往出接口转发报文的字节数 */
    UINT32 uiWrongIfPktNum;      /* 从错误入接口收到的报文数目 */
    UINT32 auiDrvContext[IPMC_ENTRY_DRV_LEN];      /* 驱动维护的产品数据信息 */
}IPMC_COUNTERS_S;

/* MTUNNEL操作类型 */
typedef enum tagIPMC_MTUNNEL_TYPE_E
{
    IPMC_MTUNNEL_CREATE,          /* 添加MTUNNEL */ 
    IPMC_MTUNNEL_DELETE,          /* 删除MTUNNEL*/
    IPMC_MTUNNEL_UP,              /* MTUNNEL UP，携带完整封装信息 */ 
    IPMC_MTUNNEL_DOWN,            /* MTUNNEL DOWN */
    IPMC_MTUNNEL_TYPE_BUTT
}IPMC_MTUNNEL_TYPE_E;

/* 组播tunnel模式 */
typedef enum tagMTUNNEL_MODE
{
    MTUNNEL_MODE_IPV4_MD_GRE = 1,
    MTUNNEL_MODE_IPV4_VXLAN_UDP,
    MTUNNEL_MODE_MAX
}MTUNNEL_MODE_E;
/* 下发IPv4组播MTunnel接口的数据结构 */
typedef struct tagIPMC_MTUNNEL_S 
{
    UINT32 uiCmdType;               /* 控制命令, IPMC_MTUNNEL_TYPE_E */
    UINT32 uiTunnelMode;            /* 隧道模式 */
    IF_INDEX ifIndex;               /* MTI接口索引 */
    VRF_INDEX vrfIndex;             /* VRF索引 */
    USHORT usFlag;                  /* 接口属性，暂未使用 */
    UINT32 uiSrcIp;                 /* 隧道源IP地址，网络序 */
    UINT32 uiDestIp;                /* 隧道目的IP地址，网络序 */
    UINT32 uiMTU;                   /* 隧道接口MTU */
    UINT32 auiDrvContext[IPMC_IF_TUN_DRV_LEN]; /* 驱动维护的产品数据信息 */        
}IPMC_MTUNNEL_S;

#define  IPMC_L3MCAST_ENABLE           0x0      /* 使能三层组播 */
#define  IPMC_L3MCAST_DISABLE          0x1      /* 关闭三层组播 */
#define  IPMC_IGMPSOOPING_ENABLE       0x2      /* 使能IGMP Snooping协议 */
#define  IPMC_IGMPSOOPING_DISABLE      0x3      /* 去使能IGMP Snooping协议 */
#define  IPMC_PIMSOOPING_ENABLE        0x4      /* 使能PIM Snooping协议 */
#define  IPMC_PIMSOOPING_DISABLE       0x5      /* 去使能PIM Snooping协议 */
#define  IPMC_DROPUNKNOWN_ENABLE       0x6      /* 使能未知组播数据报文丢弃功能 */
#define  IPMC_DROPUNKNOWN_DISABLE      0x7      /* 去使能未知组播数据报文丢弃功能 */
#define  IPMC_ROUTERPORT_ADD           0x8      /* 添加路由器端口 */
#define  IPMC_ROUTERPORT_DELETE        0x9      /* 删除路由器端口 */
#define  IPMC_MVLAN_ENABLE             0xa      /* 使能组播VLAN*/
#define  IPMC_MVLAN_DISABLE            0xb      /* 去使能组播VLAN */
#define  IPMC_SOURCEDENY_ENABLE        0xc      /* 使能组播数据源端口抑制功能*/
#define  IPMC_SOURCEDENY_DISABLE       0xd      /* 去使能组播数据源端口抑制功能 */
#define  IPMC_BIDIR_ENABLE             0xe      /* 使能双向PIM */
#define  IPMC_BIDIR_DISABLE            0xf      /* 关闭双向PIM */
#define  IPMC_IGMPSOOPING_VSI_ENABLE   0x10     /* VSI内使能IGMP snooping协议 */
#define  IPMC_IGMPSOOPING_VSI_DISABLE  0x11     /* VSI内去使能IGMP snooping协议 */
#define  IPMC_PIMSOOPING_VSI_ENABLE    0x12     /* VSI内使能PIM snooping协议 */
#define  IPMC_PIMSOOPING_VSI_DISABLE   0x13     /* VSI内去使能PIM snooping协议 */
#define  IPMC_DROPUNKNOWN_VSI_ENABLE   0x14     /* 在VSI内使能未知组播数据报文丢弃功能*/
#define  IPMC_DROPUNKNOWN_VSI_DISABLE  0x15     /* 在VSI内去使能未知组播数据报文丢弃功能 */
#define  IPMC_ROUTERPORT_VSI_ADD       0x16     /* VSI内添加路由器端口 */
#define  IPMC_ROUTERPORT_VSI_DELETE    0x17     /* VSI内删除路由器端口 */


#define IPMC_ENABLE_DRV_LEN    2

/* 驱动向平台注册组播驱动接口的函数指针数据结构 */
typedef struct tagIPMC_DRV_INTF_S
{
    ULONG (*pfDRV_MC_HandleMRouteChange)(IN OUT IPMCINFO_S * pstIpmcinfo );
    ULONG (*pfDRV_MC_EnableL3Multicast)(IN UINT32 uiFlag, IN IF_INDEX ifIndex, IN UINT32 uiVlanID,
                                        IN UINT32 uiIfType, IN VRF_INDEX vrfIndex,  INOUT UINT32 auiDrvContext[2]);
    ULONG (*pfDRV_MC_GetIPMCStatistic)(IN OUT IPMC_COUNTERS_S *pstIPMCCounter);
    ULONG (*pfDRV_MC_EnableL2Multicast)(IN UINT32 uiCmdType, IN UINT32 uiVlanIDorVSIID, INOUT UINT32 auiDrvContext[IPMC_ENABLE_DRV_LEN]);
    ULONG (*pfDRV_MC_EnableDropUnknown) (IN UINT32 uiFlag, IN UINT32 uiVlanIDorVSIID, INOUT UINT32 auiDrvContext[2]);
    ULONG (*pfDRV_MC_SetRouterPort)(IN UINT32 uiCmdType, IN UINT32 uiVlanIDorVSIID, IN IPMC_PORT_S *pstPort);
    ULONG (*pfDRV_MC_MVlanEnable)(IN UINT32 uiCmdType, IN UINT32 uiVlanID);
    ULONG (*pfDRV_MC_SourceDeny)(IN UINT32 uiCmdType, IN IPMC_PORT_S *pstPort);
    ULONG (*pfDRV_MC_HandleMTunnelChange)(INOUT IPMC_MTUNNEL_S *pstIpmcMtunnel);
}IPMC_DRV_INTF_S, *pIPMC_DRV_INTF_S;

/* 驱动向平台注册组播驱动接口的函数 */
ULONG IPMC_Register_Driver(IN ULONG ulVersion, IN IPMC_DRV_INTF_S *psDriverIPMCIntf);



/* 组播转发定制 */
typedef struct tagIPMC_CONF
{
    ULONG   ulSpecMaxIpv4L3McEntryNum;  /* ipv4三层转发表项规格 */
    ULONG   ulSpecMaxIpv4MvlanEntryNum; /* ipv4组播VLAN转发表项规格 */
    ULONG   ulSpecMaxIpv4BidirRPNum;    /* ipv4双向PIM RP规格 */
    ULONG   ulSpecMaxSoftFwdOIfNum;     /* 软转发支持的出接口数量.       */
    BOOL_T  bSupportAsicFwd;            /* 是否支持下驱动.               */
    BOOL_T  bSupportSoftFwdCnt;         /* 是否支持软转发计数统计,不支持的情况下只统计硬件转发记数，建议定义为false节省内存 */
} IPMC_CONF_S;


/* 驱动向平台注册IPv4三层组播转发规格的函数 */
VOID IPMC_Init_Conf(IN IPMC_CONF_S *pstConfData);

/* 定制注册和初始化函数 */
ULONG USR_IPMC_DrvInit( IN LPVOID pContext );
ULONG USR_IPMC_Init( IN LPVOID pContext);
ULONG USR_IPMC_Run( IN LPVOID pContext );

/* 组播转发模块初始化. */
ULONG IPMC_Init( IN LPVOID pStartupContext);
ULONG IPMC_Run( IN LPVOID pStartupContext);

/* 三层组播中向VLAN出接口转发时，只做指定VLAN接口发送，不再遍历VLAN内的二层出端口逐个发送。 */
VOID IPMC_EnableVlanOutIfHardForwarding(VOID);
/* 三层组播中，向VLAN入接口内的其它端口软件转发。 */
VOID IPMC_EnableVlanInIfSoftForwarding(VOID);
/* 双向PIM支持每表项维护DF */
VOID IPMC_EnableBidirPimPerEntryDf(VOID);


#ifdef __KERNEL__

/* 驱动通知的事件类型 */
typedef enum tagIPMC_EVT
{
    /* MTI相关: 通知业务环回口是否可用， pData为BOOL_T类型，BOOL_TRUE表示可用，BOOL_FALSE表示不可用。
       默认是可用的，依赖于业务环回口的产品需要在初始化阶段调用完IPMC_Run()后通知平台不可用，
       分布式设备上只需要在主控板调用，接口板不处理该事件。
     */ 
    IPMC_EVT_MTI_SET_SERVICE_LOOPBACK = 1, 

    /* MTI相关: 重刷隧道表项事件， pData无意义 */
    IPMC_EVT_MTI_MD_REFRESH,      /* 组播VPN的组播隧道重刷 */
    IPMC_EVT_MTI_VXLAN_REFRESH,    /* VXLAN的组播隧道重刷 */

    IPMC_EVT_BUTT, 
}IPMC_EVT_E;

/*****************************************************************************
    Func Name: IPMC_EventNotify
 Date Created: 2012/9/7 
       Author: x02188
  Description: 驱动通知平台消息接口
        Input: IN IPMC_EVT_E enEvtType, 事件类型
               IN VOID *pData, 参数，具体与事件类型相关  
       Output: NA
       Return: ULONG, ERROR_SUCCESS 处理成功
                      ERROR_FAILED  处理失败
      Caution: 需要区分VD
------------------------------------------------------------------------------
  Modification History                                                      
  DATE        NAME             DESCRIPTION                                  
  --------------------------------------------------------------------------
                                                                            
*****************************************************************************/
ULONG IPMC_EventNotify(IN IPMC_EVT_E enEvtType, IN VOID *pData);

/*****************************************************************************
    Func Name: IPMC_MtunnelReceivePacket
 Date Created: 2012/12/24 
       Author: x02188
  Description: 驱动上送MTI解封装后的报文
        Input: IN IF_INDEX ifIndex, MTI接口
               IN MBUF_S *pstMBuf, 解封装后的IP报文
       Output: NA
       Return: ULONG, ERROR_SUCCESS 处理成功
                      ERROR_FAILED  处理失败
      Caution: 解封装后的报文无二层头，加封装的报文不能使用本接口上送
------------------------------------------------------------------------------
  Modification History                                                      
  DATE        NAME             DESCRIPTION                                  
  --------------------------------------------------------------------------
                                                                            
*****************************************************************************/
ULONG IPMC_MtunnelReceivePacket(IN IF_INDEX ifIndex, IN MBUF_S *pstMBuf);

#endif  /* __KERNEL__ */

#ifdef __cplusplus
}
#endif


/*#endif    __KERNEL__ */

#endif   /* _DRV_IPMC_H_ */
