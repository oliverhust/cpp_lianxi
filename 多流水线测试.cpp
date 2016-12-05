#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <windows.h>

using namespace std;

void time_c(const char *pcName, void (*pfCallback)(void))
{
	auto t_elaspe = clock();
	pfCallback();
	t_elaspe = clock() - t_elaspe;	
	cout << pcName << " elaspe " << t_elaspe << " ms." << endl;
}

void fill_rand_bin(char *pcData, int size)
{
	int  i;

	for (i = 0; i<size; i++)
	{
		pcData[i] = rand() % 256;
	}
}

void test1()
{
#define TEST_SIZE		1
	typedef int MYTYPE;

	MYTYPE *pA, *pB, *pC, *pD;

	pA = (MYTYPE *)malloc(TEST_SIZE * sizeof(MYTYPE));
	pB = (MYTYPE *)malloc(TEST_SIZE * sizeof(MYTYPE));
	pC = (MYTYPE *)malloc(TEST_SIZE * sizeof(MYTYPE));
	pD = (MYTYPE *)malloc(TEST_SIZE * sizeof(MYTYPE));
	fill_rand_bin((char *)pA, TEST_SIZE * sizeof(MYTYPE));
	fill_rand_bin((char *)pB, TEST_SIZE * sizeof(MYTYPE));
	fill_rand_bin((char *)pC, TEST_SIZE * sizeof(MYTYPE));
	fill_rand_bin((char *)pD, TEST_SIZE * sizeof(MYTYPE));

	auto t_elaspe = clock();

	int i;
	long s = 1, r = 3, q = 4, p = 6;
	for (i = 1; i < 1e9; i++)
	{
		//s *= i;
		//r *= i;
		//q *= i;
		//p *= i;	

		r = r * i + i;
		//s = s * i + i;
		//q = q / i + i;
		//p = p * i + 1;
		//*pB = *pB * (i + 9);

	}

	t_elaspe = clock() - t_elaspe;
	printf("Test1(%d): %ld, %ld, %ld, %ld\r\n", i, s, r, q, p);
	cout << "Test1" << " elaspe " << t_elaspe << " ms." << endl;

	

#undef TEST_SIZE
}


int main()
{
	srand(time(NULL));

	test1();

	return 0;
}