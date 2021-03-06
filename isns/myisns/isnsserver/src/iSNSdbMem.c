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

/* 静态数据的个数 */
#define ISNS_MEM_LIST_GLOBAL_NUM            4

/* 表示数据不存在的非法指针 */
#define ISNS_INVALID_PTR_NOTEXIST           ((VOID *)1)

/* 版本号的初始值(任意但<步进)和步进(2^n) */
#define ISNS_MEM_LIST_VER_INIT               0x0756CCEE
#define ISNS_MEM_LIST_VER_STEP              0x100000000

typedef enum
{
    ISNS_MEM_INVALID = 0,
    ISNS_MEM_BIN,      /* 二进制/数字类型 */
    ISNS_MEM_NUM,      /* 数字类型(UINT32) */
    ISNS_MEM_STR,      /* 字符串类型(uiSize为最大空间) */
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

/* KEY类型，相对于ISNS_DBKey的位置，(最大)大小 */
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
    [ISCSI_IDX_KEY] = {ISNS_MEM_BIN, offsetof(SOIP_DB_Entry, data.iscsi_idx), sizeof(SOIP_ISCSI_Index)},
    [PORTAL_IDX_KEY] = {ISNS_MEM_BIN, offsetof(SOIP_DB_Entry, data.portal_idx), sizeof(SOIP_Portal_Index)},
};

/* 十字链的位置信息 */
STATIC const ISNS_MEM_LIST_INFO_S g_auiIsnsListOffset[DATA_LIST_MAX] =
{
    [ISCSI_DD_LIST] = {offsetof(SOIP_Iscsi, dd_id_list), ISCSI_ID_KEY},
    [ENTITY_PORTAL_LIST] = {offsetof(SOIP_Entity, iportal_list), ENTITY_ID_KEY},
    [ENTITY_ISCSI_LIST] = {offsetof(SOIP_Entity, iscsi_node_list), ENTITY_ID_KEY},
    [DD_MEMBER_LIST] = {offsetof(SOIP_Dd, member_list), DD_ID_KEY},
    [DD_PORTAL_LIST] = {offsetof(SOIP_Dd, portal_list), DD_ID_KEY},
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
        /* 如果是字符串则在末尾加\0 */
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
     Func Name : _isns_FreeMemListHead
  Date Created : 2016/10/26
        Author : liangjinchao@dian
   Description : 释放十字链表头
         Input : IN const ISNS_DBKey *pstDbKey
        Output :
        Return : 成功/失败
       Caution : 无
----------------------------------------------------------------------
 Modification History
    DATE        NAME             DESCRIPTION
----------------------------------------------------------------------

*********************************************************************/
STATIC VOID _isns_FreeMemListHead(IN const ISNS_DBKey *pstDbKey)
{
    SOIP_DB_Entry stEntry;
    ISNS_LIST *pstListPPtr;
    ISNS_LIST_S *pstList;
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
        pstListPPtr = (ISNS_LIST *)(pcParent + g_auiIsnsListOffset[uiI].uiOffset);
        pstList = pstListPPtr->pstList;
        if(NULL != pstList)
        {
            memset(pstList, 0, sizeof(ISNS_LIST_S));
            free(pstList);
            memset(pstListPPtr, 0, sizeof(ISNS_LIST));
        }
    }

    return ;
}

/*********************************************************************
     Func Name : _isns_FreeNodeData
  Date Created : 2016/10/26
        Author : liangjinchao@dian
   Description : 释放十字链表节点
         Input : IN DTQ_NODE_S *pstDTQNode
        Output :
        Return : 成功/失败
       Caution : 入参为DTQ_NODE_S *类型
----------------------------------------------------------------------
 Modification History
    DATE        NAME             DESCRIPTION
----------------------------------------------------------------------

*********************************************************************/
STATIC VOID _isns_FreeNodeData(IN VOID *pDTQNode)
{
    ISNS_LIST_NODE *pstNode = DTQ_ENTRY((DTQ_NODE_S *)pDTQNode, ISNS_LIST_NODE, stNode);

    free(pstNode->data);
    memset(pstNode, 0, sizeof(ISNS_LIST_NODE));  /* 破坏版本号等数据 */
    free(pstNode);

    return ;
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
       Caution : pstDbKey只填TAG和VAL
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
        ISNS_ERROR ("When write -- Undefined database tag: %i", pstDbKey->tag);
        return ISNS_UNKNOWN_ERR;
    }

    if(ERROR_SUCCESS != ISNS_MEMDATA_Write(pstDbKey->tag, stKey, stValue))
    {
        ISNS_ERROR ("When write -- Write Memory data failed, tag = %i", pstDbKey->tag);
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
    UINT32 uiTmp;

    _isns_FreeMemListHead(pstDbKey);

    if(ERROR_SUCCESS != _isns_FormatByKey(pstDbKey, &stKey, &uiTmp))
    {
        ISNS_ERROR ("When delete -- Undefined database tag: %i", pstDbKey->tag);
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
    UINT32 uiTmp;

    if(ERROR_SUCCESS != _isns_FormatByKey(pstDbKey, &stKey, &uiTmp))
    {
        ISNS_ERROR ("When read -- Undefined database tag: %i", pstDbKey->tag);
        return ISNS_UNKNOWN_ERR;
    }

    stValue = ISNS_MEMDATA_Read(pstDbKey->tag, stKey);
    if(NULL == stValue.dptr)   /* 目前存的数据里没有是空VALUE的情况  */
    {
        return ISNS_NO_SUCH_ENTRY_ERR;
    }

    if(ERROR_SUCCESS != _isns_Value2Entry(pstDbKey->tag, stValue, pstEntry))
    {
        ISNS_ERROR ("When read -- Can not convert value, tag = %i", pstDbKey->tag);
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
    UINT32 uiTmp;

    memset(&stKey, 0, sizeof(stKey));
    if(0 != pstDbKey->len)
    {
        if(ERROR_SUCCESS != _isns_FormatByKey(pstDbKey, &stKey, &uiTmp))
        {
            ISNS_ERROR ("When get next -- Undefined database tag: %i", uiType);
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
        ISNS_ERROR ("When get next -- Can not convert value, tag = %i", uiType);
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
        ISNS_ERROR ("When get next -- Can not convert value, tag = %i", uiType);
        return ISNS_UNKNOWN_ERR;
    }

    if(NULL != pstEntry)
    {
        if(ERROR_SUCCESS != _isns_Value2Entry(uiType, stValue, pstEntry))
        {
            ISNS_ERROR ("When get next -- Can not convert value, tag = %i", uiType);
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
BOOL_T ISNS_MEM_List_IsInit(IN const ISNS_LIST *pstListPPtr)
{
    ISNS_LIST_S *pstList;

    if(NULL == pstListPPtr || NULL == pstListPPtr->pstList)
    {
        ISNS_ERROR("The list is not init, ptr = %p", pstListPPtr);
        return BOOL_FALSE;
    }

    pstList = pstListPPtr->pstList;
    if(pstList->list_id <= DATA_LIST_INVALID || pstList->list_id >= DATA_LIST_MAX ||
       NULL == pstList->stHead.stHead.pstNext || NULL == pstList->stHead.stHead.pstPrev)
    {
        ISNS_ERROR("The list is not init, list type = %u", pstList->list_id);
        return BOOL_FALSE;
    }

    if(pstList->uiVersion % ISNS_MEM_LIST_VER_STEP != ISNS_MEM_LIST_VER_INIT)
    {
        ISNS_ERROR("Invalid list version %llx, list type = %u", pstList->uiVersion, pstList->list_id);
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
    ISNS_LIST *pstListPPtr = NULL;
    ISNS_LIST_S *pstList;

    if(DATA_LIST_INVALID < iListId && iListId < DATA_LIST_MAX)
    {
        pstListPPtr = (ISNS_LIST *)((UCHAR *)pRecord + g_auiIsnsListOffset[iListId].uiOffset);
    }
    else if(DATA_LIST_MAX < iListId && iListId < DATA_LIST_OLD_MAX)
    {
        return SUCCESS;
    }
    else
    {
        ISNS_ERROR("Init List: Unknown list type %d", iListId);
        return ISNS_UNKNOWN_ERR;
    }

    memset(pstListPPtr, 0, sizeof(ISNS_LIST));

    pstList = (ISNS_LIST_S *)malloc(sizeof(ISNS_LIST_S));
    if(NULL == pstList)
    {
        return ISNS_UNKNOWN_ERR;
    }
    DTQ_Init(&pstList->stHead);
    pstList->list_id = iListId;
    pstList->uiVersion = ISNS_MEM_LIST_VER_INIT;

    pstListPPtr->pstList = pstList;

    return ( SUCCESS );
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
INT ISNS_MEM_List_AddNode(IN ISNS_LIST *pstListPPtr, IN CHAR *pcData, IN INT iDataSize)
{
    ISNS_LIST_S *pstList;
    ISNS_LIST_NODE *pstNode;

    if(BOOL_FALSE == ISNS_MEM_List_IsInit(pstListPPtr))
    {
        return ISNS_UNKNOWN_ERR;
    }

    pstList = pstListPPtr->pstList;
    pstList->uiVersion += ISNS_MEM_LIST_VER_STEP;
    pstNode = (ISNS_LIST_NODE *)malloc(sizeof(ISNS_LIST_NODE));
    if(NULL == pstNode)
    {
        return ISNS_UNKNOWN_ERR;
    }

    memset(pstNode, 0, sizeof(ISNS_LIST_NODE));
    pstNode->list_id = pstList->list_id;
    pstNode->data = malloc(iDataSize + 1);
    if(NULL == pstNode->data)
    {
        free(pstNode);
        return ISNS_UNKNOWN_ERR;
    }

    memcpy(pstNode->data, pcData, iDataSize);
    ((CHAR *)pstNode->data)[iDataSize] = 0;
    pstNode->data_size = iDataSize;
    pstNode->uiVersion = pstList->uiVersion;
    DTQ_AddTail(&pstList->stHead, &pstNode->stNode);

    return ( SUCCESS );
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
INT ISNS_MEM_List_RemoveNode(IN ISNS_LIST *pstListPPtr, IN ISNS_LIST_NODE *pstNode)
{
    ISNS_LIST_S *pstList;

    if(BOOL_FALSE == ISNS_MEM_List_IsInit(pstListPPtr))
    {
        return ISNS_UNKNOWN_ERR;
    }

    pstList = pstListPPtr->pstList;
    pstList->uiVersion += ISNS_MEM_LIST_VER_STEP;

    DTQ_Del(&pstNode->stNode);
    _isns_FreeNodeData((VOID *)&pstNode->stNode);

    return ( SUCCESS );
}

/*********************************************************************
     Func Name : ISNS_MEM_List_Delete
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
INT ISNS_MEM_List_Delete(IN ISNS_LIST *pstListPPtr)
{
    ISNS_LIST_S *pstList;

    if(BOOL_FALSE == ISNS_MEM_List_IsInit(pstListPPtr))
    {
        return ISNS_UNKNOWN_ERR;
    }

    pstList = pstListPPtr->pstList;
    pstList->uiVersion += ISNS_MEM_LIST_VER_STEP;
    DTQ_FreeAll(&pstList->stHead, _isns_FreeNodeData);

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
VOID *ISNS_MEM_List_GetParent(IN ISNS_LIST *pstListPPtr)
{
    UINT32 uiListId;

    if(BOOL_FALSE == ISNS_MEM_List_IsInit(pstListPPtr))
    {
        return NULL;
    }

    uiListId = pstListPPtr->pstList->list_id;

    return (VOID *)((UCHAR *)pstListPPtr - g_auiIsnsListOffset[uiListId].uiOffset);
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
ISNS_LIST_NODE *ISNS_MEM_List_FindNode(IN ISNS_LIST *pstListPPtr,
                                       IN const CHAR *pcData, IN INT iDataSize)
{
    ISNS_LIST_S *pstList;
    ISNS_LIST_NODE *pstNode;

    if(BOOL_FALSE == ISNS_MEM_List_IsInit(pstListPPtr))
    {
        return NULL;
    }

    pstList = pstListPPtr->pstList;
    pstNode = NULL;

    DTQ_FOREACH_ENTRY((&pstList->stHead), pstNode, stNode)
    {
        if (pstNode->data_size == iDataSize &&
            0 == memcmp((CHAR *)pstNode->data, pcData, iDataSize))
        {
            break;
        }
    }

    return pstNode;
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
BOOL_T ISNS_MEM_List_IsEmpty(IN ISNS_LIST *pstListPPtr)
{
    ISNS_LIST_S *pstList;

    if(BOOL_TRUE != ISNS_MEM_List_IsInit(pstListPPtr))
    {
        return BOOL_TRUE;
    }

    pstList = pstListPPtr->pstList;
    if(DTQ_IsEmpty(&pstList->stHead))
    {
        return BOOL_TRUE;
    }

    return BOOL_FALSE;
}

/*********************************************************************
     Func Name : ISNS_MEM_List_GetNext
  Date Created : 2016/10/26
        Author : liangjinchao@dian
   Description : 获取下一个节点及数据，不存在返回NULL *piSize可以为NULL
         Input : IN ISNS_LIST *pstList, IN ISNS_LIST_NODE *pstNode
        Output : OUT CHAR *ppcData, OUT INT *piSize
        Return : 下一个节点
       Caution : *ppcData为const char *类型，外部不应修改,
                 外部需要先判断返回值是否为NULL再取数据
----------------------------------------------------------------------
 Modification History
    DATE        NAME             DESCRIPTION
----------------------------------------------------------------------

*********************************************************************/
ISNS_LIST_NODE *ISNS_MEM_List_GetNext(IN ISNS_LIST *pstListPPtr, IN ISNS_LIST_NODE *pstNode,
                                      OUT const CHAR **ppcData, OUT INT *piSize)
{
    ISNS_LIST_S *pstList;
    ISNS_LIST_NODE *pstRet;

    if(BOOL_FALSE == ISNS_MEM_List_IsInit(pstListPPtr))
    {
        return NULL;
    }

    pstList = pstListPPtr->pstList;
    if(NULL == pstNode)
    {
        pstRet = DTQ_ENTRY_FIRST(&pstList->stHead, ISNS_LIST_NODE, stNode);
    }
    else if(pstNode->uiVersion == pstList->uiVersion)
    {
        pstRet = DTQ_ENTRY_NEXT(&pstList->stHead, pstNode, stNode); /* 大多数情况 */
    }
    else
    {
        pstRet = ISNS_MEM_List_FindNode(pstListPPtr, (CHAR *)pstNode->data, pstNode->data_size);
        if(NULL != pstRet)
        {
            pstRet = DTQ_ENTRY_NEXT(&pstList->stHead, pstNode, stNode);
        }
    }

    if(NULL != pstRet)
    {
        pstRet->uiVersion = pstList->uiVersion;
        if(NULL != ppcData)
        {
            *ppcData = (CHAR *)pstRet->data;
        }
        if(NULL != ppcData)
        {
            *piSize = pstRet->data_size;
        }
    }

    return pstRet;
}

/*********************************************************************
     Func Name : ISNS_MEM_List_GetNext
  Date Created : 2016/10/26
        Author : liangjinchao@dian
   Description : 获取下一个节点及数据，不存在返回NULL *piSize可以为NULL
         Input : IN ISNS_LIST *pstList, IN ISNS_LIST_NODE *pstNode
        Output : OUT CHAR *ppcData, OUT INT *piSize
        Return : 下一个节点
       Caution : 如果遍历次数为零则外部的ISNS_LIST_NODE *仍为NULL
----------------------------------------------------------------------
 Modification History
    DATE        NAME             DESCRIPTION
----------------------------------------------------------------------

*********************************************************************/
ULONG ISNS_MEM_List_NextData(IN ISNS_LIST *pstListPPtr, INOUT ISNS_LIST_NODE **ppstNode,
                             INOUT VOID *pOutBuff, IN UINT uiBuffSize)
{
    ISNS_LIST_S *pstList;
    ISNS_LIST_NODE *pstNode;

    if(BOOL_FALSE == ISNS_MEM_List_IsInit(pstListPPtr) || NULL == ppstNode || NULL == pOutBuff)
    {
        return ERROR_FAILED;
    }

    pstList = pstListPPtr->pstList;
    pstNode = *ppstNode;

    if(ISNS_INVALID_PTR_NOTEXIST == (VOID *)pstNode || DTQ_IsEmpty(&pstList->stHead))
    {
        return ISNS_NO_SUCH_ENTRY_ERR;
    }

    if(NULL == pstNode)
    {
        pstNode = DTQ_ENTRY_FIRST(&pstList->stHead, ISNS_LIST_NODE, stNode);
    }
    else if(pstNode->uiVersion % ISNS_MEM_LIST_VER_STEP != ISNS_MEM_LIST_VER_INIT)
    {
        ISNS_ERROR("Invalid node version(%llx), list id = %d", pstNode->uiVersion, pstList->list_id);
        return ERROR_FAILED;                /* 下一个节点已被删除或数据遭到意外破坏，无法继续 */
    }

    if(uiBuffSize < pstNode->data_size)
    {
        ISNS_ERROR("GetNextData: actual %u < %u require bytes", uiBuffSize, pstNode->data_size);
        return ERROR_FAILED;
    }
    memcpy(pOutBuff, pstNode->data, pstNode->data_size);         /* 拷贝数据，并把多余空间置0 */
    memset((CHAR *)pOutBuff + pstNode->data_size, 0, uiBuffSize - pstNode->data_size);

    pstNode = DTQ_ENTRY_NEXT(&pstList->stHead, pstNode, stNode);   /* 取下一节点 */
    if(NULL == pstNode)
    {
        pstNode = (ISNS_LIST_NODE *)ISNS_INVALID_PTR_NOTEXIST;
    }
    else
    {
        pstNode->uiVersion = pstList->uiVersion;       /* 用来下次进入时判断该节点是否删除 */
    }

    *ppstNode = pstNode;
    return ERROR_SUCCESS;
}



