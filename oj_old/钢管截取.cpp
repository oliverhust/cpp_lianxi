#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAXSIZE  10000

int g_prize[MAXSIZE + 1] = { 0, 3, 5, 8, 13, 14, 16};

int prize(int length)
{
	if (0 < length)
	{
		return g_prize[length];
		//return length;
	}
	return 0;
}

int Proc(int iInput)
{
	int i, n, t, iRes[MAXSIZE] = { 0 }, max;

	for (n = 1; n <= iInput; n++)
	{
		max = 0;
		for (i = 0; i <= n; i++)
		{
			t = prize(i) + iRes[n - i];
			if (t > max)
			{
				max = t;
			}
		}
		iRes[n] = max;
	}

	return iRes[iInput];
}

void main()
{
	int iInput;

	scanf_s("%d", &iInput, sizeof(iInput));

	printf("%d", Proc(iInput));

}