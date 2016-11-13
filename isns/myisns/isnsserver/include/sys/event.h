/*****************************************************************************
 Copyright (c) 2009, Hangzhou H3C Technologies Co., Ltd. All rights reserved.
------------------------------------------------------------------------------
                            event.h
  Project Code: Comware Leopard
   Module Name:
  Date Created: 2009-6-15
        Author: Dairuiqiang
   Description: Header file for Event module

------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
#ifndef _SYS_EVENT_H_
#define _SYS_EVENT_H_

#ifdef  __cplusplus
extern "C"{
#endif



/* LIPC知名端口号 */


#define EVT_INVALID_HANDLE  0  /* 事件注册无效句柄 */

#define EVT_DATA_MAX_LEN  20UL  /* 携带私有数据长度 */

#define EVT_DBG_TRACE       0x1UL
#define EVT_DBG_WARN        0x2UL
#define EVT_DBG_ERROR       0x4UL

typedef enum enEvt_Instance_Id
{
    EVT_IF_INSTANCE = 0,            /* 接口管理实例 */
    EVT_DEV_INSTANCE,               /* DEV事件实例 */
    /* 其它实例请在此按序添加 */
    EVT_VLAN_INSTANCE,          	/* VLAN模块事件通知实例 */
    EVT_ZONE_INSTANCE,
    EVT_MDC_INSTANCE,               /* MDC事件实例 */
    EVT_MAX_INSTANCE = 5            /* 内部限制不超过5个实例，可以按需扩大 */
}EVT_INSTANCE_ID_E;

/* 返回值为转换后的私有注册数据 */
typedef VOID (*EVT_CONVERTORDER_REGINFO_PF)(
    IN UINT uiEvent,      /* 注册事件类型 */
    INOUT VOID *pRegInfo, /* 注册事件携带的私有数据 */
    OUT UINT *puiLen      /* 转换后私有数据长度 */
);

typedef ULONG (*EVT_NOTIFY_PF)(IN UINT uiEvent, IN VOID *pEvInfo, IN UINT uiNum, IN UINT uiSeqNo, IN CHAR *pcData);

#ifndef __KERNEL__

/* 将pEvInfo转换成网络序,返回值为转换后的地址 */
typedef VOID* (*EVT_COVERT_NOTIFY_FINAL_PF)(
    IN UINT uiEvent,        /* 事件类型 */
    IN VOID *pEvInfo,       /* 触发事件时传入的事件相关数据 */
    IN UINT uiNum,          /* 批量个数 */
    OUT UINT *puiLen        /* 转换后的长度 */
);

typedef ULONG (*EVT_COVERT_NOTIFY_DATA_PF)(
    IN UINT uiEvent,              /* 事件类型 */
    IN VOID *pCur,                /* 报文指针:私有数据起始位置 */
    IN UINT  uiNum,               /* 批量个数 */
    OUT VOID **ppEvInfo,          /* 转换后事件信息数据首地址 */
    OUT CHAR **ppcData            /* 转换后私有数据 */
);

typedef VOID (*EVT_BIT_RESET_PF)(
    IN    UINT uiRetBit,                 /* 去注册事件类型 */
    INOUT UINT *puiEvent                 /* 原有事件 */
);

typedef struct tagEvt_Para
{
    EVT_BIT_RESET_PF pfBitReset;                      /* 部分事件去注册 */
    EVT_CONVERTORDER_REGINFO_PF pfConverRegInfo;      /* 将LIPC注册事件中的私有数据转换为网络序 */
    EVT_COVERT_NOTIFY_DATA_PF pfConverNotify;         /* 将收到的事件报文转换为主机序 */
    EVT_COVERT_NOTIFY_FINAL_PF pfConverNotifyFinal;   /* 将回复报文转换为网络序 */
    USHORT usPort;                                    /* 内核侦听端口号，如果不单独使用端口号值填写为LIPC_PORT_INVALID */
}EVT_PARA_S;

/* 用户线程事件实例初始化, 并建立用户态与内核的LIPC连接. pstPara传入NULL表示使用缺省属性和端口
     初始化成功后返回socket句柄，否则返回小于零的数
     －1 socket故障，可以通过errno获取详细错误码.其中非socket标准错误码
       ERROR_ALREADY_EXIST 实例已经初始化
       ERROR_NO_ENOUGH_RESOURCE 资源不足，例如申请不到内存
*/
extern INT EVT_InstanceInit(IN USHORT usInstanceID, IN EVT_PARA_S *pstPara);

/* 用户线程事件实例释放 */
extern VOID EVT_InstanceRelease(IN USHORT usInstanceID);


/* EVT_RegEvent接口用于注册事件。
     注册成功返回handle，后续去注册时需要传入此handle。
     注册失败时返回EVT_INVALID_HANDLE
*/
extern UINT EVT_RegEvent
(
    IN USHORT usInstanceID,             /* 各模块定义的注册事件特性的ID */
    IN UINT uiMID,                      /* 模块ID，仅做为调试用途 */
    IN UINT uiEvent,                    /* 事件类型 */
    IN UINT uiPri,                      /* 事件优先级 */
    IN EVT_NOTIFY_PF pfNotify,          /* 注册处理函数 */
    IN VOID *pRegCB                     /* 根据事件类型传入不同的控制块 */
);

/* 向内核去注册事件 */
extern UINT EVT_UnRegEvent
(
    IN USHORT usInstanceID,             /* 各模块定义的注册事件特性的ID */
    IN UINT uiHandle,                   /* 注册时返回的handle */
    IN UINT uiEvent                     /* 事件类型 */
);

UINT EVT_RegExtendEvent
(
    IN USHORT usInstanceID,             /* 各模块定义的注册事件特性的ID */
    IN UINT uiMID,                      /* 模块ID */
    IN UINT uiPri,                      /* 事件优先级 */
    IN const UINT *puiExtEvtList,       /* 扩展事件列表 */
    IN UINT uiExtEvtNum,                /* 扩展事件数量 */
    IN EVT_NOTIFY_PF pfNotify,          /* 注册处理函数 */
    IN VOID* pRegCB                     /* 根据事件类型传入不同的控制块 */
);

UINT EVT_UnRegExtendEvent
(
    IN USHORT usInstanceID,             /* 各模块定义的注册事件特性的ID */
    IN UINT uiHandle,                   /* 注册handle */
    IN const UINT *puiExtEvtList,       /* 扩展事件列表 */
    IN UINT uiExtEvtNum                 /* 扩展事件数量 */
);

/* 用户态通知内核事件处理结果 */
extern ULONG EVT_NotifyFinalStatus(
    IN USHORT usInstanceID,             /* 各模块定义的注册事件特性的ID */
    IN UINT uiHandle,                   /* 注册时返回的handle */
    IN UINT uiEvent,                    /* 事件类型 */
    IN VOID *pEvInfo,                   /* 批量数据:各模块自行定义 */
    IN UINT uiNum,                      /* 批量数据个数 */
    IN UINT uiSeqNo                     /* 序列号 */
);

/* 解析报文函数 */
extern ULONG EVT_ProcessPack
(
    IN USHORT usInstanceID,             /* 各模块定义的注册事件特性的ID */
    IN CHAR *pcBuf,                     /* 报文头 */
    IN ULONG ulBufLen                    /* 报文长度 */
);

/* 该函数负责iFD对应的事件报文的接收及处理 */
extern ULONG EVT_ResponseEventPkt
(
    IN USHORT usInstanceID,
    IN INT iFD
);

extern ULONG EVT_SetInstanceEvent(USHORT usInstanceID, IN UINT uiEvent, IN ULONG ulFlag);

extern ULONG EVT_NotifyEvent
(
    IN USHORT usInstanceID, /* 各模块定义的注册事件特性的 */
    IN UINT uiEvent,        /* 事件类型 */
    IN ULONG ulTimer,       /* 需要等待的时间，调用线程确定 */
    IN UINT uiNum,          /* 批量个数 */
    IN ULONG ulLen,          /* 通知数据长度 */
    IN const VOID *pEvInfo        /* 具体信息 */
);

#else

#define EVT_FLAG_KER 0     /* 内核态注册 */
#define EVT_FLAG_USR 1     /* 用户态注册 */

/* EVT_REG_INFO_S 是注册事件时携带的数据 */
typedef struct tagEVT_RegInfo
{
    VOID *pValue;    /* 注册时携带的私有数据，用来过滤需通知的模块 */
    
    union
    {
        EVT_NOTIFY_PF pfCB;
        /* 与注册者连接的IPC标识. 注:因为EVT LIB数据存储在内核，根据LIPC要求只能存socket结构指针*/
        struct socket *pstRegSocket;
    }unNotify;
    #define unpfCallBack unNotify.pfCB
    #define unpstRegSocket unNotify.pstRegSocket
    
    UINT uiMID;      /* 模块ID */
    UINT uiPriority; /* 如果是IPC方式，这里的优先级只是表明发送IPC消息的顺序，不能保证处理事件的顺序 */
    UINT uiLen;      /* pValue的长度 */
    UINT uiFlag;     /* 区分是内核注册还是用户态注册 */
    UINT uiPid;      /* 仅用户态注册时需要 */
}EVT_REG_INFO_S;

/* EVT LIB允许设置EVT实例的操作方法
   在使用回调通知方式的情况下,需要用到的方法包括:
   1. 过滤需通知的模块EVT_FILTER_PF. 在事件通知时，往往并不需要通知所有注册模块(例如接口事件，只对特定接口类型敏感)，
      因此就需要根据注册的私有数据和事件发生时的场景进行过滤
      此函数还需要考虑组装事件通知时携带的数据.
   上述函数在EVT_ENTITY侧提供(内核侧)

   2.使用IPC通知方式的情况下，需要使用者提供的方法包括
      1. 对注册私有数据的字节序转换方法(主机序到网络序),在用户侧，即用户态各进程中提供
      2. 对注册私有数据的字节序转换方法(网络序到主机序的转换)，在EVT_ENTITY侧，即内核态提供

   3.根据传入的事件类型值转换为内部存储的数组下标方法

   4.根据返回值更新存储的返回结果函数。
*/

/* 过滤后的事件相关数据，通知注册模块时使用 */
typedef VOID* (*EVT_FILTER_PF)(
    IN UINT uiEvent,
    IN VOID *pPrivateDataOnReg, /* 注册事件时携带的私有数据 */
    IN VOID *pEvInfo,           /* 触发事件时传入的事件相关数据 */
    IN UINT  uiNum,             /* 如果支持批量通知，则为pEvInfo数据个数 */
    IN UINT  uiFlag,            /* 回调方式还是LIPC方式. 如果是LIPC方式，应用模块需要考虑是否进行字节序转换 */
    OUT UINT *puiNewLen,        /* 过滤后的pNewEvInfo数据总长度 */
    OUT UINT *puiNewNum         /* 过滤后的pNewEvInfo数据总长度 */
);

typedef ULONG (*EVT_CHG_INTER_PF)(
    IN UINT uiEvent      /* 数据的事件类型 */
);

typedef VOID (*EVT_GET_INFO_PF)(
    IN UINT uiEvent,          /* 数据的事件类型 */
    OUT ULONG *pulBegin,      /* 事件类型数组下标开始值 */
    OUT ULONG *pulEnd,        /* 事件类型数组下标结束值 */
    OUT ULONG *pulEvtSave     /* 数据的事件类型内部存储值 */
);

typedef ULONG (*EVT_SET_RESULT_PF)(
    IN ULONG ulFlag,         /* 内核还是用户 */
    IN UINT uiEvent,         /* 事件类型 */
    IN VOID *pPrivateDataOnReg, /* 注册事件时携带的私有数据 */
    IN UINT uiResultNum,     /* 事件回复数据个数 */
    IN VOID *pResultData,    /* 事件回复数据 */
    IN UINT uiNum,           /* 事件存储数据个数 */
    INOUT VOID *pData        /* 事件返回数据 */
);

/* 在内核侧调用 */
typedef struct tagEvt_KPara
{
    EVT_FILTER_PF  pfEvtFilter;                   /* 组装事件通知报文函数 */
    EVT_CONVERTORDER_REGINFO_PF pfConvertRegInfo; /* 使用者将LIPC注册事件中的自身私有数据转换为主机序 */
    EVT_CHG_INTER_PF pfChgInter;                  /* 使用者根据事件类型转换为自身内部数组下标 */
    EVT_GET_INFO_PF pfGetEvtInfo;                 /* 使用者根据事件类型获取内部数组起始下标和存储值 */
    EVT_SET_RESULT_PF pfSetResult;                /* 根据事件类型设置等待节点返回结果,由于返回值数目不是固定的，
                                                     所以需要使用者自己根据需要进行返回值的更新 */
}EVT_KPARA_S;

/* 创建EVT实例, ulMaxEvtNum是可能的事件的最大个数；usPort是实例侦听的端口，如果为为0或者LIPC_PORT_INVALID时，共享使用LIPC_LOCAL_PORT_EVENT
   此接口需要在MDC上下文中调用，且需要在EVT Entity一侧调用(目前在内核中)*/
extern ULONG EVT_CreateInstance(IN USHORT usInstanceID, IN ULONG ulMaxEvtNum, IN USHORT usPort);
/* 销毁EVT实例,与创建实例相同，此接口需要在MDC上下文中调用，且需要在内核中调用 */
extern VOID EVT_DestroyInstance(IN USHORT usInstanceID);

extern ULONG EVT_KSetPara(IN USHORT usInstanceID, IN EVT_KPARA_S *pstPara);

extern ULONG EVT_KSetFlag(IN USHORT usInstanceID, IN UINT uiEvent, IN ULONG ulFlag);
extern ULONG EVT_KSetExtendEventFlag(IN USHORT usInstanceID,
                                     IN const UINT *puiEvtList,
                                     IN UINT uiEvtNum,
                                     IN ULONG ulFlag);

/* EVT_RegNotify接口用于内核模块注册事件
   uiEvent值为所要注册事件块
   注册成功返回handle，解注册时需要传入此handle值
   注册失败返回EVT_INVALID_HANDLE
*/
extern UINT EVT_RegNotify
(
    IN USHORT usInstanceID,
    IN UINT uiEvent,
    IN EVT_REG_INFO_S* pstRegInfo
);
/* ulEvent值为所要去注册事件块数组下标 */
extern UINT EVT_UnRegNotify(IN USHORT usInstanceID, IN UINT uiHandle, IN UINT uiEvent, IN UINT uiFlag);

extern UINT EVT_RegNotifyExtendEvent
(
    IN USHORT usInstanceID, 
    IN const UINT *puiEvtList, 
    IN UINT uiEvtNum,
    IN EVT_REG_INFO_S* pstRegInfo
);

extern UINT EVT_UnRegNotifyExtendEvent
(
    IN USHORT usInstanceID,
    IN const UINT *puiEvtList, 
    IN UINT uiEvtNum,
    IN UINT uiHandle,
    IN UINT uiFlag
);

/* 通知其他模块事件 */
extern ULONG EVT_Notify
(
    IN USHORT usInstanceID, /* 各模块定义的注册事件特性的 */
    IN UINT uiEvent,        /* 事件类型 */
    IN CHAR *pcData,        /* 额外携带的数据 */
    IN ULONG ulTimer,       /* 需要等待的时间，调用线程确定 */
    IN UINT uiNum,          /* 批量个数 */
    IN VOID *pEvInfo,       /* 具体信息 */
    INOUT VOID *pRet        /* 记录返回值 */
);

/* 根据回复的事件处理结果信息进行后续处理 */
extern ULONG EVT_DealOver
(
    IN USHORT usInstanceID, /* 各模块定义的注册事件特性的ID */
    IN UINT uiHandle,       /* 注册handle */
    IN UINT uiEvent,        /* 事件类型 */
    IN VOID *pEvInfo,       /* 具体信息 */
    IN UINT uiNum,          /* 批量个数 */
    IN UINT uiSeqNo,         /* 序列号 */
    IN ULONG ulFlag         /* 表示是内核处理结束还是用户态处理结束 */
);

/* 设置event调试开关 */
extern VOID EVT_SetDbgSwitch(IN ULONG ulOption, IN BOOL_T bIsUndo);

/* 获取event调试开关设置 */
extern ULONG EVT_GetDbgSwitch(VOID);

/* 内核发往用户态的事件报文数据信息 */
typedef struct tagEvt_DataMsg
{
    VOID *pEvInfo;
    UINT uiNum;
    UINT uiEvInfoLen;     /* pEvInfo的长度 */
    CHAR  *pcData;        /* 额外携带的数据 */
}EVT_DATA_MSG_S;

extern ULONG EVT_SendMsgToUsr(
    IN USHORT usInstanceID,
    IN UINT uiEvent,
    IN UINT uiHandle,
    IN UINT  uiSeqNo,
    IN EVT_DATA_MSG_S *pstEv
);

/* ulEvent值为所要查找的注册事件块数组下标 */
extern BOOL_T EVT_KIsRegNotify(IN USHORT usInstanceID, IN ULONG ulEvent);
extern ULONG EVT_ProcEventNotify(IN UINT uiCmd,
                                 IN UINT uiInBufSize,
                                 IN const VOID *pInBuf,
                                 IN UINT uiOutBufSize,
                                 OUT VOID *pOutBuf,
                                 OUT UINT *puiBytesRet);
#endif

/* 事件设置标志位 */
#define EVT_FLAG_REPLY           0x01UL   /* 事件需要等待回复 */
#define EVT_FLAG_REGNOTIFY       0x02UL   /* 事件需要在注册时通知 */
#define EVT_FLAG_KER_REPLY       0x04UL   /* 内核事件也需要等待，目前暂不支持 */

/* 使用LIPC通讯的几种消息类型 */
typedef enum enEvt_IPC_Msg{
    EVT_IPC_MSG_REG_REQT = 1,  /* 注册报文 */
    EVT_IPC_MSG_REG_RESP,      /* 解注册报文 */
    EVT_IPC_MSG_NOTIFY,        /* 事件通知报文 */
    EVT_IPC_MSG_STATUS,        /* 回复报文 */
    EVT_IPC_MSG_REG_CHK,       /* 注册成功检查 */
    EVT_IPC_MSG_UNREG_CHK,     /* 解注册检查 */
    EVT_IPC_MSG_SET_EVENT,     /* 设置实例event参数 */

    EVT_IPC_MSG_MAX
}EVT_IPC_MSG_E;

/* LIPC消息格式 */
/**********************事件报文说明*****************************************

1.事件公共报文头格式:
-----------------------------------------------------------------------
|TYPE(UINT)|TOTALLEN(USHORT)|INSTANCEID(USHORT)|HANDLE(UINT)|EVENT(UINT)|
-----------------------------------------------------------------------
共13个字节

2.事件注册报文格式:
-----------------------------------------------------------------------
|TYPE(UINT)|TOTALLEN(USHORT)|INSTANCEID(USHORT)|MID(UINT)|EVENT(UINT)|
-----------------------------------------------------------------------
|PRI(UINT)|PRIVATELEN(UINT)|PRIVATE DATA|
-----------------------------------------
PRIVATE DATA为各个事件自己定义的私有数据，解析由各个事件实例注册回调函数完成,
如果PRIVATELEN字段为0表示不存在PRIVATE DATA数据。

3.事件去注册报文格式:
-----------------------------------------------------------------------
|TYPE(UINT)|TOTALLEN(USHORT)|INSTANCEID(USHORT)|HANDLE(UINT)|EVENT(UINT)|
-----------------------------------------------------------------------
去注册报文格式与公共报文头格式相同，不含有其他信息

3.事件通知报文格式:
-----------------------------------------------------------------------
|TYPE(UINT)|TOTALLEN(USHORT)|INSTANCEID(USHORT)|HANDLE(UINT)|EVENT(UINT)|
-----------------------------------------------------------------------
|SEQ(UINT)|LEN(UINT)|DATALEN(UCHAR)|DATA|EVDATA|
------------------------------------------------
LEN为后续报文总长度，DATALEN为公共数据长度，如果为0时其后不存在DATA字段，
EVDATA为事件具体数据，需要各个事件实例注册回调函数完成解析。

4.事件回复报文格式:
-----------------------------------------------------------------------
|TYPE(UINT)|TOTALLEN(USHORT)|INSTANCEID(USHORT)|HANDLE(UINT)|EVENT(UINT)|
-----------------------------------------------------------------------
|SEQ(UINT)|LEN(UINT)|NUM(UINT)|EVDATA|
------------------------------------------------
LEN为后续报文总长度，NUM为数据个数，EVDATA为事件具体数据，需要各个事件实
例注册回调函数完成解析。

**********************事件报文说明*************************************/

typedef struct tagEvt_Base_Msg
{
    UINT   uiType;        /* 报文类型 */
    UINT   uiTotalLen;    /* 报文总长度 */
    UINT   uiHandle;      /* 模块信息，如果是注册报文时为模块ID */
    UINT   uiEvent;       /* 事件类型 */
    USHORT usInstance;    /* 实例 */
    USHORT usEnd;         /* 报文是否结束，1表示结束 */
}EVT_BASE_MSG_S;

typedef struct tagEvt_Reg_Msg
{
    UINT uiType;        /* 报文类型 */
    UINT uiTotalLen;    /* 报文总长度 */
    UINT uiHandle;      /* 注册Handle */
    UINT uiEvent;       /* 事件类型 */
    UINT uiPri;         /* 注册优先级 */
    UINT uiLen;         /* 结构体后续特殊数据长度 */
    UINT uiPid;
    UINT uiMod;
    USHORT usInstance;  /* 实例 */
    USHORT usExtEvt;    /* 是否扩展事件，1表示扩展事件 */
    USHORT usEvtNum;    /* 事件数量，扩展事件使用 */
}EVT_REG_MSG_S;

typedef struct tagEvt_Msg
{
    EVT_BASE_MSG_S stBaseMsg;         /* 消息基本类型 */
    UINT uiSeq;                       /* 序列号 */
    UINT uiLen;                       /* 结构体后续特殊数据长度 */
    UINT uiNum;                       /* 回复数据个数 */
}EVT_MSG_S;

/* 扩展事件：通过UINT的0~15bit中的两个或两个以上的位表示一个事件 */
#define EVT_EXTEND_MASK     0xFFFF

static inline BOOL_T EVT_IsExtendEvent(IN UINT uiEvent)
{
    UINT uiEventBits = uiEvent & EVT_EXTEND_MASK;
    BOOL_T bIsExtEvt = BOOL_FALSE;

    if(0 != uiEventBits)
    {
        if(0 != (uiEventBits & (uiEventBits - 1)))
        {
            bIsExtEvt = BOOL_TRUE;
        }
    }

    return bIsExtEvt;
}

#ifdef  __cplusplus
}
#endif  /* end of __cplusplus */

#endif  /* end of _SYS_EVENT_H_ */

