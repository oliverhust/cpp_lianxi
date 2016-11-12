/***********************************************************************
  Copyright (c) 2001, Nishan Systems, Inc.
  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are
  met:

  - Redistributions of source code must retain the above copyright notice,
    this list of conditions and the following disclaimer.

  - Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.

  - Neither the name of the Nishan Systems, Inc. nor the names of its
    contributors may be used to endorse or promote products derived from
    this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  IMPLIED WARRANTIES OF MERCHANTABILITY, NON-INFRINGEMENT AND FITNESS FOR A
  PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL NISHAN SYSTEMS, INC.
  OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
  OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
  WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
  OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

***********************************************************************/


/*
 * This file contains source code for implementing the
 * main processing loop for the SoIP service task.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <string.h>

#include <sys/basetype.h>
#include <sys/error.h>
#include <sys/list.h>

#include "iSNStypes.h"
#include "iSNSNdb.h"
#include "iSNSNdbLdap.h"

int ndbm_errno = 0;


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
int ndb_init(const NDB_INITIALIZE_S *pstNdbInit)
{
    int iRet, i;

    iRet = ndb_ldap_init(pstNdbInit);
    if(NDB_SUCCESS != iRet)
    {
        for(i = 0; i < NDB_RECONNECT_LDAP_TIMES; i++)
        {
            iRet = ndb_ldap_reconnect();
            if(NDB_SUCCESS == iRet)
            {
                break;
            }
            sleep(1);
        }
    }

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
    ndb_ldap_close();
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
    return ndb_ldap_delete(iDirId, stKey);
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
    return ndb_ldap_firstkey(iDirId);
}

/*****************************************************************************
    Func Name: ndb_nextkey
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
    return ndb_ldap_nextkey(iDirId, stKey);
}

#if NDB_DATA_COMPRESS_LEVEL == 0

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
    return ndb_ldap_fetch(iDirId, stKey);
}

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
    return ndb_ldap_store_sns(iDirId, stKey, stValue, iFlag);
}

#elif NDB_DATA_COMPRESS_LEVEL > 0

char *ndb_compress(const char *pcInData, int iInSize, int *piOutSize);
char *ndb_decompress(const char *pcInData, int iInSize, int *piOutSize);

/*****************************************************************************
    Func Name: ndb_datum_compress
 Date Created: 2016/10/8
       Author: liangjinchao@dian
  Description: 压缩datum
        Input: datum stDatum
       Output: 无
       Return: 压缩的数据
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
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

/*****************************************************************************
    Func Name: ndb_datum_decompress
 Date Created: 2016/10/8
       Author: liangjinchao@dian
  Description: 解压datum
        Input: datum stDatum
       Output: 无
       Return: 解压的datum
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
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
    datum stZipValue;
    stZipValue = ndb_ldap_fetch(iDirId, stKey);
    return ndb_datum_decompress(stZipValue);
}

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

    stZipValue = ndb_datum_compress(stValue);
    if(NULL == stZipValue.dptr)
    {
        return NDB_FAILED;
    }

    iRet = ndb_ldap_store_sns(iDirId, stKey, stZipValue, iFlag);
    free(stZipValue.dptr);

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
