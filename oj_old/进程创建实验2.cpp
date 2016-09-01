#include <stdio.h>
#include <windows.h>

WCHAR URLs[][256] = {
	L"IEXPLORE http://www.sina.com.cn",
	L"IEXPLORE http://www.hao123.com",
	L"IEXPLORE http://www.baidu.com",
	L"IEXPLORE http://www.163.com"
};

int main()
{
	STARTUPINFO siStartInfo;
	PROCESS_INFORMATION piProcInfo;
	ZeroMemory(&siStartInfo, sizeof(STARTUPINFO));
	siStartInfo.cb = sizeof(siStartInfo);
	int i;
	errno_t err;

	/*
	CreateProcess(L"D:\\val.exe",
		NULL,
		NULL, NULL, FALSE, 0, NULL,
		L"D:",
		&siStartInfo, &piProcInfo);
	*/


	
	//自动打开网页
	if (1)
	{
		for (i = 0; i<sizeof(URLs) / 256; i++)
		{
			err = CreateProcess(L"C:\\Program Files\\Internet Explorer\\IEXPLORE.EXE", 
								URLs[i], 
								NULL, NULL, FALSE, 0, NULL, 
								L"C:\\Program Files\\Internet Explorer",
								&siStartInfo, &piProcInfo);
			if (ERROR_SUCCESS != err)
			{
				printf("Error to Creat process\r\n");
			}
			else
			{
				printf("Open Successfully: %s\r\n", URLs[i]);
			}
			Sleep(5000);
		}
	}
	

	printf("Main Process Finish\r\n");
	//(void)getchar();

	return 0;
}