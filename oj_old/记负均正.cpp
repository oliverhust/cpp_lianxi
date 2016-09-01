#include <stdio.h>

#define MAXSIZE 8192

void main()
{
	int data, num_pos = 0, num_neg = 0, sum_pos = 0;

	while (scanf_s("%d", &data, sizeof(data)) != EOF)
	{
		if (data < 0)
		{
			num_neg++;
		}
		else
		{
			num_pos++;
			sum_pos += data;
		}
	}

	if (0 == num_pos)
	{
		num_pos = 1;
	}

	printf("%d\n%.1f", num_neg, (float)sum_pos / num_pos);

	(void)getchar();
}