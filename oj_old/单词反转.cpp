#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAXSIZE 8192

#define True    1
#define False   0


int IsPunctuation(char c)
{
	if (',' == c || ' ' == c || '.' == c || '!' == c || '?' == c)
	{
		return True;
	}

	return False;
}
/* ÄæÐò´òÓ¡£¬ÒªÇóx<=y */
void RevPrint(char *str, int x, int y)
{
	int i;
	for (i = y; i >= x; i--)
	{
		putchar(str[i]);
	}
}

void main()
{
	int i, j, n;
	char buf[MAXSIZE];

	fgets(buf, sizeof(buf), stdin);

	n = strlen(buf);
	if (n > 0 && '\n' == buf[n - 1])
	{
		n--;
		buf[n] = '\0';
	}

	for (i = j = 0; IsPunctuation(buf[i]) || isalpha(buf[i]); i = j)
	{
		if (isalpha(buf[i]))
		{
			while (isalpha(buf[j]))
			{
				j++;
			}
			RevPrint(buf, i, j - 1);
		}
		else
		{
			while (IsPunctuation(buf[j]))
			{
				putchar(buf[j]);
				j++;
			}
		}

	}

}