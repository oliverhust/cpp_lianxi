#include <stdio.h>
#include <string.h>

static int getTotalCount(int monthCount)
{
	int i, x, t1 = 1,t2 = 1;

	if (monthCount < 3)	return 1;

	for (i = 2; i < monthCount; i++)
	{
		x = t1 + t2;
		t1 = t2;
		t2 = x;
	}

	return x;
}

void main()
{
	int i;
	scanf_s("%d", &i, sizeof(i));
	printf("%d", getTotalCount(i));
}