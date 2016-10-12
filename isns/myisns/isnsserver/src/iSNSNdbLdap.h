#ifndef _ISNSNDBLDAP_H_
#define _ISNSNDBLDAP_H_


/* Modeled after ldap functions */

#define NDB_DIR_NAME_MAX_LEN        31
#define NDB_NON_KEY_MAX_LEN         63

#define NDB_ATTR_NUM_MAX            3
#define NDB_ATTR_KEY                "isnsKey"
#define NDB_ATTR_VALUE              "isnsValue"
#define NDB_OBJCLASS_DATA           "isnsData"

#define NDB_OBJCLASS                "objectclass"
#define NDB_OU_OBJCLASS             "organizationalUnit"
#define NDB_OBJ_OU                  "ou"

/* 对某个目录的遍历的回调函数, 返回非0值就终止遍历 */
typedef int (*NDB_LDAP_SCAN_PF)(int iDirId, datum stKey, datum stValue, void *pSelfData);

int ndb_ldap_init(const char *pcLdapUrl, const char *pcAdminDn, const char *pcPassword, const char *pcBase);
int ndb_ldap_dir_set(int iDirCount);
void ndb_ldap_close();

int ndb_ldap_store_sns (int iDirId, datum stKey, datum stValue, int iFlag);
datum ndb_ldap_fetch (int iDirId, datum stKey);
datum ndb_ldap_fetch_sns (int iDirId, datum stKey, void *pDst);
int ndb_ldap_delete (int iDirId, datum stKey);
datum ndb_ldap_firstkey (int iDirId);
datum ndb_ldap_nextkey (int iDirId, datum stKey);




#endif

