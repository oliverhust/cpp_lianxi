#ifndef __sys_in_h__
#define __sys_in_h__


#include <netinet/in.h>

#ifndef INET_ADDRSTRLEN
#define INET_ADDRSTRLEN  16        /* for IPv4 dotted_decimal */
#endif

#ifndef INET6_ADDRSTRLEN
#define INET6_ADDRSTRLEN 46        /* for IPv6 hex string */
#endif

typedef struct in_addr INADDR_S;
typedef struct in6_addr IN6ADDR_S;


/* ��ַͨ�ýṹ�д洢�ĵ�ַת���ַ�������󳤶ȣ�����'\0' */
#define INET_ADDR_STR_LEN      INET6_ADDRSTRLEN

/* IPv4��IPv6ʹ�õ�ͨ��IP��ַ�ṹ. ���еĵ�ַ����洢Ϊ������ */
typedef struct tagINET_ADDR
{
    USHORT   usFamily;             /* ��ַЭ����(AF_INET/AF_INET6) */
    USHORT   usReserved;           /* �����ֶ� */
    union
    {
        IN6ADDR_S stIP6Addr;
        INADDR_S  stIP4Addr;
    } un_addr;                      /* IP��ַ�ֶ� */

    #define uIP6_Addr       un_addr.stIP6Addr
    #define uIP4_Addr       un_addr.stIP4Addr
}INET_ADDR_S;

/* ��ַͨ�ýṹ��Ա��غ� */
#define INET_ADDR_FAMILY(pstAddrM)      ((pstAddrM)->usFamily)
#define INET_ADDR_IP4ADDR(pstAddrM)     ((pstAddrM)->uIP4_Addr)
#define INET_ADDR_IP4ADDRUINT(pstAddrM) INET_ADDR_IP4ADDR(pstAddrM).s_addr
#define INET_ADDR_IP6ADDR(pstAddrM)     ((pstAddrM)->uIP6_Addr)
#define INET_ADDR_IP6ADDRUINT(pstAddrM) INET_ADDR_IP6ADDR(pstAddrM).s6_addr32


#endif
