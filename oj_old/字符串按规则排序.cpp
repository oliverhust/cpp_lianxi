#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAXSIZE 8192

void main()
{
	int i, j, k,n;
	char buf[MAXSIZE];

	fgets(buf, sizeof(buf), stdin);

	if ((n = strlen(buf)) > 0 && '\n' == buf[n - 1])
	{
		n--;
		buf[n] = '\0';
	}

	for (i = 0; i < n; i++)
	{
		if (!isalpha(buf[i]))
		{
			continue;
		}
		for (j = n - 1; j > i; j--)
		{
			if (!isalpha(buf[j]))
			{
				continue;
			}

			k = j - 1;
			while (k > i && !isalpha(buf[k]))
			{
				k--;
			}

			if (tolower(buf[k]) > tolower(buf[j]))
			{
				/* Á½Õß½»»» */
				buf[j] ^= buf[k] ^= buf[j] ^= buf[k];
			}

		}
	}

	printf("%s", buf);

}