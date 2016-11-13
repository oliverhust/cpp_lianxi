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



/* LIPC֪���˿ں� */


#define EVT_INVALID_HANDLE  0  /* �¼�ע����Ч��� */

#define EVT_DATA_MAX_LEN  20UL  /* Я��˽�����ݳ��� */

#define EVT_DBG_TRACE       0x1UL
#define EVT_DBG_WARN        0x2UL
#define EVT_DBG_ERROR       0x4UL

typedef enum enEvt_Instance_Id
{
    EVT_IF_INSTANCE = 0,            /* �ӿڹ���ʵ�� */
    EVT_DEV_INSTANCE,               /* DEV�¼�ʵ�� */
    /* ����ʵ�����ڴ˰������ */
    EVT_VLAN_INSTANCE,          	/* VLANģ���¼�֪ͨʵ�� */
    EVT_ZONE_INSTANCE,
    EVT_MDC_INSTANCE,               /* MDC�¼�ʵ�� */
    EVT_MAX_INSTANCE = 5            /* �ڲ����Ʋ�����5��ʵ�������԰������� */
}EVT_INSTANCE_ID_E;

/* ����ֵΪת�����˽��ע������ */
typedef VOID (*EVT_CONVERTORDER_REGINFO_PF)(
    IN UINT uiEvent,      /* ע���¼����� */
    INOUT VOID *pRegInfo, /* ע���¼�Я����˽������ */
    OUT UINT *puiLen      /* ת����˽�����ݳ��� */
);

typedef ULONG (*EVT_NOTIFY_PF)(IN UINT uiEvent, IN VOID *pEvInfo, IN UINT uiNum, IN UINT uiSeqNo, IN CHAR *pcData);

#ifndef __KERNEL__

/* ��pEvInfoת����������,����ֵΪת����ĵ�ַ */
typedef VOID* (*EVT_COVERT_NOTIFY_FINAL_PF)(
    IN UINT uiEvent,        /* �¼����� */
    IN VOID *pEvInfo,       /* �����¼�ʱ������¼�������� */
    IN UINT uiNum,          /* �������� */
    OUT UINT *puiLen        /* ת����ĳ��� */
);

typedef ULONG (*EVT_COVERT_NOTIFY_DATA_PF)(
    IN UINT uiEvent,              /* �¼����� */
    IN VOID *pCur,                /* ����ָ��:˽��������ʼλ�� */
    IN UINT  uiNum,               /* �������� */
    OUT VOID **ppEvInfo,          /* ת�����¼���Ϣ�����׵�ַ */
    OUT CHAR **ppcData            /* ת����˽������ */
);

typedef VOID (*EVT_BIT_RESET_PF)(
    IN    UINT uiRetBit,                 /* ȥע���¼����� */
    INOUT UINT *puiEvent                 /* ԭ���¼� */
);

typedef struct tagEvt_Para
{
    EVT_BIT_RESET_PF pfBitReset;                      /* �����¼�ȥע�� */
    EVT_CONVERTORDER_REGINFO_PF pfConverRegInfo;      /* ��LIPCע���¼��е�˽������ת��Ϊ������ */
    EVT_COVERT_NOTIFY_DATA_PF pfConverNotify;         /* ���յ����¼�����ת��Ϊ������ */
    EVT_COVERT_NOTIFY_FINAL_PF pfConverNotifyFinal;   /* ���ظ�����ת��Ϊ������ */
    USHORT usPort;                                    /* �ں������˿ںţ����������ʹ�ö˿ں�ֵ��дΪLIPC_PORT_INVALID */
}EVT_PARA_S;

/* �û��߳��¼�ʵ����ʼ��, �������û�̬���ں˵�LIPC����. pstPara����NULL��ʾʹ��ȱʡ���ԺͶ˿�
     ��ʼ���ɹ��󷵻�socket��������򷵻�С�������
     ��1 socket���ϣ�����ͨ��errno��ȡ��ϸ������.���з�socket��׼������
       ERROR_ALREADY_EXIST ʵ���Ѿ���ʼ��
       ERROR_NO_ENOUGH_RESOURCE ��Դ���㣬�������벻���ڴ�
*/
extern INT EVT_InstanceInit(IN USHORT usInstanceID, IN EVT_PARA_S *pstPara);

/* �û��߳��¼�ʵ���ͷ� */
extern VOID EVT_InstanceRelease(IN USHORT usInstanceID);


/* EVT_RegEvent�ӿ�����ע���¼���
     ע��ɹ�����handle������ȥע��ʱ��Ҫ�����handle��
     ע��ʧ��ʱ����EVT_INVALID_HANDLE
*/
extern UINT EVT_RegEvent
(
    IN USHORT usInstanceID,             /* ��ģ�鶨���ע���¼����Ե�ID */
    IN UINT uiMID,                      /* ģ��ID������Ϊ������; */
    IN UINT uiEvent,                    /* �¼����� */
    IN UINT uiPri,                      /* �¼����ȼ� */
    IN EVT_NOTIFY_PF pfNotify,          /* ע�ᴦ���� */
    IN VOID *pRegCB                     /* �����¼����ʹ��벻ͬ�Ŀ��ƿ� */
);

/* ���ں�ȥע���¼� */
extern UINT EVT_UnRegEvent
(
    IN USHORT usInstanceID,             /* ��ģ�鶨���ע���¼����Ե�ID */
    IN UINT uiHandle,                   /* ע��ʱ���ص�handle */
    IN UINT uiEvent                     /* �¼����� */
);

UINT EVT_RegExtendEvent
(
    IN USHORT usInstanceID,             /* ��ģ�鶨���ע���¼����Ե�ID */
    IN UINT uiMID,                      /* ģ��ID */
    IN UINT uiPri,                      /* �¼����ȼ� */
    IN const UINT *puiExtEvtList,       /* ��չ�¼��б� */
    IN UINT uiExtEvtNum,                /* ��չ�¼����� */
    IN EVT_NOTIFY_PF pfNotify,          /* ע�ᴦ���� */
    IN VOID* pRegCB                     /* �����¼����ʹ��벻ͬ�Ŀ��ƿ� */
);

UINT EVT_UnRegExtendEvent
(
    IN USHORT usInstanceID,             /* ��ģ�鶨���ע���¼����Ե�ID */
    IN UINT uiHandle,                   /* ע��handle */
    IN const UINT *puiExtEvtList,       /* ��չ�¼��б� */
    IN UINT uiExtEvtNum                 /* ��չ�¼����� */
);

/* �û�̬֪ͨ�ں��¼������� */
extern ULONG EVT_NotifyFinalStatus(
    IN USHORT usInstanceID,             /* ��ģ�鶨���ע���¼����Ե�ID */
    IN UINT uiHandle,                   /* ע��ʱ���ص�handle */
    IN UINT uiEvent,                    /* �¼����� */
    IN VOID *pEvInfo,                   /* ��������:��ģ�����ж��� */
    IN UINT uiNum,                      /* �������ݸ��� */
    IN UINT uiSeqNo                     /* ���к� */
);

/* �������ĺ��� */
extern ULONG EVT_ProcessPack
(
    IN USHORT usInstanceID,             /* ��ģ�鶨���ע���¼����Ե�ID */
    IN CHAR *pcBuf,                     /* ����ͷ */
    IN ULONG ulBufLen                    /* ���ĳ��� */
);

/* �ú�������iFD��Ӧ���¼����ĵĽ��ռ����� */
extern ULONG EVT_ResponseEventPkt
(
    IN USHORT usInstanceID,
    IN INT iFD
);

extern ULONG EVT_SetInstanceEvent(USHORT usInstanceID, IN UINT uiEvent, IN ULONG ulFlag);

extern ULONG EVT_NotifyEvent
(
    IN USHORT usInstanceID, /* ��ģ�鶨���ע���¼����Ե� */
    IN UINT uiEvent,        /* �¼����� */
    IN ULONG ulTimer,       /* ��Ҫ�ȴ���ʱ�䣬�����߳�ȷ�� */
    IN UINT uiNum,          /* �������� */
    IN ULONG ulLen,          /* ֪ͨ���ݳ��� */
    IN const VOID *pEvInfo        /* ������Ϣ */
);

#else

#define EVT_FLAG_KER 0     /* �ں�̬ע�� */
#define EVT_FLAG_USR 1     /* �û�̬ע�� */

/* EVT_REG_INFO_S ��ע���¼�ʱЯ�������� */
typedef struct tagEVT_RegInfo
{
    VOID *pValue;    /* ע��ʱЯ����˽�����ݣ�����������֪ͨ��ģ�� */
    
    union
    {
        EVT_NOTIFY_PF pfCB;
        /* ��ע�������ӵ�IPC��ʶ. ע:��ΪEVT LIB���ݴ洢���ںˣ�����LIPCҪ��ֻ�ܴ�socket�ṹָ��*/
        struct socket *pstRegSocket;
    }unNotify;
    #define unpfCallBack unNotify.pfCB
    #define unpstRegSocket unNotify.pstRegSocket
    
    UINT uiMID;      /* ģ��ID */
    UINT uiPriority; /* �����IPC��ʽ����������ȼ�ֻ�Ǳ�������IPC��Ϣ��˳�򣬲��ܱ�֤�����¼���˳�� */
    UINT uiLen;      /* pValue�ĳ��� */
    UINT uiFlag;     /* �������ں�ע�ỹ���û�̬ע�� */
    UINT uiPid;      /* ���û�̬ע��ʱ��Ҫ */
}EVT_REG_INFO_S;

/* EVT LIB��������EVTʵ���Ĳ�������
   ��ʹ�ûص�֪ͨ��ʽ�������,��Ҫ�õ��ķ�������:
   1. ������֪ͨ��ģ��EVT_FILTER_PF. ���¼�֪ͨʱ������������Ҫ֪ͨ����ע��ģ��(����ӿ��¼���ֻ���ض��ӿ���������)��
      ��˾���Ҫ����ע���˽�����ݺ��¼�����ʱ�ĳ������й���
      �˺�������Ҫ������װ�¼�֪ͨʱЯ��������.
   ����������EVT_ENTITY���ṩ(�ں˲�)

   2.ʹ��IPC֪ͨ��ʽ������£���Ҫʹ�����ṩ�ķ�������
      1. ��ע��˽�����ݵ��ֽ���ת������(������������),���û��࣬���û�̬���������ṩ
      2. ��ע��˽�����ݵ��ֽ���ת������(�������������ת��)����EVT_ENTITY�࣬���ں�̬�ṩ

   3.���ݴ�����¼�����ֵת��Ϊ�ڲ��洢�������±귽��

   4.���ݷ���ֵ���´洢�ķ��ؽ��������
*/

/* ���˺���¼�������ݣ�֪ͨע��ģ��ʱʹ�� */
typedef VOID* (*EVT_FILTER_PF)(
    IN UINT uiEvent,
    IN VOID *pPrivateDataOnReg, /* ע���¼�ʱЯ����˽������ */
    IN VOID *pEvInfo,           /* �����¼�ʱ������¼�������� */
    IN UINT  uiNum,             /* ���֧������֪ͨ����ΪpEvInfo���ݸ��� */
    IN UINT  uiFlag,            /* �ص���ʽ����LIPC��ʽ. �����LIPC��ʽ��Ӧ��ģ����Ҫ�����Ƿ�����ֽ���ת�� */
    OUT UINT *puiNewLen,        /* ���˺��pNewEvInfo�����ܳ��� */
    OUT UINT *puiNewNum         /* ���˺��pNewEvInfo�����ܳ��� */
);

typedef ULONG (*EVT_CHG_INTER_PF)(
    IN UINT uiEvent      /* ���ݵ��¼����� */
);

typedef VOID (*EVT_GET_INFO_PF)(
    IN UINT uiEvent,          /* ���ݵ��¼����� */
    OUT ULONG *pulBegin,      /* �¼����������±꿪ʼֵ */
    OUT ULONG *pulEnd,        /* �¼����������±����ֵ */
    OUT ULONG *pulEvtSave     /* ���ݵ��¼������ڲ��洢ֵ */
);

typedef ULONG (*EVT_SET_RESULT_PF)(
    IN ULONG ulFlag,         /* �ں˻����û� */
    IN UINT uiEvent,         /* �¼����� */
    IN VOID *pPrivateDataOnReg, /* ע���¼�ʱЯ����˽������ */
    IN UINT uiResultNum,     /* �¼��ظ����ݸ��� */
    IN VOID *pResultData,    /* �¼��ظ����� */
    IN UINT uiNum,           /* �¼��洢���ݸ��� */
    INOUT VOID *pData        /* �¼��������� */
);

/* ���ں˲���� */
typedef struct tagEvt_KPara
{
    EVT_FILTER_PF  pfEvtFilter;                   /* ��װ�¼�֪ͨ���ĺ��� */
    EVT_CONVERTORDER_REGINFO_PF pfConvertRegInfo; /* ʹ���߽�LIPCע���¼��е�����˽������ת��Ϊ������ */
    EVT_CHG_INTER_PF pfChgInter;                  /* ʹ���߸����¼�����ת��Ϊ�����ڲ������±� */
    EVT_GET_INFO_PF pfGetEvtInfo;                 /* ʹ���߸����¼����ͻ�ȡ�ڲ�������ʼ�±�ʹ洢ֵ */
    EVT_SET_RESULT_PF pfSetResult;                /* �����¼��������õȴ��ڵ㷵�ؽ��,���ڷ���ֵ��Ŀ���ǹ̶��ģ�
                                                     ������Ҫʹ�����Լ�������Ҫ���з���ֵ�ĸ��� */
}EVT_KPARA_S;

/* ����EVTʵ��, ulMaxEvtNum�ǿ��ܵ��¼�����������usPort��ʵ�������Ķ˿ڣ����ΪΪ0����LIPC_PORT_INVALIDʱ������ʹ��LIPC_LOCAL_PORT_EVENT
   �˽ӿ���Ҫ��MDC�������е��ã�����Ҫ��EVT Entityһ�����(Ŀǰ���ں���)*/
extern ULONG EVT_CreateInstance(IN USHORT usInstanceID, IN ULONG ulMaxEvtNum, IN USHORT usPort);
/* ����EVTʵ��,�봴��ʵ����ͬ���˽ӿ���Ҫ��MDC�������е��ã�����Ҫ���ں��е��� */
extern VOID EVT_DestroyInstance(IN USHORT usInstanceID);

extern ULONG EVT_KSetPara(IN USHORT usInstanceID, IN EVT_KPARA_S *pstPara);

extern ULONG EVT_KSetFlag(IN USHORT usInstanceID, IN UINT uiEvent, IN ULONG ulFlag);
extern ULONG EVT_KSetExtendEventFlag(IN USHORT usInstanceID,
                                     IN const UINT *puiEvtList,
                                     IN UINT uiEvtNum,
                                     IN ULONG ulFlag);

/* EVT_RegNotify�ӿ������ں�ģ��ע���¼�
   uiEventֵΪ��Ҫע���¼���
   ע��ɹ�����handle����ע��ʱ��Ҫ�����handleֵ
   ע��ʧ�ܷ���EVT_INVALID_HANDLE
*/
extern UINT EVT_RegNotify
(
    IN USHORT usInstanceID,
    IN UINT uiEvent,
    IN EVT_REG_INFO_S* pstRegInfo
);
/* ulEventֵΪ��Ҫȥע���¼��������±� */
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

/* ֪ͨ����ģ���¼� */
extern ULONG EVT_Notify
(
    IN USHORT usInstanceID, /* ��ģ�鶨���ע���¼����Ե� */
    IN UINT uiEvent,        /* �¼����� */
    IN CHAR *pcData,        /* ����Я�������� */
    IN ULONG ulTimer,       /* ��Ҫ�ȴ���ʱ�䣬�����߳�ȷ�� */
    IN UINT uiNum,          /* �������� */
    IN VOID *pEvInfo,       /* ������Ϣ */
    INOUT VOID *pRet        /* ��¼����ֵ */
);

/* ���ݻظ����¼���������Ϣ���к������� */
extern ULONG EVT_DealOver
(
    IN USHORT usInstanceID, /* ��ģ�鶨���ע���¼����Ե�ID */
    IN UINT uiHandle,       /* ע��handle */
    IN UINT uiEvent,        /* �¼����� */
    IN VOID *pEvInfo,       /* ������Ϣ */
    IN UINT uiNum,          /* �������� */
    IN UINT uiSeqNo,         /* ���к� */
    IN ULONG ulFlag         /* ��ʾ���ں˴�����������û�̬������� */
);

/* ����event���Կ��� */
extern VOID EVT_SetDbgSwitch(IN ULONG ulOption, IN BOOL_T bIsUndo);

/* ��ȡevent���Կ������� */
extern ULONG EVT_GetDbgSwitch(VOID);

/* �ں˷����û�̬���¼�����������Ϣ */
typedef struct tagEvt_DataMsg
{
    VOID *pEvInfo;
    UINT uiNum;
    UINT uiEvInfoLen;     /* pEvInfo�ĳ��� */
    CHAR  *pcData;        /* ����Я�������� */
}EVT_DATA_MSG_S;

extern ULONG EVT_SendMsgToUsr(
    IN USHORT usInstanceID,
    IN UINT uiEvent,
    IN UINT uiHandle,
    IN UINT  uiSeqNo,
    IN EVT_DATA_MSG_S *pstEv
);

/* ulEventֵΪ��Ҫ���ҵ�ע���¼��������±� */
extern BOOL_T EVT_KIsRegNotify(IN USHORT usInstanceID, IN ULONG ulEvent);
extern ULONG EVT_ProcEventNotify(IN UINT uiCmd,
                                 IN UINT uiInBufSize,
                                 IN const VOID *pInBuf,
                                 IN UINT uiOutBufSize,
                                 OUT VOID *pOutBuf,
                                 OUT UINT *puiBytesRet);
#endif

/* �¼����ñ�־λ */
#define EVT_FLAG_REPLY           0x01UL   /* �¼���Ҫ�ȴ��ظ� */
#define EVT_FLAG_REGNOTIFY       0x02UL   /* �¼���Ҫ��ע��ʱ֪ͨ */
#define EVT_FLAG_KER_REPLY       0x04UL   /* �ں��¼�Ҳ��Ҫ�ȴ���Ŀǰ�ݲ�֧�� */

/* ʹ��LIPCͨѶ�ļ�����Ϣ���� */
typedef enum enEvt_IPC_Msg{
    EVT_IPC_MSG_REG_REQT = 1,  /* ע�ᱨ�� */
    EVT_IPC_MSG_REG_RESP,      /* ��ע�ᱨ�� */
    EVT_IPC_MSG_NOTIFY,        /* �¼�֪ͨ���� */
    EVT_IPC_MSG_STATUS,        /* �ظ����� */
    EVT_IPC_MSG_REG_CHK,       /* ע��ɹ���� */
    EVT_IPC_MSG_UNREG_CHK,     /* ��ע���� */
    EVT_IPC_MSG_SET_EVENT,     /* ����ʵ��event���� */

    EVT_IPC_MSG_MAX
}EVT_IPC_MSG_E;

/* LIPC��Ϣ��ʽ */
/**********************�¼�����˵��*****************************************

1.�¼���������ͷ��ʽ:
-----------------------------------------------------------------------
|TYPE(UINT)|TOTALLEN(USHORT)|INSTANCEID(USHORT)|HANDLE(UINT)|EVENT(UINT)|
-----------------------------------------------------------------------
��13���ֽ�

2.�¼�ע�ᱨ�ĸ�ʽ:
-----------------------------------------------------------------------
|TYPE(UINT)|TOTALLEN(USHORT)|INSTANCEID(USHORT)|MID(UINT)|EVENT(UINT)|
-----------------------------------------------------------------------
|PRI(UINT)|PRIVATELEN(UINT)|PRIVATE DATA|
-----------------------------------------
PRIVATE DATAΪ�����¼��Լ������˽�����ݣ������ɸ����¼�ʵ��ע��ص��������,
���PRIVATELEN�ֶ�Ϊ0��ʾ������PRIVATE DATA���ݡ�

3.�¼�ȥע�ᱨ�ĸ�ʽ:
-----------------------------------------------------------------------
|TYPE(UINT)|TOTALLEN(USHORT)|INSTANCEID(USHORT)|HANDLE(UINT)|EVENT(UINT)|
-----------------------------------------------------------------------
ȥע�ᱨ�ĸ�ʽ�빫������ͷ��ʽ��ͬ��������������Ϣ

3.�¼�֪ͨ���ĸ�ʽ:
-----------------------------------------------------------------------
|TYPE(UINT)|TOTALLEN(USHORT)|INSTANCEID(USHORT)|HANDLE(UINT)|EVENT(UINT)|
-----------------------------------------------------------------------
|SEQ(UINT)|LEN(UINT)|DATALEN(UCHAR)|DATA|EVDATA|
------------------------------------------------
LENΪ���������ܳ��ȣ�DATALENΪ�������ݳ��ȣ����Ϊ0ʱ��󲻴���DATA�ֶΣ�
EVDATAΪ�¼��������ݣ���Ҫ�����¼�ʵ��ע��ص�������ɽ�����

4.�¼��ظ����ĸ�ʽ:
-----------------------------------------------------------------------
|TYPE(UINT)|TOTALLEN(USHORT)|INSTANCEID(USHORT)|HANDLE(UINT)|EVENT(UINT)|
-----------------------------------------------------------------------
|SEQ(UINT)|LEN(UINT)|NUM(UINT)|EVDATA|
------------------------------------------------
LENΪ���������ܳ��ȣ�NUMΪ���ݸ�����EVDATAΪ�¼��������ݣ���Ҫ�����¼�ʵ
��ע��ص�������ɽ�����

**********************�¼�����˵��*************************************/

typedef struct tagEvt_Base_Msg
{
    UINT   uiType;        /* �������� */
    UINT   uiTotalLen;    /* �����ܳ��� */
    UINT   uiHandle;      /* ģ����Ϣ�������ע�ᱨ��ʱΪģ��ID */
    UINT   uiEvent;       /* �¼����� */
    USHORT usInstance;    /* ʵ�� */
    USHORT usEnd;         /* �����Ƿ������1��ʾ���� */
}EVT_BASE_MSG_S;

typedef struct tagEvt_Reg_Msg
{
    UINT uiType;        /* �������� */
    UINT uiTotalLen;    /* �����ܳ��� */
    UINT uiHandle;      /* ע��Handle */
    UINT uiEvent;       /* �¼����� */
    UINT uiPri;         /* ע�����ȼ� */
    UINT uiLen;         /* �ṹ������������ݳ��� */
    UINT uiPid;
    UINT uiMod;
    USHORT usInstance;  /* ʵ�� */
    USHORT usExtEvt;    /* �Ƿ���չ�¼���1��ʾ��չ�¼� */
    USHORT usEvtNum;    /* �¼���������չ�¼�ʹ�� */
}EVT_REG_MSG_S;

typedef struct tagEvt_Msg
{
    EVT_BASE_MSG_S stBaseMsg;         /* ��Ϣ�������� */
    UINT uiSeq;                       /* ���к� */
    UINT uiLen;                       /* �ṹ������������ݳ��� */
    UINT uiNum;                       /* �ظ����ݸ��� */
}EVT_MSG_S;

/* ��չ�¼���ͨ��UINT��0~15bit�е��������������ϵ�λ��ʾһ���¼� */
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

