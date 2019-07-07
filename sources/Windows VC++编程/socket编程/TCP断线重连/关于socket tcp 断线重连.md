## [关于socket tcp 断线重连](https://www.cnblogs.com/zhangdongsheng/p/4817625.html)
这个问题困扰过我几次，都没有来得及研究，今天研究一下。

首先写一个最简易的socket tcp程序，连接成功后再关闭服务器然后再用客户端各种操作看是什么情况
测试表明
（1）客户端已经连接，当服务端关闭程序时，客户端调用send函数发送失败，WSAGetLastError() 返回10054(远程主机强迫关闭了一个现有的连接) 

（2）客户端已经连接，当客户端关闭程序时，服务端调用recv函数接收失败，WSAGetLastError() 返回10054(远程主机强迫关闭了一个现有的连接) ,这时对这个客户端的socket调用select函数时，会立即返回可读。

（3）客户端已经连接，当服务端关闭程序时，客户端调用recv函数接收失败，WSAGetLastError() 返回10054

（4）WSAAsyncSelect模式的服务端，当客户端软件关闭时，服务器收到的消息为FD_CLOSE，错误代码为10053（您的主机中的软件中止了一个已建立的连接）

### 关于客户端断线重连
粗略的做法是，当客户端调用send或recv失败，然后WSAGetLastError()判断错误编号，然后关闭socket。再创建一个socket。然后进行connect操作。
