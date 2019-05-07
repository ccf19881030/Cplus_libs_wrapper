// WSAAsyncSelect Demo
#include <windows.h>
#include <WinSock2.h>
#include <stdio.h>

#pragma comment(lib, "ws2_32.lib")

#define WM_SOCKET WM_USER + 1	// 自定义一个消息

#define DEFAULT_BUFLEN 1024


//声明回调函数
LRESULT CALLBACK WinProc(
	HWND hwnd,      // handle to window
	UINT uMsg,      // message identifier
	WPARAM wParam,  // first message parameter
	LPARAM lParam   // second message parameter
);

//定义WinMian函数
int WINAPI WinMain(
	HINSTANCE hInstance,      // handle to current instance
	HINSTANCE hPrevInstance,  // handle to previous instance
	LPSTR lpCmdLine,          // command line
	int nCmdShow              // show state
)
{
	//设计一个窗口类
	WNDCLASS wndcls;
	wndcls.cbClsExtra = 0;
	wndcls.cbWndExtra = 0;
	wndcls.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndcls.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndcls.hIcon = NULL;
	//wndcls.hIcon=LoadIcon(NULL,IDI_ERROR);
	wndcls.hInstance = hInstance;
	wndcls.lpfnWndProc = WinProc;
	wndcls.lpszClassName = "FirstWindowApp";
	wndcls.lpszMenuName = NULL;
	wndcls.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&wndcls);

	//创建窗口，并保存成功创建窗口后返回的句柄
	HWND hwnd;
	hwnd = CreateWindow("FirstWindowApp", "FirstWindowApp", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 600, 400, NULL, NULL, hInstance, NULL);

	// 初始化WinSock，并创建套接字
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

	SOCKET ListenSocket;		// 定义监听套接字

	// 创建监听套接字
	ListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (ListenSocket == INVALID_SOCKET) {
		printf("Error at socket(): %ld\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}

	//----------------------
	SOCKADDR_IN	servAddr;	// 定义地址结构变量
 // The sockaddr_in structure specifies the address family,
 // IP address, and port for the socket that is being bound.
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	//servAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servAddr.sin_port = htons(5150);
	
	//----------------------
	// Bind the socket.
	if (bind(ListenSocket,
		(SOCKADDR*)&servAddr,
		sizeof(servAddr)) == SOCKET_ERROR) {
		printf("bind() failed.\n");
		closesocket(ListenSocket);
		return 1;
	}

	// 对于监听套接字，使用上面定义的WM_SOCKET，调用WSAAsyncSelect函数
	WSAAsyncSelect(ListenSocket, hwnd, WM_SOCKET, FD_ACCEPT | FD_CLOSE);
	listen(ListenSocket, 5);	// 启动套接字监听

	//显示及更新窗口
	ShowWindow(hwnd, SW_SHOWNORMAL);
	UpdateWindow(hwnd);

	//定义消息结构体，开始消息循环
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

//定义窗口回调函数
LRESULT CALLBACK WinProc(
	HWND hwnd,      // handle to window
	UINT uMsg,      // message identifier
	WPARAM wParam,  // first message parameter
	LPARAM lParam   // second message parameter
)
{
	SOCKET AcceptSocket = SOCKET_ERROR;	// 定义服务器端的响应套接字

	switch (uMsg)
	{
	case WM_SOCKET:
		// 使用WSAGETSELECTERROR()宏来决定在套接字上是否发生错误
		if (WSAGETSELECTERROR(lParam))
		{
			// 显示错误消息并且关闭套接字
			MessageBox(hwnd, "socket error", "message", 0);
			//closesocket(wParam);
			break;
		}

		// 决定在该套接字上出现了什么事件
		switch (WSAGETSELECTEVENT(lParam))
		{
		case FD_ACCEPT:
			// 表示监听套接字收到了一个连接请求，接受它，产生响应套接字
			//----------------------
			// Accept the connection.
			AcceptSocket = accept(wParam, NULL, NULL);
			if (AcceptSocket == INVALID_SOCKET) {
				char tmp[512] = { 0 };
				sprintf(tmp, "accept failed: %d", WSAGetLastError());
				MessageBox(hwnd, tmp, "accept msg", 0);
				closesocket(wParam);
				WSACleanup();
				return 1;
			}
			else
			{
				MessageBox(hwnd, "Client connected.", "accept msg", 0);
			}
				
			// 为响应套接字注册read、write和close事件，启动消息通知
			WSAAsyncSelect(AcceptSocket, hwnd, WM_SOCKET, FD_READ | FD_WRITE | FD_CLOSE);
			break;
		case FD_READ:
			// 数据已到，可从wParam中的套接字接收数据
			SOCKET recvSocket = (SOCKET)wParam;

			// 对该套接字进行读操作
			char recvbuf[DEFAULT_BUFLEN];
			int iResult = -1;
			int recvbuflen = DEFAULT_BUFLEN;
			memset(recvbuf, 0, recvbuflen);

			iResult = recv(recvSocket, recvbuf, recvbuflen, 0);
			char temp[1024] = { 0 };
			if (iResult > 0) {
				sprintf(temp, "Bytes received: %d,msg:%s\n", iResult, recvbuf);
				MessageBox(hwnd, temp, "recv msg", 0);
			}
			else if (iResult == 0)
			{
				MessageBox(hwnd, "Connection closing...\n", "recv msg", 0);
			}
			else {
				sprintf(temp, "recv failed: %d\n", WSAGetLastError());
				MessageBox(hwnd, temp, "recv msg", 0);
				closesocket(AcceptSocket);
				WSACleanup();
				return 1;
			}
			break;
		case FD_WRITE:
			// 表示在wParam中的套接字已经准备好发送数据
			break;
		case FD_CLOSE:
			// 表示连接已经关闭，可关闭套接字
			closesocket(wParam);
			break;
		default:
			break;
		}
	case WM_CHAR:
		char szChar[20];
		sprintf(szChar, "char code is %d", wParam);
		MessageBox(hwnd, szChar, "char", 0);
		break;
	case WM_LBUTTONDOWN:
		MessageBox(hwnd, "mouse clicked", "message", 0);
		HDC hdc;
		hdc = GetDC(hwnd);
		TextOut(hdc, 0, 50, "Hello Windows", strlen("Hello Windows"));
		//ReleaseDC(hwnd,hdc);
		break;
	case WM_PAINT:
		HDC hDC;
		PAINTSTRUCT ps;
		hDC = BeginPaint(hwnd, &ps);
		TextOut(hDC, 0, 0, "FirstWindowApp", strlen("FirstWindowApp"));
		EndPaint(hwnd, &ps);
		break;
	case WM_CLOSE:
		if (IDYES == MessageBox(hwnd, "是否真的退出？", "message", MB_YESNO))
		{
			DestroyWindow(hwnd);
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	return 0;
}
