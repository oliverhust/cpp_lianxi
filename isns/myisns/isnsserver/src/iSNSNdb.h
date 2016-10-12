#ifndef _ISNSNDB_H_
#define _ISNSNDB_H_


/* Modeled after ldap functions */

/* 持久化数据的压缩级别，0为不压缩 */
#define NDB_DATA_COMPRESS_LEVEL             0

#define NDB_SUCCESS                         0
#define NDB_FAILED                          (-1)

/* 数据库中一级目录(ou)的最大数量 */
#define NDB_MAX_DIRS_COUNT                  16

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
void ndb_close ();

int ndb_store_sns (int iDirId, datum stKey, datum stValue, int iFlag);
datum ndb_fetch (int iDirId, datum stKey);
datum ndb_fetch_sns (int iDirId, datum stKey, void *pDst);
int ndb_delete (int iDirId, datum stKey);
datum ndb_firstkey (int iDirId);
datum ndb_nextkey (int iDirId, datum stKey);

/* 返回的指针需调用者释放，适合用于很多连续的0的数据压缩 */
char *ndb_compress(const char *pcInData, int iInSize, int *piOutSize);

/* 返回的指针需调用者释放 */
char *ndb_decompress(const char *pcInData, int iInSize, int *piOutSize);


static inline datum ndb_datum_compress(datum stDatum)
{
    datum stNew;

    if(NULL == stDatum.dptr)
    {
        return stDatum;
    }

    stNew.dptr = ndb_compress(stDatum.dptr, stDatum.dsize, &stNew.dsize);
    return stNew;
}

static inline datum ndb_datum_decompress(datum stDatum)
{
    datum stNew;

    if(NULL == stDatum.dptr)
    {
        return stDatum;
    }

    stNew.dptr = ndb_decompress(stDatum.dptr, stDatum.dsize, &stNew.dsize);
    return stNew;
}




#endif

