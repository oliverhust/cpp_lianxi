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

/*****************************************************************************
    Func Name: ndb_init
 Date Created: 2016/10/8
       Author: liangjinchao@dian
  Description: 数据库初始化
        Input: const char *pcLdapUrl
               const char *pcAdminDn
               const char *pcPassword
               const char *pcBase
       Output: 无
       Return: 成功/失败
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
int ndb_init(const char *pcLdapUrl, const char *pcAdminDn, const char *pcPassword, const char *pcBase)
{
    int iRet, iDir;

    iRet = ndb_hash_open();
    if(NDB_SUCCESS != iRet)
    {
        return iRet;
    }

    /*
    iRet = ndb_ldap_init(pcLdapUrl, pcAdminDn, pcPassword, pcBase);
    if(NDB_SUCCESS != iRet)
    {
        return iRet;
    }

    iRet = ndb_ldap_dir_set(NDB_MAX_DIRS_COUNT);

    // 数据恢复到内存中
    for(iDir = 0; iDir < NDB_MAX_DIRS_COUNT; iDir++)
    {
        iRet |= ndb_ldap_scan_dir(iDir, _ndb_recover_data, NULL);
    }
    */

    return iRet;
}

/*****************************************************************************
    Func Name: ndb_close
 Date Created: 2016/10/8
       Author: liangjinchao@dian
  Description: 关闭数据库
        Input: 无
       Output: 无
       Return: 无
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
void ndb_close()
{
    ndb_hash_close();
    //ndb_ldap_close();
}

/*****************************************************************************
    Func Name: ndb_fetch_sns
 Date Created: 2016/10/8
       Author: liangjinchao@dian
  Description: 获取数据并复制到输出
        Input: int iDirId, datum stKey
       Output: void *pDst
       Return: 获取到的数据
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
datum ndb_fetch_sns (int iDirId, datum stKey, void *pDst)
{
    datum stValue;

    stValue = ndb_fetch(iDirId, stKey);
    memcpy(pDst, stValue.dptr, stValue.dsize);
    return stValue;

}

/*****************************************************************************
    Func Name: ndb_delete
 Date Created: 2016/10/8
       Author: liangjinchao@dian
  Description: 删除数据
        Input: int iDirId, datum stKey
       Output: 无
       Return: 成功/失败
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
int ndb_delete (int iDirId, datum stKey)
{
    return ndb_hash_delete(iDirId, stKey);
}

/*****************************************************************************
    Func Name: ndb_firstkey
 Date Created: 2016/10/8
       Author: liangjinchao@dian
  Description: 获取第一个key
        Input: int iDirId
       Output: 无
       Return: 第一个KEY
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
datum ndb_firstkey (int iDirId)
{
    return ndb_hash_firstkey(iDirId);
}

/*****************************************************************************
    Func Name: ndb_delete
 Date Created: 2016/10/8
       Author: liangjinchao@dian
  Description: 获取下一个KEY
        Input: int iDirId, datum stKey
       Output: 无
       Return: 下一个KEY
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
datum ndb_nextkey (int iDirId, datum stKey)
{
    return ndb_hash_nextkey(iDirId, stKey);
}

/*****************************************************************************
    Func Name: ndb_fetch
 Date Created: 2016/10/8
       Author: liangjinchao@dian
  Description: 获取数据
        Input: int iDirId, datum stKey
       Output:
       Return: 获取到的数据
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
datum ndb_fetch (int iDirId, datum stKey)
{
    return ndb_hash_fetch(iDirId, stKey);
}


#if NDB_DATA_COMPRESS_LEVEL == 0

/*****************************************************************************
    Func Name: ndb_store_sns
 Date Created: 2016/10/8
       Author: liangjinchao@dian
  Description: 保存数据
        Input: int iDirId, datum stKey, datum stValue, int iFlag
       Output:
       Return: 成功/失败
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
int ndb_store_sns (int iDirId, datum stKey, datum stValue, int iFlag)
{
    int iRet;

    iRet = ndb_hash_store(iDirId, stKey, stValue);
    //iRet |= ndb_ldap_store_sns(iDirId, stKey, stValue, iFlag);
    return iRet;
}

/*****************************************************************************
    Func Name: _ndb_recover_data
 Date Created: 2016/10/8
       Author: liangjinchao@dian
  Description: 数据恢复的回调函数
        Input: datum stKey  数据的KEY
               datum stValue 数据的VALUE
               void *pSelfData 私有数据
       Output: 无
       Return: 是否终止遍历
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
static int _ndb_recover_data(int iDirId, datum stKey, datum stValue, void *pSelfData)
{
    return ndb_hash_store(iDirId, stKey, stValue);
}

#elif NDB_DATA_COMPRESS_LEVEL > 0

/*****************************************************************************
    Func Name: ndb_store_sns
 Date Created: 2016/10/8
       Author: liangjinchao@dian
  Description: 保存数据
        Input: int iDirId, datum stKey, datum stValue, int iFlag
       Output:
       Return: 成功/失败
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
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

/*****************************************************************************
    Func Name: _ndb_recover_data
 Date Created: 2016/10/8
       Author: liangjinchao@dian
  Description: 数据恢复的回调函数
        Input: datum stKey  数据的KEY
               datum stValue 数据的VALUE
               void *pSelfData 私有数据
       Output: 无
       Return: 是否终止遍历
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
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

/*****************************************************************************
    Func Name: _passZeroBytes
 Date Created: 2016/10/8
       Author: liangjinchao@dian
  Description: 将指针向后移动到非0处,最多跳过255个0
        Input: const char **ppcData, int *piRestSize
       Output:
       Return: 0的数量
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
static unsigned char _passZeroBytes(const char **ppcData, int *piRestSize)
{
    const char *pcData = *ppcData;
    unsigned int uiI;

    for(uiI = 0; (int)uiI < *piRestSize && uiI < UCHAR_MAX && 0 == pcData[uiI]; uiI++);

    *ppcData += uiI;
    *piRestSize -= uiI;
    return (unsigned char)uiI;
}

/*****************************************************************************
    Func Name: ndb_compress
 Date Created: 2016/10/8
       Author: liangjinchao@dian
  Description: 压缩数据
        Input: const char *pcInData, int iInSize, int *piOutSize
       Output: 压缩的数据大小
       Return: 压缩的数据
      Caution: 返回的指针需调用者释放，适合用于很多连续的0的数据压缩
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
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

/*****************************************************************************
    Func Name: _mallocUnzipSpace
 Date Created: 2016/10/8
       Author: liangjinchao@dian
  Description: 申请内存用来后面存放解压的数据
        Input: const char *pcInData, int iInSize
       Output: 无
       Return: 分配的内存指针
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
static char *_mallocUnzipSpace(const char *pcInData, int iInSize)
{
    int i, iSize = iInSize + 1;

    for(i = 0; i < iInSize; i += NDB_BLKSIZE + 1)
    {
        iSize += (int)(unsigned int)(unsigned char)pcInData[i] - 1;
    }

    return malloc(iSize);
}

/*****************************************************************************
    Func Name: ndb_decompress
 Date Created: 2016/10/8
       Author: liangjinchao@dian
  Description: 解压数据
        Input: const char *pcInData, int iInSize, int *piOutSize
       Output: 解压缩的数据大小
       Return: 解压压缩的数据
      Caution: 返回的指针需调用者释放，适合用于很多连续的0的数据压缩
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
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

/* 转义字符 */
#define NDB_SPECIAL_CHAR                '\xEE'

/*****************************************************************************
    Func Name: ndb_compress
 Date Created: 2016/10/8
       Author: liangjinchao@dian
  Description: 压缩数据
        Input: const char *pcInData, int iInSize, int *piOutSize
       Output: 压缩的数据大小
       Return: 压缩的数据
      Caution: 返回的指针需调用者释放，适合用于很多连续的0的数据压缩
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
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

/*****************************************************************************
    Func Name: _mallocUnzipSpace
 Date Created: 2016/10/8
       Author: liangjinchao@dian
  Description: 申请内存用来后面存放解压的数据
        Input: const char *pcInData, int iInSize
       Output: 无
       Return: 分配的内存指针
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
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

/*****************************************************************************
    Func Name: ndb_decompress
 Date Created: 2016/10/8
       Author: liangjinchao@dian
  Description: 解压数据
        Input: const char *pcInData, int iInSize, int *piOutSize
       Output: 解压缩的数据大小
       Return: 解压压缩的数据
      Caution: 返回的指针需调用者释放，适合用于很多连续的0的数据压缩
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
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
