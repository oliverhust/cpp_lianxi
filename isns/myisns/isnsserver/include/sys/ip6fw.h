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
 *             �궨��                               *
 ****************************************************/
/* ����MID */


/* ����IP6ת��ģ��ʹ�õ�ȫ��֪���˿� */





/* CIOCTL���� */

#define IP6FW_CIOCTL_CMD_PROC     CIOCTL_CMD_DEF(CIOCTL_CMDMOD_IP6FW, 1)
#define IP6FW_CIOCTL_CMD_SMOOTH   CIOCTL_CMD_DEF(CIOCTL_CMDMOD_IP6FW, 2)
#define IP6FW_CIOCTL_CMD_BATCHIF  CIOCTL_CMD_DEF(CIOCTL_CMDMOD_IP6FW, 3)
#define IP6FW_CIOCTL_CMD_BATCHGLBBASE   CIOCTL_CMD_DEF(CIOCTL_CMDMOD_IP6FW, 4)
#define IP6FW_CIOCTL_CMD_BATCHGLBALL    CIOCTL_CMD_DEF(CIOCTL_CMDMOD_IP6FW, 5)


/* DUMP���ں������� */

#define IP6FW_DUMP_CMD_SHOW       DUMP_CMD_DEF(DUMP_CMDMOD_IP6FW, 1)

/* MBUF͸��ͨ������ */
#define MBUF_RELAY_TYPE_IP6_DELIVERTOUP  10
#define MBUF_RELAY_TYPE_IP6_IFOUTPUT     11

/* �������ͺ���������� */
#define IP6_FLOWINFO_MASK       0x0fffffff  /* flow info (28 bits) */
#define IP6_FLOWLABEL_MASK      0x000fffff  /* flow label (20 bits) */

/* IPv6���İ汾 */
#define IPV6_VERSION         0x06

/* �ӿ���ȱʡ��IPv6 MTU��ֵ, Ŀǰ���нӿ���ȱʡMTU����ͬ�� */
#define IP6_DEFAULT_MTU      1500
/* �ӿ��Ͽ����õ���СIPv6 MTU */
#define IP6_MIN_MTU          1280
/* �ӿ��Ͽ����õ����IPv6 MTU */  
#define IP6_MAX_MTU          10240


/* IPv6�����غɵ���󳤶� */
#define IPV6_MAXPACKET      65535

#define IP6F_OFF_MASK       0xfff8  /* mask out offset from _offlg */
#define IP6F_RESERVED_MASK  0x0006  /* reserved bits in ip6f_offlg */
#define IP6F_MORE_FRAG      0x0001  /* more-fragments flag */

#define IP6_REASS_MODE_GLOBAL    0
#define IP6_REASS_MODE_LOCAL     1

/****************************************************
 *             ���Ľṹ                             *
 ****************************************************/
/* IPv6 ����ͷ */
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
 *             ����������ݶ���                     *
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

/* ת����Ϣ����,�����û�̬���ں�̬����,���û�̬�������ͬ�� */ 
ISSU typedef enum tagIP6_MsgType
{
    IP6_MSGTYPE_CFGMSG,                 /* ����������Ϣ */
    IP6_MSGTYPE_BATCHIFCFG,             /* �����ӿ�������Ϣ */
    IP6_MSGTYPE_BATCHGLBCFG,            /* ����ȫ��������Ϣ */
    IP6_MSGTYPE_SMOOTH_START,           /* ƽ����ʼ��Ϣ */
    IP6_MSGTYPE_SMOOTH_END,             /* ƽ��������Ϣ */    
    IP6_MSGTYPE_BATCHGLBCFGALL,
    IP6_MSGTYPE_BUTT
} IP6_MSG_TYPE_E;

/* ת��������Ϣ���� */
ISSU typedef enum tagIP6_CfgMsgType
{
    IP6_CFGMSG_SHOWSTAT,        /* IPv6ͳ����Ϣ��ʾ */
    IP6_CFGMSG_CLEARSTAT,       /* ���IPv6ͳ����Ϣ */    
    IP6_CFGMSG_SETDEBUG,        /* ����IPv6������Ϣ���� */
    IP6_CFGMSG_GETDEBUG,        /* ��ȡIPv6������Ϣ���� */
    IP6_CFGMSG_SHOWIFSTAT,      /* IPv6�ӿ�ͳ����Ϣ��ʾ */  
    IP6_CFGMSG_SETIP6MTU,       /* ���ýӿ�ipv6 mtu */
    IP6_CFGMSG_GETIP6MTU,       /* ��ȡ�ӿ�ipv6 mtu */
    IP6_CFGMSG_SETIP6ECMP,      /* ����ipv6 ecmp */
    IP6_CFGMSG_GETIP6ECMP,      /* ��ȡipv6 ecmp */
    IP6_CFGMSG_GETIFICMP6STAT,  /* ��ȡICMP6�ӿ�ͳ����Ϣ */
    IP6_CFGMSG_GETMIBIFSTAT,    /* ��ȡMIB IPv6�ӿ�ͳ����Ϣ��ʾ */  
    IP6_CFGMSG_GETMIBSTAT,      /* ��ȡMIB IPv6ȫ��ͳ����Ϣ��ʾ */ 
    IP6_CFGMSG_SETOPTIONDROP,   /* ����ipv6 option drop ���� */
    IP6_CFGMSG_SETREASSMODE,    /* ��������ģʽ */
    IP6_CFGMSG_GETBPASTAT,      /* ��ȡBPAͳ����Ϣ */
    IP6_CFGMSG_BUTT
} IP6_CFGMSG_TYPE_E;

/* ��Ҫ�������ݵ�Ӧ��ģ������ */
typedef enum tagIp6fw_Mod_ID
{
    IP6FW_MOD_BPA,             /* BPAģ��ID*/
    IP6FW_MOD_SMA,             /* SMAģ��ID*/
    IP6FW_MOD_BUTT            
} IP6FW_MODID_E;

/* ת��������Ϣ�ṹ */
ISSU typedef struct tagIP6_MsgHead
{
    USHORT      usType;         /* ת��������Ϣ���� */
    USHORT      usMsgLen;       /* ת��������Ϣ����,���Ȱ���ͷ�� */
} IP6_MSG_HEAD_S;

ISSU typedef struct tagIP6_CFG_DEBUG
{
    UINT        uiType;
    UINT        uiEnable;
    UINT        uiAcl;
}IP6_CFG_DEBUG_S;

ISSU typedef struct tagIP6_CFG_MTU
{
    IF_INDEX    ifIndex;          /* �ӿ����� */
    UINT        uiIP6MTU;         /* ���õĽӿ�Э��MTU */
} IP6_CFG_MTU_S;

ISSU typedef struct tagIP6_CFG_ECMP
{
    UINT        uiCurrEcmpNum;   /* ��ǰECMPֵ */
    UINT        uiNextEcmpNum;   /* �´�������ECMPֵ */
} IP6_CFG_ECMP_S;

ISSU typedef struct tagIP6_CFG_REASS
{
    UINT        uiIP6ReassMode;
} IP6_CFG_REASSMODE_S;

typedef struct tagIP6_CFG_OPTION
{
    UINT        uiIP6OptionDropFlag;    /* ipv6 option drop ���λ */
} IP6_CFG_OPTION_S;

/* ��ʾ DEBUG ����״̬ */
ISSU typedef struct tagIP6_DebugInfo
{
    UINT        uiIP6DbgPktACL; 
    UCHAR       ucIP6PktDebugFlag;
    UCHAR       ucIP6ErrDebugFlag;         
} IP6_DEBUG_INFO_S;

/* ��;: �洢DBM�Ľӿ����ݽṹ,ͬ�������ӿ���Ϣ�ṹ,�����ӿ����ں˽ṹ */
ISSU typedef struct tagIP6_IFSYNDATA
{
    CHAR        szIfName[IF_MAX_NAME_LEN + 1];   /* ��DBM��ʹ�� */
    IF_INDEX    ifIndex;                         /* �ӿ�����, ͬ����������ʹ�� */
    UINT        uiIP6MTU;                        /* �ӿ�Э��MTU */
} IP6_IFSYNDATA_S;

ISSU typedef struct tagIP6_GLBBASEDATA
{
    UINT        uiIP6ECMP;                      /* Ipv6���ECMP�� */
} IP6_GLBBASEDATA_S;

ISSU typedef struct tagIP6_GLBSYNDATA
{
    UINT        uiIP6ECMP;                       /* Ipv6���ECMP�� */
    UINT        uiIP6OptionDropFlag;             /* ipv6 option drop ���λ*/
    UINT        uiIsGlbDBMRecover;               /* �ж��Ƿ��Ƕ����ƻָ����µ�ƽ�� */
} IP6_GLBSYNDATA_S;

/* ͳ����Ϣ���ӿ�ͳ��ȫ��ͳ�ƹ��� */
ISSU typedef struct tagIP6_Stat
{
    UINT64  uiTotalReceived;    /* �յ���IPv6�������� */
    UINT64  uiInForwDatagrams;  /* �յ���Ŀ�ĵ�ַ���Ǳ���IPv6��ַ��IPv6���� */
    UINT64  uiForward;          /* �ɹ�ת����IPv6���� */
    UINT64  uiOutTransmits;     /* �����ύ����·���IPv6������ */
    UINT  uiTooShort;           /* �յ���IPv6���ĳ��Ȳ���40�ֽڸ��� */
    UINT  uiTooSmall;           /* �յ��Ľضϵ�IPv6���ĸ��� */
    UINT  uiHopLimitExceed;     /* �յ���������������IPv6������ */
    UINT  uiBadHeader;          /* �յ���ͷ������IPv6������ */
    UINT  uiBadOption;          /* �յ���ѡ�����IPv6������ */
    UINT  uiFragments;          /* �յ�IPv6��Ƭ������ */
    UINT  uiReassembled;        /* IPv6����ɹ������� */
    UINT  uiFragDropped;        /* ������IPv6��Ƭ������ */
    UINT  uiFragTimeout;        /* ��ʱ��������ʧ�ܴ��� */
    UINT  uiNoProto;            /* �ϲ�Э�鲻ʶ������ */
    UINT  uiDelivered;          /* ���ͱ���IPv6������ */
    UINT  uiNoRoute;            /* ת���в�FIB6ʧ�ܱ�IPv6���� */
    UINT  uiDstAddrErr;         /* �յ���Ŀ�ĵ�ַ�Ƿ�IPv6������ */
    UINT  uiOFailedFragments;   /* ����ʱ�����ķ�Ƭ������ */
    UINT  uiLocalOut;           /* �����ɹ����͵ķǷ�ƬIPv6������ */
    UINT  uiTooBig;             /* ����ʱ��Ҫ��Ƭ���ǲ������Ƭ������IPv6������ */
    UINT  uiFragmented;         /* �ɹ�����Ƭ��IPv6������ */
    UINT  uiOFragments;         /* �ɹ�����IPv6��Ƭ������ */
    UINT  uiNotMember;          /* �յ��ķ���ӿ������鲥���IPv6������ */
    UINT  uiInMulticasts;       /* �յ��鲥IPv6������ */
    UINT  uiOutMulticasts;      /* ����IPv6�鲥������ */
    UINT  uiInDiscards;         /* ����������ԭ���������� */
    UINT  uiOutDiscards;        /* ���������ԭ���������� */
    UINT  uiOutNoRoutes;        /* IPv6����������·�ɱ�ʧ�ܵı����� */
    UINT  uiOutFragReqds;       /* IPv6���ͱ���ʱ��Ҫ��Ƭ�ı����� */
    UINT  uiOutRequests;        /* IPv6������ύ�������ı����� */
}IP6_STAT_S;

ISSU typedef struct tagIP6_StatShow
{
    IP6_STAT_S   stIP6Stat;
    ICMP6_STAT_S stICMP6Stat;
    RIP6_STAT_S  stRIP6Stat;
} IP6_STAT_SHOW_S;

/* IPv6αͷ�ṹ���� */
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
 *             �궨��                               *
 ****************************************************/
/* ����ĺ����ڵ��ú���IP6_Outputʱ���ò���lFlag */
#define IP6_UNSPECSRC              0x00000001   /* ��ʾ�����ı���Դ��ַΪ0 */
#define IP6_USEMINMTU              0x00000002   /* ��ʾ����С��1280������Ƭ���� */
#define IP6_DONT_FRAG              0x00000004   /* ��ʾ�������Ƭ */
#define IP6_NEXTHOP_SPECIFIED      0x00000008   /* ��ʾָ����һ������ */
#define IP6_ISINTERMEDIATE         0x00000010   /* ��ʾNAT-PT���� */
#define IP6_PROXYOUTPUT            0x00000020   /* ��ʾ�����ڱ��巢�� */

/* ָ��ת������ȱʡ����ĵ�һ��ҵ�� */
#define IP6_DEFAULT_SERVICEBITMAP  ((ULONG)-1)

/* ҵ���־λ Bitmap ��ֵ������ */
#define IP6_SERVICE_SETBITMAP(pulBitmapM, uiServiceIDM) \
    __set_bit((BITS_PER_LONG - 1) - uiServiceIDM, pulBitmapM);

#define IP6_SERVICE_CLEARBITMAP(pulBitmapM, uiServiceIDM) \
    __clear_bit((BITS_PER_LONG - 1) - uiServiceIDM, pulBitmapM);


/****************************************************
 *             ö�ٶ���                             *
 ****************************************************/

/* IPv6ת��ҵ����ձ�����ڽ׶� */
typedef enum tagIP6FW_PreAll
{
    IP6FW_PRE_ALL_ATKPROXY, 
    IP6FW_PRE_ALL_BUTT
} IP6FW_PRE_ALL_SERVICE_ID_E;

/* IPv6ת��ҵ�����뷽��ҵ��ID */
typedef enum tagIP6FW_PreRoutingServiceId
{
    IP6FW_PRE_ROUTING_TCPREASSEMBLE, /* TCP���� */
    IP6FW_PRE_ROUTING_NETSTREAM,
    IP6FW_PRE_ROUTING_MGROUP,           /* Mirror Groupҵ�� */
    IP6FW_PRE_ROUTING_IPSEC,            /* IPsec Ԥ���� */
    IP6FW_PRE_ROUTING_FLOWMGR,
    IP6FW_PRE_ROUTING_INTERZONE,     /*��䴦��*/
    IP6FW_PRE_ROUTING_SMA,           /* SMAҵ�� */
    IP6FW_PRE_ROUTING_ATK,
    IP6FW_PRE_ROUTING_LBPRE,
    IP6FW_PRE_ROUTING_IPOE,
    IP6FW_PRE_ROUTING_WEBREDIRECT,
    IP6FW_PRE_ROUTING_PORTAL,
    IP6FW_PRE_ROUTING_TWAMP,            /* TWAMPҵ�� */    
    IP6FW_PRE_ROUTING_APR,           /* Э��ʶ�� */
    IP6FW_PRE_ROUTING_APPSTAT,       /* ����ͳ�� */
    IP6FW_PRE_ROUTING_CONNLMT,       /* ���������� */
    IP6FW_PRE_ROUTING_BPASTAT,          /* ����BGP���Ե�����ͳ�� */ 
    IP6FW_PRE_ROUTING_AUTHORIZE,     /* ��Ȩ��鴦��׶� */
    IP6FW_PRE_ROUTING_FILTER,           /* PFLT �Ĵ���׶� */
    IP6FW_PRE_ROUTING_LB,               /* LB ���ؾ��⴦��׶� */
    IP6FW_PRE_ROUTING_QPPB,             /* QPPB */
    IP6FW_PRE_ROUTING_PRIMAP,
    IP6FW_PRE_ROUTING_QOS,              /* QoS */
    IP6FW_PRE_ROUTING_AFT,           /* ��ַת�� */
    IP6FW_PRE_ROUTING_BUTT
}IP6FW_PRE_ROUTING_SERVICE_ID_E;

/* ���ձ��Ĳ�ѯ·�ɺ���׶� */
typedef enum tagIp6fw_PostRouting
{
    IP6FW_POST_ROUTING_PBR,
    IP6FW_POST_ROUTING_LB,
    IP6FW_POST_ROUTING_LISP,
    IP6FW_POST_ROUTING_OFP,
    IP6FW_POST_ROUTING_WAAS,
    IP6FW_POST_ROUTING_BUTT
} IP6FW_POST_ROUTING_SERVICEID_E;

/* IPv6�������ͱ�������ǰҵ��ID */
typedef enum tagIP6FW_LocalInBeforeDefragServiceId
{
    IP6FW_LOCAL_IN_INTERZONE, /* ��䴦��׶� */
    IP6FW_LOCAL_IN_ATK,
    IP6FW_LOCAL_IN_CONNLMT,          /* ���������� */
    IP6FW_LOCAL_IN_APPL7,
    IP6FW_LOCAL_IN_BEFOREDEFRAG_BUTT
}IP6FW_LOACL_IN_BEFOREDEFRAG_SERVICE_ID_E;

/* IPv6ת��ҵ�������ͱ���ҵ��ID */
typedef enum tagIP6FW_LocalInServiceId
{
    IP6FW_LOCAL_IN_GTSM,
    IP6FW_LOCAL_IN_BFD,
    IP6FW_LOCAL_IN_TCP_PROXY,
    IP6FW_LOCAL_IN_BUTT
}IP6FW_LOACL_IN_SERVICE_ID_E;

/* IPv6ת��ҵ������������ҵ��ID */
typedef enum tagIP6FW_LocalOutServiceId
{
    IP6FW_LOCAL_OUT_INTERZONE,       /* ��䴦��׶� */
    IP6FW_LOCAL_OUT_WAAS,
    IP6FW_LOCAL_OUT_BUTT
}IP6FW_LOCAL_OUT_SERVICE_ID_E;

/* �������ͱ��Ĳ�ѯ·�ɺ���׶� */
typedef enum tagIp6fw_LocalOutPostRouting
{
    IP6FW_LOCAL_OUT_POST_ROUTING_PBR,
    IP6FW_LOCAL_OUT_POST_ROUTING_LISP,
    IP6FW_LOCAL_OUT_POST_ROUTING_OFP,
    IP6FW_LOCAL_OUT_POST_ROUTING_BUTT
} IP6FW_LOCAL_OUT_POST_ROUTING_SERVICEID_E;

/* IPv6ת��ҵ������ӿڷ���ҵ��ID */
typedef enum tagIP6FW_PostRoutingServiceId
{
    IP6FW_POST_ROUTING_BEFOREFRAG_FLOWMGR,      /* ��������ҵ�� */
    IP6FW_POST_ROUTING_BEFOREFRAG_FILLTAG,      /* ������������� */
    IP6FW_POST_ROUTING_BEFOREFRAG_INTERZONE,     /* ��䴦��׶� */
    IP6FW_POST_ROUTING_BEFOREFRAG_LB,           /* LB ���ؾ��⴦��׶� */
    IP6FW_POST_ROUTING_BEFOREFRAG_ATK,           /* ATK */
    IP6FW_POST_ROUTING_BEFOREFRAG_SMA,          /* SMAҵ�� */
    IP6FW_POST_ROUTING_BEFOREFRAG_FILTER,       /* PFLT �Ĵ���׶� */
    IP6FW_POST_ROUTING_BEFOREFRAG_AUTHORIZE,     /* ��Ȩ��鴦��׶� */
    IP6FW_POST_ROUTING_BEFOREFRAG_CONNLMT,       /* ���������� */
    IP6FW_POST_ROUTING_BEFOREFRAG_APR,           /* Э��ʶ�� */
    IP6FW_POST_ROUTING_BEFOREFRAG_APPSTAT,       /* ����ͳ�� */
    IP6FW_POST_ROUTING_BEFOREFRAG_APPL7,
    IP6FW_POST_ROUTING_BEFOREFRAG_BPASTAT,      /* ����BGP���Ե�����ͳ�� */
    IP6FW_POST_ROUTING_BEFOREFRAG_PORTAL,
    IP6FW_POST_ROUTING_BEFOREFRAG_IPOE,
    IP6FW_POST_ROUTING_BEFOREFRAG_PREQOS,       /* QoSԤ���� */
    IP6FW_POST_ROUTING_BEFOREFRAG_TWAMP,    
    IP6FW_POST_ROUTING_BEFOREFRAG_IPSEC,        /* IPsec ҵ��� */
    IP6FW_POST_ROUTING_BEFOREFRAG_NETSTREAM,
    IP6FW_POST_ROUTING_BEFOREFRAG_MGROUP,       /* Mirror Group */
    IP6FW_POST_ROUTING_BEFOREFRAG_WAAS,
    IP6FW_POST_ROUTING_BEFOREFRAG_OFP,          /* OpenFlow */
    IP6FW_POST_ROUTING_BEFOREFRAG_BUTT
}IP6FW_POST_ROUTING_SERVICE_ID_E;

/* ����͸��ǰ�׶� */
typedef enum tagIP6FW_PreRelayServiceId
{
    IP6FW_PRE_RELAY_INTERZONE,  /* ��䴦��׶� */
    IP6FW_PRE_RELAY_CONNLMT, /* ���������� */
    IP6FW_PRE_RELAY_APPL7,
    IP6FW_PRE_RELAY_BUTT
} IP6FW_PRE_RELAY_SERVICEID_E;

/* ����͸�����ս׶� */
typedef enum tagIP6FW_RelayReceiveServiceId
{
    IP6FW_RELAY_RECEIVE_INTERZONE, /* ��䴦��׶� */
    IP6FW_RELAY_RECEIVE_BUTT
} IP6FW_RELAY_RECEIVE_SERVICEID_E;

/* �鲥ѡ��������ķ���ʱʹ�� */
typedef struct tagIP6_Moptions
{
    IF_INDEX ifIP6MIndex;            
    UCHAR ucIP6MHlim;              
    UCHAR ucIP6MLoop;                  
}IP6_MOPTIONS_S;

/* IP6 ת��ͳ�����Ͷ��� */
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
 *             ����ԭ��                             *
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
/* FPGA: ֧��ת����Ŀ���� */
extern UINT  IP6_GetAndIncrIfSeqNo(IN UINT uiPhase, IN IF_INDEX ifIndex);
extern UINT IP6_GetIfCfgSeqNo(IN UINT uiPhase, IN IF_INDEX ifIndex);

extern VOID IP6_IfStat_Inc(IN IF_INDEX ifIndex, IN IP6STAT_TYPE_E enIp6StatType);

extern VOID IP6_Stat_Inc (IN IP6STAT_TYPE_E enIp6StatType);

extern VOID IP6_Stat_Dec (IN IP6STAT_TYPE_E enIp6StatType);

extern VOID IP6_FirstFragInOrder_Enable(UCHAR ucServiceBit);
extern VOID IP6_FirstFragInOrder_Disable(UCHAR ucServiceBit);
extern UCHAR IP6_FirstFragInOrder_GetStatus(VOID);

/* ������չͷ�����IPv6ͷ��ƫ�ƺ���չͷ���ȣ���ȡ��չͷָ��.
   uiOff, ��չͷ�����IPv6ͷ��ƫ��;
   uiLen, ��չͷ��Ҫ���������ĳ���
   uiMid, ģ��ID */
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
/* �ṩ���ⲿģ��ĺ꣬ͬʱ�Խӿ�ͳ�ƺ�ȫ��ͳ�Ƽ�1 */
#define IP6_INCREASE_STAT(ifIndexM, FieldM) \
{\
    IP6_STAT_S *pt = (IP6_STAT_S *)0x1UL;\
    IP6_UpdateStat((ifIndexM), (ULONG)((UCHAR *)&pt->FieldM-(UCHAR *)pt), sizeof(pt->FieldM), 1);\
}

/* �ṩ���ⲿģ��ĺ꣬ͬʱ�Խӿ�ͳ�ƺ�ȫ��ͳ�Ƽ�1 */
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

/* �ӿ�ת�����ݲ�ѯ */
extern ULONG IP6_GetFWDData(IN IF_INDEX ifIndex,
                            IN IP6FW_MODID_E enFWDType,
                            OUT ULONG *pulRetValue);

/* �ӿ�ת���������� */
extern ULONG IP6_SetFWDData(IN IF_INDEX ifIndex,
                            IN IP6FW_MODID_E enFWDType,
                            IN ULONG ulSetValue);

extern VOID IP6_GetServiceNodeAndIFType(IN IF_INDEX ifIndex, OUT LIP_ADDR *pstLipNode, OUT UINT *puiIfType);

extern ULONG IP6_SetBPA(IN IF_INDEX ifIndex, IN const IP_BPA_BASE_S *pstBpaBase);
extern VOID IP6_SetLoadSharing(IN UINT uiLoadShareMode, IN UINT uiOptionBitMap);
extern ULONG IP6_GetFwdIndex(IN const MBUF_S *pstMBuf, OUT UINT32 *puiFwdIndex);
extern UINT32 IP6_GetFwdMode(VOID);
extern VOID IP6_SetLocalFirst(UINT uiLocalFirstCfg);

#else /* �����û�̬�ṩ */

extern ULONG IP6_GetBpaStat(IN IF_INDEX ifIndex, OUT IP_BPA_STAT_S *pstStat);

extern ULONG IP6_GetMibStat (IN LIP_ADDR stLipNode, OUT IP6_STAT_S *pstIp6MibStat);

/* ��ʾ�ӿ���Ϣʱ��ת��ģ���ȡ�ӿ�ת��ͳ�� */
extern ULONG IP6_GetIfStat(IN IF_INDEX ifIndex, IN UINT uiCmd, OUT IP6_STAT_S *pstIP6Stat)      ;

extern ULONG IP6_GetIcmp6IfStat(IN IF_INDEX ifIndex, OUT ICMP6_IFSTAT_S *pstIcmp6IfStat);

/*****************************************************************************
    Func Name: IP6_GeneralCheckSum
 Date Created: 2013-01-06
       Author: j09070
        Input: uiSize    ���泤��
               pBuffer   ����
       Output: ��
       Return: У���
      Caution: ���ؽ����������
Enter context: ����������
 Exit context: ��
  Locking seq: ��
  Description: �����ݼ���У���
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
        Input: ucNextPro      �ϲ�Э��(TCP��UDP��)
               uiOffset       �ϲ�Э��ͷ�ڻ����е�ƫ��
               uiBufferLen    �������ݳ���
               pBuffer        IPv6���Ļ��棬ָ��IPV6ͷ
       Output: ��
       Return: У���
      Caution: ���ؽ����������
Enter context: ����������
 Exit context: ��
  Locking seq: ��
  Description: �û�̬����У��͵ĺ���
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
    /* αͷ */
    pstIp6 = (IP6_S *)(VOID *)pBuffer;
    uiDataLen = uiBufferLen - uiOffset;
    stPseudoHeader.stIPv6SrcAddr = pstIp6->stIp6Src;
    stPseudoHeader.stIPv6DstAddr = pstIp6->stIp6Dst;
    stPseudoHeader.uiDataLen = htonl(uiDataLen);
    stPseudoHeader.ucNextPro = ucNextPro;

    /* ����αͷ��У���� */
    usPesuChksum = IP6_GeneralCheckSum((UINT)sizeof(stPseudoHeader), (VOID *)&stPseudoHeader);

    /* �������ݵ�У���� */
    pucData = (UCHAR *)(VOID *)pBuffer + uiOffset;
    usDataChksum = IP6_GeneralCheckSum(uiDataLen, (VOID *)pucData);

    /* ��αͷ��У��������ݱ��ĵ�У������д��� */
    usPesuChksum = ~(UINT)usPesuChksum;
    usDataChksum = ~(UINT)usDataChksum;
    uiChkSum += usPesuChksum;
    uiChkSum += usDataChksum;
    if (0xffff < uiChkSum)
    {
        uiChkSum = (uiChkSum & 0xffff) + (uiChkSum >> 16);
    }

    uiChkSum = ~uiChkSum;

    /* ���ΪUDP���ͣ���У���Ϊ0 ʱ������0xffff */
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
