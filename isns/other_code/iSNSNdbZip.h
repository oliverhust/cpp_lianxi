#ifndef _ISNSCOMPRESS_H_
#define _ISNSCOMPRESS_H_


/* ���ص�ָ����������ͷţ��ʺ����ںܶ�������0������ѹ�� */
char *ndb_compress(const char *pcInData, int iInSize, int *piOutSize);

/* ���ص�ָ����������ͷ� */
char *ndb_decompress(const char *pcInData, int iInSize, int *piOutSize);


#endif
