/*****************************************************************************
 Copyright (c) 2010, Hangzhou H3C Technologies Co., Ltd. All rights reserved.
------------------------------------------------------------------------------
                               iSNSNdbMem.c

  Project Code: Comware V7
   Module Name: ISNS
  Date Created: 2016-10-25
        Author: liangjinchao@dian
   Description: 对基础数据结构进行操作，各种数据的查找、添加、删除操作

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


typedef enum
{
    ISNS_MEM_INVALID = 0,
    ISNS_MEM_BIN,      /* 二进制类型(数字/结构体) */
    ISNS_MEM_STR,      /* 字符串类型(uiSize为最大空间) */
    ISNS_MEM_TYPE_MAX,
}ISNS_MEM_TYPE_E;

typedef struct tagIsnsMemStruct
{
    ISNS_MEM_TYPE_E enType;
    UINT uiOffset;
    UINT uiSize;
}ISNS_MEM_STRUCT_S;

/* KEY类型，相对于ISNS_DBKey的位置，(最大)大小 */
STATIC const ISNS_MEM_STRUCT_S g_astIsnsMemKey[ISNS_DATABASE_MAX] =
{
    [ENTITY_ID_KEY] = {ISNS_MEM_STR, offsetof(ISNS_DBKey, val.entity_key.id), ENTITY_ID_SIZE},
    [PORTAL_ID_KEY] = {ISNS_MEM_BIN, offsetof(ISNS_DBKey, val.portal_key), sizeof(SOIP_Portal_Key)},
    [DDS_ID_KEY] = {ISNS_MEM_BIN, offsetof(ISNS_DBKey, val.dds_key.id), DDS_KEY_SIZE},
    [DD_ID_KEY] = {ISNS_MEM_BIN, offsetof(ISNS_DBKey, val.dd_key.id), DD_KEY_SIZE},
    [ISCSI_ID_KEY] = {ISNS_MEM_STR, offsetof(ISNS_DBKey, val.iscsi_key.v), MAX_ISCSI_NODE_ID_SIZE},
    [PORTAL_GROUP_ID_KEY] = {ISNS_MEM_BIN, offsetof(ISNS_DBKey, val.portal_group_key), sizeof(SOIP_Portal_Group_Key)},
    [ENTITY_IDX_KEY] = {ISNS_MEM_BIN, offsetof(ISNS_DBKey, val.idx), sizeof(SOIP_IDX_Key)},
    [ISCSI_IDX_KEY] = {ISNS_MEM_BIN, offsetof(ISNS_DBKey, val.idx), sizeof(SOIP_IDX_Key)},
    [PORTAL_IDX_KEY] = {ISNS_MEM_BIN, offsetof(ISNS_DBKey, val.idx), sizeof(SOIP_IDX_Key)},
};

/* VALUE类型，相对于SOIP_DB_Entry的位置，(最大)大小 */
STATIC const ISNS_MEM_STRUCT_S g_astIsnsMemValue[ISNS_DATABASE_MAX] =
{
    [ENTITY_ID_KEY] = {ISNS_MEM_BIN, offsetof(SOIP_DB_Entry, data.entity), sizeof(SOIP_Entity)},
    [PORTAL_ID_KEY] = {ISNS_MEM_BIN, offsetof(SOIP_DB_Entry, data.portal), sizeof(SOIP_Portal)},
    [DDS_ID_KEY] = {ISNS_MEM_BIN, offsetof(SOIP_DB_Entry, data.dds), sizeof(SOIP_Dds)},
    [DD_ID_KEY] = {ISNS_MEM_BIN, offsetof(SOIP_DB_Entry, data.dd), sizeof(SOIP_Dd)},
    [ISCSI_ID_KEY] = {ISNS_MEM_BIN, offsetof(SOIP_DB_Entry, data.scsi_node), sizeof(SOIP_Iscsi)},
    [PORTAL_GROUP_ID_KEY] = {ISNS_MEM_BIN, offsetof(SOIP_DB_Entry, data.portal_group), sizeof(SOIP_Portal_Group)},
    [ENTITY_IDX_KEY] = {ISNS_MEM_BIN, offsetof(SOIP_DB_Entry, data.entity_idx), sizeof(SOIP_Entity_Id)},
    [ISCSI_IDX_KEY] = {ISNS_MEM_BIN, offsetof(SOIP_DB_Entry, data.iscsi_idx), sizeof(SOIP_ISCSI_Node_Id)},
    [PORTAL_IDX_KEY] = {ISNS_MEM_BIN, offsetof(SOIP_DB_Entry, data.portal_idx), sizeof(SOIP_DB_Portal)},
};

/* 十字链的位置信息 */
STATIC const UINT g_auiIsnsListOffset[DATA_LIST_MAX] =
{
    [ISCSI_DD_LIST] = offsetof(SOIP_Iscsi, dd_id_list),
    [ENTITY_PORTAL_LIST] = offsetof(SOIP_Entity, iportal_list),
    [ENTITY_ISCSI_LIST] = offsetof(SOIP_Entity, iscsi_node_list),
    [DD_MEMBER_LIST] = offsetof(SOIP_Dd, member_list),
    [DD_DDS_LIST] = offsetof(SOIP_Dd, dds_list),
    [DDS_DD_LIST] = offsetof(SOIP_Dds, dd_list),

    [SCN_CALLBACK_LIST] = 0,
    [SCN_LIST] = 0,
};

int isns_list_debug = 0;

/*********************************************************************
     Func Name : _isns_FormatByKey
  Date Created : 2016/10/26
        Author : liangjinchao@dian
   Description : DB KEY 转 KEY
         Input : IN const ISNS_DBKey *pstDbKey
        Output : OUT datum *pstOutKey
        Return : 成功/失败
       Caution : 无
----------------------------------------------------------------------
 Modification History
    DATE        NAME             DESCRIPTION
----------------------------------------------------------------------

*********************************************************************/
STATIC ULONG _isns_FormatByKey(IN const ISNS_DBKey *pstDbKey, OUT datum *pstOutKey)
{
    const ISNS_MEM_STRUCT_S *pstMem;
    datum stKey = { 0 };

    if(NULL == pstDbKey)
    {
        *pstOutKey = stKey;
        return ERROR_SUCCESS;
    }

    pstMem = &g_astIsnsMemKey[pstDbKey->tag];
    if(pstMem->enType <= ISNS_MEM_INVALID || pstMem->enType >= ISNS_MEM_TYPE_MAX)
    {
        *pstOutKey = stKey;
        return ERROR_FAILED;
    }

    stKey.dptr = (CHAR *)pstDbKey + pstMem->uiOffset;
    if(ISNS_MEM_STR == pstMem->enType)
    {
        stKey.dsize = strlen(stKey.dptr);
    }
    else
    {
        stKey.dsize = pstMem->uiSize;
    }

    *pstOutKey = stKey;
    return ERROR_SUCCESS;

}

/*********************************************************************
     Func Name : _isns_FormatByKeyValue
  Date Created : 2016/10/26
        Author : liangjinchao@dian
   Description : DB KEY 转 KEY, ENTRY 转 VALUE
         Input : IN const ISNS_DBKey *pstDbKey, IN const SOIP_DB_Entry *pstEntry
        Output : OUT datum *pstOutKey,  OUT datum *pstOutValue
        Return : 成功/失败
       Caution : 无
----------------------------------------------------------------------
 Modification History
    DATE        NAME             DESCRIPTION
----------------------------------------------------------------------

*********************************************************************/
STATIC ULONG _isns_FormatByKeyValue(IN const ISNS_DBKey *pstDbKey, IN const SOIP_DB_Entry *pstEntry,
                                    OUT datum *pstOutKey,  OUT datum *pstOutValue)
{
    const ISNS_MEM_STRUCT_S *pstMem;
    ULONG ulRet = ERROR_SUCCESS;
    datum stValue = { 0 };

    if(ERROR_SUCCESS != _isns_FormatByKey(pstDbKey, pstOutKey))
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
   Description : KEY 转 DB KEY
         Input : IN UINT uiType, IN datum stKey
        Output : OUT ISNS_DBKey *pstDbKey
        Return : 成功/失败
       Caution : 无
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

    pstMem = &g_astIsnsMemKey[uiType];
    if(pstMem->enType <= ISNS_MEM_INVALID || pstMem->enType >= ISNS_MEM_TYPE_MAX)
    {
        return ERROR_FAILED;
    }

    pcDst = (CHAR *)pstDbKey + pstMem->uiOffset;
    if(ISNS_MEM_STR == pstMem->enType)
    {
        /* 如果是字符串则在末尾加\0 */
        __ISNS_COPY(pcDst, pstMem->uiSize-1, stKey.dptr, stKey.dsize);
        pstDbKey->len = stKey.dsize;
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
   Description : VALUE 转 ENTRY
         Input : IN UINT uiType, IN datum stValue
        Output : OUT SOIP_DB_Entry *pstEntry
        Return : 成功/失败
       Caution : 无
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
     Func Name : ISNS_MEM_Init
  Date Created : 2016/10/25
        Author : liangjinchao@dian
   Description : 初始化内存数据库
         Input : IN UINT uiMaxTypeCount
        Output : 无
        Return : 成功/失败
       Caution : 无
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
   Description : 去初始化内存数据库
         Input : 无
        Output : 无
        Return : 无
       Caution : 无
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
   Description : 保存数据到内存，如果已存在则覆盖
         Input : IN const ISNS_DBKey *pstDbKey, IN SOIP_DB_Entry *pstEntry
        Output : 无
        Return : 成功/失败
       Caution : 无
----------------------------------------------------------------------
 Modification History
    DATE        NAME             DESCRIPTION
----------------------------------------------------------------------

*********************************************************************/
INT ISNS_MEM_Write(IN const ISNS_DBKey *pstDbKey, IN const SOIP_DB_Entry *pstEntry)
{
    datum stKey, stValue;

    if(ERROR_SUCCESS != _isns_FormatByKeyValue(pstDbKey, pstEntry, &stKey, &stValue))
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
   Description : 删除内存中的数据
         Input : IN const ISNS_DBKey *pstDbKey
        Output : 无
        Return : 成功/失败
       Caution : 无
----------------------------------------------------------------------
 Modification History
    DATE        NAME             DESCRIPTION
----------------------------------------------------------------------

*********************************************************************/
INT ISNS_MEM_Delete(IN const ISNS_DBKey *pstDbKey)
{
    datum stKey;

    if(ERROR_SUCCESS != _isns_FormatByKey(pstDbKey, &stKey))
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
   Description : 读取内存中的数据
         Input : IN const ISNS_DBKey *pstDbKey, OUT SOIP_DB_Entry *pstEntry
        Output : 无
        Return : 成功/失败
       Caution : 调用者可以不填pstDbKey中的len
----------------------------------------------------------------------
 Modification History
    DATE        NAME             DESCRIPTION
----------------------------------------------------------------------

*********************************************************************/
INT ISNS_MEM_Read(IN const ISNS_DBKey *pstDbKey, OUT SOIP_DB_Entry *pstEntry)
{
    datum stKey, stValue;

    if(ERROR_SUCCESS != _isns_FormatByKey(pstDbKey, &stKey))
    {
        __LOG_ERROR ("When read -- Undefined database tag: %i", pstDbKey->tag);
        return ISNS_UNKNOWN_ERR;
    }

    stValue = ISNS_MEMDATA_Read(pstDbKey->tag, stKey);
    if(NULL == stValue.dptr)   /* 目前存的数据里没有是空VALUE的情况  */
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
   Description : 获取下一个KEY
         Input : INOUT ISNS_DBKey *pstDbKey
        Output : INOUT ISNS_DBKey *pstDbKey
        Return : 是否要继续遍历
       Caution : 为兼容旧接口而写，如果有性能要求，不推荐调此接口来作遍历
                 如果要取第一个key，则pstDbKey->len置0
----------------------------------------------------------------------
 Modification History
    DATE        NAME             DESCRIPTION
----------------------------------------------------------------------

*********************************************************************/
INT ISNS_MEM_NextKey(INOUT ISNS_DBKey *pstDbKey)
{
    datum stKey, stNextKey;
    UINT uiType = pstDbKey->tag;

    memset(&stKey, 0, sizeof(stKey));
    if(0 != pstDbKey->len)
    {
        if(ERROR_SUCCESS != _isns_FormatByKey(pstDbKey, &stKey))
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
   Description : 迭代遍历接口
         InOut : INOUT ISNS_DBKey *pstDbKey, INOUT VOID **ppIter
        Output : OUT SOIP_DB_Entry *pstEntry
        Return : 是否要继续遍历
       Caution : ppIter为一个VOID *型指针(其值初始化为0)地址
                 eg: ISNS_DBKey key = { 0 };  key.tag = xxx;
                     VOID *pIter = NULL;
                     SOIP_DB_Entry entry;
                 while(SUCCESS == ISNS_MEM_Iter(&key, &pIter, &entry)){xxx}
                 如果遍历次数为0则pIter还是NULL
                 如果只想遍历KEY不取数据则第三个参数为NULL
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
   Description : 判断列表是否初始化
         Input : IN const ISNS_LIST *pstList
        Output : 无
        Return : YES/NO
       Caution : 无
----------------------------------------------------------------------
 Modification History
    DATE        NAME             DESCRIPTION
----------------------------------------------------------------------

*********************************************************************/
BOOL_T ISNS_MEM_List_IsInit(IN const ISNS_LIST *pstList)
{
    if(pstList->list_id <= 0 || pstList->list_id >= DATA_LIST_MAX ||
       NULL == pstList->pstHead)
    {
        return BOOL_FALSE;
    }
    return BOOL_TRUE;
}

/*********************************************************************
     Func Name : ISNS_MEM_List_Init
  Date Created : 2016/10/26
        Author : liangjinchao@dian
   Description : 初始化列表
         Input : IN INT iListId, IN VOID *pRecord
        Output : 无
        Return : 成功/失败
       Caution : 无
----------------------------------------------------------------------
 Modification History
    DATE        NAME             DESCRIPTION
----------------------------------------------------------------------

*********************************************************************/
INT ISNS_MEM_List_Init(IN INT iListId, IN VOID *pRecord)
{
    ISNS_LIST *pstList = NULL;

    if(0 < iListId && iListId < DATA_LIST_MAX)
    {
        pstList = (ISNS_LIST *)((UCHAR *)pRecord + g_auiIsnsListOffset[iListId]);
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
     Func Name : ISNS_MEM_List_Free
  Date Created : 2016/10/26
        Author : liangjinchao@dian
   Description : 删除/去初始化列表
         Input : IN ISNS_LIST *pstList
        Output : 无
        Return : 成功/失败
       Caution : 无
----------------------------------------------------------------------
 Modification History
    DATE        NAME             DESCRIPTION
----------------------------------------------------------------------

*********************************************************************/
INT ISNS_MEM_List_Free(IN ISNS_LIST *pstList)
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

    free(pstList->pstHead);
    pstList->pstHead = NULL;

    return (SUCCESS);
}

/*********************************************************************
     Func Name : ISNS_MEM_List_GetParent
  Date Created : 2016/10/26
        Author : liangjinchao@dian
   Description : 获取存放列表的外部结构体指针
         Input : IN ISNS_LIST *pstList
        Output : 无
        Return : 成功/失败
       Caution : 无
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
    return (VOID *)((UCHAR *)pstList - g_auiIsnsListOffset[pstList->list_id]);
}

/*********************************************************************
     Func Name : ISNS_MEM_List_RemoveNode
  Date Created : 2016/10/26
        Author : liangjinchao@dian
   Description : 删除列表的一个节点
         Input : IN ISNS_LIST *pstList, IN ISNS_LIST_NODE *pstNode
        Output : 无
        Return : 成功/失败
       Caution : 无
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
   Description : 获取节点数据
         Input : IN ISNS_LIST_NODE *pstNode, OUT INT *piSize
        Output : 无
        Return : 成功/失败
       Caution : 无
----------------------------------------------------------------------
 Modification History
    DATE        NAME             DESCRIPTION
----------------------------------------------------------------------
*********************************************************************/
VOID *ISNS_MEM_List_GetNodeData(IN ISNS_LIST_NODE *pstNode, OUT INT *piSize)
{
    __DEBUG (isns_list_debug &1, GetNodeData);
    if(NULL != piSize)
    {
        *piSize = pstNode->data_size;
    }

    return ( pstNode->data );
}

/*********************************************************************
     Func Name : ISNS_MEM_List_FindNode
  Date Created : 2016/10/26
        Author : liangjinchao@dian
   Description : 查找列表中的某个数据的节点
         Input : IN ISNS_LIST *pstList, IN CHAR *pcData, IN INT iDataSize
        Output : 无
        Return : 节点
       Caution : 无
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
   Description : 添加一个数据到列表末尾
         Input : IN ISNS_LIST *pstList, IN CHAR *pcData, IN INT iDataSize
        Output : 无
        Return : 成功/失败
       Caution : 无
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
   Description : 判断列表是否为空
         Input : IN ISNS_LIST *pstList
        Output : 无
        Return : 成功/失败
       Caution : 无
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
   Description : 获取下一个节点及数据，不存在返回NULL
                 OUT CHAR *ppcData, OUT INT *piSize可以为NULL
         Input : IN ISNS_LIST *pstList, IN ISNS_LIST_NODE *pstNode
        Output : OUT CHAR *ppcData, OUT INT *piSize
        Return : 下一个节点
       Caution : *ppcData为const char *类型，外部不应修改
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




