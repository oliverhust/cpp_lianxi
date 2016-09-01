#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAXSIZE 4096

int getMax(int i, int j)
{
	return (i > j ? i : j);
}

/* 计算两个字符串头部的共同长度 */
int getCommonHead(char * pFirstStr, char * pSecondStr)
{
	int i = 0;
	while (pFirstStr[i] != '\0' && pSecondStr != '\0')
	{
		if (0 != _strnicmp(pFirstStr + i, pSecondStr + i, 1))
		{
			break;
		}
		i++;
	}

	return i;
}

int getCommonStrLength(char * pFirstStr, char * pSecondStr)
{
	int iLen1, iLen2, iResult;
	int i, j;

	iResult = 0;
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

/* 去掉末尾的回车换行符 */
/*
void ridCrLf(char *pStr)
{
	int n;

	n = strlen(pStr);
	if (n != 0 && '\n' == pStr[n - 1])
	{
		n--;
		pStr[n] = '\0';
	}

	return;
}
*/
void main()
{
	char buf1[MAXSIZE], buf2[MAXSIZE];

	/*
	fgets(buf1, sizeof(buf1), stdin);
	fgets(buf2, sizeof(buf2), stdin);

	ridCrLf(buf1);
	ridCrLf(buf2);
	*/
	scanf_s("%s", buf1, sizeof(buf1));
	scanf_s("%s", buf2, sizeof(buf1));

	printf("%d" ,getCommonStrLength(buf1, buf2));

	(void)getchar();
}

/* 不区分大小写得比较两个字符 */
/*
int charicmp(char c1, char c2)
{
	if (c1 == c2)
	{
		return 0;
	}

	if (isalpha(c1) && isalpha(c2) &&
		(c1 - c2 == 'a' - 'A' || c2 - c1 == 'a' - 'A'))
	{
		return 0;
	}

	return 1;
}
*/