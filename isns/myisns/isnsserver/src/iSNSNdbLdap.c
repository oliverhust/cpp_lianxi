#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#ifndef LDAP_DEPRECATED
#define LDAP_DEPRECATED 1
#endif
//#include <openldap/ldap.h>
#include <ldap.h>

#include "iSNSNdb.h"
#include "iSNSNdbLdap.h"

static LDAP *g_pstLDAP = NULL;
static char *g_pcBase = NULL;

/* Search Cache */
static LDAPMessage *g_pstRes = NULL;
static LDAPMessage *g_pstE = NULL;


static short _ldap_IsDnExist(LDAP *pstLd, const char *pcDN)
{
    short bRet;
    LDAPMessage *pstRes;

    bRet = 1;
    if(LDAP_SUCCESS != ldap_search_s(pstLd, pcDN, LDAP_SCOPE_BASE, NULL, NULL, 0, &pstRes))
    {
        bRet = 0;
    }

    ldap_msgfree(pstRes);
    return bRet;
}

static int _ldap_set_string_attr(LDAPMod **pstAttr,
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

    *pstAttr = pstMod;

    return NDB_SUCCESS;
}

/* string指针数组的内存释放 */
static void _ldap_ModStrings_free(char ***pppStrings)
{
    char **ppStrings = *pppStrings;
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
    *pppStrings = NULL;
}

/* bValue指针数组的内存释放 */
static void _ldap_ModBvalues_free(struct berval ***pppstData)
{
    struct berval **ppstData = *pppstData;
    int i;

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

static int _ndb_CheckLdapInit()
{
    if(NULL == g_pstLDAP || NULL == g_pcBase)
    {
        return NDB_FAILED;
    }

    return NDB_SUCCESS;
}

/* 输入字符串以%x开头 */
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

/* 返回数据指针，长度 */
static char *_ldap_GetValByDn(LDAP *pstLd, const char *pcDN, int *piDataSize)
{
    LDAPMessage *pstRes, *pstFirst;
    char *pcData = NULL;
    char **ppcStrVals;

    *piDataSize = 0;

    if(LDAP_SUCCESS != ldap_search_s(pstLd, pcDN, LDAP_SCOPE_BASE, NULL, NULL, 0, &pstRes))
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

static int _ndb_DirId2DirName(int iDirId, char *pcDirName, int iSize)
{
    return snprintf(pcDirName, iSize, "%d", iDirId);
}

/* 以%x开头 */
static char *_ndb_Bin2HexStr_Alloc(void *pData, int iByteSize)
{
    char *pcRet, *pcPtr;
    int i, iSize, iOffset = 0;

    iSize = 2 + 2 * iByteSize + 1;
    pcRet = malloc(iSize);
    if(NULL == pcRet)
    {
        return pcRet;
    }

    iOffset += snprintf(pcRet + iOffset, iSize - iOffset, "%s", "%x");

    pcPtr = (char *)pData;
    for(i = 0; i < iByteSize; i++)
    {
        iOffset += snprintf(pcRet + iOffset, iSize - iOffset, "%02X",
                            (unsigned int)(unsigned char)(*pcPtr));
        pcPtr++;
    }

    return pcRet;
}

static char *_ndb_DnName_Alloc(int iDirId, const datum *pstKey)
{
    char *pcRet, *pcHexStr;
    char szDirName[NDB_DIR_NAME_MAX_LEN + 1];
    int iSize;

    (void)_ndb_DirId2DirName(iDirId, szDirName, sizeof(szDirName));

    if(NULL == pstKey)
    {
        pcRet = malloc(NDB_NON_KEY_MAX_LEN + 1);
        snprintf(pcRet, NDB_NON_KEY_MAX_LEN + 1, NDB_OBJ_OU"=%s,%s", szDirName, g_pcBase);
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
             NDB_ATTR_KEY, pcHexStr, szDirName, g_pcBase);

    free(pcHexStr);
    return pcRet;
}

static void _ndb_FreePointer(void **ppPtr)
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
    Func Name: ndb_ldap_open
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
int ndb_ldap_init(const char *pcLdapUrl, const char *pcAdminDn, const char *pcPassword, const char *pcBase)
{
    int   iRet;
    int   iProtocolVersion;

    iRet = ldap_initialize(&g_pstLDAP, pcLdapUrl);
    if (NDB_SUCCESS != iRet)
    {
        return iRet;
    }

    iProtocolVersion = LDAP_VERSION3;
    iRet = ldap_set_option(g_pstLDAP, LDAP_OPT_PROTOCOL_VERSION, &iProtocolVersion);
    if (NDB_SUCCESS != iRet)
    {
        return iRet;
    }

    iRet = ldap_simple_bind_s(g_pstLDAP, pcAdminDn, pcPassword);
    if (NDB_SUCCESS != iRet)
    {
        return iRet;
    }

    g_pcBase = strdup(pcBase);
    if(NULL == g_pcBase)
    {
        return NDB_FAILED;
    }

    return NDB_SUCCESS;

}

static int _ndb_ldap_store_dir(int iDirId)
{
    LDAPMod *apstAttrs[] = { NULL, NULL, NULL };
    char szDirName[NDB_DIR_NAME_MAX_LEN + 1];
    char *pcDirDn;
    int iRet = NDB_SUCCESS;

    pcDirDn = _ndb_DnName_Alloc(iDirId, NULL);
    if(NULL == pcDirDn)
    {
        return NDB_FAILED;
    }

    if(_ldap_IsDnExist(g_pstLDAP, pcDirDn))
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
    Func Name: ndb_ldap_set_dir
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
int ndb_ldap_dir_set (int iDirCount)
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
    _ndb_FreePointer(&g_pcBase);
    ldap_unbind(g_pstLDAP);

    return;
}

static int _ndb_store_sns(const char *pcDn, const char *pcKey, const char *pcValue)
{
    LDAPMod *apstAttrs[NDB_ATTR_NUM_MAX + 1] = { 0 };
    int iRet, i = 0;

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

    if(_ldap_IsDnExist(g_pstLDAP, pcDn))
    {
        iRet = ldap_modify_ext_s(g_pstLDAP, pcDn, apstAttrs, 0, 0);
    }
    else
    {
        iRet = ldap_add_ext_s(g_pstLDAP, pcDn, apstAttrs, 0, 0);
    }

    _ldap_attrs_free(apstAttrs);
    return iRet;
}

/*****************************************************************************
    Func Name: ndb_ldap_store_sns
 Date Created: 2016/10/8
       Author: liangjinchao@dian
  Description: 保存数据
        Input:
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
        Input:
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

datum ndb_ldap_fetch_sns (int iDirId, datum stKey, void *pDst)
{
    datum stValue;

    memset(&stValue, 0, sizeof(stValue));
    stValue = ndb_ldap_fetch(iDirId, stKey);
    memcpy(pDst, stValue.dptr, stValue.dsize);
    return stValue;
}

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
    if(iRet == LDAP_NO_SUCH_OBJECT)
    {
        iRet = NDB_SUCCESS;
    }

    free(pcDn);
    return iRet;
}

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
    if (LDAP_SUCCESS != ldap_search_s(g_pstLDAP, pcDirDn, LDAP_SCOPE_ONELEVEL, 0, 0, 0, &g_pstRes))
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
    if (LDAP_SUCCESS != ldap_search_s(g_pstLDAP, pcDirDn, LDAP_SCOPE_ONELEVEL, 0, 0, 0, &g_pstRes))
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

    iRet = ldap_search_s(g_pstLDAP, pcDirDn, LDAP_SCOPE_ONELEVEL, 0, 0, 0, &pstRes);
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

    for(pstEntry = ldap_first_entry(g_pstLDAP); \
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

        if(NDB_SUCCESS != pfCallback(stKey, stValue, pSelfData))
        {
            free(stValue.dptr);
            free(stKey.dptr);
            break;
        }

        free(stValue.dptr);
        free(stKey.dptr);
    }

    return NDB_SUCCESS;
}



