#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAXSIZE 8192
#define IPMAX 255

void main()
{
	int i = 0;
	unsigned int ip[4] = { 0 };
	unsigned long ulIP;

	i += scanf_s("%u.%u.%u.%u", ip, ip + 1, ip + 2, ip + 3, sizeof(int), sizeof(int), sizeof(int), sizeof(int));
	i += scanf_s("%u", &ulIP, sizeof(ulIP));

	if (i != 5 || ip[0] > IPMAX || ip[1] > IPMAX || ip[2] > IPMAX || ip[3] > IPMAX)
	{
		printf("Error\n", i);
	}
	else
	{
		printf("%u\r\n", ip[0] * (1UL << 24) + ip[1] * (1UL << 16) + ip[2] * (1UL << 8) + ip[3]);

		ip[0] = (ulIP >> 24) & 0xffUL;
		ip[1] = (ulIP >> 16) & 0xffUL;
		ip[2] = (ulIP >> 8) & 0xffUL;
		ip[3] = (ulIP >> 0) & 0xffUL;

		printf("%u.%u.%u.%u", ip[0], ip[1], ip[2], ip[3]);

	}




}