#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#include <direct.h>
#include <time.h>

#include "mylib\Mystr.h"

#define COMMAND_MAX			256
#define COMMIT_INFO_MAX		512
#define PATH_NAME_MAX		256

#define ERROR_FAILED		1

errno_t argc_check(int argc, char *argv[])
{
	if (argc != 2)
	{
		printf("Usage:%s <RemoteBranchName>\r\n", argv[0]);
		printf("Example: %s work\r\n", argv[0]);
		(void)getchar();

		exit(ERROR_FAILED);
	}

	printf("argc=%d,argv[0]=%s,argv[1]=%s\r\n", argc, argv[0], argv[1]);

	return ERROR_SUCCESS;
}

/* 切换目录到上一级 */
errno_t sys_chdir(char *pcArg0)
{
	char szPath[PATH_NAME_MAX] = { 0 };

	/* 获取当前目录 */
	Mystr_GetBasename_Char(pcArg0, szPath, sizeof(szPath));
	if ('\0' == szPath[0] || strncmp(".", szPath, sizeof(szPath)) == 0)
	{
		strcpy_s(szPath, sizeof(szPath), "..");
	}
	else
	{
		/* 获取上一级工作目录 */
		Mystr_GetBasename_Char(szPath, szPath, sizeof(szPath));
		if ('\0' == szPath[0] || strncmp(".", szPath, sizeof(szPath)) == 0)
		{
			strcpy_s(szPath, sizeof(szPath), ".");
		}
	}

	printf("Change dir to %s\r\n", szPath);
	if (ERROR_SUCCESS != _chdir(szPath))
	{
		printf("Change dir error\r\n");
		exit(ERROR_FAILED);
	}

	_getcwd(szPath, sizeof(szPath));
	printf("Current dir:%s\r\n", szPath);

	return ERROR_SUCCESS;

}

errno_t commit_info_get(char *pcInfo, int size)
{
	printf("Commit Info:\r\n");
	gets_s(pcInfo, size);

	return ERROR_SUCCESS;
}

errno_t Git_AddAll()
{
	printf("Start to Add non-version files..\r\n");
	system("git add -A");

	return ERROR_SUCCESS;
}

errno_t Git_Commit(char *pcInfo)
{
	char cmd[COMMAND_MAX] = { 0 };

	printf("Start to Commit..\r\n");
	if ('\0' != pcInfo[0] && '\n' != pcInfo[0])
	{
		sprintf_s(cmd, sizeof(cmd), "git commit -a -m \"%s\"", pcInfo);
		system(cmd);
	}
	else
	{
		system("git commit -a -m \"-\"");
	}

	printf("--------------------------------------------------\r\n");
	return ERROR_SUCCESS;
}

errno_t Git_Fetch()
{
	printf("Start to Fetch..\r\n");
	system("git.exe fetch -v --progress \"origin\"");

	return ERROR_SUCCESS;
}

errno_t Git_Push(char *pcBranch)
{
	char cmd[COMMAND_MAX] = { 0 };
	
	printf("Start to Push..\r\n");
	sprintf_s(cmd, sizeof(cmd), "git.exe push --progress  \"origin\" master:%s", pcBranch);
	system(cmd);

	return ERROR_SUCCESS;
}

errno_t Fini()
{
	printf("Press enter to finish...\r\n");
	(void)getchar();

	return ERROR_SUCCESS;
}

int main(int argc, char *argv[])
{
	char cmd[COMMAND_MAX] = { 0 };
	char info[COMMIT_INFO_MAX] = { 0 };
	
	/* 检查入参 */
	argc_check(argc, argv);

	/* 设置工作路径 */
	sys_chdir(argv[0]);

	/* 提示输入Commit信息 */
	commit_info_get(info, sizeof(info));

	/* 添加新文件 */
	Git_AddAll();

	/* 提交本地 */
	Git_Commit(info);

	/* 更新 */
	Git_Fetch();

	/* 上传 */
	Git_Push(argv[1]);
	
	/* 结束 */
	Fini();

	return 0;
}
