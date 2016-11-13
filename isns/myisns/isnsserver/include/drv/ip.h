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
/**********************BPA��ʶλ***********************/
#define BPA_MARK_TYPE_NONE        0x0       /* ����ͳ��ȱʡ��Ϣ */
#define BPA_MARK_TYPE_SRC         0x01      /* ���û���Դ������ͳ�� */
#define BPA_MARK_TYPE_DST         0x02      /* ���û���Ŀ�ĵ�����ͳ�� */

/* BPAͳ����Ϣ�ṹ */
typedef struct tagIP_BPAStat
{
    UINT64 uiInPktCnt;            /* �뷽��ı����� */
    UINT64 uiInPktByteCnt;        /* �뷽����ֽ��� */
    UINT64 uiOutPktCnt;           /* ������ı����� */
    UINT64 uiOutPktByteCnt;       /* ��������ֽ��� */
    UINT64 uiFlowIndex;           /* ��������ֵ */
} IP_BPA_STAT_S;

/* FIB4 Ӳ���������� */
typedef struct tagFIB4_SpecConf
{
    UINT uiMaxECMP;     /* FIB4 ���ȼ�·���� */
    UINT uiMaxPublic;   /* FIB4 ���������� */
    UINT uiMaxVrf;      /* FIB4 ˽�������� */
    UINT uiECMPMode;    /* FIB4 �ȼ�·��ģʽ������������*/
} FIB4_SPECCONF_S;

/* FIB6 Ӳ���������� */
typedef struct tagFIB6_SpecConf
{
    UINT uiMaxECMP6;    /* FIB6 ���ȼ�·���� */
    UINT uiMaxPublic6;  /* FIB6 ���������� */
    UINT uiMaxVrf6;     /* FIB6 ˽�������� */
} FIB6_SPECCONF_S;


/**********************FIBǰ׺��VN�·�ʱ�����DrvContext����***********************/
#define FIB_DRVCONTEXT_LEN_MAX     8
#define VN_DRVCONTEXT_LEN          16
#define VN_NEXTHOP_DRVCONTEXT_LEN  4

/***************************IP���طֵ���ʽ����*********************************/
typedef enum tagIP_LoadSharingModeEnum
{
    IP_LOADSHARING_MODE_DEFAULT = 0,        /* ȱʡ�ĸ��طֵ�������Ʒ����ȱʡ��һ��*/
    IP_LOADSHARING_MODE_PACKET,             /* ������طֵ� */
    IP_LOADSHARING_MODE_FLOW,               /* �������طֵ� */
    IP_LOADSHARING_MODE_FLOW_ALGORITHM,     /* �������طֵ��㷨 */
    IP_LOADSHARING_MODE_FLOW_TUNNEL,        /* tunnel hashģʽ */
    IP_LOADSHARING_MODE_BUTT
}IP_LOADSHARING_MODE_E;

/*�������طֵ���ʽλͼ����*/
#define IP_LOADSHARING_OPTION_DEFAULT       0x00
#define IP_LOADSHARING_OPTION_SRCIP         0x01
#define IP_LOADSHARING_OPTION_DSTIP         0x02
#define IP_LOADSHARING_OPTION_PROTOCOL      0x04
#define IP_LOADSHARING_OPTION_SRCPORT       0x08
#define IP_LOADSHARING_OPTION_DSTPORT       0x10
#define IP_LOADSHARING_OPTION_INGRESSPORT   0x20

#define IP_LOADSHARING_LOCALFIRST_SLOT      1  /* ֧�ָ��طֵ��������� */
#define IP_LOADSHARING_LOCALFIRST_CHASSIS   2  /*֧�ָ��طֵ���������*/

/**********************QPPB��ʶλ***********************/
#define QPPB_MARK_TYPE_NONE        0x0          /* QPPBȱʡ��Ϣ */
#define QPPB_MARK_TYPE_IPPRE       0x01         /* ����ip-precedence��Ϣ */
#define QPPB_MARK_TYPE_LOCALID     0x02         /* ����qos-local-id��Ϣ */
/* tunnel hash��ʽ���� */
#define IP_LOADSHARING_OPTION_TUNNEL_DEFAULT 0x00
#define IP_LOADSHARING_OPTION_TUNNEL_OUT     0x01
#define IP_LOADSHARING_OPTION_TUNNEL_IN      0x02
#define IP_LOADSHARING_OPTION_TUNNEL_ALL     0x04
/*���ڼ���IPV4/6���طֵ�ģʽ�µȼ�·��ѡ·��Ϣ-��������*/
#define IP_LOADSHARING_PATH_PKTTYPE_IPV4OE 1
#define IP_LOADSHARING_PATH_PKTTYPE_IPV6OE 2

/*���ڼ���IPV4/V6���طֵ�ģʽ�µȼ�·��ѡ·��Ϣ-����*/
typedef struct tagIP_LoadSharingPathPara
{
    IF_INDEX    ifIndexIngress;/*��ӿ�����*/
    UINT        uiPktType;     /*���ķ�װ����*/
    INET_ADDR_S stDstIp;       /*Ŀ��IP*/
    INET_ADDR_S stSrcIp;       /*ԴIP*/
    UINT        uiProID;       /*Protocol ID*/
    USHORT      usDstPort;     /*Ŀ��L4�˿�*/
    USHORT      usSrcPort;     /*ԴL4�˿�*/
    VRF_INDEX   vrfIndex;      /*L3VPN��VPNʵ������*/
}IP_LOADSHARING_PATH_PARA_S;

/*���ڼ���IPV4/V6���طֵ�ģʽ�µȼ�·��ѡ·��Ϣ-���*/
typedef struct tagIP_LoadSharingPathResult
{
    INET_ADDR_S stNextHop;     /*��������ѡ��·����һ��*/
    IF_INDEX    ifIndexEgress; /*��������ѡ��·�ɳ��ӿ�*/
}IP_LOADSHARING_PATH_RESULT_S;

#ifdef __KERNEL__

#ifndef _DRV_ADJ_H_
#include <drv/adj.h>
#endif

#ifndef _MPLS_NHLFE_H_
#include <drv/mpls_nhlfe.h>
#endif

/*******************�������صı�־******************************/
/* VN��ˢ���Ƿ���Ҫͬ���궨�� */
#define L3_VN_RELOAD_NOSYN   0      /* ����Ҫͬ�� */
#define L3_VN_RELOAD_SYN     1      /* ��Ҫͬ�� */

/* ��ˢǰ׺���Ƿ���Ҫͬ���궨�� */
#define L3_ROUTE_RELOAD_NOSYN  0    /* ����Ҫͬ�� */
#define L3_ROUTE_RELOAD_SYN    1    /* ��Ҫͬ�� */


/**************************·�������ӿ�����***************************/
/*****************************·�����Զ���***************************/
/* ���ӿ�������ԣ���16λ */
#define L3_ROUTE_BLACKHOLE   0x00000001   /* �ڶ�·�ɣ��������ģ������������� */
#define L3_ROUTE_CPU         0x00000002   /* �����Ľ���������� */
#define L3_ROUTE_LOOPBACK    0x00000004   /* ���ͱ���Э��ջ���� */
#define L3_ROUTE_TUNNEL      0x00000008   /* ���ӿ�ΪTUNNEL�ӿ� */
#define L3_ROUTE_NORMAL      0x00008000   /* �������������������ת��·�ɣ�����������ֵ�� */

/* ǰ׺������ԣ���16λ*/
#define L3_ROUTE_DIRECT      0x00010000   /* ֱ��·�� */
#define L3_ROUTE_INDIRECT    0x00020000   /* ��ֱ��·�� */
#define L3_ROUTE_ECMP        0x00040000   /* ECMP·�� */
#define L3_ROUTE_FTN         0x00080000   /* FTN����ǰ׺���б�ǩ */
#define L3_ROUTE_BGP         0x00100000   /* BGP·�� */
#define L3_ROUTE_TUNNEL_END  0x00200000   /* TUNNEL����ս� */
#define L3_ROUTE_NAT         0x00400000   /* NAT���ɵ�·�� */
#define L3_ROUTE_ARP         0x00800000   /* ARP����·�� */
#define L3_ROUTE_ND          L3_ROUTE_ARP /* ND����·�� */
#define L3_ROUTE_BROADCAST   0x01000000   /* �㲥·�� */
#define L3_ROUTE_FTNSTAT     0x02000000   /* FTNͳ��ʹ�� */
#define L3_ROUTE_ALL         0x1FFFF000   /*����FIBǰ׺���ԣ���������������ֵ*/

/* ǰ׺MPLS������ԣ���4λ */
#define L3_ROUTE_MPLS_NORMAL 0x00000001   /* ��ͨTransit LSP�����CRLSP֮���BGP LSP��LDP LSP����̬LSP */
#define L3_ROUTE_MPLS_CRLSP  0x00000002   /* Transit CRLSP���� */
#define L3_ROUTE_MPLS_VLL    0x00000004   /* VPWS PW���� */
#define L3_ROUTE_MPLS_VPLS   0x00000008   /* VPLS PW���� */
#define L3_ROUTE_MPLS_ALL    0x0000000F   /* ����MPLS�����������������ֵ*/

/************************************************************
  ����ģ�鶨��, ��·���������ָ����Ҫ��ˢ��·��
************************************************************/
#define L3_ROUTE_MIDROUTE    0x40000000  /*FIB��VRFFIBģ��*/
#define L3_ROUTE_MIDMPLS     0x80000000  /*MPLSģ��*/
#define L3_ROUTE_MIDTUNNEL   0x20000000  /*���ģ��*/

/*****************************��һ������******************************************/
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

/*****************************��һ����״̬******************************************/
#define L3_NEXTHOP_STATUS_ACTIVE      0x01

#define IP_INVALID_PRECE     0xFFFF                /* ��Ч��ip���ȼ� */
#define IP_INVALID_LOCALID   0xFFFF                /* ��Ч��QOS local id */
#define IP_INVALID_FLOWINDEX ((UINT64)-1)          /* ��Ч��BGP�������� */
/******************************�ӿ�Э��mtu������������******************************/
#define IOCTL_COMMON_SET_IP_MTU                  IF_IOCTL_CMD_DEF(MID_IPFW, IF_COMMON_TYPE, 0)
#define IOCTL_COMMON_SET_IP6_MTU                 IF_IOCTL_CMD_DEF(MID_IP6FW, IF_COMMON_TYPE, 0)

/******************************QPPB������������******************************/
#define IOCTL_COMMON_SET_IP_QPPB                 IF_IOCTL_CMD_DEF(MID_IPFW, IF_COMMON_TYPE, 1)

typedef struct tagQPPBPolicy
{
  USHORT usMarkSIPType;
  USHORT usMarkDIPType;
} QPPB_POLICY_S;

#define IP_PROTOENDRVCONTEXTLEN 2

/******************************BPA������������******************************/
#define IOCTL_COMMON_SET_IP_BPA              IF_IOCTL_CMD_DEF(MID_IPFW, IF_COMMON_TYPE, 2)

/* �����������ṹ */
typedef struct tagIPBPA
{
  USHORT usMarkInputType;           /* �뷽��ͳ�� */
  USHORT usMarkOutputType;          /* ������ͳ�� */
} IP_BPA_S;

/********************************VN�·�������������*********************************/
typedef enum tagL3VnOpType
{
    L3_VN_ADD,        /* ����һ������ */
    L3_VN_MOD,        /* �޸�һ������ */
    L3_VN_DEL,        /* ɾ��һ������ */
    L3_VN_BUTT
} L3_VN_OPTYPE_E;

/**************************·���·�������������***************************/
typedef enum tagL3RouteOpType
{
    L3_ROUTE_ADD,        /* ����һ������ */
    L3_ROUTE_MODIFY,     /* �޸�һ������ */
    L3_ROUTE_DEL,        /* ɾ��һ������ */
    L3_ROUTE_BUTT
 } L3_ROUTE_OPTYPE_E;

/**************************ת���ض����ͱ���ö�ٶ���***************************/
typedef enum tagIPFORWARD_OPTION
{
    IPFORWARD_UNREACHABLE,                 /*�����ַ���ɴﱨ��*/
    IPFORWARD_TTLEXPIRE,                   /*TTLΪ1�ı���*/
    IPFORWARD_ICMPREDIRECT,                /*ICMP�ض�����*/
    IPFORWARD_ICMP_FRAG_FORWARDING,        /*�����Ƿ�ת��ICMP��Ƭ���ĵĹ���*/
    IPFORWARD_LOADSHARING_MODE,            /*���ò�ƷӲ������ת���ĸ��طֵ���ʽ*/
    IPFORWARD_DIRECTBROADCAST,             /*ֱ���㲥����*/
    IPFORWARD_IF_DIRECTBROADCAST,          /*���ڽӿڵ�ֱ���㲥����*/
    IPFORWARD_ARPSRCMAC_CHECK,             /* ����ARP����ԴMACһ���Լ�� */
    IPFORWARD_COMPATIBLE_IPV4_FORWARDING,  /* �����Ƿ�ת��IPv4����IPv6��ַ��ʽ�ı��� */
    IPFORWARD_MC_NONRPF,                   /* ���ö�RPF���ʧ�ܵ������鲥���ĵĴ���ʽ */
    IPFORWARD_SETMAXECMP,                  /*�������ECMP����*/
    IPFORWARD_SET_ECMP_MODE,               /* ����ECMP ģʽ */
    IPFORWARD_SET_OPTION_DROP,             /* ����ipv6 option dropʹ�ܱ�� */  
    IPFORWARD_SET_LOCALFIRST,               /* ���ñ������� */
    IPFORWARD_OPTION_BUTT
} IPFORWARD_OPTION_E;

/**************************ת�����Ĵ������ͺ궨��***************************/
#define IPFORWARD_TOCPU         1   /* ����CPU */
#define IPFORWARD_NOTTOCPU      2   /* ������CPU */
#define IPFORWARD_BROADCAST     3   /* �㲥 */
#define IPFORWARD_NOTBROADCAST  4   /* ���㲥 */
#define IPFORWARD_BRIDGING      5   /* ���ж���ת�� */
#define IPFORWARD_NOTBRIDGING   6   /* �����ж���ת�� */

typedef struct tagIPLoadSharingConf
{
    UINT uiLocalFirstSupport;       /* �Ƿ�֧�ָ��طֵ�������������,ENABLE��ʾ֧��,DISABLE��ʾ��֧�� */
    UINT uiLocalFirstType;          /* ���طֵ�������������,����or�������� */
    UINT uiLocalFirstDefault;       /*���طֵ���������ȱʡֵ��ENABLE��ʾ����,DISABLE��ʾ�ر� */
}IP_LOADSHARING_CONF_S;

/* �趨RPF���ʧ�ܵ������鲥���ĵĴ���ʽ�����Ƿ�����CPU������VLAN�ڹ㲥�� */
typedef struct  tagIPMCNONRPF_PARAM
{
    IF_INDEX ifIndex;     /* �ӿ����� */
    USHORT usType;        /* ����ʽ */
    USHORT usVlanID;      /* ifIndex��ӦVLAN��ӿڵ�VLAN������*/
}IPMCNONRPF_PARAM_S;

/*�������ø��طֵ�ģʽ*/
typedef struct tagIP_LoadSharingMode
{
    ULONG  ulType;            /*�����������*/
    UINT64 uiOptionBitMap;   /*����ʱ,��Щ�����������븺�طֵ��ľ���*/
}IP_LOADSHARING_MODE_S;

/*****************************ת�������·���ض���********************************/
/* VN��һ����Ϣ */
typedef struct tagL3Nexthop
{
    UCHAR        ucPrefixIndex;               /* ����һ��·���Ե�Ӧǰ׺����ı�� */
    UCHAR        ucStatus;                    /* ��һ��״̬ */
    USHORT       usFlag;                      /* ��һ������ */
    IF_INDEX     ifIndex;                     /* ���ӿ����� */
    INET_ADDR_S  stNexthopIp;                 /* ��һ����ַ */
    UINT32       uiNhRouteStatus;             /* ��һ����ַת������ */
    INET_ADDR_S  stLastAddr;                  /* BGP�ڽ�����һ�� */
    UINT32       uiDataMTU;
    UINT32       uiWeight;                    /* Ȩ�� */
    UINT64       auiTunnelInfo[2];
    union                                   /* ����uiFlag���Ƿ���L3_VN_MPLS������װ���� */
    {
        L3_ADJACENT_LINK_S   stL3ADJLink;   /* ��·���װ��Ϣ */
        MPLS_NHLFE_FWDINFO_S stNhlfeInfo;   /* MPLS��װ��Ϣ */
    }unNexthop;
    #define nh_stL3ADJLink unNexthop.stL3ADJLink
    #define nh_stNhlfeInfo unNexthop.stNhlfeInfo
    UINT32     auiContext[VN_NEXTHOP_DRVCONTEXT_LEN];  /* ����������Ϣ */
} L3_NEXTHOP_S;

/* VN�������ṹ */
typedef struct tagL3Vn
{
    USHORT usVnFlag;                            /* Vn���ԣ�������һ�������Ժϼ� */
    USHORT usVnNexthopNum;                      /* ��һ������ */
    UINT32 auiVnDrvContext[VN_DRVCONTEXT_LEN];  /* VN DrvContext */
    L3_NEXTHOP_S astL3Nexthop[1];               /* ��һ����Ϣ */
} L3_VN_S;


/* �������·�IPv4 ROUTE��������ݽṹ */
typedef struct tagL3Route
{
    INADDR_S stDstIp;
    INADDR_S stDstMask;
    VRF_INDEX vrfIndex;
    BOOL_T bIsVnDownload;
    USHORT usIpPrecedence;         /* QPPB�������õ�IP���ȼ� */
    USHORT usQosLocalId;           /* QPPB�������õ�Qos-local-idֵ */
    USHORT usFlowIndex;            /* BGP���������� */
    ULONG  ulVnHandle;
    UINT32 uiRouteStatus;
    UINT32 uiOriginAs;
    UINT32 uiLastAs;               /* BGP�ڽ������� */
    UINT32 uiLabel;                /* ��ǩ  */
    UINT32 uiLabelBackup;          /* ���ݱ�ǩ  */
    L3_NEXTHOP_S stNexthop;
    UINT32 auiVnDrvContext[VN_DRVCONTEXT_LEN];   /* VN�����·�������VnDrvContext */
    UINT32 auiContext[FIB_DRVCONTEXT_LEN_MAX];   /*���ڻ���*/
    UINT32 uiFIBSeq;               /* FPGA: FIB��� */
}L3_ROUTE_S;

/* �������·�IPv6 ROUTE��������ݽṹ */
typedef struct tagL3Route6
{
    IN6ADDR_S stDstIp;
    UCHAR     ucDstPrefixLen;
    VRF_INDEX vrfIndex;
    BOOL_T bIsVnDownload;
    USHORT usIpPrecedence;         /* QPPB�������õ�IP���ȼ� */
    USHORT usQosLocalId;           /* QPPB�������õ�Qos-local-idֵ */
    USHORT usFlowIndex;            /* BGP���������� */
    ULONG  ulVnHandle;
    UINT32 uiRouteStatus;
    UINT32 uiOriginAs;
    UINT32 uiLastAs;               /* BGP�ڽ������� */
    UINT32 uiLabel;                /* ��ǩ  */
    UINT32 uiLabelBackup;          /* ���ݱ�ǩ  */
    L3_NEXTHOP_S stNexthop;
    UINT32 auiVnDrvContext[VN_DRVCONTEXT_LEN];   /* VN�����·�������VnDrvContext */
    UINT32 auiContext[FIB_DRVCONTEXT_LEN_MAX];   /*���ڻ���*/
    UINT32 uiFIBSeq;               /* FPGA: FIB��� */
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

/* ��ȡ/���IPͳ����Ϣ�����ӿڣ�оƬֻ֧��ȫ��ͳ����Ϣ����ʱ�ṩipv4ͳ�� */
typedef struct tagIPStatDrvIntf
{
    VOID (*pfDRV_IPv4_GetStat)(IN BOOL_T bGlobalStat, IN IF_INDEX ifIndex, OUT VOID* pParam);
    VOID (*pfDRV_IPv4_ResetStat)(IN BOOL_T bGlobalStat, IN IF_INDEX ifIndex);
}IP_STAT_DRV_INTF_S, *PIP_STAT_DRV_INTF_S;

/* ��ȡBPAͳ����Ϣ�����ӿ� */
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

/* IP�����ӿ�ע�� */
extern ULONG IP_Install_Drv(IN LPVOID pStartContext);



/*****************************��Ʒ���Ƽ���ʼ��******************************************/
/*******************************************************************/
/*                            �궨��                               */
/*******************************************************************/
#define FIB4_HASH_LEVELNUM_MAX         7    /* ����FIB4 HASH������� */


/*******************************************************************/
/*                            ���ݽṹ                             */
/*******************************************************************/

/* FIB�洢��ʽ���� */
typedef enum tagFIB4_StorageEnum
{
    FIB4_STORAGE_HASH_1,   /* Hash��ʽ20-4-4-4(4��)��ռ���ڴ���Զ�һ�㣬��ѯЧ����Ը�һЩ */
    FIB4_STORAGE_HASH_2,   /* Hash��ʽ16-8-8(3��)��ռ���ڴ���٣���ѯЧ����Ե�һЩ(����HASH_1��RADIX֮��) */
    FIB4_STORAGE_RADIX,    /* Radix��ʽ��ռ���ڴ��٣���ѯЧ�ʵ���Hash��ʽ */
    FIB4_STORAGE_BUTT
}FIB4_STORAGE_E;

typedef struct tagFIB4_Conf
{
    FIB4_STORAGE_E enPublic;    /* ����FIB�洢��ʽ���� */
    UINT uiDrvContextLen;       /* FIB4ǰ׺DrvContext��������L3_ROUTE_S�е�DrvContext��Ӧ����Ĭ��ֵ�������·��������Զ���Ϊ0 */
    UINT uiVNDrvContextLen;     /* VN DrvContext��������L3_VN_S�е�DrvContext��Ӧ����Ĭ��ֵ�������·��������Զ���Ϊ0 */
    FIB4_SPECCONF_S stSpecConf; /* FIB4 Ӳ���������� */
} FIB4_CONF_S;

typedef struct tagFIB6_Conf
{
    UINT uiDrvContextLen;       /* FIB6ǰ׺DrvContext��������L3_ROUTE6_S�е�DrvContext��Ӧ����Ĭ��ֵ�������·��������Զ���Ϊ0 */
    FIB6_SPECCONF_S stSpecConf; /* FIB6 Ӳ���������� */
} FIB6_CONF_S;

/*
 * J03845: Ϊ��֤�����ӿڷ�װ�ԣ���֧�ֿ���չ��IPv4��ص������ӿڷ�װ��EOM��ʽ��
 * ��ģ����ʹ�������ӿ�ʱ����EOM��ȡ��Ӧ�Ľӿڣ����жϽӿ��Ƿ���ڣ�����ʹ��ԭ��
 * ��ȡȫ�ֱ����ķ�ʽ�����С�
 */
#define EOM_ID_IP                  ((ULONG)EOM_OID_DEFINE(MID_WAN_BASE, 0x01))
#define EOM_ID_IPSTAT              ((ULONG)EOM_OID_DEFINE(MID_WAN_BASE, 0x11))
#define EOM_ID_IPBPASTAT           ((ULONG)EOM_OID_DEFINE(MID_WAN_BASE, 0x16))

/*******************************************************************/
/*                            ����ԭ��                             */
/*******************************************************************/
/* FIB��ؽӿ� */
/* ������� */
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
/*                      ��Ʒ���ƿ�ת���ṹ����                   */
/*******************************************************************/
/* IP ��ת���� */
typedef struct tagIPFS_Conf
{
    ULONG ulHashSize;     /* ��ת�������� */
    UINT  uiCachePerCPU;  /* ÿCPU����Ŀ�ת���� */
    UINT8 uiIsCRCHash;    /* �Ƿ�ʹ�ÿ���CRC�㷨 */
} IPFS_CONF_S;


/*******************************************************************/
/*                      ��ת����ԭ��                               */
/*******************************************************************/
/*��ת���ע�ắ��*/
extern VOID IPFS_Init_Conf(IN IPFS_CONF_S *pstConfData);

extern ULONG USR_IPFS_Init( IN LPVOID pStartContext );

/*******************************************************************/
/*                    ��Ʒ����IPv6��ת���ṹ����                 */
/*******************************************************************/
typedef struct tagIP6FS_Conf
{
    ULONG ulHashSize;     /* ��ת�������� */
    UINT  uiCachePerCPU;  /* ÿCPU����Ŀ�ת���� */
    UINT8 uiIsCRCHash;    /* �Ƿ�ʹ�ÿ���CRC�㷨 */
} IP6FS_CONF_S;

/*******************************************************************/
/*                      IPv6��ת����ԭ��                           */
/*******************************************************************/
extern VOID IP6FS_Init_Conf(IN IP6FS_CONF_S *pstConfData);

extern ULONG USR_IP6FS_Init(IN LPVOID pStartContext);

#endif /* __KERNEL__ */


#ifdef __cplusplus
}
#endif /* end of __cplusplus */

#endif /* end of _DRV_IP_H_ */


