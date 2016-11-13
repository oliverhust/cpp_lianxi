/*****************************************************************************
 Copyright (c) 2010, Hangzhou H3C Technologies Co., Ltd. All rights reserved.
------------------------------------------------------------------------------
                               vpn.h

  Project Code: Comware Leopard
   Module Name: L3VPN
  Date Created: 2010-07-13
        Author: wangjiangbo
   Description: L3VPN�ⲿ����ͷ�ļ�

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

/* L3VPN�����˿� */


#define L3VPN_VRF_CREATE      0x00000001    /* VRF�����¼� */
#define L3VPN_VRF_DELETE      0x00000002    /* VRFɾ���¼� */
#define L3VPN_INTF_BIND       0x00000004    /* �ӿڰ��¼� */
#define L3VPN_INTF_UNBIND     0x00000008    /* �ӿ�ȥ���¼� */

#define L3VPN_RD_ASSIGN       0x00000010    /* RD�����¼� */
#define L3VPN_RD_DELETE       0x00000020    /* RDɾ���¼� */

#define L3VPN_IRT_CHANGE         0x00000040    /* IPv4 RT����Ա仯�¼� */
#define L3VPN_ERT_CHANGE         0x00000080    /* IPv4 RT�����Ա仯�¼� */

#define L3VPN_IRP_CHANGE      0x00000400    /* IPv4 ��·�ɲ��Ա仯�¼� */
#define L3VPN_ERP_CHANGE      0x00000800    /* IPv4 ��·�ɲ��Ա仯�¼� */
#define L3VPN_TP_CHANGE       0x00001000    /* IPv4 ������Ա仯�¼� */
#define L3VPN_MAX_LIMIT       0x00002000    /* IPv4 VPN�����·�����Ʊ仯�¼� */

#define L3VPN_IRT6_CHANGE        0x00004000    /* IPv6 RT����Ա仯�¼� */
#define L3VPN_ERT6_CHANGE        0x00008000    /* IPv6 RT�����Ա仯�¼� */


#define L3VPN_IRP6_CHANGE     0x00040000    /* IPv6 ��·�ɲ��Ա仯�¼� */
#define L3VPN_ERP6_CHANGE     0x00080000    /* IPv6 ��·�ɲ��Ա仯�¼� */
#define L3VPN_TP6_CHANGE      0x00100000    /* IPv6 ������Ա仯�¼� */
#define L3VPN_MAX6_LIMIT      0x00200000    /* IPv6 VPN�����·�����Ʊ仯�¼� */


#define L3VPN_BATCH_BEGIN     0x00400000    /* ����֪ͨ��ʼ������ע�� */
#define L3VPN_BATCH_END       0x00800000    /* ����֪ͨ����������ע�� */
#define L3VPN_EVENT           0x00FFFFFF

#define MT_EVENT_FLAG         0x01000000    /* �������¼� */
#define L3VPN_RT_ADD       0x01          /* RT��ӱ�־ */
#define L3VPN_RT_DEL       0x02          /* RTɾ����־ */

#define L3VPN_MAX_RT_NTFY_NUM          8     /* VRFʵ�����ο����õ����RT���� */

#define L3VPN_MAX_RDRT_LEN             8UL    /* RD/RT��ֵ�洢���ȣ�ע���Ϊ��ֵ�����ַ��� */

#define L3VPN_MAX_VRFNAME_LEN          31UL   /* VRF������󳤶� */
#define L3VPN_MAX_ROUTEPOLICYNAME_LEN  63UL   /* ���뷽���������󳤶�*/
#define L3VPN_MAX_TNLPOLICYNAME_LEN    19UL   /* �����������󳤶� */
#define L3VPN_RTRD_NAME_MAX_LEN        21     /* RD/RT�ַ�����󳤶� */
#define L3VPN_RTRD_NAME_MIN_LEN        3      /* ��ЧRD/RT�ַ�������С���� */
#define L3VPN_MAX_SNMP_CONTEXT_LEN     32UL   /* snmp context�ַ�����󳤶� */

#define L3VPN_MAX_FEATURE_NAME  15U


#define L3VPN_INVALID_VRF_INDEX     ((VRF_INDEX)(~0))    /* ��ЧVRF����ֵ */
#define L3VPN_INVALID_ROUTE_LIMIT   ((UINT)(~0))  /* ��Ч��ÿVPN���·���� */
#define L3VPN_INVALID_THRESHOLD     ((USHORT)(~0))  /* ��Ч�����·������ֵ */
#define L3VPN_INVALID_RTRD_VALUE    (0xFF)
#define L3VPN_INVALID_ORIGID        ((UINT)(~0))  /* ��Ч��origin id */
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
    USHORT      usReserveVlan;  /* ����vlan id��VLAN_INVALID_VALUE��ʾ��Чֵ */
    UINT64      auiDrvContext[L3VPN_DRV_CONTEXT_LEN];
}L3VPN_RESVVLANMSG_S;

ISSU typedef struct tagL3VPN_InstanceShow
{
    VRF_INDEX   vrfIndex;                            /* VPN ID */
    USHORT      usEpoch;                             /* ����ʱ�� */
    CHAR        szVPNName[L3VPN_MAX_VRFNAME_LEN+1];    /* VPNʵ���� */
    UINT64      auiDrvContext[L3VPN_DRV_CONTEXT_LEN];/* �������� */
    USHORT      usReserveVlan;                       /* ����vlan id */
}L3VPN_INSTANCESHOW_S;

ISSU typedef struct tagL3VPN_BindShow
{
    IF_INDEX    ifIndex;
    VRF_INDEX   vrfIndex;
    USHORT      usEpoch;
}L3VPN_BINDSHOW_S;

#define L3VPN_APP_BATCHCFGMSG          (0xFF-3)
#define L3VPN_APP_CFGSMOOTHSTART       (0xFF-2)  /* ����������ʼ */
#define L3VPN_APP_CFGSMOOTHEND         (0xFF-1)  /* ������������ */
#define L3VPN_APP_CFGMSG               0xFF      /* Ӧ�ó������α������Ϣ */

#define L3VPN_APP_IFBIND               0x30
#define L3VPN_APP_VRF                  0x40 

/* �ýṹͬl3v_cfg.c�ļ��е�L3VPN_CFG_IFBIND_S�ṹ��ͬ */
ISSU typedef struct tagL3VPN_APP_IFBIND
{
    IF_INDEX ifIndex;                                   /* �ӿ����� */
    CHAR     szVrfName[L3VPN_MAX_VRFNAME_LEN+1];        /* VPNʵ������ */
    BOOL_T   bIsUndo;
    UINT     uiOrigID;                                  /* ��Դģ��ID */
}L3VPN_APP_IFBIND_S;

/* �ýṹͬl3v_cfg.c�ļ��е�L3VPN_CFG_VRF_S�ṹ��ͬ */
ISSU typedef struct  tagL3VPN_APP_VRF
{
    CHAR       szVrfName[L3VPN_MAX_VRFNAME_LEN+1];      /* VPNʵ������ */
    VRF_INDEX  vrfIndex;                                /* VPNʵ������ */   
    BOOL_T     bIsUndo;
    UINT       uiOrigID;                                /* Դģ��ID */
}L3VPN_APP_VRF_S;

/* L3VPN�¼�����ص��������Ͷ��� */
typedef ULONG (*L3VPN_CALLBACK_FUNC_PF)(IN UINT uiEvent, IN VOID *pData, IN VOID *pUserData);


ISSU typedef struct tagL3VPN_MAXROUTE_LIMIT
{
    UINT  uiMaxLimit;       /* VPN����󼤻�·������ */
    USHORT usThreshHold;    /* VPN�¼���·�ɸ澯�ٷֱ� */
}L3VPN_MAXROUTE_LIMIT_S;


ISSU typedef struct tagL3VPN_VRFIF
{
    VRF_INDEX vrfIndex;     /* �ӿ�VRF������ȥ��ʱΪ�ӿ��ϵ�vrf���� */
    IF_INDEX  ifIndex;     /* �ӿ����� */
} L3VPN_VRFIF_S;


ISSU typedef struct tagL3VPN_RT_CHG
{
    UCHAR     ucCfgType;                                         /* RT�¼���������,���ɾ�� */
    VRF_INDEX vrfIndex;                                          /* VRF���� */
    UINT      uiRTNum;
    UCHAR     aucRTList[L3VPN_MAX_RT_NTFY_NUM][L3VPN_MAX_RDRT_LEN];    /* RT�б� */
} L3VPN_RT_CHG_S;


ISSU typedef struct tagL3VPN_VRF_CHG
{
    VRF_INDEX vrfIndex;                                 /* VRF���� */
    union {
        UCHAR   aucRD[L3VPN_MAX_RDRT_LEN];               /* VRF RDֵ */
        CHAR    szVrfName[L3VPN_MAX_VRFNAME_LEN+1];       /* VRF���� */
        CHAR    szRP[L3VPN_MAX_ROUTEPOLICYNAME_LEN+1];    /* ��·�ɲ��� */
        CHAR    szTP[L3VPN_MAX_TNLPOLICYNAME_LEN+1];      /* ������� */
        L3VPN_MAXROUTE_LIMIT_S stMaxRoute;              /* VPN�¼���·������ */
    } unInfo;
}L3VPN_VRF_CHG_S;



/* RTֵ���� */
#define L3VPN_RDRT_ISVALID(szValue) \
(\
    ((*((UINT*)((VOID*)(szValue)))) != 0xFFFFFFFF) \
  ||((*((UINT*)((VOID*)((szValue) + sizeof(UINT))))) != 0xFFFFFFFF) \
)


/* ·�����Ƽ��� */
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
        Input: pcFeatureName  ������
               uiEvType       ���ĵ��¼�����
               ucPriority     ���ȼ�
               pfNotify       �¼�����ص�
               pUserData      �û�����
       Output: ��
       Return: �ɹ�����LIPC���Ӿ����ʧ�ܷ���-1
      Caution: ��
Enter context: ��
 Exit context: ��
  Locking seq: ��
  Description: L3VPNע�ᴦ��ӿ�
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
        Input: iHandle  �ļ����
       Output: ��
       Return: ��
      Caution: ��
Enter context: ��
 Exit context: ��
  Locking seq: ��
  Description:  �û�̬Ӧ��ģ����L3VPNȥע��
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
        Input: iHandle  �ļ����
       Output: ��
       Return: ERROR_SUCCESS ����ɹ�  ERROR_FAILED ����ʧ��
      Caution: ��
Enter context: ��
 Exit context: ��
  Locking seq: ��
  Description: �û�̬Ӧ��ģ���֪��FD�仯��ͨ��L3VPN�ṩ�Ľӿڴ���VPN�¼�
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
        Input: iHandle  �ļ����
               vrfIndex VRF����
       Output: ��
       Return: ERROR_SUCCESS ��ȡ�ɹ�
               ERROR_FAILED ��ȡʧ��
      Caution: ��
Enter context: ��
 Exit context: ��
  Locking seq: ��
  Description: �û�̬Ӧ��ģ��������ȡVPN�¼�,vrfIndexΪ��Чֵ��ʶ��ȡ����
               VPN�������ȡָ��VPN��
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
        Input: iHandle    �ļ����
               uiEvType   L3VPN�¼�
               vrfIndex   vrf����
       Output: ��
       Return: ERROR_SUCCESS   ERROR_FAILED
      Caution: ��
Enter context: ��
 Exit context: ��
  Locking seq: ��
  Description: �û�̬Ӧ��ģ�鴦��VPN�¼���ɺ�ʹ��L3VPN�ṩ�Ľӿ�֪ͨL3VPN
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
        Input: pucRd      RDֵ
       Output: pucRdName  ת������ַ���
       Return: ERROR_SUCCESS ת���ɹ�  ERROR_INVALID_PARAMETER ת��ʧ��
      Caution: ��
Enter context: ��
 Exit context: ��
  Locking seq: ��
  Description: ��RDֵת��Ϊ�ַ�����ʽ
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
        Input: pucRdName  RD�ַ���
       Output: pucRd      RDֵ
       Return: ERROR_SUCCESS ת���ɹ�  ERROR_INVALID_PARAMETER ת��ʧ��
      Caution: ��
Enter context: ��
 Exit context: ��
  Locking seq: ��
  Description: ��RD�ַ�����ʽת��Ϊֵ
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
        Input: pucRt       RTֵ
       Output: pucRtName   ת������ַ���
       Return: ERROR_SUCCESS ת���ɹ�  ERROR_INVALID_PARAMETER ת��ʧ��
      Caution: ��
Enter context: ��
 Exit context: ��
  Locking seq: ��
  Description: ��RTֵת��Ϊ�ַ�����ʽ
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
        Input: pucRtName    RT�ַ���
       Output: pucRt        RTֵ
       Return: ERROR_SUCCESS ת���ɹ�  ERROR_INVALID_PARAMETER ת��ʧ��
      Caution: ��
Enter context: ��
 Exit context: ��
  Locking seq: ��
  Description: ��RT�ַ�����ʽת��Ϊֵ
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
        Input: uiModuleID  - ����������Ϣ��ģ��ID
               ucMsgType   - ��Ϣ����
               pUserData   - �û���Ϣ����
       Output: ��
       Return: ERROR_SUCCESS ��ȡ�ɹ�
               ERROR_FAILED ��ȡʧ��
  Description: �û�̬Ӧ��ģ����L3VPN�����������ݣ�Ŀǰ��֧�ֽӿڰ�VPN����
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
  Description: ������L3VPN������
        Input: NONE
       Output: NONE
       Return: INT
Enter context: ��
 Exit context: ��
  Locking seq: ��
      Caution: ������L3VPN�����������
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
  Description: �ر���L3VPN������
        Input: IN INT iHandle
       Output: NONE
       Return: VOID
Enter context: ��
 Exit context: ��
  Locking seq: ��
      Caution: ��
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
  Description: �����ӿڰ�
        Input: IN UINT uiModuleID
               IN L3VPN_APP_BATCHBIND_S *pstUserData[]
               IN UINT  uiCount
       Output: NONE
       Return: ULONG
Enter context: ��
 Exit context: ��
  Locking seq: ��
      Caution: �����ǳ�����
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
        Input: ��
       Output: ��
       Return: ERROR_SUCCESS �����ɹ���������  ERROR_FAILED ����ʧ��
      Caution: ��
Enter context: ��
 Exit context: ��
  Locking seq: ��
  Description: ��
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
        Input: vrfIndex       VRF����
       Output: ��
       Return: ����VLANֵ
      Caution: ��
Enter context: ��
 Exit context: ��
  Locking seq: ��
  Description: ���ر���VLAN��VLAN_INVALID_VALUE��ʾδ���ñ���VLAN
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
        Input: ��
       Output: ��
       Return: ���VRF����
      Caution: ��
Enter context: ��
 Exit context: ��
  Locking seq: ��
  Description: Ӧ��ģ���ȡϵͳ��֧�ֵ�VPN����񣬰�������ʵ��0
               �˹��ͬʱҲ��ϵͳVRF ID�����ֵ+1
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
        Input: pcVrfName    VPNʵ����
       Output: pusVrfIndex  �ӿ�����
       Return: ERROR_SUCCESS   ERROR_FAILED
      Caution: ��
Enter context: ��
 Exit context: ��
  Locking seq: ��
  Description: ͨ��VPNʵ������ȡ�������ϵ����нӿ�����
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
        Input: usVrfIndex  �ӿ�����
                 uiNameBufLen  ���ֻ���������,����'\0'������
       Output: pcVrfName   VPNʵ����
       Return: ERROR_SUCCESS   ERROR_FAILED
      Caution: ��
Enter context: ��
 Exit context: ��
  Locking seq: ��
  Description: ͨ���ӿ�������ȡ��󶨵�VPNʵ����
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
        Input: ifIndex       �ӿ�����
       Output: pusVrfIndex   VPNʵ������
       Return: ERROR_SUCCESS   ERROR_FAILED
      Caution: ��
Enter context: ��
 Exit context: ��
  Locking seq: ��
  Description: ͨ���ӿ�������ȡ��󶨵�VPNʵ������
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
        Input: CHAR *pcSnmpContext snmp ������
       Output: VRF_INDEX *pvrfIndex     VPNʵ������
       Return: ERROR_SUCCESS   ERROR_FAILED
      Caution: ��
Enter context: ��
 Exit context: ��
  Locking seq: ��
  Description: ͨ��snmp context ��ȡ�����õ�VPNʵ������, ��L3VPNæ��10s��δ�ظ����ݣ�
                   ��Ͽ����ӣ�������Ч��VRFINDEX����ֹЭ�����������recv
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
