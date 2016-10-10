#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ldap.h>

#include "iSNSNdb.h"

#define ISNS_LDAP_SERVER_URL            "ldap://127.0.0.1:38989"
#define ISNS_LDAP_ADMIN_DN              "cn=root,dc=abc,dc=com"
#define ISNS_LDAP_ADMIN_PASS            "123456"
#define ISNS_LDAP_ISNS_BASE             "dc=abc,dc=com"

void init()
{
    int iRet;
    iRet = ndb_init(ISNS_LDAP_SERVER_URL, ISNS_LDAP_ADMIN_DN, ISNS_LDAP_ADMIN_PASS, ISNS_LDAP_ISNS_BASE);
    if(0 != iRet)
    {
        printf("Init Failed!\r\n");
    }

    if(0 != iRet)
    {
        printf("Set dir Failed!\r\n");
    }
}

void fini()
{
    ndb_close();
}

void test_rw()
{
    datum stKey, stValue;
    int iRet;

    #define KEY_LEN 4
    #define VALUE_LEN 26

    stKey.dptr = malloc(KEY_LEN);
    stKey.dsize = KEY_LEN;

    for(int i=0; i < KEY_LEN; i++)
    {
        (stKey.dptr)[i] = 'A' + i % 26;
    }

    stValue.dptr = malloc(VALUE_LEN);
    stValue.dsize = VALUE_LEN;

    for(int i=0; i < VALUE_LEN; i++)
    {
        (stValue.dptr)[i] = 'a' + i % 26;
    }

    iRet = ndb_store_sns(3, stKey, stValue, 0);
    if(0 != iRet)
    {
        printf("Store failed!\r\n");
        return;
    }

    //----------------------------------------------------------

    datum rValue;
    char *pData = malloc(2048);

    rValue = ndb_fetch_sns(3, stKey, pData);
    if(rValue.dptr == NULL)
    {
        printf("Can not fetch!\r\n");
        return;
    }

    if(rValue.dsize != stValue.dsize)
    {
        printf("Data size Error!\r\n");
    }

    if(0 != memcmp(rValue.dptr, stValue.dptr, rValue.dsize))
    {
        printf("Data is not equal!\r\n");
    }

    if(0 != memcmp(pData, stValue.dptr, rValue.dsize))
    {
        printf("Data sns is not equal!\r\n");
    }

    free(pData);

    //-----------------------------------------------------------

    iRet = ndb_delete(3, stKey);
    if(0 != iRet)
    {
        printf("Delete failed!\r\n");
        return;
    }

    rValue = ndb_fetch_sns(3, stKey, pData);
    if(rValue.dptr != NULL)
    {
        printf("Delete but still Exist!\r\n");
    }

    stKey.dptr[0]++;
    iRet = ndb_delete(3, stKey);
    if(0 != iRet)
    {
        printf("Delete None failed!\r\n");
        return;
    }
    stKey.dptr[1]--;
    printf("Test rw success\r\n");
}

#define K1  16
#define V1  32


int _gene(datum k[], datum v[], int count)
{
    int i, j;
    unsigned char tmp;
    int sum = 0;

    for(i = 0; i < count; i++)
    {
        k[i].dptr = malloc(K1 + i);
        k[i].dsize = K1 + i;
        for(j = 0; j < k[i].dsize; j++)
        {
            (k[i].dptr)[j] = tmp = ('A' + j % 12);
            sum += tmp;
        }

        v[i].dptr = malloc(V1 - i);
        v[i].dsize = V1 - i;
        for(j = 0; j < v[i].dsize; j++)
        {
            (v[i].dptr)[j] = tmp = ('H' + j % 12);
            sum += tmp;
        }
    }

    return sum;
}

short _is_gen(datum k[], datum v[], int count, int sum)
{
    int i, j;
    int mysum = 0;

    for(i = 0; i < count; i++)
    {
        for(j = 0; j < k[i].dsize; j++)
        {
            mysum += (k[i].dptr)[j];
        }

        for(j = 0; j < v[i].dsize; j++)
        {
            mysum += (v[i].dptr)[j];
        }
    }

    if(mysum == sum)
    {
        printf("Old sum=%d, new sum=%d\r\n", sum, mysum);
        return 1;
    }

    printf("Old sum=%d, new sum=%d\r\n", sum, mysum);
    return 0;
}


void test_findall()
{
#define GEN_N 3
#define DIR_ID 1
    datum key[GEN_N], value[GEN_N];
    datum key_[GEN_N], value_[GEN_N];
    datum k, v;
    int iRet, checksum, newsum;

    checksum = _gene(key, value, GEN_N);

    for(int i=0; i < GEN_N; i++)
    {
        iRet = ndb_store_sns(DIR_ID, key[i], value[i], 0);
        if(iRet != 0)
        {
            printf("Findall: Store error!\r\n");
        }
    }

    if(!_is_gen(key, value, GEN_N, checksum))
    {
        printf("Selfcheck failed!\r\n");
        return;
    }

    int i = 0;
    for(k = ndb_firstkey(DIR_ID); k.dptr; k = ndb_nextkey(DIR_ID, k), i++)
    {
        v = ndb_fetch(DIR_ID, k);
        key_[i] = k;
        value_[i] = v;
    }

    if(i != GEN_N)
    {
        printf("List Number failed, old=%d,new=%d!\r\n", GEN_N, i);
    }

    if(!_is_gen(key_, value_, GEN_N, checksum))
    {
        printf("Checksum failed\r\n");
    }

    printf("Find All Check success\r\n");

}


int main()
{
    init();
    test_rw();
    test_findall();
    fini();

    return 0;
}


