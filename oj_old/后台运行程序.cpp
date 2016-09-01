#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <direct.h>
#include <time.h>

#include "mylib\Mystr.h"
#include "mylib\Write_Log.h"

/* ���������ں�ִ̨��һ������ */
#pragma comment(linker, "/subsystem:windows /entry:mainCRTStartup")

/* ��ӡ��Ϣ���ض����ļ� */
#define printf WriteLog

#define PATH_COMMANDS			"WinExec_Hide_Command.txt"
#define PATH_LOG				"RunWindowHideLog.txt"
#define PATH_LOG_FATALERR		"D:\\RunWindowHideLog.txt"

//��ӡ��ǰʱ��
void print_time(void)
{
	time_t tim;
	char buf[512];
	time(&tim);
	ctime_s(buf, sizeof(buf), &tim);

	printf("%s", buf);

	return;
}

int main(int argc, char *argv[])
{
	char buf[4096];
	FILE *fil;
	int i, pos = 0;
	errno_t err;

	/* ��ȡ��ǰ����·�� */
	if (NULL == Mystr_GetBasename_Char(argv[0], buf, sizeof(buf)))
	{
		system("echo CanNotGetBaseName >>"PATH_LOG_FATALERR);
		exit(1);
	}

	//���õ�ǰ����·��
	if (ERROR_SUCCESS != _chdir(buf))
	{
		system("echo CanNotGetChangeDir >>"PATH_LOG_FATALERR);
		exit(1);
	}

	WriteLog_Init(PATH_LOG);
	printf("=========================================================\r\n");
	print_time();
	printf("My Basename:%s\r\n", buf);
	printf("argc = %d\r\n", argc);
	for (i = 0; i < argc; i++)
	{
		printf("argv[%d] = %s\r\n", i, argv[i]);
	}


	//���ݲ�������ִ�в�ͬ����
	if (argc < 2)
	{
		printf("Function:Execute a program background\r\n"
			"Usage: WinExec_Hide.exe ThePromgramYouWantToExecute ArgumentsOfTheProgram\r\n"
			"Example: WinExec_Hide.exe D:\\GetMaxNum.exe 100 1000\r\n");

		//����޲���������ļ��ж�ȡ����
		err = fopen_s(&fil, PATH_COMMANDS, "a+");
		if (ERROR_SUCCESS != err || NULL == fil)
		{
			printf("Unable to open %s\r\n", PATH_COMMANDS);
			WriteLog_Fini();
			exit(1);
		}
		

		//һ���ж�ȡ�ļ���ִ��
		//WinExec(PATH_COMMANDS, SW_HIDE);
		
		while (NULL != fgets(buf, sizeof(buf), fil))
		{
			printf("%s\r\n", buf);
			WinExec(buf, SW_HIDE);
		}
		

		fclose(fil);
		WriteLog_Fini();
		exit(1);
	}
	
	
	

	//ƴ�Ӳ���
	for (i = 1; i < argc; i++)
	{
		pos += sprintf_s(buf + pos, sizeof(buf) - pos, "%s ", argv[i]);
	}

	//���г���
	WinExec(buf, SW_HIDE);
	

	WriteLog_Fini();

	return 0;
}

//D:\MyDocuments\VC\Git_AutoCA\Git_AutoCA\Debug\Git_AutoCA.exe start


//HWND hWnd;
//WinExec("D:\\123.exe", SW_HIDE);
//system("echo 123 >> D:\\123.txt");

/*
SetConsoleTitle(L"Hello");
hWnd = ::FindWindow(NULL, L"Hello");
ShowWindow(hWnd, SW_HIDE);
*/