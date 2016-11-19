/*****************************************************************************
 Copyright (c) 2010, Hangzhou H3C Technologies Co., Ltd. All rights reserved.
------------------------------------------------------------------------------
                               iSNSMemData.c

  Project Code: Comware V7
   Module Name: ISNS
  Date Created: 2016-10-24
        Author: liangjinchao@dian
   Description: Memory data List model

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
#include <sys/list.h>

#include "iSNStypes.h"
#include "iSNSList.h"
#include "iSNSmsg.h"
#include "iSNSdebug.h"
#include "iSNSNdb.h"
#include "iSNSMemData.h"


#define ISNS_MEMDATA_MAGIC_TEST_NUM         0xEC4785ABU

/* �ڲ�ʵ����ؽṹ�� */
typedef struct tagISNS_MemData
{
    datum stKey;
    datum stValue;
}ISNS_MEM_DATA_S;

typedef struct tagISNS_MemNode
{
    DTQ_NODE_S stNode;
    ISNS_MEM_DATA_S stData;
}ISNS_MEM_NODE_S;


STATIC UINT g_uiIsnsMemMagicTestBegin = ISNS_MEMDATA_MAGIC_TEST_NUM;
STATIC UINT g_uiMaxTypeCount = 0;
STATIC DTQ_HEAD_S *g_pstTable = NULL;
STATIC UINT g_uiIsnsMemMagicTestEnd = ~ISNS_MEMDATA_MAGIC_TEST_NUM;


/*********************************************************************
     Func Name : ISNS_MEMDATA_Init
  Date Created : 2016/10/25
        Author : liangjinchao@dian
   Description : ��ʼ�������ڴ�����ͷ
         Input : UINT uiMaxTypeCount
        Output : ��
        Return : �ɹ�/ʧ��
       Caution : ��
----------------------------------------------------------------------
 Modification History
    DATE        NAME             DESCRIPTION
----------------------------------------------------------------------

*********************************************************************/
ULONG ISNS_MEMDATA_Init(UINT uiMaxTypeCount)
{
    UINT uiI;

    g_uiIsnsMemMagicTestBegin = ISNS_MEMDATA_MAGIC_TEST_NUM;
    g_uiIsnsMemMagicTestEnd = ~ISNS_MEMDATA_MAGIC_TEST_NUM;
    g_pstTable = (DTQ_HEAD_S *)malloc(uiMaxTypeCount * sizeof(DTQ_HEAD_S));
    if(NULL == g_pstTable)
    {
        return ERROR_FAILED;
    }

    g_uiMaxTypeCount = uiMaxTypeCount;
    for(uiI = 0; uiI < uiMaxTypeCount; uiI++)
    {
        DTQ_Init(g_pstTable + uiI);
    }

    return ERROR_SUCCESS;
}

/*********************************************************************
     Func Name : _isns_MemKeyCmp
  Date Created : 2016/10/25
        Author : liangjinchao@dian
   Description : �ڴ�����ļ�ֵ�Ƚ�
         Input : IN datum stKey1
                 IN datum stKey2
        Output : ��
        Return : ����0��ʾ��
       Caution : ��
----------------------------------------------------------------------
 Modification History
    DATE        NAME             DESCRIPTION
----------------------------------------------------------------------

*********************************************************************/
STATIC INT _isns_MemKeyCmp(IN datum stKey1, IN datum stKey2)
{
    INT iCmp;

    if(stKey1.dsize < stKey2.dsize)
    {
        iCmp = memcmp(stKey1.dptr, stKey2.dptr, stKey1.dsize);
        if(0 == iCmp)
        {
            iCmp = -1;
        }
    }
    else if(stKey1.dsize > stKey2.dsize)
    {
        iCmp = memcmp(stKey1.dptr, stKey2.dptr, stKey2.dsize);
        if(0 == iCmp)
        {
            iCmp = 1;
        }
    }
    else
    {
        iCmp = memcmp(stKey1.dptr, stKey2.dptr, stKey1.dsize);
    }

    return iCmp;
}

/*********************************************************************
     Func Name : _isns_MemCoverNode
  Date Created : 2016/10/25
        Author : liangjinchao@dian
   Description : �ڴ���������ݸ���
         Input : IN datum stInValue, OUT datum *pstOutValue
        Output : ��
        Return : �ɹ�/ʧ��
       Caution : ��
----------------------------------------------------------------------
 Modification History
    DATE        NAME             DESCRIPTION
----------------------------------------------------------------------

*********************************************************************/
STATIC ULONG _isns_MemCoverNode(IN datum stInValue, OUT datum *pstOutValue)
{
    CHAR *pcNewValue;

    if(NULL != pstOutValue->dptr)
    {
        free(pstOutValue->dptr);
    }

    if(NULL != stInValue.dptr)
    {
        pcNewValue = (CHAR *)malloc(stInValue.dsize);
        if(NULL == pcNewValue)
        {
            return ERROR_FAILED;
        }
        memcpy(pcNewValue, stInValue.dptr, stInValue.dsize);
        pstOutValue->dptr = pcNewValue;
        pstOutValue->dsize = stInValue.dsize;
    }
    else
    {
        pstOutValue->dptr = NULL;
        pstOutValue->dsize = 0;
    }

    return ERROR_SUCCESS;
}

/*********************************************************************
     Func Name : _isns_MemNewNode
  Date Created : 2016/10/25
        Author : liangjinchao@dian
   Description : �ڴ��������½�һ���ڵ�
         Input : IN datum stKey
                 IN datum stValue
        Output : ��
        Return : �½��Ľڵ�
       Caution : ��
----------------------------------------------------------------------
 Modification History
    DATE        NAME             DESCRIPTION
----------------------------------------------------------------------

*********************************************************************/
STATIC ISNS_MEM_NODE_S *_isns_MemNewNode(IN datum stKey, IN datum stValue)
{
    ISNS_MEM_NODE_S *pstNode;
    CHAR *pcNewKey, *pcNewValue;

    pstNode = (ISNS_MEM_NODE_S *)malloc(sizeof(ISNS_MEM_NODE_S));
    if(NULL == pstNode)
    {
        return pstNode;
    }
    memset(pstNode, 0, sizeof(ISNS_MEM_NODE_S));

    pcNewKey = (CHAR *)malloc(stKey.dsize);
    if(NULL == pcNewKey)
    {
        free(pstNode);
        return NULL;
    }
    memcpy(pcNewKey, stKey.dptr, stKey.dsize);
    pstNode->stData.stKey.dptr = pcNewKey;
    pstNode->stData.stKey.dsize = stKey.dsize;

    if(NULL != stValue.dptr)
    {
        pcNewValue = (CHAR *)malloc(stValue.dsize);
        if(NULL == pcNewValue)
        {
            free(pcNewKey);
            free(pstNode);
            return NULL;
        }
        memcpy(pcNewValue, stValue.dptr, stValue.dsize);
        pstNode->stData.stValue.dptr = pcNewValue;
        pstNode->stData.stValue.dsize = stValue.dsize;
    }
    else
    {
        pstNode->stData.stValue.dptr = NULL;
        pstNode->stData.stValue.dsize = 0;
    }

    return pstNode;
}

/*********************************************************************
     Func Name : _isns_GetDTQHead
  Date Created : 2016/10/25
        Author : liangjinchao@dian
   Description : ��ȡ����ͷ
         Input : IN UINT uiType,
        Output : OUT DTQ_HEAD_S **ppstHead
        Return : �ɹ�/ʧ��
       Caution : ��
----------------------------------------------------------------------
 Modification History
    DATE        NAME             DESCRIPTION
----------------------------------------------------------------------

*********************************************************************/
STATIC ULONG _isns_GetDTQHead(IN UINT uiType, OUT DTQ_HEAD_S **ppstHead)
{
    *ppstHead = NULL;

    if(g_uiIsnsMemMagicTestBegin != ISNS_MEMDATA_MAGIC_TEST_NUM ||
       g_uiIsnsMemMagicTestEnd != ~ISNS_MEMDATA_MAGIC_TEST_NUM)
    {
        __LOG_ERROR("The global memory data has been destoryed.");
        return ERROR_FAILED;
    }

    if(uiType < g_uiMaxTypeCount)
    {
        *ppstHead = g_pstTable + uiType;
    }
    else
    {
        return ERROR_FAILED;
    }

    return ERROR_SUCCESS;
}

/*********************************************************************
     Func Name : ISNS_MEMDATA_Write
  Date Created : 2016/10/25
        Author : liangjinchao@dian
   Description : ĳ�����������һ�����ݣ���������򸲸�
         Input : IN UINT uiType, IN datum stKey, IN datum stValue
        Output : ��
        Return : �ɹ�/ʧ��
       Caution : ��
----------------------------------------------------------------------
 Modification History
    DATE        NAME             DESCRIPTION
----------------------------------------------------------------------

*********************************************************************/
ULONG ISNS_MEMDATA_Write(IN UINT uiType, IN datum stKey, IN datum stValue)
{
    DTQ_HEAD_S *pstHead;
    ISNS_MEM_NODE_S *pstNode = NULL, *pstNewNode;
    INT iCmp = -1;
    ULONG ulRet = ERROR_SUCCESS;

    if(ERROR_SUCCESS != _isns_GetDTQHead(uiType, &pstHead))
    {
        return ERROR_FAILED;
    }

    DTQ_FOREACH_ENTRY(pstHead, pstNode, stNode)
    {
        iCmp = _isns_MemKeyCmp(pstNode->stData.stKey, stKey);
        if(iCmp >= 0)
        {
            break;
        }
    }

    if(0 == iCmp)
    {
        ulRet = _isns_MemCoverNode(stValue, &pstNode->stData.stValue);
    }
    else if(iCmp > 0)
    {
        pstNewNode = _isns_MemNewNode(stKey, stValue);
        if(NULL == pstNewNode)
        {
            return ERROR_FAILED;
        }
        DTQ_AddBefore(&pstNode->stNode, &pstNewNode->stNode);
    }
    else
    {
        pstNewNode = _isns_MemNewNode(stKey, stValue);
        if(NULL == pstNewNode)
        {
            return ERROR_FAILED;
        }
        DTQ_AddTail(pstHead, &pstNewNode->stNode);
    }

    return ulRet;
}

/*********************************************************************
     Func Name : ISNS_MEMDATA_Read
  Date Created : 2016/10/25
        Author : liangjinchao@dian
   Description : ��ȡ�����е�ĳ������
         Input : ��
        Output : ��
        Return : �ɹ�/ʧ��
       Caution : ����ָ��ָ���ڲ�����
----------------------------------------------------------------------
 Modification History
    DATE        NAME             DESCRIPTION
----------------------------------------------------------------------

*********************************************************************/
datum ISNS_MEMDATA_Read(IN UINT uiType, IN datum stKey)
{
    DTQ_HEAD_S *pstHead;
    ISNS_MEM_NODE_S *pstNode = NULL;
    datum stValue = { 0 };

    if(ERROR_SUCCESS != _isns_GetDTQHead(uiType, &pstHead))
    {
        return stValue;
    }

    DTQ_FOREACH_ENTRY(pstHead, pstNode, stNode)
    {
        if(pstNode->stData.stKey.dsize == stKey.dsize &&
           0 == memcmp(pstNode->stData.stKey.dptr, stKey.dptr, stKey.dsize))
        {
            stValue = pstNode->stData.stValue;
            break;
        }
    }

    return stValue;
}

/*********************************************************************
     Func Name : ISNS_MEMDATA_Delete
  Date Created : 2016/10/25
        Author : liangjinchao@dian
   Description : ĳ��������ɾ��һ������
         Input : ��
        Output : ��
        Return : �ɹ�/ʧ��
       Caution : ��
----------------------------------------------------------------------
 Modification History
    DATE        NAME             DESCRIPTION
----------------------------------------------------------------------

*********************************************************************/
ULONG ISNS_MEMDATA_Delete(IN UINT uiType, IN datum stKey)
{
    DTQ_HEAD_S *pstHead;
    ISNS_MEM_NODE_S *pstNode = NULL;
    ULONG ulRet = ERROR_NOT_FOUND;

    if(ERROR_SUCCESS != _isns_GetDTQHead(uiType, &pstHead))
    {
        return ERROR_FAILED;
    }

    DTQ_FOREACH_ENTRY(pstHead, pstNode, stNode)
    {
        if(pstNode->stData.stKey.dsize == stKey.dsize &&
           0 == memcmp(pstNode->stData.stKey.dptr, stKey.dptr, stKey.dsize))
        {
            ulRet = ERROR_SUCCESS;
            break;
        }
    }

    if(ERROR_SUCCESS == ulRet)
    {
        DTQ_Del(&pstNode->stNode);
        free(pstNode->stData.stKey.dptr);
        if(NULL != pstNode->stData.stValue.dptr)
        {
            free(pstNode->stData.stValue.dptr);
        }
        free(pstNode);
    }

    return ulRet;
}

/*********************************************************************
     Func Name : ISNS_MEMDATA_GetNext
  Date Created : 2016/10/25
        Author : liangjinchao@dian
   Description : ��ȡָ���������һ������
         Input : ��
        Output : ��һ��VALUE
        Return : ��һ��KEY
       Caution : ������������⣬���Ƽ����˽ӿ���������
                 ����ָ��ָ���ڲ�����
----------------------------------------------------------------------
 Modification History
    DATE        NAME             DESCRIPTION
----------------------------------------------------------------------

*********************************************************************/
datum ISNS_MEMDATA_GetNext(IN UINT uiType, IN datum stKey, OUT datum *pstNextValue)
{
    DTQ_HEAD_S *pstHead;
    ISNS_MEM_NODE_S *pstNode = NULL;
    datum stNextKey = { 0 };

    if(ERROR_SUCCESS != _isns_GetDTQHead(uiType, &pstHead))
    {
        return stNextKey;
    }

    if(NULL != stKey.dptr)
    {
        DTQ_FOREACH_ENTRY(pstHead, pstNode, stNode)
        {
            if(_isns_MemKeyCmp(pstNode->stData.stKey, stKey) > 0)
            {
                break;
            }
        }
    }
    else
    {
        pstNode = DTQ_ENTRY_FIRST(pstHead, ISNS_MEM_NODE_S, stNode);
    }

    if(NULL != pstNode)
    {
        stNextKey = pstNode->stData.stKey;
        if(NULL != pstNextValue)
        {
            *pstNextValue = pstNode->stData.stValue;
        }
    }

    return stNextKey;
}

/*********************************************************************
     Func Name : ISNS_MEMDATA_Iter
  Date Created : 2016/10/25
        Author : liangjinchao@dian
   Description : �������ݽӿڣ����ط�0���ʾ��������
         Input : ��
        Output : ��һ��VALUE
        Return : �ɹ�/ʧ��
       Caution : ����ָ��ָ���ڲ�����
----------------------------------------------------------------------
 Modification History
    DATE        NAME             DESCRIPTION
----------------------------------------------------------------------

*********************************************************************/
ULONG ISNS_MEMDATA_Iter(IN UINT uiType, INOUT VOID **ppIter,
                        OUT datum *pstNextKey, OUT datum *pstNextValue)
{
    DTQ_HEAD_S *pstHead;
    ISNS_MEM_NODE_S *pstIter, *pstNode = NULL;

    if(ERROR_SUCCESS != _isns_GetDTQHead(uiType, &pstHead) || NULL == ppIter)
    {
        return ERROR_FAILED;
    }

    /* ������ָ��ָ�������Ϊ����ӵ�һ����ʼȡ������ȥȡһ�� */
    if(NULL == *ppIter)
    {
        pstNode = DTQ_ENTRY_FIRST(pstHead, ISNS_MEM_NODE_S, stNode);
    }
    else
    {
        pstIter = (ISNS_MEM_NODE_S *)(*ppIter);
        pstNode = DTQ_ENTRY_NEXT(pstHead, pstIter, stNode);
    }

    if(NULL != pstNode)
    {
        *ppIter = (VOID *)pstNode;
        *pstNextKey = pstNode->stData.stKey;
        if(NULL != pstNextValue)
        {
            *pstNextValue = pstNode->stData.stValue;
        }
    }
    else
    {
        /* �Ѿ������һ�� */
        return ERROR_FAILED;
    }

    return ERROR_SUCCESS;
}


