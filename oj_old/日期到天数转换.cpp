#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAXSIZE 8192

/* 每月天数，月份从1开始 */
int mon[13] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

/* 根据是否闰年计算二月份的天数  */
void MonthTwoDays(int year)
{
	if (year % 400 == 0 || (year % 100 != 0 && year % 4 == 0))
	{
		mon[2] = 29;
	}
}

static int iConverDateToDay(int year, int month, int day)
{
	int i, sum = day;

	MonthTwoDays(year);

	if (day > mon[month] || month > 12 || year < 0 || month < 1 || day < 1)
	{
		return -1;
	}

	for (i = 1; i < month; i++)
	{
		sum += mon[i];
	}

	return sum;
}


void main()
{
	int year, month, day;

	scanf_s("%d", &year, sizeof(year));
	scanf_s("%d", &month, sizeof(month));
	scanf_s("%d", &day, sizeof(day));

	printf("%d", iConverDateToDay(year, month, day));
}