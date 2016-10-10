#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>
#include <string.h>


#define BLKSIZE         16

/* 将指针向后移动到非0处,最多跳过255个0 */
static unsigned char _passZeroBytes(char **ppcData, int *piRestSize)
{
    char *pcData = *ppcData;
    unsigned int uiI;

    for(uiI = 0; uiI < *piRestSize && uiI < UCHAR_MAX && 0 == pcData[uiI]; uiI++);

    *ppcData += uiI;
    *piRestSize -= uiI;
    return (unsigned char)uiI;
}

/* 返回的指针需调用者释放 */
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


/* 返回的指针需调用者释放 */
char *uncompress(const char *pcInData, int iInSize, int *piOutSize)
{


}


