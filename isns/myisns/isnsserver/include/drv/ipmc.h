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

/* ת����������� */
typedef enum tagIPMC_TYPE_E
{
    IPMC_L3_ADD_IPM,          /* ��������鲥���� */ 
    IPMC_L3_DEL_IPM,          /* ɾ�������鲥���� */
    IPMC_L3_ADD_OIF,          /* ������������ӿ� */
    IPMC_L3_DEL_OIF,          /* ɾ�����������ӿ� */
    IPMC_L3_SET_IIF,          /* ������ӿ� */
    IPMC_L3_ADD_PORT,         /* ������������˿� */
    IPMC_L3_DEL_PORT,         /* ɾ�����������˿� */
    IPMC_L3_ADD_SLOT,         /* ����������ֲ�ʽת���ĳ��ӿڰ���Ϣ */
    IPMC_L3_DEL_SLOT,         /* ɾ���������ֲ�ʽת���ĳ��ӿڰ���Ϣ */
    IPMC_L3_SET_DATA_GROUP,   /* ������������Ӧ��Data-Group��ַ */
    IPMC_L2_ADD_IP,           /* ��Ӷ����鲥IP���� */
    IPMC_L2_DEL_IP,           /* ɾ�������鲥IP���� */
    IPMC_L2_ADD_IP_PORT,      /* ��Ӷ����鲥IP������˿� */
    IPMC_L2_DEL_IP_PORT,      /* ɾ�������鲥IP������˿� */
    IPMC_L2_ADD_IP_SLOT,      /* ��Ӷ����鲥IP����ֲ�ʽת���ĳ��ӿڰ���Ϣ */
    IPMC_L2_DEL_IP_SLOT,      /* ɾ�������鲥IP����ֲ�ʽת���ĳ��ӿڰ���Ϣ */
    IPMC_L2_ADD_MAC,          /* ��Ӷ����鲥MAC���� */
    IPMC_L2_DEL_MAC,          /* ɾ�������鲥MAC���� */
    IPMC_L2_ADD_MAC_PORT,     /* ��Ӷ����鲥MAC������˿� */
    IPMC_L2_DEL_MAC_PORT,     /* ɾ�������鲥MAC������˿� */
    IPMC_L2_ADD_MAC_SLOT,     /* ��Ӷ����鲥MAC����ֲ�ʽת���ĳ��ӿڰ���Ϣ */
    IPMC_L2_DEL_MAC_SLOT,     /* ɾ�������鲥MAC����ֲ�ʽת���ĳ��ӿڰ���Ϣ */
    IPMC_L3_SET_ENTRY_RP,     /* ���ı����RP��Ϣ */
    IPMC_L3_ADD_ENTRY_DFIF,   /* ��ӱ����DF�ӿ���Ϣ */
    IPMC_L3_DEL_ENTRY_DFIF,   /* ɾ�������DF�ӿ���Ϣ */
    IPMC_L3_ADD_RP,           /* ���RP�����DF�ӿ���Ϣ */
    IPMC_L3_DEL_RP,           /* ɾ��RP�����DF�ӿ���Ϣ */
    IPMC_L3_ADD_RP_DFIF,      /* ���RP��DF�ӿ���Ϣ */
    IPMC_L3_DEL_RP_DFIF,      /* ɾ��RP��DF�ӿ���Ϣ */
    IPMC_L3_SET_RP_IIF,       /* ����RP�������ӿ� */
    IPMC_TYPE_BUTT
}IPMC_TYPE_E;


/* �鲥ת��������ӿڵ�λͼ���� */
#define  L3_MROUTE_TUNNEL   0x1      /* ���ӿ�ΪTUNNEL�ӿ� */
#define  L3_MROUTE_VLANIF   0x2      /* ���ӿ�ΪVLAN��ӿ� */
#define  L3_MROUTE_ROUTERIF 0x4      /* ���ӿ�Ϊ·�����ӿ� */
#define  L3_MROUTE_INIF     0x8      /* ���ӿ�Ϊ��ӿ� */
#define  L3_MROUTE_MTUNNEL  0x10     /* ���ӿ�ΪMTUNNEL�ӿ� */
#define  L3_MROUTE_PRIVATE  0x20     /* ���ӿ�ΪMTUNNEL�ӿ�ʱ�����ͬʱЯ���˱���־��
                                        ������˽��MTUNNEL���ӿڣ���Ҫ�ӷ�װ��
                                        û�б���־������ǹ���MTUNNEL���ӿڣ���Ҫ���װ */
#define  L3_MROUTE_SUBVLAN  0x40     /* ���ӿ�ΪSUBVLAN�ӿ� */

#define  L3_MROUTE_NBMALINK 0x80     /* ���ӿ�ΪNBMA���ͽӿ� */

#define IPMC_PORT_DRV_LEN    2

typedef struct tagIPMC_PORT_S
{
    IF_INDEX  ifIndex;               /* ���˿ڵĽӿ����� */
    UINT32    uiIfType;              /* ���˿ڵ����ͣ���ӿڹ����ṩ��������ͬ */
    UINT32 auiDrvContext[IPMC_PORT_DRV_LEN];           /* ����ά���Ĳ�Ʒ������Ϣ */
}IPMC_PORT_S;

#define IPMC_IF_DRV_LEN       2
#define IPMC_IF_TUN_DRV_LEN   2

/* ���ӿڵ�������ṹ */
typedef struct tagIPMC_OIF_S
{
    struct tagIPMC_OIF_S *pstNext;   /* ��������ṹ��һ���ڵ��ָ�� */
    IF_INDEX ifIndex;                /* ���ӿڵĽӿ����� */
    UINT32 uiVlanID;                 /* VLAN���ӿڵ�VLAN ID */
    UINT32 uiIfType;                 /* ���ӿڵ����ͣ���ӿڹ����ṩ������ֵ��ͬ */
    UINT32 uiFlag;                   /* ���ӿ����� */
    UINT32 uiPortNum;                /* ���˿������еĶ˿���Ŀ */
    IPMC_PORT_S *pstPortList;        /* �˿�����ָ�� */
    UINT32 auiDrvContext[IPMC_IF_DRV_LEN];           /* ����ά���Ĳ�Ʒ������Ϣ */
    UINT32 auiTunnelDrvContext[IPMC_IF_TUN_DRV_LEN]; /* ������ӿڵĲ�Ʒ������Ϣ */
    UINT32 uiDataGrpIp;              /* MTunnel���ӿڵ�DataGroup IP��ַ�������� */
    UINT32   uiSecondVlanID;         /* VLAN ID      */
    UINT32   uiUserID;               /* User ID      */ 
    UINT32   uiPVC;                  /* PVC ID      */ 
    UINT32 auiRemoteLinkAddr[4];     /* NBMA��Զ�����ӵ�ַ */
}IPMC_OIF_S;


/* �鲥ת�������λͼ���� */
#define  L3_MROUTE_FWD              0x1      /* ��������ת�� */
#define  L3_MROUTE_DROPPKT          0x2      /* �������� */
#define  L3_MROUTE_TOCPU            0x4      /* ������CPU������ת�� */
#define  L3_MROUTE_FWDANDTOCPU      0x8      /* ����ת����ͬʱ����һ����CPU���� */
#define  L3_MROUTE_RPFCHECKUNICAST  0x10     /* RPF��鷽ʽ�����Ӳ������·�ɱ� */
#define  L3_MROUTE_RPFCHECKDESIGNED 0x20     /* RPF��鷽ʽ������鲥������ӿ� */
#define  L3_MROUTE_DUMMY            0x40     /* �����鲥��ת���� */
#define  L3_MROUTE_MVLAN            0x80     /* �鲥VLAN���� */
#define  L3_MROUTE_IIFMTUNNEL       0x100    /* ������ӿ�Ϊ�鲥TUNNEL�ӿ� */
#define  L2_MROUTE_FWD              0x200    /* ��������ת�� */
#define  L2_MROUTE_DROPPKT          0x400    /* �������� */
#define  L2_MROUTE_TOCPU            0x800    /* ������CPU������ת�� */
#define  L2_MROUTE_FWDANDTOCPU      0x1000   /* ����ת����ͬʱ����һ����CPU���� */
/* ��������������CPU���ԣ�L2_MROUTE_TOCPU��L2_MROUTE_FWDANDTOCPU��L3_MROUTE_TOCPU��L3_MROUTE_FWDANDTOCPU��
 * ��λ��Ĺ�ϵ��λ���ϱ���־λ�����������͵�CPU�Ǹ���CPU������������CPU */
#define  L2_MROUTE_CFDPROXYCPU      0x2000   /* ���͸���CPU������ */
#define  L3_MROUTE_BIDIR            0x4000   /* ˫��PIM���� */
#define  L2_MROUTE_VSI              0x8000   /* VSI���� */
#define  L3_MROUTE_MTI_NEXTHOP      0x10000  /* �鲥tunnel����һ����Ϣ */
#define  L2_MROUTE_TRILLGATEWAY     0x20000  /* TRILLж��ģʽ�У��������·���MAC���� */
#define  L2_MROUTE_TRILLACCESS      0x40000  /* TRILLж��ģʽ�У��ڷ������·���MAC���� */

#define  L3_MROUTE_IIFNBMA          0x80000  /* ������ӿ�ΪNBMA�ӿ� */

#define IPMC_ENTRY_DRV_LEN    4

/* �·�IPv4���������鲥��������ݽṹ */
typedef struct tagIPMCINFO_S
{
    /* ������Ϣ */ 
    UINT32 uiSrcIp;            /* �鲥ԴIP��ַ�������� */
    UINT32 uiGrpIp;            /* �鲥��IP��ַ�������� */
    UCHAR  aucGrpMACAddr[MAC_ADDR_LEN];   /* �鲥��MAC��ַ */
    UINT32 uiCmdType;          /* �������� */
    UINT32 uiFlag;             /* �������� */
    VRF_INDEX vrfIndex;        /* VRF���� */
    UINT32 auiDrvContext[IPMC_ENTRY_DRV_LEN];    /* ����ά���Ĳ�Ʒ������Ϣ */
    
    /* ��ӿ���Ϣ */
    IF_INDEX ifInIfIndex;
    UINT32 uiInVlanID;
    UINT32 uiInIfType;

    /* ԭ��ӿ���Ϣ */
    IF_INDEX ifInIfIndexOld;
    UINT32 uiInVlanIDOld;
    UINT32 uiInIfTypeOld;

    /* L2�ĳ��˿���Ϣ */
    UINT32 uiPortNum;                /* ���˿������еĶ˿���Ŀ */
    IPMC_PORT_S *pstPortList;        /* �˿�����ָ�� */

    /* L3���ӿ�/���˿���Ϣ */
    UINT32 uiOIfSize;
    IPMC_OIF_S *pstOIfList;
    
    /* �����Ӧ��RP��Ϣ */
    UINT32 uiRPAddress;

    /* �ֲ�ʽ�ӿڰ���Ϣ��L2/L3ͨ�� */
    BITMAP_S *pstSlotBitmap;

    UINT32 uiSeq; /* �鲥ת��������� */

    UINT32 uiRemoteIp;         /* NBMA��ӿ�Զ��IP��ַ�������� */
    
}IPMCINFO_S;


/* ��ȡ�����鲥�����ͳ����Ϣ�����ݽṹ */
typedef struct tagIPMC_COUNTERS_S
{
    VRF_INDEX vrfIndex;          /* VRF���� */
    UINT32 uiSrcIp;              /* �����鲥����Դ��ַ�������� */
    UINT32 uiDestIp;             /* �����鲥�������ַ�������� */
    UINT32 uiFlag;               /* �������� */
    IF_INDEX ifInIfIndex;        /* ���������ӿ����� */
    UINT32 uiInVlanID;           /* ���������ӿ�VLAN ID */
    UINT32 uiMatchedPacketNum;   /* ת������ƥ�䵽�ı�����Ŀ */
    UINT32 uiMatchedPacketByte;  /* ת������ƥ�䵽�ı��ĵ��ֽ���Ŀ */
    UINT32 uiForwardPacketNum;   /* ƥ��ת������������ӿ�ת�����ĵ���Ŀ */
    UINT32 uiForwardPacketByte;  /* ƥ��ת������������ӿ�ת�����ĵ��ֽ��� */
    UINT32 uiWrongIfPktNum;      /* �Ӵ�����ӿ��յ��ı�����Ŀ */
    UINT32 auiDrvContext[IPMC_ENTRY_DRV_LEN];      /* ����ά���Ĳ�Ʒ������Ϣ */
}IPMC_COUNTERS_S;

/* MTUNNEL�������� */
typedef enum tagIPMC_MTUNNEL_TYPE_E
{
    IPMC_MTUNNEL_CREATE,          /* ���MTUNNEL */ 
    IPMC_MTUNNEL_DELETE,          /* ɾ��MTUNNEL*/
    IPMC_MTUNNEL_UP,              /* MTUNNEL UP��Я��������װ��Ϣ */ 
    IPMC_MTUNNEL_DOWN,            /* MTUNNEL DOWN */
    IPMC_MTUNNEL_TYPE_BUTT
}IPMC_MTUNNEL_TYPE_E;

/* �鲥tunnelģʽ */
typedef enum tagMTUNNEL_MODE
{
    MTUNNEL_MODE_IPV4_MD_GRE = 1,
    MTUNNEL_MODE_IPV4_VXLAN_UDP,
    MTUNNEL_MODE_MAX
}MTUNNEL_MODE_E;
/* �·�IPv4�鲥MTunnel�ӿڵ����ݽṹ */
typedef struct tagIPMC_MTUNNEL_S 
{
    UINT32 uiCmdType;               /* ��������, IPMC_MTUNNEL_TYPE_E */
    UINT32 uiTunnelMode;            /* ���ģʽ */
    IF_INDEX ifIndex;               /* MTI�ӿ����� */
    VRF_INDEX vrfIndex;             /* VRF���� */
    USHORT usFlag;                  /* �ӿ����ԣ���δʹ�� */
    UINT32 uiSrcIp;                 /* ���ԴIP��ַ�������� */
    UINT32 uiDestIp;                /* ���Ŀ��IP��ַ�������� */
    UINT32 uiMTU;                   /* ����ӿ�MTU */
    UINT32 auiDrvContext[IPMC_IF_TUN_DRV_LEN]; /* ����ά���Ĳ�Ʒ������Ϣ */        
}IPMC_MTUNNEL_S;

#define  IPMC_L3MCAST_ENABLE           0x0      /* ʹ�������鲥 */
#define  IPMC_L3MCAST_DISABLE          0x1      /* �ر������鲥 */
#define  IPMC_IGMPSOOPING_ENABLE       0x2      /* ʹ��IGMP SnoopingЭ�� */
#define  IPMC_IGMPSOOPING_DISABLE      0x3      /* ȥʹ��IGMP SnoopingЭ�� */
#define  IPMC_PIMSOOPING_ENABLE        0x4      /* ʹ��PIM SnoopingЭ�� */
#define  IPMC_PIMSOOPING_DISABLE       0x5      /* ȥʹ��PIM SnoopingЭ�� */
#define  IPMC_DROPUNKNOWN_ENABLE       0x6      /* ʹ��δ֪�鲥���ݱ��Ķ������� */
#define  IPMC_DROPUNKNOWN_DISABLE      0x7      /* ȥʹ��δ֪�鲥���ݱ��Ķ������� */
#define  IPMC_ROUTERPORT_ADD           0x8      /* ���·�����˿� */
#define  IPMC_ROUTERPORT_DELETE        0x9      /* ɾ��·�����˿� */
#define  IPMC_MVLAN_ENABLE             0xa      /* ʹ���鲥VLAN*/
#define  IPMC_MVLAN_DISABLE            0xb      /* ȥʹ���鲥VLAN */
#define  IPMC_SOURCEDENY_ENABLE        0xc      /* ʹ���鲥����Դ�˿����ƹ���*/
#define  IPMC_SOURCEDENY_DISABLE       0xd      /* ȥʹ���鲥����Դ�˿����ƹ��� */
#define  IPMC_BIDIR_ENABLE             0xe      /* ʹ��˫��PIM */
#define  IPMC_BIDIR_DISABLE            0xf      /* �ر�˫��PIM */
#define  IPMC_IGMPSOOPING_VSI_ENABLE   0x10     /* VSI��ʹ��IGMP snoopingЭ�� */
#define  IPMC_IGMPSOOPING_VSI_DISABLE  0x11     /* VSI��ȥʹ��IGMP snoopingЭ�� */
#define  IPMC_PIMSOOPING_VSI_ENABLE    0x12     /* VSI��ʹ��PIM snoopingЭ�� */
#define  IPMC_PIMSOOPING_VSI_DISABLE   0x13     /* VSI��ȥʹ��PIM snoopingЭ�� */
#define  IPMC_DROPUNKNOWN_VSI_ENABLE   0x14     /* ��VSI��ʹ��δ֪�鲥���ݱ��Ķ�������*/
#define  IPMC_DROPUNKNOWN_VSI_DISABLE  0x15     /* ��VSI��ȥʹ��δ֪�鲥���ݱ��Ķ������� */
#define  IPMC_ROUTERPORT_VSI_ADD       0x16     /* VSI�����·�����˿� */
#define  IPMC_ROUTERPORT_VSI_DELETE    0x17     /* VSI��ɾ��·�����˿� */


#define IPMC_ENABLE_DRV_LEN    2

/* ������ƽ̨ע���鲥�����ӿڵĺ���ָ�����ݽṹ */
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

/* ������ƽ̨ע���鲥�����ӿڵĺ��� */
ULONG IPMC_Register_Driver(IN ULONG ulVersion, IN IPMC_DRV_INTF_S *psDriverIPMCIntf);



/* �鲥ת������ */
typedef struct tagIPMC_CONF
{
    ULONG   ulSpecMaxIpv4L3McEntryNum;  /* ipv4����ת�������� */
    ULONG   ulSpecMaxIpv4MvlanEntryNum; /* ipv4�鲥VLANת�������� */
    ULONG   ulSpecMaxIpv4BidirRPNum;    /* ipv4˫��PIM RP��� */
    ULONG   ulSpecMaxSoftFwdOIfNum;     /* ��ת��֧�ֵĳ��ӿ�����.       */
    BOOL_T  bSupportAsicFwd;            /* �Ƿ�֧��������.               */
    BOOL_T  bSupportSoftFwdCnt;         /* �Ƿ�֧����ת������ͳ��,��֧�ֵ������ֻͳ��Ӳ��ת�����������鶨��Ϊfalse��ʡ�ڴ� */
} IPMC_CONF_S;


/* ������ƽ̨ע��IPv4�����鲥ת�����ĺ��� */
VOID IPMC_Init_Conf(IN IPMC_CONF_S *pstConfData);

/* ����ע��ͳ�ʼ������ */
ULONG USR_IPMC_DrvInit( IN LPVOID pContext );
ULONG USR_IPMC_Init( IN LPVOID pContext);
ULONG USR_IPMC_Run( IN LPVOID pContext );

/* �鲥ת��ģ���ʼ��. */
ULONG IPMC_Init( IN LPVOID pStartupContext);
ULONG IPMC_Run( IN LPVOID pStartupContext);

/* �����鲥����VLAN���ӿ�ת��ʱ��ֻ��ָ��VLAN�ӿڷ��ͣ����ٱ���VLAN�ڵĶ�����˿�������͡� */
VOID IPMC_EnableVlanOutIfHardForwarding(VOID);
/* �����鲥�У���VLAN��ӿ��ڵ������˿����ת���� */
VOID IPMC_EnableVlanInIfSoftForwarding(VOID);
/* ˫��PIM֧��ÿ����ά��DF */
VOID IPMC_EnableBidirPimPerEntryDf(VOID);


#ifdef __KERNEL__

/* ����֪ͨ���¼����� */
typedef enum tagIPMC_EVT
{
    /* MTI���: ֪ͨҵ�񻷻ؿ��Ƿ���ã� pDataΪBOOL_T���ͣ�BOOL_TRUE��ʾ���ã�BOOL_FALSE��ʾ�����á�
       Ĭ���ǿ��õģ�������ҵ�񻷻ؿڵĲ�Ʒ��Ҫ�ڳ�ʼ���׶ε�����IPMC_Run()��֪ͨƽ̨�����ã�
       �ֲ�ʽ�豸��ֻ��Ҫ�����ذ���ã��ӿڰ岻������¼���
     */ 
    IPMC_EVT_MTI_SET_SERVICE_LOOPBACK = 1, 

    /* MTI���: ��ˢ��������¼��� pData������ */
    IPMC_EVT_MTI_MD_REFRESH,      /* �鲥VPN���鲥�����ˢ */
    IPMC_EVT_MTI_VXLAN_REFRESH,    /* VXLAN���鲥�����ˢ */

    IPMC_EVT_BUTT, 
}IPMC_EVT_E;

/*****************************************************************************
    Func Name: IPMC_EventNotify
 Date Created: 2012/9/7 
       Author: x02188
  Description: ����֪ͨƽ̨��Ϣ�ӿ�
        Input: IN IPMC_EVT_E enEvtType, �¼�����
               IN VOID *pData, �������������¼��������  
       Output: NA
       Return: ULONG, ERROR_SUCCESS ����ɹ�
                      ERROR_FAILED  ����ʧ��
      Caution: ��Ҫ����VD
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
  Description: ��������MTI���װ��ı���
        Input: IN IF_INDEX ifIndex, MTI�ӿ�
               IN MBUF_S *pstMBuf, ���װ���IP����
       Output: NA
       Return: ULONG, ERROR_SUCCESS ����ɹ�
                      ERROR_FAILED  ����ʧ��
      Caution: ���װ��ı����޶���ͷ���ӷ�װ�ı��Ĳ���ʹ�ñ��ӿ�����
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
