#include <stdio.h>
#include <string.h>

#define MAXSIZE 128
#define MAXBUF  21

/* 删除指定位置的字符 */
void DelOnePos(char *buf, int iLen, int iPos)
{
	int i;

	for (i = iPos; i < iLen; i++)
	{
		buf[i] = buf[i + 1];
	}

}


/* 删除出现次数为iMin的字符 */
void DelMinChars(char *buf, int *count, int iMin)
{
	int i;
	for (i = 0; i < (int)strlen(buf); i++)
	{
		if (iMin == count[buf[i]])
		{
			DelOnePos(buf, (int)strlen(buf) ,i);
			i--;
		}
	}
}


/* 统计buf里面各个ASCII出现的次数，存到count[]中 */
void AscCount(char *buf, int *count)
{
	int i;
	
	for (i = 0; i < (int)strlen(buf); i++)
	{
		count[buf[i]]++;
	}
}

void main()
{
	char buf[MAXBUF] = { 0 };
	int iMin = MAXBUF, count[MAXSIZE] = { 0 }, i, result = 0;

	scanf_s("%s", buf, sizeof(buf));
	
	/* 统计buf里面各个ASCII出现的次数，存到count[]中 */
	AscCount(buf, count);
	
	/* 取出最小的count (0除外)*/
	for (i = 0; i < MAXSIZE; i++)
	{
		iMin = (count[i] < iMin && count[i] != 0) ? (count[i]) : iMin;
	}

	/* 从头到尾一个个删除，如果该字符的次数等于iMin */
	DelMinChars(buf, count, iMin);

	printf("%s", buf);

	(void)getchar();
}
