#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <windows.h>

using namespace std;

void fill_rand_str(char *pcData, int size)
{
	int  i;
	char c;

	i = 0;
	for (i = 0; i < size - 1; i++)
	{
		c = (unsigned int)rand() % (26 * 2);
		if (c < 26)
		{
			c += 'a';
		}
		else
		{
			c += 'A' - 26;
		}
		pcData[i] = c;
	}

	pcData[size - 1] = '\0';
	return;
}

char *tolower1(char *pcStr)
{
	int i;

	for (i = 0; '\0' != pcStr[i]; i++)
	{
		if (pcStr[i] >= 'A' && pcStr[i] <= 'Z')
		{
			pcStr[i] += ('a' - 'A');
		}
	}

	return pcStr;
}

char *tolower2(char *pcStr)
{
	char *pcTmp = pcStr;
	char cTmp;

	while ('\0' != (cTmp = *pcTmp))
	{
		if (cTmp >= 'A' && cTmp <= 'Z')
		{
			*pcTmp = cTmp + ('a' - 'A');
		}
		pcTmp++;
	}

	return pcStr;
}

#define TYPE_T int

void _tolower3_int(int *pd)
{
	int t, d;

	d = *pd;

	t = d & 0xff;
	if (t >= 'A' && t <= 'Z')
	{
		d += ('a' - 'A');
	}

	t = d & 0xff00;
	if (t >= ((int)'A' << 8) && t <= ((int)'Z' << 8))
	{
		d += ((int)'a' - 'A') << 8;
	}

	t = d & 0xff0000;
	if (t >= ((int)'A' << 16) && t <= ((int)'Z' << 16))
	{
		d += ((int)'a' - 'A') << 16;
	}

	t = d & 0xff000000;
	if (t >= ((int)'A' << 24) && t <= ((int)'Z' << 24))
	{
		d += ((int)'a' - 'A') << 24;
	}

	*pd = d;

	return;
}

void _tolower3_int(long long *pd)
{
	long long t, d;

	d = *pd;

	t = d & 0xff;
	if (t >= 'A' && t <= 'Z')
	{
		d += ('a' - 'A');
	}

	t = d & 0xff00;
	if (t >= ((long long)'A' << 8) && t <= ((long long)'Z' << 8))
	{
		d += ((long long)'a' - 'A') << 8;
	}

	t = d & 0xff0000;
	if (t >= ((long long)'A' << 16) && t <= ((long long)'Z' << 16))
	{
		d += ((long long)'a' - 'A') << 16;
	}

	t = d & 0xff000000;
	if (t >= ((long long)'A' << 24) && t <= ((long long)'Z' << 24))
	{
		d += ((long long)'a' - 'A') << 24;
	}

	t = d & 0xff00000000;
	if (t >= ((long long)'A' << 32) && t <= ((long long)'Z' << 32))
	{
		d += ((long long)'a' - 'A') << 32;
	}

	t = d & 0xff0000000000;
	if (t >= ((long long)'A' << 40) && t <= ((long long)'Z' << 40))
	{
		d += ((long long)'a' - 'A') << 40;
	}

	t = d & 0xff000000000000;
	if (t >= ((long long)'A' << 48) && t <= ((long long)'Z' << 48))
	{
		d += ((long long)'a' - 'A') << 48;
	}

	t = d & 0xff00000000000000;
	if (t >= ((long long)'A' << 56) && t <= ((long long)'Z' << 56))
	{
		d += ((long long)'a' - 'A') << 54;
	}

	*pd = d;

	return;
}

char *tolower3(char *pcStr)
{
	unsigned int i, s;

	s = strlen(pcStr);
	for (i = 0; i + sizeof(TYPE_T) <= s; i += sizeof(TYPE_T))
	{
		_tolower3_int((TYPE_T *)(pcStr + i));		
	}

	for (; i < s; i++)
	{
		if (pcStr[i] >= 'A' && pcStr[i] <= 'Z')
		{
			pcStr[i] += ('a' - 'A');
		}
	}

	return pcStr;
}
#undef TYPE_T

char *tolower4(char *pcStr)
{
	char cTmp;
	int i, s;

	s = strlen(pcStr);

	for (i = 0; i + 4 <= s; i += 4)
	{
		cTmp = pcStr[i];
		if (cTmp >= 'A' && cTmp <= 'Z')
		{
			pcStr[i] = cTmp + ('a' - 'A');
		}

		cTmp = pcStr[i+1];
		if (cTmp >= 'A' && cTmp <= 'Z')
		{
			pcStr[i + 1] = cTmp + ('a' - 'A');
		}

		cTmp = pcStr[i+2];
		if (cTmp >= 'A' && cTmp <= 'Z')
		{
			pcStr[i + 2] = cTmp + ('a' - 'A');
		}

		cTmp = pcStr[i+3];
		if (cTmp >= 'A' && cTmp <= 'Z')
		{
			pcStr[i + 3] = cTmp + ('a' - 'A');
		}
	}

	for (; i < s; i++)
	{
		cTmp = pcStr[i];
		if (cTmp >= 'A' && cTmp <= 'Z')
		{
			pcStr[i] = cTmp + ('a' - 'A');
		}
	}

	return pcStr;
}

int test_tolower(const char *pcTestName, char* (*pfFunc)(char *), unsigned uiTestSize)
{
	char *pcStr;

	pcStr = (char *)malloc(uiTestSize);
	if (NULL == pcStr)
	{
		cout << "No enough memory" << endl;
		return 1;
	}

	fill_rand_str(pcStr, uiTestSize);
	//cout << pcStr << endl; // debug
	auto t_elaspe = clock();
	pfFunc(pcStr);
	t_elaspe = clock() - t_elaspe;
	//cout << pcStr << endl;   // debug
	cout << pcTestName << " (size=" << uiTestSize << ") elaspe " << t_elaspe << " ms" << endl;

	free(pcStr);
	return t_elaspe;
}

int main()
{
	srand(time(NULL));

	//test_tolower("Tolower1", tolower1, 1e8);
	//test_tolower("Tolower2", tolower2, 1e8);
	test_tolower("Tolower3", tolower3, 1e8);
	//test_tolower("Tolower4", tolower4, 1e8);
	//test_tolower("Strupr", strupr, 1e8);

	getchar();
	return 0;
}