// Socket模型（二）：完成端口（IOCP）: http://www.cnblogs.com/renyuan/p/5100296.html

#include "WinSock2.h"
#include <stdio.h>
#pragma comment(lib, "ws2_32.lib")

#define MESSAGESIZE 1024

SOCKET serverSocket;
DWORD WINAPI SocketProcAccept(LPVOID pParam);
DWORD WINAPI SocketProcMain(LPVOID pParam);

enum SOCKETOPERATE
{
	soREVC
};

struct SOCKETDATA
{
	WSAOVERLAPPED        overlapped;
	WSABUF                buf;
	char                sMessage[MESSAGESIZE];
	DWORD                dwBytes;
	DWORD                flag;
	SOCKETOPERATE        socketType;
	void Clear(SOCKETOPERATE type)
	{
		ZeroMemory(this, sizeof(SOCKETDATA));
		buf.buf = sMessage;
		buf.len = MESSAGESIZE;
		socketType = type;
	}
};

SOCKET CreateServiceSocket(int Port)
{
	int iError;
	WSAData data;
	iError = WSAStartup(0x0202, &data);
	SOCKET tmp = socket(AF_INET, SOCK_STREAM, 0);
	if (tmp == INVALID_SOCKET)
	{
		return INVALID_SOCKET;
	}

	SOCKADDR_IN addr;
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_family = AF_INET;
	addr.sin_port = htons(Port);
	if ((bind(tmp, (sockaddr*)&addr, sizeof(addr))) != 0)
	{
		closesocket(tmp);
		return INVALID_SOCKET;
	}

	if ((listen(tmp, INFINITE)) != 0)
	{
		closesocket(tmp);
		return INVALID_SOCKET;
	}

	return tmp;
}

int main(int argc, char* argv[])
{
	HANDLE CP = INVALID_HANDLE_VALUE;
	CP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	SYSTEM_INFO systemInfo;
	GetSystemInfo(&systemInfo);
	for (int i = 0; i < systemInfo.dwNumberOfProcessors; i++)
	{
		CreateThread(NULL, NULL, &SocketProcMain, CP, NULL, NULL);
	}
	serverSocket = CreateServiceSocket(6565);
	if (serverSocket == INVALID_SOCKET)
	{
		return 0;
	}

	CreateThread(NULL, NULL, &SocketProcAccept, CP, NULL, NULL);

	while (1)
	{
		Sleep(10000);
	}
	CloseHandle(CP);
	closesocket(serverSocket);
	WSACleanup();
	return 0;
}

DWORD WINAPI SocketProcAccept(LPVOID pParam)
{
	HANDLE CP = (HANDLE)pParam;
	SOCKADDR_IN addr;
	int len = sizeof(SOCKADDR_IN);
	SOCKET tmp;
	SOCKETDATA *lpSocketData;
	while (1)
	{
		tmp = accept(serverSocket, (sockaddr*)&addr, &len);
		printf("Client Accept:%s\t:%d\n", inet_ntoa(addr.sin_addr), htons(addr.sin_port));
		CreateIoCompletionPort((HANDLE)tmp, CP, (DWORD)tmp, INFINITE);
		lpSocketData = (SOCKETDATA *)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(SOCKETDATA));
		lpSocketData->Clear(soREVC);
		WSARecv(tmp, &lpSocketData->buf, 1, &lpSocketData->dwBytes, &lpSocketData->flag, &lpSocketData->overlapped, NULL);
	}
}

DWORD WINAPI SocketProcMain(LPVOID pParam)
{
	HANDLE CP = (HANDLE)pParam;
	SOCKADDR_IN addr;
	DWORD dwBytes;
	SOCKETDATA *lpSocketData;
	SOCKET clientSocket;

	while (1)
	{
		GetQueuedCompletionStatus(CP, &dwBytes, (PULONG_PTR)&clientSocket, (LPOVERLAPPED*)&lpSocketData, INFINITE);
		if (dwBytes == 0xFFFFFFFF)
		{
			return 0;
		}

		if (lpSocketData->socketType == soREVC)
		{
			if (dwBytes == 0)
			{
				closesocket(clientSocket);
				HeapFree(GetProcessHeap(), 0, lpSocketData);
			}
			else
			{
				lpSocketData->sMessage[dwBytes] = '\0';
				printf("%x\t:%s\n", (DWORD)clientSocket, lpSocketData->sMessage);
				lpSocketData->Clear(soREVC);
				WSARecv(clientSocket, &lpSocketData->buf, 1, &lpSocketData->dwBytes, &lpSocketData->flag, &lpSocketData->overlapped, NULL);
			}
		}
	}
}
