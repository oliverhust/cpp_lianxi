#ifndef _ISNSCOMPRESS_H_
#define _ISNSCOMPRESS_H_

#define NDB_DATA_COMPRESS_LEVEL 2

/* ���ص�ָ����������ͷţ��ʺ����ںܶ�������0������ѹ�� */
char *ndb_compress(const char *pcInData, int iInSize, int *piOutSize);

/* ���ص�ָ����������ͷ� */
char *ndb_decompress(const char *pcInData, int iInSize, int *piOutSize);


#endif

