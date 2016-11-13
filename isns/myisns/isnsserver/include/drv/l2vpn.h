/*****************************************************************************
 Copyright (c) 2010, Hangzhou H3C Technologies Co., Ltd. All rights reserved.
------------------------------------------------------------------------------
                              drv/l2vpn.h
  Project Code: Comware
   Module Name: L2VPN
  Date Created: 2011-05-05
        Author: y02971
   Description: driver macros/data types/interfaces for L2VPN
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------
  YYYY-MM-DD

*****************************************************************************/

#ifndef _DRV_L2VPN_H_
#define _DRV_L2VPN_H_

#ifdef __cplusplus
extern "C"{
#endif

#ifndef _SYS_IN_H_
#include <sys/in.h>
#endif

#define EOM_ID_L2VPNDRV            ((ULONG)EOM_OID_DEFINE(MID_MPLS_BASE, 0x02))
#define EOM_ID_MINMDRV             ((ULONG)EOM_OID_DEFINE(MID_MPLS_BASE, 0x03))

#define L2VPN_INVALID_VSIINDEX 0xFFFFFFFF
#define L2VPN_INVALID_LINKID   0xFFFF
#define L2VPN_INVALID_ISID     ((UINT)0)           /* 无效的I-SID */
#define L2VPN_INVALID_VXLANID  (UINT)(-1)          /* 无效的VXLAN-ID */

#define L2VPN_INVALID_NVGREID  (UINT)(-1)          /* 无效的NVGRE-ID */

#define L2VPN_INVALID_UINTLINKID   0xFFFFFFFF

#define L2VPN_MINM_DRVCONTEXT_LENGTH   6
#define L2VPN_MINMIF_DRVCONTEXT_LENGTH 2
#define L2VPN_DRVCONTEXT_MAXLENGTH   8
#define L2VPN_FORWARD_DRVCONTEXT_LENGTH 2


#define L2VPN_INVALID_DRVCONTEXT  (UINT)(-1)

/* VSI索引的最高位为1表明是VPWS实例，为0表明是VPLS实例 */
#define L2VPN_VSI_VPWSFLAG 0x80000000
#define L2VPN_VSI_ISVPWS(uiVsiIndex) ((L2VPN_INVALID_VSIINDEX != (uiVsiIndex)) && \
                                      (L2VPN_VSI_VPWSFLAG == ((uiVsiIndex) & L2VPN_VSI_VPWSFLAG)))

/* AC封装类型  */
typedef enum tagL2VPN_AC_ENCAPTYPE
{
    L2VPN_AC_ENCAP_NONE,
    L2VPN_AC_ENCAP_EVC,        /* MEF EVC VLAN匹配列表 */
    L2VPN_AC_ENCAP_SATOP,      /* PWE3 TDM电路仿真SATOP方式 */
    L2VPN_AC_ENCAP_CESOPSN,    /* PWE3 TDM电路仿真CESoPSN方式 */
    L2VPN_AC_ENCAP_CELLTRANS,  /* PWE3 ATM信元透传 */
    L2VPN_AC_ENCAP_IPINTERWORKING, /* IP interwoking方式 */
    L2VPN_AC_ENCAP_MAX
}L2VPN_AC_ENCAPTYPE_E;

/* AC模式标记定义 */
/* Ethernet/VLAN属性占第0、1位两个bit */
#define L2VPN_AC_MODE_ETHERNET  0x01    /* 接入模式为Ethernet */
#define L2VPN_AC_MODE_VLAN      0x02    /* 接入模式为VLAN */
/* Hub-spoke属性占第2、3位两个bit: 第2位bit标记为是否使能hub-spoke，第3位bit标记hub-spoke属性 */
#define L2VPN_AC_MODE_SPOKE     0x04    /* AC为spoke */
#define L2VPN_AC_MODE_HUB       0x0C    /* AC为Hub */

/* AC封装参数信息 */
typedef struct tagL2VPN_AC_ENCAP
{
    IF_INDEX             ifIndex;          /* 接口索引 */
    USHORT               usServiceID;
    USHORT               usACLinkID;       /* AC的Link ID */
    IF_INDEX             ifLogicIndex;     /* AC的逻辑索引 */
    UCHAR                ucACModeFlag;     /* AC模式标记 */
    L2VPN_AC_ENCAPTYPE_E enACEncapType;    /* AC封装类型 */
    VOID                 *pData;           /* 结构指针 */
}L2VPN_AC_ENCAP_S;

#define L2VPN_AC_STATE_NORMAL 0 /* AC状态为非阻塞状态 */
#define L2VPN_AC_STATE_BLOCK 1 /* AC状态为阻塞状态 */

/* L2VPN AC表项信息 */
typedef struct tagL2VPN_AC_S
{
    USHORT              usMtu;             /* MTU */
    UINT                uiVsiIndex;        /* VSI索引 */
    L2VPN_AC_ENCAP_S    stAcInfo;
    USHORT              usOutLinkID;
    USHORT              usState;
    UINT                uiStatFlag;        /* Ac标记位,流量统计使用0x01 */
    UINT32              auiDrvContext[L2VPN_DRVCONTEXT_MAXLENGTH];
}L2VPN_AC_S;

/* VSI与接口绑定/去绑定信息 */
typedef struct tagL2VPN_CFG_IF
{
    UINT               uiVsiIndex;        /* VSI索引 */
    L2VPN_AC_ENCAP_S   stAcInfo;
    UINT32             auiDrvContext[L2VPN_DRVCONTEXT_MAXLENGTH];
}L2VPN_CFG_IF_S;


/* VSI属性更新标记定义 */
#define    L2VPN_VSI_UPD_BCRESTRAIN      0x0001     /* 广播抑制率更新 */
#define    L2VPN_VSI_UPD_MCRESTRAIN      0x0002     /* 组播抑制率更新 */
#define    L2VPN_VSI_UPD_UNKNOWNRESTRAIN 0x0004     /* 未知单播抑制率更新 */
#define    L2VPN_VSI_UPD_BWLIMIT         0x0008     /* VSI带宽值更新 */
#define    L2VPN_VSI_UPD_MACLIMIT        0x0010     /* MAC地址容量更新 */
#define    L2VPN_VSI_UPD_MACLEARNING     0x0020     /* MAC学习能力更新 */
#define    L2VPN_VSI_UPD_DROPUNKNOWN     0x0040     /* MAC表达到最大数目后丢弃未知单播报文 */
#define    L2VPN_VSI_UPD_ISID            0x0080     /* 更新I-SID */
#define    L2VPN_VSI_UPD_PWSIGNAL        0x0100     /* 更新PWSIGNAL */
#define    L2VPN_VSI_UPD_DUPLICATEMODE   0x0200     /* SPB的复制模式更新 */
#define    L2VPN_VSI_UPD_PBBENCAP        0x0400     /* PBB的封装模式更新 */
#define    L2VPN_VSI_UPD_VXLANID         0x0800     /* VXLAN的ID更新 */
#define    L2VPN_VSI_UPD_MACLEARNRATE    0x1000     /* MAC地址学习速率限制更新 */
#define    L2VPN_VSI_UPD_VSISTATISTICS   0x2000     /* VSI统计能力更新 */
#define    L2VPN_VSI_UPD_NVGREID         0x4000     /* NVGRE的ID更新 */

/* VSI模式标记定义 */
#define    L2VPN_VSI_MODE_VPLS       0x01     /* VPLS属性 */
#define    L2VPN_VSI_MODE_VPWS       0x02     /* VPWS属性 */
#define    L2VPN_VSI_MODE_PW         0x04     /* PW模式(STATIC、LDP、BGP) */
#define    L2VPN_VSI_MODE_HUB_SPOKE  0x08     /* HUB-SPOKE模式 */
#define    L2VPN_VSI_MODE_COREDUP    0x10     /* 核心复制模式 */
#define    L2VPN_VSI_MODE_PBB_ETH    0x20     /* PBB ethernet封装 */

/* L2VPN MAC学习标记定义 */
#define  L2VPN_VSI_MACLEARN_ENABLE          0x01
#define  L2VPN_VSI_MACLEARN_MAX_DROP        0x02

/* L2VPN VSI流量统计标记定义 */
#define  L2VPN_VSI_STATISTICS_ENABLE        0x01

/* L2VPN AC流量统计标记定义 */
#define  L2VPN_AC_STATISTICS_ENABLE        0x01

/* L2VPN AC软学习使能标记定义 */
#define  L2VPN_AC_SOFTWARE_MACLEARNING_ENABLE        0x02

/* L2VPN VSI相关配置信息 */
typedef struct tagL2VPN_CFG_VSI
{
    UCHAR     ucMacLearnFlag;       /* MAC学习标记 */
    UINT      uiVsiIndex;           /* 对应的VSI索引值 */
    UINT      uiBandwidth;          /* VPN速率限制 */
    UINT      uiMacLimit;           /* VPN内MAC地址总数限制 */
    UINT      uiMinmISID;           /* 此字段为VSI的PBB I-SID*/
    UINT      uiSpbISID;            /* 此字段为VSI的SPB I-SID*/
    UINT      uiVxlanID;            /* 此字段为VSI的VXLAN-ID */
    IF_INDEX  ifIndex;              /* VSI虚接口接口索引 */
    USHORT    usUpdateFlag;         /* VSI更新标记 */
    UCHAR     ucVsiMode;            /* VSI模式 */
    UCHAR     ucStatisticsFlag;     /* VSI流量统计标记 */
    USHORT    usMacLearnRate;       /* MAC地址学习速率限制，一个MAC学习的毫秒数 */
    UINT      uiBroadCastRestrain;  /* 广播抑制，默认5120 kbps */
    UINT      uiMultiCastRestrain;  /* 组播抑制，默认5120 kbps */
    UINT      uiUnknownRestrain;    /* 未知单播抑制，默认5120 kbps */
    UINT      uiNvgreID;            /* 此字段为VSI的NVGRE-ID */
    UINT32    auiDrvContext[L2VPN_DRVCONTEXT_MAXLENGTH];      /* 产品返回的信息 */
}L2VPN_CFG_VSI_S;

ISSU typedef enum tagL2VIF_TYPE
{
    L2VIF_AC_TYPE,
    L2VIF_MPLSPW_TYPE,
    L2VIF_MINM_UC_TYPE,
    L2VIF_MINM_MCPBB_TYPE,
    L2VIF_MINM_MCSPB_TYPE,
    L2VIF_OVERLAY_UCTUNNEL_TYPE,
    L2VIF_OVERLAY_MCTUNNEL_TYPE,
    L2VIF_NVGRETUNNEL_TYPE,
    L2VIF_MAX_TYPE,
}L2VIF_TYPE_E;

/* Overlay Tunnel LinkID定义:
  0                   1                   2                   3
  0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 |L2VIfType(8bit)|                 Tunnel Number                 |
 +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/

/* 通过LinkID判断是否是Overlay Tunnel */
#define L2VPN_IS_OVERLAYTUNNEL(uiLinkID) (((uiLinkID >> 24) == L2VIF_OVERLAY_UCTUNNEL_TYPE) ||\
                                       ((uiLinkID >> 24) == L2VIF_OVERLAY_MCTUNNEL_TYPE) ||\
                                       ((uiLinkID >> 24) == L2VIF_NVGRETUNNEL_TYPE))

/* 流量统计基本数据 */
typedef struct tagL2VPN_STATISTICS
{
    UINT64 uiInPackets;
    UINT64 uiInOctets;
    UINT64 uiInErrors;
    UINT64 uiInDiscards;
    UINT64 uiOutPackets;
    UINT64 uiOutOctets;
    UINT64 uiOutErrors;
    UINT64 uiOutDiscards;
    UINT32 uiIllegalVlan;    /* PW所收到的含非法VLAN报文的个数，例如缺失VLAN或携带错误的VLAN */
    UINT32 uiIllegalLength;  /* PW所收到的长度非法的报文的个数，例如报文长度超过MTU或小于以太网报文最小长度 */
}L2VPN_STATISTICS_S;

/* 流量统计类型 */
typedef enum tagL2VPN_STATISTICS_TYPE
{
    L2VPN_STATISTICS_PW,     /* PW流量统计类型 */
    L2VPN_STATISTICS_VSI,    /* VSI流量统计类型 */
    L2VPN_STATISTICS_AC,    /* AC流量统计类型 */
    L2VPN_STATISTICS_MAX
}L2VPN_STATISTICS_TYPE_E;

/* 流量统计对象:VSI */
typedef struct tagL2VPN_STATISTICS_OBJECT_VSI
{
    UINT    uiVsiIndex;                             /* VSI Index */
    UINT32  auiContext[L2VPN_DRVCONTEXT_MAXLENGTH]; /* VSI DrvContext */
}L2VPN_STATISTICS_OBJECT_VSI_S;

/* 配置操作类型 */
typedef enum tagL2VPN_IOCTL
{
    L2VPN_IOCTL_GLOBALENABLE,               /* L2VPN全局使能 */
    L2VPN_IOCTL_GLOBALDISABLE,              /* L2VPN全局禁止 */
    L2VPN_IOCTL_VLAN_RESERVE,               /* 保留VLAN     */
    L2VPN_IOCTL_VLAN_RELEASE,               /* 释放保留的VLAN */
    L2VPN_IOCTL_VSI_ADD,                    /* VSI创建 */
    L2VPN_IOCTL_VSI_DELETE,                 /* VSI删除 */
    L2VPN_IOCTL_VSI_UPDATE,                 /* VSI更新 */
    L2VPN_IOCTL_INTF_BIND,                  /* 接口绑定二层VPN */
    L2VPN_IOCTL_INTF_UNBIND,                /* 接口去绑定二层VPN */
    L2VPN_IOCTL_SYNC_START,                 /* 同步开始标识 */
    L2VPN_IOCTL_SYNC_END,                   /* 同步结束标识 */
    L2VPN_IOCTL_INTF_ENABLEMINM,            /* 接口使能MinM */
    L2VPN_IOCTL_INTF_DISABLEMINM,           /* 接口去使能MinM */
    L2VPN_IOCTL_BATCHPROC_START,            /* L2VPN批量表项处理开始 */
    L2VPN_IOCTL_BATCHPROC_END,              /* L2VPN批量表项处理结束 */
    L2VPN_IOCTL_VXLAN_RESERVE,              /* 保留VXLAN     */
    L2VPN_IOCTL_VXLAN_RELEASE,              /* 释放保留的VXLAN */
    L2VPN_IOCTL_VSIIF_BIND,                 /* VSI绑定VSI虚接口 */
    L2VPN_IOCTL_VSIIF_UNBIND,               /* VSI去绑定VSI虚接口 */
    L2VPN_IOCTL_REFLECTOR_ENABLE,           /* 配置检测地址 */
    L2VPN_IOCTL_REFLECTOR_DISABLE,          /* 取消检测地址 */
    L2VPN_IOCTL_MAX
}L2VPN_IOCTL_E;

/* L2VPN Reflector相关配置信息 */
typedef struct tagL2VPN_REFLECTOR
{
    UINT        uiVsiIndex;           /* 对应的VSI索引值 */
    IF_INDEX    ifIndex;              /* 检测地址对应的接口索引 */
    INET_ADDR_S stReflectorIpAddr;    /* Reflector IP address */
    UCHAR       aucMacAddress[MAC_ADDR_LEN];     /* Reflector IP address对应的MAC地址 */
    USHORT      usSrcPort;            /* Reflector 源端口号 */
    USHORT      usDstPort;            /* Reflector 目的端口号 */
    UINT32      auiDrvContext[L2VPN_DRVCONTEXT_MAXLENGTH];/* 产品返回的信息 */
}L2VPN_REFLECTOR_S;

/* L2VPN 表项变化的操作命令字 */
typedef enum tagL2VPN_CMD
{
    L2VPN_CMD_ADD,
    L2VPN_CMD_DELETE,
    L2VPN_CMD_MODIFY,
    L2VPN_CMD_PWRECV,   /* ip interworking下发recv pw方向表项 */
    L2VPN_CMD_ACRECV,   /* ip interworking下发recv ac方向表项 */
    L2VPN_CMD_MAX
}L2VPN_CMD_E;

/* 以太网服务相关定义 */
typedef struct tagL2VPN_VLAN_RANGE
{
    USHORT   usVlanStart;              /* 起始vlan */
    USHORT   usVlanEnd;                /* 结束vlan */
}L2VPN_VLAN_RANGE_S;

#define L2VPN_VLANSEG_MAXNUM 8         /* VLAN段最大数目 */

/* 匹配规则中的VLAN Tag列表 */
typedef struct tagL2VPN_VLAN_LIST_ENCAP
{
    USHORT             usTPID;
    USHORT             usDot1p;
    USHORT             usRangeNum;         /* 表示下面指针数组的大小 */
    L2VPN_VLAN_RANGE_S  *pstVlanRangeList; /* VLAN段指针数组 */
}L2VPN_VLAN_LIST_ENCAP_S;

/* 单Tag匹配规则 */
typedef struct tagL2VPN_VLAN_TAG_ENCAP
{
    USHORT             usTPID;
    USHORT             usVlan;
    USHORT             usDot1p;
}L2VPN_VLAN_TAG_ENCAP_S;

/* 接口服务实例封装类型 */
#define L2VPN_ENCAP_NONE            0x0     /* 未配置封装类型 */
#define L2VPN_ENCAP_UNTAGGED        0x01    /* 无标签封装类型*/
#define L2VPN_ENCAP_CTAG            0x02    /* 带一层C标签  */
#define L2VPN_ENCAP_CTAG_LIST       0x03    /* 带一层C标签列表  */
#define L2VPN_ENCAP_STAG            0x04    /* 带一层S标签或没有标签（PVID为S标签时）  */
#define L2VPN_ENCAP_STAG_LIST       0x05    /* 带一层S标签列表或没有标签（PVID为在S标签列表中时）  */
#define L2VPN_ENCAP_STAG_CTAG       0x06    /* 带二层单个标签  */
#define L2VPN_ENCAP_STAG_CTAGLIST   0x07    /* 带二层标签，外层单个，内层列表 */
#define L2VPN_ENCAP_STAG_CTAGALL    0x08    /* 带二层标签，外层单个，内层ALL */
#define L2VPN_ENCAP_DEFAULT         0x09    /* 当无法匹配其它条件时,默认匹配 */
#define L2VPN_ENCAP_TAGGED          0x0A    /* 匹配所有带tag报文 */
#define L2VPN_ENCAP_STAG_ONLY_TAGGED  0x0B  /* 带一层S标签 */
#define L2VPN_ENCAP_STAG_LIST_ONLY_TAGGED  0x0C/* 带一层S标签列表  */

/* 以太网接口匹配规则 */
typedef struct tagL2VPN_EVC_ENCAP
{
    UINT                       uiTagEncapType;
    L2VPN_VLAN_LIST_ENCAP_S    stSTagList;
    L2VPN_VLAN_LIST_ENCAP_S    stCTagList;
}L2VPN_EVC_ENCAP_S;

/* 标签操作类型 */
#define L2VPN_REWRITE_UNKNOWN   0x0         /* 无效Rewrite动作 */
#define L2VPN_REWRITE_NEST_S    0x01        /* 插入S标签  */
#define L2VPN_REWRITE_NEST_C    0x02        /* 插入C标签  */
#define L2VPN_REWRITE_NEST_SC   0x03        /* 插入SC标签  */
#define L2VPN_REWRITE_STRIP_S   0x04        /* 剥离S标签  */
#define L2VPN_REWRITE_STRIP_C   0x05        /* 剥离C标签  */
#define L2VPN_REWRITE_STRIP_SC  0x06        /* 剥离SC标签  */
#define L2VPN_REWRITE_REMARK_1TO1  0x07     /* 1:1改写  */
#define L2VPN_REWRITE_REMARK_1TO2  0x08     /* 1:2改写  */
#define L2VPN_REWRITE_REMARK_2TO1  0x09     /* 2:1改写  */
#define L2VPN_REWRITE_REMARK_2TO2  0x0a     /* 2:2改写  */

/* 单个方向的以太网报文VLAN Tag改写规则 */
typedef struct tagL2VPN_VLAN_REWRITE
{
    UINT                      uiRewriteType;
    L2VPN_VLAN_TAG_ENCAP_S    stSTag;
    L2VPN_VLAN_TAG_ENCAP_S    stCTag;
}L2VPN_VLAN_REWRITE_S;

/* 改写动作的方向 */
#define L2VPN_REWRITE_INBOUND     0x0001  /* 入方向 */
#define L2VPN_REWRITE_OUTBOUND    0x0002  /* 出方向 */

/* 以太网报文改写规则 */
typedef struct tagL2VPN_EVC_REWRITE
{
    UINT     uiDirection;
    BOOL_T   bSymmetric;
    L2VPN_VLAN_REWRITE_S astRewriteEntry[2];
}L2VPN_EVC_REWRITE_S;

/* 以太网服务信息 */
typedef struct tagL2VPN_EVC
{
    L2VPN_EVC_ENCAP_S       stEncap;
    L2VPN_EVC_REWRITE_S     *pstRewrite;
}L2VPN_EVC_S;

/* MAC表项出接口类型 */
typedef enum tagL2VPN_MAC_TYPE
{
    L2VPN_MAC_LOCAL = 0,     /* 本地MAC */
    L2VPN_MAC_MPLSPW,        /* MPLS PW远端MAC */
    L2VPN_MAC_MINMPW,        /* MINM 远端MAC */
    L2VPN_MAC_VXLANTUNNEL,   /* VXLAN隧道 远端MAC */
    L2VPN_MAC_NVGRETUNNEL,   /* NVGRE隧道 远端MAC */
    L2VPN_MAC_ALL,           /* 所有类型MAC */
}L2VPN_MAC_TYPE_E;

/* MAC表项的查询时携带的条件 */
typedef struct tagL2VPN_MAC_CONDITION
{
    UINT         uiDisplayOpt;
    UINT         uiVsiIndex;      /* VPLS实例索引，缺省为显示所有VPLS实例的MAC */
    UINT         uiStatus;
    UCHAR        aucMacAddress[6];   /* 单播或组播MAC地址 */
    IF_INDEX     ifIndex;           /* 指定接口索引 */
    USHORT       usVlanID;          /* 表项的VLAN ID */
    UINT         uiLinkID;          /* 指定link ID */
    L2VPN_MAC_TYPE_E enMacEntryType;
}L2VPN_MAC_CONDITION_S;

#define L2VPN_MAC_MAXNUM 25
/* L2VPN MAC表项信息 */
typedef struct tagL2VPN_MAC_ENTRY
{
    L2VPN_MAC_TYPE_E enMacEntryType;
    UCHAR      aucMacAddress[6];     /* 单播MAC地址 */
    union
    {
        UINT       uiLinkID;        /* Link ID，指向PW或者AC */
        IF_INDEX   ifIndexTunnel;   /* Tunnel口索引 */
    }unEntryID;
    UINT       uiVsiIndex;          /* VPLS实例索引 */
    UINT       uiStatus;            /* 表项的状态 */
    UINT       uiAgingTime;         /* 老化时间 */
}L2VPN_MAC_ENTRY_S;

/* MAC表项变化的操作命令字 */
typedef enum tagL2VPN_MAC_CMD
{
    L2VPN_MAC_ADD,            /* 添加MAC*/
    L2VPN_MAC_DELETE,         /* 删除MAC*/
    L2VPN_MAC_WITHDRAW,       /* MAC地址回收*/
    L2VPN_MAC_CMD_MAX
}L2VPN_MAC_CMD_E;

/*  ATM 信元传送相关定义  */
typedef struct tagL2VPN_CTR
{
    USHORT   usMaxPackCells;
    UINT     uiMaxPackTime;
}L2VPN_CTR_S;

typedef enum tagL2VPN_CEM_CR
{
    L2VPN_CEM_CR_NONE,      /* 不需要时钟恢复 */
    L2VPN_CEM_CR_ACR,       /* ACR时钟恢复方式 */
    L2VPN_CEM_CR_DCR,       /* DCR时钟恢复方式 */
    L2VPN_CEM_CR_MAX
}L2VPN_CEM_CR_E;

/* TDM 电路仿真相关定义 */
typedef struct tagL2VPN_CEM
{
    UINT           uiJitterBufferSize;
    L2VPN_CEM_CR_E enClockRecoverMode;
    USHORT         usPayloadByte;
    UCHAR          ucIdleCode;
}L2VPN_CEM_S;

/* MPLS PW封装类型，即RFC 4446 定义的PW类型 */
typedef enum tagMPLS_PW_ENCAPTYPE
{
    MPLS_PW_ENCAP_UNKNOWN,
    MPLS_PW_ENCAP_FR_DLCI_MARTINI = 0x0001,
    MPLS_PW_ENCAP_ATM_AAL5_SDU_VCC,
    MPLS_PW_ENCAP_ATM_CELL_TRANSPORT,
    MPLS_PW_ENCAP_VLAN,
    MPLS_PW_ENCAP_ETHERNET,
    MPLS_PW_ENCAP_HDLC,
    MPLS_PW_ENCAP_PPP = 0x0007,
    MPLS_PW_ENCAP_CESOM,
    MPLS_PW_ENCAP_ATM_CELL_VCC = 0x0009,
    MPLS_PW_ENCAP_ATM_CELL_VPC,
    MPLS_PW_ENCAP_IP_INTERWORKING,
    MPLS_PW_ENCAP_ATM_CELL_1TO1_VCC,
    MPLS_PW_ENCAP_ATM_CELL_1TO1_VPC,
    MPLS_PW_ENCAP_ATM_AAL5_PDU_VCC,
    MPLS_PW_ENCAP_FR_PORT = 0x000f,
    MPLS_PW_ENCAP_CEP,
    MPLS_PW_ENCAP_TDM_SATOP_E1,
    MPLS_PW_ENCAP_TDM_SATOP_T1,
    MPLS_PW_ENCAP_TDM_SATOP_E3,
    MPLS_PW_ENCAP_TDM_SATOP_T3,
    MPLS_PW_ENCAP_TDM_CESOPSN_BASIC,
    MPLS_PW_ENCAP_TDMoIP_AAL1_Mode,
    MPLS_PW_ENCAP_TDM_CESOPSN_WITH_CAS = 0x0017,
    MPLS_PW_ENCAP_TDMoIP_AAL2_Mode,
    MPLS_PW_ENCAP_FR_DLCI = 0x0019,
    MPLS_PW_ENCAP_MAX
}MPLS_PW_ENCAPTYPE_E;

#define    L2VPN_SERVICE_INVALID     0x0       /* 无效值 */
#define    L2VPN_SERVICE_STA         0x01      /* 转发统计 */
#define    L2VPN_SERVICE_QOS         0x02      /* QOS */

/* MPLS PW上的特性（转发流量统计、入网络方向CAR、出网络方向CAR等） */
typedef struct tagMPLS_PW_SERVICE
{
    UINT        uiServiceFlag;     /* 按bit设置，不同业务设置不同位置的bit */
    USHORT      usInCarID;         /* 入网络侧方向CAR ID */
    USHORT      usOutCarID;        /* 出网络侧方向CAR ID */
}MPLS_PW_SERVICE_S;

/* PW属性标记定义 */
#define  L2VPN_PW_NORMAL           0x00000000    /* 普通PW */
#define  L2VPN_PW_BACKUP           0x00000001    /* 备份PW */
#define  L2VPN_PW_UPW              0x00000002    /* 用户侧PW */
#define  L2VPN_PW_HUBPW            0x00000004    /* HUB侧PW */
#define  L2VPN_PW_SPOKEPW          0x00000008    /* SPOKE侧PW */
#define  L2VPN_PW_CTRLW            0x00000010    /* PW支持控制字 */
#define  L2VPN_PW_SIGNAL           0x00000020    /* 电路仿真的信令分组传送PW */
#define  L2VPN_PW_FRAGMENT         0x00000040    /* 电路仿真CESoPSN的PW支持分片属性 */
#define  L2VPN_PW_CCC              0x00000080    /* 远程CCC PW */
#define  L2VPN_PW_DUALRECEIVE      0x00000100    /* 主备pw支持双收功能 */
#define  L2VPN_PW_SEQUENCING_SEND  0x00000200    /* 报文发送方向排序处理 */
#define  L2VPN_PW_SEQUENCING_RECV  0x00000400    /* 报文接收方向排序处理 */
#define  L2VPN_PW_RTP_HEADER       0x00000800    /* PW上发送的电路仿真报文携带RTP头 */
#define  L2VPN_PW_RTP_DIFFMODE     0x00001000    /* PW上发送的电路仿真报文的RTP头的时间戳为差分模式 */
#define  L2VPN_PW_STAT             0x00002000    /* 使能统计，主PW设置 */
#define  L2VPN_PW_BYPASS           0x00004000    /* BYPASS PW */

typedef struct tagL2VFIB_SPEC
{
    UINT   uiVplsVsiNumber;      /* VPLS实例个数 */
    UINT   uiVpwsVsiNumber;      /* VPWS实例个数 */
    UCHAR  ucAcDrvContextLen;
    UCHAR  ucPwDrvContextLen;
    UCHAR  ucCfgIfContextLen;
    UCHAR  ucCfgVsiContextLen;
    UINT   uiAcNumPerVsi;        /* 每VSI的AC规格 */
    UINT   uiPwNumPerVsi;        /* 每VSI的PW规格 */
    BOOL_T bSupportSPB;          /* 是否支持SPB */
    BOOL_T bSupportPBB;          /* 是否支持PBB */
    BOOL_T bSharedBMAC;          /* VSI是否共享BMAC */
    UINT   uiUCMinMNumber;       /* 整机的单播MinM表项规格 */
    BOOL_T bSupportVxlan;        /* 是否支持VXLAN */
    BOOL_T bSupportNvgre;        /* 是否支持NVGRE */
    UCHAR  ucReflectorDrvContextLen;
}L2VFIB_SPEC_S;

/*uiFlag表项标记*/
#define MINM_HEADDUPLICATE  0x0001   /* 头端复制 */
#define MINM_COREDUPLICATE  0x0002   /* 核心复制 */
#define MINM_PBB            0x0004   /* PBB */
#define MINM_SPB            0x0008   /* SPB */
#define MINM_UNICAST        0x0010   /* 单播表项 */
#define MINM_MULTICAST      0x0020   /* 组播表项 */
#define MINM_DOUBLEBVLAN    0x0040   /* 双BVLAN模式 */

#pragma pack(4)
typedef struct tagMinM_Port
{
    IF_INDEX    ifIndex;                                        /* 接口索引 */
    UINT32      auiIfContext[L2VPN_MINMIF_DRVCONTEXT_LENGTH];   /* 接口DrvContext信息 */
}MINM_PORT_S;
#pragma pack()

#define MINM_AGE            1
#define MINM_CANCEL_AGE     2

/* MinM老化表项 */
typedef struct tagMINM_ENTRYINDEX
{
    UCHAR       aucDBMac[6];
    USHORT      usBVlan;
}MINM_ENTRYINDEX_S;

#ifdef __KERNEL__
/* 单条MPLS PW表项参数 */
typedef struct tagMPLS_PW_ENTRY
{
    MPLS_PW_ENCAPTYPE_E  enPWEncapType;  /* PW的封装类型 */
    UINT        uiInVCLabel;        /* 入标签 */
    UINT        uiOutVCLabel;       /* 出标签 */
    UINT        uiPWFlag;           /* PW属性标记，参见L2VPN_PW_ */
    IF_INDEX    ifLogicIndex;       /* PW逻辑接口索引 */
    USHORT      usRmtVTag;          /* 对端peer的P-Tag */
}MPLS_PW_ENTRY_S;

/* PW的出方向类型 */
#define MPLS_PW_OUT_NO      0x00   /* VPLS的PW，不关心usOutLinkID */
#define MPLS_PW_OUT_AC      0x01   /* AC->PW交换，需要关心usOutLinkID */
#define MPLS_PW_OUT_PW      0x02   /* PW->PW交换，需要关心usOutLinkID */
#define MPLS_PW_OUT_BYPASS  0x03   /* BYPASS PW PW<->PW 交换，不关心usOutLinkID */

/* MPLS PW信息 */
typedef struct tagMPLS_PW
{
    MPLS_PW_ENTRY_S       *pstPWMainEntry;   /* 配置的主用PW */
    MPLS_PW_ENTRY_S       *pstPWBkupEntry;   /* 配置的备份PW */
    UINT                  uiVsiIndex;        /* VSI索引 */
    USHORT                usLinkID;          /* PW 的link ID，指向PW */
    USHORT                usMtu;             /* MTU */
    ULONG                 ulVnHandle;        /* VN表项的Handle */
    UINT32                auiVnDrvContext[VN_DRVCONTEXT_LEN]; /* 下一跳信息*/

    MPLS_PW_SERVICE_S     stPwService;
    USHORT                usOutType;
    USHORT                usOutLinkID;
    UINT32                auiContext[L2VPN_DRVCONTEXT_MAXLENGTH];
}MPLS_PW_S;

/* MPLS PW RECV信息 */
typedef struct tagMPLS_PW_RECV
{
    UINT                  uiVsiIndex;        /* VSI索引 */
    USHORT                usLinkID;          /* PW 的link ID，指向PW */
    UINT                  uiInLabel;         /* PW 的入方向标签 */
    L3_ADJACENT_LINK_S    stL3ADJLink;       /* AC侧链路层封装信息 */
}MPLS_PW_RECV_S;

/* L2VPN AC RECV表项信息 */
typedef struct tagL2VPN_AC_RECV
{
    UINT                uiVsiIndex;        /* VSI索引 */
    IF_INDEX            ifIndex;           /* 本端AC对应的接口索引 */
    USHORT              usLinkID;          /* AC对应的Link ID */
    USHORT              usServiceID;       /* AC对应的Service ID */
    L3_ADJACENT_LINK_S  stL3ADJLink;       /* AC侧链路层封装信息 */
}L2VPN_AC_RECV_S;

/* 驱动返回是否同步标记定义 */
#define L2VPN_DRVFLAG_NOSYNC    0x00000000
#define L2VPN_DRVFLAG_SYNC      0x00000001

/* L2VPN_IOCTL_SYNC_START/END时标记所有VSI */
#define L2VPN_SYNC_ALL_VSI      0xFFFFFFFF

/* VSI类型标记，获取指定类型VSI的MAC表项 */
#define L2VPN_VSI_TYPEFLAG_ALL     0x0     /* 所有类型 */
#define L2VPN_VSI_TYPEFLAG_LOCAL   0x1     /* 本地MAC标记  */
#define L2VPN_VSI_TYPEFLAG_REMOTE  0x2     /* 远程MAC标记 */
#define L2VPN_VSI_TYPEFLAG_VXLAN   0x4     /* VXLAN标记位 */
#define L2VPN_VSI_TYPEFLAG_VPLS    0x8     /* VPLS标记位  */
#define L2VPN_VSI_TYPEFLAG_MINM    0x10    /* MINM标记位  */
#define L2VPN_VSI_TYPEFLAG_NVGRE   0x20    /* NVGRE标记位  */
/* 流量统计对象:PW */
typedef struct tagL2VPN_STATISTICS_OBJECT_PW
{
    MPLS_PW_ENCAPTYPE_E  enPWEncapType;  /* PW的封装类型 */
    UINT                 uiVsiIndex;     /* PW所属的VSI Index */
    USHORT               usLinkID;       /* PW的LINK ID */
    UINT                 uiPWFlag;       /* PW属性标记 */
    UINT32               auiContext[L2VPN_DRVCONTEXT_MAXLENGTH]; /* PW的DrvContext */
}L2VPN_STATISTICS_OBJECT_PW_S;

/* ARP/ND报文上送CPU类型 */
typedef enum tagL2VPN_FORWARD_CTL
{
    L2VPN_ARP_CONTROL,                  /* 设置ARP报文控制行为。基于VSI设置 */
    L2VPN_ND_CONTROL,                   /* 设置ND报文控制行为。基于VSI设置 */
    L2VPN_FORWARD_BUTT
}L2VPN_FORWARD_CTL_E;

typedef enum tagL2VPN_ACTION
{
    L2VPN_ACTION_FORWARD,               /* 正常转发报文，这是缺省状态 */
    L2VPN_ACTION_REDIRECT_TO_CPU,       /* 将报文重定向到CPU */
    L2VPN_ACTION_BUTT
}L2VPN_ACTION_E;

/* 驱动接口函数指针 */
typedef struct tagL2VPN_DRV_INTF_S
{
    ULONG (* DRV_L2VPN_Ioctl) (
        IN L2VPN_IOCTL_E    enIoctl,
        IN VOID             *pData);
    ULONG ( * DRV_L2VPN_HandleAcChange) (
        IN L2VPN_CMD_E     enAcCmd,
        IN VOID           *pstData,
        IN VOID           *pstOldData,
        OUT ULONG         *pulSynFlag);
    ULONG ( *DRV_L2VPN_HandleMacChange ) (
        IN L2VPN_MAC_CMD_E      enCmd,
        IN L2VPN_MAC_ENTRY_S    *pstMacEntry);
    ULONG ( *DRV_L2VPN_GetMacEntry ) (
        IN L2VPN_MAC_CONDITION_S    *pstCond,
        INOUT  UINT *puiCount,
        INOUT  UINT auiDrvContext[L2VPN_DRVCONTEXT_MAXLENGTH],
        OUT L2VPN_MAC_ENTRY_S *pstMacArray);
    ULONG ( *DRV_L2VPN_HandlePwChange ) (
        IN L2VPN_CMD_E enCmd,
        INOUT VOID *pData,
        INOUT VOID *pOldData,
        OUT ULONG *pulSynFlag);
    BOOL_T  (*DRV_L2VPN_MacIsHit)(
            IN UINT uiVsiIndex,
            IN UINT uiLinkID,
            IN UCHAR ucMacAddress[MAC_ADDR_LEN]);
    ULONG (*DRV_L2VPN_SetForwardingOption) (
        IN UINT uiVSI,
        IN UINT uiCmd,
        IN OUT VOID *pParam,
        INOUT UINT32 auiDrvContext[L2VPN_FORWARD_DRVCONTEXT_LENGTH]);
    ULONG ( *DRV_L2VPN_FindMacEntry ) (
        IN UINT  uiVsiIndex,
        IN UCHAR aucMacAddress[MAC_ADDR_LEN],
        OUT L2VPN_MAC_ENTRY_S *pstMacEntry);
    ULONG ( *DRV_L2VPN_GetFirstMacEntry ) (
        IN UINT  uiVsiTypeFlag,
        OUT L2VPN_MAC_ENTRY_S *pstMacEntry);
    ULONG ( *DRV_L2VPN_GetNextMacEntry ) (
        IN UINT  uiVsiTypeFlag,
        IN UINT  uiVsiIndex,
        IN UCHAR aucMacAddress[MAC_ADDR_LEN],
        OUT L2VPN_MAC_ENTRY_S *pstMacEntry);
    ULONG (*DRV_L2VPN_SetFloodingMode) (
        IN BOOL_T bFlooding,
        IN UINT uiVSI,
        INOUT UINT32 auiDrvContext[2] );
    ULONG (*DRV_L2VPN_SetFloodingMac) (
        IN UINT uiOperation,
        IN UINT uiVSI,
        IN UCHAR aucMacAddress[MAC_ADDR_LEN]);
    ULONG ( *DRV_L2VPN_CreateVsiIF) (
        IN IF_INDEX ifIndex,
        INOUT UINT64 aui64DrvContext[2]);
    ULONG ( *DRV_L2VPN_DestoryVsiIF) (
        IN IF_INDEX ifIndex,
        IN UINT64 aui64DrvContext[2]);
    ULONG (* DRV_L2VPN_GetStatistics) (
        IN  L2VPN_STATISTICS_TYPE_E enStatisticsType,
        IN  VOID *pstStatisticsObject,
        OUT L2VPN_STATISTICS_S  *pstStatisticsData);
    ULONG (* DRV_L2VPN_ResetStatistics) (
        IN  L2VPN_STATISTICS_TYPE_E enStatisticsType,
        IN  VOID *pstStatisticsObject);
}L2VPN_DRV_INTF_S, *PL2VPN_DRV_INTF_S;

ULONG L2VFIB_Init(IN LPVOID pStartContext);
ULONG L2VFIB_Run(IN LPVOID pStartContext);

ULONG L2VFW_VPLS_Init(IN LPVOID pStartContext);

/* 用以初始化L2VPN的驱动接口函数 */
ULONG L2VPN_Register_Driver( IN ULONG ulVersion, IN L2VPN_DRV_INTF_S *pstL2VPNDrvIntf);
ULONG L2VFIB_Register_Spec(IN L2VFIB_SPEC_S *pstL2vfibSpec);

/* 注册MPLS PW功能 */
ULONG L2VPN_Register_MplsPW(VOID);
ULONG L2VPN_Register_MplsPWDriver(VOID);

/* 注册Overlay功能 */
ULONG L2VPN_Register_Overlay(VOID);

typedef enum tagL2VPN_ENTRYTYPE
{
    L2VPN_AC,                /* AC表项 */
    L2VPN_MPLSPW,            /* MPLS PW表项 */
    L2VPN_MINMPW,            /* MAC-in-MAC PW表项 */
}L2VPN_ENTRYTYPE_E;

/* 根据VSI索引和LinkID，获取对应的表项信息 */
ULONG L2VPN_GetAcDrvInfo(IN UINT uiVsiIndex, IN USHORT usLinkID, OUT L2VPN_AC_S *pstAcData);
ULONG L2VPN_GetMplsPwDrvInfo(IN UINT uiVsiIndex, IN USHORT usLinkID, OUT MPLS_PW_S *pstPwData);

/* 根据接口索引和服务实例获取AC信息 */
ULONG L2VPN_GetAcDrvByIf(IN IF_INDEX ifIndex, IN USHORT usSrvID, OUT L2VPN_AC_S *pstAcData);

/* L2VPN接入L3VPN特性根据其中1个VE口索引获取另一个编号一致的VE口索引 */
IF_INDEX L2VPN_GetVEIfIndex(IN IF_INDEX ifIndex);

typedef ULONG (*VSI_TUNNEL_VISIT_PF)(IN IF_INDEX ifIndex, IN UINT uiVsiIndex, IN VOID *);

/* Tunnel隧道下一跳变化，产品需要变更Tunnel所属的所有VSI对应的转发信息 */
VOID L2VPN_ScanVsiByTunnel(IN IF_INDEX ifIndexOfTunnel,
                         IN const VSI_TUNNEL_VISIT_PF pfFunVisit,
                         IN VOID *pUserData);

/* 根据VsiIndex获取VsiName */
ULONG L2VPN_GetVsiNameByIndex(IN UINT uiVsiIndex, IN ULONG ulBufLen, OUT CHAR *pcVsiName);

/* 通知L2VPN配置重新下发驱动 */
ULONG L2VPN_ReloadCfg(VOID);
/* 通知VPLS AC表项重新下发驱动 */
ULONG L2VPN_ReloadVplsAc(VOID);
/* 通知VPLS PW表项重新下发驱动 */
ULONG L2VPN_ReloadVplsPw(VOID);
/* 通知VPWS AC表项重新下发驱动 */
ULONG L2VPN_ReloadVpwsAc(VOID);
/* 通知VPWS PW表项重新下发驱动 */
ULONG L2VPN_ReloadVpwsPw(VOID);
/* 通知MINM表项重新下发驱动 */
ULONG L2VPN_ReloadMinM(VOID);
/* 通知OVERLAYTUNNEL表项重新下驱动 */
ULONG L2VPN_ReloadOverlayTunnel(VOID);
/* 通知REFLECT表项重新下发驱动 */
ULONG L2VPN_ReloadReflector(VOID);

/*************************MINM相关数据结构定义***********************************/
typedef enum tagMINM_CMD
{
    MINM_CMD_ADD,              /* 添加表项 */
    MINM_CMD_DEL,              /* 删除表项 */
    MINM_MC_ADDOUTIF,          /* 组播表项增加若干出接口 */
    MINM_MC_DELOUTIF,          /* 组播表项删除若干出接口 */
    MINM_UC_MODIFYOUTIF,       /* 单播表项修改出接口 */
    MINM_UC_MODIFYMODE,        /* SPB单播表项修改组播复制模式 */
    MINM_ECT_ADD,              /* ECT迁移过程中，新增SPB单播表项 */
    MINM_ECT_MODIFY,           /* ECT迁移过程中，修改SPB单播表项出端口属性 */
    MINM_ECT_DEL,              /* ECT迁移过程中，删除SPB单播表项 */
    MINM_CMD_BUTT              /* 无效值 */
}MINM_CMD_E;

typedef struct tagMINM_MC
{
    UINT        uiVsiIndex;                /* VSI索引，对于表项中不关注VSI的产品，产品直接忽略 */
    UINT        uiISID;                    /* 服务实例号 */
    USHORT      usFlag;                    /* 表项标记 */
    UCHAR       aucDBMac[MAC_ADDR_LEN];    /* 骨干网目的MAC，该地址为组播地址 */
    UCHAR       aucSBMac[MAC_ADDR_LEN];    /* 骨干网源BMAC，即本设备的MACtoME地址 */
    UINT        uiBVlanID;       /* 骨干网VLAN ID */
    UINT        uiPortNum;       /* 出端口个数 */
    MINM_PORT_S *pstPortList;     /* 表项的出接口列表，包含对应的DrvContext信息 */
    UINT32      auiContext[L2VPN_MINM_DRVCONTEXT_LENGTH];  /* 驱动上下文。保留字段，产品驱动软件根据需要使用 */
}MINM_MC_S;

typedef struct tagMINM_UCIF
{
    UINT        uiBVlanID;         /* 骨干网VLAN ID */
    IF_INDEX    ifIndex;           /* 接口索引 */
    UINT32      auiIfContext[L2VPN_MINMIF_DRVCONTEXT_LENGTH];   /* 接口DrvContext信息 */
}MINM_UCIF_S;

typedef struct tagMINM_UC
{
    UINT          uiVsiIndex;               /* VSI索引，对于表项中不关注VSI的产品，产品直接忽略 */
    UINT          uiISID;                   /* 服务实例号 */
    USHORT        usLinkID;                 /* MinM表项的索引，在Marvel芯片是VSI内部的索引，并且和VPLS的Link ID在同一空间。BCM芯片无意义 */
    USHORT        usFlag;                   /* 表项标记 */
    UCHAR         aucDBMac[MAC_ADDR_LEN];   /* 骨干网目的MAC */
    UCHAR         aucSBMac[MAC_ADDR_LEN];   /* 骨干网源BMAC，即本设备的MACtoME地址 */
    UCHAR         aucSCMac[MAC_ADDR_LEN];   /* 用户源MAC */
    MINM_UCIF_S   stPort;                   /* 出端口 */
    UINT32        auiContext[L2VPN_MINM_DRVCONTEXT_LENGTH]; /* 驱动上下文。保留字段，产品驱动软件根据需要使用。初始为全F */
}MINM_UC_S;

typedef struct tagMINM_ECT
{
    UINT        uiVsiIndex;               /* VSI索引，对于表项中不关注VSI的产品，产品直接忽略 */
    UINT        uiISID;                   /* 服务实例号*/
    USHORT      usLinkID;                 /* MinM表项的索引，在Marvel芯片是VSI内部的索引，并且和VPLS的Link ID在同一空间。BCM芯片无意义 */
    USHORT      usFlag;                   /* 表项标记 */
    UCHAR       aucDBMac[MAC_ADDR_LEN];   /* 骨干网目的MAC */
    UCHAR       aucSBMac[MAC_ADDR_LEN];   /* 骨干网源BMAC，即本设备的MACtoME地址 */
    MINM_UCIF_S stPort;                   /* 端口能对MINM报文执行收发包操作 */
    MINM_UCIF_S *pstPortRecvOnly;         /* 端口仅能对MINM报文执行收包操作 */
    UINT32      auiContext[L2VPN_MINM_DRVCONTEXT_LENGTH]; /* 驱动上下文。保留字段，产品驱动软件根据需要使用。初始为全F */
}MINM_ECT_S;

typedef struct tag_MINM_DRV_INTF
{
    ULONG (*pfDRV_MINM_HandleMcChange)(IN MINM_CMD_E enCmd, IN MINM_MC_S *pstOldData, INOUT MINM_MC_S *pstData,OUT ULONG *pulSynFlag);
    ULONG (*pfDRV_MINM_HandleUcChange)(IN MINM_CMD_E enCmd, IN MINM_UC_S *pstOldData, INOUT MINM_UC_S *pstData,OUT ULONG *pulSynFlag);
    ULONG (*pfDRV_MINM_HandleEctChange)(IN MINM_CMD_E enCmd, IN MINM_ECT_S *pstOldData, INOUT MINM_ECT_S *pstData,OUT ULONG *pulSynFlag);
    BOOL_T (*pfDRV_MINM_IsSupport)(IN USHORT usSlotId);
}MINM_DRV_INTF_S;

ULONG MINM_Register_Driver(IN ULONG ulVersion, IN MINM_DRV_INTF_S  *pstDrvIntf);
ULONG MINM_NotifyAgedEntry (IN  UINT uiOperType, IN UINT uiVsiIndex, IN  UINT uiIsid,
                            IN  MINM_ENTRYINDEX_S *pstDataItem, IN UINT uiItemNum);

/* 注册CEM */
ULONG L2VPN_Register_Cem(VOID);

#endif

#ifdef __cplusplus
}
#endif /* end of __cplusplus */

#endif
