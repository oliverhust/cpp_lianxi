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

    bRet = BOOL_TRUE;
    if(LDAP_SUCCESS != ldap_search_s(pstLd, pcDN, LDAP_SCOPE_BASE, NULL, NULL, 0, &pstRes))
    {
        bRet = BOOL_FALSE;
    }

    ldap_msgfree(pstRes);
    return bRet;
}

static void _ndb_DirId2DirName(int iDirId, char *pcDirName, int iStrSize)
{
    snprintf(pcDirName, iStrSize, "%d", iDirId);
}




static void _ndb_AllocDnbyDatum(int iDirId, datum stKey, char *pcDnOut, int iStrSize)
{
    int iSize;

    iSize = strlen(NDB_ATTR_KEY) + 3 + 2 * stKey.dsize + 1 +  + 1
}


static int _ndb_store_replace()
{
    LDAPMod stMod;
    LDAPMod *apstMod[] = {&stMod, NULL};
    char *apcModVal[] = {NULL, NULL};

    apcModVal[0] = val;

    stMod.mod_op = LDAP_MOD_REPLACE;
    stMod.mod_type = NDB_ATTR_VALUE;
    stMod.mod_values = apcModVal;

    if(LDAP_SUCCESS != ldap_modify_ext_s(ld, "isnsKey=%xABCDEF,ou=DD,dc=abc,dc=com", apstMod, 0, 0))
    {
       ldap_perror(ld, "ldap_modify_ext_s");
       ldap_unbind(ld);
       exit(1);
    }
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


}


datum ndb_fetch (int iDirId, datum stKey)
{


}



datum ndb_fetch_sns (int iDirId, datum stKey, void *pDst)
{


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











