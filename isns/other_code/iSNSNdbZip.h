#ifndef _ISNSCOMPRESS_H_
#define _ISNSCOMPRESS_H_

#define NDB_DATA_COMPRESS_LEVEL 1

/* 返回的指针需调用者释放，适合用于很多连续的0的数据压缩 */
char *ndb_compress(const char *pcInData, int iInSize, int *piOutSize);

/* 返回的指针需调用者释放 */
char *ndb_decompress(const char *pcInData, int iInSize, int *piOutSize);


#endif

