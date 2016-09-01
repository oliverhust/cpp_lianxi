#include <stdio.h>
#include <string.h>

#define MAXLEN 8192

void main()
{
	char buf[MAXLEN + 1];
	int i,n,count;
	char t;

	fgets(buf, sizeof(buf), stdin);

	n = strlen(buf);
	if (n > 0 && '\n' == buf[n - 1])
	{
		n--;
		buf[n] = '\0';
	}
	count = n / 2;

	for (i = 0; i < count; i++)
	{
		t = buf[i];
		buf[i] = buf[n - 1 - i];
		buf[n - 1 - i] = t;
	}

	printf("%s", buf);

}