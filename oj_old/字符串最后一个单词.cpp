#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAXSIZE 129

void main()
{
	int i,n, count = 0;
	char buf[MAXSIZE];

	fgets(buf, sizeof(buf), stdin);

	/* ȥ��fgetsĩβ����\n */
	n = strlen(buf);
	if (n > 0 && '\n' == buf[n])
	{
		n--;
		buf[n] = '\0';
	}

	/* �����һ���ַ���ʼ�� */
	for (n--; n >= 0 && !isalpha(buf[n]); n--);

	for (i = 0; n > 0 && i <= n; i++)
	{
		if (isalpha(buf[i]))
		{
			count++;
		}
		else
		{
			count = 0;
		}
	}

	printf("%d", count);

	(void)getchar();
}