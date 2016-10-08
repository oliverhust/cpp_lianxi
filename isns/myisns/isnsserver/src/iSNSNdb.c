#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/basetype.h>
#include <sys/error.h>
#include <ldap.h>


static LDAP *g_pstLDAP = NULL;



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
ULONG ndb_init(const char *pcLdapUrl, const char *pcAdminDn, const char *pcPassword)
{
    int   iRet;
    int   iProtocol_Version;

    iRet = ldap_initialize(&g_pstLDAP, pcLdapUrl);
    if (ERROR_SUCCESS != iRet)
    {
        return iRet;
    }

    iProtocol_Version = LDAP_VERSION3;
    iRet = ldap_set_option(g_pstLDAP, LDAP_OPT_PROTOCOL_VERSION, &iProtocol_Version);
    if (ERROR_SUCCESS != iRet)
    {
        return iRet;
    }

    iRet = ldap_simple_bind_s(g_pstLDAP, pcAdminDn, pcPassword);
    if (ERROR_SUCCESS != iRet)
    {
        return iRet;
    }

    return ERROR_SUCCESS;

}

/*****************************************************************************
    Func Name: ndb_open
 Date Created: 2016/10/8
       Author: liangjinchao@dian
  Description: 如果不存在数据节点则新建
        Input:
       Output:
       Return:
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
NDB_FILE ndb_open (char *pcDbNodeName, int iBlockSize, int iFlags, void (*pfException)())
{
    IGNORE_PARAM(iBlockSize);
    IGNORE_PARAM(iFlags);
    IGNORE_PARAM(pfException);

add
dasddasdasdasd

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
void ndb_close (NDB_FILE stNdbFile)
{

    ldap_unbind(g_pstLDAP);

    return;
}


/*****************************************************************************
    Func Name: ndb_store
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
int ndb_store (NDB_FILE stFile, datum stKey, datum stValue, int iFlag)
{



}


datum ndb_fetch __P((uint32_t key_type, datum));
int ndb_delete __P((uint32_t, datum));
datum ndb_firstkey __P((uint32_t key_type));
datum ndb_nextkey __P((uint32_t key_type, datum));

datum ndb_fetch_sns ();
datum ndb_firstkey_isns (uint32_t key_type, char *key);
datum ndb_nextkey_isns (uint32_t key_type, datum key, char *pkey);









