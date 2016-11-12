/***********************************************************************
  Copyright (c) 2001, Nishan Systems, Inc.
  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are
  met:

  - Redistributions of source code must retain the above copyright notice,
    this list of conditions and the following disclaimer.

  - Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.

  - Neither the name of the Nishan Systems, Inc. nor the names of its
    contributors may be used to endorse or promote products derived from
    this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  IMPLIED WARRANTIES OF MERCHANTABILITY, NON-INFRINGEMENT AND FITNESS FOR A
  PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL NISHAN SYSTEMS, INC.
  OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
  OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
  WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
  OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

***********************************************************************/


/*
 * This file contains source code for implementing the
 * main processing loop for the SoIP service task.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/basetype.h>
#include <sys/error.h>
#include <sys/list.h>

#ifndef LDAP_DEPRECATED
#define LDAP_DEPRECATED 1
#endif
#include <openldap/ldap.h>
//#include <ldap.h>

#include "iSNSNdb.h"
#include "iSNSNdbLdap.h"

static LDAP *g_pstLDAP = NULL;
static NDB_INITIALIZE_S g_stInitData = { 0 };

/* Search Cache */
static LDAPMessage *g_pstRes = NULL;
static LDAPMessage *g_pstE = NULL;


/*****************************************************************************
    Func Name: _ldap_IsDnExist
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
static int _ldap_IsDnExist(LDAP *pstLd, const char *pcDN, short *pbIsExist)
{
    int iRet;
    LDAPMessage *pstRes;

    *pbIsExist = 0;
    iRet = ldap_search_s(pstLd, pcDN, LDAP_SCOPE_BASE, NULL, NULL, 0, &pstRes);
    if(LDAP_SUCCESS == iRet)
    {
        *pbIsExist = 1;
    }
    else if(LDAP_NO_SUCH_OBJECT == iRet)
    {
        iRet = NDB_SUCCESS;
    }

    ldap_msgfree(pstRes);
    return iRet;
}

/*****************************************************************************
    Func Name: _ldap_TrySearch
 Date Created: 2016/10/8
       Author: liangjinchao@dian
  Description: 尝试LDAP查找，如果失败则重连再试
        Input: LDAP *pstLd,
               const char *pcBase,
               int iScope,
               const char *pcFilter,
               char **ppcAttrs,
               int iAttrsonly,
               LDAPMessage **ppstRes
       Output: 成功/失败
       Return:
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
static inline int _ldap_TrySearch(
    LDAP *pstLd,
    const char *pcBase,
    int iScope,
    const char *pcFilter,
    char **ppcAttrs,
    int iAttrsonly,
    LDAPMessage **ppstRes)
{
    int iRet;

    iRet = ldap_search_s(pstLd, pcBase, iScope, pcFilter, ppcAttrs, iAttrsonly, ppstRes);
    if(LDAP_NO_SUCH_OBJECT == iRet)
    {
        return iRet;
    }
    else if(LDAP_SUCCESS != iRet)
    {
        if(NDB_SUCCESS == ndb_ldap_reconnect())
        {
            iRet = ldap_search_s(pstLd, pcBase, iScope, pcFilter, ppcAttrs, iAttrsonly, ppstRes);
        }
    }

    return iRet;
}

/*****************************************************************************
    Func Name: _ldap_set_string_attr
 Date Created: 2016/10/8
       Author: liangjinchao@dian
  Description: 设置ldap属性中的字符串
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
static int _ldap_set_string_attr(LDAPMod **ppstAttr,
                                 const char *pcAttrValue,
                                 const char *pcAttrName)
{
    LDAPMod  *pstMod;
    char **ppcVals;
    char *pcAttrNameTmp;

    pstMod = malloc(sizeof(LDAPMod));
    if(NULL == pstMod)
    {
        return NDB_FAILED;
    }

    ppcVals = malloc(2 * sizeof(char *));
    if(NULL == ppcVals)
    {
        free(pstMod);
        return NDB_FAILED;
    }

    ppcVals[0] = strdup(pcAttrValue);   /* 隐式申请了内存 */
    if(NULL == ppcVals[0])
    {
        free(pstMod);
        free(ppcVals);
        return NDB_FAILED;
    }
    ppcVals[1] = NULL;

    pcAttrNameTmp = strdup(pcAttrName);
    if(NULL == pcAttrNameTmp)
    {
        free(pstMod);
        free(ppcVals[0]);
        free(ppcVals);
        return NDB_FAILED;
    }

    memset(pstMod, 0, sizeof(LDAPMod));
    pstMod->mod_op = LDAP_MOD_REPLACE;
    pstMod->mod_values = ppcVals;
    pstMod->mod_type = pcAttrNameTmp;

    *ppstAttr = pstMod;

    return NDB_SUCCESS;
}

/*****************************************************************************
    Func Name: _ldap_ModStrings_free
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
static void _ldap_ModStrings_free(char ***pppcStrings)
{
    char **ppStrings = *pppcStrings;
    int i;

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
    Func Name: _ldap_ModBvalues_free
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
static void _ldap_ModBvalues_free(struct berval ***pppstData)
{
    struct berval **ppstData = *pppstData;
    int i;

    if(NULL == ppstData)
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
    Func Name: _ldap_attrs_free
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
static void _ldap_attrs_free(LDAPMod **ppstAttrs)
{
    LDAPMod *pstAttr;
    int iAttrNum;

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
            _ldap_ModBvalues_free(&pstAttr->mod_bvalues);
        }
        else
        {
            _ldap_ModStrings_free(&pstAttr->mod_values);
        }

        free(pstAttr);
        ppstAttrs[iAttrNum] = NULL;
    }

    return;
}

/*****************************************************************************
    Func Name: _ndb_CheckLdapInit
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
static int _ndb_CheckLdapInit()
{
    if(NULL == g_pstLDAP || NULL == g_stInitData.pcBase)
    {
        return NDB_FAILED;
    }

    return NDB_SUCCESS;
}

/*****************************************************************************
    Func Name: _ndb_HexStr2Bin_Alloc
 Date Created: 2016/10/8
       Author: liangjinchao@dian
  Description: 输入字符串以%x开头
        Input: const char *pcHexStr, int *piDataSize
       Output: 输出数据的长度
       Return: 二进制数据
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
static char *_ndb_HexStr2Bin_Alloc(const char *pcHexStr, int *piDataSize)
{
    const char *pcPtr = pcHexStr;
    char *pcBinData = NULL;
    int i, iMallocSize, iTmp;

    if(0 == strncmp(pcHexStr, "%x", 2))
    {
        pcPtr += 2;
    }

    iMallocSize = (strlen(pcPtr) + 1) / 2;
    pcBinData = (char *)malloc(iMallocSize);

    for(i = 0; i < iMallocSize; i++)
    {
        if(sscanf(pcPtr, "%02X", &iTmp) <= 0)
        {
            break;
        }
        *(pcBinData + i) = (char)iTmp;
        pcPtr += 2;
    }

    *piDataSize = i;

    return pcBinData;
}

/*****************************************************************************
    Func Name: _ldap_GetValByDn
 Date Created: 2016/10/8
       Author: liangjinchao@dian
  Description: 输入字符串以%x开头
        Input: LDAP *pstLd, const char *pcDN, int *piDataSize
       Output: 返回数据长度
       Return: 返回数据指针
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
static char *_ldap_GetValByDn(LDAP *pstLd, const char *pcDN, int *piDataSize)
{
    LDAPMessage *pstRes, *pstFirst;
    char *pcData = NULL;
    char **ppcStrVals;

    *piDataSize = 0;

    if(LDAP_SUCCESS != _ldap_TrySearch(pstLd, pcDN, LDAP_SCOPE_BASE, NULL, NULL, 0, &pstRes))
    {
        return pcData;
    }

    pstFirst = ldap_first_entry(pstLd, pstRes);
    if(NULL == pstFirst)
    {
        ldap_msgfree(pstRes);
        return pcData;
    }

    ppcStrVals = ldap_get_values(pstLd, pstFirst, NDB_ATTR_VALUE);
    if(NULL == ppcStrVals[0])
    {
        ldap_msgfree(pstRes);
        return pcData;
    }

    pcData = _ndb_HexStr2Bin_Alloc(ppcStrVals[0], piDataSize);
    ldap_value_free(ppcStrVals);

    ldap_msgfree(pstRes);
    return pcData;
}

/*****************************************************************************
    Func Name: _ndb_MsgCacheFree
 Date Created: 2016/10/8
       Author: liangjinchao@dian
  Description: 释放消息内存
        Input: 无
       Output: 无
       Return: 无
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
static void _ndb_MsgCacheFree()
{
    if(NULL != g_pstRes)
    {
        ldap_msgfree(g_pstRes);
        g_pstRes = NULL;
    }
    g_pstE = NULL;

    return;
}

/*****************************************************************************
    Func Name: _ndb_DirId2DirName
 Date Created: 2016/10/8
       Author: liangjinchao@dian
  Description: 转换为dirname
        Input: 无
       Output: 无
       Return: 无
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
static int _ndb_DirId2DirName(int iDirId, char *pcDirName, int iSize)
{
    return snprintf(pcDirName, iSize, "%d", iDirId);
}

/*****************************************************************************
    Func Name: _ndb_Bin2HexStr_Alloc
 Date Created: 2016/10/8
       Author: liangjinchao@dian
  Description: 二进制数据转十六进制字符串
        Input: 无
       Output: 无
       Return: 无
      Caution: 以%x开头
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
static char *_ndb_Bin2HexStr_Alloc(const char *pcBinData, int iByteSize)
{
    char *pcRet, *pcPtr, *pcHex = "0123456789ABCDEF";
    unsigned char ucTmp;
    int i;

    pcRet = pcPtr = (char *)malloc(2 + 2 * iByteSize + 1);
    if(NULL == pcRet)
    {
        return pcRet;
    }

    *pcPtr = '%';
    *++pcPtr = 'x';

    for(i = 0; i < iByteSize; i++)
    {
        ucTmp = pcBinData[i];
        *++pcPtr = pcHex[ucTmp >> 4];
        *++pcPtr = pcHex[ucTmp & 0x0f];
    }

    *++pcPtr = '\0';
    return pcRet;
}

/*****************************************************************************
    Func Name: _ndb_DnName_Alloc
 Date Created: 2016/10/8
       Author: liangjinchao@dian
  Description: 根据dirId和KEY得到dn
        Input: int iDirId, const datum *pstKey
       Output: 无
       Return: 无
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
static char *_ndb_DnName_Alloc(int iDirId, const datum *pstKey)
{
    char *pcRet, *pcHexStr;
    char szDirName[NDB_DIR_NAME_MAX_LEN + 1];
    int iSize;

    (void)_ndb_DirId2DirName(iDirId, szDirName, sizeof(szDirName));

    if(NULL == pstKey)
    {
        pcRet = malloc(NDB_NON_KEY_MAX_LEN + 1);
        snprintf(pcRet, NDB_NON_KEY_MAX_LEN + 1, NDB_OBJ_OU"=%s,%s", szDirName, g_stInitData.pcBase);
        return pcRet;
    }

    iSize = 2*pstKey->dsize + NDB_NON_KEY_MAX_LEN + 1;
    pcRet = malloc(iSize);
    if(NULL == pcRet)
    {
        return pcRet;   
    }

    pcHexStr = _ndb_Bin2HexStr_Alloc(pstKey->dptr, pstKey->dsize);
    if(NULL == pcHexStr)
    {
        free(pcRet);
        return NULL;
    }

    memset(pcRet, 0, iSize);
    snprintf(pcRet, iSize, "%s=%s,"NDB_OBJ_OU"=%s,%s",
             NDB_ATTR_KEY, pcHexStr, szDirName, g_stInitData.pcBase);

    free(pcHexStr);
    return pcRet;
}

/*****************************************************************************
    Func Name: _ndb_ldap_store_dir
 Date Created: 2016/10/8
       Author: liangjinchao@dian
  Description: 设置多个数据目录，目录编号从0开始，不同目录数据隔开存放
        Input:
       Output:
       Return:
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
static int _ndb_ldap_store_dir(int iDirId)
{
    LDAPMod *apstAttrs[] = { NULL, NULL, NULL };
    char szDirName[NDB_DIR_NAME_MAX_LEN + 1];
    char *pcDirDn;
    int iRet = NDB_SUCCESS;
    short bIsDnExist;

    pcDirDn = _ndb_DnName_Alloc(iDirId, NULL);
    if(NULL == pcDirDn)
    {
        return NDB_FAILED;
    }

    if(NDB_SUCCESS != _ldap_IsDnExist(g_pstLDAP, pcDirDn, &bIsDnExist))
    {
        free(pcDirDn);
        return NDB_FAILED;
    }
    if(bIsDnExist)
    {
        free(pcDirDn);
        return iRet;
    }

    _ndb_DirId2DirName(iDirId, szDirName, sizeof(szDirName));
    if(NDB_SUCCESS != _ldap_set_string_attr(&apstAttrs[0], szDirName, NDB_OBJ_OU))
    {
        _ldap_attrs_free(apstAttrs);
        free(pcDirDn);
        return NDB_FAILED;
    }

    if(NDB_SUCCESS != _ldap_set_string_attr(&apstAttrs[1], NDB_OU_OBJCLASS, NDB_OBJCLASS))
    {
        _ldap_attrs_free(apstAttrs);
        free(pcDirDn);
        return NDB_FAILED;
    }

    iRet = ldap_add_s(g_pstLDAP, pcDirDn, apstAttrs);

    _ldap_attrs_free(apstAttrs);
    free(pcDirDn);
    return iRet;
}

/*****************************************************************************
    Func Name: ndb_ldap_dir_set
 Date Created: 2016/10/8
       Author: liangjinchao@dian
  Description: 设置多个数据目录，目录编号从0开始，不同目录数据隔开存放
        Input:
       Output:
       Return:
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
static int _ndb_ldap_dir_set (int iDirCount)
{
    int i, iRet = NDB_SUCCESS;

    for(i = 0; i < iDirCount; i++)
    {
        if(NDB_SUCCESS != _ndb_ldap_store_dir(i))
        {
            iRet = NDB_FAILED;
            break;
        }
    }

    return iRet;
}


/*****************************************************************************
    Func Name: _ndb_FreePointer
 Date Created: 2016/10/8
       Author: liangjinchao@dian
  Description: 释放指针
        Input: char **ppPtr
       Output:
       Return:
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
static void _ndb_FreePointer(char **ppPtr)
{
    void *pReal;

    pReal = *ppPtr;

    if(NULL != pReal)
    {
        free(pReal);
        *ppPtr = NULL;
    }

    return ;
}

/*****************************************************************************
    Func Name: _ndb_FreeInitData
 Date Created: 2016/10/8
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
static void _ndb_FreeInitData()
{
    g_stInitData.iDirMaxCount = 0;
    _ndb_FreePointer(&g_stInitData.pcAdminDn);
    _ndb_FreePointer(&g_stInitData.pcBase);
    _ndb_FreePointer(&g_stInitData.pcLdapUrl);
    _ndb_FreePointer(&g_stInitData.pcPassword);
}

/*****************************************************************************
    Func Name: _ndb_SaveInitData
 Date Created: 2016/10/8
       Author: liangjinchao@dian
  Description: 保存初始化数据，便于重新连接
        Input:
       Output:
       Return:
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
static int _ndb_SaveInitData(const NDB_INITIALIZE_S *pstNdbInit)
{
    int iRet = NDB_SUCCESS;

    memset(&g_stInitData, 0, sizeof(g_stInitData));

    g_stInitData.iDirMaxCount = pstNdbInit->iDirMaxCount;

    g_stInitData.pcAdminDn = strdup(pstNdbInit->pcAdminDn);
    if(NULL == g_stInitData.pcAdminDn)
    {
        return NDB_FAILED;
    }

    g_stInitData.pcBase = strdup(pstNdbInit->pcBase);
    if(NULL == g_stInitData.pcBase)
    {
        return NDB_FAILED;
    }

    g_stInitData.pcLdapUrl = strdup(pstNdbInit->pcLdapUrl);
    if(NULL == g_stInitData.pcLdapUrl)
    {
        return NDB_FAILED;
    }

    g_stInitData.pcPassword = strdup(pstNdbInit->pcPassword);
    if(NULL == g_stInitData.pcPassword)
    {
        return NDB_FAILED;
    }

    return iRet;
}

/*****************************************************************************
    Func Name: ndb_ldap_init
 Date Created: 2016/10/8
       Author: liangjinchao@dian
  Description: 初始化ndb的全局变量，建立ldap连接
        Input:
       Output:
       Return:
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
int ndb_ldap_init(const NDB_INITIALIZE_S *pstNdbInit)
{
    int   iRet;

    g_pstLDAP = NULL;
    g_pstRes = NULL;
    g_pstE = NULL;

    iRet = _ndb_SaveInitData(pstNdbInit);
    if(NDB_SUCCESS != iRet)
    {
        return iRet;
    }

    iRet = ndb_ldap_reconnect();

    return iRet;
}

/*****************************************************************************
    Func Name: ndb_ldap_reconnect
 Date Created: 2016/10/8
       Author: liangjinchao@dian
  Description: 重连LDAP SERVER (出错情况用 或
               可用作启动时判断SERVER是否启动成功(反复调用直到返回成功))
        Input: 无
       Output: 无
       Return: 连接成功NDB_SUCCESS或失败NDB_FAILED
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
int ndb_ldap_reconnect()
{
    int   iRet;
    int   iProtocolVersion;
    short bHasBase;

    if(NULL == g_stInitData.pcAdminDn || NULL == g_stInitData.pcBase ||
       NULL == g_stInitData.pcLdapUrl ||  NULL == g_stInitData.pcPassword)
    {
        return NDB_FAILED;
    }

    if(NULL == g_pstLDAP)
    {
        iRet = ldap_initialize(&g_pstLDAP, g_stInitData.pcLdapUrl);
        if (NDB_SUCCESS != iRet)
        {
            return iRet;
        }
    }

    iProtocolVersion = LDAP_VERSION3;
    iRet = ldap_set_option(g_pstLDAP, LDAP_OPT_PROTOCOL_VERSION, &iProtocolVersion);
    if (NDB_SUCCESS != iRet)
    {
        return iRet;
    }

    iRet = ldap_simple_bind_s(g_pstLDAP, g_stInitData.pcAdminDn, g_stInitData.pcPassword);
    if (NDB_SUCCESS != iRet)
    {
        return iRet;
    }

    iRet = _ldap_IsDnExist(g_pstLDAP, g_stInitData.pcBase, &bHasBase);
    if(NDB_SUCCESS != iRet || !bHasBase)
    {
        return NDB_FAILED;
    }

    iRet = _ndb_ldap_dir_set(g_stInitData.iDirMaxCount);

    return iRet;
}

/*****************************************************************************
    Func Name: ndb_ldap_close
 Date Created: 2016/10/8
       Author: liangjinchao@dian
  Description: 关闭数据库
        Input:
       Output:
       Return:
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
void ndb_ldap_close()
{
    _ndb_MsgCacheFree();
    _ndb_FreeInitData();
    if(NULL != g_pstLDAP)
    {
        ldap_unbind(g_pstLDAP);
        g_pstLDAP = NULL;
    }

    return;
}

/*****************************************************************************
    Func Name: ndb_ldap_store_sns
 Date Created: 2016/10/8
       Author: liangjinchao@dian
  Description: 保存数据
        Input: const char *pcDn, const char *pcKey, const char *pcValue
       Output:
       Return:
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
static int _ndb_store_sns(const char *pcDn, const char *pcKey, const char *pcValue)
{
    LDAPMod *apstAttrs[NDB_ATTR_NUM_MAX + 1] = { 0 };
    int iRet, i = 0;
    short bIsDnExist;

    /* 如果连接不上则尝试重连 */
    if(NDB_SUCCESS != _ldap_IsDnExist(g_pstLDAP, pcDn, &bIsDnExist))
    {
        if(NDB_SUCCESS != ndb_ldap_reconnect())
        {
            return NDB_FAILED;
        }

        /* 重连成功之后再次判断 */
        if(NDB_SUCCESS != _ldap_IsDnExist(g_pstLDAP, pcDn, &bIsDnExist))
        {
            return NDB_FAILED;
        }
    }

    if(_ldap_set_string_attr(&apstAttrs[i++], NDB_OBJCLASS_DATA, NDB_OBJCLASS))
    {
        _ldap_attrs_free(apstAttrs);
        return NDB_FAILED;
    }

    if(_ldap_set_string_attr(&apstAttrs[i++], pcKey, NDB_ATTR_KEY))
    {
        _ldap_attrs_free(apstAttrs);
        return NDB_FAILED;
    }

    if(_ldap_set_string_attr(&apstAttrs[i++], pcValue, NDB_ATTR_VALUE))
    {
        _ldap_attrs_free(apstAttrs);
        return NDB_FAILED;
    }

    if(bIsDnExist)
    {
        iRet = ldap_modify_s(g_pstLDAP, pcDn, apstAttrs);
    }
    else
    {
        iRet = ldap_add_s(g_pstLDAP, pcDn, apstAttrs);
    }

    _ldap_attrs_free(apstAttrs);
    return iRet;
}

/*****************************************************************************
    Func Name: ndb_ldap_store_sns
 Date Created: 2016/10/8
       Author: liangjinchao@dian
  Description: 保存数据
        Input: int iDirId, datum stKey, datum stValue, int iFlag
       Output:
       Return:
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
int ndb_ldap_store_sns (int iDirId, datum stKey, datum stValue, int iFlag)
{
    char *pcDn, *pcKey, *pcValue;
    int iRet;

    if(NDB_SUCCESS != _ndb_CheckLdapInit())
    {
        return NDB_FAILED;
    }

    pcDn = _ndb_DnName_Alloc(iDirId, &stKey);
    if(NULL == pcDn)
    {
        return NDB_FAILED;
    }

    pcKey = _ndb_Bin2HexStr_Alloc(stKey.dptr, stKey.dsize);
    if(NULL == pcKey)
    {
        free(pcDn);
        return NDB_FAILED;
    }

    pcValue = _ndb_Bin2HexStr_Alloc(stValue.dptr, stValue.dsize);
    if(NULL == pcValue)
    {
        free(pcKey);
        free(pcDn);
        return NDB_FAILED;
    }

    iRet = _ndb_store_sns(pcDn, pcKey, pcValue);

    free(pcValue);
    free(pcKey);
    free(pcDn);
    return iRet;
}

/*****************************************************************************
    Func Name: ndb_ldap_fetch
 Date Created: 2016/10/8
       Author: liangjinchao@dian
  Description: 获取数据
        Input: int iDirId, datum stKey
       Output:
       Return:
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
datum ndb_ldap_fetch (int iDirId, datum stKey)
{
    char *pcDn;
    datum stRet;
    int iDataSize;

    memset(&stRet, 0, sizeof(stRet));
    if(NDB_SUCCESS != _ndb_CheckLdapInit())
    {
        return stRet;
    }

    pcDn = _ndb_DnName_Alloc(iDirId, &stKey);
    if(NULL == pcDn)
    {
        return stRet;
    }

    stRet.dptr = _ldap_GetValByDn(g_pstLDAP, pcDn, &iDataSize);
    stRet.dsize = iDataSize;
    return stRet;
}

/*****************************************************************************
    Func Name: ndb_ldap_fetch_sns
 Date Created: 2016/10/8
       Author: liangjinchao@dian
  Description: 获取数据
        Input: int iDirId, datum stKey, void *pDst
       Output: 复制到pDst
       Return:
      Caution: 复制到pDst
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
datum ndb_ldap_fetch_sns (int iDirId, datum stKey, void *pDst)
{
    datum stValue;

    memset(&stValue, 0, sizeof(stValue));
    stValue = ndb_ldap_fetch(iDirId, stKey);
    memcpy(pDst, stValue.dptr, stValue.dsize);
    return stValue;
}

/*****************************************************************************
    Func Name: ndb_ldap_delete
 Date Created: 2016/10/8
       Author: liangjinchao@dian
  Description: 删除数据
        Input: int iDirId, datum stKey
       Output: 成功失败
       Return:
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
int ndb_ldap_delete (int iDirId, datum stKey)
{
    char *pcDn;
    int iRet;

    if(NDB_SUCCESS != _ndb_CheckLdapInit())
    {
        return NDB_FAILED;
    }

    pcDn = _ndb_DnName_Alloc(iDirId, &stKey);
    if(NULL == pcDn)
    {
        return NDB_FAILED;
    }

    iRet = ldap_delete_s(g_pstLDAP, pcDn);
    if(LDAP_NO_SUCH_OBJECT == iRet)
    {
        iRet = NDB_SUCCESS;
    }
    else if(LDAP_SUCCESS != iRet)
    {
        /* 尝试重连 */
        if(NDB_SUCCESS == ndb_ldap_reconnect())
        {
            iRet = ldap_delete_s(g_pstLDAP, pcDn);
        }
    }

    free(pcDn);
    return iRet;
}

/*****************************************************************************
    Func Name: _ndb_AllocRdnDataByDn
 Date Created: 2016/10/8
       Author: liangjinchao@dian
  Description: 通过DN得到RDN
        Input: const char *pcDn, int *piDataSize
       Output:
       Return:
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
static char *_ndb_AllocRdnDataByDn(const char *pcDn, int *piDataSize)
{
    char **ppcRdn;
    char *pcRnHexStr, *pcBinData;

    *piDataSize = 0;
    ppcRdn = ldap_explode_rdn(pcDn, 1);
    pcRnHexStr = ppcRdn[0];
    pcBinData = _ndb_HexStr2Bin_Alloc(pcRnHexStr, piDataSize);
    ldap_value_free(ppcRdn);

    return pcBinData;
}

/*****************************************************************************
    Func Name: ndb_ldap_firstkey
 Date Created: 2016/10/8
       Author: liangjinchao@dian
  Description: 获取dir下面的第一个key
        Input: int iDirId
       Output:
       Return:
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
datum ndb_ldap_firstkey (int iDirId)
{
    char *pcDirDn, *pcDn;
    datum stRet;
    int iDatSize;

    memset(&stRet, 0, sizeof(stRet));
    if(NDB_SUCCESS != _ndb_CheckLdapInit())
    {
        return stRet;
    }

    pcDirDn = _ndb_DnName_Alloc(iDirId, NULL);
    if(NULL == pcDirDn)
    {
        return stRet;
    }

    _ndb_MsgCacheFree();
    if (LDAP_SUCCESS != _ldap_TrySearch(g_pstLDAP, pcDirDn, LDAP_SCOPE_ONELEVEL, 0, 0, 0, &g_pstRes))
    {
        free(pcDirDn);
        return stRet;
    }

    g_pstE = ldap_first_entry(g_pstLDAP, g_pstRes);
    if(NULL == g_pstE)
    {
        _ndb_MsgCacheFree();
        free(pcDirDn);
        return stRet;
    }

    pcDn = ldap_get_dn(g_pstLDAP, g_pstE);
    stRet.dptr = _ndb_AllocRdnDataByDn(pcDn, &iDatSize);
    stRet.dsize = iDatSize;
    free(pcDn);

    /* 不需要free g_pstRes */
    free(pcDirDn);
    return stRet;
}

/*****************************************************************************
    Func Name: ndb_ldap_nextkey
 Date Created: 2016/10/8
       Author: liangjinchao@dian
  Description: 获取dir下面的第一个key
        Input: int iDirId, const char *pcDnInput
       Output: 下一个key
       Return:
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
static datum _ndb_GetNextKey(int iDirId, const char *pcDnInput)
{
    char *pcDirDn, *pcDnTmp;
    datum stRet;
    int iDatSize;

    memset(&stRet, 0, sizeof(stRet));

    pcDirDn = _ndb_DnName_Alloc(iDirId, NULL);
    if(NULL == pcDirDn)
    {
        return stRet;
    }

    _ndb_MsgCacheFree();
    if (LDAP_SUCCESS != _ldap_TrySearch(g_pstLDAP, pcDirDn, LDAP_SCOPE_ONELEVEL, 0, 0, 0, &g_pstRes))
    {
        free(pcDirDn);
        return stRet;
    }

    g_pstE = ldap_first_entry(g_pstLDAP, g_pstRes);
    if(NULL == g_pstE)
    {
        _ndb_MsgCacheFree();
        free(pcDirDn);
        return stRet;
    }

    /* 找到名字相同的再寻找下一个 */
    for(; g_pstE != NULL; g_pstE = ldap_next_entry(g_pstLDAP, g_pstE))
    {
        pcDnTmp = ldap_get_dn(g_pstLDAP, g_pstE);
        if(0 == strcmp(pcDnTmp, pcDirDn))
        {
            free(pcDnTmp);
            break;
        }

        free(pcDnTmp);
    }

    if(NULL != g_pstE)
    {
        pcDnTmp = ldap_get_dn(g_pstLDAP, g_pstE);
        stRet.dptr = _ndb_AllocRdnDataByDn(pcDnTmp, &iDatSize);
        stRet.dsize = iDatSize;
    }
    else
    {
        _ndb_MsgCacheFree();
    }

    /* 正常情况不需要free g_pstRes */
    free(pcDirDn);
    return stRet;
}

/*****************************************************************************
    Func Name: _ndb_GetNextKey_Fast
 Date Created: 2016/10/8
       Author: liangjinchao@dian
  Description: 快速获取dir下面的第一个key
        Input:
       Output: 下一个key
       Return:
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
static datum _ndb_GetNextKey_Fast()
{
    datum stRet;
    char *pcDn;
    int iDatSize;

    memset(&stRet, 0, sizeof(stRet));
    g_pstE = ldap_next_entry(g_pstLDAP, g_pstE);
    if(NULL == g_pstE)
    {
        _ndb_MsgCacheFree();
        return stRet;
    }

    pcDn = ldap_get_dn(g_pstLDAP, g_pstE);
    stRet.dptr = _ndb_AllocRdnDataByDn(pcDn, &iDatSize);
    stRet.dsize = iDatSize;
    free(pcDn);

    return stRet;
}

/*****************************************************************************
    Func Name: ndb_ldap_nextkey
 Date Created: 2016/10/8
       Author: liangjinchao@dian
  Description: 获取dir下面的第一个key
        Input: int iDirId, datum stKey
       Output: 下一个key
       Return:
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
datum ndb_ldap_nextkey (int iDirId, datum stKey)
{
    datum stRet;
    char *pcDn, *pcLastDn;

    memset(&stRet, 0, sizeof(stRet));
    if(NDB_SUCCESS != _ndb_CheckLdapInit())
    {
        return stRet;
    }

    pcDn = _ndb_DnName_Alloc(iDirId, &stKey);
    pcLastDn = ldap_get_dn(g_pstLDAP, g_pstE);
    if(0 == strcmp(pcDn, pcLastDn))
    {
        stRet = _ndb_GetNextKey_Fast();
    }
    else
    {
        stRet = _ndb_GetNextKey(iDirId, pcDn);
    }

    free(pcLastDn);
    free(pcDn);

    return stRet;
}

/*****************************************************************************
    Func Name: ndb_ldap_scan_dir
 Date Created: 2016/10/8
       Author: liangjinchao@dian
  Description: 遍历某个DIR下面的所有数据
        Input: int iDirId, NDB_LDAP_SCAN_PF pfCallback, void *pSelfData
       Output:
       Return: 成功/失败
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
int ndb_ldap_scan_dir(int iDirId, NDB_LDAP_SCAN_PF pfCallback, void *pSelfData)
{
    LDAPMessage *pstRes, *pstEntry;
    datum stKey, stValue;
    char *pcDirDn, *pcDn, **ppcStrVals;
    int iRet;

    if(NDB_SUCCESS != _ndb_CheckLdapInit())
    {
        return NDB_FAILED;
    }

    pcDirDn = _ndb_DnName_Alloc(iDirId, NULL);
    if(NULL == pcDirDn)
    {
        return NDB_FAILED;
    }

    iRet = _ldap_TrySearch(g_pstLDAP, pcDirDn, LDAP_SCOPE_ONELEVEL, 0, 0, 0, &pstRes);
    if (LDAP_NO_SUCH_OBJECT == iRet)
    {
        free(pcDirDn);
        return NDB_SUCCESS;
    }
    else if(LDAP_SUCCESS != iRet)
    {
        free(pcDirDn);
        return NDB_FAILED;
    }
    free(pcDirDn);

    for(pstEntry = ldap_first_entry(g_pstLDAP, pstRes); \
        NULL != pstEntry; \
        pstEntry = ldap_next_entry(g_pstLDAP, pstEntry))
    {
        pcDn = ldap_get_dn(g_pstLDAP, pstEntry);
        stKey.dptr = _ndb_AllocRdnDataByDn(pcDn, &stKey.dsize);
        free(pcDn);

        ppcStrVals = ldap_get_values(g_pstLDAP, pstEntry, NDB_ATTR_VALUE);
        if(NULL == ppcStrVals)
        {
            free(stKey.dptr);
            continue;
        }

        stValue.dptr = _ndb_HexStr2Bin_Alloc(ppcStrVals[0], &stValue.dsize);
        ldap_value_free(ppcStrVals);

        if(NDB_SUCCESS != pfCallback(iDirId, stKey, stValue, pSelfData))
        {
            free(stValue.dptr);
            free(stKey.dptr);
            break;
        }

        free(stValue.dptr);
        free(stKey.dptr);
    }

    ldap_msgfree(pstRes);
    return NDB_SUCCESS;
}



