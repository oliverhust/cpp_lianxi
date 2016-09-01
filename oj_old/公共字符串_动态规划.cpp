#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAXSIZE 32768

int Max(int i, int j)
{
	return (i > j ? i : j);
}

/* 计算表格中的一个斜线列 方向为左上，返回该斜线列最大值 要求x<len1,y<len2*/
int GetSyncMax(char *str1, char *str2, int x, int y)
{
	int Result = 0, t = 0, i = x, j = y;

	for (;i >= 0 && j >= 0;i--, j--)
	{
		t = (0 == _strnicmp(str1 + i, str2 + j, 1)) ? (t + 1) : 0;
		Result = Max(t, Result);
	}

	return Result;
}

int getCommonStrLength(char * pFirstStr, char * pSecondStr)
{
	int i,Result = 0;

	for (i = 0; i < (int)strlen(pFirstStr); i++)
	{
		Result = Max(GetSyncMax(pFirstStr, pSecondStr, i, strlen(pSecondStr) - 1), Result);
	}

	for (i = 0; i < (int)strlen(pSecondStr); i++)
	{
		Result = Max(GetSyncMax(pFirstStr, pSecondStr, strlen(pFirstStr) - 1, i), Result);
	}

	return Result;

}

void main()
{
	char buf1[MAXSIZE], buf2[MAXSIZE];

	scanf_s("%s", buf1, sizeof(buf1));
	scanf_s("%s", buf2, sizeof(buf1));

	printf("%d", getCommonStrLength(buf1, buf2));

	(void)getchar();
}
