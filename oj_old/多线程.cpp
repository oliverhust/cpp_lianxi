#include <stdio.h>
#include <windows.h>

char  g_write[1032] = { 0 };
HANDLE handle1, handle2, handle3, handle4;
/* ��ʣ�¶����� */
int Remain = 0;

/* ���� */
DWORD WINAPI ThreadFun1(LPVOID pM);
DWORD WINAPI ThreadFun2(LPVOID pM);
DWORD WINAPI ThreadFun3(LPVOID pM);
DWORD WINAPI ThreadFun4(LPVOID pM);

void Print(char c)
{
	int i;

	for (i = 0; g_write[i] != '\0'; i++);

	g_write[i] = c;
	printf("%c", c);
}

DWORD WINAPI ThreadFun1(LPVOID pM)
{
	Print('A');
	handle2 = CreateThread(NULL, 0, ThreadFun2, NULL, 0, NULL);
	return 0;
}

DWORD WINAPI ThreadFun2(LPVOID pM)
{
	Print('B');
	handle3 = CreateThread(NULL, 0, ThreadFun3, NULL, 0, NULL);
	return 0;
}

DWORD WINAPI ThreadFun3(LPVOID pM)
{
	Print('C');
	handle4 = CreateThread(NULL, 0, ThreadFun4, NULL, 0, NULL);
	return 0;
}

DWORD WINAPI ThreadFun4(LPVOID pM)
{
	Print('D');
	Remain--;
	if (0 != Remain)
	{
		handle1 = CreateThread(NULL, 0, ThreadFun1, NULL, 0, NULL);
	}
	return 0;
}

/* Ҫ�ȵ�RemainΪ0�����������Ȼ��ѭ����wait */
void Release()
{
	while (0 != Remain)
	{
		WaitForSingleObject(handle1, INFINITE);
		WaitForSingleObject(handle2, INFINITE);
		WaitForSingleObject(handle3, INFINITE);
		WaitForSingleObject(handle4, INFINITE);
	}
}

void main()
{
	scanf_s("%d", &Remain);

	/* �����߳�1���Ѳ���Remain���ݸ��� */
	handle1 = CreateThread(NULL, 0, ThreadFun1, (LPVOID)&Remain, 0, NULL);
	
	Release();

}

