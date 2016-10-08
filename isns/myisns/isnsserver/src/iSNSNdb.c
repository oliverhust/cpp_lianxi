#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ldap.h>

#include "iSNSNdb.h"

int ndbm_errno = 0;
static LDAP *g_pstLDAP = NULL;
static char *g_pcBase = NULL;


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
void ndb_close ()
{
    free(g_pcBase);
    ldap_unbind(g_pstLDAP);

    return;
}

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


static char *_ndb_DnName_Alloc(int iDirId, datum stKey)
{
    char *pcRet, *pcHexStr;
    char szDirName[NDB_DIR_NAME_MAX_LEN + 1];
    int iSize;

    (void)_ndb_DirId2DirName(iDirId, szDirName, sizeof(szDirName));

    iSize = 2*stKey.dsize + NDB_NON_KEY_MAX_LEN + 1;
    pcRet = malloc(pcRet);
    if(NULL == pcRet)
    {
        return pcRet;   
    }

    pcHexStr = _ndb_Bin2HexStr_Alloc(stKey.dptr, stKey.dsize);
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

    pcDn = _ndb_DnName_Alloc(iDirId, stKey);
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

    pcDn = _ndb_DnName_Alloc(iDirId, stKey);
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

}


datum ndb_firstkey (int iDirId)
{

}


datum ndb_nextkey (int iDirId, datum stKey)
{

}











