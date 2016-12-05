#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "iSNSNdbZip.h"

/* 返回的指针需调用者释放 */
char *ndb_compress(const char *pcInData, int iInSize, int *piOutSize);

/* 返回的指针需调用者释放 */
char *ndb_decompress(const char *pcInData, int iInSize, int *piOutSize);


/*
基本功能测试
随机值测试
特殊值测试
压力测试
越界访问测试
*/


void fill_rand_str(char *pcData, int size)
{
    int  i;

    for(i=0; i<size; i++)
    {
        pcData[i] = rand() % 256;
    }
}

void test_compress_speed(int dataCompressTimes)
{
    char *pcDat, *pcNew;
    unsigned int i, iSize, iNew, iNewAll = 0;

    iSize = 1e8;
    pcDat = malloc(iSize);
    fill_rand_str(pcDat, iSize);

    printf("\r\n####### Test Speed of Compress %d MB random data...\r\n", dataCompressTimes * 100);
    int clk = clock();
    for (i = 0; i < dataCompressTimes; i++)
    {
        pcNew = ndb_compress(pcDat, iSize, &iNew);
        if (pcNew == NULL)
        {
            printf("No enough memory.\r\n");
            break;
        }
        free(pcDat);
        pcDat = pcNew;
        iNewAll += iNew;
    }

    clk = clock() - clk;
    printf("Finish! Elapse %d ms, Size: %u --> %u Byte\r\n", clk, iSize*dataCompressTimes, iNewAll);
    free(pcNew);
}

void test_uncompress_speed(int dataCompressTimes)
{
    char *pcDat, *pcNew;
    unsigned int i, iSize, iNew, iNewAll = 0;

    iSize = 1e8;
    pcDat = malloc(iSize);
    fill_rand_str(pcDat, iSize);

    printf("\r\n####### Test Speed of UnCompress %d MB random data...\r\n", dataCompressTimes * 100);
    int clk = clock();
    for (i = 0; i < dataCompressTimes; i++)
    {
        pcNew = ndb_decompress(pcDat, iSize, &iNew);
        if (pcNew == NULL)
        {
            printf("No enough memory.\r\n");
            break;
        }
        free(pcDat);
        pcDat = pcNew;
        iNewAll += iNew;
    }

    clk = clock() - clk;
    printf("Finish! Elapse %d ms, Size: %u --> %u Byte\r\n", clk, iSize*dataCompressTimes, iNewAll);
    free(pcNew);
}

void test_basic_zip()
{
    int testlist[] = {107, 400, 802, 1600, 1e5};
    int i, oldsize, zipsize, unzipsize;
    char *pcOld, *pcZip, *pcUnzip;

    printf("\r\n####### Basic test Start:\r\n");

    for(i = 0; i < sizeof(testlist)/sizeof(int); i++)
    {
        oldsize = testlist[i];
        pcOld = malloc(oldsize);
        if(i == 3)
        {
            fill_rand_str(pcOld, oldsize);
        }

        pcZip = ndb_compress(pcOld, oldsize, &zipsize);
        printf("Compress Rate: %d -> %d\r\n", oldsize, zipsize);


        pcUnzip = ndb_decompress(pcZip, zipsize, &unzipsize);
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
            printf("Success ndb_compress, value check ok.\r\n");
        }

        printf("\r\n");

        free(pcUnzip);
        free(pcZip);
        free(pcOld);
    }

}


int main()
{
	srand(time(NULL));

	test_basic_zip();
	/*
    test_compress_speed(1);
    test_compress_speed(4);
    test_compress_speed(10);
	*/
    test_uncompress_speed(1);
    test_uncompress_speed(4);
    test_uncompress_speed(10);
    getchar();
    return 0;
}




