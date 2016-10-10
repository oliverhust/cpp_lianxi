#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

#define BLKSIZE         16


void stop(const char *pcStr)
{
	//printf("Stop at: %s\r\n", pcStr);
	//getchar();
}

#define stop(x) 

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

/* 返回的指针需调用者释放 */
char *compress(const char *pcInData, int iInSize, int *piOutSize)
{
	const char *p = pcInData;
	char *pcOut, *pcRet;
    int iRest = iInSize, iMalloc;

	printf("pcInData=%p, iInSize=%d, piOutSize=%p\r\n", pcInData, iInSize, piOutSize);

    *piOutSize = 0;
	iMalloc = iInSize + iInSize / 16 + 1;
	pcOut = pcRet = malloc(iMalloc);
    if(NULL == pcRet)
    {
        return pcRet;
    }

	printf("pcOut=%p, malloc %d\r\n", pcOut, iMalloc);
	stop("after malloc");

    while(1)
    {		
		*pcOut++ = _passZeroBytes(&p, &iRest);
		stop("after pass zero");
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
			stop("after final cp");
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

    for(i = 0; i < iInSize; i += BLKSIZE)
    {
        iSize += (int)(unsigned int)(unsigned char)pcInData[i] - 1;
    }

    return malloc(iSize);
}

/* 返回的指针需调用者释放 */
char *uncompress(const char *pcInData, int iInSize, int *piOutSize)
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


