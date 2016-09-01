#include <stdio.h>
#include <string.h>

#define MAXSIZE 32768

/* 用于记录输入长度 */
int g_Len = 0;

int Max(int i, int j)
{
	return (i > j ? i : j);
}

/* 获取以pcStr[X]为中心的最大对称字符串长度 */
int find1(char *pcStr, int X)
{
	int i = 1;
	
	while (X - i >= 0 && X + i < g_Len && pcStr[X - i] == pcStr[X + i])
	{
		i++;
	}

	return (1 + 2 * (i - 1));
}

/* 获取以pcStr[X]和pcStr[X+1]为中心的最大对称字符串长度 */
int find2(char *pcStr, int X)
{
	int i = 0;

	while (X - i >= 0 && X + 1 + i < g_Len && pcStr[X - i] == pcStr[X + 1 + i])
	{
		i++;
	}

	return (2 * i);
}

void main()
{
	char buf[MAXSIZE] = { 0 };
	int i, result = 0;

	scanf_s("%s", buf, sizeof(buf));

	g_Len = strlen(buf);

	for (i = 0; i < g_Len; i++)
	{
		result = Max(find1(buf, i), result);
		result = Max(find2(buf, i), result);
	}

	printf("%d", result);

	(void)getchar();
}
