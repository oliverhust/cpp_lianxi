#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <direct.h>
#include <time.h>

#include "mylib\Write_Log.h"
#define printf WriteLog

#pragma comment(lib, "ws2_32.lib")

#define DEFAULT_PORT			8000
#define IP_SERVER				"192.168.56.77"
#define THREAD_MAX				1000
#define EACH_DELAY				0    /* ��λ������ */
#define SEND_DELAY				1

#define SEND_MAXSIZE			100

int ID, Ready = 0, Start = 0;

int Receive(SOCKET fd, char *szText, int len)
{
	int cnt;
	int rc;
	cnt = len;

	if (cnt>0)
	{
		rc = recv(fd, szText, cnt, 0);
		if (rc == SOCKET_ERROR)
		{
			return -1;
		}

		if (rc == 0)
		{
			return len - cnt;
		}

		szText += rc;
		cnt -= rc;

	}

	return len;

}

int Send(SOCKET fd, char *szText, int len)
{

	int cnt;
	int rc;

	cnt = len;

	if (cnt>0)
	{

		rc = send(fd, szText, cnt, 0);

		if (rc == SOCKET_ERROR)
		{
			return -1;
		}

		if (rc == 0)
		{
			return len - cnt;
		}

		szText += rc;
		cnt -= rc;

	}

	return len;

}

//ʹ����߳�ͬʱ  //ռ�ý϶�CPU
void ThreadSync(int id)
{
	//�����ܶ������ͬʱ�������ӷ�����
	if (THREAD_MAX - 1 != id)
	{
		Ready++;
		while (0 == Start);  //ռ�ý϶�CPU
		Ready--;
		if (0 == Ready)
		{
			Start = 0;
		}
	}
	else
	{
		while (THREAD_MAX - 1 != Ready);	//ռ�ý϶�CPU
		Start = 1;
	}
}

DWORD WINAPI ThreadFun(LPVOID pM)
{
	ULONG dwIP;
	sockaddr_in local;
	SOCKET socketTmp;
	int id;
	CHAR szSend[SEND_MAXSIZE] = { 0 };
	CHAR szRev[SEND_MAXSIZE] = { 0 };
	
	//��¼�Լ����߳�id
	id = ID;
	ID = 0;
	dwIP = inet_addr(IP_SERVER);
	
	printf("pM=%d\r\n", *(int *)pM);

	//������AF_INET,��ʾ��socket��Internet���н���ͨ��
	local.sin_family = AF_INET;

	//�˿ں�
	local.sin_port = htons(DEFAULT_PORT);

	//��������IP��ַ��
	local.sin_addr.S_un.S_addr = dwIP;

	while (1)
	{
		////��ʼ��Socket
		socketTmp = socket(AF_INET, SOCK_STREAM, 0);

		
		//�����ܶ������ͬʱ�������ӷ�����
		//ThreadSync(id);

		//���ӷ�����
		if (connect(socketTmp, (LPSOCKADDR)&local, sizeof(local)) < 0)
		{
			perror("=======Failed to connect server.");
			closesocket(socketTmp);
			Sleep(2000);
			continue;
		}

		//���ӳɹ�
		printf("%d Connect success.\r\n", id);

		sprintf_s(szSend, sizeof(szSend), "Message [%d]: Hello", id);

		//����ǰ��ʱ
		Sleep(SEND_DELAY);

		//��������Ϊ��ֻ��100�ֽڣ��ڷ�������Ҳ�涨100�ֽڡ�
		Send(socketTmp, szSend, sizeof(szSend));

		//��ȡ�������˷��ص����ݡ�
		memset(szRev, 0, sizeof(szRev));

		//���շ������˵Ļ�Ӧ��
		Receive(socketTmp, szRev, sizeof(szRev));

		printf("[%d] Receive Message:%s\r\n", id, szRev);

		if (0 == strcmp(szSend, szRev))
		{
			printf("Receive SUCCSEE !!!\r\n");
		}
		else
		{
			printf("=======================Error:Receive != Send===========================\r\n");
		}

		closesocket(socketTmp);

		Sleep(EACH_DELAY);
	}
}



int main()
{
	HANDLE handle;
	WSADATA  Ws;
	INT i;

	system("echo ON > D:\\TmpLog.txt");
	WriteLog_Init("D:\\TmpLog.txt");

	//Init Windows Socket
	if (WSAStartup(MAKEWORD(2, 2), &Ws) != 0)
	{
		printf("Init Windows Socket Failed, ErrorId = %d\r\n", GetLastError());
		return 1;
	}

	for (i = 0; i < THREAD_MAX; i++)
	{
		ID = i;
		handle = CreateThread(NULL, 0, ThreadFun, (LPVOID)&i, 0, NULL);

		//�ȴ��̶߳�ȡ�Լ���ID�����һ���̳߳���
		while (0 != ID && THREAD_MAX - 1 != ID)
		{
			;
		}
	}

	while (1)
	{
		Sleep(30000);
	}

	WSACleanup();

	return 0;

}
