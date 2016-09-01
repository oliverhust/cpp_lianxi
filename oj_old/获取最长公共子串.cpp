#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAXSIZE 8192
int X, Y, Result = 0;
char CommStr[MAXSIZE];

int Max(int i, int j)
{
	return (i > j ? i : j);
}

/* 计算表格中的一个斜线列 方向为左上，返回该斜线列最大值 要求x<len1,y<len2*/
void GetSyncMax(char *str1, char *str2, int x, int y)
{
	int t = 0, i = x, j = y;

	for (; i >= 0 && j >= 0; i--, j--)
	{
		t = (str1[i] == str2[j]) ? (t + 1) : 0;
		if (t > Result)
		{
			Result = t;
			X = i;
			Y = j;
		}
	}

}

char *iQueryMaxCommString(char * pFirstStr, char * pSecondStr)
{
	int i;

	for (i = 0; i < (int)strlen(pFirstStr); i++)
	{
		GetSyncMax(pFirstStr, pSecondStr, i, strlen(pSecondStr) - 1);
	}

	for (i = 0; i < (int)strlen(pSecondStr); i++)
	{
		GetSyncMax(pFirstStr, pSecondStr, strlen(pFirstStr) - 1, i);
	}
	
	/* 把结果复制到数组里去 */
	memcpy(CommStr, pFirstStr + X, Result);
	CommStr[Result] = '\0';

	return CommStr;
}


void main()
{
	char buf1[MAXSIZE], buf2[MAXSIZE];

	scanf_s("%s", buf1, sizeof(buf1));
	scanf_s("%s", buf2, sizeof(buf1));

	printf("%s", iQueryMaxCommString(buf1, buf2));

	(void)getchar();
}
