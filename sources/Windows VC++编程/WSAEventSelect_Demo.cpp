// WSAEventSelect Demo

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <WinSock2.h>
#include <stdio.h>

#pragma comment(lib, "ws2_32.lib")

#define DEFAULT_BUFLEN 1024


int WSASock_Init()
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	wVersionRequested = MAKEWORD(2, 2);

	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0) {
		/* Tell the user that we could not find a usable */
		/* WinSock DLL.                                  */
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
	return 0;
}

int WSAEventSelect_Demo()
{
	WSASock_Init();

	SOCKET SocketArray[WSA_MAXIMUM_WAIT_EVENTS];		// 套接字数组
	WSAEVENT EventArray[WSA_MAXIMUM_WAIT_EVENTS];		// 事件句柄数组
	SOCKET ListenSocket;								// 监听套接字
	SOCKET AcceptSocket;								// 响应套接字
	DWORD  EventTotal = 0;								// 为上面两个数组所设置的计数器
	DWORD  Index = 0;
	WSANETWORKEVENTS	NetWorkEvents;
	SOCKADDR_IN	InternetAddr;							// 定义地址结构变量

	// 创建一个流式套接字，设置它在5150端口上监听
	// Create a SOCKET for listening for
    // incoming connection requests.
	ListenSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (ListenSocket == INVALID_SOCKET) {
		printf("Error at socket(): %ld\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}

	//----------------------
	// The sockaddr_in structure specifies the address family,
	// IP address, and port for the socket that is being bound.
	InternetAddr.sin_family = AF_INET;
	InternetAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	//InternetAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	InternetAddr.sin_port = htons(5150);

	if (bind(ListenSocket,
		(SOCKADDR*)&InternetAddr,
		sizeof(InternetAddr)) == SOCKET_ERROR) {
		printf("bind() failed.\n");
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}

	
	// 创建一个事件对象，将它与监听套接字相关联，并注册网络事件
	HANDLE NewEvent = WSACreateEvent();
	WSAEventSelect(ListenSocket, NewEvent, FD_ACCEPT | FD_CLOSE);

	// 启动监听，并将监听套接字和对应的事件添加到相应的数组中
	//----------------------
	// Listen for incoming connection requests.
	// on the created socket
	if (listen(ListenSocket, 5) == SOCKET_ERROR) {
		printf("Error listening on socket.\n");
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}

	SocketArray[EventTotal] = ListenSocket;
	EventArray[EventTotal] = NewEvent;
	EventTotal++;


	// 不断循环，等待连接请求，并处理套接字的I/O
	while (TRUE)
	{
		// 在所有的套接字上等待网络事件的发生
		Index = WSAWaitForMultipleEvents(EventTotal, EventArray, FALSE, WSA_INFINITE, FALSE);

		// 检查消息通知对应的套接字上所发生的网络事件类型
		WSAEnumNetworkEvents(SocketArray[Index - WSA_WAIT_EVENT_0], EventArray[Index - WSA_WAIT_EVENT_0], 
						&NetWorkEvents);

		// 检查FD_ACCEPT消息
		if (NetWorkEvents.lNetworkEvents & FD_ACCEPT)
		{
			if (NetWorkEvents.iErrorCode[FD_ACCEPT_BIT] != 0)
			{
				printf("FD_ACCEPT failed with error: %d\n", NetWorkEvents.iErrorCode[FD_ACCEPT_BIT]);
				break;
			}

			// 是FFD_ACCEPT消息，并且没有错误，那就接收这个新的连接请求，并把产生的套接字添加到套接字和事件数组中
			AcceptSocket = accept(SocketArray[Index - WSA_WAIT_EVENT_0], NULL, NULL);
			// 在将产生的套接字添加到套接字数组中之前，首先检查套接字数目是否超限
			// 如果超限，就关闭AcceptSocket套接字，退出
			if (EventTotal > WSA_MAXIMUM_WAIT_EVENTS)
			{
				printf("Too many Connections\n");
				closesocket(AcceptSocket);
				break;
			}

			// 为AcceptSocket套接字创建一个新的事件对象
			NewEvent = WSACreateEvent();
			// 将该事件对象与AcceptSocket套接字相关联，并注册网络事件
			WSAEventSelect(AcceptSocket, NewEvent, FD_READ | FD_WRITE | FD_CLOSE);
			// 将该套接字和对应的事件对象添加到数组中，统一管理
			EventArray[EventTotal] = NewEvent;
			SocketArray[EventTotal] = AcceptSocket;
			EventTotal++;
			printf("Socket %d connected\n", AcceptSocket);
		}

		// 处理FD_READ消息
		if (NetWorkEvents.lNetworkEvents & FD_READ)
		{
			if (NetWorkEvents.iErrorCode[FD_READ_BIT] != 0)
			{
				printf("FD_READ failed with error: %d\n", NetWorkEvents.iErrorCode[FD_READ_BIT]);
				break;
			}
			
			// 从套接字读取数据
			char recvbuf[DEFAULT_BUFLEN];
			int recvbuflen = DEFAULT_BUFLEN;

			memset(recvbuf, 0, recvbuflen);

			int iResult = recv(SocketArray[Index - WSA_WAIT_EVENT_0], recvbuf, recvbuflen, 0);
			char temp[1024] = { 0 };
			if (iResult > 0) {
				printf("Bytes received: %d,msg:%s\n", iResult, recvbuf);
			}
			else if (iResult == 0)
			{
				printf("Connection closing...\n");
			}
			else {
				printf("recv failed: %d\n", WSAGetLastError());
				closesocket(SocketArray[Index - WSA_WAIT_EVENT_0]);
				SocketArray[Index - WSA_WAIT_EVENT_0] = INVALID_SOCKET;
				WSACleanup();
				return 1;
			}
		}

		// 处理FD_WRITE消息通知
		if (NetWorkEvents.lNetworkEvents & FD_WRITE)
		{
			if (NetWorkEvents.iErrorCode[FD_WRITE_BIT] != 0)
			{
				printf("FD_WRITE failed with error: %d\n", NetWorkEvents.iErrorCode[FD_WRITE_BIT]);
				break;
			}

			// 写数据到套接字
			char sendBuf[DEFAULT_BUFLEN] = "send msg to client";
			int sendBufLen = strlen(sendBuf) + 1;

			send(SocketArray[Index - WSA_WAIT_EVENT_0], sendBuf, sizeof(sendBuf), 0);

		}

		// 处理FD_CLOSE消息通知
		if (NetWorkEvents.lNetworkEvents & FD_CLOSE)
		{
			if (NetWorkEvents.iErrorCode[FD_CLOSE_BIT] != 0)
			{
				printf("FD_CLOSE failed with error: %d\n", NetWorkEvents.iErrorCode[FD_CLOSE_BIT]);
				break;
			}

			// 关闭套接字
			closesocket(SocketArray[Index - WSA_WAIT_EVENT_0]);
			// 从套接字和事件数组中删除该套接字和相应的事件句柄，紧凑两个数组并将EventTotal计数器减1，
			// 该函数的实现省略了
			//CompressArrays(Event, Socket, &EventTotal);
		}
	} 

	return 0;
}

int main(int argc, char *argv[])
{
	WSAEventSelect_Demo();
	
	return 0;
}
