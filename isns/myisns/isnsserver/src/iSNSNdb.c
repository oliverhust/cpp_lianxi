#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ldap.h>

#include "iSNSNdb.h"

int ndbm_errno = 0;
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

    return pcPtr;
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


static int _ndb_store_replace(const char *pcDn, datum stValue)
{
    LDAPMod stMod;
    LDAPMod *apstMod[] = {&stMod, NULL};
    char *apcModVal[] = {NULL, NULL};
    int iRet;

    apcModVal[0] = _ndb_Bin2HexStr_Alloc(stValue.dptr, stValue.dsize);

    stMod.mod_op = LDAP_MOD_REPLACE;
    stMod.mod_type = NDB_ATTR_VALUE;
    stMod.mod_values = apcModVal;

    iRet = ldap_modify_ext_s(g_pstLDAP, pcDn, apstMod, 0, 0);
    free(apcModVal[0]);

    return iRet;
}

static int _ndb_store_add(const char *pcDn, datum stValue)
{
    LDAPMod stMod, stObjClass;
    LDAPMod *apstMod[] = {&stMod, &stObjClass, NULL};
    char *apcModVal[] = {NULL, NULL};
    char *apcObjClass[] = { NULL, NULL };
    int iRet;

    apcModVal[0] = _ndb_Bin2HexStr_Alloc(stValue.dptr, stValue.dsize);
    stMod.mod_type = NDB_ATTR_VALUE;
    stMod.mod_values = apcModVal;
    stMod.mod_op = LDAP_MOD_REPLACE;

    apcObjClass[0] = NDB_ATTR_OBJECT;
    stObjClass.mod_type = NDB_OBJCLASS;
    stObjClass.mod_values = apcObjClass;
    stMod.mod_op = LDAP_MOD_REPLACE;

    iRet = ldap_add_ext_s(g_pstLDAP, pcDn, apstMod, 0, 0);
    free(apcModVal[0]);

    return iRet;
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
    Func Name: ndb_open
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
int ndb_init(const char *pcLdapUrl, const char *pcAdminDn, const char *pcPassword, const char *pcBase)
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

/*****************************************************************************
    Func Name: ndb_set_dir
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
int ndb_dir_set (int iDirCount)
{
    return NDB_SUCCESS;
}

/*****************************************************************************
    Func Name: ndb_close
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
void ndb_close()
{
    _ndb_MsgCacheFree();
    _ndb_FreePointer(&g_pcBase);
    ldap_unbind(g_pstLDAP);

    return;
}

/*****************************************************************************
    Func Name: ndb_store_sns
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
int ndb_store_sns (int iDirId, datum stKey, datum stValue, int iFlag)
{
    char *pcDn;
    int iRet;

    IGNORE_PARAM(iFlag);

    pcDn = _ndb_DnName_Alloc(iDirId, &stKey);
    if(NULL == pcDn)
    {
        return NDB_FAILED;
    }

    if(_ldap_IsDnExist(g_pstLDAP, pcDn))
    {
        iRet = _ndb_store_replace(pcDn, stValue);
    }
    else
    {
        iRet = _ndb_store_add(pcDn, stValue);
    }

    free(pcDn);
    return NDB_SUCCESS;
}


datum ndb_fetch (int iDirId, datum stKey)
{
    char *pcDn;
    datum stRet;
    int iDataSize;

    memset(stRet, 0, sizeof(stRet));

    pcDn = _ndb_DnName_Alloc(iDirId, &stKey);
    if(NULL == pcDn)
    {
        return stRet;
    }

    stRet.dptr = _ldap_GetValByDn(g_pstLDAP, pcDn, &iDataSize);
    return stRet;
}

datum ndb_fetch_sns (int iDirId, datum stKey, void *pDst)
{
    datum stValue;

    stValue = ndb_fetch(iDirId, stKey);
    memcpy(pDst, stValue.dptr, stValue.dsize);
    return stValue;
}

int ndb_delete (int iDirId, datum stKey)
{
    char *pcDn;
    int iRet;

    pcDn = _ndb_DnName_Alloc(iDirId, &stKey);
    if(NULL == pcDn)
    {
        return NDB_FAILED;
    }

    iRet = NDB_SUCCESS;
    if(ldap_delete_s(g_pstLDAP, pcDn))
    {
        iRet = NDB_FAILED;
    }

    free(pcDn);
    return iRet;
}

char *_ndb_GetRdnDataByDn(const char *pcDn, int *piDataSize)
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

datum ndb_firstkey (int iDirId)
{
    char *pcDirDn, *pcDn;
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

    pcDn = ldap_get_dn(g_pstLDAP, g_pstE);
    stRet.dptr = _ndb_GetRdnDataByDn(pcDn, &iDatSize);
    stRet.dsize = iDatSize;
    free(pcDn);

    /* 不需要free g_pstRes */
    free(pcDirDn);
    return stRet;
}

datum _ndb_GetNextKey(int iDirId, const char *pcDnInput)
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
        stRet.dptr = _ndb_GetRdnDataByDn(pcDnTmp, &iDatSize);
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

datum _ndb_GetNextKey_Fast()
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
    stRet.dptr = _ndb_GetRdnDataByDn(pcDn, &iDatSize);
    stRet.dsize = iDatSize;
    free(pcDn);

    return stRet;
}

datum ndb_nextkey (int iDirId, datum stKey)
{
    datum stRet;
    char *pcDn, *pcLastDn;

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





