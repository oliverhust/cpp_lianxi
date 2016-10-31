/*****************************************************************************
 Copyright (c) 2010, Hangzhou H3C Technologies Co., Ltd. All rights reserved.
------------------------------------------------------------------------------
                               iSNSNdbLdap.c

  Project Code: Comware V7
   Module Name: ISNS
  Date Created: 2016-10-29
        Author: liangjinchao@dian
   Description: 对LDAP数据进行操作，各种数据的查找、添加、删除、遍历操作

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

#ifndef LDAP_DEPRECATED
#define LDAP_DEPRECATED 1
#endif
//#include <openldap/ldap.h>
#include <ldap.h>


/*****************************************************************************
    Func Name: _ldap_ModStringAttr
 Date Created: 2016/10/8
       Author: liangjinchao@dian
  Description: 设置/修改/删除ldap属性中的字符串
        Input: LDAPMod **pstAttr,
               const char *pcAttrValue,
               const char *pcAttrName
               INT iModOp
       Output:
       Return: 成功/失败
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
STATIC ULONG _ldap_ModStringAttr(LDAPMod **ppstAttr,
                                 const CHAR *pcAttrValue,
                                 const CHAR *pcAttrName,
                                 INT iModOp)
{
    LDAPMod  *pstMod;
    CHAR **ppcVals;
    CHAR *pcAttrNameTmp;

    pstMod = malloc(sizeof(LDAPMod));
    if(NULL == pstMod)
    {
        return ERROR_FAILED;
    }

    ppcVals = malloc(2 * sizeof(char *));
    if(NULL == ppcVals)
    {
        free(pstMod);
        return ERROR_FAILED;
    }

    ppcVals[0] = strdup(pcAttrValue);   /* 隐式申请了内存 */
    if(NULL == ppcVals[0])
    {
        free(pstMod);
        free(ppcVals);
        return ERROR_FAILED;
    }
    ppcVals[1] = NULL;

    pcAttrNameTmp = strdup(pcAttrName);
    if(NULL == pcAttrNameTmp)
    {
        free(pstMod);
        free(ppcVals[0]);
        free(ppcVals);
        return ERROR_FAILED;
    }

    memset(pstMod, 0, sizeof(LDAPMod));
    pstMod->mod_op = iModOp;
    pstMod->mod_values = ppcVals;
    pstMod->mod_type = pcAttrNameTmp;

    *ppstAttr = pstMod;

    return NDB_SUCCESS;
}

/*****************************************************************************
    Func Name: _ldap_FreeModStrings
 Date Created: 2016/10/8
       Author: liangjinchao@dian
  Description: string指针数组的内存释放
        Input: char ***pppStrings
       Output:
       Return: 无
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
STATIC VOID _ldap_FreeModStrings(CHAR ***pppcStrings)
{
    CHAR **ppStrings = *pppcStrings;
    INT i;

    if(NULL == ppStrings)
    {
        return;
    }

    for(i = 0; NULL != ppStrings[i]; i++)
    {
        free(ppStrings[i]);
        ppStrings[i] = NULL;
    }

    free(ppStrings);
    *pppcStrings = NULL;
}

/*****************************************************************************
    Func Name: _ldap_FreeModBvalues
 Date Created: 2016/10/8
       Author: liangjinchao@dian
  Description: bValue指针数组的内存释放
        Input: struct berval ***pppstData
       Output:
       Return: 无
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
STATIC VOID _ldap_FreeModBvalues(struct berval ***pppstData)
{
    struct berval **ppstData = *pppstData;
    INT i;

    if(NULL == pppstData)
    {
        return;
    }

    for(i = 0; NULL != ppstData[i]; i++)
    {
        if(NULL != ppstData[i]->bv_val)
        {
            free(ppstData[i]->bv_val);
            ppstData[i]->bv_val = NULL;
        }

        free(ppstData[i]);
        ppstData[i] = NULL;
    }

    free(ppstData);
    *pppstData = NULL;

    return;
}

/*****************************************************************************
    Func Name: ISNS_LDAP_IsDnExist
 Date Created: 2016/10/8
       Author: liangjinchao@dian
  Description: 判断DN是否存在
        Input: LDAP *pstLd, const char *pcDN
       Output: 成功/失败
       Return:
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
BOOL_T ISNS_LDAP_IsDnExist(LDAP *pstLd, const CHAR *pcDN)
{
    BOOL_T bRet;
    LDAPMessage *pstRes;

    bRet = 1;
    if(LDAP_SUCCESS != ldap_search_s(pstLd, pcDN, LDAP_SCOPE_BASE, NULL, NULL, 0, &pstRes))
    {
        bRet = 0;
    }

    ldap_msgfree(pstRes);
    return bRet;
}

/*****************************************************************************
    Func Name: ISNS_LDAP_SetStringAttr
 Date Created: 2016/10/8
       Author: liangjinchao@dian
  Description: 设置ldap属性中的字符串
        Input: LDAPMod **pstAttr,
               const char *pcAttrValue,
               const char *pcAttrName
               INT iModOp
       Output:
       Return: 成功/失败
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
ULONG ISNS_LDAP_SetStringAttr(LDAPMod **ppstAttr,
                                 const CHAR *pcAttrValue,
                                 const CHAR *pcAttrName)
{
    return _ldap_ModStringAttr(ppstAttr, pcAttrValue, pcAttrName, LDAP_MOD_REPLACE);
}

/*****************************************************************************
    Func Name: ISNS_LDAP_AddStringAttr
 Date Created: 2016/10/8
       Author: liangjinchao@dian
  Description: 在ldap属性中添加一个字符串
        Input: LDAPMod **pstAttr,
               const char *pcAttrValue,
               const char *pcAttrName
       Output:
       Return: 成功/失败
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
ULONG ISNS_LDAP_AddStringAttr(LDAPMod **ppstAttr,
                              const CHAR *pcAttrValue,
                              const CHAR *pcAttrName)
{
    return _ldap_ModStringAttr(ppstAttr, pcAttrValue, pcAttrName, LDAP_MOD_ADD);
}

/*****************************************************************************
    Func Name: ISNS_LDAP_DelStringAttr
 Date Created: 2016/10/8
       Author: liangjinchao@dian
  Description: 删除ldap属性中的一个字符串
        Input: LDAPMod **pstAttr,
               const char *pcAttrValue,
               const char *pcAttrName
       Output:
       Return: 成功/失败
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
ULONG ISNS_LDAP_DelStringAttr(LDAPMod **ppstAttr,
                                 const CHAR *pcAttrValue,
                                 const CHAR *pcAttrName)
{
    return _ldap_ModStringAttr(ppstAttr, pcAttrValue, pcAttrName, LDAP_MOD_DELETE);
}

/*****************************************************************************
    Func Name: ISNS_LDAP_FreeAttrs
 Date Created: 2016/10/8
       Author: liangjinchao@dian
  Description: 属性值的内存释放
        Input: LDAPMod **ppstAttrs
       Output:
       Return: 无
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
VOID ISNS_LDAP_FreeAttrs(LDAPMod **ppstAttrs)
{
    LDAPMod *pstAttr;
    INT iAttrNum;

    if(NULL == ppstAttrs)
    {
        return;
    }

    for(iAttrNum = 0; NULL != ppstAttrs[iAttrNum]; iAttrNum++)
    {
        pstAttr = ppstAttrs[iAttrNum];
        if(NULL != pstAttr->mod_type)
        {
            free(pstAttr->mod_type);
            pstAttr->mod_type = NULL;
        }

        if(LDAP_MOD_BVALUES & pstAttr->mod_op)
        {
            _ldap_FreeModBvalues(&pstAttr->mod_bvalues);
        }
        else
        {
            _ldap_FreeModStrings(&pstAttr->mod_values);
        }

        free(pstAttr);
        ppstAttrs[iAttrNum] = NULL;
    }

    return;
}

/*****************************************************************************
    Func Name: ISNS_LDAP_FreeAll
 Date Created: 2016/10/8
       Author: liangjinchao@dian
  Description: 属性值的内存释放
        Input: LDAPMod **ppstAttrs
       Output:
       Return: 无
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
VOID ISNS_LDAP_FreeAll(LDAPMod **ppstAttrs)
{

}


