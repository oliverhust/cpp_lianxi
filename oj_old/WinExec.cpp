#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

int main(int argc, char *argv[])
{
	int i, pos = 0;
	char buf[4096] = { 0 };

	//对每个参数进行拼接
	for (i = 1; i < argc; i++)
	{
		pos += sprintf_s(buf + pos, sizeof(buf)-pos, "%s ", argv[i]);
	}

	WinExec(buf, SW_SHOW);

	return 0;
}