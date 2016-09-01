#include <stdio.h>
#include <stdlib.h>

#define MAXSIZE 4096

int Num;

int Comp(const void *a, const void *b)
{
	return *(int*)a - *(int*)b;
}

int RevComp(const void *a, const void *b)
{
	return *(int*)b - *(int*)a;
}

void sortIntegerArray(int *pIntegerArray, int iSortFlag)
{
	int i;

	if (0 == iSortFlag)
	{
		qsort(pIntegerArray, Num, sizeof(int), Comp);
	}
	else
	{
		qsort(pIntegerArray, Num, sizeof(int), RevComp);
	}

	for (i = 0; i < Num; i++)
	{
		if (0 == i)
		{
			printf("%d", pIntegerArray[i]);
		}
		else
		{
			printf(" %d", pIntegerArray[i]);
		}
	}
}



void main()
{
	int i, mark;
	int buf[MAXSIZE];

	scanf_s("%d", &Num, sizeof(Num));

	for (i = 0; i < Num; i++)
	{
		scanf_s("%d", buf + i, sizeof(int));
	}

	scanf_s("%d", &mark, sizeof(mark));

	sortIntegerArray(buf, mark);

	(void)getchar();


}