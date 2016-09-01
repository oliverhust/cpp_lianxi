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

	//把字符串形式的IP地址转成IN_ADDR结构需要的形式。
	//printf("Input server`s ip address:");
	//scanf_s("%s", szText, sizeof(szText));
	dwIP = inet_addr(IP_SERVER);

	//Init Windows Socket
	if (WSAStartup(MAKEWORD(2, 2), &Ws) != 0)
	{
		printf("Init Windows Socket Failed, ErrorId = %d\r\n", GetLastError());
		return 1;
	}		

	//必须是AF_INET,表示该socket在Internet域中进行通信
	local.sin_family = AF_INET;

	//端口号
	local.sin_port = htons(DEFAULT_PORT);

	//服务器的IP地址。
	local.sin_addr.S_un.S_addr = dwIP;
	
	while (1)
	{
		////初始化Socket
		socketTmp = socket(AF_INET, SOCK_STREAM, 0);
		
		//连接服务器
		if (connect(socketTmp, (LPSOCKADDR)&local, sizeof(local)) < 0)
		{
			closesocket(socketTmp);
			printf("Failed to connect server.\r\n");
			return 1;
		}
	
		//连接成功
		printf("Connect success.\r\n");

		memset(szText, 0, sizeof(szText));
		printf("Input Message to Send:");
		gets_s(szText, sizeof(szText));
		szText[100 - 1] = '\0';

		//发送请求，为简单只发100字节，在服务器端也规定100字节。
		Send(socketTmp, szText, 100);

		//读取服务器端返回的数据。
		memset(szText, 0, MAX_PATH);

		//接收服务器端的回应。
		Receive(socketTmp, szText, 100);

		printf("Receive Message:%s\r\n", szText);
	
		closesocket(socketTmp);
	}
	WSACleanup();

	return 0;

}
