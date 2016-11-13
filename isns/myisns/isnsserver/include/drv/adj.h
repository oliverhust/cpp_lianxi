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

/* ����NickNameʹ�õ����������������� */
#ifndef TRILL_NICKNAME
#define TRILL_NICKNAME USHORT
#endif

/* �ڽӱ���ˢ�������Ƿ���Ҫͬ�� */
#define L3_ADJACENT_RELOAD_NOSYN   0      /* ����Ҫͬ�� */
#define L3_ADJACENT_RELOAD_SYN     1      /* ��Ҫͬ�� */

#define ADJ_NEXTHOP_CONTEXT_LEN 2     /* nexthop��Դ���� */
#define ADJ_DRV_CONTEXT_LEN     6     /* �ڽӱ���Դ���� */
#define MAX_LINKHEAD_SIZE       52UL  /* �����·��ͷ���� */

/* ADJ�������ԣ���Ҫ������ARP��ND�����·�������ʱ��
  ��֪�����·����Ƿ��Ǿ�̬�����ñ���Լ�������˿��Ƿ���UP״̬��
  ����ģ����ݴ���������Ӧ�Ĵ��� */

#define ADJ_ENTRY_ATTRIBUTE_LONGSTATIC   (1<<0)  /* ��̬������ARP��ND���� */
#define ADJ_ENTRY_ATTRIBUTE_DOWN         (1<<1)  /* �������ڶ˿�down */
#define ADJ_ENTRY_ATTRIBUTE_MULTIPORT    (1<<2)  /* ��˿�ARP��ND���� */
#define ADJ_ENTRY_ATTRIBUTE_IPOEDHCP     (1<<3)  /* ipoe dhcp���ɵı��� */
#define ADJ_ENTRY_ATTRIBUTE_TRILLREMOTE  (1<<4)  /* TRILLԶ�˱��� */

/*****************����ģ�鶨��**********************/
/* ֪ͨ�����ڽӱ�������� */
typedef enum tagL3ADJ_LinkOpType
{
    L3_ADJLINK_ADD = 0,        /* ����һ������ */
    L3_ADJLINK_MOD,            /* �޸�һ������ */
    L3_ADJLINK_DEL,            /* ɾ��һ������ */
    L3_ADJLINK_BUTT
} L3_ADJLINK_OPTYPE_E;

/* ADJ������·��ҵ������ */
typedef enum tagADJ_ServiceType
{
    ADJ_SERVICE_UNKNOWN = 0,  /* δ֪ҵ������ */
    ADJ_SERVICE_ETH,          /* ETHҵ������ */
    ADJ_SERVICE_ATM,          /* ATMҵ������ */
    ADJ_SERVICE_PPP,          /* PPPҵ������ */
    ADJ_SERVICE_HDLC,         /* HDLCҵ������ */
    ADJ_SERVICE_FR,           /* FRҵ������*/
    ADJ_SERVICE_TUNNEL,       /* Tunnelҵ������*/
    ADJ_SERVICE_MTUNNEL,      /* MTunnelҵ������ */
    ADJ_SERVICE_TUNNELBDL,    /* Tunnel Bundlelҵ������ */
    ADJ_SERVICE_SSLVPNAC,     /* SSLVPN accessҵ������ */
    ADJ_SERVICE_CMTUNNEL, /* CMTUNNELҵ������ */    
    ADJ_SERVICE_BUTT       
} ADJ_SERVICE_TYPE_E;

/* ADJ������·��ṹ���� */
typedef enum tagADJ_LinkMedia
{
    ADJ_LINKMEDIA_UNKNOWN = 0,   /* Unknown link */
    ADJ_LINKMEDIA_BROADCAST,     /* Broadcast link */
    ADJ_LINKMEDIA_P2P,           /* P2P link */
    ADJ_LINKMEDIA_NBMA,          /* NBMA link */
    ADJ_LINKMEDIA_BUTT           /* Butt */
} ADJ_LINKMEDIA_TYPE_E;

/* ADJ��������� */
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

/* �ڽӱ��·������ṹ */
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
    UINT64 auiTrillVnDrvContext[2];                /* Զ��ARP���õ�VN��drvcontext  */
} L3_ADJACENT_LINK_S;

/*******************************************************************/
/*                      ��Ʒ�����ڽӱ���ṹ����                 */
/*******************************************************************/
/* ARPģ�鶨�ƽṹ */
typedef struct tagARP_Conf
{
    ULONG ulMaxARPNum;           /* ARP�������� */
    ULONG ulMaxMultiportNum;     /* ��˿�ARP����� */
    ULONG ulMaxBlackRouteNum;    /* ARP�ڶ�·�ɶ��� */     
    ULONG ulDefaultRateLimit;    /* ARP���ٶ��� */
    ULONG ulMaxARPSuppNum;       /* evi ARP suppression�������� */
    BOOL_T bSupportMFF;          /* �Ƿ�֧��MFF */
    BOOL_T bSupportFastReply;    /* �Ƿ�֧�ִ��� */
    BOOL_T bSupportDetection;    /* �Ƿ�֧��Detection */
    BOOL_T bSupportSnooping;     /* �Ƿ�֧��Snooping */
    BOOL_T bSupportPortFilter;   /* �Ƿ�֧��PortFilter */
    BOOL_T bSupportEviFastReply; /* �Ƿ�֧��Evi����Ӧ�� */
    BOOL_T bSupportOverlaySup;   /* �Ƿ�֧��Overlay ARP suppression */    
    BOOL_T bSupportVpwsSup;      /* �Ƿ�֧��Vpws ARP suppression */    
    UINT uiMaxSnoopingNum;       /* ARP Snooping���������Ŀ */
    UINT uiMaxOverlaySupNum;     /* Overlay ARP suppression���������Ŀ */
    UINT uiMaxVpwsSupNum;        /* Vpws ARP suppression���������Ŀ */
} ARP_CONF_S;

/* WADJ4ģ�鶨�ƽṹ */
typedef struct tagWADJ4_Conf
{
    ULONG ulMaxWADJNum;   /* WADJ4�������� */
} WADJ4_CONF_S;

/* WADJ6ģ�鶨�ƽṹ */
typedef struct tagWADJ6_Conf
{
    ULONG ulMaxWADJ6Num;   /* WADJ6�������� */
} WADJ6_CONF_S;

/* NDģ�鶨�ƽṹ */
typedef struct tagND_Conf
{
    ULONG ulMaxNDNum;                    /* ND�������� */
    BOOL_T bSupportVpwsSup;              /* �Ƿ�֧��ND vpws suppression */
    UINT uiMaxVpwsSupNum;                /* ND vpws suppression���������Ŀ */
    BOOL_T bSupportSnooping;   /* �Ƿ�֧��Snooping */
    BOOL_T bSupportDetection;  /* �Ƿ�֧��Detection */
    UINT uiMaxSnoopingNum;     /* ND Snooping��������� */
    BOOL_T bSupportRaguard;    /* �Ƿ�֧��Ra guard */
    UINT uiMaxRaguardPolicyNum;  /* Ra guard�����Ը��� */
}ND_CONF_S;

/* IPv4�ڽӱ�ˢ�½ӿ� */
extern ULONG ADJ4_ReloadByIfIp(IN IF_INDEX ifIndex, IN INADDR_S stIpAddr);
extern ULONG ADJ4_ReloadByIf(IN IF_INDEX ifIndex, IN BOOL_T bRefreshSubIf);
extern ULONG ADJ4_ReloadByPW(IN UINT uiVsiIndex, IN UINT uiLpwId);

extern ULONG ADJ4_ReloadNoResource(VOID);

/* MAC�仯֪ͨ�ӿ�*/
extern ULONG ARP_MacNotify(IN const UCHAR aucMacAddr[MAC_ADDR_LEN], 
                           IN USHORT usVlanId, 
                           IN IF_INDEX ifIndexPort);
extern ULONG ARP_TrillMacNotify(IN const UCHAR aucMacAddr[MAC_ADDR_LEN],
                                IN USHORT usVlanId,
                                IN TRILL_NICKNAME nickNameRb);
/* IPv6�ڽӱ�ˢ�½ӿ� */
extern ULONG ADJ6_ReloadByIfIp(IN IF_INDEX ifIndex, IN IN6ADDR_S stIpv6Addr);
extern ULONG ADJ6_ReloadByIf(IN IF_INDEX ifIndex, IN BOOL_T bRefreshSubIf);
extern ULONG ADJ6_ReloadNoResource(VOID);
extern ULONG ADJ6_ReloadByPW(IN UINT uiVsiIndex, IN UINT uiLpwId);

/* MAC�仯֪ͨ�ӿ�*/
extern ULONG ND_MacNotify(IN const UCHAR aucMacAddr[MAC_ADDR_LEN],
                          IN USHORT usVlanId,
                          IN IF_INDEX ifIndexPort);
extern ULONG ND_TrillMacNotify(IN const UCHAR aucMacAddr[MAC_ADDR_LEN],
                               IN USHORT usVlanId,
                               IN TRILL_NICKNAME nickNameRb);

/* ARPģ���ʼ����غ��� */
extern ULONG ARP_Init_Conf(IN const ARP_CONF_S *pstConfData);
extern ULONG ARP_ModInit(INOUT LPVOID pStartContext);
extern ULONG ARP_ModRun(INOUT LPVOID pStartContext);
extern ULONG USR_ARP_Init (INOUT LPVOID pStartContext);
extern ULONG USR_ARP_Run(INOUT LPVOID pStartContext);

/* ARPģ��MDC�ص����� */
extern ULONG ARP_MdcCallBack(IN UINT uiEvent, IN VOID *pData);

/* WADJ4ģ���ʼ����غ��� */
extern ULONG WADJ4_Init_Conf(IN const WADJ4_CONF_S *pstConfData);
extern ULONG WADJ4_ModInit(INOUT LPVOID pStartContext);
extern ULONG WADJ4_ModRun(INOUT LPVOID pStartContext);
extern ULONG USR_WADJ4_Init (INOUT LPVOID pStartContext);
extern ULONG USR_WADJ4_Run(INOUT LPVOID pStartContext);

/* WADJ4ģ��MDC�ص����� */
extern ULONG WADJ4_MdcCallBack(IN UINT uiEvent, IN VOID *pData);

/* NDģ���ʼ����غ��� */
extern ULONG ND_Init_Conf(IN const ND_CONF_S *pstConfData);
extern ULONG ND_ModInit(INOUT LPVOID pStartContext);
extern ULONG ND_ModRun(INOUT LPVOID pStartContext);
extern ULONG USR_ND_Init (INOUT LPVOID pStartContext);
extern ULONG USR_ND_Run(INOUT LPVOID pStartContext);

/* WADJ6ģ���ʼ����غ��� */
extern ULONG WADJ6_Init_Conf(IN const WADJ6_CONF_S *pstConfData);
extern ULONG WADJ6_ModInit(INOUT LPVOID pStartContext);
extern ULONG WADJ6_ModRun(INOUT LPVOID pStartContext);
extern ULONG USR_WADJ6_Init (INOUT LPVOID pStartContext);
extern ULONG USR_WADJ6_Run(INOUT LPVOID pStartContext);

/* WADJ6ģ��MDC�ص����� */
extern ULONG WADJ6_MdcCallBack(IN UINT uiEvent, IN VOID *pData);

/* NDģ��MDC�ص����� */
extern ULONG ND_MdcCallBack(IN UINT uiEvent, IN VOID *pData);

/* ADJģ���ʼ����غ��� */
extern ULONG ADJ_ModInit(INOUT LPVOID pStartContext);
extern ULONG ADJ_ModRun(INOUT LPVOID pStartContext);

/* ADJģ��MDC������ */
extern ULONG ADJ_MdcCallBack(IN UINT uiEvent, IN VOID *pData);

#ifdef __KERNEL__ /* used only in kernel space */

#ifndef _SYS_FSBUF_H_
#include <sys/fsbuf.h>
#endif

/* �������ⷢ�ͺ��� */
typedef ULONG (*ADJ_DriverSpecialOut_PF)(IN const UINT32 *puiNexthopDrv,
                                         IN const UINT32 *puiDrvContext, 
                                         INOUT MBUF_S *pMbuf);

typedef ULONG (*ADJ_DriverRawSpecialOut_PF)(IN const IF_INDEX ifIndex,
                                            IN const UINT32 *puiNexthopDrv,
                                            IN const UINT32 *puiDrvContext,
                                            INOUT FSBUF_PKTINFO_S *pstPktInfo,
                                            INOUT FSBUF_BLOCKINFO_S *pstBlockInfo);

/* ע�����ⷢ�ͺ����Ļ�ȡ���� */
extern ULONG ADJ_RegDriverSpecialOut(IN ADJ_DriverSpecialOut_PF pfDriverSpecialout,
                                     IN ADJ_DriverRawSpecialOut_PF pfDriverRawSpecialout);

#endif

#ifdef __cplusplus
}
#endif /* end of __cplusplus */

#endif /* end of _DRV_ADJ_H_ */

