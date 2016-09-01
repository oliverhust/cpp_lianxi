#include <stdio.h>
#include <string.h>

#define MAXSIZE 8192

/* ���ڼ�¼���볤�� */
int g_Len = 0;

int Max(int i, int j)
{
	return (i > j ? i : j);
}

/* ��ȡ��pcStr[X]Ϊ���ĵ����Գ��ַ������� */
int find1(char *pcStr, int X)
{
	int i = 1;
	
	while (X - i >= 0 && X + i < g_Len && pcStr[X - i] == pcStr[X + i])
	{
		i++;
	}

	return (1 + 2 * (i - 1));
}

/* ��ȡ��pcStr[X]��pcStr[X+1]Ϊ���ĵ����Գ��ַ������� */
int find2(char *pcStr, int X)
{
	int i = 0;

	while (X - i >= 0 && X + 1 + i < g_Len && pcStr[X - i] == pcStr[X + 1 + i])
	{
		i++;
	}

	return (2 * i);
}

/*  ���ô˽ӿ�ǰҪ�Ȳ���g_Len  */
void GetCipherMaxLen(char *CipherContent, int *piCipherLen)
{
	int i, result = 0;
	
	for (i = 0; i < g_Len; i++)
	{
		result = Max(find1(CipherContent, i), result);
		result = Max(find2(CipherContent, i), result);
	}

	*piCipherLen = result;
}


void main()
{
	char buf[MAXSIZE] = { 0 };
	int i = 0, j, result[MAXSIZE] = { 0 };

	while (EOF != scanf_s("%s", buf, sizeof(buf)))
	{
		g_Len = strlen(buf);
		GetCipherMaxLen(buf, result + i);
		i++;
	}

	for (j = 0; j < i; j++)
	{
		if (0 == j)
		{
			printf("%d", result[0]);
		}
		else
		{
			printf("\n%d", result[j]);
		}
	}
	
}
