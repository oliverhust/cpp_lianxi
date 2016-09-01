#include <windows.h>
#include <stdio.h>
#include <string.h>
#define MAX_LINE_LEN 128
int main()
{
	//local variables
	char cmdLine[MAX_LINE_LEN];
	//CreateProcess parameters
	LPSECURITY_ATTRIBUTES processA = NULL;//Default
	LPSECURITY_ATTRIBUTES threadA = NULL;//Default
	BOOL shareRights = TRUE;//Default
	DWORD creationMask = CREATE_NEW_CONSOLE;//Window per process.
	LPVOID enviroment = NULL;//Default
	LPCWSTR curDir = NULL;//Default
	STARTUPINFO startInfo;//Result
	PROCESS_INFORMATION procInfo;//Result

	while (fgets(cmdLine, MAX_LINE_LEN, stdin) != NULL)
	{
		// Read a command from the file
		if (cmdLine[strlen(cmdLine) - 1] == '\n')
			cmdLine[strlen(cmdLine) - 1] = '\0';//Remove NEWLINE
		//Create a new process to execute the command
		ZeroMemory(&startInfo, sizeof(startInfo));
		startInfo.cb = sizeof(startInfo);
		if (!CreateProcess(
			(LPWSTR)cmdLine,//File name of executable
			NULL,//command line
			processA,//Process inherited security
			threadA, //Thread inherited security
			shareRights,//Rights propagation
			creationMask,//various creation flags
			enviroment,//Enviroment variable
			curDir, //Child's current directory
			&startInfo,
			&procInfo
			)
			)
		{
			fprintf(stderr, "CreatProcess failed on error %d\n", GetLastError());
		}
	}
	//Terminate after all commands have finished.
	printf("Father Process Finish\r\n");
	(void)getchar();
	return 0;
}
