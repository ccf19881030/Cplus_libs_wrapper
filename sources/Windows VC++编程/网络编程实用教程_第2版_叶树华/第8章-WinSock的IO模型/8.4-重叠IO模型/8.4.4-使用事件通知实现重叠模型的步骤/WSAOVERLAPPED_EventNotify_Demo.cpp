// 8.4.4 使用事件通知实现重叠模型的步骤
// WSAOVERLAPPED Event notify Demo

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <WinSock2.h>
#include <stdio.h>

#pragma comment(lib, "ws2_32.lib")

#define DATA_BUFSIZE 4096			// 接收缓冲区大小


int WSAOVERLAPPED_EventNotify_Demo()
{
	// 1、定义变量
	SOCKET ListenSocket;								// 监听套接字
	SOCKET AcceptSocket;								// 响应套接字(与客户端进行通信)
	WSAOVERLAPPED AcceptOverlapped;						// 重叠结构
	WSAEVENT EventArray[WSA_MAXIMUM_WAIT_EVENTS];		// 用来通知重叠操作完成的事件句柄数组
	WSABUF DataBuf;						
	DWORD  dwEventTotal = 0;							// 程序中事件的总数
	DWORD  dwRecvBytes = 0;								// 接收到的字符长度
	DWORD  dwFlags = 0;									// WSARecv的参数
	
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
	SOCKADDR_IN	ServerAddr;							// 定义地址结构变量
	// 分配端口及协议族并绑定
	ServerAddr.sin_family = AF_INET;
	//ServerAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	ServerAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	ServerAddr.sin_port = htons(5150);

	// 绑定套接字
	if (bind(ListenSocket,
		(SOCKADDR*)&ServerAddr,
		sizeof(ServerAddr)) == SOCKET_ERROR) {
		printf("bind() failed.\n");
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}

	// 开始监听
	//----------------------
	// Listen for incoming connection requests.
	// on the created socket
	if (listen(ListenSocket, 5) == SOCKET_ERROR) {
		printf("Error listening on socket.\n");
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}

	// 3、接受一个客户端的连接请求
	SOCKADDR_IN ClientAddr;				// 定义一个客户端的地址结构作为参数
	int addrLen = sizeof(ClientAddr);
	AcceptSocket = accept(ListenSocket, (SOCKADDR*)&ClientAddr, &addrLen);
	LPCTSTR lpIP = inet_ntoa(ClientAddr.sin_addr);		// 获取客户端的IP
	UINT nPort = ClientAddr.sin_port;

	// 4、建立并初始化重叠结构
	// 创建一个事件对象，将它与监听套接字相关联
	EventArray[dwEventTotal] = WSACreateEvent();			// dwEventTotal初始值为0
	ZeroMemory(&AcceptOverlapped, sizeof(WSAOVERLAPPED));	// 置零
	AcceptOverlapped.hEvent = EventArray[dwEventTotal];		// 关联事件
	char buffer[DATA_BUFSIZE];
	ZeroMemory(buffer, DATA_BUFSIZE);

	DataBuf.len = DATA_BUFSIZE;
	DataBuf.buf = buffer;									// 初始化一个WSABUF结构
	dwEventTotal++;											// 事件总数加1
	
	// 在套接字上不断投递WSARecv请求
	while (TRUE)
	{
		// 5、以WSAOVERLAPPED结构为参数，在套接字上投递WSARecv请求
		if (WSARecv(AcceptSocket, &DataBuf, 1, &dwRecvBytes, &dwFlags,
			&AcceptOverlapped, NULL) == SOCKET_ERROR)
		{
			// 返回WSA_IO_PENDING是正常情况，表示IO操作正在进行，不能立即完成
			// 如果不是返回WSA_IO_PENDING错误，就有问题了
			if (WSAGetLastError() != WSA_IO_PENDING)
			{
				closesocket(AcceptSocket);
				WSACloseEvent(EventArray[dwEventTotal]);
			}
		}


		// 6、调用WSAWaitForMultipleEvents函数，等待重叠操作返回的结果
		DWORD dwIndex;
		// 等待重叠I/O调用结束
		// 因为我们把事件和Overlapped绑定在一起，重叠操作完成后我们会接到事件通知
		dwIndex = WSAWaitForMultipleEvents(dwEventTotal, EventArray, FALSE, WSA_INFINITE, FALSE);
		// 注意这里返回的Index并非是事件在数组里的Index,而是需要减去WSA_WAIT_EVENT_0
		dwIndex = dwIndex - WSA_WAIT_EVENT_0;

		// 7、使用WSAResetEvent函数重设当前这个用完的事件对象
		WSAResetEvent(EventArray[dwIndex]);

		// 8、使用WSAGetOverlappedResult函数取得重叠调用的返回状态
		DWORD dwBytesTransferred;
		WSAGetOverlappedResult(AcceptSocket, &AcceptOverlapped, &dwBytesTransferred, FALSE, &dwFlags);
		// 先检查通信对方是否已经关闭连接，如果连接已经关闭，则关闭套接字
		if (dwBytesTransferred == 0)
		{
			closesocket(AcceptSocket);
			WSACloseEvent(EventArray[dwIndex]);
			return 1;
		}

		// 9、使用接收到的数据
		printf("recv msg: %s\n", DataBuf.buf);
	}

	return 0;
}

int main(int argc, char *argv[])
{
	WSAOVERLAPPED_EventNotify_Demo();

	return 0;
}
