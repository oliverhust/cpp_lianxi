#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXSIZE 8192


void main()
{
	int i, n = 0, num[MAXSIZE];

	do
	{
		scanf_s("%d", num + n, sizeof(int));
		n++;
	} while (0 != num[n - 1]);

	for (i = 0; i < n - 1; i++)
	{
		printf("%d\n", num[i] / 2);
	}

}