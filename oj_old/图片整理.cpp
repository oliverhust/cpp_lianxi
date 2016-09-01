#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLEN 1024

int Comp(const void *a, const void *b)
{
	return *(char*)a - *(char*)b;
}

void main()
{
	int n;
	char buf[MAXLEN + 1] = { 0 };
	
	scanf_s("%s", buf, sizeof(buf));

	n = strlen(buf);
	qsort(buf, n, sizeof(char), Comp);

	printf("%s", buf);
}