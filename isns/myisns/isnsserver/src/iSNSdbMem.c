/*****************************************************************************
 Copyright (c) 2010, Hangzhou H3C Technologies Co., Ltd. All rights reserved.
------------------------------------------------------------------------------
                               iSNSNdbMem.c

  Project Code: Comware V7
   Module Name: ISNS
  Date Created: 2016-10-25
        Author: liangjinchao@dian
   Description: �Ի������ݽṹ���в������������ݵĲ��ҡ���ӡ�ɾ������

--------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
--------------------------------------------------------------------------------

*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/basetype.h>
#include <sys/error.h>
#include <sys/assert.h>
#include <sys/list.h>

#include "iSNStypes.h"
#include "iSNSList.h"
#include "iSNSmsg.h"
#include "iSNSdebug.h"
#include "iSNSNdb.h"
#include "iSNSdbMem.h"
#include "iSNSMemData.h"

#define ISNS_MEM_LIST_GLOBAL_NUM        4

typedef enum
{
    ISNS_MEM_INVALID = 0,
    ISNS_MEM_BIN,      /* ������/�������� */
    ISNS_MEM_NUM,      /* ��������(UINT32) */
    ISNS_MEM_STR,      /* �ַ�������(uiSizeΪ���ռ�) */
    ISNS_MEM_TYPE_MAX,
}ISNS_MEM_TYPE_E;

typedef struct tagIsnsMemStruct
{
    ISNS_MEM_TYPE_E enType;
    UINT uiOffset;
    UINT uiSize;
}ISNS_MEM_STRUCT_S;

typedef struct tagIsnsMemListInfo
{
    UINT uiOffset;
    ISNS_Database_Key enParent;
}ISNS_MEM_LIST_INFO_S;

/* KEY���ͣ������ISNS_DBKey��λ�ã�(���)��С */
STATIC const ISNS_MEM_STRUCT_S g_astIsnsMemKey[ISNS_DATABASE_MAX] =
{
    [ENTITY_ID_KEY] = {ISNS_MEM_STR, offsetof(ISNS_DBKey, val.entity_key.id), ENTITY_ID_SIZE},
    [PORTAL_ID_KEY] = {ISNS_MEM_BIN, offsetof(ISNS_DBKey, val.portal_key), sizeof(SOIP_Portal_Key)},
    [DDS_ID_KEY] = {ISNS_MEM_NUM, offsetof(ISNS_DBKey, val.dds_key.id), DDS_KEY_SIZE},
    [DD_ID_KEY] = {ISNS_MEM_NUM, offsetof(ISNS_DBKey, val.dd_key.id), DD_KEY_SIZE},
    [ISCSI_ID_KEY] = {ISNS_MEM_STR, offsetof(ISNS_DBKey, val.iscsi_key.v), MAX_ISCSI_NODE_ID_SIZE},
    [PORTAL_GROUP_ID_KEY] = {ISNS_MEM_BIN, offsetof(ISNS_DBKey, val.portal_group_key), sizeof(SOIP_Portal_Group_Key)},
    [ENTITY_IDX_KEY] = {ISNS_MEM_BIN, offsetof(ISNS_DBKey, val.idx), sizeof(SOIP_IDX_Key)},
    [ISCSI_IDX_KEY] = {ISNS_MEM_BIN, offsetof(ISNS_DBKey, val.idx), sizeof(SOIP_IDX_Key)},
    [PORTAL_IDX_KEY] = {ISNS_MEM_BIN, offsetof(ISNS_DBKey, val.idx), sizeof(SOIP_IDX_Key)},
};

/* VALUE���ͣ������SOIP_DB_Entry��λ�ã�(���)��С */
STATIC const ISNS_MEM_STRUCT_S g_astIsnsMemValue[ISNS_DATABASE_MAX] =
{
    [ENTITY_ID_KEY] = {ISNS_MEM_BIN, offsetof(SOIP_DB_Entry, data.entity), sizeof(SOIP_Entity)},
    [PORTAL_ID_KEY] = {ISNS_MEM_BIN, offsetof(SOIP_DB_Entry, data.portal), sizeof(SOIP_Portal)},
    [DDS_ID_KEY] = {ISNS_MEM_BIN, offsetof(SOIP_DB_Entry, data.dds), sizeof(SOIP_Dds)},
    [DD_ID_KEY] = {ISNS_MEM_BIN, offsetof(SOIP_DB_Entry, data.dd), sizeof(SOIP_Dd)},
    [ISCSI_ID_KEY] = {ISNS_MEM_BIN, offsetof(SOIP_DB_Entry, data.scsi_node), sizeof(SOIP_Iscsi)},
    [PORTAL_GROUP_ID_KEY] = {ISNS_MEM_BIN, offsetof(SOIP_DB_Entry, data.portal_group), sizeof(SOIP_Portal_Group)},
    [ENTITY_IDX_KEY] = {ISNS_MEM_BIN, offsetof(SOIP_DB_Entry, data.entity_idx), sizeof(SOIP_Entity_Id)},
    [ISCSI_IDX_KEY] = {ISNS_MEM_BIN, offsetof(SOIP_DB_Entry, data.iscsi_idx), sizeof(SOIP_ISCSI_Index)},
    [PORTAL_IDX_KEY] = {ISNS_MEM_BIN, offsetof(SOIP_DB_Entry, data.portal_idx), sizeof(SOIP_DB_Portal)},
};

/* ʮ������λ����Ϣ */
STATIC const ISNS_MEM_LIST_INFO_S g_auiIsnsListOffset[DATA_LIST_MAX] =
{
    [ISCSI_DD_LIST] = {offsetof(SOIP_Iscsi, dd_id_list), ISCSI_ID_KEY},
    [ENTITY_PORTAL_LIST] = {offsetof(SOIP_Entity, iportal_list), ENTITY_ID_KEY},
    [ENTITY_ISCSI_LIST] = {offsetof(SOIP_Entity, iscsi_node_list), ENTITY_ID_KEY},
    [DD_MEMBER_LIST] = {offsetof(SOIP_Dd, member_list), DD_ID_KEY},
    [DD_DDS_LIST] = {offsetof(SOIP_Dd, dds_list), DD_ID_KEY},
    [DDS_DD_LIST] = {offsetof(SOIP_Dds, dd_list), DDS_ID_KEY},

    [SCN_CALLBACK_LIST] = { 0 },
    [SCN_LIST] = { 0 },
};

int isns_list_debug = 0;


/*********************************************************************
     Func Name : _isns_FormatByKey
  Date Created : 2016/10/26
        Author : liangjinchao@dian
   Description : DB KEY ת KEY
         Input : IN const ISNS_DBKey *pstDbKey
        Output : OUT datum *pstOutKey
        Return : �ɹ�/ʧ��
       Caution : ��
----------------------------------------------------------------------
 Modification History
    DATE        NAME             DESCRIPTION
----------------------------------------------------------------------

*********************************************************************/
STATIC ULONG _isns_FormatByKey(IN const ISNS_DBKey *pstDbKey, OUT datum *pstOutKey, OUT UINT32 *puiTmp)
{
    const ISNS_MEM_STRUCT_S *pstMem;
    datum stKey = { 0 };

    if(NULL == pstDbKey)
    {
        *pstOutKey = stKey;
        return ERROR_SUCCESS;
    }

    if(pstDbKey->tag >= ISNS_DATABASE_MAX)
    {
        *pstOutKey = stKey;
        return ERROR_FAILED;
    }

    pstMem = &g_astIsnsMemKey[pstDbKey->tag];
    if(pstMem->enType <= ISNS_MEM_INVALID || pstMem->enType >= ISNS_MEM_TYPE_MAX)
    {
        *pstOutKey = stKey;
        return ERROR_FAILED;
    }

    stKey.dptr = (CHAR *)pstDbKey + pstMem->uiOffset;
    stKey.dsize = pstMem->uiSize;
    if(ISNS_MEM_STR == pstMem->enType)
    {
        stKey.dsize = strlen(stKey.dptr);
    }
    else if(ISNS_MEM_NUM == pstMem->enType && sizeof(UINT32) ==  pstMem->uiSize)
    {
        *puiTmp = htonl(*(UINT32 *)(UCHAR *)stKey.dptr);
        stKey.dptr = (CHAR *)puiTmp;
    }

    *pstOutKey = stKey;
    return ERROR_SUCCESS;

}

/*********************************************************************
     Func Name : _isns_FormatByKeyValue
  Date Created : 2016/10/26
        Author : liangjinchao@dian
   Description : DB KEY ת KEY, ENTRY ת VALUE
         Input : IN const ISNS_DBKey *pstDbKey, IN const SOIP_DB_Entry *pstEntry
        Output : OUT datum *pstOutKey,  OUT datum *pstOutValue
        Return : �ɹ�/ʧ��
       Caution : ��
----------------------------------------------------------------------
 Modification History
    DATE        NAME             DESCRIPTION
----------------------------------------------------------------------

*********************************************************************/
STATIC ULONG _isns_FormatByKeyValue(IN const ISNS_DBKey *pstDbKey, IN const SOIP_DB_Entry *pstEntry,
                                    OUT datum *pstOutKey,  OUT datum *pstOutValue, OUT UINT32 *puiTmp)
{
    const ISNS_MEM_STRUCT_S *pstMem;
    ULONG ulRet = ERROR_SUCCESS;
    datum stValue = { 0 };

    if(ERROR_SUCCESS != _isns_FormatByKey(pstDbKey, pstOutKey, puiTmp))
    {
        *pstOutValue = stValue;
        return ERROR_FAILED;
    }

    pstMem = &g_astIsnsMemValue[pstDbKey->tag];
    stValue.dptr = (CHAR *)pstEntry + pstMem->uiOffset;
    stValue.dsize = pstMem->uiSize;

    *pstOutValue = stValue;
    return ulRet;

}

/*********************************************************************
     Func Name : _isns_Key2DbKey
  Date Created : 2016/10/26
        Author : liangjinchao@dian
   Description : KEY ת DB KEY
         Input : IN UINT uiType, IN datum stKey
        Output : OUT ISNS_DBKey *pstDbKey
        Return : �ɹ�/ʧ��
       Caution : ��
----------------------------------------------------------------------
 Modification History
    DATE        NAME             DESCRIPTION
----------------------------------------------------------------------

*********************************************************************/
STATIC ULONG _isns_Key2DbKey(IN UINT uiType, IN datum stKey, OUT ISNS_DBKey *pstDbKey)
{
    const ISNS_MEM_STRUCT_S *pstMem;
    CHAR *pcDst;

    memset(pstDbKey, 0, sizeof(ISNS_DBKey));
    pstDbKey->tag = uiType;

    if(uiType >= ISNS_DATABASE_MAX)
    {
        return ERROR_FAILED;
    }

    pstMem = &g_astIsnsMemKey[uiType];
    if(pstMem->enType <= ISNS_MEM_INVALID || pstMem->enType >= ISNS_MEM_TYPE_MAX)
    {
        return ERROR_FAILED;
    }

    pcDst = (CHAR *)pstDbKey + pstMem->uiOffset;
    if(ISNS_MEM_STR == pstMem->enType)
    {
        /* ������ַ�������ĩβ��\0 */
        __ISNS_COPY(pcDst, pstMem->uiSize-1, stKey.dptr, stKey.dsize);
        pstDbKey->len = stKey.dsize;
    }
    else if(ISNS_MEM_NUM == pstMem->enType && sizeof(UINT32) == pstMem->uiSize)
    {
        *(UINT32 *)pcDst = ntohl(*(UINT32 *)(UCHAR *)stKey.dptr);
        pstDbKey->len = sizeof(UINT32);
    }
    else
    {
        memcpy(pcDst, stKey.dptr, pstMem->uiSize);
        pstDbKey->len = pstMem->uiSize;
    }

    return ERROR_SUCCESS;
}

/*********************************************************************
     Func Name : _isns_Value2Entry
  Date Created : 2016/10/26
        Author : liangjinchao@dian
   Description : VALUE ת ENTRY
         Input : IN UINT uiType, IN datum stValue
        Output : OUT SOIP_DB_Entry *pstEntry
        Return : �ɹ�/ʧ��
       Caution : ��
----------------------------------------------------------------------
 Modification History
    DATE        NAME             DESCRIPTION
----------------------------------------------------------------------

*********************************************************************/
STATIC ULONG _isns_Value2Entry(IN UINT uiType, IN datum stValue, OUT SOIP_DB_Entry *pstEntry)
{
    const ISNS_MEM_STRUCT_S *pstMem;

    memset(pstEntry, 0, sizeof(SOIP_DB_Entry));
    pstEntry->data_type = uiType;
    if(NULL == stValue.dptr)
    {
        return ERROR_FAILED;
    }

    pstMem = &g_astIsnsMemValue[uiType];
    if(pstMem->enType <= ISNS_MEM_INVALID || pstMem->enType >= ISNS_MEM_TYPE_MAX)
    {
        return ERROR_FAILED;
    }

    memcpy((CHAR *)pstEntry + pstMem->uiOffset, stValue.dptr, pstMem->uiSize);

    return ERROR_SUCCESS;
}

/*********************************************************************
     Func Name : _isns_FreeMemListHead
  Date Created : 2016/10/26
        Author : liangjinchao@dian
   Description : �ͷ�ʮ������ͷ
         Input : IN const ISNS_DBKey *pstDbKey
        Output :
        Return : �ɹ�/ʧ��
       Caution : ��
----------------------------------------------------------------------
 Modification History
    DATE        NAME             DESCRIPTION
----------------------------------------------------------------------

*********************************************************************/
STATIC VOID _isns_FreeMemListHead(IN const ISNS_DBKey *pstDbKey)
{
    SOIP_DB_Entry stEntry;
    ISNS_LIST *pstList;
    CHAR *pcParent;
    UINT uiI;

    for(uiI = 0; uiI < DATA_LIST_MAX; uiI++)
    {
        if(g_auiIsnsListOffset[uiI].enParent != pstDbKey->tag)
        {
            continue;
        }

        if(ERROR_SUCCESS != ISNS_MEM_Read(pstDbKey, &stEntry))
        {
            continue;
        }

        pcParent = (CHAR *)&stEntry + g_astIsnsMemValue[pstDbKey->tag].uiOffset;
        pstList = (ISNS_LIST *)(pcParent + g_auiIsnsListOffset[uiI].uiOffset);
        if(NULL != pstList->pstHead)
        {
            memset(pstList->pstHead, 0, sizeof(DTQ_HEAD_S));
            free(pstList->pstHead);
            pstList->pstHead = NULL;
        }
    }

    return ;
}

/*********************************************************************
     Func Name : ISNS_MEM_Init
  Date Created : 2016/10/25
        Author : liangjinchao@dian
   Description : ��ʼ���ڴ����ݿ�
         Input : IN UINT uiMaxTypeCount
        Output : ��
        Return : �ɹ�/ʧ��
       Caution : ��
----------------------------------------------------------------------
 Modification History
    DATE        NAME             DESCRIPTION
----------------------------------------------------------------------

*********************************************************************/
ULONG ISNS_MEM_Init(IN UINT uiMaxTypeCount)
{
    return ISNS_MEMDATA_Init(uiMaxTypeCount);
}

/*********************************************************************
     Func Name : ISNS_MEM_Fini
  Date Created : 2016/10/25
        Author : liangjinchao@dian
   Description : ȥ��ʼ���ڴ����ݿ�
         Input : ��
        Output : ��
        Return : ��
       Caution : ��
----------------------------------------------------------------------
 Modification History
    DATE        NAME             DESCRIPTION
----------------------------------------------------------------------

*********************************************************************/
VOID ISNS_MEM_Fini()
{
    return ;
}

/*********************************************************************
     Func Name : ISNS_MEM_Write
  Date Created : 2016/10/26
        Author : liangjinchao@dian
   Description : �������ݵ��ڴ棬����Ѵ����򸲸�
         Input : IN const ISNS_DBKey *pstDbKey, IN SOIP_DB_Entry *pstEntry
        Output : ��
        Return : �ɹ�/ʧ��
       Caution : pstDbKeyֻ��TAG��VAL
----------------------------------------------------------------------
 Modification History
    DATE        NAME             DESCRIPTION
----------------------------------------------------------------------

*********************************************************************/
INT ISNS_MEM_Write(IN const ISNS_DBKey *pstDbKey, IN const SOIP_DB_Entry *pstEntry)
{
    datum stKey, stValue;
    UINT32 uiTmp;

    if(ERROR_SUCCESS != _isns_FormatByKeyValue(pstDbKey, pstEntry, &stKey, &stValue, &uiTmp))
    {
        __LOG_ERROR ("When write -- Undefined database tag: %i", pstDbKey->tag);
        return ISNS_UNKNOWN_ERR;
    }

    if(ERROR_SUCCESS != ISNS_MEMDATA_Write(pstDbKey->tag, stKey, stValue))
    {
        __LOG_ERROR ("When write -- Write Memory data failed, tag = %i", pstDbKey->tag);
        return ISNS_UNKNOWN_ERR;
    }

    return SUCCESS;
}

/*********************************************************************
     Func Name : ISNS_MEM_Delete
  Date Created : 2016/10/26
        Author : liangjinchao@dian
   Description : ɾ���ڴ��е�����
         Input : IN const ISNS_DBKey *pstDbKey
        Output : ��
        Return : �ɹ�/ʧ��
       Caution : ��
----------------------------------------------------------------------
 Modification History
    DATE        NAME             DESCRIPTION
----------------------------------------------------------------------

*********************************************************************/
INT ISNS_MEM_Delete(IN const ISNS_DBKey *pstDbKey)
{
    datum stKey;
    UINT32 uiTmp;

    _isns_FreeMemListHead(pstDbKey);

    if(ERROR_SUCCESS != _isns_FormatByKey(pstDbKey, &stKey, &uiTmp))
    {
        __LOG_ERROR ("When delete -- Undefined database tag: %i", pstDbKey->tag);
        return ISNS_UNKNOWN_ERR;
    }

    (VOID)ISNS_MEMDATA_Delete(pstDbKey->tag, stKey);

    return SUCCESS;
}

/*********************************************************************
     Func Name : ISNS_MEM_Read
  Date Created : 2016/10/26
        Author : liangjinchao@dian
   Description : ��ȡ�ڴ��е�����
         Input : IN const ISNS_DBKey *pstDbKey, OUT SOIP_DB_Entry *pstEntry
        Output : ��
        Return : �ɹ�/ʧ��
       Caution : �����߿��Բ���pstDbKey�е�len
----------------------------------------------------------------------
 Modification History
    DATE        NAME             DESCRIPTION
----------------------------------------------------------------------

*********************************************************************/
INT ISNS_MEM_Read(IN const ISNS_DBKey *pstDbKey, OUT SOIP_DB_Entry *pstEntry)
{
    datum stKey, stValue;
    UINT32 uiTmp;

    if(ERROR_SUCCESS != _isns_FormatByKey(pstDbKey, &stKey, &uiTmp))
    {
        __LOG_ERROR ("When read -- Undefined database tag: %i", pstDbKey->tag);
        return ISNS_UNKNOWN_ERR;
    }

    stValue = ISNS_MEMDATA_Read(pstDbKey->tag, stKey);
    if(NULL == stValue.dptr)   /* Ŀǰ���������û���ǿ�VALUE�����  */
    {
        return ISNS_NO_SUCH_ENTRY_ERR;
    }

    if(ERROR_SUCCESS != _isns_Value2Entry(pstDbKey->tag, stValue, pstEntry))
    {
        __LOG_ERROR ("When read -- Can not convert value, tag = %i", pstDbKey->tag);
        return ISNS_UNKNOWN_ERR;
    }

    return SUCCESS;
}

/*********************************************************************
     Func Name : ISNS_MEM_NextKey
  Date Created : 2016/10/26
        Author : liangjinchao@dian
   Description : ��ȡ��һ��KEY
         Input : INOUT ISNS_DBKey *pstDbKey
        Output : INOUT ISNS_DBKey *pstDbKey
        Return : �Ƿ�Ҫ��������
       Caution : Ϊ���ݾɽӿڶ�д�����������Ҫ�󣬲��Ƽ����˽ӿ���������
                 ���Ҫȡ��һ��key����pstDbKey->len��0
----------------------------------------------------------------------
 Modification History
    DATE        NAME             DESCRIPTION
----------------------------------------------------------------------

*********************************************************************/
INT ISNS_MEM_NextKey(INOUT ISNS_DBKey *pstDbKey)
{
    datum stKey, stNextKey;
    UINT uiType = pstDbKey->tag;
    UINT32 uiTmp;

    memset(&stKey, 0, sizeof(stKey));
    if(0 != pstDbKey->len)
    {
        if(ERROR_SUCCESS != _isns_FormatByKey(pstDbKey, &stKey, &uiTmp))
        {
            __LOG_ERROR ("When get next -- Undefined database tag: %i", uiType);
            return ISNS_UNKNOWN_ERR;
        }
    }

    stNextKey = ISNS_MEMDATA_GetNext(uiType, stKey, NULL);
    if(NULL == stNextKey.dptr)
    {
        return ISNS_NO_SUCH_ENTRY_ERR;
    }

    /* KEY --> DB KEY */
    if(ERROR_SUCCESS != _isns_Key2DbKey(uiType, stNextKey, pstDbKey))
    {
        __LOG_ERROR ("When get next -- Can not convert value, tag = %i", uiType);
        return ISNS_UNKNOWN_ERR;
    }

    return SUCCESS;
}

/*********************************************************************
     Func Name : ISNS_MEM_Iter
  Date Created : 2016/10/26
        Author : liangjinchao@dian
   Description : ���������ӿ�
         InOut : INOUT ISNS_DBKey *pstDbKey, INOUT VOID **ppIter
        Output : OUT SOIP_DB_Entry *pstEntry
        Return : �Ƿ�Ҫ��������
       Caution : ppIterΪһ��VOID *��ָ��(��ֵ��ʼ��Ϊ0)��ַ
                 eg: ISNS_DBKey key = { 0 };  key.tag = xxx;
                     VOID *pIter = NULL;
                     SOIP_DB_Entry entry;
                 while(SUCCESS == ISNS_MEM_Iter(&key, &pIter, &entry)){xxx}
                 �����������Ϊ0��pIter����NULL
                 ���ֻ�����KEY��ȡ���������������ΪNULL
----------------------------------------------------------------------
 Modification History
    DATE        NAME             DESCRIPTION
----------------------------------------------------------------------

*********************************************************************/
INT ISNS_MEM_Iter(INOUT ISNS_DBKey *pstDbKey, INOUT VOID **ppIter, OUT SOIP_DB_Entry *pstEntry)
{
    datum stKey = { 0 }, stValue = { 0 };
    UINT uiType = pstDbKey->tag;
    ULONG ulRet;

    if(NULL != pstEntry)
    {
        ulRet = ISNS_MEMDATA_Iter(uiType, ppIter, &stKey, &stValue);
    }
    else
    {
        ulRet = ISNS_MEMDATA_Iter(uiType, ppIter, &stKey, NULL);
    }

    if(ERROR_SUCCESS != ulRet)
    {
        return ISNS_NO_SUCH_ENTRY_ERR;
    }

    /* KEY --> DB KEY ,  VALUE --> ENTRY */
    if(ERROR_SUCCESS != _isns_Key2DbKey(uiType, stKey, pstDbKey))
    {
        __LOG_ERROR ("When get next -- Can not convert value, tag = %i", uiType);
        return ISNS_UNKNOWN_ERR;
    }

    if(NULL != pstEntry)
    {
        if(ERROR_SUCCESS != _isns_Value2Entry(uiType, stValue, pstEntry))
        {
            __LOG_ERROR ("When get next -- Can not convert value, tag = %i", uiType);
            return ISNS_UNKNOWN_ERR;
        }
    }

    return SUCCESS;
}

/*********************************************************************
     Func Name : ISNS_MEM_List_IsInit
  Date Created : 2016/10/26
        Author : liangjinchao@dian
   Description : �ж��б��Ƿ��ʼ��
         Input : IN const ISNS_LIST *pstList
        Output : ��
        Return : YES/NO
       Caution : ��
----------------------------------------------------------------------
 Modification History
    DATE        NAME             DESCRIPTION
----------------------------------------------------------------------

*********************************************************************/
BOOL_T ISNS_MEM_List_IsInit(IN const ISNS_LIST *pstList)
{
    if(pstList->list_id <= DATA_LIST_INVALID || pstList->list_id >= DATA_LIST_MAX ||
       NULL == pstList->pstHead || NULL == pstList->pstHead->stHead.pstNext ||
       NULL == pstList->pstHead->stHead.pstPrev)
    {
        return BOOL_FALSE;
    }

    return BOOL_TRUE;
}

/*********************************************************************
     Func Name : ISNS_MEM_List_Init
  Date Created : 2016/10/26
        Author : liangjinchao@dian
   Description : ��ʼ���б�
         Input : IN INT iListId, IN VOID *pRecord
        Output : ��
        Return : �ɹ�/ʧ��
       Caution : ��
----------------------------------------------------------------------
 Modification History
    DATE        NAME             DESCRIPTION
----------------------------------------------------------------------

*********************************************************************/
INT ISNS_MEM_List_Init(IN INT iListId, IN VOID *pRecord)
{
    ISNS_LIST *pstList = NULL;

    if(DATA_LIST_INVALID < iListId && iListId < DATA_LIST_MAX)
    {
        pstList = (ISNS_LIST *)((UCHAR *)pRecord + g_auiIsnsListOffset[iListId].uiOffset);
    }
    else if(DATA_LIST_MAX < iListId && iListId < DATA_LIST_OLD_MAX)
    {
        return SUCCESS;
    }
    else
    {
        __LOG_ERROR("Init List: Unknown list type %d", iListId);
        return ISNS_UNKNOWN_ERR;
    }

    memset(pstList, 0, sizeof(ISNS_LIST));

    pstList->pstHead = (DTQ_HEAD_S *)malloc(sizeof(DTQ_HEAD_S));
    if(NULL == pstList->pstHead)
    {
        return ISNS_UNKNOWN_ERR;
    }
    DTQ_Init(pstList->pstHead);
    pstList->list_id = iListId;

    __DEBUG (isns_list_debug &1,InitList iListId:%i,pstList->list_id);

    return ( SUCCESS );
}

/*********************************************************************
     Func Name : ISNS_MEM_List_Delete
  Date Created : 2016/10/26
        Author : liangjinchao@dian
   Description : ɾ��/ȥ��ʼ���б�
         Input : IN ISNS_LIST *pstList
        Output : ��
        Return : �ɹ�/ʧ��
       Caution : ��
----------------------------------------------------------------------
 Modification History
    DATE        NAME             DESCRIPTION
----------------------------------------------------------------------

*********************************************************************/
INT ISNS_MEM_List_Delete(IN ISNS_LIST *pstList)
{
    DTQ_HEAD_S *pstHead = pstList->pstHead;
    ISNS_LIST_NODE *pstNode;

    __DEBUG (isns_list_debug &1,DeleteList list_id:%i,pstList->list_id);

    if(BOOL_FALSE == ISNS_MEM_List_IsInit(pstList))
    {
        return ISNS_UNKNOWN_ERR;
    }
    if(NULL == pstList->pstHead)
    {
        return (SUCCESS);
    }

    DTQ_FOREACH_ENTRY(pstHead, pstNode, stNode)
    {
        DTQ_Del(&pstNode->stNode);
        free(pstNode->data);
        free(pstNode);
    }

    return (SUCCESS);
}

/*********************************************************************
     Func Name : ISNS_MEM_List_GetParent
  Date Created : 2016/10/26
        Author : liangjinchao@dian
   Description : ��ȡ����б���ⲿ�ṹ��ָ��
         Input : IN ISNS_LIST *pstList
        Output : ��
        Return : �ɹ�/ʧ��
       Caution : ��
----------------------------------------------------------------------
 Modification History
    DATE        NAME             DESCRIPTION
----------------------------------------------------------------------

*********************************************************************/
VOID *ISNS_MEM_List_GetParent(IN ISNS_LIST *pstList)
{
    if(BOOL_FALSE == ISNS_MEM_List_IsInit(pstList))
    {
        __LOG_ERROR ("List_GetParent: Not init, listId=%d", pstList->list_id);
        return NULL;
    }
    return (VOID *)((UCHAR *)pstList - g_auiIsnsListOffset[pstList->list_id].uiOffset);
}

/*********************************************************************
     Func Name : ISNS_MEM_List_RemoveNode
  Date Created : 2016/10/26
        Author : liangjinchao@dian
   Description : ɾ���б��һ���ڵ�
         Input : IN ISNS_LIST *pstList, IN ISNS_LIST_NODE *pstNode
        Output : ��
        Return : �ɹ�/ʧ��
       Caution : ��
----------------------------------------------------------------------
 Modification History
    DATE        NAME             DESCRIPTION
----------------------------------------------------------------------

*********************************************************************/
INT ISNS_MEM_List_RemoveNode(IN ISNS_LIST *pstList, IN ISNS_LIST_NODE *pstNode)
{
    __DEBUG (isns_list_debug &1,Remove Node);

    if(BOOL_FALSE == ISNS_MEM_List_IsInit(pstList))
    {
        return ISNS_UNKNOWN_ERR;
    }

    DTQ_Del(&pstNode->stNode);
    free(pstNode->data);
    free(pstNode);

    return ( SUCCESS );
}

/*********************************************************************
     Func Name : ISNS_MEM_List_GetNodeData
  Date Created : 2016/10/26
        Author : liangjinchao@dian
   Description : ��ȡ�ڵ�����
         Input : IN ISNS_LIST_NODE *pstNode, OUT INT *piSize
        Output : ��
        Return : �ɹ�/ʧ��
       Caution : ��
----------------------------------------------------------------------
 Modification History
    DATE        NAME             DESCRIPTION
----------------------------------------------------------------------
*********************************************************************/
VOID *ISNS_MEM_List_GetNodeData(IN ISNS_LIST_NODE *pstNode, OUT INT *piSize)
{
    STATIC VOID *apTmp[DATA_LIST_MAX][ISNS_MEM_LIST_GLOBAL_NUM] = { { 0 } };
    STATIC UINT auiPos[DATA_LIST_MAX] = { 0 };
    VOID *pData;
    UINT uiPos;

    if(NULL != piSize)
    {
        *piSize = pstNode->data_size;
    }

    if(pstNode->list_id >= DATA_LIST_MAX)
    {
        return NULL;
    }

    uiPos = auiPos[pstNode->list_id];
    pData = apTmp[pstNode->list_id][uiPos];
    if(NULL != pData)
    {
        free(pData);
        apTmp[pstNode->list_id][uiPos] = NULL;
    }

    pData = malloc(pstNode->data_size);
    if(NULL == pData)
    {
        return NULL;
    }
    apTmp[pstNode->list_id][uiPos] = pData;
    auiPos[pstNode->list_id] = (auiPos[pstNode->list_id] + 1) % ISNS_MEM_LIST_GLOBAL_NUM;

    memcpy(pData, pstNode->data, pstNode->data_size);

    return pData;
}

/*********************************************************************
     Func Name : ISNS_MEM_List_FindNode
  Date Created : 2016/10/26
        Author : liangjinchao@dian
   Description : �����б��е�ĳ�����ݵĽڵ�
         Input : IN ISNS_LIST *pstList, IN CHAR *pcData, IN INT iDataSize
        Output : ��
        Return : �ڵ�
       Caution : ��
----------------------------------------------------------------------
 Modification History
    DATE        NAME             DESCRIPTION
----------------------------------------------------------------------

*********************************************************************/
ISNS_LIST_NODE *ISNS_MEM_List_FindNode(IN ISNS_LIST *pstList,
                                       IN CHAR *pcData, IN INT iDataSize)
{
    ISNS_LIST_NODE *pstNode;
    const CHAR *pcTmpData;
    INT iTmpSize;

    __DEBUG (isns_list_debug &1,FindNode list_id:%i, pstList->list_id);

    if(BOOL_FALSE == ISNS_MEM_List_IsInit(pstList))
    {
        __LOG_ERROR ("Find Node: Not init, listId=%d, dataSize=%d", pstList->list_id, iDataSize);
        return NULL;
    }

    pstNode = NULL;
    while (NULL != (pstNode=ISNS_MEM_List_GetNext(pstList, pstNode, &pcTmpData, &iTmpSize)) )
    {
        if (iTmpSize == iDataSize && 0 == memcmp(pcTmpData, pcData, iDataSize))
        {
            break;
        }
    }

    return pstNode;
}

/*********************************************************************
     Func Name : ISNS_MEM_List_AddNode
  Date Created : 2016/10/26
        Author : liangjinchao@dian
   Description : ���һ�����ݵ��б�ĩβ
         Input : IN ISNS_LIST *pstList, IN CHAR *pcData, IN INT iDataSize
        Output : ��
        Return : �ɹ�/ʧ��
       Caution : ��
----------------------------------------------------------------------
 Modification History
    DATE        NAME             DESCRIPTION
----------------------------------------------------------------------

*********************************************************************/
INT ISNS_MEM_List_AddNode(IN ISNS_LIST *pstList, IN CHAR *pcData, IN INT iDataSize)
{
    ISNS_LIST_NODE *pstNode;

    __DEBUG (isns_list_debug &1, AddNode - list_id:%i,pstList->list_id);

    if(BOOL_FALSE == ISNS_MEM_List_IsInit(pstList))
    {
        __LOG_ERROR ("Add Node: Not init, listId=%d, dataSize=%d", pstList->list_id, iDataSize);
        return ISNS_UNKNOWN_ERR;
    }

    pstNode = (ISNS_LIST_NODE *)malloc(sizeof(ISNS_LIST_NODE));
    if(NULL == pstNode)
    {
        return ISNS_UNKNOWN_ERR;
    }

    memset(pstNode, 0, sizeof(ISNS_LIST_NODE));
    pstNode->list_id = pstList->list_id;
    pstNode->data = (char *)malloc(iDataSize + 1);
    if(NULL == pstNode->data)
    {
        free(pstNode);
        return ISNS_UNKNOWN_ERR;
    }

    memcpy(pstNode->data, pcData, iDataSize);
    ((CHAR *)pstNode->data)[iDataSize] = 0;
    pstNode->data_size = iDataSize;
    DTQ_AddTail(pstList->pstHead, &pstNode->stNode);

    return ( SUCCESS );
}

/*********************************************************************
     Func Name : ISNS_MEM_List_IsEmpty
  Date Created : 2016/10/26
        Author : liangjinchao@dian
   Description : �ж��б��Ƿ�Ϊ��
         Input : IN ISNS_LIST *pstList
        Output : ��
        Return : �ɹ�/ʧ��
       Caution : ��
----------------------------------------------------------------------
 Modification History
    DATE        NAME             DESCRIPTION
----------------------------------------------------------------------

*********************************************************************/
BOOL_T ISNS_MEM_List_IsEmpty(IN ISNS_LIST *pstList)
{
    if(NULL == pstList->pstHead || DTQ_IsEmpty(pstList->pstHead))
    {
        return BOOL_TRUE;
    }
    return BOOL_FALSE;
}

/*********************************************************************
     Func Name : ISNS_MEM_List_GetNext
  Date Created : 2016/10/26
        Author : liangjinchao@dian
   Description : ��ȡ��һ���ڵ㼰���ݣ������ڷ���NULL *piSize����ΪNULL
         Input : IN ISNS_LIST *pstList, IN ISNS_LIST_NODE *pstNode
        Output : OUT CHAR *ppcData, OUT INT *piSize
        Return : ��һ���ڵ�
       Caution : *ppcDataΪconst char *���ͣ��ⲿ��Ӧ�޸�,
                 �ⲿ��Ҫ���жϷ���ֵ�Ƿ�ΪNULL��ȡ����
----------------------------------------------------------------------
 Modification History
    DATE        NAME             DESCRIPTION
----------------------------------------------------------------------

*********************************************************************/
ISNS_LIST_NODE *ISNS_MEM_List_GetNext(IN ISNS_LIST *pstList, IN ISNS_LIST_NODE *pstNode,
                                      OUT const CHAR **ppcData, OUT INT *piSize)
{
    ISNS_LIST_NODE *pstRet;

    __DEBUG (isns_list_debug &1,GetNextNode list_id:%i, pstList->list_id);

    if(BOOL_FALSE == ISNS_MEM_List_IsInit(pstList))
    {
        __LOG_ERROR ("GetNextNode: Not init, listId=%d", pstList->list_id);
        return NULL;
    }

    if(NULL == pstNode)
    {
        pstRet = DTQ_ENTRY_FIRST(pstList->pstHead, ISNS_LIST_NODE, stNode);
    }
    else
    {
        pstRet = DTQ_ENTRY_NEXT(pstList->pstHead, pstNode, stNode);
    }

    if(NULL != pstRet)
    {
        if(NULL != ppcData)
        {
            *ppcData = pstRet->data;
        }
        if(NULL != ppcData)
        {
            *piSize = pstRet->data_size;
        }
    }

    return pstRet;
}




