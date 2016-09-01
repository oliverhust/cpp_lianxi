#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <direct.h>
#include <time.h>

#pragma comment(lib, "ws2_32.lib")

#define DEFAULT_PORT			8000
#define IP_SERVER				"192.168.56.77"

int Receive(SOCKET fd, char *szText, int len)
{
	int cnt;
	int rc;
	cnt = len;

	//while (cnt>0)
	{
		rc = recv(fd, szText, cnt, 0);
		if (rc == SOCKET_ERROR)
		{
			return -1;
		}
		else if (rc == 0)
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

	//while (cnt>0)
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

int main()
{
	ULONG dwIP;
	WSADATA  Ws;
	sockaddr_in local;
	SOCKET socketTmp;
	CHAR szText[MAX_PATH];

	//���ַ�����ʽ��IP��ַת��IN_ADDR�ṹ��Ҫ����ʽ��
	//printf("Input server`s ip address:");
	//scanf_s("%s", szText, sizeof(szText));
	dwIP = inet_addr(IP_SERVER);

	//Init Windows Socket
	if (WSAStartup(MAKEWORD(2, 2), &Ws) != 0)
	{
		printf("Init Windows Socket Failed, ErrorId = %d\r\n", GetLastError());
		return 1;
	}		

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
		
		//���ӷ�����
		if (connect(socketTmp, (LPSOCKADDR)&local, sizeof(local)) < 0)
		{
			closesocket(socketTmp);
			printf("Failed to connect server.\r\n");
			return 1;
		}
	
		//���ӳɹ�
		printf("Connect success.\r\n");

		memset(szText, 0, sizeof(szText));
		printf("Input Message to Send:");
		gets_s(szText, sizeof(szText));
		szText[100 - 1] = '\0';

		//��������Ϊ��ֻ��100�ֽڣ��ڷ�������Ҳ�涨100�ֽڡ�
		Send(socketTmp, szText, 100);

		//��ȡ�������˷��ص����ݡ�
		memset(szText, 0, MAX_PATH);

		//���շ������˵Ļ�Ӧ��
		Receive(socketTmp, szText, 100);

		printf("Receive Message:%s\r\n", szText);
	
		closesocket(socketTmp);
	}
	WSACleanup();

	return 0;

}
