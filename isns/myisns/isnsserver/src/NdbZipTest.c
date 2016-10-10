#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "iSNSNdbZip.h"

/* ���ص�ָ����������ͷ� */
char *compress(const char *pcInData, int iInSize, int *piOutSize);

/* ���ص�ָ����������ͷ� */
char *uncompress(const char *pcInData, int iInSize, int *piOutSize);


/*
�������ܲ���
����ֵ����
ѹ������
Խ����ʲ���
*/

void test_basic_zip()
{
    int testlist[] = {4001, 200, 400};
    int i, oldsize, zipsize, unzipsize;
    char *pcOld, *pcZip, *pcUnzip;

    for(i = 0; i < sizeof(testlist)/sizeof(int); i++)
    {
        oldsize = testlist[i];
        pcOld = malloc(oldsize);
        pcZip = compress(pcOld, oldsize, &zipsize);
        printf("\r\nCompress Rate: %d : %d\r\n", oldsize, zipsize);

        pcUnzip = uncompress(pcZip, zipsize, &unzipsize);
        if(unzipsize != oldsize)
        {
            printf("@@@@@@@@@@@@ Size not equal, old=%d, new=%d !\r\n", oldsize, unzipsize);
        }

        if(0 != memcmp(pcOld, pcUnzip, oldsize))
        {
            printf("@@@@@@@@@@@@ Value Not Equal!\r\n");

        }
        else
        {
            printf("Success compress.\r\n");
        }

        free(pcUnzip);
        free(pcZip);
		free(pcOld);
    }

}


int main()
{
    test_basic_zip();
	getchar();
    return 0;
}




