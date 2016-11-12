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




#define IP6ADDR_FEATURENAME ("ip6addr")     /* ��ַ����ģ���������� */
#define IP6ADDR_DBM_EFFECTED_CFG ("eIP6ADDR")       /* ��Ч���ݿ��� */
#define IP6ADDR_DBM_PENDING_CFG ("pIP6ADDR")       /* δ��Ч���ݿ��� */

/*********************************************************************************************************************
                         ��ַ��Դģ����Ҫ���ĵĶ���(BEGIN)
*********************************************************************************************************************/
/* IPv6��ַ��Դ���Ͷ��� */
#define IP6ADDR_ORIGIN_TYPE_NONE          (1<<0)      /* ��Դ��Чֵ */
#define IP6ADDR_ORIGIN_TYPE_INLOOPBACK    (1<<1)      /* �ڲ����ص�ַ��::1 */
#define IP6ADDR_ORIGIN_TYPE_MANUAL        (1<<2)      /* �ֹ����õ�ַ */
#define IP6ADDR_ORIGIN_TYPE_EUI64         (1<<3)      /* �ֹ����õ�ַ */
#define IP6ADDR_ORIGIN_TYPE_DHCP          (1<<4)      /* DHCP�����ַ */
#define IP6ADDR_ORIGIN_TYPE_VRRP          (1<<5)      /* VRRP��ַ */
#define IP6ADDR_ORIGIN_TYPE_LINKLAYER     (1<<6)      /* ��״̬�Զ����õ�ַ */
#define IP6ADDR_ORIGIN_TYPE_RANDOM        (1<<7)      /* �����ַ������RFC3041 privacy address */
#define IP6ADDR_ORIGIN_TYPE_VSRP          (1<<8)      /* �ֹ�����VSRP���ַ */
#define IP6ADDR_ORIGIN_TYPE_TRILL_VR      (1<<9)      /* TRILL VR��ַ */
#define IP6ADDR_ORIGIN_TYPE_AFT           (1<<10)     /* AFT��ַ */
#define IP6ADDR_ORIGIN_TYPE_PROXY         (1<<11)     /* ͨ��������̬ǰ׺���ɵĵ�ַ */
#define IP6ADDR_ORIGIN_TYPE_MTUNNEL       (1<<12)     /* MTUNNEL��ַ */
#define IP6ADDR_ORIGIN_TYPE_ALL           0xFFFFFFFF  /* ������Դ���ͺϼ� */

/* ��ַ��Դ���õ����Ժ궨�� */
#define IP6ADDR_ORIGIN_ATTR_DEPENDENT     (1<<0)      /* ��ַ�޷�ʹ�ܽӿ�ipv6���� */
#define IP6ADDR_ORIGIN_ATTR_NODAD         (1<<1)      /* ��ַ������DAD������VRRP��ַ */
#define IP6ADDR_ORIGIN_ATTR_EUI64         (1<<2)      /* EUI-64��ַ����ַ������Ҫ���Ͻӿ�ID���������ĵ�ַ */
#define IP6ADDR_ORIGIN_ATTR_ANYCAST       (1<<3)      /* �β���ַ */

/* ���ַ���ͼ��� */
#define IP6ADDR_ORIGIN_TYPE_VR (IP6ADDR_ORIGIN_TYPE_VRRP | IP6ADDR_ORIGIN_TYPE_TRILL_VR)

/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
                         ��ַ��Դģ����Ҫ���ĵĶ���(END)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/


/*********************************************************************************************************************
                         ��ַʹ��ģ����Ҫ���ĵĶ���(BEGIN)
*********************************************************************************************************************/
/* ��ַ״̬�궨�� */
#define IP6ADDR_STATUS_PREFERRED     (1<<1)     /* preferred״̬����ַ������ԴĿ�ĵ�ַ */
#define IP6ADDR_STATUS_DEPRECATED    (1<<2)     /* deprecated״̬����ַ������Ŀ�ĵ�ַ������������Դ��ַ */
#define IP6ADDR_STATUS_INVALID       (1<<3)     /* ��Ч״̬����������ͨ�� */
#define IP6ADDR_STATUS_TENTATIVE     (1<<6)     /* δ����ַ�ظ�����״̬����������ͨ�� */
#define IP6ADDR_STATUS_DUPLICATE     (1<<7)     /* �ظ�״̬����������ͨ�� */

#define IP6ADDR_STATUS_ACTIVE        (IP6ADDR_STATUS_PREFERRED|IP6ADDR_STATUS_DEPRECATED)
#define IP6ADDR_STATUS_ALL           (0xFFFFFFFF)

/* ��ַ���Ժ궨�� */
#define IP6ADDR_PROPERTY_ANYCAST     (1<<0)     /* �β���ַ */
#define IP6ADDR_PROPERTY_LLAMAIN     (1<<1)     /* LINKLOCAL����ַ�����������UP/DOWN���ӿ�ʹ��IPV6ʱ����Ҫ�д˵�ַ */
#define IP6ADDR_PROPERTY_GATEWAY     (1<<2)     /* ����RA���� */

/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
                         ��ַʹ��ģ����Ҫ���ĵĶ���(END)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/

#define IP6ADDR_DAD_OK           1    /* DADͨ�������ظ���ַ */
#define IP6ADDR_DAD_DUPLICATE    2    /* DAD��⵽�ظ���ַ */

/*******************************ipv6��ַ�������Ͷ���**********************************/
#define IP6CFG_CFG_TYPE_LINKLOCAL    (1<<1)      /* ��·���ص�ַ���� */
#define IP6CFG_CFG_TYPE_GLOBAL       (1<<2)      /* ȫ�򵥲���ַ���� */
#define IP6CFG_CFG_TYPE_EUI64        (1<<3)      /* EUI64��ַ���� */
#define IP6CFG_CFG_TYPE_ANYCAST      (1<<4)      /* �β���ַ���� */


/***************************************** �����붨�� *****************************************/
/* Error facility */
#define ERROR_IP6ADDR_FACILITY       ERROR_GET_MODULEFACILITY(MID_IP6ADDR)

/* Error code of success level: 0x0~0x20 */

/* Error code of information level: 0x21~0x40 */
/* ��ַ��ͻ */
#define ERROR_IP6ADDR_CONFLICT       ERROR_DEFINE(ERROR_SEVERITY_INFO, ERROR_IP6ADDR_FACILITY, 0x21UL)
#define ERROR_IP6PREFIX_EXCEED       ERROR_DEFINE(ERROR_SEVERITY_INFO, ERROR_IP6ADDR_FACILITY, 0xAAUL)

/* Error code of warning level: 0x41~0x60 */

/* Error code of error level: 0x61~0x80 */
/* ��ַ��������Ƿ� */
#define ERROR_IP6ADDR_INVALIDADDR    ERROR_DEFINE(ERROR_SEVERITY_ERROR, ERROR_IP6ADDR_FACILITY, 0x61UL)
/* ��ز����޷�ִ�� */
#define ERROR_IP6ADDR_ERRCONNECTION  ERROR_DEFINE(ERROR_SEVERITY_ERROR, ERROR_IP6ADDR_FACILITY, 0x62UL)

/* ��ַ��Ϣ�ṹ */
ISSU typedef struct tagIP6ADDR_Key
{
    VRF_INDEX vrfIndex;
    IF_INDEX  ifIndex;
    IN6ADDR_S stAddr;
    UINT      uiPrefixLen;
    UINT      uiOriginType;      /* ��ַ��Դ���ͣ�����ַ��Դ���� */
}IP6ADDR_KEY_S;

#define IP6ADDR_ISSAMEADDR(_pstKeySrcM, _pstKeyDstM) \
        ( ((_pstKeySrcM)->vrfIndex == (_pstKeyDstM)->vrfIndex) && \
          ((_pstKeySrcM)->ifIndex == (_pstKeyDstM)->ifIndex) && \
          (0 == IN6ADDR_Cmp(&(_pstKeySrcM)->stAddr, &(_pstKeyDstM)->stAddr)) && \
          ((_pstKeySrcM)->uiPrefixLen == (_pstKeyDstM)->uiPrefixLen) && \
          ((_pstKeySrcM)->uiOriginType == (_pstKeyDstM)->uiOriginType) )

/* ��ַ��Ϣ�ṹ */
ISSU typedef struct tagIP6ADDR_Info
{
    IP6ADDR_KEY_S stAddrKey;
    UINT          uiProperty;     /* ��ַ���ԣ�����ַ���Ժ궨�� */
    UINT          uiStatus;       /* ��ַ״̬������ַ״̬�궨�� */
}IP6ADDR_INFO_S;

#define IP6ADDR_LIFETIME_INFINITE    0xFFFFFFFF

ISSU typedef struct tagIP6ADDR_LifeTime
{
    UINT uiValidTime;      /* ����ʱ�䣬����prefer״̬ʱ��+deprecated״̬ʱ�� */
    UINT uiPreferrdTime;   /* prefer״̬��ʱ�� */
}IP6ADDR_LIFETIME_S;

ISSU typedef struct tagIP6ADDR_OriginInfo
{
    IP6ADDR_KEY_S      stAddrKey;
    UINT               uiOriginAttr;    /* ��ַ��Դ���� */
    IP6ADDR_LIFETIME_S stLifeTime;      /* ��ַ����ʱ�� */
}IP6ADDR_ORIGIN_INFO_S;

/* �����ĵ�ַ��Ϣ */
#pragma pack(4)
ISSU typedef struct tagIP6ADDR_FullInfo
{
    IP6ADDR_INFO_S stIP6Addr;
    UINT uiOriginSubId;      /* ��Դ���ֲܷ��ڲ�ͬ�壬��subID��ʶλ�ã�����Դָ�� */
    UINT uiOriginAttr;
    UINT uiFlag;             /* �����ֶ� */
    IP6ADDR_LIFETIME_S stLifeTime;
    UINT uiCreateTime;
    UINT uiOriginalCreateTime;
    UINT uiLastChangedTime;
    UINT uiEpoch;
    UINT64 uiContext;
}IP6ADDR_FULLINFO_S;
#pragma pack()

#define IP6ADDR_PREFIX_ORIGIN_NDCFG     (1<<0) /* ND�ֹ����� */
#define IP6ADDR_PREFIX_ORIGIN_MANUAL    (1<<1) /*�ֹ����ã���ʾ�ֹ����õĵ�ַ����*/
#define IP6ADDR_PREFIX_ORIGIN_WELLKNOWN (1<<2) /* ֪��ǰ׺ */
#define IP6ADDR_PREFIX_ORIGIN_DHCP      (1<<3) /* DHCP�õ���ǰ׺ */
#define IP6ADDR_PREFIX_ORIGIN_ROUTERADV (1<<4) /* RA�������ɵ�ǰ׺ */
#define IP6ADDR_PREFIX_ORIGIN_OTHER     (1<<5) /* ��������ȷ�������Դ���������Դ */
#define IP6ADDR_PREFIX_ORIGIN_PREFIXCFG (1<<6) /* IPv6ǰ׺����̬���� */
#define IP6ADDR_PREFIX_ORIGIN_DHCPPREFIXALLOC (1<<7) /* DHCPǰ׺����õ���ǰ׺*/
#define IP6ADDR_PREFIX_ORIGIN_PROXY (1<<8) /* ����ǰ׺�����ǰ׺ */
#define IP6ADDR_PREFIX_ORIGIN_ALL       0xFFFFFFFF /* ������Դ */

/**************Begin:�漰��DHCPv6 Clientͨ��������ݽ������******************/
#define IP6ADDR_CFG_DHCPALLOC       (1<<0)    /* DHCPv6 Client ��ַ�������� */
#define IP6ADDR_CFG_DHCPINFO        (1<<1)    /* DHCPv6 Client ��Ϣ��������*/

ISSU typedef struct tagIP6AddrDhcpcCfg
{ 
    UINT   uiServType;         /* �������� */
    UINT   uiOptionGrp;        /* ѡ����ţ����û������ѡ���飬��ΪȫF*/
    UINT   uiRapidFlag;        /* Rapid ʹ�ܱ�ǣ�ENABLE����DISABLE */
}IP6ADDR_DHCPCCFG_S;
/**************Begin:�漰��DHCPv6 Clientͨ��������ݽṹ����******************/

ISSU typedef struct tagIP6ADDR_PrefixKey 
{
    VRF_INDEX vrfIndex;         /* vrf���� */
    IF_INDEX ifIndex;           /* �ӿ����� */
    IN6ADDR_S stPrefixAddr;     /* ǰ׺��ַ */
    UINT uiPrefixLength;        /* ǰ׺���� */
    UINT uiPrefixOrigin;        /* ǰ׺��Դ����ǰ׺��Դ���ͺ궨��*/
}IP6ADDR_PREFIX_KEY_S;

ISSU typedef struct tagIP6ADDR_Prefix
{
    IP6ADDR_PREFIX_KEY_S   stPrefixKey;     /* prefix�ؼ���Ϣ */
    IP6ADDR_LIFETIME_S     stLifeTime;      /* ��Чʱ�� */
    UCHAR                  ucFlags;         /* ǰ׺��־ */
}IP6ADDR_PREFIX_S;

/* ������ǰ׺��Ϣ */
ISSU typedef struct tagIP6ADDR_PREFIX_FullInfo
{
    IP6ADDR_PREFIX_S    stPrefix;
    UINT                uiCreateTime;
    UINT                uiEpoch;
}IP6ADDR_PREFIX_FULLINFO_S;

ISSU typedef struct tagIP6ADDR_GatewayKey 
{
    IF_INDEX ifIndex;            /* �ӿ����� */
    IN6ADDR_S stGatewayAddr;     /* ���ص�ַ */
    VRF_INDEX vrfIndex;
}IP6ADDR_GATEWAY_KEY_S;

ISSU typedef struct tagIP6ADDR_GATEWAY
{
    IP6ADDR_GATEWAY_KEY_S   stGatewayKey;
    UCHAR                   ucFlag;             /* RA�����еı����Ϣ */
    CHAR                    cPriority;
    UINT                    uiRouterLifetime;
    UINT                    uiCreateTime;
    UINT                    uiEpoch;
}IP6ADDR_GATEWAY_S;

/* IP6ADDRǰ׺���ýṹ */
ISSU typedef struct tagIP6ADDR_PREFIXCONFIG
{
    IF_INDEX ifIndex;
    VRF_INDEX vrfIndex;             /* VRF���� */
    IN6ADDR_S stPrefix;             /* ǰ׺��ַ */ 
    UINT uiValidTime;               /* ǰ׺��Ч�������� */
    UINT uiPreferredTime;           /* ǰ׺��ѡ�������� */   
    USHORT usPrefixLen;             /* ǰ׺���볤�� */
    UCHAR ucLAFlag;                 /* OnLink��auto��־ */
    BOOL_T bIsUndo;                 /* UNDO��ʶ����undo:1  ��undo:0 */
    BOOL_T bIsDefaultPara;          /*��ʶ�Ƿ�Я���������޲���:1��������:0*/
}IP6ADDR_PREFIXCONFIG_S;

#ifndef __KERNEL__
extern INT IP6Addr_Open(VOID);
extern VOID IP6Addr_Close(IN INT iHandle);

extern ULONG IP6Addr_AddAddr(IN INT iHandle, IN const IP6ADDR_ORIGIN_INFO_S *pstIP6AddrOrigin, IN UINT uiFlags);
extern ULONG IP6Addr_DelAddr(IN INT iHandle, IN const IP6ADDR_ORIGIN_INFO_S *pstIP6AddrOrigin, IN UINT uiFlags);

extern ULONG IP6Addr_SmoothStart(IN INT iHandle, IN UINT uiOriginType);
extern ULONG IP6Addr_SmoothEnd(IN INT iHandle, IN UINT uiOriginType);

/**************Begin:IPv6ǰ׺������غ���*********************/
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
/**************End:IPv6ǰ׺������غ���*********************/

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

/* =============================��������summary����ʹ��============================= */
ULONG IP6Addr_GetIp6Summary(IN ULONG ulBufLen, OUT CHAR *pcString);

#ifdef __KERNEL__
extern VOID IP6Addr_NotifyDADResult( IN IF_INDEX ifIndex, IN const IN6ADDR_S *pstIP6Addr, IN UINT uiDADResult );

/* �ӿ��ϵ�LinkLocal��ַ����Ϊ��������ΪRAԴ��ַ��������Ϊ�¹��������� */
extern ULONG IP6Addr_LinkLocalAddrRAHalt(IN IF_INDEX ifIndex);
/* �ӿ��ϵ�LinkLocal��ַ����Ϊ������ΪRAԴ��ַ(Ĭ�����)����������Ϊ�¹��������� */
extern ULONG IP6Addr_LinkLocalAddrRANoHalt(IN IF_INDEX ifIndex);

extern BOOL_T IP6Addr_IsMCAddrOnIf(IN IF_INDEX ifIndex, IN const IN6ADDR_S *pstIP6MCAddr);
extern ULONG IP6Addr_AddMCAddr(IN IF_INDEX ifIndex, IN const IN6ADDR_S *pstIP6MCAddr);
extern ULONG IP6Addr_DelMCAddr(IN IF_INDEX ifIndex, IN const IN6ADDR_S *pstIP6MCAddr);

/* ǰ׺��ؽӿ� */
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


/* ��״̬�Զ����ù��ܽӿ� */                                    
extern BOOL_T IP6Addr_IsStatelessAutoCfg( IN IF_INDEX ifIndex );                                    
extern ULONG IP6Addr_RefreshRAPrefix( IN const IP6ADDR_PREFIX_S *pstPrefix );
extern ULONG IP6Addr_RefreshDefaultRouter( IN IF_INDEX ifIndex, 
                                           IN const IN6ADDR_S *pstRouterAddr, 
                                           IN UCHAR ucFlags, 
                                           IN UINT uiLifeTime );

#endif


/**************************************************************************************************************
                                ��ַ�¼�ע��ӿ�(BEGIN)
**************************************************************************************************************/
#define IP6ADDR_EVENT_ADDR            ((1<<0)<<16)   /* ��ַ�¼� */
#define IP6ADDR_EVENT_PREFIX          ((1<<1)<<16)   /* ǰ׺�¼� */
#define IP6ADDR_EVENT_GATEWAY         ((1<<2)<<16)   /* �����¼��¼� */
#define IP6ADDR_EVENT_PREFIXM         ((1<<3)<<16)   /* ǰ׺�����¼����� */


#define IP6ADDR_EVENT_ADDR_ADD         (IP6ADDR_EVENT_ADDR|(1<<0))   /* ��ַ��Ӻ��¼� */
#define IP6ADDR_EVENT_ADDR_UPDATE      (IP6ADDR_EVENT_ADDR|(1<<1))   /* ��ַ״̬�ı���¼� */
#define IP6ADDR_EVENT_ADDR_PRE_DELETE  (IP6ADDR_EVENT_ADDR|(1<<2))   /* ��ַɾ��ǰ�¼� */
#define IP6ADDR_EVENT_ADDR_DELETE      (IP6ADDR_EVENT_ADDR|(1<<3))   /* ��ַɾ�����¼� */

#define IP6ADDR_EVENT_PREFIX_ADD       (IP6ADDR_EVENT_PREFIX|(1<<0))   /* ǰ׺��Ӻ��¼� */
#define IP6ADDR_EVENT_PREFIX_DELETE    (IP6ADDR_EVENT_PREFIX|(1<<1))   /* ǰ׺ɾ�����¼� */

#define IP6ADDR_EVENT_GATEWAY_ADD     (IP6ADDR_EVENT_GATEWAY|(1<<0))   /* ������Ӻ��¼� */
#define IP6ADDR_EVENT_GATEWAY_DELETE  (IP6ADDR_EVENT_GATEWAY|(1<<1))   /* ����ɾ�����¼� */

#define IP6ADDR_EVENT_PREFIXM_ADD      (IP6ADDR_EVENT_PREFIXM|(1<<0))   /* ǰ׺������Ӻ��¼� */
#define IP6ADDR_EVENT_PREFIXM_DELETE   (IP6ADDR_EVENT_PREFIXM|(1<<1))   /* ǰ׺����ɾ�����¼� */


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

/***********Begin:IPv6ǰ׺����ǰ׺�¼���غ���***********/
typedef VOID ( *IP6ADDR_PREFIXMNGCHANGE_PROC_PF ) (IN UINT uiPrefixEvent, 
                                                IN UINT uiPrefixIndex,
                                                IN const IP6ADDR_PREFIX_S *pstPrefix );
extern ULONG IP6Addr_RegisterPrefixMngEvent(IN UINT uiMID,
                                            IN UINT uiPrefixEvent,
                                            IN IP6ADDR_PREFIXMNGCHANGE_PROC_PF pfProcPrefixEvent );
/***********End:IPv6ǰ׺����ǰ׺�¼���غ���***********/

/***********Begin:���ñ仯�¼���ض���***********/
typedef ULONG ( *IP6ADDR_CONFIG_CHANGE_PROC_PF) (IN UINT uiIfCfgOper, 
                                                 IN IF_INDEX ifIndex, 
                                                 IN const VOID *pstCfgData ); 
extern ULONG IP6Addr_RegisterConfigChangeEvent(IN INT iHandle,
                                               IN UINT uiCfgType,
                                               IN IP6ADDR_CONFIG_CHANGE_PROC_PF pfCfgProc);
extern VOID IP6Addr_DeregisterConfigChangeEvent (IN INT iHandle, IN UINT uiCfgType);
extern ULONG IP6Addr_HandleConfigChangeEvent (IN INT iHandle);
extern ULONG IP6Addr_RequestConfig (IN INT iHandle);
/***********End:���ñ仯�¼���ض���***********/

#endif


/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
                                ��ַ�¼�ע��ӿ�(END)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/


/**************************************************************************************************************
                                ����IP��ȡMAC�Ľӿ�(BEGIN)
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

#define IP6ADDR_GETMAC_FLAG_PROXY   (1<<0)   /* ��MAC�Ǳ���MAC��ֻ���������Ӧ�� */
#define IP6ADDR_GETMAC_FLAG_MACLB   (1<<1)   /* ��IP��Ӧ���MAC���и��طֵ������ܷ��͹㲥���� */

typedef struct tagIP6ADDR_GetMacAttachInfo
{
    IN6ADDR_S stRequesterIP;                  /* �����ߵ�IP��ַ */
    UCHAR     aucRequesterMac[MAC_ADDR_LEN];  /* �����ߵ�MAC��ַ */
}IP6ADDR_GETMAC_ATTACHINFO_S;

typedef ULONG (*IP6ADDR_GETMAC_PF)( IN VRF_INDEX vrfIndex,   /* Vrf���� */
                                    IN IF_INDEX ifIndex,     /* �ӿ����� */
                                    IN const IN6ADDR_S *pstIPAddr, /* ָ���ı���IP��ַ  */
                                    IN const IP6ADDR_GETMAC_ATTACHINFO_S *pstAttachInfo, /* ������Ϣ������Ϣ�ṩ��VRRPEʹ�� */
                                    OUT UCHAR *pucLocalMac,  /* ָ��IP��ַ��Ӧ��MAC��ַ */
                                    OUT UINT *puiFlag);      /* ������;��ǣ�IP6ADDR_GETMAC_FLAG_xxx */
/* ����ֵ��*/
#define IP6ADDR_GETMAC_OK         0        /* ��ȡMAC�ɹ� */
#define IP6ADDR_GETMAC_NOMAC      1        /* �����͵ĵ�ַ����Ҫ��Ӧ */
#define IP6ADDR_GETMAC_NOTSPEC    2        /* ��Ҫ��Ӧ���ǲ�����MAC */
#define IP6ADDR_GETMAC_NOTSEND    3        /* ��ֹ��Ӧ����ʹ����������Ҳ���ܻ�Ӧ */

extern VOID IP6Addr_RegGetMacCallback(IN UINT uiOriginType, 
                                      IN IP6ADDR_GETMAC_PRI_E enGetMacPrior, 
                                      IN IP6ADDR_GETMAC_PF pfGetMacFunc);
extern VOID IP6Addr_DeregGetMacCallback(IN UINT uiOriginType);

extern ULONG IP6Addr_GetMac(IN IF_INDEX ifIndex, 
                            IN VRF_INDEX vrfIndex,
                            IN const IN6ADDR_S *pstIPAddr, 
                            IN const IP6ADDR_GETMAC_ATTACHINFO_S *pstAttachInfo, 
                            OUT UCHAR *pucMac, 
                            OUT UINT *puiFlag);       /* ������;��ǣ�IP6ADDR_GETMAC_FLAG_xxx */

#endif
/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
                                ����IP��ȡMAC�Ľӿ�(END)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/


/* =======================�ں˳�ʼ��������������============================================= */
#ifdef __KERNEL__
extern ULONG IP6Addr_StartInit(INOUT LPVOID pStartContext);
extern ULONG IP6Addr_StartRun(INOUT LPVOID pStartContext);

extern ULONG IP6Addr_MDCEventProc(IN UINT uiEvent, IN VOID *pData);
#endif
/**************************************************************************************************************
                                IPv6��ַ�����û�̬�ں�̬ͨ�Ŷ���(BEGIN)
**************************************************************************************************************/
/* DUMP �������ֵ */
#define IP6ADDR_DUMP_BUFSIZE       2048UL

/* ��ȡ�ں��¼��仯��Ϣ�Ļ����С */
#define IP6ADDR_KEVENT_BUFSIZE     2048

/* �����Դ�� */
#define IP6ADDR_ORIGINTYPE_MAX (sizeof(UINT)*8)

/* ��ȡFULLINFO�е�ַ�ؼ���Ϣ�ĺ� */
#define IP6ADDR_ADDRKEY_INFULLINFO(pstAddrFullInfo)  (&((pstAddrFullInfo)->stIP6Addr.stAddrKey))

/* ����ַ�ؼ���Ϣ��Ϊ��Ч */
#define IP6ADDR_SET_INVALIDADDRKEY(_pstIP6AddrKey) \
{\
    memset(_pstIP6AddrKey, 0, sizeof(IP6ADDR_KEY_S));\
    (_pstIP6AddrKey)->vrfIndex = (USHORT)L3VPN_INVALID_VRF_INDEX;\
}
/* ��ǰ׺�ؼ���Ϣ��Ϊ��Ч */
#define IP6ADDR_SET_INVALIDPREKEY(_pstIP6PreKey) \
{\
    memset(_pstIP6PreKey, 0, sizeof(IP6ADDR_PREFIX_KEY_S));\
}

/* �жϵ�ַ�ؼ���Ϣ�Ƿ�Ϊ��Ч�ĺ� */
#define IP6ADDR_ISINVALIDADDRKEY(pstIP6AddrKey) ((USHORT)L3VPN_INVALID_VRF_INDEX == (pstIP6AddrKey)->vrfIndex)


typedef struct tagIP6ADDR_MulticastInfo
{
    IN6ADDR_S stIP6MutiAddr;                    /* �鲥��ַ */
    UINT uiRefCount;                            /* �鲥��ַ�����ü��� */
}IP6ADDR_MULTICAST_S;

/* �ں˵�ַ�����ṩ��ͨ�ù��˽ṹ */
typedef struct tagIP6ADDR_Slt
{
    UINT uiScope;
    UINT uiFlag;             /* ��ǽṹ�ж�Ӧ���Ƿ���Ч����λ����ĳ�Ա�Ƿ���Ч���趨�� */
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
/* ���º���������IP6ADDR_SLT_S��Ӧλ */
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

/* ���˽ṹ��ʼ���� */
#define IP6ADDR_SLT_INIT(_pstSLT)\
{\
    memset((_pstSLT), 0, sizeof(IP6ADDR_SLT_S));\
}

#define IP6ADDR_SLT_CLEARFLAG(_pstSLT, _uiFlag) ((_pstSLT)->uiFlag &= ~(_uiFlag))
#define IP6ADDR_SLT_TESTFLAG(_pstSLT, _uiFlag) ((_pstSLT)->uiFlag & (_uiFlag))

/* LIPC�˿ڶ��� */
/* ����ȫ��֪���˿� */


/* �¼������ı���֪���˿� */


/* ʵʱͬ���鲥ȫ��֪���˿� */


/* ����ͬ��ȫ��֪���˿� */



/* CIOCTL������ݽṹ���� */

#define IP6ADDR_CIOCTL_CMD_DRVK    CIOCTL_CMD_DEF(CIOCTL_CMDMOD_IP6ADDR, 1)


#define IP6ADDR_DUMP_CMD_SHOW      DUMP_CMD_DEF(DUMP_CMDMOD_IP6ADDR, 1)

#define KEVENT_FAMILY_IP6ADDR  1
#define IP6ADDR_KEVENT_ID_NOTIFY   KEVENT_ID_DEF(KEVENT_FAMILY_IP6ADDR, 1)
#define IP6ADDR_KEVENT_ID_AUTO     KEVENT_ID_DEF(KEVENT_FAMILY_IP6ADDR, 2)

/* CIOCTL ��Ϣ���� */
ISSU typedef enum tagIP6ADDR_CioctlMsgType
{
    IP6ADDR_CIOCTL_ADDADDR = 0,          /* ��ӵ�ַ */
    IP6ADDR_CIOCTL_DELADDR,              /* ɾ����ַ */
    IP6ADDR_CIOCTL_MODIFYADDR,           /* �޸ĵ�ַ״̬ */
    IP6ADDR_CIOCTL_STARTDAD,             /* ����DAD */
    IP6ADDR_CIOCTL_STOPDAD,              /* ����DAD */
    IP6ADDR_CIOCTL_GETNEXTIF,            /* ��ȡ��һ���ӿ����� */
    IP6ADDR_CIOCTL_GETNEXTBATCH,         /* ��ȡ��һ����ַ */
    IP6ADDR_CIOCTL_OVERLAPCHECK,         /* ��ַoverlap��ͻ��� */
    IP6ADDR_CIOCTL_LOOKUPADDRESS,        /* ��ȷ���� */
    IP6ADDR_CIOCTL_SELECTSRCADDR,        /* ѡ��Դ��ַ */
    IP6ADDR_CIOCTL_SELECTSRCADDRNON,     /* ��ȫ�򵥲���ַѡ��Դ��ַ */
    IP6ADDR_CIOCTL_SELECTSRCADDRONIF,    /* ��ָ���ӿ���ѡ��Դ��ַ */
    IP6ADDR_CIOCTL_BESTMATCHONIF,        /* �ӿ���ѡ�����ŵĵ�ַ */
    IP6ADDR_CIOCTL_GETADDREPOCH,         /* ��ȡ��ַʱ�� */
    IP6ADDR_CIOCTL_INCADDREPOCH,         /* ���µ�ַʱ�� */
    IP6ADDR_CIOCTL_GETIFID,              /* ��ȡ�ӿ�ID */
    IP6ADDR_CIOCTL_GETLINKSTATE,         /* ��ȡ��·��״̬ */
    IP6ADDR_CIOCTL_GETKEVENT,            /* ��ȡ�ں˱仯�¼� */
    IP6ADDR_CIOCTL_CLEARKEVENT,          /* �������ӿ��ں��¼�ƽ�� */
    IP6ADDR_CIOCTL_PREPAREIFKEVENT,      /* ׼������ӿ��ں��¼�ƽ�� */
    IP6ADDR_CIOCTL_GETIFCTLBLKINFO,      /* ��ȡ�ӿڿ��ƿ��ϵ���Ϣ */
    /* ǰ׺��Ϣ���� */
    IP6ADDR_CIOCTL_ADDPREFIX,            /* ���ǰ׺ */  
    IP6ADDR_CIOCTL_DELPREFIX,            /* ɾ��ǰ׺ */ 
    IP6ADDR_CIOCTL_LOOKUPPREFIX,         /* ��ȷ����ǰ׺ */ 
    IP6ADDR_CIOCTL_GETNEXTBATCHPREFIX,   /* ��ȡ��һ��ǰ׺ */
    IP6ADDR_CIOCTL_GETPREFIXEPOCH,       /* ��ȡǰ׺ʱ��� */ 
    IP6ADDR_CIOCTL_INCPREFIXEPOCH,       /* ����ǰ׺ʱ��� */
    IP6ADDR_CIOCTL_GETPUBADDRBYPRE,      /* ����ָ��ǰ׺��ȡ���е�ַ */
    /* ������Ϣ���� */
    IP6ADDR_CIOCTL_ADDGATEWAY,           /* ������� */   
    IP6ADDR_CIOCTL_DELGATEWAY,           /* ɾ������ */ 
    IP6ADDR_CIOCTL_DELIFGATEWAY,         /* ɾ���ӿ��ϵ�������Ϣ */
    IP6ADDR_CIOCTL_GETNEXTBATCHGATEWAY,  /* ��ȡ��һ������ */
    IP6ADDR_CIOCTL_GETGATEWAYEPOCH,      /* ��ȡ����ʱ��� */ 
    IP6ADDR_CIOCTL_INCGATEWAYEPOCH,      /* ��������ʱ��� */ 
    IP6ADDR_CIOCTL_GETGWMINTIME,         /* ��ȡ��������ϻ�ʱ�� */
    IP6ADDR_CIOCTL_SETGWREACHABILITY,    /* ˢ�����ؿɴ��� */
    IP6ADDR_CIOCTL_SELECTGATEWAY,        /* ��ѡ���� */
    IP6ADDR_CIOCTL_GETNEXTBATCHPBGW,     /* ��ȡ��һ���ѷ��������� */
    IP6ADDR_CIOCTL_REFRESHPUBGATEWAY,    /* ����˽�����Ѿ�������������Ϣ */
    /* AUTO CONFIG��Ϣ���� */
    IP6ADDR_CIOCTL_SETAUTOENABLE,        /* ʹ��Auto����/Auto linklocal���� */
    IP6ADDR_CIOCTL_GETAUTOINFO,          /* ��ȡ��״̬�Զ����� */
    IP6ADDR_CIOCTL_GETBATCHAUTO,         /* ������ȡ��״̬�Զ����� */
    IP6ADDR_CIOCTL_RSSTART,              /* RS���ķ��� */
    IP6ADDR_CIOCTL_GETAUTOMINTIME,       /* ��ȡauto����ϻ��¼� */
    IP6ADDR_CIOCTL_GETAUTOEPOCH,         /* ��ȡ��״̬ʹ��ʱ��� */
    IP6ADDR_CIOCTL_INCAUTOEPOCH,         /* ������״̬ʹ��ʱ��� */
    IP6ADDR_CIOCTL_GETSTAT,              /* ��ȡͳ����Ϣ */
    IP6ADDR_CIOCTL_GETNEXTADDRINVRF,     /* ��ȡvrf�е���һ����ַ��Ϣ */
    IP6ADDR_CIOCTL_GETNEXTGWINVRF,       /* ��ȡ��һ������ON vrf */
    IP6ADDR_CIOCTL_GETGWBYKEY,           /* ����KEY��ȡ���� */
    IP6ADDR_CIOCTL_SETPREFERTEMPADDR,    /* ������ѡ��ʱ��ַ��Ϣ */
    IP6ADDR_CIOCTL_GETPREFERTEMPADDR,    /* ��ȡ��ѡ��ʱ��ַ��� */
    IP6ADDR_CIOCTL_GETPREFIX,            /* ����key���ҽӿ��ϵ�ǰ׺ */    
    IP6ADDR_CIOCTL_GETBESTMATCHPRELEN,   /* ���Һ͵�ַ���ƥ���ǰ׺���� */
    IP6ADDR_CIOCTL_BUTT
}IP6ADDR_CIOCTL_MSGTYPE_E;

/* DUMP ��Ϣ���� */
typedef enum tagIP6ADDR_DumpMsgType
{
    IP6ADDR_DUMP_GETSTATISTIC=0,   /* ��ȡͳ����Ϣ */
    IP6ADDR_DUMP_BATCHGETUC,       /* ������ȡ������ַ */
    IP6ADDR_DUMP_BATCHGETMCONIF,   /* ������ȡָ���ӿ����鲥��ַ */
    IP6ADDR_DUMP_BATCHGETPREONIF,  /* ������ȡ�ӿ���ǰ׺ */
    IP6ADDR_DUMP_BATCHGETGWONIF,   /* ������ȡ�ӿ������� */
    IP6ADDR_DUMP_BUTT
}IP6ADDR_DUMP_MSGTYPE_E;

/* ͳ����Ϣ���� */
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

/* DAD��������ͽṹ */
ISSU typedef struct tagIP6ADDR_DADMsg
{
    IN6ADDR_S   stIP6Addr;   /* ��ַ */
    IF_INDEX    ifIndex;     /* �ӿ����� */
    UINT        uiDADState;  /* DAD����� */
}IP6ADDR_DADMSG_S;

/* �ӿ�ID�仯��Ϣ�ṹ */
ISSU typedef struct tagIP6ADDR_IIDMsg
{
    IF_INDEX            ifIndex;         /* �仯�Ľӿ����� */
    IP6_INTERFACEID_S   stInterfaceID;   /* �仯��ӿ�ID */
}IP6ADDR_IIDMSG_S;

/* �ӿ�Nego״̬�仯��Ϣ�ṹ */
ISSU typedef struct tagIP6ADDR_NegotiateMsg
{
    IF_INDEX            ifIndex;         /* �仯�Ľӿ����� */
    UINT                uiNegoStatus;    /* Nego״̬ */
    BOOL_T              bDownBefore;     /* ����down�� */
    USHORT              usReserve;
}IP6ADDR_NEGOTIATEMSG_S;

/* ��Ϣ��װ��غ꼰���ݽṹ���� */
ISSU typedef struct tagIP6ADDR_MsgHead
{
    UINT uiMsgType;    /* ��Ϣ���� */
    UINT uiInfoLen;    /* ���س��� */
}IP6ADDR_MSGHEAD_S;

/* ��ȡָ��V��ָ�� */
#define IP6ADDR_TLV_GETVP(buf) ((VOID *)((CHAR *)(buf) + sizeof(IP6ADDR_MSGHEAD_S)))
/* ���װʱ��ȡtype */
#define IP6ADDR_TLV_GETT(buf) (ntohl(((IP6ADDR_MSGHEAD_S *)(buf))->uiMsgType))
/* ���װʱ��ȡlen */
#define IP6ADDR_TLV_GETL(buf) (ntohl(((IP6ADDR_MSGHEAD_S *)(buf))->uiInfoLen))
/* ��ȡ��Ϣ���� */
#define IP6ADDR_TLV_GETHEADLEN (sizeof(IP6ADDR_MSGHEAD_S))

/* ��װT & L */
#define IP6ADDR_TLV_ENCAPTL(type, len, buf) \
{ \
    ((IP6ADDR_MSGHEAD_S *)(buf))->uiMsgType = htonl(type); \
    ((IP6ADDR_MSGHEAD_S *)(buf))->uiInfoLen = htonl(len); \
}

/* һ����Ϣ���Ͷ��� */
ISSU typedef enum tagIP6Addr_MsgType
{
    IP6ADDR_MSGTYPE_ADDR,          /* ��ַ��Ϣ */
    IP6ADDR_MSGTYPE_PREFIX,        /* ǰ׺��Ϣ */
    IP6ADDR_MSGTYPE_GATEWAY,       /* ������Ϣ */
    IP6ADDR_MSGTYPE_SMOOTH,        /* ƽ����Ϣ */  
    IP6ADDR_MSGTYPE_IP6CFG,        /* ��ַ������Ϣ */
    IP6ADDR_MSGTYPE_KEVENTNOTIFY,  /* �ں��¼�֪ͨ */ 
    IP6ADDR_MSGTYPE_NET,           /* �������Ϣ */
    IP6ADDR_MSGTYPE_DAD,           /* �������Ϣ */
    IP6ADDR_MSGTYPE_AUTO,          /* ��״̬�Զ�������Ϣ */
    IP6ADDR_MSGTYPE_RRSEQ,         /* RR���к�ͬ����Ϣ */
    IP6ADDR_MSGTYPE_CFGCHANGE,     /* ע����Ӧ���ñ仯��Ϣ */
    IP6ADDR_MSGTYPE_NOTIFY,        /* ֪ͨ��ַ�����ٷ�һ����������Ϣ */
    IP6ADDR_MSGTYPE_BUTT
} IP6ADDR_MSGTYPE_E;

/* �ں�����������Ϣ���� */
ISSU typedef enum tagIP6ADDR_NETTYPE
{
    IP6ADDR_NET_NEGOTIATESTATUS = 0,     /* ���������UP/DOWN��Ϣ */
    IP6ADDR_NET_INTERFACEID,             /* �ӿ�ID�仯 */
    IP6ADDR_NET_BUTT
}IP6ADDR_NETTYPE_E;

/* �ں�AUTO������Ϣ���� */
ISSU typedef enum tagIP6ADDR_AUTOTYPE
{
    IP6ADDR_AUTO_PREFIX = 0,       /* ˢ��ǰ׺��Ϣ��Ϣ */
    IP6ADDR_AUTO_GATEWAY,          /* ˢ��������Ϣ��Ϣ */
    IP6ADDR_AUTO_BUTT
}IP6ADDR_AUTOTYPE_E;


ISSU typedef enum tagIP6ADDR_ENABLETYPE
{
    IP6ADDR_ENABLE_AUTO = 0,       /* �ӿ�ʹ����״̬�Զ����ù��� */
    IP6ADDR_ENABLE_AUTOLL,         /* �ӿ�ʹ���Զ�����linklocal��ַ���� */
    IP6ADDR_ENABLE_BUTT
}IP6ADDR_ENABLETYPE_E;

#define IP6ADDR_ENABLE_AUTO_BIT           (1<<IP6ADDR_ENABLE_AUTO)     /* ʹ����״̬�Զ����� */
#define IP6ADDR_ENABLE_AUTOLL_BIT         (1<<IP6ADDR_ENABLE_AUTOLL)   /* ʹ��auto link-local���� */

/* �Զ�������Ϣ�ṹ�� */
ISSU typedef struct tagIP6ADDR_AutoCfg
{
    UINT uiEpoch[IP6ADDR_ENABLE_BUTT];
    UINT uiRAPrefixCount;
    UINT uiAutoAddrCount;
    UINT uiEnableFlag;
}IP6ADDR_AUTOCFG_S;

/* �ӿڿ��ƿ����Խṹ */
ISSU typedef struct tagIP6ADDR_IFCtlBlkInfo
{
    UINT uiFlag;
    UINT uiIFEvFlag;
    IP6ADDR_AUTOCFG_S stAutoCfg;
}IP6ADDR_IFCTLBLKINFO_S;

/* ʹ��ͬ����Ϣ */
ISSU typedef struct tagIP6ADDR_EnableInfo
{
    IF_INDEX ifIndex;
    UINT uiEnableFlag;
}IP6ADDR_ENABLEINFO_S;

/*****************************************************************************
    Func Name: IP6ADDR_LifeTime_HTON
 Date Created: 2010/12/22 
       Author: l06132
  Description: IP6ADDR_LIFETIME_S������ת������
        Input: IP6ADDR_LIFETIME_S *pstLifeTime, ��ת������
       Output: IP6ADDR_LIFETIME_S *pstLifeTime, ת�������
       Return: ��
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
  Description: IP6ADDR_KEY_S������ת������
        Input: IP6ADDR_KEY_S *pstAddrKey, ��ת������
       Output: IP6ADDR_KEY_S *pstAddrKey, ת�������
       Return: ��
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
  Description: IP6ADDR_INFO_S������ת������
        Input: IP6ADDR_INFO_S *pstAddrInfo, ��ת������
       Output: IP6ADDR_INFO_S *pstAddrInfo, ת�������
       Return: ��
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
  Description: IP6ADDR_PREFIX_KEY_S������ת������
        Input: IP6ADDR_PREFIX_KEY_S *pstPrefixKey, ��ת������
       Output: IP6ADDR_PREFIX_KEY_S *pstPrefixKey, ת�������
       Return: ��
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
  Description: IP6ADDR_PREFIX_S������ת������
        Input: IP6ADDR_PREFIX_S *pstPrefixInfo, ��ת������
       Output: IP6ADDR_PREFIX_S *pstPrefixInfo, ת�������
       Return: ��
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
  Description: IP6ADDR_PREFIX_FULLINFO_S������ת������
        Input: IP6ADDR_PREFIX_FULLINFO_S *pstPrefixFullInfo, ��ת������
       Output: IP6ADDR_PREFIX_FULLINFO_S *pstPrefixFullInfo, ת�������
       Return: ��
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
  Description: IP6ADDR_GATEWAY_KEY_S������ת������
        Input: IP6ADDR_GATEWAY_KEY_S *pstGatewayKey, ��ת������
       Output: IP6ADDR_GATEWAY_KEY_S *pstGatewayKey, ת�������
       Return: ��
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
  Description: IP6ADDR_GATEWAY_S������ת������
        Input: IP6ADDR_GATEWAY_S *pstGateway, ��ת������
       Output: IP6ADDR_GATEWAY_S *pstGateway, ת�������
       Return: ��
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
  Description: IP6ADDR_MULTICAST_S������ת������
        Input: IP6ADDR_MULTICAST_S *pstMCInfo, ��ת������
       Output: IP6ADDR_MULTICAST_S *pstMCInfo, ת�������
       Return: ��
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
  Description: IP6ADDR_FULLINFO_S������ת������
        Input: IP6ADDR_FULLINFO_S *pstAddrFullInfo, ��ת������
       Output: IP6ADDR_FULLINFO_S *pstAddrFullInfo, ת�������
       Return: ��
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
  Description: IP6ADDR_SLT_S������ת������
        Input: IP6ADDR_SLT_S *pstSLT, ��ת������
       Output: IP6ADDR_SLT_S *pstSLT, ת�������
       Return: ��
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
  Description: IP6ADDR_DADMSG_S������ת������
        Input: IP6ADDR_DADMSG_S *pstDadMsg, ��ת������
       Output: IP6ADDR_DADMSG_S *pstDadMsg, ת�������
       Return: ��
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
  Description: IP6ADDR_IIDMSG_S������ת������
        Input: IP6ADDR_IIDMSG_S *pstIIDMsg, ��ת������
       Output: IP6ADDR_IIDMSG_S *pstIIDMsg, ת�������
       Return: ��
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
  Description: IP6ADDR_NEGOTIATEMSG_S������ת������
        Input: IP6ADDR_NEGOTIATEMSG_S *pstNegotiateMsg, ��ת������
       Output: IP6ADDR_NEGOTIATEMSG_S *pstNegotiateMsg, ת�������
       Return: ��
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
  Description: �Ƚ�����UINT�޷������Ĵ�С
        Input: uiLeft  �Ƚ���ֵ
               uiRight �Ƚ���ֵ   
       Output: 
       Return: 1   ��ֵ>��ֵ
               0   ��ֵ=��ֵ
               -1  ��ֵ<��ֵ
      Caution: ���ܱȽϴ���32bit���޷�����
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
  Description: �Զ�������Ϣ������ת������
        Input: IP6ADDR_AUTOCFG_S *pstAutoCfg    ָ���Զ�������Ϣ��ָ��
       Output: IP6ADDR_AUTOCFG_S *pstAutoCfg    ָ���Զ�������Ϣ��ָ��
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
  Description: IP6ADDR_IFCTLBLKINFO_S������ת������
        Input: IP6ADDR_IFCTLBLKINFO_S *pstIFCtlBlkInfo, ��ת������
       Output: IP6ADDR_IFCTLBLKINFO_S *pstIFCtlBlkInfo, ת�������
       Return: ��
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
  Description: IP6ADDR_ENABLEINFO_S������ת������
        Input: IP6ADDR_ENABLEINFO_S *pstEnableInfo, ��ת������
       Output: IP6ADDR_ENABLEINFO_S *pstEnableInfo, ת�������
       Return: ��
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

