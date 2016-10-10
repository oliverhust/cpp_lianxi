#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>
#include <string.h>

#define BLKSIZE         16


/* ��ָ������ƶ�����0��,�������255��0 */
static unsigned char _passZeroBytes(char **ppcData, int *piRestSize)
{
    char *pcData = *ppcData;
    unsigned int uiI;

    for(uiI = 0; uiI < *piRestSize && uiI < UCHAR_MAX && 0 == pcData[uiI]; uiI++);

    *ppcData += uiI;
    *piRestSize -= uiI;
    return (unsigned char)uiI;
}

/* ���ص�ָ����������ͷ� */
char *compress(const char *pcInData, int iInSize, int *piOutSize)
{
    char *pcOut, *pcRet, *p = pcInData;
    int iRest = iInSize;

    *piOutSize = 0;
    pcOut = pcRet = malloc((iInSize+BLKSIZE-1)/BLKSIZE*(BLKSIZE+1));
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
    int i, iSize = iInSize;

    for(i = 0; i < iInSize; i += BLKSIZE)
    {
        iSize += (int)(unsigned int)(unsigned char)pcInData[i] - 1;
    }

    return malloc(iSize);
}

/* ���ص�ָ����������ͷ� */
char *uncompress(const char *pcInData, int iInSize, int *piOutSize)
{
    char *pcOut, *pcRet, *p = pcInData;
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
            memcpy(pcOut, pcInData + i; BLKSIZE);
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


