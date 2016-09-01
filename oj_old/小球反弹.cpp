#include <stdio.h>

static double getTenthHigh(int high)
{
	return ((double)high/32.0);
}

static double getJourney(int high)
{
	return (double)high * 23.0 / 8.0;
}

void main()
{
	int high;
	
	scanf_s("%d", &high);
	
	printf("%g\n%g\n", getJourney(high), getTenthHigh(high));

	(void)getchar();

}