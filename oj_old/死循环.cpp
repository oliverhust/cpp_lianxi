#include <stdio.h>
#include <windows.h>


/* 如果带参数，则把参数打印到文件中 */

int main(int argc, char *argv)
{
	int i, pos = 0;
	char buf[4096] = { 0 };
	char sys[4096] = { 0 };
	

	if (1 == argc)
	{
		while (1)
		{
			Sleep(1000);
		}
	}

	for (i = 0; i < argc; i++)
	{
		pos += sprintf_s(buf + pos, sizeof(buf) - pos, "%s", argv[i]);
	}

	sprintf_s(sys, sizeof(sys), "echo %s >> 123.txt", buf);

	system(sys);
	
	while (1)
	{
		Sleep(1000);
	}

	return 0;
}