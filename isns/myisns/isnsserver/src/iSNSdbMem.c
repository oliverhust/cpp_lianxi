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


/*********************************************************************
     Func Name : _isns_FormatByKey
  Date Created : 2016/10/26
        Author : liangjinchao@dian
   Description : DB KEY 转 KEY
         Input : 无
        Output : 无
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
         Input : 无
        Output : 无
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
         Input : 无
        Output : 无
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
         Input : 无
        Output : 无
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
         Input : UINT uiMaxTypeCount
        Output : 无
        Return : 成功/失败
       Caution : 无
----------------------------------------------------------------------
 Modification History
    DATE        NAME             DESCRIPTION
----------------------------------------------------------------------

*********************************************************************/
ULONG ISNS_MEM_Init(UINT uiMaxTypeCount)
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
         Input : 无
        Output : 无
        Return : 成功/失败
       Caution : 无
----------------------------------------------------------------------
 Modification History
    DATE        NAME             DESCRIPTION
----------------------------------------------------------------------

*********************************************************************/
INT ISNS_MEM_Write(IN const ISNS_DBKey *pstDbKey, IN SOIP_DB_Entry *pstEntry)
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

    return ISNS_NO_ERR;
}

/*********************************************************************
     Func Name : ISNS_MEM_Delete
  Date Created : 2016/10/26
        Author : liangjinchao@dian
   Description : 删除内存中的数据
         Input : 无
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

    return ISNS_NO_ERR;
}

/*********************************************************************
     Func Name : ISNS_MEM_Read
  Date Created : 2016/10/26
        Author : liangjinchao@dian
   Description : 读取内存中的数据
         Input : 无
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

    return ISNS_NO_ERR;
}

/*********************************************************************
     Func Name : ISNS_MEM_NextKey
  Date Created : 2016/10/26
        Author : liangjinchao@dian
   Description : 获取下一个KEY
         Input : 无
        Output : 无
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
         Input : 无
        Output : 无
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





