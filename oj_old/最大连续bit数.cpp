#include <stdio.h>

void main()
{
	int i, s = 0, max = 0;
	unsigned int n;

	scanf_s("%u", &n, sizeof(n));

	if (n > 255)
	{
		return;
	}

	for (i = 0; i < 8; i++)
	{
		if ((n >> i) & 0x01)
		{
			s++;
			if (s > max)
			{
				max = s;
			}
		}
		else
		{
			s = 0;
		}
	}

	printf("%d", max);

}