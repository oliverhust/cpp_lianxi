/*****************************************************************************
 Copyright (c) 2010, Hangzhou H3C Technologies Co., Ltd. All rights reserved.
------------------------------------------------------------------------------
                               iSNSLdapLib.c

  Project Code: Comware V7
   Module Name: ISNS
  Date Created: 2016-10-29
        Author: liangjinchao@dian
   Description: ��LDAP���ݽ��в������������ݵĲ��ҡ���ӡ�ɾ������������

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
#include "iSNSList.h"
#include "iSNSmsg.h"
#include "iSNSdebug.h"
#include "iSNSdbLdap.h"
#include "iSNSLdapLib.h"

#define ISNS_LDAP_INNER_ERR             0x1000

STATIC LDAP *g_pstLDAP = NULL;
STATIC ISNS_LDAP_INIT_S g_stInitData = { 0 };


/*****************************************************************************
    Func Name: _ldap_ModStringAttr
 Date Created: 2016/11/3
       Author: liangjinchao@dian
  Description: ����/�޸�/ɾ��ldap�����е��ַ���
        Input: LDAPMod **pstAttr,
               const char *pcAttrValue,
               const char *pcAttrName
               INT iModOp
       Output:
       Return: �ɹ�/ʧ��
      Caution: pcAttrValue����ΪNULL
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

    ppcVals[0] = strdup(pcAttrValue);   /* ��ʽ�������ڴ� */
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
    Func Name: _ldap_ModDelAttr
 Date Created: 2016/11/18
       Author: liangjinchao@dians
  Description: ɾ������
        Input: LDAPMod **pstAttr,
               const char *pcAttrName
       Output:
       Return: �ɹ�/ʧ��
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
STATIC ULONG _ldap_ModDelAttr(IN const CHAR *pcAttrName, OUT LDAPMod **ppstAttr)
{
    LDAPMod  *pstMod;
    CHAR *pcAttrNameTmp;

    *ppstAttr = NULL;
    pstMod = malloc(sizeof(LDAPMod));
    if(NULL == pstMod)
    {
        return ERROR_FAILED;
    }

    pcAttrNameTmp = strdup(pcAttrName);
    if(NULL == pcAttrNameTmp)
    {
        free(pstMod);
        return ERROR_FAILED;
    }

    memset(pstMod, 0, sizeof(LDAPMod));
    pstMod->mod_op = LDAP_MOD_REPLACE;
    pstMod->mod_values = NULL;
    pstMod->mod_type = pcAttrNameTmp;

    *ppstAttr = pstMod;

    return ERROR_SUCCESS;

}

/*****************************************************************************
    Func Name: _ldap_FreeModStrings
 Date Created: 2016/11/3
       Author: liangjinchao@dian
  Description: stringָ��������ڴ��ͷ�
        Input: char ***pppStrings
       Output:
       Return: ��
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
STATIC VOID _ldap_FreeModStrings(IN CHAR ***pppcStrings)
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
  Description: bValueָ��������ڴ��ͷ�
        Input: struct berval ***pppstData
       Output:
       Return: ��
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
STATIC VOID _ldap_FreeModBvalues(IN struct berval ***pppstData)
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
    Func Name: _ldap_FreeMod
 Date Created: 2016/11/3
       Author: liangjinchao@dian
  Description: MOD�ڴ��ͷ�
        Input: IN LDAPMod **ppstAttrs
       Output:
       Return: ��
      Caution: ����ȷ��*ppstAttrs��ΪNULL
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
STATIC VOID _ldap_FreeMod(IN LDAPMod **ppstAttrs)
{
    LDAPMod *pstAttr = *ppstAttrs;

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
    *ppstAttrs = NULL;

    return ;
}

/*****************************************************************************
    Func Name: _ldap_SaveInitData
 Date Created: 2016/11/3
       Author: liangjinchao@dian
  Description: LDAP��ʼ�����ݱ���
        Input: IN ISNS_LDAP_INIT_S *pstInit
       Output: ��
       Return: �ɹ�/ʧ��
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
  Description: LDAP��ʼ�����ݱ���
        Input: IN ISNS_LDAP_INIT_S *pstInit
       Output: ��
       Return: �ɹ�/ʧ��
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
  Description: �ͷų�ʼ������
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
  Description: �ж��Ƿ��ʼ����
        Input: ��
       Output: ��
       Return: ��
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
        ISNS_ERROR("Ldap is not init or configure.");
        return ERROR_FAILED;
    }

    return ERROR_SUCCESS;
}

/*****************************************************************************
    Func Name: _ldap_IsDnExist
 Date Created: 2016/11/3
       Author: liangjinchao@dian
  Description: �ж�DN�Ƿ����
        Input: IN LDAP *pstLd,
               IN const CHAR *pcDN,
       Output: OUT ULONG *pulErr ��������Ŀ���ΪNULL
       Return: �ɹ�/ʧ��
      Caution: ����BOOL_TRUE��һ�������ҳɹ�
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
STATIC BOOL_T _ldap_IsDnExist(IN LDAP *pstLd, IN const CHAR *pcDN, OUT INT *piErr)
{
    INT iRet;
    LDAPMessage *pstRes;
    BOOL_T bRet;

    iRet = ldap_search_s(pstLd, pcDN, LDAP_SCOPE_BASE, NULL, NULL, 0, &pstRes);
    ldap_msgfree(pstRes);

    if(LDAP_SUCCESS == iRet)
    {
        bRet = BOOL_TRUE;
    }
    else if(LDAP_NO_SUCH_OBJECT == iRet)
    {
        bRet = BOOL_FALSE;
        iRet = LDAP_SUCCESS;
    }
    else
    {
        bRet = BOOL_FALSE;
    }

    if(NULL != piErr)
    {
        *piErr = iRet;
    }

    return bRet;
}

/*****************************************************************************
    Func Name: _ldap_ConnectServer
 Date Created: 2016/11/3
       Author: liangjinchao@dian
  Description: ��LDAP SERVER
        Input: ��
       Output: ��
       Return: ���ӳɹ�ERROR_SUCCESS��ʧ��ERROR_FAILED
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
STATIC ULONG _ldap_ConnectServer()
{
    INT iErr;
    INT iProtocolVersion, i;

    if(NULL == g_stInitData.pcAdminDn || NULL == g_stInitData.pcBase ||
       NULL == g_stInitData.pcLdapUrl ||  NULL == g_stInitData.pcPassword)
    {
        ISNS_ERROR("The ldap setings is not config");
        return ERROR_FAILED;
    }

    iErr = ldap_initialize(&g_pstLDAP, g_stInitData.pcLdapUrl);
    if (LDAP_SUCCESS != iErr)
    {
        ISNS_ERROR("Ldap initialize error(%d): %s", iErr, ldap_err2string(iErr));
        return ERROR_FAILED;
    }

    iProtocolVersion = LDAP_VERSION3;
    iErr = ldap_set_option(g_pstLDAP, LDAP_OPT_PROTOCOL_VERSION, &iProtocolVersion);
    if (LDAP_SUCCESS != iErr)
    {
        ISNS_ERROR("Ldap set option error(%d): %s", iErr, ldap_err2string(iErr));
        return ERROR_FAILED;
    }

    for(i = 0; i < ISNS_LDAP_INIT_TRY_MAX; i++)
    {
        iErr = ldap_simple_bind_s(g_pstLDAP, g_stInitData.pcAdminDn, g_stInitData.pcPassword);
        if (LDAP_SUCCESS == iErr)
        {
            if(BOOL_TRUE == _ldap_IsDnExist(g_pstLDAP, g_stInitData.pcBase, NULL))
            {
                break;
            }
        }
        iErr = ISNS_LDAP_INNER_ERR;
        sleep(ISNS_LDAP_INIT_TRY_INTERVAL);
    }

    if(LDAP_SUCCESS != iErr)
    {
        ISNS_ERROR("Ldap connect server(%s) error(%d): %s", g_stInitData.pcLdapUrl, iErr, ldap_err2string(iErr));
        return ERROR_FAILED;
    }

    return ERROR_SUCCESS;
}

/*****************************************************************************
    Func Name: _ldap_Reconnect
 Date Created: 2016/11/3
       Author: liangjinchao@dian
  Description: ����LDAP SERVER
        Input: ��
       Output: ��
       Return: ���ӳɹ�ERROR_SUCCESS��ʧ��ERROR_FAILED
      Caution: ������֤ʧ��Ҳ���ܿ���Search�����޷�Add/Del
               ��֮����Search�ɹ���������Add/Del
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
STATIC ULONG _ldap_Reconnect()
{
    INT iErr;
    INT iProtocolVersion;

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

    iErr = ldap_initialize(&g_pstLDAP, g_stInitData.pcLdapUrl);
    if (LDAP_SUCCESS != iErr)
    {
        ISNS_ERROR("Ldap ReInitialize error(%d): %s", iErr, ldap_err2string(iErr));
        return ERROR_FAILED;
    }

    iProtocolVersion = LDAP_VERSION3;
    iErr = ldap_set_option(g_pstLDAP, LDAP_OPT_PROTOCOL_VERSION, &iProtocolVersion);
    if (LDAP_SUCCESS != iErr)
    {
        ISNS_ERROR("Ldap reconnect set option error(%d): %s", iErr, ldap_err2string(iErr));
        return ERROR_FAILED;
    }

    iErr = ldap_simple_bind_s(g_pstLDAP, g_stInitData.pcAdminDn, g_stInitData.pcPassword);
    if (LDAP_SUCCESS != iErr)
    {
        ISNS_ERROR("Ldap reconnect bind error(%d): %s", iErr, ldap_err2string(iErr));
        return ERROR_FAILED;
    }

    if(BOOL_TRUE != _ldap_IsDnExist(g_pstLDAP, g_stInitData.pcBase, &iErr))
    {
        ISNS_ERROR("Ldap reconnect IsDnExist error(%d): %s", iErr, ldap_err2string(iErr));
        return ERROR_FAILED;
    }

    return ERROR_SUCCESS;
}

/*****************************************************************************
    Func Name: _ldap_Mkdir
 Date Created: 2016/11/3
       Author: liangjinchao@dian
  Description: ��base�����Ŀ¼
        Input: const CHAR *pcDirName
       Output:
       Return: �ɹ�/ʧ��
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
STATIC ULONG _ldap_Mkdir(const CHAR *pcDirName)
{
    CHAR szDn[ISNS_LDAP_DN_MAX_SIZE];
    const CHAR *apcName[ISNS_LDAP_ATTR_MAX_SIZE] = { 0 }, *apcValue[ISNS_LDAP_ATTR_MAX_SIZE] = { 0 };
    LDAPMod **ppstMod;
    ULONG ulRet = ERROR_SUCCESS;
    INT iErr;

    scnprintf(szDn, sizeof(szDn), ISNS_LDAP_OBJ_OU"=%s,%s", pcDirName, g_stInitData.pcBase);

    if(BOOL_TRUE == _ldap_IsDnExist(g_pstLDAP, szDn, &iErr))
    {
        return ulRet;  /* ����Ѵ�����ֱ�ӷ��سɹ� */
    }
    else if(LDAP_SUCCESS != iErr)
    {
        ISNS_ERROR("Ldap mkdir check(%s) exist error(%d): %s", pcDirName, iErr, ldap_err2string(iErr));
        return ERROR_FAILED;
    }

    apcName[0] = ISNS_LDAP_OBJCLASS;
    apcName[1] = ISNS_LDAP_OBJ_OU;
    apcValue[0] = ISNS_LDAP_OU_OBJCLASS;
    apcValue[1] = pcDirName;

    ppstMod = ISNS_LDAP_NewSingleAttrs(apcName, apcValue);
    if(NULL == ppstMod)
    {
        return ERROR_FAILED;
    }

    iErr = ldap_add_s(g_pstLDAP, szDn, ppstMod);
    if(LDAP_SUCCESS != iErr)
    {
        ISNS_ERROR("Ldap mkdir (%s) error(%d): %s", pcDirName, iErr, ldap_err2string(iErr));
        ulRet = ERROR_FAILED;
    }

    ISNS_LDAP_FreeAttrs(&ppstMod);

    return ulRet;
}

/*****************************************************************************
    Func Name: _ldap_Mkdirs
 Date Created: 2016/11/3
       Author: liangjinchao@dian
  Description: ��base�����Ŀ¼
        Input: const CHAR *pcDirName
       Output:
       Return: �ɹ�/ʧ��
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
  Description: ���VALUELIST
        Input: IN LDAPMessage *pstEntry, IN const CHAR **ppcNameList
       Output: OUT CHAR ***pppcValuesList
       Return: ��
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
  Description: ���VALUELIST
        Input: IN LDAPMessage *pstEntry, IN const CHAR **ppcNameList
       Output: OUT CHAR ***pppcValuesList
       Return: ��
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
    Func Name: _ldap_RmEmptyAttr
 Date Created: 2016/11/3
       Author: liangjinchao@dian
  Description: ɾ����ֵ����
        Input: IN LDAPMod **ppstAttrs
       Output:
       Return: ��
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
STATIC VOID _ldap_RmEmptyAttr(IN LDAPMod **ppstAttrs)
{
    INT i, iValid = 0;

    for(i = 0; i < ISNS_LDAP_ATTR_MAX_SIZE && NULL != ppstAttrs[i]; i++)
    {
        if(NULL != ppstAttrs[i]->mod_values)
        {
            ppstAttrs[iValid] = ppstAttrs[i];
            iValid++;
        }
        else
        {
            _ldap_FreeMod(&ppstAttrs[i]);
        }
    }

    ppstAttrs[iValid] = NULL;

    return ;
}

/*****************************************************************************
    Func Name: ISNS_LDAP_Num2BitStr
 Date Created: 2016/11/3
       Author: liangjinchao@dian
  Description: ����תLDAP�������ַ���
        Input: IN UINT uiNum,
       Output: OUT CHAR *pcOut
       Return: �ɹ�/ʧ��
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
    BOOL_T bStartPuts = BOOL_FALSE;   /* ����������ͷ��0 */

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
  Description: δת���NAME,VALUE-->ת����RDN
        Input: IN const CHAR *pcAttrName, IN const CHAR *pcAttrValue
       Output:
       Return: RDN ����"xxx=xxx"
      Caution: ������Ҫ�ͷŷ��ص�ָ��
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
    INT iErr;

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

    iErr = ldap_rdn2str((LDAPRDN)&astRdn, &pcRdn, LDAP_DN_FORMAT_LDAPV2);
    if(0 != iErr)
    {
        free(pcName);
        free(pcValue);
        ISNS_ERROR("Ldap escape error(%d): %s, Attr(%s) Value(%s)", iErr, ldap_err2string(iErr), pcAttrName, pcAttrValue);
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
  Description: ����ldap�����е��ַ���(��ֵ����)
        Input: LDAPMod **pstAttr,
               const char *pcAttrValue,
               const char *pcAttrName
               INT iModOp
       Output:
       Return: �ɹ�/ʧ��
      Caution: pcAttrValue����ΪNULL��������Ϊ""
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
ULONG ISNS_LDAP_SetStringAttr(IN const CHAR *pcAttrName, IN const CHAR *pcAttrValue, OUT LDAPMod **ppstAttr)
{
    ULONG ulRet;

    if(pcAttrValue[0] != '\0')
    {
        ulRet = _ldap_ModStringAttr(pcAttrName, pcAttrValue, LDAP_MOD_REPLACE, ppstAttr);
    }
    else
    {
        ulRet = _ldap_ModDelAttr(pcAttrName, ppstAttr);
    }

    return ulRet;
}

/*****************************************************************************
    Func Name: ISNS_LDAP_AddStringAttr
 Date Created: 2016/11/3
       Author: liangjinchao@dian
  Description: ��ldap���������һ���ַ���
        Input: LDAPMod **pstAttr,
               const char *pcAttrValue,
               const char *pcAttrName
       Output:
       Return: �ɹ�/ʧ��
      Caution: pcAttrValue��ΪNULL
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
  Description: ɾ��ldap�����е�һ���ַ���
        Input: IN const CHAR *pcAttrName,
               IN const CHAR *pcAttrValue,
               OUT LDAPMod **ppstAttr
       Output:
       Return: �ɹ�/ʧ��
      Caution: pcAttrValueΪNULL��ɾ��ȫ��ֵ
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
ULONG ISNS_LDAP_DelStringAttr(IN const CHAR *pcAttrName, IN const CHAR *pcAttrValue, OUT LDAPMod **ppstAttr)
{
    ULONG ulRet;

    if(NULL != pcAttrValue)
    {
        ulRet = _ldap_ModStringAttr(pcAttrName, pcAttrValue, LDAP_MOD_DELETE, ppstAttr);
    }
    else
    {
        ulRet = _ldap_ModDelAttr(pcAttrName, ppstAttr);
    }

    return ulRet;
}

/*****************************************************************************
    Func Name: ISNS_LDAP_NewSingleAttrs
 Date Created: 2016/11/3
       Author: liangjinchao@dian
  Description: ���ö����ֵ����
        Input: IN const CHAR **ppcNameList, �ַ���ָ�����飬��NULLָ���β
               IN const CHAR **ppcValueList �ַ���ָ�����飬��NULLָ���β
       Output:
       Return: �ɹ�/ʧ��
      Caution: LDAPMod **�������ҪISNS_LDAP_FreeAttrs�ͷ�
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
   Description : ɾ��PG
         Input : IN const SOIP_Portal_Group_Key *pstKey
        Output : ��
        Return : �ɹ�/ʧ��
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
   Description : ɾ��PG
         Input : IN const SOIP_Portal_Group_Key *pstKey
        Output : ��
        Return : �ɹ�/ʧ��
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
   Description : ɾ��PG
         Input : IN const SOIP_Portal_Group_Key *pstKey
        Output : ��
        Return : �ɹ�/ʧ��
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

    if(ERROR_SUCCESS != ISNS_LDAP_DelStringAttr(pcAttrName, NULL, ppstAttrs))
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
  Description: ����ISNS_LDAP_ATTR_MAX_NUM + 1��ָ��Ŀռ�
        Input:
       Output:
       Return: LDAPMod **ppstAttrs
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
  Description: ����ֵ���ڴ��ͷź�ָ����0
        Input: LDAPMod **ppstAttrs
       Output:
       Return: ��
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
VOID ISNS_LDAP_FreeAttrs(IN LDAPMod ***pppstAttrs)
{
    LDAPMod **ppstAttrs;
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
        _ldap_FreeMod(&ppstAttrs[iAttrNum]);
    }

    free(ppstAttrs);
    *pppstAttrs = NULL;

    return;
}

/*****************************************************************************
    Func Name: ISNS_LDAP_ServerInit
 Date Created: 2016/11/3
       Author: liangjinchao@dian
  Description: LDAPȫ�ֳ�ʼ��
        Input: IN const ISNS_LDAP_INIT_S *pstInit
               CHAR **ppcPreDirs;  Ԥ�Ƚ�����Ŀ¼������NULL��β���ַ���ָ��
       Output: ��
       Return: ��
      Caution: ppcPreDirs ppcRdnNameָ������ݱ�����ȫ�ֵģ�һֱ���ڵ�
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
ULONG ISNS_LDAP_ServerInit(IN const ISNS_LDAP_INIT_S *pstInit,
                           IN const CHAR *const *ppcPreDirs)
{
    /* �����ʼ������ */
    if(ERROR_SUCCESS != _ldap_SaveInitData(pstInit))
    {
        return ERROR_FAILED;
    }

    /* �������� */
    if(ERROR_SUCCESS != _ldap_ConnectServer())
    {
        return ERROR_FAILED;
    }

    /* Ԥ�ȴ���BASE */


    /* Ԥ�ȴ�����Ŀ¼ */
    if(ERROR_SUCCESS != _ldap_Mkdirs(ppcPreDirs))
    {
        ISNS_ERROR("Ldap make pre-dirs failed");
        return ERROR_FAILED;
    }

    return ERROR_SUCCESS;
}

/*****************************************************************************
    Func Name: ISNS_LDAP_ServerFini
 Date Created: 2016/11/3
       Author: liangjinchao@dian
  Description: LDAPȫ��ȥ��ʼ��
        Input: ��
       Output: OUT LDAP **pstLd
       Return: ��
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
  Description: ���ø��ǣ�������������½�
        Input: IN const CHAR *pcDn, OUT ULONG *pulErr
       Output: ��
       Return: ��
      Caution: pulErr����ΪNULL
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
BOOL_T ISNS_LDAP_IsDnExist(IN const CHAR *pcDn, OUT ULONG *pulErr)
{
    INT iErr;
    BOOL_T bRet;

    if(ERROR_SUCCESS != _ldap_CheckLdapInit())
    {
        *pulErr = ERROR_FAILED;
        return BOOL_FALSE;
    }

    /* �ɹ���ֱ�ӷ��أ������������� */
    bRet = _ldap_IsDnExist(g_pstLDAP, pcDn, &iErr);
    if(LDAP_SUCCESS == iErr)
    {
        *pulErr = ERROR_SUCCESS;
        return bRet;
    }

    if(ERROR_SUCCESS != _ldap_Reconnect())
    {
        *pulErr = ERROR_FAILED;
        return BOOL_FALSE;
    }

    bRet = _ldap_IsDnExist(g_pstLDAP, pcDn, &iErr);
    if(LDAP_SUCCESS != iErr)
    {
        ISNS_ERROR("Ldap IsDnExist error(%d): %s, DN(%s)", iErr, ldap_err2string(iErr), pcDn);
        *pulErr = ERROR_FAILED;
        return bRet;
    }

    *pulErr = ERROR_SUCCESS;
    return bRet;
}

/*****************************************************************************
    Func Name: ISNS_LDAP_AddEntry
 Date Created: 2016/11/3
       Author: liangjinchao@dian
  Description: ���ø��ǣ�������������½�
        Input: IN const CHAR *pcDn, IN LDAPMod **ppstAttrs
       Output: ��
       Return: ��
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
INT ISNS_LDAP_AddEntry(IN const CHAR *pcDn, IN LDAPMod **ppstAttrs)
{
    INT iErr;

    if(ERROR_SUCCESS != _ldap_CheckLdapInit())
    {
        return ERROR_FAILED;
    }

    /* �ɹ���ֱ�ӷ��أ������������� */
    iErr = ldap_add_s(g_pstLDAP, pcDn, ppstAttrs);
    if(LDAP_SUCCESS == iErr)
    {
        return iErr;
    }

    if(ERROR_SUCCESS != _ldap_Reconnect())
    {
        return ERROR_FAILED;
    }

    return ldap_add_s(g_pstLDAP, pcDn, ppstAttrs);
}

/*****************************************************************************
    Func Name: ISNS_LDAP_ModifyEntry
 Date Created: 2016/11/3
       Author: liangjinchao@dian
  Description: �����޸�
        Input: IN const CHAR *pcDn, IN LDAPMod **ppstAttrs
       Output: ��
       Return: ��
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
INT ISNS_LDAP_ModifyEntry(IN const CHAR *pcDn, IN LDAPMod **ppstAttrs)
{
    INT iErr;

    if(ERROR_SUCCESS != _ldap_CheckLdapInit())
    {
        return ISNS_LDAP_INNER_ERR;
    }

    /* �ɹ���ֱ�ӷ��أ������������� */
    iErr = ldap_modify_s(g_pstLDAP, pcDn, ppstAttrs);
    if(LDAP_SUCCESS == iErr)
    {
        return iErr;
    }

    if(ERROR_SUCCESS != _ldap_Reconnect())
    {
        return ISNS_LDAP_INNER_ERR;
    }

    return ldap_modify_s(g_pstLDAP, pcDn, ppstAttrs);
}

/*****************************************************************************
    Func Name: ISNS_LDAP_ReplaceEntry
 Date Created: 2016/11/3
       Author: liangjinchao@dian
  Description: ������Ŀ��ָ�������ԣ������Ŀ���������½�
        Input: IN const CHAR *pcDn, IN LDAPMod **ppstAttrs
       Output: ��
       Return: �ɹ�/ʧ��
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
ULONG ISNS_LDAP_ReplaceEntry(IN const CHAR *pcDn, IN LDAPMod **ppstAttrs)
{
    ULONG ulRet;
    INT iErr;
    BOOL_T bIsExist;

    bIsExist = ISNS_LDAP_IsDnExist(pcDn, &ulRet);
    if(ERROR_SUCCESS != ulRet)
    {
        return ulRet;
    }

    if(BOOL_FALSE == bIsExist)
    {
        _ldap_RmEmptyAttr(ppstAttrs);      /* LDAP�������½���Ŀʱ���ÿ�ֵ���� */
        iErr = ISNS_LDAP_AddEntry(pcDn, ppstAttrs);
    }
    else
    {
        iErr = ISNS_LDAP_ModifyEntry(pcDn, ppstAttrs);
    }

    if(LDAP_SUCCESS != iErr)
    {
        ISNS_ERROR("Ldap replace entry error(%d): %s, IsExist(%d), DN(%s)",
                         iErr, ldap_err2string(iErr), (INT)bIsExist, pcDn);
        return ERROR_FAILED;
    }

    return ERROR_SUCCESS;
}

/*****************************************************************************
    Func Name: ISNS_LDAP_SingleReplace
 Date Created: 2016/11/3
       Author: liangjinchao@dian
  Description: ������Ŀ��ָ���ĵ�ֵ���ԣ������Ŀ���������½�
        Input: IN const CHAR *pcDn,
               IN const CHAR *const *ppcNameList,
               IN const CHAR *const *ppcValueList
       Output: ��
       Return: �ɹ�/ʧ��
      Caution: ĳ��ValueΪNULL��ʾ���Ķ���Ϊ""��ʾ�ÿ�
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
ULONG ISNS_LDAP_SingleReplace(IN const CHAR *pcDn,
                              IN const CHAR *const *ppcNameList,
                              IN const CHAR *const *ppcValueList)
{
    ULONG ulRet;
    LDAPMod **ppstAttrs;

    ppstAttrs = ISNS_LDAP_NewSingleAttrs(ppcNameList, ppcValueList);
    if(NULL == ppstAttrs)
    {
        return ERROR_FAILED;
    }

    ulRet = ISNS_LDAP_ReplaceEntry(pcDn, ppstAttrs);
    ISNS_LDAP_FreeAttrs(&ppstAttrs);

    return ulRet;
}

/*****************************************************************************
    Func Name: ISNS_LDAP_DelEntry
 Date Created: 2016/11/3
       Author: liangjinchao@dian
  Description: ɾ����Ŀ
        Input: IN const CHAR *pcDn
       Output: ��
       Return: ��
      Caution: ɾ�������ڵ���Ȼ���سɹ�
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
ULONG ISNS_LDAP_DelEntry(IN const CHAR *pcDn)
{
    ULONG ulRet = ERROR_SUCCESS;
    INT iErr;

    if(ERROR_SUCCESS != _ldap_CheckLdapInit())
    {
        return ERROR_FAILED;
    }

    /* �ɹ���ֱ�ӷ��أ������������� */
    iErr = ldap_delete_s(g_pstLDAP, pcDn);
    if(LDAP_SUCCESS == iErr || LDAP_NO_SUCH_OBJECT == iErr)
    {
        return ulRet;
    }

    if(ERROR_SUCCESS != _ldap_Reconnect())
    {
        return ERROR_FAILED;
    }

    iErr = ldap_delete_s(g_pstLDAP, pcDn);
    if(LDAP_SUCCESS != iErr && LDAP_NO_SUCH_OBJECT != iErr)
    {
        ISNS_ERROR("Ldap delete error(%d): %s, DN(%s)", iErr, ldap_err2string(iErr), pcDn);
        ulRet = ERROR_FAILED;
    }

    return ulRet;
}

/*****************************************************************************
    Func Name: ISNS_LDAP_SearchEntry
 Date Created: 2016/11/3
       Author: liangjinchao@dian
  Description: ����LDAP���ң����ʧ������������
        Input: const char *pcBase,
               int iScope,
               const char *pcFilter,
               char **ppcAttrs,
               int iAttrsonly,
       Output: OUT LDAPMessage **ppstRes
               OUT LDAP **ppstLdap  ���ܺ�����õ�
       Return: �ɹ�/ʧ��
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
INT ISNS_LDAP_SearchEntry(
    IN const CHAR *pcBase,
    IN INT iScope,
    IN const CHAR *pcFilter,
    IN CHAR **ppcAttrs,
    IN INT iAttrsonly,
    OUT LDAPMessage **ppstRes,
    OUT LDAP **ppstLdap)
{
    INT iErr;

    if(ERROR_SUCCESS != _ldap_CheckLdapInit())
    {
        return ERROR_FAILED;
    }

    iErr = ldap_search_s(g_pstLDAP, pcBase, iScope, pcFilter, ppcAttrs, iAttrsonly, ppstRes);
    if(LDAP_NO_SUCH_OBJECT == iErr || LDAP_SUCCESS == iErr)
    {
        if(NULL != ppstLdap)
        {
            *ppstLdap = g_pstLDAP;
        }
        return iErr;
    }

    if(ERROR_SUCCESS != _ldap_Reconnect())
    {
        return ERROR_FAILED;
    }

    iErr = ldap_search_s(g_pstLDAP, pcBase, iScope, pcFilter, ppcAttrs, iAttrsonly, ppstRes);
    if(NULL != ppstLdap)
    {
        *ppstLdap = g_pstLDAP;
    }

    return iErr;
}

/*****************************************************************************
    Func Name: ISNS_LDAP_ScanDir
 Date Created: 2016/11/3
       Author: liangjinchao@dian
  Description: ����ĳ��Ŀ¼�µ�һ����Ŀ����ȡָ�����Ե�ֵ
        Input: IN const CHAR *pcDirDn,  Ҫ������Ŀ¼��DN
               IN const CHAR **ppcNameList,Ҫ��ȡ������ֵ�ַ���ָ�����飬��NULL��β
               IN ISNS_LDAP_SCAN_PF pfCallback,  �ص�����
               INOUT VOID *pSelfData  �ص�������˽������
       Output: ��
       Return: �ɹ�/ʧ��/�����ڸ�Ŀ¼
      Caution: ���pcDirDn�������򷵻�LDAP_NO_SUCH_OBJECT������������Ϊ�쳣
               ����ص��������ط�0����ֹ����
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
    INT iErr;

    iErr = ISNS_LDAP_SearchEntry(pcDirDn, LDAP_SCOPE_ONELEVEL, 0, 0, 0, &pstRes, NULL);
    if(LDAP_SUCCESS != iErr)
    {
        ISNS_ERROR("Ldap scan dir search error(%d): %s, DN(%s)", iErr, ldap_err2string(iErr), pcDirDn);
        return ERROR_FAILED;
    }

    for(pstEntry = ldap_first_entry(g_pstLDAP, pstRes); \
        NULL != pstEntry; \
        pstEntry = ldap_next_entry(g_pstLDAP, pstEntry))
    {
        memset(appcValuesList, 0, sizeof(appcValuesList));
        if(ERROR_SUCCESS != _ldap_FillValuesList(pstEntry, ppcNameList, appcValuesList))
        {
            /* ����û��ָ�����Ե���Ŀ������ */
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



