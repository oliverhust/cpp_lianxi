#include <stdio.h>
#include <string.h>
#include <ctype.h>


void main()
{
	int i, m, sum = 0;

	scanf_s("%d", &m, sizeof(m));

	for (i = 1; i <= m; i++)
	{
		sum += 3 * i - 1;
	}

	printf("%d", sum);
}