#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <string.h>

#include "iSNStypes.h"
#include "iSNSNdb.h"
#include "iSNShash.h"
#include "iSNSNdbLdap.h"

int ndbm_errno = 0;

static int _ndb_recover_data(int iDirId, datum stKey, datum stValue, void *pSelfData);

int ndb_init(const char *pcLdapUrl, const char *pcAdminDn, const char *pcPassword, const char *pcBase)
{
    int iRet, iDir;

    iRet = ndb_hash_open();
    if(NDB_SUCCESS != iRet)
    {
        return iRet;
    }

    iRet = ndb_ldap_init(pcLdapUrl, pcAdminDn, pcPassword, pcBase);
    if(NDB_SUCCESS != iRet)
    {
        return iRet;
    }

    iRet = ndb_ldap_dir_set(NDB_MAX_DIRS_COUNT);

    /* ���ݻָ����ڴ��� */
    for(iDir = 0; iDir < NDB_MAX_DIRS_COUNT; iDir++)
    {
        iRet |= ndb_ldap_scan_dir(iDir, _ndb_recover_data, NULL);
    }

    return iRet;
}

void ndb_close()
{
    ndb_hash_close();
    ndb_ldap_close();
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
    (void)ndb_hash_delete(iDirId, stKey);
    return ndb_ldap_delete(iDirId, stKey);
}


datum ndb_firstkey (int iDirId)
{
    return ndb_hash_firstkey(iDirId);
}


datum ndb_nextkey (int iDirId, datum stKey)
{
    return ndb_hash_nextkey(iDirId, stKey);
}


datum ndb_fetch (int iDirId, datum stKey)
{
    return ndb_hash_fetch(iDirId, stKey);
}


#if NDB_DATA_COMPRESS_LEVEL == 0

int ndb_store_sns (int iDirId, datum stKey, datum stValue, int iFlag)
{
    int iRet;

    iRet = ndb_hash_store(iDirId, stKey, stValue);
    iRet |= ndb_ldap_store_sns(iDirId, stKey, stValue, iFlag);
    return iRet;
}

static int _ndb_recover_data(int iDirId, datum stKey, datum stValue, void *pSelfData)
{
    return ndb_hash_store(iDirId, stKey, stValue);
}

#elif NDB_DATA_COMPRESS_LEVEL > 0

int ndb_store_sns (int iDirId, datum stKey, datum stValue, int iFlag)
{
    datum stZipValue;
    int iRet;

    iRet = ndb_hash_store(iDirId, stKey, stValue);

    stZipValue = ndb_datum_compress(stValue);
    if(NULL == stZipValue.dptr)
    {
        return NDB_FAILED;
    }

    iRet |= ndb_ldap_store_sns(iDirId, stKey, stZipValue, iFlag);
    free(stZipValue.dptr);

    return iRet;
}

static int _ndb_recover_data(int iDirId, datum stKey, datum stValue, void *pSelfData)
{
    datum stUnZipValue;
    int iRet = NDB_FAILED;

    stUnZipValue = ndb_datum_decompress(stValue);
    if(NULL != stUnZipValue.dptr)
    {
        iRet = ndb_hash_store(iDirId, stKey, stUnZipValue);
        free(stUnZipValue.dptr);
    }
    return iRet;
}

#endif


#if NDB_DATA_COMPRESS_LEVEL == 1

#define NDB_BLKSIZE             16

/* ��ָ������ƶ�����0��,�������255��0 */
static unsigned char _passZeroBytes(const char **ppcData, int *piRestSize)
{
    const char *pcData = *ppcData;
    unsigned int uiI;

    for(uiI = 0; (int)uiI < *piRestSize && uiI < UCHAR_MAX && 0 == pcData[uiI]; uiI++);

    *ppcData += uiI;
    *piRestSize -= uiI;
    return (unsigned char)uiI;
}

/* ���ص�ָ����������ͷţ��ʺ����ںܶ�������0������ѹ�� */
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

/* �����ڴ����������Ž�ѹ������ */
static char *_mallocUnzipSpace(const char *pcInData, int iInSize)
{
    int i, iSize = iInSize + 1;

    for(i = 0; i < iInSize; i += NDB_BLKSIZE + 1)
    {
        iSize += (int)(unsigned int)(unsigned char)pcInData[i] - 1;
    }

    return malloc(iSize);
}

/* ���ص�ָ����������ͷ� */
char *ndb_decompress(const char *pcInData, int iInSize, int *piOutSize)
{
    char *pcOut, *pcRet;
    int iN, i = 0;

    *piOutSize = 0;
    pcOut = pcRet = _mallocUnzipSpace(pcInData, iInSize);
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

#elif NDB_DATA_COMPRESS_LEVEL == 2

/* ת���ַ� */
#define NDB_SPECIAL_CHAR                '\xEE'

/* ���ص�ָ����������ͷţ��ʺ����ںܶ�������0������ѹ�� */
char *ndb_compress(const char *pcInData, int iInSize, int *piOutSize)
{
    char *pcOut, *pcRet;
    int i = 0, iN;

    *piOutSize = 0;
    pcRet = pcOut = malloc(2 * iInSize);
    if(NULL == pcRet)
    {
        return pcRet;
    }

    while(i < iInSize)
    {
        if(NDB_SPECIAL_CHAR == pcInData[i])
        {
            *pcOut++ = NDB_SPECIAL_CHAR;
            *pcOut++ = 0;
            i++;
        }
        else if(0 == pcInData[i] && i+1 < iInSize && 0 == pcInData[i+1])
        {
            i += 2;
            for(iN=2; i<iInSize && 0==pcInData[i] && iN < UCHAR_MAX; iN++,i++);
            *pcOut++ = NDB_SPECIAL_CHAR;
            *pcOut++ = (char)iN;
        }
        else
        {
            *pcOut++ = pcInData[i++];
        }
    }

    *piOutSize = pcOut - pcRet;
    return pcRet;
}

static char *_mallocUnzipSpace(const char *pcInData, int iInSize)
{
    int iSize = iInSize;
    const char *pcPos = pcInData - 1, *pcEnd = pcInData + iInSize - 1;

    while(++pcPos <= pcEnd)
    {
        if(*pcPos == NDB_SPECIAL_CHAR && pcPos < pcEnd)
        {
            iSize += (unsigned char)(*++pcPos);
        }
    }

    return malloc(iSize);
}

/* ���ص�ָ����������ͷ� */
char *ndb_decompress(const char *pcInData, int iInSize, int *piOutSize)
{
    char *pcRet, *pcOut;
    int i = 0, iN;

    *piOutSize = 0;
    pcRet = pcOut = _mallocUnzipSpace(pcInData, iInSize);
    if(NULL == pcRet)
    {
        return pcRet;
    }

    while(i < iInSize)
    {
        if(NDB_SPECIAL_CHAR != pcInData[i])
        {
            *pcOut++ = pcInData[i++];
        }
        else if(i + 1 < iInSize && 2 <= (iN=(unsigned char)pcInData[i+1]))
        {
            memset(pcOut, 0, iN);
            pcOut += iN;
            i += 2;
        }
        else
        {
            *pcOut++ = NDB_SPECIAL_CHAR;
            i += 2;
        }
    }

    *piOutSize = pcOut - pcRet;
    return pcRet;
}

#endif
