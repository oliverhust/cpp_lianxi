#include <stdio.h>
#include <string.h>

#define MAXLINEIN 100
#define EACHLINE 8

void main()
{
	char szOut[5000][EACHLINE + 1] = { 0 }, szBuf[MAXLINEIN + 1] = { 0 };
	int i,j,k,input_n,Len,z0;

	scanf_s("%d\n", &input_n, sizeof(input_n));

	for (i = j = 0; input_n > 0 && i < input_n; i++)
	{
		memset(szBuf, 0, sizeof(szBuf));
		fgets(szBuf, sizeof(szBuf), stdin);

		/* ��ĩβ�Ļ��з�תΪ��\0',fgets��ë�� */
		Len = strlen(szBuf);
		if ('\n' == szBuf[Len - 1])
		{
			szBuf[--Len] = '\0';
		}

		/* ������szBuf���Ƶ�szOut */
		for (k = 0; k < (Len + EACHLINE - 1) / EACHLINE; k++, j++)
		{
			memcpy_s(szOut[j], EACHLINE, szBuf + EACHLINE * k, EACHLINE);
			szOut[j][EACHLINE] = '\0';
		}

		/* ���һ�п������'0' */
		for (z0 = Len % 8 ; z0 < EACHLINE; szOut[j - 1][z0++] = '0');

	}
	
	for (i = 0; i < j; i++)
	{
		puts(szOut[i]);
	}

}