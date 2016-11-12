/*****************************************************************************
 Copyright (c) 2010, Hangzhou H3C Technologies Co., Ltd. All rights reserved.
------------------------------------------------------------------------------
                               iSNSLdapLib.c

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

#ifndef LDAP_DEPRECATED
#define LDAP_DEPRECATED 1
#endif
#include <openldap/ldap.h>
//#include <ldap.h>

#include "iSNStypes.h"
#include "iSNSdbLdap.h"
#include "iSNSLdapLib.h"

STATIC LDAP *g_pstLDAP = NULL;
STATIC ISNS_LDAP_INIT_S g_stInitData = { 0 };


/*****************************************************************************
    Func Name: _ldap_ModStringAttr
 Date Created: 2016/11/3
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
STATIC ULONG _ldap_ModStringAttr(IN const CHAR *pcAttrName,
                                 IN const CHAR *pcAttrValue,
                                 IN INT iModOp,
                                 OUT LDAPMod **ppstAttr)
{
    LDAPMod  *pstMod;
    CHAR **ppcVals;
    CHAR *pcAttrNameTmp;

    *ppstAttr = NULL;
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

    return ERROR_SUCCESS;
}

/*****************************************************************************
    Func Name: _ldap_FreeModStrings
 Date Created: 2016/11/3
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
 Date Created: 2016/11/3
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
    Func Name: _ldap_SaveInitData
 Date Created: 2016/11/3
       Author: liangjinchao@dian
  Description: LDAP初始化数据保存
        Input: IN ISNS_LDAP_INIT_S *pstInit
       Output: 无
       Return: 成功/失败
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
STATIC VOID _ldap_FreePtr(INOUT CHAR **ppcData)
{
    if(NULL != *ppcData)
    {
        free(*ppcData);
        *ppcData = NULL;
    }
}

/*****************************************************************************
    Func Name: _ldap_SaveInitData
 Date Created: 2016/11/3
       Author: liangjinchao@dian
  Description: LDAP初始化数据保存
        Input: IN ISNS_LDAP_INIT_S *pstInit
       Output: 无
       Return: 成功/失败
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
STATIC ULONG _ldap_SaveInitData(IN const ISNS_LDAP_INIT_S *pstInit)
{
    memset(&g_stInitData, 0, sizeof(g_stInitData));

    g_stInitData.pcAdminDn = strdup(pstInit->pcAdminDn);
    if(NULL == g_stInitData.pcAdminDn)
    {
        return ERROR_FAILED;
    }

    g_stInitData.pcBase = strdup(pstInit->pcBase);
    if(NULL == g_stInitData.pcBase)
    {
        _ldap_FreePtr(&g_stInitData.pcAdminDn);
        return ERROR_FAILED;
    }

    g_stInitData.pcLdapUrl = strdup(pstInit->pcLdapUrl);
    if(NULL == g_stInitData.pcLdapUrl)
    {
        _ldap_FreePtr(&g_stInitData.pcAdminDn);
        _ldap_FreePtr(&g_stInitData.pcBase);
        return ERROR_FAILED;
    }

    g_stInitData.pcPassword = strdup(pstInit->pcPassword);
    if(NULL == g_stInitData.pcPassword)
    {
        _ldap_FreePtr(&g_stInitData.pcAdminDn);
        _ldap_FreePtr(&g_stInitData.pcBase);
        _ldap_FreePtr(&g_stInitData.pcLdapUrl);
    }

    return ERROR_SUCCESS;
}

/*****************************************************************************
    Func Name: _ldap_FreeInitData
 Date Created: 2016/11/3
       Author: liangjinchao@dian
  Description: 释放初始化数据
        Input:
       Output:
       Return:
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
STATIC VOID _ldap_FreeInitData()
{
    _ldap_FreePtr(&g_stInitData.pcAdminDn);
    _ldap_FreePtr(&g_stInitData.pcBase);
    _ldap_FreePtr(&g_stInitData.pcLdapUrl);
    _ldap_FreePtr(&g_stInitData.pcPassword);
}

/*****************************************************************************
    Func Name: _ldap_CheckLdapInit
 Date Created: 2016/11/3
       Author: liangjinchao@dian
  Description: 判断是否初始化过
        Input: 无
       Output: 无
       Return: 无
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
STATIC INLINE ULONG _ldap_CheckLdapInit()
{
    if(NULL == g_pstLDAP || NULL == g_stInitData.pcBase)
    {
        return ERROR_FAILED;
    }

    return ERROR_SUCCESS;
}

/*****************************************************************************
    Func Name: _ldap_IsDnExist
 Date Created: 2016/11/3
       Author: liangjinchao@dian
  Description: 判断DN是否存在
        Input: IN LDAP *pstLd,
               IN const CHAR *pcDN,
       Output: OUT ULONG *pulErr 如果不关心可以为NULL
       Return: 成功/失败
      Caution: 返回BOOL_TRUE则一定存在且成功
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
STATIC BOOL_T _ldap_IsDnExist(IN LDAP *pstLd, IN const CHAR *pcDN, OUT ULONG *pulErr)
{
    ULONG ulRet;
    LDAPMessage *pstRes;
    BOOL_T bRet;

    ulRet = ldap_search_s(pstLd, pcDN, LDAP_SCOPE_BASE, NULL, NULL, 0, &pstRes);
    ldap_msgfree(pstRes);

    if(LDAP_SUCCESS == ulRet)
    {
        bRet = BOOL_TRUE;
        ulRet = ERROR_SUCCESS;
    }
    else if(LDAP_NO_SUCH_OBJECT == ulRet)
    {
        bRet = BOOL_FALSE;
        ulRet = ERROR_SUCCESS;
    }
    else
    {
        bRet = BOOL_FALSE;
        ulRet = ERROR_FAILED;
    }

    if(NULL != pulErr)
    {
        *pulErr = ulRet;
    }

    return bRet;
}

/*****************************************************************************
    Func Name: _ldap_ConnectServer
 Date Created: 2016/11/3
       Author: liangjinchao@dian
  Description: 连LDAP SERVER
        Input: 无
       Output: 无
       Return: 连接成功ERROR_SUCCESS或失败ERROR_FAILED
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
STATIC ULONG _ldap_ConnectServer()
{
    ULONG   ulRet;
    INT   iProtocolVersion, i;

    if(NULL == g_stInitData.pcAdminDn || NULL == g_stInitData.pcBase ||
       NULL == g_stInitData.pcLdapUrl ||  NULL == g_stInitData.pcPassword)
    {
        return ERROR_FAILED;
    }

    ulRet = ldap_initialize(&g_pstLDAP, g_stInitData.pcLdapUrl);
    if (ERROR_SUCCESS != ulRet)
    {
        return ulRet;
    }

    iProtocolVersion = LDAP_VERSION3;
    ulRet = ldap_set_option(g_pstLDAP, LDAP_OPT_PROTOCOL_VERSION, &iProtocolVersion);
    if (ERROR_SUCCESS != ulRet)
    {
        return ulRet;
    }

    ulRet = ERROR_FAILED;
    for(i = 0; i < ISNS_LDAP_INIT_TRY_MAX; i++)
    {
        if (ERROR_SUCCESS == ldap_simple_bind_s(g_pstLDAP, g_stInitData.pcAdminDn, g_stInitData.pcPassword))
        {
            if(BOOL_TRUE == _ldap_IsDnExist(g_pstLDAP, g_stInitData.pcBase, NULL))
            {
                ulRet = ERROR_SUCCESS;
                break;
            }
        }
        sleep(ISNS_LDAP_INIT_TRY_INTERVAL);
    }

    return ulRet;
}

/*****************************************************************************
    Func Name: _ldap_Reconnect
 Date Created: 2016/11/3
       Author: liangjinchao@dian
  Description: 重连LDAP SERVER
        Input: 无
       Output: 无
       Return: 连接成功ERROR_SUCCESS或失败ERROR_FAILED
      Caution: 密码认证失败也可能可以Search，但无法Add/Del
               反之，能Search成功不代表能Add/Del
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
STATIC ULONG _ldap_Reconnect()
{
    ULONG   ulRet;
    INT   iProtocolVersion;

    if(NULL == g_stInitData.pcAdminDn || NULL == g_stInitData.pcBase ||
       NULL == g_stInitData.pcLdapUrl ||  NULL == g_stInitData.pcPassword)
    {
        return ERROR_FAILED;
    }

    if(NULL != g_pstLDAP)
    {
        ldap_unbind(g_pstLDAP);
        g_pstLDAP = NULL;
    }

    ulRet = ldap_initialize(&g_pstLDAP, g_stInitData.pcLdapUrl);
    if (ERROR_SUCCESS != ulRet)
    {
        return ulRet;
    }

    iProtocolVersion = LDAP_VERSION3;
    ulRet = ldap_set_option(g_pstLDAP, LDAP_OPT_PROTOCOL_VERSION, &iProtocolVersion);
    if (ERROR_SUCCESS != ulRet)
    {
        return ulRet;
    }

    ulRet = ldap_simple_bind_s(g_pstLDAP, g_stInitData.pcAdminDn, g_stInitData.pcPassword);
    if (ERROR_SUCCESS != ulRet)
    {
        return ulRet;
    }

    if(BOOL_TRUE != _ldap_IsDnExist(g_pstLDAP, g_stInitData.pcBase, NULL))
    {
        return ERROR_FAILED;
    }

    return ulRet;
}

/*****************************************************************************
    Func Name: _ldap_Mkdir
 Date Created: 2016/11/3
       Author: liangjinchao@dian
  Description: 在base下添加目录
        Input: const CHAR *pcDirName
       Output:
       Return: 成功/失败
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
STATIC ULONG _ldap_Mkdir(const CHAR *pcDirName)
{
    ULONG ulRet;
    CHAR szDn[ISNS_LDAP_DN_MAX_SIZE];
    const CHAR *apcName[ISNS_LDAP_ATTR_MAX_SIZE] = { 0 }, *apcValue[ISNS_LDAP_ATTR_MAX_SIZE] = { 0 };
    LDAPMod **ppstMod;

    scnprintf(szDn, sizeof(szDn), ISNS_LDAP_OBJ_OU"=%s,%s", pcDirName, g_stInitData.pcBase);

    /* 如果已存在或出错则直接返回 */
    if(BOOL_TRUE == _ldap_IsDnExist(g_pstLDAP, szDn, &ulRet) || ERROR_SUCCESS != ulRet)
    {
        return ulRet;
    }

    apcName[0] = ISNS_LDAP_OBJCLASS,            apcValue[0] = ISNS_LDAP_OU_OBJCLASS;
    apcName[1] = ISNS_LDAP_OBJ_OU,              apcValue[1] = pcDirName;

    ppstMod = ISNS_LDAP_NewSingleAttrs(apcName, apcValue);
    if(NULL == ppstMod)
    {
        return ERROR_FAILED;
    }

    ulRet = ldap_add_s(g_pstLDAP, szDn, ppstMod);
    ISNS_LDAP_FreeAttrs(&ppstMod);

    return ulRet;
}

/*****************************************************************************
    Func Name: _ldap_Mkdirs
 Date Created: 2016/11/3
       Author: liangjinchao@dian
  Description: 在base下添加目录
        Input: const CHAR *pcDirName
       Output:
       Return: 成功/失败
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
STATIC ULONG _ldap_Mkdirs(const CHAR *const *ppcDirNames)
{
    ULONG ulRet = ERROR_SUCCESS;
    INT i;

    for(i = 0; NULL != ppcDirNames[i] && '\0' != ppcDirNames[i][0]; i++)
    {
        ulRet |= _ldap_Mkdir(ppcDirNames[i]);
    }

    return ulRet;
}

/*****************************************************************************
    Func Name: _ldap_FillValuesList
 Date Created: 2016/11/3
       Author: liangjinchao@dian
  Description: 填充VALUELIST
        Input: IN LDAPMessage *pstEntry, IN const CHAR **ppcNameList
       Output: OUT CHAR ***pppcValuesList
       Return: 无
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
STATIC VOID _ldap_FreeValuesList(IN CHAR ***pppcValuesList)
{
    INT i;

    for(i = 0; i < ISNS_LDAP_ATTR_MAX_SIZE && NULL != pppcValuesList[i]; i++)
    {
        ldap_value_free(pppcValuesList[i]);
        pppcValuesList[i] = NULL;
    }

    return ;
}

/*****************************************************************************
    Func Name: _ldap_FillValuesList
 Date Created: 2016/11/3
       Author: liangjinchao@dian
  Description: 填充VALUELIST
        Input: IN LDAPMessage *pstEntry, IN const CHAR **ppcNameList
       Output: OUT CHAR ***pppcValuesList
       Return: 无
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
STATIC ULONG _ldap_FillValuesList(IN LDAPMessage *pstEntry, IN const CHAR *const *ppcNameList,
                                  OUT CHAR ***pppcValuesList)
{
    ULONG ulRet = ERROR_SUCCESS;
    INT i;

    for(i = 0; i < ISNS_LDAP_ATTR_MAX_SIZE && NULL != ppcNameList[i]; i++)
    {
        pppcValuesList[i] = ldap_get_values(g_pstLDAP, pstEntry, ppcNameList[i]);
        if(NULL == pppcValuesList[i])
        {
            _ldap_FreeValuesList(pppcValuesList);
            ulRet = ERROR_FAILED;
            break;
        }
    }

    return ulRet;
}

/*****************************************************************************
    Func Name: ISNS_LDAP_Num2BitStr
 Date Created: 2016/11/3
       Author: liangjinchao@dian
  Description: 数字转LDAP二进制字符串
        Input: IN UINT uiNum,
       Output: OUT CHAR *pcOut
       Return: 成功/失败
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
VOID ISNS_LDAP_Num2BitStr(IN UINT32 uiNum, OUT CHAR *pcOut)
{
    char *pcStr = pcOut;
    int i;
    BOOL_T bStartPuts = BOOL_FALSE;   /* 用来跳过开头的0 */

    *pcStr++ = '\'';
    for(i = sizeof(uiNum) * 8 - 1; i >= 0; i--)
    {
        if(uiNum & (1 << i))
        {
            *pcStr++ = '1';
            bStartPuts = BOOL_TRUE;
        }
        else if(BOOL_TRUE == bStartPuts)
        {
            *pcStr++ = '0';
        }
    }

    if(0 == uiNum)
    {
        *pcStr++ = '0';
    }

    *pcStr++ = '\'';
    *pcStr++ = 'B';
    *pcStr++ = '\0';

    return;
}

/*****************************************************************************
    Func Name: ISNS_LDAP_EscapeRdn
 Date Created: 2016/11/11
       Author: liangjinchao@dian
  Description: 未转义的NAME,VALUE-->转义后的RDN
        Input: IN const CHAR *pcAttrName, IN const CHAR *pcAttrValue
       Output:
       Return: RDN 形如"xxx=xxx"
      Caution: 用完需要释放返回的指针
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
CHAR *ISNS_LDAP_EscapeRdn(IN const CHAR *pcAttrName, IN const CHAR *pcAttrValue)
{
    LDAPAVA stAva;
    LDAPAVA *astRdn[] = { &stAva, NULL };
    CHAR *pcName, *pcValue;
    char *pcRdn;

    pcName = strdup(pcAttrName);
    if(NULL == pcName)
    {
        return NULL;
    }

    pcValue = strdup(pcAttrValue);
    if(NULL == pcValue)
    {
        free(pcName);
        return NULL;
    }

    stAva.la_attr.bv_val = pcName;
    stAva.la_attr.bv_len = strlen(pcName);
    stAva.la_value.bv_val = pcValue;
    stAva.la_value.bv_len = strlen(pcValue);
    stAva.la_flags = LDAP_AVA_STRING;

    if(0 !=  ldap_rdn2str((LDAPRDN)&astRdn, &pcRdn, LDAP_DN_FORMAT_LDAPV2))
    {
        free(pcName);
        free(pcValue);
        return NULL;
    }

    free(pcName);
    free(pcValue);
    return pcRdn;
}

/*****************************************************************************
    Func Name: ISNS_LDAP_SetStringAttr
 Date Created: 2016/11/3
       Author: liangjinchao@dian
  Description: 设置ldap属性中的字符串(单值属性)
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
ULONG ISNS_LDAP_SetStringAttr(IN const CHAR *pcAttrName, IN const CHAR *pcAttrValue, OUT LDAPMod **ppstAttr)
{
    return _ldap_ModStringAttr(pcAttrName, pcAttrValue, LDAP_MOD_REPLACE, ppstAttr);
}

/*****************************************************************************
    Func Name: ISNS_LDAP_AddStringAttr
 Date Created: 2016/11/3
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
ULONG ISNS_LDAP_AddStringAttr(IN const CHAR *pcAttrName, IN const CHAR *pcAttrValue, OUT LDAPMod **ppstAttr)
{
    return _ldap_ModStringAttr(pcAttrName, pcAttrValue, LDAP_MOD_ADD, ppstAttr);
}

/*****************************************************************************
    Func Name: ISNS_LDAP_DelStringAttr
 Date Created: 2016/11/3
       Author: liangjinchao@dian
  Description: 删除ldap属性中的一个字符串
        Input: IN const CHAR *pcAttrName,
               IN const CHAR *pcAttrValue,
               OUT LDAPMod **ppstAttr
       Output:
       Return: 成功/失败
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
ULONG ISNS_LDAP_DelStringAttr(IN const CHAR *pcAttrName, IN const CHAR *pcAttrValue, OUT LDAPMod **ppstAttr)
{
    return _ldap_ModStringAttr(pcAttrName, pcAttrValue, LDAP_MOD_DELETE, ppstAttr);
}

/*****************************************************************************
    Func Name: ISNS_LDAP_NewSingleAttrs
 Date Created: 2016/11/3
       Author: liangjinchao@dian
  Description: 设置多个单值属性
        Input: IN const CHAR **ppcNameList, 字符串指针数组，以NULL指针结尾
               IN const CHAR **ppcValueList 字符串指针数组，以NULL指针结尾
       Output:
       Return: 成功/失败
      Caution: LDAPMod **用完后需要ISNS_LDAP_FreeAttrs释放
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
LDAPMod **ISNS_LDAP_NewSingleAttrs(IN const CHAR *const *ppcNameList,
                                   IN const CHAR *const *ppcValueList)
{
    LDAPMod **ppstMod;
    UINT uiI, uiM;

    ppstMod = ISNS_LDAP_NewAttrs();
    if(NULL == ppstMod)
    {
        return ppstMod;
    }

    uiM = 0;
    for(uiI = 0;  uiI < ISNS_LDAP_ATTR_MAX_SIZE && NULL != ppcNameList[uiI]; uiI++)
    {
        if(NULL == ppcValueList[uiI])
        {
            continue;
        }

        if(ERROR_SUCCESS != ISNS_LDAP_SetStringAttr(ppcNameList[uiI], ppcValueList[uiI], ppstMod + uiM))
        {
            ISNS_LDAP_FreeAttrs(&ppstMod);
            break;
        }
        uiM++;
    }

    return ppstMod;
}

/*********************************************************************
     Func Name : ISNS_LDAP_MultValueAdd
  Date Created : 2016/10/29
        Author : liangjinchao@dian
   Description : 删除PG
         Input : IN const SOIP_Portal_Group_Key *pstKey
        Output : 无
        Return : 成功/失败
       Caution :
----------------------------------------------------------------------
 Modification History
    DATE        NAME             DESCRIPTION
----------------------------------------------------------------------

*********************************************************************/
ULONG ISNS_LDAP_MultValueAdd(IN const CHAR *pcDn, IN const CHAR *pcAttrName, IN const CHAR *pcValue)
{
    LDAPMod **ppstAttrs;
    ULONG ulRet;

    ppstAttrs = ISNS_LDAP_NewAttrs();
    if(NULL == ppstAttrs)
    {
        return ERROR_FAILED;
    }

    if(ERROR_SUCCESS != ISNS_LDAP_AddStringAttr(pcAttrName, pcValue, ppstAttrs))
    {
        ISNS_LDAP_FreeAttrs(&ppstAttrs);
        return ERROR_FAILED;
    }

    ulRet = ISNS_LDAP_ModifyEntry(pcDn, ppstAttrs);
    ISNS_LDAP_FreeAttrs(&ppstAttrs);

    return ulRet;
}

/*********************************************************************
     Func Name : ISNS_LDAP_MultValueDel
  Date Created : 2016/10/29
        Author : liangjinchao@dian
   Description : 删除PG
         Input : IN const SOIP_Portal_Group_Key *pstKey
        Output : 无
        Return : 成功/失败
       Caution :
----------------------------------------------------------------------
 Modification History
    DATE        NAME             DESCRIPTION
----------------------------------------------------------------------

*********************************************************************/
ULONG ISNS_LDAP_MultValueDel(IN const CHAR *pcDn, IN const CHAR *pcAttrName, IN const CHAR *pcValue)
{
    LDAPMod **ppstAttrs;
    ULONG ulRet;

    ppstAttrs = ISNS_LDAP_NewAttrs();
    if(NULL == ppstAttrs)
    {
        return ERROR_FAILED;
    }

    if(ERROR_SUCCESS != ISNS_LDAP_DelStringAttr(pcAttrName, pcValue, ppstAttrs))
    {
        ISNS_LDAP_FreeAttrs(&ppstAttrs);
        return ERROR_FAILED;
    }

    ulRet = ISNS_LDAP_ModifyEntry(pcDn, ppstAttrs);
    ISNS_LDAP_FreeAttrs(&ppstAttrs);

    return ulRet;
}

/*********************************************************************
     Func Name : ISNS_LDAP_MultValueDelAll
  Date Created : 2016/10/29
        Author : liangjinchao@dian
   Description : 删除PG
         Input : IN const SOIP_Portal_Group_Key *pstKey
        Output : 无
        Return : 成功/失败
       Caution :
----------------------------------------------------------------------
 Modification History
    DATE        NAME             DESCRIPTION
----------------------------------------------------------------------

*********************************************************************/
ULONG ISNS_LDAP_MultValueDelAll(IN const CHAR *pcDn, IN const CHAR *pcAttrName)
{
    LDAPMod **ppstAttrs;
    ULONG ulRet;

    ppstAttrs = ISNS_LDAP_NewAttrs();
    if(NULL == ppstAttrs)
    {
        return ERROR_FAILED;
    }

    if(ERROR_SUCCESS != ISNS_LDAP_SetStringAttr(pcAttrName, "", ppstAttrs))
    {
        ISNS_LDAP_FreeAttrs(&ppstAttrs);
        return ERROR_FAILED;
    }

    ulRet = ISNS_LDAP_ModifyEntry(pcDn, ppstAttrs);
    ISNS_LDAP_FreeAttrs(&ppstAttrs);

    return ulRet;
}

/*****************************************************************************
    Func Name: ISNS_LDAP_NewAttrs
 Date Created: 2016/11/3
       Author: liangjinchao@dian
  Description: 申请ISNS_LDAP_ATTR_MAX_NUM + 1个指针的空间
        Input: LDAPMod **ppstAttrs
       Output:
       Return: 无
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
LDAPMod **ISNS_LDAP_NewAttrs()
{
    LDAPMod **ppstMod;
    UINT uiSize;

    uiSize = ISNS_LDAP_ATTR_MAX_SIZE * sizeof(LDAPMod *);
    ppstMod = (LDAPMod **)malloc(uiSize);
    if(NULL != ppstMod)
    {
        memset(ppstMod, 0, uiSize);
    }

    return ppstMod;
}

/*****************************************************************************
    Func Name: ISNS_LDAP_FreeAttrs
 Date Created: 2016/11/3
       Author: liangjinchao@dian
  Description: 属性值的内存释放和指针置0
        Input: LDAPMod **ppstAttrs
       Output:
       Return: 无
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
VOID ISNS_LDAP_FreeAttrs(IN LDAPMod ***pppstAttrs)
{
    LDAPMod *pstAttr, **ppstAttrs;
    INT iAttrNum;

    if(NULL == pppstAttrs)
    {
        return;
    }

    ppstAttrs = *pppstAttrs;
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

    free(ppstAttrs);
    *pppstAttrs = NULL;

    return;
}

/*****************************************************************************
    Func Name: ISNS_LDAP_ServerInit
 Date Created: 2016/11/3
       Author: liangjinchao@dian
  Description: LDAP全局初始化
        Input: IN const ISNS_LDAP_INIT_S *pstInit
               CHAR **ppcPreDirs;  预先建立的目录名，以NULL结尾的字符串指针
       Output: 无
       Return: 无
      Caution: ppcPreDirs ppcRdnName指向的数据必须是全局的，一直存在的
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
ULONG ISNS_LDAP_ServerInit(IN const ISNS_LDAP_INIT_S *pstInit,
                           IN const CHAR *const *ppcPreDirs)
{
    /* 保存初始化数据 */
    if(ERROR_SUCCESS != _ldap_SaveInitData(pstInit))
    {
        return ERROR_FAILED;
    }

    /* 建立连接 */
    if(ERROR_SUCCESS != _ldap_ConnectServer())
    {
        return ERROR_FAILED;
    }

    /* 预先创建BASE */


    /* 预先创建子目录 */
    if(ERROR_SUCCESS != _ldap_Mkdirs(ppcPreDirs))
    {
        return ERROR_FAILED;
    }

    return ERROR_SUCCESS;
}

/*****************************************************************************
    Func Name: ISNS_LDAP_ServerFini
 Date Created: 2016/11/3
       Author: liangjinchao@dian
  Description: LDAP全局去初始化
        Input: 无
       Output: OUT LDAP **pstLd
       Return: 无
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
VOID ISNS_LDAP_ServerFini()
{
    _ldap_FreeInitData();

    if(NULL != g_pstLDAP)
    {
        ldap_unbind(g_pstLDAP);
        g_pstLDAP = NULL;
    }
}

/*****************************************************************************
    Func Name: ISNS_LDAP_IsDnExist
 Date Created: 2016/11/3
       Author: liangjinchao@dian
  Description: 配置覆盖，如果不存在则新建
        Input: IN const CHAR *pcDn, OUT ULONG *pulErr
       Output: 无
       Return: 无
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
BOOL_T ISNS_LDAP_IsDnExist(IN const CHAR *pcDn, OUT ULONG *pulErr)
{
    ULONG ulRet;
    BOOL_T bRet;

    if(ERROR_SUCCESS != _ldap_CheckLdapInit())
    {
        *pulErr = ERROR_FAILED;
        return BOOL_FALSE;
    }

    /* 成功了直接返回，否则重连再试 */
    bRet = _ldap_IsDnExist(g_pstLDAP, pcDn, &ulRet);
    if(ERROR_SUCCESS == ulRet)
    {
        *pulErr = ERROR_SUCCESS;
        return bRet;
    }

    if(ERROR_SUCCESS != _ldap_Reconnect())
    {
        *pulErr = ERROR_FAILED;
        return BOOL_FALSE;
    }

    bRet = _ldap_IsDnExist(g_pstLDAP, pcDn, pulErr);
    return bRet;
}

/*****************************************************************************
    Func Name: ISNS_LDAP_AddEntry
 Date Created: 2016/11/3
       Author: liangjinchao@dian
  Description: 配置覆盖，如果不存在则新建
        Input: IN const CHAR *pcDn, IN LDAPMod **ppstAttrs
       Output: 无
       Return: 无
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
ULONG ISNS_LDAP_AddEntry(IN const CHAR *pcDn, IN LDAPMod **ppstAttrs)
{
    ULONG ulRet;

    if(ERROR_SUCCESS != _ldap_CheckLdapInit())
    {
        return ERROR_FAILED;
    }

    /* 成功了直接返回，否则重连再试 */
    ulRet = (ULONG)ldap_add_s(g_pstLDAP, pcDn, ppstAttrs);
    if(LDAP_SUCCESS == ulRet)
    {
        return ulRet;
    }

    if(ERROR_SUCCESS != _ldap_Reconnect())
    {
        return ERROR_FAILED;
    }

    return (ULONG)ldap_add_s(g_pstLDAP, pcDn, ppstAttrs);
}

/*****************************************************************************
    Func Name: ISNS_LDAP_ModifyEntry
 Date Created: 2016/11/3
       Author: liangjinchao@dian
  Description: 配置修改
        Input: IN const CHAR *pcDn, IN LDAPMod **ppstAttrs
       Output: 无
       Return: 无
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
ULONG ISNS_LDAP_ModifyEntry(IN const CHAR *pcDn, IN LDAPMod **ppstAttrs)
{
    ULONG ulRet;

    if(ERROR_SUCCESS != _ldap_CheckLdapInit())
    {
        return ERROR_FAILED;
    }

    /* 成功了直接返回，否则重连再试 */
    ulRet = (ULONG)ldap_modify_s(g_pstLDAP, pcDn, ppstAttrs);
    if(LDAP_SUCCESS == ulRet)
    {
        return ulRet;
    }

    if(ERROR_SUCCESS != _ldap_Reconnect())
    {
        return ERROR_FAILED;
    }

    return (ULONG)ldap_modify_s(g_pstLDAP, pcDn, ppstAttrs);
}

/*****************************************************************************
    Func Name: ISNS_LDAP_ReplaceEntry
 Date Created: 2016/11/3
       Author: liangjinchao@dian
  Description: 覆盖条目中指定的属性，如果条目不存在则新建
        Input: IN const CHAR *pcDn, IN LDAPMod **ppstAttrs
       Output: 无
       Return: 无
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
ULONG ISNS_LDAP_ReplaceEntry(IN const CHAR *pcDn, IN LDAPMod **ppstAttrs)
{
    ULONG ulRet;
    BOOL_T bIsExist;

    bIsExist = ISNS_LDAP_IsDnExist(pcDn, &ulRet);
    if(ERROR_SUCCESS != ulRet)
    {
        return ulRet;
    }

    if(BOOL_FALSE == bIsExist)
    {
        ulRet = ISNS_LDAP_AddEntry(pcDn, ppstAttrs);
    }
    else
    {
        ulRet = ISNS_LDAP_ModifyEntry(pcDn, ppstAttrs);
    }

    return ulRet;
}

/*****************************************************************************
    Func Name: ISNS_LDAP_DelEntry
 Date Created: 2016/11/3
       Author: liangjinchao@dian
  Description: 删除条目
        Input: IN const CHAR *pcDn
       Output: 无
       Return: 无
      Caution: 删除不存在的仍然返回成功
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
ULONG ISNS_LDAP_DelEntry(IN const CHAR *pcDn)
{
    ULONG ulRet;

    if(ERROR_SUCCESS != _ldap_CheckLdapInit())
    {
        return ERROR_FAILED;
    }

    /* 成功了直接返回，否则重连再试 */
    ulRet = (ULONG)ldap_delete_s(g_pstLDAP, pcDn);
    if(LDAP_SUCCESS == ulRet || LDAP_NO_SUCH_OBJECT == ulRet)
    {
        ulRet = ERROR_SUCCESS;
        return ulRet;
    }

    if(ERROR_SUCCESS != _ldap_Reconnect())
    {
        return ERROR_FAILED;
    }

    ulRet = (ULONG)ldap_delete_s(g_pstLDAP, pcDn);
    if(LDAP_SUCCESS == ulRet || LDAP_NO_SUCH_OBJECT == ulRet)
    {
        ulRet = ERROR_SUCCESS;
    }
    return ulRet;
}

/*****************************************************************************
    Func Name: ISNS_LDAP_SearchEntry
 Date Created: 2016/11/3
       Author: liangjinchao@dian
  Description: 尝试LDAP查找，如果失败则重连再试
        Input: const char *pcBase,
               int iScope,
               const char *pcFilter,
               char **ppcAttrs,
               int iAttrsonly,
       Output: OUT LDAPMessage **ppstRes
               OUT LDAP **ppstLdap  可能后面会用到
       Return: 成功/失败
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
ULONG ISNS_LDAP_SearchEntry(
    IN const CHAR *pcBase,
    IN INT iScope,
    IN const CHAR *pcFilter,
    IN CHAR **ppcAttrs,
    IN INT iAttrsonly,
    OUT LDAPMessage **ppstRes,
    OUT LDAP **ppstLdap)
{
    ULONG ulRet;

    if(ERROR_SUCCESS != _ldap_CheckLdapInit())
    {
        return ERROR_FAILED;
    }

    ulRet = ldap_search_s(g_pstLDAP, pcBase, iScope, pcFilter, ppcAttrs, iAttrsonly, ppstRes);
    if(LDAP_NO_SUCH_OBJECT == ulRet || LDAP_SUCCESS == ulRet)
    {
        if(NULL != ppstLdap)
        {
            *ppstLdap = g_pstLDAP;
        }
        return ulRet;
    }

    if(ERROR_SUCCESS != _ldap_Reconnect())
    {
        return ERROR_FAILED;
    }

    ulRet = ldap_search_s(g_pstLDAP, pcBase, iScope, pcFilter, ppcAttrs, iAttrsonly, ppstRes);
    if(NULL != ppstLdap)
    {
        *ppstLdap = g_pstLDAP;
    }

    return ulRet;
}

/*****************************************************************************
    Func Name: ISNS_LDAP_ScanDir
 Date Created: 2016/11/3
       Author: liangjinchao@dian
  Description: 遍历某个目录下的一级条目，获取指定属性的值
        Input: IN const CHAR *pcDirDn,  要遍历的目录的DN
               IN const CHAR **ppcNameList,要获取的属性值字符串指针数组，以NULL结尾
               IN ISNS_LDAP_SCAN_PF pfCallback,  回调函数
               INOUT VOID *pSelfData  回调函数的私有数据
       Output: 无
       Return: 成功/失败/不存在该目录
      Caution: 如果pcDirDn不存在则返回LDAP_NO_SUCH_OBJECT，其他错误码为异常
               如果回调函数返回非0则终止遍历
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
ULONG ISNS_LDAP_ScanDir(IN const CHAR *pcDirDn, IN const CHAR *const *ppcNameList,
                        IN ISNS_LDAP_SCAN_PF pfCallback, INOUT VOID *pSelfData)
{
    LDAPMessage *pstRes, *pstEntry;
    CHAR **appcValuesList[ISNS_LDAP_ATTR_MAX_SIZE];
    ULONG ulRet;

    ulRet = ISNS_LDAP_SearchEntry(pcDirDn, LDAP_SCOPE_ONELEVEL, 0, 0, 0, &pstRes, NULL);
    if(LDAP_SUCCESS != ulRet)
    {
        return ulRet;
    }

    for(pstEntry = ldap_first_entry(g_pstLDAP, pstRes); \
        NULL != pstEntry; \
        pstEntry = ldap_next_entry(g_pstLDAP, pstEntry))
    {
        memset(appcValuesList, 0, sizeof(appcValuesList));
        if(ERROR_SUCCESS != _ldap_FillValuesList(pstEntry, ppcNameList, appcValuesList))
        {
            /* 遇到没有指定属性的条目则跳过 */
            continue;
        }

        if(ERROR_SUCCESS != pfCallback(ppcNameList, appcValuesList, pSelfData))
        {
            _ldap_FreeValuesList(appcValuesList);
            break;
        }

        _ldap_FreeValuesList(appcValuesList);
    }

    ldap_msgfree(pstRes);
    return ERROR_SUCCESS;
}



