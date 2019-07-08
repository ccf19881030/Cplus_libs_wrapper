// 设置socket属性值（保活、发送时限和接收时限）
```cpp
void DataUpload::set_socket_options()
{
	// the timeout value
	unsigned int timeout_milli = 10000;	// 10s

	// platform-specific switch
#if defined _WIN32 || defined WIN32 || defined OS_WIN64 || defined _WIN64 || defined WIN64 || defined WINNT
  // use windows-specific time

	if (socket_.is_open())
	{
		int32_t client_socket_handle = socket_.native_handle();

		int32_t timeout = timeout_milli;
		//发送时限 
		setsockopt(client_socket_handle, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout));
		//接收时限 
		setsockopt(client_socket_handle, SOL_SOCKET, SO_SNDTIMEO, (const char*)&timeout, sizeof(timeout));

		// Added Keepalive flag
		// 保持长连接
		boost::asio::socket_base::keep_alive opt_keep_alive(true);
		socket_.set_option(opt_keep_alive);

		DWORD retBytes;
		tcp_keepalive inKeepSetting;
		tcp_keepalive retKeepSetting;

		inKeepSetting.onoff = 3;					//探测次数
		inKeepSetting.keepalivetime = 5500;        // 首次探测开始前的tcp无数据收发空闲时间
		inKeepSetting.keepaliveinterval = 3000;    // 每次探测的间隔时间

		if (WSAIoctl(client_socket_handle, SIO_KEEPALIVE_VALS,
			&inKeepSetting,
			sizeof(inKeepSetting),
			&retKeepSetting,
			sizeof(retKeepSetting),
			&retBytes,
			NULL,
			NULL) != 0)
		{
			printf("WSAIoctl Error: %d/n", WSAGetLastError());
		}
	}

#else
  // assume everything else is posix
	int32_t client_socket_handle = socket_.native_handle();

	struct timeval tv;
	tv.tv_sec = timeout_milli / 1000;
	tv.tv_usec = (timeout_milli % 1000) * 1000;
	setsockopt(client_socket_handle, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
	setsockopt(client_socket_handle, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv));

	// 以秒为单位
	int keepAlive = 1;       //设定KeepAlive 
	int keepIdle = 60;		// 如该连接在60秒内没有任何数据往来,则进行探测 
	int keepInterval = 5;	// 探测时发包的时间间隔为5 秒
	int keepCount = 3;		// 探测尝试的次数.如果第1次探测包就收到响应了,则后2次的不再发.

	setsockopt(client_socket_handle, SOL_SOCKET, SO_KEEPALIVE, (const char*)&keepAlive, sizeof(keepAlive));
	setsockopt(client_socket_handle, SOL_TCP, TCP_KEEPIDLE, (const char*)&keepIdle, sizeof(keepIdle));
	setsockopt(client_socket_handle, SOL_TCP, TCP_KEEPCNT, (const char*)&keepCount, sizeof(keepCount));
	setsockopt(client_socket_handle, SOL_TCP, TCP_KEEPINTVL, (const char*)&keepInterval, sizeof(keepInterval));
#endif
}
```
