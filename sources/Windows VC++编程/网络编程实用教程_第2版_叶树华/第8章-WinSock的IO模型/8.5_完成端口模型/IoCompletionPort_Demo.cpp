// 8.5 使用完成端口模型示例程序
// 本例使用完成端口模型，完成一个简单的回应服务器应用。本服务器的功能是将收到的客户端数据原封不动地返回客户端。

#define WIN32_LEAN_AND_MEAN
#include <WinSock2.h>
#include <windows.h>
#include <stdio.h>

#pragma comment(lib, "ws2_32.lib")
 
#define	 PORT 5150
#define	 DATA_BUFSIZE 4096			// 接收缓冲区大小

typedef struct 
{
	OVERLAPPED Overlapped;
	WSABUF DataBuf;
	CHAR Buffer[DATA_BUFSIZE];
	DWORD BytesSend;
	DWORD BytesRecv;
}PER_IO_OPERATION_DATA, *LPPER_IO_OPERATION_DATA;

typedef struct 
{
	SOCKET Socket;
}PER_HANDLE_DATA, *LPPER_HANDLE_DATA;

// 工作线程服务例程
DWORD WINAPI ServerWorkerThread(LPVOID CompletionPortID);

int IoCompletionPort_Demo()
{
	// 1、定义变量
	SOCKADDR_IN ServerAddr;
	SOCKET ListenSocket;								// 监听套接字
	SOCKET AcceptSocket;								// 响应套接字(与客户端进行通信)
	HANDLE CompletionPort;
	SYSTEM_INFO SystemInfo;
	LPPER_HANDLE_DATA PerHandleData;
	LPPER_IO_OPERATION_DATA PerIoData;
	int i = 0;
	DWORD  RecvBytes = 0;								// 接收到的字符长度
	DWORD  dwFlags = 0;									
	DWORD ThreadID;

	WORD wVersionRequested;
	WSADATA wsaData;
	DWORD Ret;

	wVersionRequested = MAKEWORD(2, 2);

	Ret = WSAStartup(wVersionRequested, &wsaData);
	if (Ret != 0) {
		/* Tell the user that we could not find a usable */
		/* WinSock DLL.                                  */
		printf("WSAStartUp() failed with error: %d\n", Ret);
		return 1;
	}

	/* Confirm that the WinSock DLL supports 2.2.*/
	/* Note that if the DLL supports versions greater    */
	/* than 2.2 in addition to 2.2, it will still return */
	/* 2.2 in wVersion since that is the version we      */
	/* requested.                                        */

	if (LOBYTE(wsaData.wVersion) != 2 ||
		HIBYTE(wsaData.wVersion) != 2) {
		/* Tell the user that we could not find a usable */
		/* WinSock DLL.                                  */
		WSACleanup();
		return 1;
	}

	/* The WinSock DLL is acceptable. Proceed. */

	// 创建一个完成端口对象
	if ((CompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0)) == NULL)
	{
		printf("CreateIoCompletionPort failed with error: %d\n", GetLastError());
		return 1;
	}

	// 决定系统安装了多少个处理器
	GetSystemInfo(&SystemInfo);

	// 根据处理器的数量，创建工作线程，数量为处理器数量的两倍
	for (i = 0; i < SystemInfo.dwNumberOfProcessors * 2; ++i)
	{
		HANDLE ThreadHandle;
		// 创建一个服务工作线程，并将完成端口传递给它 
		if ((ThreadHandle = CreateThread(NULL, 0, ServerWorkerThread, CompletionPort, 0, &ThreadID)) == NULL)
		{
			printf("CreateThread() failed wirth error: %d\n", GetLastError());
			return 1;
		}
		// 关闭线程句柄
		CloseHandle(ThreadHandle);
	}

	// 创建监听套接字，并绑定端口
	ListenSocket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (ListenSocket == INVALID_SOCKET) {
		printf("WSASocket() failed with error: %d\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}

	// 分配端口及协议族并绑定
	ServerAddr.sin_family = AF_INET;
	//ServerAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	ServerAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	ServerAddr.sin_port = htons(PORT);

	// 绑定套接字
	if (bind(ListenSocket,
		(SOCKADDR*)&ServerAddr,
		sizeof(ServerAddr)) == SOCKET_ERROR) {
		printf("bind() failed with error: %d.\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}

	// 启动监听套接字，开始监听
	//----------------------
	// Listen for incoming connection requests.
	// on the created socket
	if (listen(ListenSocket, 5) == SOCKET_ERROR) {
		printf("listen() failed with error: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}

	// 接受连接，并关联到完成端口
	while (TRUE)
	{
		if ((AcceptSocket = WSAAccept(ListenSocket, NULL, NULL, NULL, 0)) == SOCKET_ERROR)
		{
			printf("WSAAccept() failed with error: %d\n", WSAGetLastError());
			return 1;
		}

		// 创建一个与套接字相关的套接字信息结构
		ZeroMemory(&PerHandleData, sizeof(PerHandleData));
		if ((PerHandleData = (LPPER_HANDLE_DATA)GlobalAlloc(GPTR,
			sizeof(PER_HANDLE_DATA))) == NULL)
		{
			printf("GlobalAlloc() failed with error: %d\n", GetLastError());
			return 1;
		}

		// 将接收到的套接字与现存的完成端口关联
		printf("Socket number %d connected\n", AcceptSocket);
		PerHandleData->Socket = AcceptSocket;

		if (CreateIoCompletionPort((HANDLE)AcceptSocket, CompletionPort, (DWORD)PerHandleData, 0) == NULL)
		{
			printf("CreateIoCompletionPort() failed with error: %d\n", GetLastError());
			return 1 ;
		}

		// 创建与以下WSARecv调用相关的单I/O套接字信息结构
		if ((PerIoData = (LPPER_IO_OPERATION_DATA)GlobalAlloc(GPTR, 
					sizeof(PER_IO_OPERATION_DATA))) == NULL)
		{
			printf("GlobalAlloc() failed with error: %d\n", GetLastError());
			return 1;
		}

		ZeroMemory(&(PerIoData->Overlapped), sizeof(OVERLAPPED));
		PerIoData->BytesSend = 0;
		PerIoData->BytesRecv = 0;
		PerIoData->DataBuf.len = DATA_BUFSIZE;
		PerIoData->DataBuf.buf = PerIoData->Buffer;

		dwFlags = 0;
		if (WSARecv(AcceptSocket, &(PerIoData->DataBuf), 1, &RecvBytes, &dwFlags,
					&(PerIoData->Overlapped), NULL) == SOCKET_ERROR)
		{
			if (WSAGetLastError() != ERROR_IO_PENDING)
			{
				printf("WSARecv() failed with error: %d\n", WSAGetLastError());
				return 1;
			}
		}
	}
	

	return 0;
}

int main(int argc, char *argv[])
{
	IoCompletionPort_Demo();

	return 0;
}


DWORD WINAPI ServerWorkerThread(LPVOID CompletionPortID)
{
	HANDLE CompletionPort = (HANDLE)CompletionPortID;
	DWORD BytesTransferred;
	LPOVERLAPPED Overlapped;
	LPPER_HANDLE_DATA PerHandleData;
	LPPER_IO_OPERATION_DATA PerIoData;
	DWORD SendBytes;
	DWORD RecvBytes;
	DWORD Flags;

	while (TRUE)
	{
		if (GetQueuedCompletionStatus(CompletionPort, &BytesTransferred,
			(LPDWORD)&PerHandleData, (LPOVERLAPPED*)&PerIoData, INFINITE) == 0)
		{
			printf("GetQueuedCompletionStatus failed with error: %d\n", GetLastError());
			return 0;
		}

		// 如果套接字有错，关闭套接字，并清除套接字信息结构
		if (BytesTransferred == 0)
		{
			printf("Closing socket: %d\n", PerHandleData->Socket);

			if (closesocket(PerHandleData->Socket) == SOCKET_ERROR)
			{
				printf("closesocket() failed with error: %d\n", WSAGetLastError());
				return 0;
			}

			GlobalFree(PerHandleData);
			GlobalFree(PerIoData);
			continue;
		}

		// Check to see if the ByteisRecv field equals zero. If this is so,then
		// this means a WSARecv call just completed so update the BytesRecv field
		// with the BytesTransferred value from the completed WSARecv() call.

		if (PerIoData->BytesRecv == 0)
		{
			PerIoData->BytesRecv = BytesTransferred;
			PerIoData->BytesSend = 0;
		}
		else
		{
			PerIoData->BytesSend += BytesTransferred;
		}

		if (PerIoData->BytesRecv > PerIoData->BytesSend)
		{
			// Post another WSASend() request.
			// Since WSASend() is not gauranteed to send all of the bytes requested,
			// continue posting WSASend() calls until all received bytes are sent.

			ZeroMemory(&(PerIoData->Overlapped), sizeof(OVERLAPPED));

			PerIoData->DataBuf.buf = PerIoData->Buffer + PerIoData->BytesSend;
			PerIoData->DataBuf.len = PerIoData->BytesRecv - PerIoData->BytesSend;

			printf("recv message: %s,bytes: %d\n", PerIoData->DataBuf.buf, PerIoData->DataBuf.len);

			if (WSASend(PerHandleData->Socket, &(PerIoData->DataBuf), 1, &SendBytes, 0,
				&(PerIoData->Overlapped), NULL) == SOCKET_ERROR)
			{
				if (WSAGetLastError() != ERROR_IO_PENDING)
				{
					printf("WSASend() failed with error: %d\n", WSAGetLastError());
					return 0;
				}
			}
		}
		else
		{
			PerIoData->BytesRecv = 0;

			// Now that there are no more bytes to send post another WSARecv() request.
			Flags = 0;
			ZeroMemory(&(PerIoData->Overlapped), sizeof(OVERLAPPED));

			PerIoData->DataBuf.len = DATA_BUFSIZE;
			PerIoData->DataBuf.buf = PerIoData->Buffer;

			if (WSARecv(PerHandleData->Socket, &(PerIoData->DataBuf), 1, &RecvBytes,
					&Flags, &(PerIoData->Overlapped), NULL) == SOCKET_ERROR)
			{
				if (WSAGetLastError() != ERROR_IO_PENDING)
				{
					printf("WSARecv() failed with error: %d\n", WSAGetLastError());
					return 0;
				}
			}
		}
	}
}
