#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <string.h>

#include "iSNSNdb.h"
#include "iSNSNdbLdap.h"

int ndbm_errno = 0;

#define NDB_BLKSIZE         16


int ndb_init(const char *pcLdapUrl, const char *pcAdminDn, const char *pcPassword, const char *pcBase)
{
    int iRet;

    iRet = ndb_ldap_init(pcLdapUrl, pcAdminDn, pcPassword, pcBase);
    if(NDB_SUCCESS != iRet)
    {
        return iRet;
    }

    iRet = ndb_ldap_dir_set(NDB_MAX_DIRS_COUNT);

    return iRet;
}


void ndb_close()
{
    ndb_ldap_close();
}

int ndb_store_sns (int iDirId, datum stKey, datum stValue, int iFlag)
{
    datum stZipKey, stZipValue;
    int iRet;

    stZipKey = ndb_datum_compress(stKey);
    if(NULL == stZipKey.dptr)
    {
        return NDB_FAILED;
    }

    stZipValue = ndb_datum_compress(stValue);
    if(NULL == stZipValue.dptr)
    {
        free(stZipKey.dptr);
        return NDB_FAILED;
    }

    iRet = ndb_ldap_store_sns(iDirId, stZipKey, stZipValue, iFlag);

    free(stZipKey.dptr);
    free(stZipValue.dptr);
    return iRet;
}


datum ndb_fetch (int iDirId, datum stKey)
{
    datum stZipKey, stZipValue, stValue;

    stZipKey = ndb_datum_compress(stKey);
    if(NULL == stZipKey.dptr)
    {
        return stZipKey;
    }

    stZipValue = ndb_ldap_fetch(iDirId, stZipKey);
    stValue = ndb_datum_decompress(stZipValue);

    if(NULL != stZipValue.dptr)
    {
        free(stZipValue.dptr);
    }
    free(stZipKey.dptr);
    return stValue;
}


datum ndb_fetch_sns (int iDirId, datum stKey, void *pDst)
{
    datum stZipKey, stZipValue, stValue;

    stZipKey = ndb_datum_compress(stKey);
    if(NULL == stZipKey.dptr)
    {
        return stZipKey;
    }

    stZipValue = ndb_ldap_fetch_sns(iDirId, stZipKey, pDst);
    free(stZipKey.dptr);

    stValue = ndb_datum_decompress(stZipValue);
    if(NULL != stZipValue.dptr)
    {
        free(stZipValue.dptr);
    }
    return stValue;
}


int ndb_delete (int iDirId, datum stKey)
{
    datum stZipKey;
    int iRet;

    stZipKey = ndb_datum_compress(stKey);
    if(NULL == stZipKey.dptr)
    {
        return NDB_FAILED;
    }

    iRet = ndb_ldap_delete(iDirId, stZipKey);

    free(stZipKey.dptr);
    return iRet;

}


datum ndb_firstkey (int iDirId)
{
    datum stKey, stZipKey;

    stZipKey = ndb_ldap_firstkey(iDirId);
    stKey = ndb_datum_decompress(stZipKey);

    if(NULL != stZipKey.dptr)
    {
        free(stZipKey.dptr);
    }
    return stKey;
}


datum ndb_nextkey (int iDirId, datum stKey)
{
    datum stNextKey, stZipKey, stZipNextKey;

    stZipKey = ndb_datum_compress(stKey);
    stZipNextKey = ndb_ldap_nextkey(iDirId, stZipKey);
    stNextKey = ndb_datum_decompress(stZipNextKey);

    if(NULL != stZipKey.dptr)
    {
        free(stZipKey.dptr);
    }
    if(NULL != stZipNextKey.dptr)
    {
        free(stZipNextKey.dptr);
    }

    return stNextKey;
}


/* 将指针向后移动到非0处,最多跳过255个0 */
static unsigned char _passZeroBytes(const char **ppcData, int *piRestSize)
{
    const char *pcData = *ppcData;
    unsigned int uiI;

    for(uiI = 0; uiI < (unsigned char)*piRestSize && uiI < UCHAR_MAX && 0 == pcData[uiI]; uiI++);

    *ppcData += uiI;
    *piRestSize -= uiI;
    return (unsigned char)uiI;
}

/* 返回的指针需调用者释放，适合用于很多连续的0的数据压缩 */
char *ndb_compress(const char *pcInData, int iInSize, int *piOutSize)
{
    const char *p = pcInData;
    char *pcOut, *pcRet;
    int iRest = iInSize;

    *piOutSize = 0;
    pcOut = pcRet = malloc(iInSize + iInSize / NDB_BLKSIZE + 1);
    if(NULL == pcRet)
    {
        return pcRet;
    }

    while(1)
    {
        *pcOut++ = _passZeroBytes(&p, &iRest);
        if(iRest > NDB_BLKSIZE)
        {
            memcpy(pcOut, p, NDB_BLKSIZE);
            pcOut += NDB_BLKSIZE;
            p += NDB_BLKSIZE;
            iRest -= NDB_BLKSIZE;
        }
        else
        {
            memcpy(pcOut, p, iRest);
            pcOut += iRest;
            break;
        }
    }

    *piOutSize = pcOut - pcRet;
    return pcRet;
}

/* 申请内存用来后面存放解压的数据 */
static char *_MallocUnzipSpace(const char *pcInData, int iInSize)
{
    int i, iSize = iInSize + 1;

    for(i = 0; i < iInSize; i += NDB_BLKSIZE + 1)
    {
        iSize += (int)(unsigned int)(unsigned char)pcInData[i] - 1;
    }

    return malloc(iSize);
}

/* 返回的指针需调用者释放 */
char *ndb_decompress(const char *pcInData, int iInSize, int *piOutSize)
{
    char *pcOut, *pcRet;
    int iN, i = 0;

    *piOutSize = 0;
    pcOut = pcRet = _MallocUnzipSpace(pcInData, iInSize);
    if(NULL == pcRet)
    {
        return pcRet;
    }

    while(i < iInSize)
    {
        iN = (int)(unsigned int)(unsigned char)pcInData[i];
        memset(pcOut, 0, iN);
        pcOut += iN;
        i++;

        if(i + NDB_BLKSIZE < iInSize)
        {
            memcpy(pcOut, pcInData + i, NDB_BLKSIZE);
            pcOut += NDB_BLKSIZE;
            i += NDB_BLKSIZE;
        }
        else
        {
            memcpy(pcOut, pcInData + i, iInSize - i);
            pcOut += iInSize - i;
            break;
        }
    }

    *piOutSize = pcOut - pcRet;
    return pcRet;
}



