#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

#include "iSNSNdbZip.h"

#if NDB_DATA_COMPRESS_LEVEL == 1

#define NDB_BLKSIZE         16

/* 将指针向后移动到非0处,最多跳过255个0 */
static unsigned char _passZeroBytes(const char **ppcData, int *piRestSize)
{
    const char *pcData = *ppcData;
    unsigned int uiI;

    for(uiI = 0; (int)uiI < *piRestSize && uiI < UCHAR_MAX && 0 == pcData[uiI]; uiI++);

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

#elif NDB_DATA_COMPRESS_LEVEL == 2

/* 转义字符 */
#define NDB_SPECIAL_CHAR                '\xEE'

/* 返回的指针需调用者释放，适合用于很多连续的0的数据压缩 */
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


static char *_MallocUnzipSpace(const char *pcInData, int iInSize)
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

/* 返回的指针需调用者释放 */
char *ndb_decompress(const char *pcInData, int iInSize, int *piOutSize)
{
    char *pcRet, *pcOut;
    int i = 0, iN;

    *piOutSize = 0;
    pcRet = pcOut = _MallocUnzipSpace(pcInData, iInSize);
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
