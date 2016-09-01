#include <stdio.h>
#include <string.h>

#define MAXSIZE 65536

int getMax(int i, int j)
{
	return (i > j ? i : j);
}

/* 计算两个字符串头部的共同长度 */
int getCommonHead(char * pFirstStr, char * pSecondStr)
{
	int i;

	for (i = 0; pFirstStr[i] != '\0' && pSecondStr != '\0' && pFirstStr[i] == pSecondStr[i]; i++);

	return i;
}

int getCommonStrLength(char * pFirstStr, char * pSecondStr)
{
	int iLen1, iLen2, iResult = 0;
	int i, j;

	iLen1 = strlen(pFirstStr);
	iLen2 = strlen(pSecondStr);

	for (i = 0; i < iLen1; i++)
	{
		for (j = 0; j < iLen2; j++)
		{
			iResult = getMax(getCommonHead(pFirstStr + i, pSecondStr + j), iResult);
		}
	}

	return iResult;

}

void reverse(char *buf1, char *buf2, int n)
{
	int i;

	memset(buf2, 0, n + 1);
	for (i = 0; i < n; i++)
	{
		buf2[i] = buf1[n - 1 - i];
	}
}

void main()
{
	char buf1[MAXSIZE] = { 0 }, buf2[MAXSIZE] = { 0 };

	scanf_s("%s", buf1, sizeof(buf1));

	/* 把字符串逆序输出到buf2 */
	reverse(buf1, buf2, strlen(buf1));

	/* 求出两个逆序字符串的最大公共长度 */
	printf("%d", getCommonStrLength(buf1, buf2));

	(void)getchar();
}
