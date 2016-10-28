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


/*********************************************************************
     Func Name : _isns_FormatByKey
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
STATIC ULONG _isns_FormatByKey(IN const ISNS_DBKey *pstDbKey, OUT datum *pstOutKey)
{
    ULONG ulRet = ERROR_SUCCESS;
    datum stKey = { 0 };

    if(NULL == pstDbKey)
    {
        *pstOutKey = stKey;
        return ERROR_SUCCESS;
    }

    switch (pstDbKey->tag)
    {
        case DDS_ID_KEY:
        {
            stKey.dptr = (char *)&pstDbKey->val.dds_key.id;
            stKey.dsize = DDS_KEY_SIZE;
            break;
        }
        case DD_ID_KEY:
        {
            stKey.dptr = (char *)&pstDbKey->val.dd_key.id;
            stKey.dsize = DD_KEY_SIZE;
            break;
        }
        case ISCSI_ID_KEY:
        {
            stKey.dptr = (char *)pstDbKey->val.node_key.v;
            stKey.dsize = strlen(pstDbKey->val.node_key.v);
            break;
        }
        case ENTITY_ID_KEY:
        {
            stKey.dptr = (char *)pstDbKey->val.entity_key.id;
            stKey.dsize = strlen(pstDbKey->val.entity_key.id);
            break;
        }
        case PORTAL_ID_KEY:
        {
            stKey.dptr = (char *)&pstDbKey->val.portal_key;
            stKey.dsize = sizeof(SOIP_Portal_Key);
            break;
        }
        case PORTAL_GROUP_ID_KEY:
        {
            stKey.dptr = (char *)&pstDbKey->val.portal_group_key;
            stKey.dsize = sizeof(SOIP_Portal_Group_Key);
            break;
        }
        /*  ###################  Index相关数据   ##################   */
        case ENTITY_IDX_KEY:
        case ISCSI_IDX_KEY:
        case PORTAL_IDX_KEY:
        {
            stKey.dptr = (char *)&pstDbKey->val.idx;
            stKey.dsize = sizeof(SOIP_IDX_Key);
            break;
        }

        default:
        {
            ulRet = ERROR_FAILED;
            break;
        }
    }

    *pstOutKey = stKey;

    return ulRet;

}

/*********************************************************************
     Func Name : _isns_FormatByKeyValue
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
STATIC ULONG _isns_FormatByKeyValue(IN const ISNS_DBKey *pstDbKey, IN const SOIP_DB_Entry *pstEntry,
                                    OUT datum *pstOutKey,  OUT datum *pstOutValue)
{
    ULONG ulRet = ERROR_SUCCESS;
    datum stKey = { 0 }, stValue = { 0 };

    switch (pstDbKey->tag)
    {
        case DDS_ID_KEY:     /*  SOIP_Dds  */
        {
            stKey.dptr = (char *)&pstDbKey->val.dds_key.id;
            stKey.dsize = DDS_KEY_SIZE;
            stValue.dptr = (char *)&pstEntry->data.dds;
            stValue.dsize = sizeof(SOIP_Dds);
            break;
        }
        case DD_ID_KEY:      /*  SOIP_Dd  */
        {
            stKey.dptr = (char *)&pstDbKey->val.dd_key.id;
            stKey.dsize = DD_KEY_SIZE;
            stValue.dptr = (char *)&pstEntry->data.dd;
            stValue.dsize = sizeof(SOIP_Dd);
            break;
        }
        case ISCSI_ID_KEY:   /*  SOIP_Iscsi  */
        {
            stKey.dptr = (char *)pstDbKey->val.node_key.v;
            stKey.dsize = strlen(pstDbKey->val.node_key.v);
            stValue.dptr = (char *)&pstEntry->data.scsi_node;
            stValue.dsize = sizeof(SOIP_Iscsi);
            break;
        }
        case ENTITY_ID_KEY:  /*  SOIP_Entity  */
        {
            stKey.dptr = (char *)pstDbKey->val.entity_key.id;
            stKey.dsize = strlen(pstDbKey->val.entity_key.id);
            stValue.dptr = (char *)&pstEntry->data.entity;
            stValue.dsize = sizeof(SOIP_Entity);
            break;
        }
        case PORTAL_ID_KEY:  /*  SOIP_Portal  */
        {
            stKey.dptr = (char *)&pstDbKey->val.portal_key;
            stKey.dsize = sizeof(SOIP_Portal_Key);
            stValue.dptr = (char *)&pstEntry->data.portal;
            stValue.dsize = sizeof(SOIP_Portal);
            break;
        }
        case PORTAL_GROUP_ID_KEY:  /*  SOIP_Portal_Group  */
        {
            stKey.dptr = (char *)&pstDbKey->val.portal_group_key;
            stKey.dsize = sizeof(SOIP_Portal_Group_Key);
            stValue.dptr = (char *)&pstEntry->data.portal_group;
            stValue.dsize = sizeof(SOIP_Portal_Group);
            break;
        }
        /*  ###################  Index相关数据   ##################   */
        case ENTITY_IDX_KEY:      /*  SOIP_Entity_Id  */
        {
            stKey.dptr = (char *)&pstDbKey->val.idx;
            stKey.dsize = sizeof(SOIP_IDX_Key);
            stValue.dptr = (char *)&pstEntry->data.entity_idx;
            stValue.dsize = sizeof(SOIP_Entity_Id);
            break;
        }
        case ISCSI_IDX_KEY:       /*  SOIP_ISCSI_Node_Id  */
        {
            stKey.dptr = (char *)&pstDbKey->val.idx;
            stKey.dsize = sizeof(SOIP_IDX_Key);
            stValue.dptr = (char *)&pstEntry->data.iscsi_idx;
            stValue.dsize = sizeof(SOIP_ISCSI_Node_Id);
            break;
        }
        case PORTAL_IDX_KEY:     /*   SOIP_DB_Portal   */
        {
            stKey.dptr = (char *)&pstDbKey->val.idx;
            stKey.dsize = sizeof(SOIP_IDX_Key);
            stValue.dptr = (char *)&pstEntry->data.portal_idx;
            stValue.dsize = sizeof(SOIP_DB_Portal);
            break;
        }

        default:
        {
            ulRet = ERROR_FAILED;
            break;
        }
    }

    *pstOutKey = stKey;
    *pstOutValue = stValue;

    return ulRet;

}

/*********************************************************************
     Func Name : _isns_Key2DbKey
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
STATIC ULONG _isns_Key2DbKey(IN UINT uiType, IN datum stKey, OUT ISNS_DBKey *pstDbKey)
{
    ULONG ulRet = ERROR_SUCCESS;

    memset(pstDbKey, 0, sizeof(ISNS_DBKey));
    pstDbKey->tag = uiType;

    /* 如果是字符串则在末尾加\0 */
    switch (uiType)
    {
        case DDS_ID_KEY:
        {
            memcpy(&pstDbKey->val.dds_key.id, stKey.dptr, DDS_KEY_SIZE);
            pstDbKey->len = DDS_KEY_SIZE;
            break;
        }
        case DD_ID_KEY:
        {
            memcpy(&pstDbKey->val.dd_key.id, stKey.dptr, DD_KEY_SIZE);
            pstDbKey->len = DD_KEY_SIZE;
            break;
        }
        case ISCSI_ID_KEY:
        {
            __ISNS_COPY(pstDbKey->val.node_key.v, NODE_SYM_NAME_SIZE-1, stKey.dptr, stKey.dsize);
            pstDbKey->len = PAD4(stKey.dsize);
            break;
        }
        case ENTITY_ID_KEY:
        {
            __ISNS_COPY(pstDbKey->val.entity_key.id, ENTITY_ID_SIZE-1, stKey.dptr, stKey.dsize);
            pstDbKey->len = PAD4(stKey.dsize);
            break;
        }
        case PORTAL_ID_KEY:
        {
            memcpy(&pstDbKey->val.portal_key, stKey.dptr, sizeof(SOIP_Portal_Key));
            pstDbKey->len = sizeof(SOIP_Portal_Key);
            break;
        }
        case PORTAL_GROUP_ID_KEY:
        {
            memcpy(&pstDbKey->val.portal_group_key, stKey.dptr, sizeof(SOIP_Portal_Group_Key));
            pstDbKey->len = sizeof(SOIP_Portal_Group_Key);
            break;
        }
        /*  ###################  Index相关数据   ##################   */
        case ENTITY_IDX_KEY:
        case ISCSI_IDX_KEY:
        case PORTAL_IDX_KEY:
        {
            memcpy(&pstDbKey->val.idx, stKey.dptr, sizeof(SOIP_IDX_Key));
            pstDbKey->len = sizeof(SOIP_IDX_Key);
            break;
        }

        default:
        {
            ulRet = ERROR_FAILED;
            break;
        }
    }

    return ulRet;

}

/*********************************************************************
     Func Name : _isns_Value2Entry
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
STATIC ULONG _isns_Value2Entry(IN UINT uiType, IN datum stValue, OUT SOIP_DB_Entry *pstEntry)
{
    ULONG ulRet = ERROR_SUCCESS;
    CHAR *pcData = stValue.dptr;

    memset(pstEntry, 0, sizeof(SOIP_DB_Entry));
    pstEntry->data_type = uiType;
    if(NULL == pcData)
    {
        return ERROR_FAILED;
    }

    switch (uiType)
    {
        case DDS_ID_KEY:     /*  SOIP_Dds  */
        {
            memcpy(&pstEntry->data.dds, pcData, sizeof(SOIP_Dds));
            break;
        }
        case DD_ID_KEY:      /*  SOIP_Dd  */
        {
            memcpy(&pstEntry->data.dd, pcData, sizeof(SOIP_Dd));
            break;
        }
        case ISCSI_ID_KEY:   /*  SOIP_Iscsi  */
        {
            memcpy(&pstEntry->data.scsi_node, pcData, sizeof(SOIP_Iscsi));
            break;
        }
        case ENTITY_ID_KEY:  /*  SOIP_Entity  */
        {
            memcpy(&pstEntry->data.entity, pcData, sizeof(SOIP_Entity));
            break;
        }
        case PORTAL_ID_KEY:  /*  SOIP_Portal  */
        {
            memcpy(&pstEntry->data.portal, pcData, sizeof(SOIP_Portal));
            break;
        }
        case PORTAL_GROUP_ID_KEY:  /*  SOIP_Portal_Group  */
        {
            memcpy(&pstEntry->data.portal_group, pcData, sizeof(SOIP_Portal_Group));
            break;
        }
        /*  ###################  Index相关数据   ##################   */
        case ENTITY_IDX_KEY:      /*  SOIP_Entity_Id  */
        {
            memcpy(&pstEntry->data.entity_idx, pcData, sizeof(SOIP_Entity_Id));
            break;
        }
        case ISCSI_IDX_KEY:       /*  SOIP_ISCSI_Node_Id  */
        {
            memcpy(&pstEntry->data.iscsi_idx, pcData, sizeof(SOIP_ISCSI_Node_Id));
            break;
        }
        case PORTAL_IDX_KEY:     /*   SOIP_DB_Portal   */
        {
            memcpy(&pstEntry->data.portal_idx, pcData, sizeof(SOIP_DB_Portal));
            break;
        }

        default:
        {
            ulRet = ERROR_FAILED;
            break;
        }
    }

    return ulRet;

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

    if(ERROR_SUCCESS != ISNS_MEMDATA_Add(pstDbKey->tag, stKey, stValue))
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

    (VOID)ISNS_MEMDATA_Del(pstDbKey->tag, stKey);

    return ISNS_NO_ERR;
}

/*********************************************************************
     Func Name : ISNS_MEM_Read
  Date Created : 2016/10/26
        Author : liangjinchao@dian
   Description : 删除内存中的数据
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

    stValue = ISNS_MEMDATA_Get(pstDbKey->tag, stKey);
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





