#ifndef _ISNSCOMPRESS_H_
#define _ISNSCOMPRESS_H_


/* 返回的指针需调用者释放 */
char *compress(const char *pcInData, int iInSize, int *piOutSize);

/* 返回的指针需调用者释放 */
char *uncompress(const char *pcInData, int iInSize, int *piOutSize);


#endif

