#ifndef _ISNSNDB_H_
#define _ISNSNDB_H_


/* Modeled after ldap functions */

#define NDB_DIR_NAME_MAX_LEN        31
#define NDB_NON_KEY_MAX_LEN         63

#define NDB_ATTR_NUM_MAX                3
#define NDB_ATTR_KEY                "isnsKey"
#define NDB_ATTR_VALUE              "isnsValue"
#define NDB_OBJCLASS_DATA           "isnsData"

#define NDB_OBJCLASS                "objectclass"
#define NDB_OU_OBJCLASS             "organizationalUnit"
#define NDB_OBJ_OU                  "ou"

#define NDB_SUCCESS                 0
#define NDB_FAILED                  (-1)


typedef struct {
    char  *dptr;
    int   dsize;
} datum;

typedef int NDB_FILE;

enum
{
    NDBM_INSERT = 0,
    NDBM_REPLACE = 1,
};


extern int ndbm_errno;

int ndb_init(const char *pcLdapUrl, const char *pcAdminDn, const char *pcPassword, const char *pcBase);
int ndb_dir_set (int iDirCount);
void ndb_close ();

int ndb_store_sns (int iDirId, datum stKey, datum stValue, int iFlag);
datum ndb_fetch (int iDirId, datum stKey);
datum ndb_fetch_sns (int iDirId, datum stKey, void *pDst);
int ndb_delete (int iDirId, datum stKey);
datum ndb_firstkey (int iDirId);
datum ndb_nextkey (int iDirId, datum stKey);




#endif

