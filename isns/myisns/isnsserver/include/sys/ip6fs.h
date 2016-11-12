/*****************************************************************************
 Copyright (c) 2010, Hangzhou H3C Technologies Co., Ltd. All rights reserved.
------------------------------------------------------------------------------
                            ip6fs.h
  Project Code: Comware V7
   Module Name: IP6FS
  Date Created: 2010-8-19 
        Author: z03132
   Description: Header file for IP6FS module

------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------
                                                                             
*****************************************************************************/

#ifndef _SYS_IP6FS_H_
#define _SYS_IP6FS_H_

#ifdef  __cplusplus
extern "C"{
#endif

#ifndef _SYS_LIST_H_
#include <sys/list.h>
#endif

#ifndef _SYS_HASH_H_
#include <sys/hash.h>
#endif

/****************************************************
 *             Macro definition                     *
 ****************************************************/
/* 定义MID */


/* 快转配置LIPC 全局端口*/


/* 定义CIOCTL */

#define IP6FS_CIOCTL_CMD_PROC  CIOCTL_CMD_DEF(CIOCTL_CMDMOD_IP6FS, 1)

/* 定义DUMP */

#define IP6FS_DUMP_CMD_CACHE          DUMP_CMD_DEF(DUMP_CMDMOD_IP6FS, 1)
#define IP6FS_DUMP_CMD_MACFW_CACHE    DUMP_CMD_DEF(DUMP_CMDMOD_IP6FS, 2)
#define IP6FS_DUMP_CMD_BRIDGE_CACHE   DUMP_CMD_DEF(DUMP_CMDMOD_IP6FS, 3)
#define IP6FS_DUMP_CMD_INLINE_CACHE   DUMP_CMD_DEF(DUMP_CMDMOD_IP6FS, 4)

/* 二层头长度 =
   最大链路层头长度(MAX_LINKHEAD_SIZE) + 
   最大nhlfe标签栈长度(4 * MAX_LABEL_DEPTH) +
   前缀标签栈长度(4) = 76 */
#define IP6FS_PREPEND_LEN        76       /* IPv6 快转贴头长度宏定义 */

#define IP6FS_CACHE_AGING_TIME   30       /* 快转表项缺省老化时间(s) */

/* 使用首片先过的业务(具备使能/去使能功能) */
typedef enum tagIP6_FirstFragInOrder_BIT
{
    IP6_FIRSTFRAGINORDER_IPSEC,
    IP6_FIRSTFRAGINORDER_APR,
    IP6_FIRSTFRAGINORDER_APPSTAT,
    IP6_FIRSTFRAGINORDER_CONNLMT,
    IP6_FIRSTFRAGINORDER_FILTER,
    IP6_FIRSTFRAGINORDER_AFT,
    IP6_FIRSTFRAGINORDER_LB,
    IP6_FIRSTFRAGINORDER_BUTT = 16
} IP6_FIRSTFRAGINORDER_SERVICEBIT_E;

/* 该结构用于用户态显示*/
#pragma pack(4)
ISSU typedef struct tagIP6FS_CacheShow
{
    UINT64      uiRTNode;           /* pointer to FIB/PBR */
    UINT64      uiAdjEntry;         /* pointer to adjacency table */
    USHORT      usRTSeq;            /* FIB/PBR sequence num */
    USHORT      usAdjSeq;           /* Adj sequence num */
    USHORT      usCfgSeq;           /* config sequence num */
    USHORT      usCacheSeq;         /* IP6FS cache sequence num, relating to all sub sequence num */

    VRF_INDEX   vrfIndex;           /* receive vpn */
    USHORT      usMDCID;            /* MDC ID */
    IF_INDEX    ifIndexRcv;         /* receive interface */
    IN6ADDR_S   stIP6SrcAddr;       /* source address */
    IN6ADDR_S   stIP6DstAddr;       /* dest address */
    IN6ADDR_S   stNextHop;          /* nexthop, used in qos */
    USHORT      usSrcPort;          /* source port */
    USHORT      usDstPort;          /* dest port */
    IF_INDEX    ifIndexOut;         /* output interface */
    IF_INDEX    ifIndexPort;        /* output port interface */
    USHORT      usVlanID;           /* Vlan ID */

    UCHAR       aucPrependItem[IP6FS_PREPEND_LEN];  /* link-layer head */
    UCHAR       ucPrependLen;       /* prepend length, link-layer head */
    UCHAR       ucReserve;
    UCHAR       ucForwardType;      /* forward type for flow branch */
    UCHAR       ucProtocol;         /* protocol */

    UINT        uiTunnelID;         /* tunnel ID */
    UCHAR       ucDirection;        /* direction of flow */
    UCHAR       ucStatus;           /* valid or not */
    UCHAR       ucRTType;           /* route type: FIB/PBR */
    UCHAR       ucFlag;             /* IP_FAST_SWITCH, ... */
    USHORT      usBitmap;           /* service bitmap */
    UINT64      uiSession;          /* pointer to session */
    LIP_ADDR     stDstLipNode;
} IP6FS_CACHE_SHOW_S;
#pragma pack()

/* IPv6 快转全局统计计数数据结构,用于用户态显示 */
ISSU typedef struct tagIP6FS_Stat
{
    UINT uiNotSupportCnt;       /* 快转不支持报文统计 */
    UINT uiErrPktCnt;           /* 错误报文统计 */
    UINT uiHitCacheCnt;         /* 命中快转表的报文数 */
    UINT uiNoCacheCnt;          /* 查询快转表项失败的报文数 */
    UINT uiTRSendCnt;           /* 透传的报文数 */
    UINT uiAppDropCnt;          /* 被业务接管的报文数 */
    UINT uiAppToIpFwCnt;        /* 业务处理完继续快转处理的报文数 */
    UINT uiNoMemory;            /* 分配内存失败次数 */
} IP6FS_STAT_S;

/* IPv6 快转业务私有信息数据结构,用于用户态显示 */
ISSU typedef struct tagIP6FS_ServiceInfoShow
{
    UINT  uiSeqInCache;
    UCHAR ucMID;
    UCHAR ucParaNum;
    UCHAR ucStatus;
} IP6FS_SERVICE_INFO_SHOW_S;

/* 定义命令行显示快转表项的IPv6地址过滤结构 */
ISSU typedef struct tagIP6FS_Filter
{
    UINT      uiFilterFlg;        /* 是否按照 IPv6 地址进行过滤 */
    IN6ADDR_S staddr6;            /* IPv6地址 */
} IP6FS_FILTER_S;

/* 定义配置消息类型 */
ISSU typedef enum tagIP6FS_MsgType
{
    IP6FS_CFG_DISPLAY_SERREG, 
    IP6FS_CFG_SET_AGTIME, 
    IP6FS_CFG_ENABLE_FS, 
    IP6FS_CFG_RESET_CACHE, 
    IP6FS_CFG_DISPLAY_STATISTICS, 
    IP6FS_CFG_RESET_STATISTICS, 
    IP6FS_CFG_PULL, 
    IP6FS_CFG_GET_CACHESEQ,
    IP6FS_CFG_ENABLE_LOADSHARE,
    IP6FS_CFG_BUTT
} IP6FS_MSGTYPE_E;


/* 内核态定义*/
#ifdef __KERNEL__

#ifndef _SYS_IPFS_H_
#include <sys/ipfs.h>
#endif

#ifndef _SYS_IP6FW_H_
#include <sys/ip6fw.h>
#endif

/* 组播快转表类型 */
#define IP6FS_MCTYPE_IN              0x1
#define IP6FS_MCTYPE_OUT             0x2

/* 快转表类型 */
#define IP6FS_CACHEKEYFLAG_MACFW     0x01
#define IP6FS_CACHEKEYFLAG_GRE       0x02
#define IP6FS_CACHEKEYFLAG_BRIDGE    0x04
#define IP6FS_CACHEKEYFLAG_INLINE    0x08
/* 指定快转流程缺省业务标志位掩码 */
#define IP6FS_DEFAULT_SERVICEBITMAP    ((ULONG)-1)

/* 快转表里携带的业务标记 */
#define IP6FS_FLAG_NODELSERVICE        0x1     /* 不需要删除pservice */
#define IP6FS_FLAG_FIBSTAT             0x2     /* FIB统计标记 */
#define IP6FS_FLAG_VPNSTAT             0x4     /* VPN统计使能 */

typedef enum tagIp6fs_Service_BIT
{
    IP6FS_SERVICEBIT_TCPREASSEMBLE,
    IP6FS_SERVICEBIT_OFP,
    IP6FS_SERVICEBIT_NS, 
    IP6FS_SERVICEBIT_MGROUP,
    IP6FS_SERVICEBIT_IPSEC,
    IP6FS_SERVICEBIT_ATK,
    IP6FS_SERVICEBIT_PORTAL,  
    IP6FS_SERVICEBIT_FILTER, 
    IP6FS_SERVICEBIT_IPOE,
    IP6FS_SERVICEBIT_PRIMAP,
    IP6FS_SERVICEBIT_QOS,
    IP6FS_SERVICEBIT_FLOWMGR,
    IP6FS_SERVICEBIT_TUNNEL,
    IP6FS_SERVICEBIT_POSTOFP,
    IP6FS_SERVICEBIT_BUTT
} IP6FS_SERVICEBIT_E;

#define IP6FS_MAX_SERVICE_NUM 16UL

/* 业务标志位 Bitmap 赋值操作宏 */
#define IP6FS_SERVICE_SETBITMAP(pulBitmapM, uiServiceIDM) \
    __set_bit((IP6FS_MAX_SERVICE_NUM - 1) - (INT)uiServiceIDM, pulBitmapM);

#define IP6FS_SERVICE_CLEARBITMAP(pulBitmapM, uiServiceIDM) \
    __clear_bit((IP6FS_MAX_SERVICE_NUM - 1) - (INT)uiServiceIDM, pulBitmapM);

/* IP6FS_CACHEKEY_S defined in sys/ipfs.h */

/* IPv6快转route信息 */
typedef struct tagIP6FS_CacheRoute
{ 
    VOID     *pRTNode;          /* pointer to FIB-BASE/PBR */
    VOID     *pAdjEntry;        /* pointer to ADJ */
    USHORT   usRTSeq;           /* FIB/PBR sequence num */
    USHORT   usAdjSeq;          /* Adj sequence num */
    USHORT   usCfgSeq;          /* global config sequence num */
    UCHAR    ucForwardType;     /* forward type for flow branch */
    UCHAR    ucDirection:2;     /* direction of flow */
    UCHAR    ucDelFlag:1;       /* in hash or not */
    UCHAR    ucRTType:1;        /* route type: FIB/PBR */
    UCHAR    ucADJType:2;       /* adj type: ADJ/NHLFE/OFP, for FTN VN */
    UCHAR    ucMCType:2;        /* multicast RT-Cache, OIF-Cache */
} IP6FS_CACHEROUTE_S; /* 24 Bytes(64-bit) */

/* IPv6快转表 */
typedef struct tagIP6FS_Cache
{ 
    HASH_NODE_S         stHashNode;         /* hash node */
    IP6FS_CACHEKEY_S    stCacheKey;         /* CacheKey */
    IP6FS_CACHEROUTE_S  stCacheRoute;       /* CacheRoute */
    UINT                uiCacheSeq;         /* cache sequence num, relating to all sub sequence num */
    UCHAR               ucFlag;             /* only IPFS_FAST_SWITCH, IPFS_HALFSWITCH in use */
    UCHAR               ucStatus;           /* enable or disable */
    USHORT              usBitmap;           /* service bitmap */
    UCHAR               ucLaggHashIndex;    /*lagg 计算物理口的HASH Index*/  
    UCHAR               ucServiceFlag;      /* 业务标志 */
    UINT                uiTunnelID;         /* tunnel ID */
    UINT                uiLabel;            /* FTN label*/
    struct rcu_head     stRcu;              /* rcu node */
    VOID                *pSession;          /* Session pointer */
    VOID                *pService;          /* other service pointer, externed in IP6FS_SERVICE_S */
    VOID                *pQoS;              /* QoS pointer */
} IP6FS_CACHE_S; /* 144 Bytes(64-bit) */

typedef struct tagIp6fsService
{
    VOID                *pTCPReassemble;    /* TCP Reassemble pointer */
    VOID                *pATK;              /* ATK pointer */
    VOID                *pPortal;           /* Portal pointer */
    VOID                *pIPsec;            /* IPsec pointer */
    VOID                *pFilter;           /* Packet filter pointer */
    VOID                *pNetStream;        /* Netstream pointer */
    VOID                *pFlowMgr;          /* FlowreMgr pointer */
    VOID                *pTunnel;           /* Tunnel pointer */
    VOID                *pOFP;              /* Openflow pointer */
    VOID                *pIPoE;             /* IPoE pointer */
}IP6FS_SERVICE_S;


/* IP6 快转业务私有信息释放接口类型定义 */
typedef ULONG (*IP6FS_SERVICEINFO_RELEASE_PF)(IN VOID *pData);

/* IP6 快转业务处理接口类型定义 */
typedef ULONG (*IP6FS_SERVICEPROC_PF)(IN IF_INDEX ifIndex, 
                                      INOUT VOID *pCache, 
                                      INOUT IP6_S **ppstIP,
                                      INOUT FSBUF_PKTINFO_S *pstPktInfo,
                                      INOUT FSBUF_BLOCKINFO_S * pstBlockInfo);

typedef UCHAR (*IP6FS_GetSwitchFlag_PF)(IN UCHAR ucDirection, IN IF_INDEX ifL3Index, IN IF_INDEX ifPhyIndex);

typedef ULONG (*IP6FS_TunnelProc_PF)(IN const VOID *pCache,
                                     IN VOID *pData,
                                     INOUT FSBUF_PKTINFO_S *pstPktInfo,
                                     INOUT FSBUF_BLOCKINFO_S *pstBlockInfo);
typedef VOID (*IP6FS_SETKEY_PF)(IN VOID *pData, INOUT IP6FS_CACHEKEY_S *pstCacheKey);

/* 会话着色处理函数 */
typedef VOID (*IP6FS_SESSION_COLORPROC)(IN VOID *pSession, IN ULONG ulExtInfo);

extern VOID IP6FS_RegisterService(IN UCHAR ucServiceBit, IN IP6FS_SERVICEPROC_PF pfServiceProcFunc);
extern VOID IP6FS_UnRegisterService(IN UCHAR ucServiceBit);
extern VOID IP6FS_ServiceRelease_Register(IN UCHAR ucServiceBit, 
                                          IN IP6FS_SERVICEINFO_RELEASE_PF pfServiceInfoReleaseFunc);
extern VOID IP6FS_ServiceRelease_UnRegister(IN UCHAR ucServiceBit);

extern ULONG IP6FS_AddCache(IN MBUF_S *pstMBuf, IN USHORT usIPOffse);
extern VOID IP6FS_FreeCache(INOUT VOID *pCache);
extern VOID IP6FS_FreeCache_RCU(INOUT VOID *pCache);
extern VOID * IP6FS_SearchSession(IN const IP6FS_CACHEKEY_S *pstKey, OUT UCHAR *pucDir);
extern ULONG IP6FS_AddCachePairToHash(INOUT IP6FS_CACHE_S *pstOriCache, INOUT IP6FS_CACHE_S *pstReplyCache);
extern VOID IP6FS_DeletePairFromHash(INOUT VOID *pSession);
extern VOID IP6FS_AdjustCache(INOUT IP6FS_CACHE_S *pstCache, INOUT VOID *pSession);
extern VOID * IP6FS_AllocCache(IN UCHAR ucDir);
extern VOID * IP6FS_ScanCache(IN BOOL_T bIsWhole,
                              INOUT UINT *puiIndex,
                              INOUT UINT *puiPos,
                              INOUT BOOL_T *pbEnd);
extern VOID IP6FS_ColorSession(IN IP6FS_SESSION_COLORPROC pfSessionProc, IN ULONG ulExtInfo);
extern UINT IP6FS_GetAgingTime(VOID);
extern VOID * IP6FS_GetMdcCB(VOID);
extern VOID IP6FS_AllocCacheAndSetKey(IN VOID *pIP6FsMdcData, IN const IP6_S *pstIp6, INOUT MBUF_S *pstMBuf);
extern VOID IP6FS_AddReplyCacheRT(IN const VOID *pOriCache, INOUT VOID *pReplyCache);

extern VOID IP6FS_SetCacheBitmap(IN UCHAR ucServiceBit, INOUT VOID *pCache);
extern VOID IP6FS_ASSIGN_CACHE_FIB(IN const VOID *pFIB6FWDInfo, INOUT IP6FS_CACHE_S *pstCache);
extern VOID IP6FS_ASSIGN_CACHE_FWDTYPE(IN UCHAR ucFwdType, INOUT IP6FS_CACHE_S *pstCache);
extern VOID IP6FS_ASSIGN_CACHE_DISABLE(INOUT IP6FS_CACHE_S *pstCache);
extern VOID IP6FS_ASSIGN_MBUF_CACHE_ADJ(IN IF_INDEX ifIndexSnd, 
                                        IN const VOID *pADJ6Entry, 
                                        IN UINT32 uiADJ6SeqNo, 
                                        INOUT MBUF_S *pstMBuf);
extern VOID IP6FS_ASSIGN_MBUF_CACHE_NHLFE(IN IF_INDEX ifIndexSnd, 
                                          IN const VOID *pNhlfeEntry, 
                                          IN UINT32 uiNhlfeSeqNo, 
                                          INOUT MBUF_S *pstMBuf);
extern VOID IP6FS_ASSIGN_MBUF_CACHE_OFP(INOUT MBUF_S *pstMBuf);

/* 提供给隧道和IPSEC的函数注册接口 */
extern VOID IP6FS_RegTunnelProc(IN UCHAR ucFwdType, IN IP6FS_TunnelProc_PF pfTunnelProcPF);

extern VOID IP6FS_IncCfgSeq(VOID);
extern VOID IP6FS_IncCacheSeq(VOID);

extern ULONG IP6FS_FastSwitch_Ucore(IN IF_INDEX ifIndex, 
                                    INOUT IP6_S *pstIP6, 
                                    INOUT UCORE_KEY_S *pstUcoreKey, 
                                    INOUT FSBUF_PKTINFO_S *pstPktInfo, 
                                    INOUT FSBUF_BLOCKINFO_S *pstBlockInfo);
extern ULONG IP6FS_FastSwitch(IN IF_INDEX ifIndex, 
                              INOUT IP6_S *pstIP6, 
                              INOUT FSBUF_PKTINFO_S *pstPktInfo, 
                              INOUT FSBUF_BLOCKINFO_S *pstBlockInfo);
extern ULONG IP6FS_FastSwitch2(IN IF_INDEX ifIndex, 
                               INOUT IP6_S *pstIP6, 
                               INOUT FSBUF_PKTINFO_S *pstPktInfo, 
                               INOUT FSBUF_BLOCKINFO_S *pstBlockInfo, 
                               IN ULONG ulFWServiceBitMap,
                               IN ULONG ulFSServiceBitMap);
extern ULONG IP6FS_Output(INOUT IP6_S *pstIP6, 
                          INOUT FSBUF_PKTINFO_S *pstPktInfo, 
                          INOUT FSBUF_BLOCKINFO_S *pstBlockInfo);
extern ULONG IP6FS_Output2(INOUT IP6_S *pstIP6, 
                           INOUT FSBUF_PKTINFO_S *pstPktInfo, 
                           INOUT FSBUF_BLOCKINFO_S *pstBlockInfo, 
                           IN ULONG ulServiceBitMap);
extern ULONG IP6FS_IF_Output(INOUT IP6_S *pstIP6, 
                             IN IP6FS_CACHE_S *pstCache, 
                             INOUT FSBUF_PKTINFO_S *pstPktInfo, 
                             INOUT FSBUF_BLOCKINFO_S *pstBlockInfo);

extern ULONG IP6FS_Init(IN LPVOID pStartContext);
extern ULONG IP6FS_Run(IN LPVOID pStartContext);

extern ULONG IP6FS_MDC_EventCallback(IN UINT uiEvent, IN VOID *pData);

extern ULONG IP6FS_DelCacheByCachePtr(IN VOID *pCache);

extern VOID IP6FS_FillCacheKey(IN IF_INDEX ifIndexRcv, 
                               IN MBUF_S *pstMBuf, 
                               IN UINT uiMDCID, 
                               OUT IP6FS_CACHEKEY_S *pstCacheKey, 
                               OUT UCHAR *pucDF);

extern VOID IP6FS_CacheEntryInit(OUT IP6FS_CACHE_S *pstCache);

extern VOID IP6FS_EntryPreFree(INOUT IP6FS_CACHE_S *pstCache);

extern BOOL_T IP6FS_IsCRCHash(VOID);

extern ULONG IP6FS_GetHashIndex(IN const IP6FS_CACHEKEY_S *pstCacheKey, IN UINT uiHashMask);

extern ULONG IP6FS_FastSwitchByCache(IN IP6FS_CACHE_S *pstCache, 
                                     IN IP6_S *pstIp6, 
                                     IN IF_INDEX ifIndexSnd, 
                                     IN FSBUF_PKTINFO_S *pstPktInfo, 
                                     INOUT FSBUF_BLOCKINFO_S *pstBlockInfo);

extern ULONG IP6FS_OutputByCache(IN IP6FS_CACHE_S *pstCache, 
                                 IN IP6_S *pstIp6, 
                                 IN FSBUF_PKTINFO_S *pstPktInfo, 
                                 INOUT FSBUF_BLOCKINFO_S *pstBlockInfo);


extern ULONG IP6FS_Service_Handle(IN IF_INDEX ifIndex, 
                                  IN IP6FS_CACHE_S *pstCache, 
                                  IN IP6_S **ppstIp6, 
                                  IN FSBUF_PKTINFO_S *pstPktInfo, 
                                  IN FSBUF_BLOCKINFO_S *pstBlockInfo);


#ifndef _SYS_ASSERT_H_
#include <sys/assert.h>
#endif
static inline VOID IP6FS_SERVICE_CLEARFLAG(INOUT IP6FS_CACHE_S *pstCache, 
                                           IN UCHAR ucServiceFlag)
{   
    pstCache->ucServiceFlag &= (UCHAR)(~ucServiceFlag);

    return;
}

static inline VOID IP6FS_SERVICE_SETFLAG(INOUT IP6FS_CACHE_S *pstCache, 
                                         IN UCHAR ucServiceFlag)
{
    pstCache->ucServiceFlag |= ucServiceFlag;

    return;
}

static inline UCHAR IP6FS_SERVICE_GETFLAG(IN const IP6FS_CACHE_S *pstCache)
{
    return (pstCache->ucServiceFlag);
}

static inline IP6FS_CACHEKEY_S *IP6FS_GET_CACHE_KEY(IN IP6FS_CACHE_S *pstCache)
{
    DBGASSERT(NULL != pstCache);

    return &(pstCache->stCacheKey);
}

static inline UCHAR IP6FS_GET_CACHE_DIR(IN const IP6FS_CACHE_S *pstCache)
{
    DBGASSERT(NULL != pstCache);

    return pstCache->stCacheRoute.ucDirection;
}

static inline VOID IP6FS_ASSIGN_CACHE_DIR(IN UCHAR ucDir, OUT IP6FS_CACHE_S *pstCache)
{
    DBGASSERT(NULL != pstCache);

    pstCache->stCacheRoute.ucDirection = ucDir;

    return;
}

static inline UCHAR IP6FS_GET_CACHE_MCTYPE(IN const IP6FS_CACHE_S *pstCache)
{
    DBGASSERT(NULL != pstCache);

    return pstCache->stCacheRoute.ucMCType;
}

static inline VOID IP6FS_ASSIGN_CACHE_MCTYPE(IN UCHAR ucMCType, OUT IP6FS_CACHE_S *pstCache)
{
    DBGASSERT(NULL != pstCache);

    pstCache->stCacheRoute.ucMCType = ucMCType;

    return;
}

static inline VOID *IP6FS_GET_CACHE_SESSION(IN const IP6FS_CACHE_S *pstCache)
{
    DBGASSERT(NULL != pstCache);

    return pstCache->pSession;
}

static inline VOID IP6FS_ASSIGN_CACHE_SESSION(IN VOID *pSession, OUT IP6FS_CACHE_S *pstCache)
{
    DBGASSERT(NULL != pstCache);

    pstCache->pSession = pSession;

    return;
}

static inline VOID *IP6FS_GET_CACHE_QOS(IN const IP6FS_CACHE_S *pstCache)
{
    DBGASSERT(NULL != pstCache);

    return pstCache->pQoS;
}

static inline VOID IP6FS_ASSIGN_CACHE_QOS(IN VOID *pQoS, OUT IP6FS_CACHE_S *pstCache)
{
    DBGASSERT(NULL != pstCache);

    pstCache->pQoS = pQoS;

    return;
}

static inline IP6FS_SERVICE_S *IP6FS_GET_CACHE_SERVICE(IN IP6FS_CACHE_S *pstCache)
{
    IP6FS_SERVICE_S *pstService = NULL;

    DBGASSERT(NULL != pstCache);

    if (NULL == pstCache->pService)
    {
        pstService = (IP6FS_SERVICE_S*)kzalloc_tag(sizeof(IP6FS_SERVICE_S), GFP_ATOMIC, MID_IP6FS);
        if (NULL == pstService)
        {
            return NULL;
        }
        pstCache->pService = (VOID*)pstService;
    }
    else
    {
        pstService = (IP6FS_SERVICE_S *)pstCache->pService;
    }

    return pstService;
}

static inline VOID *IP6FS_GET_CACHE_TCPREASSEMBLE(IN const IP6FS_CACHE_S *pstCache)
{
    DBGASSERT(NULL != pstCache);

    if (NULL == pstCache->pService)
    {
        return NULL;
    }

    return ((IP6FS_SERVICE_S *)(pstCache->pService))->pTCPReassemble;
}

static inline ULONG IP6FS_ASSIGN_CACHE_TCPREASSEMBLE(IN VOID *pTCPReassemble, OUT IP6FS_CACHE_S *pstCache)
{
    IP6FS_SERVICE_S *pstService = NULL;

    DBGASSERT(NULL != pstCache);

    pstService = IP6FS_GET_CACHE_SERVICE(pstCache);
    if (NULL == pstService)
    {
        return ERROR_FAILED;
    }

    pstService->pTCPReassemble = pTCPReassemble;

    return ERROR_SUCCESS;
}

static inline VOID *IP6FS_GET_CACHE_ATK(IN const IP6FS_CACHE_S *pstCache)
{
    DBGASSERT(NULL != pstCache);

    if (NULL == pstCache->pService)
    {
        return NULL;
    }

    return ((IP6FS_SERVICE_S *)(pstCache->pService))->pATK;
}

static inline ULONG IP6FS_ASSIGN_CACHE_ATK(IN VOID *pATK, OUT IP6FS_CACHE_S *pstCache)
{
    IP6FS_SERVICE_S *pstService = NULL;

    DBGASSERT(NULL != pstCache);

    pstService = IP6FS_GET_CACHE_SERVICE(pstCache);
    if (NULL == pstService)
    {
        return ERROR_FAILED;
    }

    pstService->pATK = pATK;

    return ERROR_SUCCESS;
}

static inline VOID *IP6FS_GET_CACHE_IPOE(IN const IP6FS_CACHE_S *pstCache)
{
    DBGASSERT(NULL != pstCache);

    if (NULL == pstCache->pService)
    {
        return NULL;
    }

    return ((IP6FS_SERVICE_S *)(pstCache->pService))->pIPoE;
}

static inline ULONG IP6FS_ASSIGN_CACHE_IPOE(IN VOID *pIPoE, OUT IP6FS_CACHE_S *pstCache)
{
    IP6FS_SERVICE_S *pstService = NULL;

    DBGASSERT(NULL != pstCache);

    pstService = IP6FS_GET_CACHE_SERVICE(pstCache);
    if (NULL == pstService)
    {
        return ERROR_FAILED;
    }

    pstService->pIPoE = pIPoE;

    return ERROR_SUCCESS;
}


static inline VOID *IP6FS_GET_CACHE_PORTAL(IN const IP6FS_CACHE_S *pstCache)
{
    DBGASSERT(NULL != pstCache);

    if (NULL == pstCache->pService)
    {
        return NULL;
    }

    return ((IP6FS_SERVICE_S *)(pstCache->pService))->pPortal;
}

static inline ULONG IP6FS_ASSIGN_CACHE_PORTAL(IN VOID *pPortal, OUT IP6FS_CACHE_S *pstCache)
{
    IP6FS_SERVICE_S *pstService = NULL;

    DBGASSERT(NULL != pstCache);

    pstService = IP6FS_GET_CACHE_SERVICE(pstCache);
    if (NULL == pstService)
    {
        return ERROR_FAILED;
    }

    pstService->pPortal = pPortal;

    return ERROR_SUCCESS;
}

static inline VOID *IP6FS_GET_CACHE_IPSEC(IN const IP6FS_CACHE_S *pstCache)
{
    DBGASSERT(NULL != pstCache);

    if (NULL == pstCache->pService)
    {
        return NULL;
    }

    return ((IP6FS_SERVICE_S *)(pstCache->pService))->pIPsec;
}

static inline ULONG IP6FS_ASSIGN_CACHE_IPSEC(IN VOID *pIPsec, OUT IP6FS_CACHE_S *pstCache)
{
    IP6FS_SERVICE_S *pstService = NULL;

    DBGASSERT(NULL != pstCache);

    pstService = IP6FS_GET_CACHE_SERVICE(pstCache);
    if (NULL == pstService)
    {
        return ERROR_FAILED;
    }

    pstService->pIPsec = pIPsec;

    return ERROR_SUCCESS;
}

static inline VOID *IP6FS_GET_CACHE_FILTER(IN const IP6FS_CACHE_S *pstCache)
{
    DBGASSERT(NULL != pstCache);

    if (NULL == pstCache->pService)
    {
        return NULL;
    }

    return ((IP6FS_SERVICE_S *)(pstCache->pService))->pFilter;
}

static inline ULONG IP6FS_ASSIGN_CACHE_FILTER(IN VOID *pFilter, OUT IP6FS_CACHE_S *pstCache)
{
    IP6FS_SERVICE_S *pstService = NULL;

    DBGASSERT(NULL != pstCache);

    pstService = IP6FS_GET_CACHE_SERVICE(pstCache);
    if (NULL == pstService)
    {
        return ERROR_FAILED;
    }

    pstService->pFilter = pFilter;

    return ERROR_SUCCESS;
}

static inline VOID *IP6FS_GET_CACHE_NETSTREAM(IN const IP6FS_CACHE_S *pstCache)
{
    DBGASSERT(NULL != pstCache);

    if (NULL == pstCache->pService)
    {
        return NULL;
    }

    return ((IP6FS_SERVICE_S *)(pstCache->pService))->pNetStream;
}

static inline ULONG IP6FS_ASSIGN_CACHE_NETSTREAM(IN VOID *pNetStream, OUT IP6FS_CACHE_S *pstCache)
{
    IP6FS_SERVICE_S *pstService = NULL;

    DBGASSERT(NULL != pstCache);

    pstService = IP6FS_GET_CACHE_SERVICE(pstCache);
    if (NULL == pstService)
    {
        return ERROR_FAILED;
    }

    pstService->pNetStream = pNetStream;

    return ERROR_SUCCESS;
}

static inline VOID *IP6FS_GET_CACHE_FLOWMGR(IN const IP6FS_CACHE_S *pstCache)
{
    DBGASSERT(NULL != pstCache);

    if (NULL == pstCache->pService)
    {
        return NULL;
    }

    return ((IP6FS_SERVICE_S *)(pstCache->pService))->pFlowMgr;
}

static inline ULONG IP6FS_ASSIGN_CACHE_FLOWMGR(IN VOID *pFlowMgr, OUT IP6FS_CACHE_S *pstCache)
{
    IP6FS_SERVICE_S *pstService = NULL;

    DBGASSERT(NULL != pstCache);

    pstService = IP6FS_GET_CACHE_SERVICE(pstCache);
    if (NULL == pstService)
    {
        return ERROR_FAILED;
    }
    pstService->pFlowMgr = pFlowMgr;

    return ERROR_SUCCESS;
}

static inline VOID *IP6FS_GET_CACHE_TUNNEL(IN const IP6FS_CACHE_S *pstCache)
{
    DBGASSERT(NULL != pstCache);

    if (NULL == pstCache->pService)
    {
        return NULL;
    }

    return ((IP6FS_SERVICE_S *)(pstCache->pService))->pTunnel;
}

static inline ULONG IP6FS_ASSIGN_CACHE_TUNNEL(IN VOID *pTunnel, OUT IP6FS_CACHE_S *pstCache)
{
    IP6FS_SERVICE_S *pstService = NULL;

    DBGASSERT(NULL != pstCache);

    pstService = IP6FS_GET_CACHE_SERVICE(pstCache);
    if (NULL == pstService)
    {
        return ERROR_FAILED;
    }
    
    pstService->pTunnel = pTunnel;
    return ERROR_SUCCESS;
}

static inline UCHAR IP6FS_GET_CACHE_LAGGINDEX (IN const IP6FS_CACHE_S *pstCache)
{
    return pstCache->ucLaggHashIndex;
}

static inline VOID IP6FS_ASSIGN_CACHE_LAGGINDEX (IN UCHAR ucLaggHashIndex, OUT IP6FS_CACHE_S *pstCache)
{
    pstCache->ucLaggHashIndex = ucLaggHashIndex;
    return;
}

static inline VOID *IP6FS_GET_CACHE_OFP(IN const IP6FS_CACHE_S *pstCache)
{
    DBGASSERT(NULL != pstCache);

    if (NULL == pstCache->pService)
    {
        return NULL;
    }

    return ((IP6FS_SERVICE_S *)(pstCache->pService))->pOFP;
}

static inline ULONG IP6FS_ASSIGN_CACHE_OFP(IN VOID *pOFP, OUT IP6FS_CACHE_S *pstCache)
{
    IP6FS_SERVICE_S *pstService = NULL;

    DBGASSERT(NULL != pstCache);

    pstService = IP6FS_GET_CACHE_SERVICE(pstCache);
    if (NULL == pstService)
    {
        return ERROR_FAILED;
    }

    pstService->pOFP = pOFP;

    return ERROR_SUCCESS;
}

/* 判断快转表记录的邻接表字段是否为邻接表指针 */
static inline BOOL_T IP6FS_IS_ADJENTRY(IN const IP6FS_CACHE_S *pstCache)
{
    const IP6FS_CACHEROUTE_S *pstCacheRoute = &(pstCache->stCacheRoute);
    UCHAR ucForwardType = pstCacheRoute->ucForwardType;

    if ((NULL != pstCacheRoute->pAdjEntry) && 
        (IPFS_TUNNEL_DECAP != ucForwardType) && 
        (IPFS_IPSEC_DECAP != ucForwardType) &&
        (IPFS_IPSEC_ENCAP != ucForwardType) &&
        (IPFS_FLOWMGR != ucForwardType) &&
        (IPFS_ADJTYPE_NHLFE != pstCacheRoute->ucADJType))
    {
        return BOOL_TRUE;
    }
    return BOOL_FALSE;
}

extern ULONG IP6FS_AllocCacheAndSetKeyL2(IN const IP6FS_SETKEY_PF pfSetKey, 
                                                     IN VOID *pData, 
                                                     INOUT MBUF_S *pstMBuf);

extern IP6FS_CACHE_S * IP6FS_SearchCacheL2(IN const IP6FS_SETKEY_PF pfSetKey, 
                                           IN VOID *pData,
                                           IN FSBUF_PKTINFO_S *pstPktInfo,
                                           IN FSBUF_BLOCKINFO_S *pstBlockInfo,
                                           IN VOID *pIP6FsMdcData,
                                           OUT IPFS_FRAGBUF_S **ppstBufList,
                                           OUT ULONG *pulRet);
IP6FS_CACHE_S * IP6FS_SearchCacheL2NoFrag(IN const IP6FS_SETKEY_PF pfSetKey, 
                                          IN VOID *pData,
                                          IN FSBUF_PKTINFO_S *pstPktInfo,
                                          IN FSBUF_BLOCKINFO_S *pstBlockInfo,
                                          IN const VOID *pIP6FsMdcData);
extern IP6FS_CACHE_S * IP6FS_CACHE_ALLOC(VOID);
extern VOID IP6FS_CACHE_FREE(IN IP6FS_CACHE_S * pstCache);
extern ULONG IP6FS_ReplaceCache(INOUT IP6FS_CACHE_S *pstOldCache, INOUT IP6FS_CACHE_S *pstCache);
extern VOID IP6FS_FillCacheKey_Raw(IN IP6_S *pstIp6, 
                                   IN FSBUF_PKTINFO_S *pstPktInfo, 
                                   IN const UINT uiMDCID, 
                                   INOUT IP6FS_CACHEKEY_S *pstCacheKey);

#endif /* end of __KERNEL__ */

#ifdef  __cplusplus
}
#endif  /* end of __cplusplus */

#endif  /* end of _SYS_IP6FS_H_ */
