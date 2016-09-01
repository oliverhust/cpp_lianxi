#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXSIZE 8192

int Comp(const void *a, const void *b)
{
	return *(char*)a - *(char*)b;
}

int RevComp(const void *a, const void *b)
{
	return *(char*)b - *(char*)a;
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

	n = strlen(buf);

	qsort(buf, n / 2, sizeof(char), Comp);
	qsort(buf + n - n / 2, n / 2, sizeof(char), RevComp);

	printf("%s", buf);

	(void)getchar();


}