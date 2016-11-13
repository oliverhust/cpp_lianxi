/*****************************************************************************
 Copyright (c) 2010, Hangzhou H3C Technologies Co., Ltd. All rights reserved.
------------------------------------------------------------------------------
                               vpn.h

  Project Code: Comware Leopard
   Module Name: L3VPN
  Date Created: 2010-07-13
        Author: wangjiangbo
   Description: L3VPN外部公共头文件

------------------------------------------------------------------------------
Modification History
  DATE        NAME              DESCRIPTION
  2010-07-13  wangjiangbo          create
------------------------------------------------------------------------------

*****************************************************************************/
#ifndef _L3VPN_H_
#define _L3VPN_H_

#ifdef  __cplusplus
    extern "C"{
#endif

#ifndef _DRV_L3VPN_H_
#include <drv/l3vpn.h>
#endif

/* L3VPN侦听端口 */


#define L3VPN_VRF_CREATE      0x00000001    /* VRF创建事件 */
#define L3VPN_VRF_DELETE      0x00000002    /* VRF删除事件 */
#define L3VPN_INTF_BIND       0x00000004    /* 接口绑定事件 */
#define L3VPN_INTF_UNBIND     0x00000008    /* 接口去绑定事件 */

#define L3VPN_RD_ASSIGN       0x00000010    /* RD分配事件 */
#define L3VPN_RD_DELETE       0x00000020    /* RD删除事件 */

#define L3VPN_IRT_CHANGE         0x00000040    /* IPv4 RT入策略变化事件 */
#define L3VPN_ERT_CHANGE         0x00000080    /* IPv4 RT出策略变化事件 */

#define L3VPN_IRP_CHANGE      0x00000400    /* IPv4 入路由策略变化事件 */
#define L3VPN_ERP_CHANGE      0x00000800    /* IPv4 出路由策略变化事件 */
#define L3VPN_TP_CHANGE       0x00001000    /* IPv4 隧道策略变化事件 */
#define L3VPN_MAX_LIMIT       0x00002000    /* IPv4 VPN下最大路由限制变化事件 */

#define L3VPN_IRT6_CHANGE        0x00004000    /* IPv6 RT入策略变化事件 */
#define L3VPN_ERT6_CHANGE        0x00008000    /* IPv6 RT出策略变化事件 */


#define L3VPN_IRP6_CHANGE     0x00040000    /* IPv6 入路由策略变化事件 */
#define L3VPN_ERP6_CHANGE     0x00080000    /* IPv6 出路由策略变化事件 */
#define L3VPN_TP6_CHANGE      0x00100000    /* IPv6 隧道策略变化事件 */
#define L3VPN_MAX6_LIMIT      0x00200000    /* IPv6 VPN下最大路由限制变化事件 */


#define L3VPN_BATCH_BEGIN     0x00400000    /* 批量通知开始，无需注册 */
#define L3VPN_BATCH_END       0x00800000    /* 批量通知结束，无需注册 */
#define L3VPN_EVENT           0x00FFFFFF

#define MT_EVENT_FLAG         0x01000000    /* 多拓扑事件 */
#define L3VPN_RT_ADD       0x01          /* RT添加标志 */
#define L3VPN_RT_DEL       0x02          /* RT删除标志 */

#define L3VPN_MAX_RT_NTFY_NUM          8     /* VRF实例单次可配置的最大RT个数 */

#define L3VPN_MAX_RDRT_LEN             8UL    /* RD/RT数值存储长度，注意此为数值，非字符串 */

#define L3VPN_MAX_VRFNAME_LEN          31UL   /* VRF名字最大长度 */
#define L3VPN_MAX_ROUTEPOLICYNAME_LEN  63UL   /* 出入方向策略名最大长度*/
#define L3VPN_MAX_TNLPOLICYNAME_LEN    19UL   /* 隧道策略名最大长度 */
#define L3VPN_RTRD_NAME_MAX_LEN        21     /* RD/RT字符串最大长度 */
#define L3VPN_RTRD_NAME_MIN_LEN        3      /* 有效RD/RT字符串的最小长度 */
#define L3VPN_MAX_SNMP_CONTEXT_LEN     32UL   /* snmp context字符串最大长度 */

#define L3VPN_MAX_FEATURE_NAME  15U


#define L3VPN_INVALID_VRF_INDEX     ((VRF_INDEX)(~0))    /* 无效VRF索引值 */
#define L3VPN_INVALID_ROUTE_LIMIT   ((UINT)(~0))  /* 无效的每VPN最大路有数 */
#define L3VPN_INVALID_THRESHOLD     ((USHORT)(~0))  /* 无效的最大路有数阈值 */
#define L3VPN_INVALID_RTRD_VALUE    (0xFF)
#define L3VPN_INVALID_ORIGID        ((UINT)(~0))  /* 无效的origin id */
#define L3VPN_TRAP_KEY "l3vpn"

ISSU typedef struct tagL3VPN_InstanceMsg
{
    VRF_INDEX   vrfIndex;
    CHAR        szVPNName[L3VPN_MAX_VRFNAME_LEN+1];
    UINT64      auiDrvContext[L3VPN_DRV_CONTEXT_LEN];
}L3VPN_INSTANCEMSG_S;

ISSU typedef struct tagL3VPN_BindMsg
{
    VRF_INDEX   vrfIndex;
    IF_INDEX    ifIndex;
    UINT64      auiDrvContext[L3VPN_DRV_CONTEXT_LEN];
}L3VPN_BINDMSG_S;

ISSU typedef struct tagL3VPN_ResvVlanMsg
{
    VRF_INDEX   vrfIndex;
    USHORT      usReserveVlan;  /* 保留vlan id，VLAN_INVALID_VALUE表示无效值 */
    UINT64      auiDrvContext[L3VPN_DRV_CONTEXT_LEN];
}L3VPN_RESVVLANMSG_S;

ISSU typedef struct tagL3VPN_InstanceShow
{
    VRF_INDEX   vrfIndex;                            /* VPN ID */
    USHORT      usEpoch;                             /* 表项时戳 */
    CHAR        szVPNName[L3VPN_MAX_VRFNAME_LEN+1];    /* VPN实例名 */
    UINT64      auiDrvContext[L3VPN_DRV_CONTEXT_LEN];/* 驱动索引 */
    USHORT      usReserveVlan;                       /* 保留vlan id */
}L3VPN_INSTANCESHOW_S;

ISSU typedef struct tagL3VPN_BindShow
{
    IF_INDEX    ifIndex;
    VRF_INDEX   vrfIndex;
    USHORT      usEpoch;
}L3VPN_BINDSHOW_S;

#define L3VPN_APP_BATCHCFGMSG          (0xFF-3)
#define L3VPN_APP_CFGSMOOTHSTART       (0xFF-2)  /* 配置批量开始 */
#define L3VPN_APP_CFGSMOOTHEND         (0xFF-1)  /* 配置批量结束 */
#define L3VPN_APP_CFGMSG               0xFF      /* 应用程序产生伪配置消息 */

#define L3VPN_APP_IFBIND               0x30
#define L3VPN_APP_VRF                  0x40 

/* 该结构同l3v_cfg.c文件中的L3VPN_CFG_IFBIND_S结构相同 */
ISSU typedef struct tagL3VPN_APP_IFBIND
{
    IF_INDEX ifIndex;                                   /* 接口索引 */
    CHAR     szVrfName[L3VPN_MAX_VRFNAME_LEN+1];        /* VPN实例名字 */
    BOOL_T   bIsUndo;
    UINT     uiOrigID;                                  /* 绑定源模块ID */
}L3VPN_APP_IFBIND_S;

/* 该结构同l3v_cfg.c文件中的L3VPN_CFG_VRF_S结构相同 */
ISSU typedef struct  tagL3VPN_APP_VRF
{
    CHAR       szVrfName[L3VPN_MAX_VRFNAME_LEN+1];      /* VPN实例名字 */
    VRF_INDEX  vrfIndex;                                /* VPN实例索引 */   
    BOOL_T     bIsUndo;
    UINT       uiOrigID;                                /* 源模块ID */
}L3VPN_APP_VRF_S;

/* L3VPN事件处理回调函数类型定义 */
typedef ULONG (*L3VPN_CALLBACK_FUNC_PF)(IN UINT uiEvent, IN VOID *pData, IN VOID *pUserData);


ISSU typedef struct tagL3VPN_MAXROUTE_LIMIT
{
    UINT  uiMaxLimit;       /* VPN下最大激活路由限制 */
    USHORT usThreshHold;    /* VPN下激活路由告警百分比 */
}L3VPN_MAXROUTE_LIMIT_S;


ISSU typedef struct tagL3VPN_VRFIF
{
    VRF_INDEX vrfIndex;     /* 接口VRF索引，去绑定时为接口老的vrf索引 */
    IF_INDEX  ifIndex;     /* 接口索引 */
} L3VPN_VRFIF_S;


ISSU typedef struct tagL3VPN_RT_CHG
{
    UCHAR     ucCfgType;                                         /* RT事件具体类型,添加删除 */
    VRF_INDEX vrfIndex;                                          /* VRF索引 */
    UINT      uiRTNum;
    UCHAR     aucRTList[L3VPN_MAX_RT_NTFY_NUM][L3VPN_MAX_RDRT_LEN];    /* RT列表 */
} L3VPN_RT_CHG_S;


ISSU typedef struct tagL3VPN_VRF_CHG
{
    VRF_INDEX vrfIndex;                                 /* VRF索引 */
    union {
        UCHAR   aucRD[L3VPN_MAX_RDRT_LEN];               /* VRF RD值 */
        CHAR    szVrfName[L3VPN_MAX_VRFNAME_LEN+1];       /* VRF名字 */
        CHAR    szRP[L3VPN_MAX_ROUTEPOLICYNAME_LEN+1];    /* 出路由策略 */
        CHAR    szTP[L3VPN_MAX_TNLPOLICYNAME_LEN+1];      /* 隧道策略 */
        L3VPN_MAXROUTE_LIMIT_S stMaxRoute;              /* VPN下激活路由限制 */
    } unInfo;
}L3VPN_VRF_CHG_S;



/* RT值检查宏 */
#define L3VPN_RDRT_ISVALID(szValue) \
(\
    ((*((UINT*)((VOID*)(szValue)))) != 0xFFFFFFFF) \
  ||((*((UINT*)((VOID*)((szValue) + sizeof(UINT))))) != 0xFFFFFFFF) \
)


/* 路由限制检查宏 */
#define L3VPN_ROUTELIMIT_ISVALID(pstMaxRoute) \
(\
    (((pstMaxRoute)->uiMaxLimit != L3VPN_INVALID_ROUTE_LIMIT) \
       || ((pstMaxRoute)->usThreshHold != L3VPN_INVALID_THRESHOLD)) \
)

#ifndef __KERNEL__

/**************************************************************************
       Format: INT L3VPN_RegEvt(IN CHAR *pcFeatureName,
                 IN UINT uiEvType,
                 IN UCHAR ucPriority,
                 IN L3VPN_CALLBACK_FUNC_PF pfNotify,
                 IN VOID *pUserData)
 Date Created: 2010-07-13
       Author: wenchenhua
        Input: pcFeatureName  特性名
               uiEvType       关心的事件类型
               ucPriority     优先级
               pfNotify       事件处理回调
               pUserData      用户数据
       Output: 无
       Return: 成功返回LIPC连接句柄，失败返回-1
      Caution: 无
Enter context: 无
 Exit context: 无
  Locking seq: 无
  Description: L3VPN注册处理接口
--------------------------------------------------------------------------
  Modification History
  DATE          NAME       DESCRIPTION
--------------------------------------------------------------------------
  2010-7-13    wenchenhua  Create
**************************************************************************/
FROZEN_IMPL INT L3VPN_RegEvt(IN const CHAR *pcFeatureName,
                 IN UINT uiEvType,
                 IN UCHAR ucPriority,
                 IN L3VPN_CALLBACK_FUNC_PF pfNotify,
                 IN VOID *pUserData);

/**************************************************************************
       Format: VOID L3VPN_DeregEvent(IN INT iHandle)
 Date Created: 2010-07-13
       Author: wenchenhua
        Input: iHandle  文件句柄
       Output: 无
       Return: 无
      Caution: 无
Enter context: 无
 Exit context: 无
  Locking seq: 无
  Description:  用户态应用模块向L3VPN去注册
--------------------------------------------------------------------------
  Modification History
  DATE          NAME       DESCRIPTION
--------------------------------------------------------------------------
  2010-7-13    wenchenhua  Create
**************************************************************************/
FROZEN_IMPL VOID L3VPN_DeregEvent(IN INT iHandle);

/**************************************************************************
       Format: ULONG L3VPN_ProcEvt(IN INT iHandle)
 Date Created: 2010-07-13
       Author: wenchenhua
        Input: iHandle  文件句柄
       Output: 无
       Return: ERROR_SUCCESS 处理成功  ERROR_FAILED 处理失败
      Caution: 无
Enter context: 无
 Exit context: 无
  Locking seq: 无
  Description: 用户态应用模块感知到FD变化后通过L3VPN提供的接口处理VPN事件
--------------------------------------------------------------------------
  Modification History
  DATE          NAME       DESCRIPTION
--------------------------------------------------------------------------
  2010-7-13    wenchenhua  Create
**************************************************************************/
FROZEN_IMPL ULONG L3VPN_ProcEvt(IN INT iHandle);


/**************************************************************************
       Format: ULONG L3VPN_RequestBatch(IN INT iHandle, IN VRF_INDEX vrfIndex)
 Date Created: 2010-07-13
       Author: wenchenhua
        Input: iHandle  文件句柄
               vrfIndex VRF索引
       Output: 无
       Return: ERROR_SUCCESS 获取成功
               ERROR_FAILED 获取失败
      Caution: 无
Enter context: 无
 Exit context: 无
  Locking seq: 无
  Description: 用户态应用模块批量获取VPN事件,vrfIndex为无效值标识获取所有
               VPN，否则获取指定VPN。
--------------------------------------------------------------------------
  Modification History
  DATE          NAME       DESCRIPTION
--------------------------------------------------------------------------
  2010-7-13    wenchenhua  Create
**************************************************************************/
FROZEN_IMPL ULONG L3VPN_RequestBatch(IN INT iHandle, IN VRF_INDEX vrfIndex);

/**************************************************************************
       Format: ULONG L3VPN_FinalStatusNotify(IN INT iHandle, VRF_INDEX vrfIndex, UINT uiEvType)
 Date Created: 2010-07-13
       Author: wenchenhua
        Input: iHandle    文件句柄
               uiEvType   L3VPN事件
               vrfIndex   vrf索引
       Output: 无
       Return: ERROR_SUCCESS   ERROR_FAILED
      Caution: 无
Enter context: 无
 Exit context: 无
  Locking seq: 无
  Description: 用户态应用模块处理VPN事件完成后使用L3VPN提供的接口通知L3VPN
--------------------------------------------------------------------------
  Modification History
  DATE          NAME       DESCRIPTION
--------------------------------------------------------------------------
  2010-7-13    wenchenhua  Create
**************************************************************************/
FROZEN_IMPL ULONG L3VPN_FinalStatusNotify(IN INT iHandle, IN VRF_INDEX vrfIndex, IN UINT uiEvType);


/**************************************************************************
       Format: ULONG L3VPN_RdValueToString(IN UCHAR *pucRd, OUT CHAR *pucRdName)
 Date Created: 2010-07-13
       Author: wenchenhua
        Input: pucRd      RD值
       Output: pucRdName  转换后的字符串
       Return: ERROR_SUCCESS 转换成功  ERROR_INVALID_PARAMETER 转换失败
      Caution: 无
Enter context: 无
 Exit context: 无
  Locking seq: 无
  Description: 将RD值转换为字符串形式
--------------------------------------------------------------------------
  Modification History
  DATE          NAME       DESCRIPTION
--------------------------------------------------------------------------
  2010-7-13    wenchenhua  Create
**************************************************************************/
ULONG L3VPN_RdValueToString(IN const UCHAR *pucRd, OUT CHAR *pcRdName);

/**************************************************************************
       Format:  ULONG L3VPN_RdStringToValue(IN CHAR *pucRdName, OUT UCHAR *pucRd)
 Date Created: 2010-07-13
       Author: wenchenhua
        Input: pucRdName  RD字符串
       Output: pucRd      RD值
       Return: ERROR_SUCCESS 转换成功  ERROR_INVALID_PARAMETER 转换失败
      Caution: 无
Enter context: 无
 Exit context: 无
  Locking seq: 无
  Description: 将RD字符串形式转换为值
--------------------------------------------------------------------------
  Modification History
  DATE          NAME       DESCRIPTION
--------------------------------------------------------------------------
  2010-7-13    wenchenhua  Create
**************************************************************************/
ULONG L3VPN_RdStringToValue(IN const CHAR *pcRdName, OUT UCHAR *pucRd);

/**************************************************************************
       Format: ULONG L3VPN_RtValueToString(IN UCHAR *pucRt, OUT CHAR *pucRtName)
 Date Created: 2010-07-13
       Author: wenchenhua
        Input: pucRt       RT值
       Output: pucRtName   转换后的字符串
       Return: ERROR_SUCCESS 转换成功  ERROR_INVALID_PARAMETER 转换失败
      Caution: 无
Enter context: 无
 Exit context: 无
  Locking seq: 无
  Description: 将RT值转换为字符串形式
--------------------------------------------------------------------------
  Modification History
  DATE          NAME       DESCRIPTION
--------------------------------------------------------------------------
  2010-7-13    wenchenhua  Create
**************************************************************************/
ULONG L3VPN_RtValueToString(IN const UCHAR *pucRt, OUT CHAR *pcRtName);


/**************************************************************************
       Format: ULONG L3VPN_RtStringToValue(IN CHAR *pucRtName, OUT UCHAR *pucRt)
 Date Created: 2010-07-13
       Author: wenchenhua
        Input: pucRtName    RT字符串
       Output: pucRt        RT值
       Return: ERROR_SUCCESS 转换成功  ERROR_INVALID_PARAMETER 转换失败
      Caution: 无
Enter context: 无
 Exit context: 无
  Locking seq: 无
  Description: 将RT字符串形式转换为值
--------------------------------------------------------------------------
  Modification History
  DATE          NAME       DESCRIPTION
--------------------------------------------------------------------------
  2010-7-13    wenchenhua  Create
**************************************************************************/
ULONG L3VPN_RtStringToValue(IN const CHAR *pcRtName, OUT UCHAR *pucRt);

/**************************************************************************
       Format: L3VPN_SendCFGForAPP
 Date Created: 2012-06-13
       Author: q01812
        Input: uiModuleID  - 发送配置消息的模块ID
               ucMsgType   - 消息类型
               pUserData   - 用户消息数据
       Output: 无
       Return: ERROR_SUCCESS 获取成功
               ERROR_FAILED 获取失败
  Description: 用户态应用模块向L3VPN发送配置数据，目前仅支持接口绑定VPN功能
--------------------------------------------------------------------------
  Modification History
  DATE          NAME       DESCRIPTION
--------------------------------------------------------------------------
  2010-7-13    wenchenhua  Create
**************************************************************************/
ULONG L3VPN_SendCFGForAPP(
                      IN UCHAR ucMsgType,
                      IN UINT  uiModuleID,
                      IN const VOID  *pUserData,
                      IN UINT  uiDataLen);

/*****************************************************************************
    Func Name: L3VPN_Init
 Date Created: 2013/4/25
       Author: z01670
  Description: 建立与L3VPN的连接
        Input: NONE
       Output: NONE
       Return: INT
Enter context: 无
 Exit context: 无
  Locking seq: 无
      Caution: 用于向L3VPN进程添加配置
------------------------------------------------------------------------------
   Modification History
   DATE        NAME               DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
INT L3VPN_Init(VOID);

/*****************************************************************************
    Func Name: L3VPN_Fini
 Date Created: 2013/4/25
       Author: z01670
  Description: 关闭与L3VPN的连接
        Input: IN INT iHandle
       Output: NONE
       Return: VOID
Enter context: 无
 Exit context: 无
  Locking seq: 无
      Caution: 无
------------------------------------------------------------------------------
   Modification History
   DATE        NAME               DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
VOID L3VPN_Fini(IN INT iHandle);

/*****************************************************************************
    Func Name: L3VPN_BatchBinding
 Date Created: 2013/4/25
       Author: z01670
  Description: 批量接口绑定
        Input: IN UINT uiModuleID
               IN L3VPN_APP_BATCHBIND_S *pstUserData[]
               IN UINT  uiCount
       Output: NONE
       Return: ULONG
Enter context: 无
 Exit context: 无
  Locking seq: 无
      Caution: 必须是长连接
------------------------------------------------------------------------------
   Modification History
   DATE        NAME               DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
ULONG L3VPN_BatchBinding(IN UINT uiModuleID, IN UINT  uiCount, IN const L3VPN_APP_IFBIND_S *pstUserData);

/**************************************************************************
       Format: L3VPN_ServiceStart
 Date Created: 2013-11-06
       Author: z09050
        Input: 无
       Output: 无
       Return: ERROR_SUCCESS 启动成功或已启动  ERROR_FAILED 启动失败
      Caution: 无
Enter context: 无
 Exit context: 无
  Locking seq: 无
  Description: 无
--------------------------------------------------------------------------
  Modification History
  DATE          NAME       DESCRIPTION
--------------------------------------------------------------------------
  2013-11-06    z09050  Create
**************************************************************************/
ULONG L3VPN_ServiceStart(VOID);

#endif /* __KERNEL__ */

#ifdef __KERNEL__
/**************************************************************************
       Format: ULONG L3VPN_GetReserveVlanByVrfIndex(IN IF_INDEX ifIndex)
 Date Created: 2012-03-21
       Author: f07574
        Input: vrfIndex       VRF索引
       Output: 无
       Return: 保留VLAN值
      Caution: 无
Enter context: 无
 Exit context: 无
  Locking seq: 无
  Description: 返回保留VLAN，VLAN_INVALID_VALUE表示未配置保留VLAN
--------------------------------------------------------------------------
  Modification History
  DATE          NAME       DESCRIPTION
--------------------------------------------------------------------------

**************************************************************************/
USHORT L3VPN_GetReserveVlanByVrfIndex(IN VRF_INDEX vrfIndex);
#endif

/**************************************************************************
       Format: USHORT L3VPN_GetMaxVrf(VOID)
 Date Created: 2010-07-13
       Author: wenchenhua
        Input: 无
       Output: 无
       Return: 最大VRF个数
      Caution: 无
Enter context: 无
 Exit context: 无
  Locking seq: 无
  Description: 应用模块获取系统所支持的VPN最大规格，包含公网实例0
               此规格同时也是系统VRF ID的最大值+1
--------------------------------------------------------------------------
  Modification History
  DATE          NAME       DESCRIPTION
--------------------------------------------------------------------------
  2010-7-13    wenchenhua  Create
**************************************************************************/
USHORT L3VPN_GetMaxVrf(VOID);

/**************************************************************************
       Format: ULONG L3VPN_GetVrfIndexByName(IN CHAR * pcVrfName,
                                             OUT VRF_INDEX *pusVrfIndex)
 Date Created: 2010-07-13
       Author: wenchenhua
        Input: pcVrfName    VPN实例名
       Output: pusVrfIndex  接口索引
       Return: ERROR_SUCCESS   ERROR_FAILED
      Caution: 无
Enter context: 无
 Exit context: 无
  Locking seq: 无
  Description: 通过VPN实例名获取绑定在其上的所有接口索引
--------------------------------------------------------------------------
  Modification History
  DATE          NAME       DESCRIPTION
--------------------------------------------------------------------------
  2010-7-13    wenchenhua  Create
**************************************************************************/
FROZEN_IMPL ULONG L3VPN_GetVrfIndexByName(IN const CHAR * pcVrfName, OUT VRF_INDEX *pvrfIndex);

/**************************************************************************
       Format: ULONG L3VPN_GetNameByVrfIndex(IN VRF_INDEX usVrfIndex,
                                            IN UINT uiNameBufLen,
                                            OUT CHAR * pcVrfName)
 Date Created: 2010-07-13
       Author: wenchenhua
        Input: usVrfIndex  接口索引
                 uiNameBufLen  名字缓冲区长度,包含'\0'结束符
       Output: pcVrfName   VPN实例名
       Return: ERROR_SUCCESS   ERROR_FAILED
      Caution: 无
Enter context: 无
 Exit context: 无
  Locking seq: 无
  Description: 通过接口索引获取其绑定的VPN实例名
--------------------------------------------------------------------------
  Modification History
  DATE          NAME       DESCRIPTION
--------------------------------------------------------------------------
  2010-7-13    wenchenhua  Create
**************************************************************************/
FROZEN_IMPL ULONG L3VPN_GetNameByVrfIndex(IN VRF_INDEX vrfIndex, IN UINT uiNameBufLen, OUT CHAR * pcVrfName);

/**************************************************************************
       Format: ULONG L3VPN_GetVrfIndexByIfIndex(IN IF_INDEX ifIndex,
                                             OUT VRF_INDEX *pusVrfIndex)
 Date Created: 2010-07-13
       Author: wenchenhua
        Input: ifIndex       接口索引
       Output: pusVrfIndex   VPN实例索引
       Return: ERROR_SUCCESS   ERROR_FAILED
      Caution: 无
Enter context: 无
 Exit context: 无
  Locking seq: 无
  Description: 通过接口索引获取其绑定的VPN实例索引
--------------------------------------------------------------------------
  Modification History
  DATE          NAME       DESCRIPTION
--------------------------------------------------------------------------
  2010-7-13    wenchenhua  Create
**************************************************************************/
FROZEN_IMPL ULONG L3VPN_GetVrfIndexByIfIndex(IN IF_INDEX ifIndex, OUT VRF_INDEX *pvrfIndex);

/**************************************************************************
       Format: L3VPN_GetVrfIndexBySnmpContext
 Date Created: 2013-08-20
       Author: chenzhe
        Input: CHAR *pcSnmpContext snmp 上下文
       Output: VRF_INDEX *pvrfIndex     VPN实例索引
       Return: ERROR_SUCCESS   ERROR_FAILED
      Caution: 无
Enter context: 无
 Exit context: 无
  Locking seq: 无
  Description: 通过snmp context 获取其配置的VPN实例索引, 若L3VPN忙，10s内未回复数据，
                   则断开链接，返回无效的VRFINDEX，防止协议进程阻塞在recv
--------------------------------------------------------------------------
  Modification History
  DATE          NAME       DESCRIPTION
--------------------------------------------------------------------------
  2013-8-20    chenzhe  Create
**************************************************************************/
ULONG L3VPN_GetVrfIndexBySnmpContext(IN const CHAR *pcSnmpContext, OUT VRF_INDEX *pvrfIndex);
ULONG L3VPN_ReloadNoResource(VOID);

ULONG L3VPN_GetNextVrfByName(INOUT CHAR * pcVrfName, OUT VRF_INDEX *pvrfIndex);

#ifdef  __cplusplus
}
#endif  /* end of __cplusplus */

#endif
