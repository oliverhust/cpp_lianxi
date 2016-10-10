#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

#define BLKSIZE         16


/* ��ָ������ƶ�����0��,�������255��0 */
static unsigned char _passZeroBytes(const char **ppcData, int *piRestSize)
{
    const char *pcData = *ppcData;
    unsigned int uiI;

    for(uiI = 0; uiI < (unsigned char)*piRestSize && uiI < UCHAR_MAX && 0 == pcData[uiI]; uiI++);

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
    pcOut = pcRet = malloc(iInSize + iInSize / BLKSIZE + 1);
    if(NULL == pcRet)
    {
        return pcRet;
    }

    while(1)
    {
        *pcOut++ = _passZeroBytes(&p, &iRest);
        if(iRest > BLKSIZE)
        {
            memcpy(pcOut, p, BLKSIZE);
            pcOut += BLKSIZE;
            p += BLKSIZE;
            iRest -= BLKSIZE;
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
static char *_MallocUnzipSpace(const char *pcInData, int iInSize)
{
    int i, iSize = iInSize + 1;

    for(i = 0; i < iInSize; i += BLKSIZE + 1)
    {
        iSize += (int)(unsigned int)(unsigned char)pcInData[i] - 1;
    }

    return malloc(iSize);
}

/* ���ص�ָ����������ͷ� */
char *ndb_decompress(const char *pcInData, int iInSize, int *piOutSize)
{
    const char *p = pcInData;
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

        if(i + BLKSIZE < iInSize)
        {
            memcpy(pcOut, pcInData + i, BLKSIZE);
            pcOut += BLKSIZE;
            i += BLKSIZE;
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


