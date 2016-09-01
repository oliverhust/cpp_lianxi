#include <stdio.h>
#include <string.h>

#define MAXSIZE 128
#define MAXBUF  21

/* ɾ��ָ��λ�õ��ַ� */
void DelOnePos(char *buf, int iLen, int iPos)
{
	int i;

	for (i = iPos; i < iLen; i++)
	{
		buf[i] = buf[i + 1];
	}

}


/* ɾ�����ִ���ΪiMin���ַ� */
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


/* ͳ��buf�������ASCII���ֵĴ������浽count[]�� */
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
	
	/* ͳ��buf�������ASCII���ֵĴ������浽count[]�� */
	AscCount(buf, count);
	
	/* ȡ����С��count (0����)*/
	for (i = 0; i < MAXSIZE; i++)
	{
		iMin = (count[i] < iMin && count[i] != 0) ? (count[i]) : iMin;
	}

	/* ��ͷ��βһ����ɾ����������ַ��Ĵ�������iMin */
	DelMinChars(buf, count, iMin);

	printf("%s", buf);

	(void)getchar();
}
