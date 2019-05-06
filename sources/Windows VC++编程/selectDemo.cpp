#include <WinSock2.h>

#include <windows.h>
#include <stdio.h>

#pragma comment(lib, "ws2_32.lib")

#define DEFAULT_BUFLEN 512

int select_Demo()
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

	//SOCKET sock;		// 定义一个套接字
	fd_set fdread;		// 定义一个套接字集合变量
	int ret;			// 返回值

	// 创建一个套接字，并接受连接
	//----------------------
  // Create a SOCKET for listening for
  // incoming connection requests.
	SOCKET ListenSocket;
	ListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (ListenSocket == INVALID_SOCKET) {
		printf("Error at socket(): %ld\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}

	//----------------------
	// The sockaddr_in structure specifies the address family,
	// IP address, and port for the socket that is being bound.
	sockaddr_in service;
	service.sin_family = AF_INET;
	service.sin_addr.s_addr = inet_addr("127.0.0.1");
	service.sin_port = htons(27015);

	if (bind(ListenSocket,
		(SOCKADDR*)&service,
		sizeof(service)) == SOCKET_ERROR) {
		printf("bind() failed.\n");
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}

	//----------------------
	// Listen for incoming connection requests.
	// on the created socket
	if (listen(ListenSocket, 1) == SOCKET_ERROR) {
		printf("Error listening on socket.\n");
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}

	//----------------------
	// Create a SOCKET for accepting incoming requests.
	SOCKET AcceptSocket;
	printf("Waiting for client to connect...\n");

	//----------------------
	// Accept the connection.
	AcceptSocket = accept(ListenSocket, NULL, NULL);
	if (AcceptSocket == INVALID_SOCKET) {
		printf("accept failed: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}
	else
		printf("Client connected.\n");

	
	// 管理该套接字上的I/O
	while (TRUE)
	{
		// 在调用select函数之前，总是要清除套接字集合变量
		FD_ZERO(&fdread);
		// 将套接字AcceptSocket添加到fdread集合中
		FD_SET(AcceptSocket, &fdread);
		// 调用select()函数，并等待它的完成，这里只是想检查AcceptSocket是否有数据可读
		if ((ret = select(0, &fdread, NULL,NULL,NULL)) == SOCKET_ERROR)
		{
			// 处理错误的代码
			printf("select() called error:%ld\n", WSAGetLastError());
		}

		// 返回值大于零，说明有符合条件的套接字
		if (ret > 0)
		{
			if (FD_ISSET(AcceptSocket, &fdread))
			{
				// 对该套接字进行读操作
				char recvbuf[DEFAULT_BUFLEN];
				int iResult, iSendResult;
				int recvbuflen = DEFAULT_BUFLEN;
				memset(recvbuf, 0, recvbuflen);

				// Receive until the peer shuts down the connection
				do {
					iResult = recv(AcceptSocket, recvbuf, recvbuflen, 0);
					if (iResult > 0) {
						printf("Bytes received: %d\n", iResult);
						printf("recv msg:%s\n", recvbuf);

						// Echo the buffer back to the sender
						iSendResult = send(AcceptSocket, recvbuf, iResult, 0);
						if (iSendResult == SOCKET_ERROR) {
							printf("send failed: %d\n", WSAGetLastError());
							closesocket(AcceptSocket);
							WSACleanup();
							return 1;
						}
						printf("Bytes sent: %d\n", iSendResult);
					} else if (iResult == 0)
					{
						printf("Connection closing...\n");
					} else {
						printf("recv failed: %d\n", WSAGetLastError());
						closesocket(AcceptSocket);
						WSACleanup();
						return 1;
					}

				} while (iResult > 0);
			}
		}
	}


	// No longer need server socket
	closesocket(ListenSocket);
	WSACleanup();

	return 0;
} 

int main(int argc, char *argv[])
{
	select_Demo();

	return 0;
}
