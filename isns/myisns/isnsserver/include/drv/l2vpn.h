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
#define L2VPN_INVALID_ISID     ((UINT)0)           /* ��Ч��I-SID */
#define L2VPN_INVALID_VXLANID  (UINT)(-1)          /* ��Ч��VXLAN-ID */

#define L2VPN_INVALID_NVGREID  (UINT)(-1)          /* ��Ч��NVGRE-ID */

#define L2VPN_INVALID_UINTLINKID   0xFFFFFFFF

#define L2VPN_MINM_DRVCONTEXT_LENGTH   6
#define L2VPN_MINMIF_DRVCONTEXT_LENGTH 2
#define L2VPN_DRVCONTEXT_MAXLENGTH   8
#define L2VPN_FORWARD_DRVCONTEXT_LENGTH 2


#define L2VPN_INVALID_DRVCONTEXT  (UINT)(-1)

/* VSI���������λΪ1������VPWSʵ����Ϊ0������VPLSʵ�� */
#define L2VPN_VSI_VPWSFLAG 0x80000000
#define L2VPN_VSI_ISVPWS(uiVsiIndex) ((L2VPN_INVALID_VSIINDEX != (uiVsiIndex)) && \
                                      (L2VPN_VSI_VPWSFLAG == ((uiVsiIndex) & L2VPN_VSI_VPWSFLAG)))

/* AC��װ����  */
typedef enum tagL2VPN_AC_ENCAPTYPE
{
    L2VPN_AC_ENCAP_NONE,
    L2VPN_AC_ENCAP_EVC,        /* MEF EVC VLANƥ���б� */
    L2VPN_AC_ENCAP_SATOP,      /* PWE3 TDM��·����SATOP��ʽ */
    L2VPN_AC_ENCAP_CESOPSN,    /* PWE3 TDM��·����CESoPSN��ʽ */
    L2VPN_AC_ENCAP_CELLTRANS,  /* PWE3 ATM��Ԫ͸�� */
    L2VPN_AC_ENCAP_IPINTERWORKING, /* IP interwoking��ʽ */
    L2VPN_AC_ENCAP_MAX
}L2VPN_AC_ENCAPTYPE_E;

/* ACģʽ��Ƕ��� */
/* Ethernet/VLAN����ռ��0��1λ����bit */
#define L2VPN_AC_MODE_ETHERNET  0x01    /* ����ģʽΪEthernet */
#define L2VPN_AC_MODE_VLAN      0x02    /* ����ģʽΪVLAN */
/* Hub-spoke����ռ��2��3λ����bit: ��2λbit���Ϊ�Ƿ�ʹ��hub-spoke����3λbit���hub-spoke���� */
#define L2VPN_AC_MODE_SPOKE     0x04    /* ACΪspoke */
#define L2VPN_AC_MODE_HUB       0x0C    /* ACΪHub */

/* AC��װ������Ϣ */
typedef struct tagL2VPN_AC_ENCAP
{
    IF_INDEX             ifIndex;          /* �ӿ����� */
    USHORT               usServiceID;
    USHORT               usACLinkID;       /* AC��Link ID */
    IF_INDEX             ifLogicIndex;     /* AC���߼����� */
    UCHAR                ucACModeFlag;     /* ACģʽ��� */
    L2VPN_AC_ENCAPTYPE_E enACEncapType;    /* AC��װ���� */
    VOID                 *pData;           /* �ṹָ�� */
}L2VPN_AC_ENCAP_S;

#define L2VPN_AC_STATE_NORMAL 0 /* AC״̬Ϊ������״̬ */
#define L2VPN_AC_STATE_BLOCK 1 /* AC״̬Ϊ����״̬ */

/* L2VPN AC������Ϣ */
typedef struct tagL2VPN_AC_S
{
    USHORT              usMtu;             /* MTU */
    UINT                uiVsiIndex;        /* VSI���� */
    L2VPN_AC_ENCAP_S    stAcInfo;
    USHORT              usOutLinkID;
    USHORT              usState;
    UINT                uiStatFlag;        /* Ac���λ,����ͳ��ʹ��0x01 */
    UINT32              auiDrvContext[L2VPN_DRVCONTEXT_MAXLENGTH];
}L2VPN_AC_S;

/* VSI��ӿڰ�/ȥ����Ϣ */
typedef struct tagL2VPN_CFG_IF
{
    UINT               uiVsiIndex;        /* VSI���� */
    L2VPN_AC_ENCAP_S   stAcInfo;
    UINT32             auiDrvContext[L2VPN_DRVCONTEXT_MAXLENGTH];
}L2VPN_CFG_IF_S;


/* VSI���Ը��±�Ƕ��� */
#define    L2VPN_VSI_UPD_BCRESTRAIN      0x0001     /* �㲥�����ʸ��� */
#define    L2VPN_VSI_UPD_MCRESTRAIN      0x0002     /* �鲥�����ʸ��� */
#define    L2VPN_VSI_UPD_UNKNOWNRESTRAIN 0x0004     /* δ֪���������ʸ��� */
#define    L2VPN_VSI_UPD_BWLIMIT         0x0008     /* VSI����ֵ���� */
#define    L2VPN_VSI_UPD_MACLIMIT        0x0010     /* MAC��ַ�������� */
#define    L2VPN_VSI_UPD_MACLEARNING     0x0020     /* MACѧϰ�������� */
#define    L2VPN_VSI_UPD_DROPUNKNOWN     0x0040     /* MAC��ﵽ�����Ŀ����δ֪�������� */
#define    L2VPN_VSI_UPD_ISID            0x0080     /* ����I-SID */
#define    L2VPN_VSI_UPD_PWSIGNAL        0x0100     /* ����PWSIGNAL */
#define    L2VPN_VSI_UPD_DUPLICATEMODE   0x0200     /* SPB�ĸ���ģʽ���� */
#define    L2VPN_VSI_UPD_PBBENCAP        0x0400     /* PBB�ķ�װģʽ���� */
#define    L2VPN_VSI_UPD_VXLANID         0x0800     /* VXLAN��ID���� */
#define    L2VPN_VSI_UPD_MACLEARNRATE    0x1000     /* MAC��ַѧϰ�������Ƹ��� */
#define    L2VPN_VSI_UPD_VSISTATISTICS   0x2000     /* VSIͳ���������� */
#define    L2VPN_VSI_UPD_NVGREID         0x4000     /* NVGRE��ID���� */

/* VSIģʽ��Ƕ��� */
#define    L2VPN_VSI_MODE_VPLS       0x01     /* VPLS���� */
#define    L2VPN_VSI_MODE_VPWS       0x02     /* VPWS���� */
#define    L2VPN_VSI_MODE_PW         0x04     /* PWģʽ(STATIC��LDP��BGP) */
#define    L2VPN_VSI_MODE_HUB_SPOKE  0x08     /* HUB-SPOKEģʽ */
#define    L2VPN_VSI_MODE_COREDUP    0x10     /* ���ĸ���ģʽ */
#define    L2VPN_VSI_MODE_PBB_ETH    0x20     /* PBB ethernet��װ */

/* L2VPN MACѧϰ��Ƕ��� */
#define  L2VPN_VSI_MACLEARN_ENABLE          0x01
#define  L2VPN_VSI_MACLEARN_MAX_DROP        0x02

/* L2VPN VSI����ͳ�Ʊ�Ƕ��� */
#define  L2VPN_VSI_STATISTICS_ENABLE        0x01

/* L2VPN AC����ͳ�Ʊ�Ƕ��� */
#define  L2VPN_AC_STATISTICS_ENABLE        0x01

/* L2VPN AC��ѧϰʹ�ܱ�Ƕ��� */
#define  L2VPN_AC_SOFTWARE_MACLEARNING_ENABLE        0x02

/* L2VPN VSI���������Ϣ */
typedef struct tagL2VPN_CFG_VSI
{
    UCHAR     ucMacLearnFlag;       /* MACѧϰ��� */
    UINT      uiVsiIndex;           /* ��Ӧ��VSI����ֵ */
    UINT      uiBandwidth;          /* VPN�������� */
    UINT      uiMacLimit;           /* VPN��MAC��ַ�������� */
    UINT      uiMinmISID;           /* ���ֶ�ΪVSI��PBB I-SID*/
    UINT      uiSpbISID;            /* ���ֶ�ΪVSI��SPB I-SID*/
    UINT      uiVxlanID;            /* ���ֶ�ΪVSI��VXLAN-ID */
    IF_INDEX  ifIndex;              /* VSI��ӿڽӿ����� */
    USHORT    usUpdateFlag;         /* VSI���±�� */
    UCHAR     ucVsiMode;            /* VSIģʽ */
    UCHAR     ucStatisticsFlag;     /* VSI����ͳ�Ʊ�� */
    USHORT    usMacLearnRate;       /* MAC��ַѧϰ�������ƣ�һ��MACѧϰ�ĺ����� */
    UINT      uiBroadCastRestrain;  /* �㲥���ƣ�Ĭ��5120 kbps */
    UINT      uiMultiCastRestrain;  /* �鲥���ƣ�Ĭ��5120 kbps */
    UINT      uiUnknownRestrain;    /* δ֪�������ƣ�Ĭ��5120 kbps */
    UINT      uiNvgreID;            /* ���ֶ�ΪVSI��NVGRE-ID */
    UINT32    auiDrvContext[L2VPN_DRVCONTEXT_MAXLENGTH];      /* ��Ʒ���ص���Ϣ */
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

/* Overlay Tunnel LinkID����:
  0                   1                   2                   3
  0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 |L2VIfType(8bit)|                 Tunnel Number                 |
 +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/

/* ͨ��LinkID�ж��Ƿ���Overlay Tunnel */
#define L2VPN_IS_OVERLAYTUNNEL(uiLinkID) (((uiLinkID >> 24) == L2VIF_OVERLAY_UCTUNNEL_TYPE) ||\
                                       ((uiLinkID >> 24) == L2VIF_OVERLAY_MCTUNNEL_TYPE) ||\
                                       ((uiLinkID >> 24) == L2VIF_NVGRETUNNEL_TYPE))

/* ����ͳ�ƻ������� */
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
    UINT32 uiIllegalVlan;    /* PW���յ��ĺ��Ƿ�VLAN���ĵĸ���������ȱʧVLAN��Я�������VLAN */
    UINT32 uiIllegalLength;  /* PW���յ��ĳ��ȷǷ��ı��ĵĸ��������籨�ĳ��ȳ���MTU��С����̫��������С���� */
}L2VPN_STATISTICS_S;

/* ����ͳ������ */
typedef enum tagL2VPN_STATISTICS_TYPE
{
    L2VPN_STATISTICS_PW,     /* PW����ͳ������ */
    L2VPN_STATISTICS_VSI,    /* VSI����ͳ������ */
    L2VPN_STATISTICS_AC,    /* AC����ͳ������ */
    L2VPN_STATISTICS_MAX
}L2VPN_STATISTICS_TYPE_E;

/* ����ͳ�ƶ���:VSI */
typedef struct tagL2VPN_STATISTICS_OBJECT_VSI
{
    UINT    uiVsiIndex;                             /* VSI Index */
    UINT32  auiContext[L2VPN_DRVCONTEXT_MAXLENGTH]; /* VSI DrvContext */
}L2VPN_STATISTICS_OBJECT_VSI_S;

/* ���ò������� */
typedef enum tagL2VPN_IOCTL
{
    L2VPN_IOCTL_GLOBALENABLE,               /* L2VPNȫ��ʹ�� */
    L2VPN_IOCTL_GLOBALDISABLE,              /* L2VPNȫ�ֽ�ֹ */
    L2VPN_IOCTL_VLAN_RESERVE,               /* ����VLAN     */
    L2VPN_IOCTL_VLAN_RELEASE,               /* �ͷű�����VLAN */
    L2VPN_IOCTL_VSI_ADD,                    /* VSI���� */
    L2VPN_IOCTL_VSI_DELETE,                 /* VSIɾ�� */
    L2VPN_IOCTL_VSI_UPDATE,                 /* VSI���� */
    L2VPN_IOCTL_INTF_BIND,                  /* �ӿڰ󶨶���VPN */
    L2VPN_IOCTL_INTF_UNBIND,                /* �ӿ�ȥ�󶨶���VPN */
    L2VPN_IOCTL_SYNC_START,                 /* ͬ����ʼ��ʶ */
    L2VPN_IOCTL_SYNC_END,                   /* ͬ��������ʶ */
    L2VPN_IOCTL_INTF_ENABLEMINM,            /* �ӿ�ʹ��MinM */
    L2VPN_IOCTL_INTF_DISABLEMINM,           /* �ӿ�ȥʹ��MinM */
    L2VPN_IOCTL_BATCHPROC_START,            /* L2VPN���������ʼ */
    L2VPN_IOCTL_BATCHPROC_END,              /* L2VPN������������ */
    L2VPN_IOCTL_VXLAN_RESERVE,              /* ����VXLAN     */
    L2VPN_IOCTL_VXLAN_RELEASE,              /* �ͷű�����VXLAN */
    L2VPN_IOCTL_VSIIF_BIND,                 /* VSI��VSI��ӿ� */
    L2VPN_IOCTL_VSIIF_UNBIND,               /* VSIȥ��VSI��ӿ� */
    L2VPN_IOCTL_REFLECTOR_ENABLE,           /* ���ü���ַ */
    L2VPN_IOCTL_REFLECTOR_DISABLE,          /* ȡ������ַ */
    L2VPN_IOCTL_MAX
}L2VPN_IOCTL_E;

/* L2VPN Reflector���������Ϣ */
typedef struct tagL2VPN_REFLECTOR
{
    UINT        uiVsiIndex;           /* ��Ӧ��VSI����ֵ */
    IF_INDEX    ifIndex;              /* ����ַ��Ӧ�Ľӿ����� */
    INET_ADDR_S stReflectorIpAddr;    /* Reflector IP address */
    UCHAR       aucMacAddress[MAC_ADDR_LEN];     /* Reflector IP address��Ӧ��MAC��ַ */
    USHORT      usSrcPort;            /* Reflector Դ�˿ں� */
    USHORT      usDstPort;            /* Reflector Ŀ�Ķ˿ں� */
    UINT32      auiDrvContext[L2VPN_DRVCONTEXT_MAXLENGTH];/* ��Ʒ���ص���Ϣ */
}L2VPN_REFLECTOR_S;

/* L2VPN ����仯�Ĳ��������� */
typedef enum tagL2VPN_CMD
{
    L2VPN_CMD_ADD,
    L2VPN_CMD_DELETE,
    L2VPN_CMD_MODIFY,
    L2VPN_CMD_PWRECV,   /* ip interworking�·�recv pw������� */
    L2VPN_CMD_ACRECV,   /* ip interworking�·�recv ac������� */
    L2VPN_CMD_MAX
}L2VPN_CMD_E;

/* ��̫��������ض��� */
typedef struct tagL2VPN_VLAN_RANGE
{
    USHORT   usVlanStart;              /* ��ʼvlan */
    USHORT   usVlanEnd;                /* ����vlan */
}L2VPN_VLAN_RANGE_S;

#define L2VPN_VLANSEG_MAXNUM 8         /* VLAN�������Ŀ */

/* ƥ������е�VLAN Tag�б� */
typedef struct tagL2VPN_VLAN_LIST_ENCAP
{
    USHORT             usTPID;
    USHORT             usDot1p;
    USHORT             usRangeNum;         /* ��ʾ����ָ������Ĵ�С */
    L2VPN_VLAN_RANGE_S  *pstVlanRangeList; /* VLAN��ָ������ */
}L2VPN_VLAN_LIST_ENCAP_S;

/* ��Tagƥ����� */
typedef struct tagL2VPN_VLAN_TAG_ENCAP
{
    USHORT             usTPID;
    USHORT             usVlan;
    USHORT             usDot1p;
}L2VPN_VLAN_TAG_ENCAP_S;

/* �ӿڷ���ʵ����װ���� */
#define L2VPN_ENCAP_NONE            0x0     /* δ���÷�װ���� */
#define L2VPN_ENCAP_UNTAGGED        0x01    /* �ޱ�ǩ��װ����*/
#define L2VPN_ENCAP_CTAG            0x02    /* ��һ��C��ǩ  */
#define L2VPN_ENCAP_CTAG_LIST       0x03    /* ��һ��C��ǩ�б�  */
#define L2VPN_ENCAP_STAG            0x04    /* ��һ��S��ǩ��û�б�ǩ��PVIDΪS��ǩʱ��  */
#define L2VPN_ENCAP_STAG_LIST       0x05    /* ��һ��S��ǩ�б��û�б�ǩ��PVIDΪ��S��ǩ�б���ʱ��  */
#define L2VPN_ENCAP_STAG_CTAG       0x06    /* �����㵥����ǩ  */
#define L2VPN_ENCAP_STAG_CTAGLIST   0x07    /* �������ǩ����㵥�����ڲ��б� */
#define L2VPN_ENCAP_STAG_CTAGALL    0x08    /* �������ǩ����㵥�����ڲ�ALL */
#define L2VPN_ENCAP_DEFAULT         0x09    /* ���޷�ƥ����������ʱ,Ĭ��ƥ�� */
#define L2VPN_ENCAP_TAGGED          0x0A    /* ƥ�����д�tag���� */
#define L2VPN_ENCAP_STAG_ONLY_TAGGED  0x0B  /* ��һ��S��ǩ */
#define L2VPN_ENCAP_STAG_LIST_ONLY_TAGGED  0x0C/* ��һ��S��ǩ�б�  */

/* ��̫���ӿ�ƥ����� */
typedef struct tagL2VPN_EVC_ENCAP
{
    UINT                       uiTagEncapType;
    L2VPN_VLAN_LIST_ENCAP_S    stSTagList;
    L2VPN_VLAN_LIST_ENCAP_S    stCTagList;
}L2VPN_EVC_ENCAP_S;

/* ��ǩ�������� */
#define L2VPN_REWRITE_UNKNOWN   0x0         /* ��ЧRewrite���� */
#define L2VPN_REWRITE_NEST_S    0x01        /* ����S��ǩ  */
#define L2VPN_REWRITE_NEST_C    0x02        /* ����C��ǩ  */
#define L2VPN_REWRITE_NEST_SC   0x03        /* ����SC��ǩ  */
#define L2VPN_REWRITE_STRIP_S   0x04        /* ����S��ǩ  */
#define L2VPN_REWRITE_STRIP_C   0x05        /* ����C��ǩ  */
#define L2VPN_REWRITE_STRIP_SC  0x06        /* ����SC��ǩ  */
#define L2VPN_REWRITE_REMARK_1TO1  0x07     /* 1:1��д  */
#define L2VPN_REWRITE_REMARK_1TO2  0x08     /* 1:2��д  */
#define L2VPN_REWRITE_REMARK_2TO1  0x09     /* 2:1��д  */
#define L2VPN_REWRITE_REMARK_2TO2  0x0a     /* 2:2��д  */

/* �����������̫������VLAN Tag��д���� */
typedef struct tagL2VPN_VLAN_REWRITE
{
    UINT                      uiRewriteType;
    L2VPN_VLAN_TAG_ENCAP_S    stSTag;
    L2VPN_VLAN_TAG_ENCAP_S    stCTag;
}L2VPN_VLAN_REWRITE_S;

/* ��д�����ķ��� */
#define L2VPN_REWRITE_INBOUND     0x0001  /* �뷽�� */
#define L2VPN_REWRITE_OUTBOUND    0x0002  /* ������ */

/* ��̫�����ĸ�д���� */
typedef struct tagL2VPN_EVC_REWRITE
{
    UINT     uiDirection;
    BOOL_T   bSymmetric;
    L2VPN_VLAN_REWRITE_S astRewriteEntry[2];
}L2VPN_EVC_REWRITE_S;

/* ��̫��������Ϣ */
typedef struct tagL2VPN_EVC
{
    L2VPN_EVC_ENCAP_S       stEncap;
    L2VPN_EVC_REWRITE_S     *pstRewrite;
}L2VPN_EVC_S;

/* MAC������ӿ����� */
typedef enum tagL2VPN_MAC_TYPE
{
    L2VPN_MAC_LOCAL = 0,     /* ����MAC */
    L2VPN_MAC_MPLSPW,        /* MPLS PWԶ��MAC */
    L2VPN_MAC_MINMPW,        /* MINM Զ��MAC */
    L2VPN_MAC_VXLANTUNNEL,   /* VXLAN��� Զ��MAC */
    L2VPN_MAC_NVGRETUNNEL,   /* NVGRE��� Զ��MAC */
    L2VPN_MAC_ALL,           /* ��������MAC */
}L2VPN_MAC_TYPE_E;

/* MAC����Ĳ�ѯʱЯ�������� */
typedef struct tagL2VPN_MAC_CONDITION
{
    UINT         uiDisplayOpt;
    UINT         uiVsiIndex;      /* VPLSʵ��������ȱʡΪ��ʾ����VPLSʵ����MAC */
    UINT         uiStatus;
    UCHAR        aucMacAddress[6];   /* �������鲥MAC��ַ */
    IF_INDEX     ifIndex;           /* ָ���ӿ����� */
    USHORT       usVlanID;          /* �����VLAN ID */
    UINT         uiLinkID;          /* ָ��link ID */
    L2VPN_MAC_TYPE_E enMacEntryType;
}L2VPN_MAC_CONDITION_S;

#define L2VPN_MAC_MAXNUM 25
/* L2VPN MAC������Ϣ */
typedef struct tagL2VPN_MAC_ENTRY
{
    L2VPN_MAC_TYPE_E enMacEntryType;
    UCHAR      aucMacAddress[6];     /* ����MAC��ַ */
    union
    {
        UINT       uiLinkID;        /* Link ID��ָ��PW����AC */
        IF_INDEX   ifIndexTunnel;   /* Tunnel������ */
    }unEntryID;
    UINT       uiVsiIndex;          /* VPLSʵ������ */
    UINT       uiStatus;            /* �����״̬ */
    UINT       uiAgingTime;         /* �ϻ�ʱ�� */
}L2VPN_MAC_ENTRY_S;

/* MAC����仯�Ĳ��������� */
typedef enum tagL2VPN_MAC_CMD
{
    L2VPN_MAC_ADD,            /* ���MAC*/
    L2VPN_MAC_DELETE,         /* ɾ��MAC*/
    L2VPN_MAC_WITHDRAW,       /* MAC��ַ����*/
    L2VPN_MAC_CMD_MAX
}L2VPN_MAC_CMD_E;

/*  ATM ��Ԫ������ض���  */
typedef struct tagL2VPN_CTR
{
    USHORT   usMaxPackCells;
    UINT     uiMaxPackTime;
}L2VPN_CTR_S;

typedef enum tagL2VPN_CEM_CR
{
    L2VPN_CEM_CR_NONE,      /* ����Ҫʱ�ӻָ� */
    L2VPN_CEM_CR_ACR,       /* ACRʱ�ӻָ���ʽ */
    L2VPN_CEM_CR_DCR,       /* DCRʱ�ӻָ���ʽ */
    L2VPN_CEM_CR_MAX
}L2VPN_CEM_CR_E;

/* TDM ��·������ض��� */
typedef struct tagL2VPN_CEM
{
    UINT           uiJitterBufferSize;
    L2VPN_CEM_CR_E enClockRecoverMode;
    USHORT         usPayloadByte;
    UCHAR          ucIdleCode;
}L2VPN_CEM_S;

/* MPLS PW��װ���ͣ���RFC 4446 �����PW���� */
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

#define    L2VPN_SERVICE_INVALID     0x0       /* ��Чֵ */
#define    L2VPN_SERVICE_STA         0x01      /* ת��ͳ�� */
#define    L2VPN_SERVICE_QOS         0x02      /* QOS */

/* MPLS PW�ϵ����ԣ�ת������ͳ�ơ������緽��CAR�������緽��CAR�ȣ� */
typedef struct tagMPLS_PW_SERVICE
{
    UINT        uiServiceFlag;     /* ��bit���ã���ͬҵ�����ò�ͬλ�õ�bit */
    USHORT      usInCarID;         /* ������෽��CAR ID */
    USHORT      usOutCarID;        /* ������෽��CAR ID */
}MPLS_PW_SERVICE_S;

/* PW���Ա�Ƕ��� */
#define  L2VPN_PW_NORMAL           0x00000000    /* ��ͨPW */
#define  L2VPN_PW_BACKUP           0x00000001    /* ����PW */
#define  L2VPN_PW_UPW              0x00000002    /* �û���PW */
#define  L2VPN_PW_HUBPW            0x00000004    /* HUB��PW */
#define  L2VPN_PW_SPOKEPW          0x00000008    /* SPOKE��PW */
#define  L2VPN_PW_CTRLW            0x00000010    /* PW֧�ֿ����� */
#define  L2VPN_PW_SIGNAL           0x00000020    /* ��·�����������鴫��PW */
#define  L2VPN_PW_FRAGMENT         0x00000040    /* ��·����CESoPSN��PW֧�ַ�Ƭ���� */
#define  L2VPN_PW_CCC              0x00000080    /* Զ��CCC PW */
#define  L2VPN_PW_DUALRECEIVE      0x00000100    /* ����pw֧��˫�չ��� */
#define  L2VPN_PW_SEQUENCING_SEND  0x00000200    /* ���ķ��ͷ��������� */
#define  L2VPN_PW_SEQUENCING_RECV  0x00000400    /* ���Ľ��շ��������� */
#define  L2VPN_PW_RTP_HEADER       0x00000800    /* PW�Ϸ��͵ĵ�·���汨��Я��RTPͷ */
#define  L2VPN_PW_RTP_DIFFMODE     0x00001000    /* PW�Ϸ��͵ĵ�·���汨�ĵ�RTPͷ��ʱ���Ϊ���ģʽ */
#define  L2VPN_PW_STAT             0x00002000    /* ʹ��ͳ�ƣ���PW���� */
#define  L2VPN_PW_BYPASS           0x00004000    /* BYPASS PW */

typedef struct tagL2VFIB_SPEC
{
    UINT   uiVplsVsiNumber;      /* VPLSʵ������ */
    UINT   uiVpwsVsiNumber;      /* VPWSʵ������ */
    UCHAR  ucAcDrvContextLen;
    UCHAR  ucPwDrvContextLen;
    UCHAR  ucCfgIfContextLen;
    UCHAR  ucCfgVsiContextLen;
    UINT   uiAcNumPerVsi;        /* ÿVSI��AC��� */
    UINT   uiPwNumPerVsi;        /* ÿVSI��PW��� */
    BOOL_T bSupportSPB;          /* �Ƿ�֧��SPB */
    BOOL_T bSupportPBB;          /* �Ƿ�֧��PBB */
    BOOL_T bSharedBMAC;          /* VSI�Ƿ���BMAC */
    UINT   uiUCMinMNumber;       /* �����ĵ���MinM������ */
    BOOL_T bSupportVxlan;        /* �Ƿ�֧��VXLAN */
    BOOL_T bSupportNvgre;        /* �Ƿ�֧��NVGRE */
    UCHAR  ucReflectorDrvContextLen;
}L2VFIB_SPEC_S;

/*uiFlag������*/
#define MINM_HEADDUPLICATE  0x0001   /* ͷ�˸��� */
#define MINM_COREDUPLICATE  0x0002   /* ���ĸ��� */
#define MINM_PBB            0x0004   /* PBB */
#define MINM_SPB            0x0008   /* SPB */
#define MINM_UNICAST        0x0010   /* �������� */
#define MINM_MULTICAST      0x0020   /* �鲥���� */
#define MINM_DOUBLEBVLAN    0x0040   /* ˫BVLANģʽ */

#pragma pack(4)
typedef struct tagMinM_Port
{
    IF_INDEX    ifIndex;                                        /* �ӿ����� */
    UINT32      auiIfContext[L2VPN_MINMIF_DRVCONTEXT_LENGTH];   /* �ӿ�DrvContext��Ϣ */
}MINM_PORT_S;
#pragma pack()

#define MINM_AGE            1
#define MINM_CANCEL_AGE     2

/* MinM�ϻ����� */
typedef struct tagMINM_ENTRYINDEX
{
    UCHAR       aucDBMac[6];
    USHORT      usBVlan;
}MINM_ENTRYINDEX_S;

#ifdef __KERNEL__
/* ����MPLS PW������� */
typedef struct tagMPLS_PW_ENTRY
{
    MPLS_PW_ENCAPTYPE_E  enPWEncapType;  /* PW�ķ�װ���� */
    UINT        uiInVCLabel;        /* ���ǩ */
    UINT        uiOutVCLabel;       /* ����ǩ */
    UINT        uiPWFlag;           /* PW���Ա�ǣ��μ�L2VPN_PW_ */
    IF_INDEX    ifLogicIndex;       /* PW�߼��ӿ����� */
    USHORT      usRmtVTag;          /* �Զ�peer��P-Tag */
}MPLS_PW_ENTRY_S;

/* PW�ĳ��������� */
#define MPLS_PW_OUT_NO      0x00   /* VPLS��PW��������usOutLinkID */
#define MPLS_PW_OUT_AC      0x01   /* AC->PW��������Ҫ����usOutLinkID */
#define MPLS_PW_OUT_PW      0x02   /* PW->PW��������Ҫ����usOutLinkID */
#define MPLS_PW_OUT_BYPASS  0x03   /* BYPASS PW PW<->PW ������������usOutLinkID */

/* MPLS PW��Ϣ */
typedef struct tagMPLS_PW
{
    MPLS_PW_ENTRY_S       *pstPWMainEntry;   /* ���õ�����PW */
    MPLS_PW_ENTRY_S       *pstPWBkupEntry;   /* ���õı���PW */
    UINT                  uiVsiIndex;        /* VSI���� */
    USHORT                usLinkID;          /* PW ��link ID��ָ��PW */
    USHORT                usMtu;             /* MTU */
    ULONG                 ulVnHandle;        /* VN�����Handle */
    UINT32                auiVnDrvContext[VN_DRVCONTEXT_LEN]; /* ��һ����Ϣ*/

    MPLS_PW_SERVICE_S     stPwService;
    USHORT                usOutType;
    USHORT                usOutLinkID;
    UINT32                auiContext[L2VPN_DRVCONTEXT_MAXLENGTH];
}MPLS_PW_S;

/* MPLS PW RECV��Ϣ */
typedef struct tagMPLS_PW_RECV
{
    UINT                  uiVsiIndex;        /* VSI���� */
    USHORT                usLinkID;          /* PW ��link ID��ָ��PW */
    UINT                  uiInLabel;         /* PW ���뷽���ǩ */
    L3_ADJACENT_LINK_S    stL3ADJLink;       /* AC����·���װ��Ϣ */
}MPLS_PW_RECV_S;

/* L2VPN AC RECV������Ϣ */
typedef struct tagL2VPN_AC_RECV
{
    UINT                uiVsiIndex;        /* VSI���� */
    IF_INDEX            ifIndex;           /* ����AC��Ӧ�Ľӿ����� */
    USHORT              usLinkID;          /* AC��Ӧ��Link ID */
    USHORT              usServiceID;       /* AC��Ӧ��Service ID */
    L3_ADJACENT_LINK_S  stL3ADJLink;       /* AC����·���װ��Ϣ */
}L2VPN_AC_RECV_S;

/* ���������Ƿ�ͬ����Ƕ��� */
#define L2VPN_DRVFLAG_NOSYNC    0x00000000
#define L2VPN_DRVFLAG_SYNC      0x00000001

/* L2VPN_IOCTL_SYNC_START/ENDʱ�������VSI */
#define L2VPN_SYNC_ALL_VSI      0xFFFFFFFF

/* VSI���ͱ�ǣ���ȡָ������VSI��MAC���� */
#define L2VPN_VSI_TYPEFLAG_ALL     0x0     /* �������� */
#define L2VPN_VSI_TYPEFLAG_LOCAL   0x1     /* ����MAC���  */
#define L2VPN_VSI_TYPEFLAG_REMOTE  0x2     /* Զ��MAC��� */
#define L2VPN_VSI_TYPEFLAG_VXLAN   0x4     /* VXLAN���λ */
#define L2VPN_VSI_TYPEFLAG_VPLS    0x8     /* VPLS���λ  */
#define L2VPN_VSI_TYPEFLAG_MINM    0x10    /* MINM���λ  */
#define L2VPN_VSI_TYPEFLAG_NVGRE   0x20    /* NVGRE���λ  */
/* ����ͳ�ƶ���:PW */
typedef struct tagL2VPN_STATISTICS_OBJECT_PW
{
    MPLS_PW_ENCAPTYPE_E  enPWEncapType;  /* PW�ķ�װ���� */
    UINT                 uiVsiIndex;     /* PW������VSI Index */
    USHORT               usLinkID;       /* PW��LINK ID */
    UINT                 uiPWFlag;       /* PW���Ա�� */
    UINT32               auiContext[L2VPN_DRVCONTEXT_MAXLENGTH]; /* PW��DrvContext */
}L2VPN_STATISTICS_OBJECT_PW_S;

/* ARP/ND��������CPU���� */
typedef enum tagL2VPN_FORWARD_CTL
{
    L2VPN_ARP_CONTROL,                  /* ����ARP���Ŀ�����Ϊ������VSI���� */
    L2VPN_ND_CONTROL,                   /* ����ND���Ŀ�����Ϊ������VSI���� */
    L2VPN_FORWARD_BUTT
}L2VPN_FORWARD_CTL_E;

typedef enum tagL2VPN_ACTION
{
    L2VPN_ACTION_FORWARD,               /* ����ת�����ģ�����ȱʡ״̬ */
    L2VPN_ACTION_REDIRECT_TO_CPU,       /* �������ض���CPU */
    L2VPN_ACTION_BUTT
}L2VPN_ACTION_E;

/* �����ӿں���ָ�� */
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

/* ���Գ�ʼ��L2VPN�������ӿں��� */
ULONG L2VPN_Register_Driver( IN ULONG ulVersion, IN L2VPN_DRV_INTF_S *pstL2VPNDrvIntf);
ULONG L2VFIB_Register_Spec(IN L2VFIB_SPEC_S *pstL2vfibSpec);

/* ע��MPLS PW���� */
ULONG L2VPN_Register_MplsPW(VOID);
ULONG L2VPN_Register_MplsPWDriver(VOID);

/* ע��Overlay���� */
ULONG L2VPN_Register_Overlay(VOID);

typedef enum tagL2VPN_ENTRYTYPE
{
    L2VPN_AC,                /* AC���� */
    L2VPN_MPLSPW,            /* MPLS PW���� */
    L2VPN_MINMPW,            /* MAC-in-MAC PW���� */
}L2VPN_ENTRYTYPE_E;

/* ����VSI������LinkID����ȡ��Ӧ�ı�����Ϣ */
ULONG L2VPN_GetAcDrvInfo(IN UINT uiVsiIndex, IN USHORT usLinkID, OUT L2VPN_AC_S *pstAcData);
ULONG L2VPN_GetMplsPwDrvInfo(IN UINT uiVsiIndex, IN USHORT usLinkID, OUT MPLS_PW_S *pstPwData);

/* ���ݽӿ������ͷ���ʵ����ȡAC��Ϣ */
ULONG L2VPN_GetAcDrvByIf(IN IF_INDEX ifIndex, IN USHORT usSrvID, OUT L2VPN_AC_S *pstAcData);

/* L2VPN����L3VPN���Ը�������1��VE��������ȡ��һ�����һ�µ�VE������ */
IF_INDEX L2VPN_GetVEIfIndex(IN IF_INDEX ifIndex);

typedef ULONG (*VSI_TUNNEL_VISIT_PF)(IN IF_INDEX ifIndex, IN UINT uiVsiIndex, IN VOID *);

/* Tunnel�����һ���仯����Ʒ��Ҫ���Tunnel����������VSI��Ӧ��ת����Ϣ */
VOID L2VPN_ScanVsiByTunnel(IN IF_INDEX ifIndexOfTunnel,
                         IN const VSI_TUNNEL_VISIT_PF pfFunVisit,
                         IN VOID *pUserData);

/* ����VsiIndex��ȡVsiName */
ULONG L2VPN_GetVsiNameByIndex(IN UINT uiVsiIndex, IN ULONG ulBufLen, OUT CHAR *pcVsiName);

/* ֪ͨL2VPN���������·����� */
ULONG L2VPN_ReloadCfg(VOID);
/* ֪ͨVPLS AC���������·����� */
ULONG L2VPN_ReloadVplsAc(VOID);
/* ֪ͨVPLS PW���������·����� */
ULONG L2VPN_ReloadVplsPw(VOID);
/* ֪ͨVPWS AC���������·����� */
ULONG L2VPN_ReloadVpwsAc(VOID);
/* ֪ͨVPWS PW���������·����� */
ULONG L2VPN_ReloadVpwsPw(VOID);
/* ֪ͨMINM���������·����� */
ULONG L2VPN_ReloadMinM(VOID);
/* ֪ͨOVERLAYTUNNEL�������������� */
ULONG L2VPN_ReloadOverlayTunnel(VOID);
/* ֪ͨREFLECT���������·����� */
ULONG L2VPN_ReloadReflector(VOID);

/*************************MINM������ݽṹ����***********************************/
typedef enum tagMINM_CMD
{
    MINM_CMD_ADD,              /* ��ӱ��� */
    MINM_CMD_DEL,              /* ɾ������ */
    MINM_MC_ADDOUTIF,          /* �鲥�����������ɳ��ӿ� */
    MINM_MC_DELOUTIF,          /* �鲥����ɾ�����ɳ��ӿ� */
    MINM_UC_MODIFYOUTIF,       /* ���������޸ĳ��ӿ� */
    MINM_UC_MODIFYMODE,        /* SPB���������޸��鲥����ģʽ */
    MINM_ECT_ADD,              /* ECTǨ�ƹ����У�����SPB�������� */
    MINM_ECT_MODIFY,           /* ECTǨ�ƹ����У��޸�SPB����������˿����� */
    MINM_ECT_DEL,              /* ECTǨ�ƹ����У�ɾ��SPB�������� */
    MINM_CMD_BUTT              /* ��Чֵ */
}MINM_CMD_E;

typedef struct tagMINM_MC
{
    UINT        uiVsiIndex;                /* VSI���������ڱ����в���עVSI�Ĳ�Ʒ����Ʒֱ�Ӻ��� */
    UINT        uiISID;                    /* ����ʵ���� */
    USHORT      usFlag;                    /* ������ */
    UCHAR       aucDBMac[MAC_ADDR_LEN];    /* �Ǹ���Ŀ��MAC���õ�ַΪ�鲥��ַ */
    UCHAR       aucSBMac[MAC_ADDR_LEN];    /* �Ǹ���ԴBMAC�������豸��MACtoME��ַ */
    UINT        uiBVlanID;       /* �Ǹ���VLAN ID */
    UINT        uiPortNum;       /* ���˿ڸ��� */
    MINM_PORT_S *pstPortList;     /* ����ĳ��ӿ��б�������Ӧ��DrvContext��Ϣ */
    UINT32      auiContext[L2VPN_MINM_DRVCONTEXT_LENGTH];  /* ���������ġ������ֶΣ���Ʒ�������������Ҫʹ�� */
}MINM_MC_S;

typedef struct tagMINM_UCIF
{
    UINT        uiBVlanID;         /* �Ǹ���VLAN ID */
    IF_INDEX    ifIndex;           /* �ӿ����� */
    UINT32      auiIfContext[L2VPN_MINMIF_DRVCONTEXT_LENGTH];   /* �ӿ�DrvContext��Ϣ */
}MINM_UCIF_S;

typedef struct tagMINM_UC
{
    UINT          uiVsiIndex;               /* VSI���������ڱ����в���עVSI�Ĳ�Ʒ����Ʒֱ�Ӻ��� */
    UINT          uiISID;                   /* ����ʵ���� */
    USHORT        usLinkID;                 /* MinM�������������MarvelоƬ��VSI�ڲ������������Һ�VPLS��Link ID��ͬһ�ռ䡣BCMоƬ������ */
    USHORT        usFlag;                   /* ������ */
    UCHAR         aucDBMac[MAC_ADDR_LEN];   /* �Ǹ���Ŀ��MAC */
    UCHAR         aucSBMac[MAC_ADDR_LEN];   /* �Ǹ���ԴBMAC�������豸��MACtoME��ַ */
    UCHAR         aucSCMac[MAC_ADDR_LEN];   /* �û�ԴMAC */
    MINM_UCIF_S   stPort;                   /* ���˿� */
    UINT32        auiContext[L2VPN_MINM_DRVCONTEXT_LENGTH]; /* ���������ġ������ֶΣ���Ʒ�������������Ҫʹ�á���ʼΪȫF */
}MINM_UC_S;

typedef struct tagMINM_ECT
{
    UINT        uiVsiIndex;               /* VSI���������ڱ����в���עVSI�Ĳ�Ʒ����Ʒֱ�Ӻ��� */
    UINT        uiISID;                   /* ����ʵ����*/
    USHORT      usLinkID;                 /* MinM�������������MarvelоƬ��VSI�ڲ������������Һ�VPLS��Link ID��ͬһ�ռ䡣BCMоƬ������ */
    USHORT      usFlag;                   /* ������ */
    UCHAR       aucDBMac[MAC_ADDR_LEN];   /* �Ǹ���Ŀ��MAC */
    UCHAR       aucSBMac[MAC_ADDR_LEN];   /* �Ǹ���ԴBMAC�������豸��MACtoME��ַ */
    MINM_UCIF_S stPort;                   /* �˿��ܶ�MINM����ִ���շ������� */
    MINM_UCIF_S *pstPortRecvOnly;         /* �˿ڽ��ܶ�MINM����ִ���հ����� */
    UINT32      auiContext[L2VPN_MINM_DRVCONTEXT_LENGTH]; /* ���������ġ������ֶΣ���Ʒ�������������Ҫʹ�á���ʼΪȫF */
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

/* ע��CEM */
ULONG L2VPN_Register_Cem(VOID);

#endif

#ifdef __cplusplus
}
#endif /* end of __cplusplus */

#endif
