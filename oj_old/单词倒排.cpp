#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAXSIZE 8192
int times = 0;

/* ÍùÏÔÊ¾»º³åÇø´òÓ¡ x<=y */
void PrintPos(char *buf, int x, int y)
{
	int i;

	if (0 != times)
	{
		putchar(' ');
	}

	times++;

	for (i = x; i <= y; i++)
	{
		putchar(buf[i]);
	}

}

void WordReverse(char *buf, int len)
{
	int i , n = 0;

	for (i = len - 1; i >= 0; i--)
	{
		if (isalpha(buf[i]))
		{
			n++;
		}
		
		if (0 != n && (!isalpha(buf[i]) || 0 == i))
		{
			if (n > 20)
			{
				n = 20;
			}

			if (!isalpha(buf[i]))
			{
				i++;
			}

			PrintPos(buf, i, i + n - 1);
			n = 0;
		}

	}
}

void main()
{
	int n;
	char buf[MAXSIZE];

	fgets(buf, sizeof(buf), stdin);

	n = strlen(buf);
	if (n > 0 && '\n' == buf[n - 1])
	{
		n--;
		buf[n] = '\0';
	}

	WordReverse(buf, n);


}