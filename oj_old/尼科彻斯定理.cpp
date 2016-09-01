#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAXSIZE 8192

char buf[MAXSIZE] = { 0 };
int off = 0;

char *GetSequeOddNum(int m)
{
	/*在这里实现功能*/
	int j;
	unsigned long i, z;

	if (m <= 0)
	{
		return NULL;
	}

	z = (unsigned long)m * m * m;
	for (i = 0; i < z; i++)
	{
		if (m * (i + m - 1) == z)
		{
			break;
		}
	}

	for (j = 0; j < m; j++)
	{
		if (0 == j)
		{
			printf("%d", i);
			off += sprintf_s(buf + off, sizeof(buf) - off, "%d", i);
		}
		else
		{
			printf("+%d", i + 2 * j);
			off += sprintf_s(buf + off, sizeof(buf) - off, "+%d", i + 2 * j);
		}
	}

	return buf;
}




void main()
{
	int m;

	scanf_s("%d", &m, sizeof(m));

	GetSequeOddNum(m);

}