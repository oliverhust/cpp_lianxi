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

#include "iSNSNdb.h"
#include "iSNSMemData.h"

/* �ڲ�ʵ����ؽṹ�� */
typedef struct tagISNS_MemData
{
    datum stKey;
    datum stValue;
}ISNS_MEMD_DATA_S;

typedef struct tagISNS_MemNode
{
    DTQ_NODE_S stNode;
    ISNS_MEMD_DATA_S stData;
    ISNS_MEMD_HEAD_S *pstSubList; /* ����ͷ���飬NULL��ʾ�� */
}ISNS_MEMD_NODE_S;

STATIC DTQ_HEAD_S *g_pstTable = NULL;
STATIC UINT g_uiMaxTypeCount = 0;



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
STATIC INT _isns_MemKeyCmp(IN datum_s stKey1, IN datum_s stKey2)
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
     Func Name : _isns_Datum2s
  Date Created : 2016/10/25
        Author : liangjinchao@dian
   Description : ����ת��
         Input : IN datum stDatum
        Output : ��
        Return :
       Caution : ��
----------------------------------------------------------------------
 Modification History
    DATE        NAME             DESCRIPTION
----------------------------------------------------------------------

*********************************************************************/
STATIC INLINE datum_s _isns_Datum2s(IN datum stDatum)
{
    datum_s stRet;

    stRet.dptr = stDatum.dptr;
    stRet.dsize = stDatum.dsize;

    return stRet;
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
STATIC ULONG _isns_MemCoverNode(IN datum_s stInValue, OUT datum *pstOutValue)
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
STATIC ISNS_MEMD_NODE_S *_isns_MemNewNode(IN datum_s stKey, IN datum_s stValue)
{
    ISNS_MEMD_NODE_S *pstNode;
    CHAR *pcNewKey, *pcNewValue;

    pstNode = (ISNS_MEMD_NODE_S *)malloc(sizeof(ISNS_MEMD_NODE_S));
    if(NULL == pstNode)
    {
        return pstNode;
    }
    memset(pstNode, 0, sizeof(ISNS_MEMD_NODE_S));

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
     Func Name : ISNS_MEMDATA_NewHead
  Date Created : 2016/11/14
        Author : liangjinchao@dian
   Description : ����N���������ڴ�����
         Input : IN UINT uiCount,�������ٸ�����
                 IN const UINT *puiEachSubListCount:
                  UINT���飬��СuiCount����Ӧÿ���������������,null��ȫ0
        Output : ��
        Return : ����ͷ����
       Caution : ��
----------------------------------------------------------------------
 Modification History
    DATE        NAME             DESCRIPTION
----------------------------------------------------------------------

*********************************************************************/
ISNS_MEMD_HEAD_S *ISNS_MEMDATA_NewHead(IN UINT uiCount, IN const UINT *puiEachSubListCount)
{
    ISNS_MEMD_HEAD_S *pstHead;
    UINT uiI;

    pstHead = (ISNS_MEMD_HEAD_S *)malloc(uiCount * sizeof(ISNS_MEMD_HEAD_S));
    if(NULL == pstHead)
    {
        return NULL;
    }

    memset(pstHead, 0, uiCount * sizeof(ISNS_MEMD_HEAD_S));
    for(uiI = 0; uiI < uiCount; uiI++)
    {
        DTQ_Init(&pstHead[uiI].stHead);
    }

    if(NULL != puiEachSubListCount)
    {
        for(uiI = 0; uiI < uiCount; uiI++)
        {
            pstHead[uiI].uiSubListCount = puiEachSubListCount[uiI];
        }
    }

    return pstHead;
}

/*********************************************************************
     Func Name : ISNS_MEMDATA_FreeHead
  Date Created : 2016/11/14
        Author : liangjinchao@dian
   Description : �ͷ�(������)�ڴ�����ͷ
         Input : IN ISNS_MEMD_HEAD_S *pstHead
        Output : ��
        Return : �ɹ�/ʧ��
       Caution : ��
----------------------------------------------------------------------
 Modification History
    DATE        NAME             DESCRIPTION
----------------------------------------------------------------------

*********************************************************************/
VOID ISNS_MEMDATA_FreeHead(IN ISNS_MEMD_HEAD_S *pstMHead)
{
    free(pstMHead);
}

/*********************************************************************
     Func Name : ISNS_MEMDATA_Write
  Date Created : 2016/10/25
        Author : liangjinchao@dian
   Description : ĳ�����������һ�����ݣ���������򸲸�
         Input : IN ISNS_MEMD_HEAD_S *pstMHead,
                 IN datum stKey, IN datum stValue
        Output : ��
        Return : �ɹ�/ʧ��
       Caution : ��������������κβ���
----------------------------------------------------------------------
 Modification History
    DATE        NAME             DESCRIPTION
----------------------------------------------------------------------

*********************************************************************/
ULONG ISNS_MEMDATA_Write(IN ISNS_MEMD_HEAD_S *pstMHead, IN datum_s stKey, IN datum_s stValue)
{
    DTQ_HEAD_S *pstHead;
    ISNS_MEMD_NODE_S *pstNode = NULL, *pstNewNode;
    INT iCmp = -1;
    ULONG ulRet = ERROR_SUCCESS;

    if(NULL == pstMHead)
    {
        return ERROR_FAILED;
    }
    pstHead = &pstMHead->stHead;

    DTQ_FOREACH_ENTRY(pstHead, pstNode, stNode)
    {
        iCmp = _isns_MemKeyCmp(_isns_Datum2s(pstNode->stData.stKey), stKey);
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
datum_s ISNS_MEMDATA_Read(IN ISNS_MEMD_HEAD_S *pstMHead, IN datum_s stKey)
{
    DTQ_HEAD_S *pstHead;
    ISNS_MEMD_NODE_S *pstNode = NULL;
    datum_s stValue = { 0 };

    if(NULL == pstMHead)
    {
        return stValue;
    }
    pstHead = &pstMHead->stHead;

    DTQ_FOREACH_ENTRY(pstHead, pstNode, stNode)
    {
        if(pstNode->stData.stKey.dsize == stKey.dsize &&
           0 == memcmp(pstNode->stData.stKey.dptr, stKey.dptr, stKey.dsize))
        {
            stValue = _isns_Datum2s(pstNode->stData.stValue);
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
       Caution : ��������������κβ������ⲿ������ɾ��
----------------------------------------------------------------------
 Modification History
    DATE        NAME             DESCRIPTION
----------------------------------------------------------------------

*********************************************************************/
ULONG ISNS_MEMDATA_Delete(IN ISNS_MEMD_HEAD_S *pstMHead, IN datum_s stKey)
{
    DTQ_HEAD_S *pstHead;
    ISNS_MEMD_NODE_S *pstNode = NULL;
    ULONG ulRet = ERROR_NOT_FOUND;

    if(NULL == pstMHead)
    {
        return ERROR_FAILED;
    }
    pstHead = &pstMHead->stHead;

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
datum_s ISNS_MEMDATA_GetNext(IN ISNS_MEMD_HEAD_S *pstMHead, IN datum_s stKey, OUT datum_s *pstNextValue)
{
    DTQ_HEAD_S *pstHead;
    ISNS_MEMD_NODE_S *pstNode = NULL;
    datum_s stNextKey = { 0 };

    if(NULL == pstMHead)
    {
        return stNextKey;
    }
    pstHead = &pstMHead->stHead;

    if(NULL != stKey.dptr)
    {
        DTQ_FOREACH_ENTRY(pstHead, pstNode, stNode)
        {
            if(_isns_MemKeyCmp(_isns_Datum2s(pstNode->stData.stKey), stKey) > 0)
            {
                break;
            }
        }
    }
    else
    {
        pstNode = DTQ_ENTRY_FIRST(pstHead, ISNS_MEMD_NODE_S, stNode);
    }

    if(NULL != pstNode)
    {
        stNextKey = _isns_Datum2s(pstNode->stData.stKey);
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
ULONG ISNS_MEMDATA_Iter(IN ISNS_MEMD_HEAD_S *pstMHead, INOUT VOID **ppIter,
                        OUT datum_s *pstNextKey, OUT datum_s *pstNextValue)
{
    DTQ_HEAD_S *pstHead;
    ISNS_MEMD_NODE_S *pstIter, *pstNode = NULL;

    if(NULL == pstMHead || NULL == ppIter)
    {
        return ERROR_FAILED;
    }
    pstHead = &pstMHead->stHead;

    /* ������ָ��ָ�������Ϊ����ӵ�һ����ʼȡ������ȥȡһ�� */
    if(NULL == *ppIter)
    {
        pstNode = DTQ_ENTRY_FIRST(pstHead, ISNS_MEMD_NODE_S, stNode);
    }
    else
    {
        pstIter = (ISNS_MEMD_NODE_S *)(*ppIter);
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

/*********************************************************************
     Func Name : ISNS_MEMDATA_Write
  Date Created : 2016/10/25
        Author : liangjinchao@dian
   Description : ĳ�����������һ�����ݣ���������򸲸�
         Input : IN ISNS_MEMD_HEAD_S *pstMHead,
                 IN datum stKey, IN datum stValue
        Output : ��
        Return : ����������ͷ
       Caution :
----------------------------------------------------------------------
 Modification History
    DATE        NAME             DESCRIPTION
----------------------------------------------------------------------

*********************************************************************/
ISNS_MEMD_HEAD_S *ISNS_MEMDATA_GetSubList(IN ISNS_MEMD_HEAD_S *pstMHead,
                                          IN datum_s stNodeKey,
                                          IN UINT uiSubListNo)
{
    ISNS_MEMD_NODE_S *pstNode = NULL;

    if(NULL == pstMHead || uiSubListNo >= pstMHead->uiSubListCount)
    {
        return NULL;
    }

    DTQ_FOREACH_ENTRY(&pstMHead->stHead, pstNode, stNode)
    {
        if(pstNode->stData.stKey.dsize == stNodeKey.dsize &&
           0 == memcmp(pstNode->stData.stKey.dptr, stNodeKey.dptr, stNodeKey.dsize))
        {
            break;
        }
    }

    if(NULL == pstNode)
    {
        return NULL;
    }

    return &pstNode->pstSubList[uiSubListNo];
}


ULONG DD_Add_Member(UINT uiDDId, SOIP_Dd_Member stMember)
{
    datum_s stDDKey, stMemberKey, stMemberValue = { 0 };
    ISNS_MEMD_HEAD_S *pstMemList;

    stDDKey = _isns_KeyFmt(DD_ID_KEY, (CHAR *)&uiDDId);
    pstMemList = ISNS_MEMDATA_GetSubList(g_apstHeads[DD_ID_KEY], stDDKey, 0);
    if(NULL == pstMemList)
    {
        return ERROR_FAILED;
    }

    stMemberKey = _isns_KeySubFmt(DD_MEMBER_LIST, &stMember);
    if(0 != ISNS_MEMDATA_Write(pstMemList, stMemberKey, stMemberValue))
    {
        return -1;
    }


}

