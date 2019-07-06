## [libevent简介和使用](https://www.open-open.com/lib/view/open1386510630330.html)
libevent是一个基于事件触发的网络库，memcached底层也是使用libevent库。

总体来说，libevent有下面一些特点和优势：

事件驱动，高性能；
轻量级，专注于网络； 
跨平台，支持 Windows、Linux、Mac Os等； 
支持多种 I/O多路复用技术， epoll、poll、dev/poll、select 和kqueue 等； 
* 支持 I/O，定时器和信号等事件；

 

libevent有下面几大部分组成：

* 事件管理包括各种IO（socket）、定时器、信号等事件，也是libevent应用最广的模块；

* 缓存管理是指evbuffer功能；

* DNS是libevent提供的一个异步DNS查询功能；

* HTTP是libevent的一个轻量级http实现，包括服务器和客户端

 

一些资料：

libevent官网：http://libevent.org/&nbsp;
libevent API：http://www.monkey.org/~provos/libevent/doxygen-2.0.1/index.html
CSDN上剖析得很赞的文章：http://blog.csdn.net/sparkliang/article/details/4957667</p>

 

// =============================================================================================

 

下面写了2个简单的使用例子，一个是定时器，一个是TCP服务器，都只涉及到libevent的事件管理模块。

 

一、简单定时器：实现程序每秒输出一个“Game Over!”

event_init() => evtimer_set() => event_add() =>event_dispatch()
```cpp
  #include <stdio.h>  
  #include <iostream>  

  // libevent头文件  
  #include <event.h>  
  using namespace std;  

  // 定时事件回调函数  
  void onTime(int sock, short event, void *arg)  
  {  
      cout << "Game Over!" << endl;  

      struct timeval tv;  
      tv.tv_sec = 1;  
      tv.tv_usec = 0;  
      // 重新添加定时事件（定时事件触发后默认自动删除）  
      event_add((struct event*)arg, &tv);  
  }  

  int main()  
  {  
      // 初始化  
      event_init();  

      struct event evTime;  
      // 设置定时事件  
      evtimer_set(&evTime, onTime, &evTime);  

      struct timeval tv;  
      tv.tv_sec = 1;  
      tv.tv_usec = 0;  
      // 添加定时事件  
      event_add(&evTime, &tv);  

      // 事件循环  
      event_dispatch();  

      return 0;  
  }  
  ```
  编译并执行，编译加 -levent：
    gapp_devnet_1:/data/home/andyawang/code/2013_11/LibeventTest # mv time.cpp timer.cpp  
  gapp_devnet_1:/data/home/andyawang/code/2013_11/LibeventTest # g++ -o timer timer.cpp -levent  
  gapp_devnet_1:/data/home/andyawang/code/2013_11/LibeventTest # ./timer   
  Game Over!  
  Game Over!  
  Game Over!  
  Game Over!  二、TCP服务器：实现监听本机8888端口并输出客户端发送过来的信息
event_base_new()=>event_set()=>event_base_set()=>event_add()=>event_base_dispatch()
```cpp
  #include <stdio.h>  
  #include <string.h>  
  #include <iostream>  
  #include <sys/socket.h>      
  #include <netinet/in.h>      
  #include <arpa/inet.h>      
  #include <netdb.h>  

  #include <event.h>  
  using namespace std;  

  // 事件base  
  struct event_base* base;  

  // 读事件回调函数  
  void onRead(int iCliFd, short iEvent, void *arg)  
  {  
      int iLen;  
      char buf[1500];  

      iLen = recv(iCliFd, buf, 1500, 0);  

      if (iLen <= 0) {  
          cout << "Client Close" << endl;  

          // 连接结束(=0)或连接错误(<0)，将事件删除并释放内存空间  
          struct event *pEvRead = (struct event*)arg;  
          event_del(pEvRead);  
          delete pEvRead;  

          close(iCliFd);  
          return;  
      }  

      buf[iLen] = 0;  
      cout << "Client Info:" << buf << endl;  
  }  

  // 连接请求事件回调函数  
  void onAccept(int iSvrFd, short iEvent, void *arg)  
  {  
      int iCliFd;  
      struct sockaddr_in sCliAddr;  

      socklen_t iSinSize = sizeof(sCliAddr);  
      iCliFd = accept(iSvrFd, (struct sockaddr*)&sCliAddr, &iSinSize);  

      // 连接注册为新事件 (EV_PERSIST为事件触发后不默认删除)  
      struct event *pEvRead = new event;  
      event_set(pEvRead, iCliFd, EV_READ|EV_PERSIST, onRead, pEvRead);  
      event_base_set(base, pEvRead);  
      event_add(pEvRead, NULL);  
  }  

  int main()  
  {  

      int iSvrFd;    
      struct sockaddr_in sSvrAddr;  

      memset(&sSvrAddr, 0, sizeof(sSvrAddr));    
      sSvrAddr.sin_family = AF_INET;    
      sSvrAddr.sin_addr.s_addr = inet_addr("127.0.0.1");      
      sSvrAddr.sin_port = htons(8888);     

      // 创建tcpSocket（iSvrFd），监听本机8888端口    
      iSvrFd = socket(AF_INET, SOCK_STREAM, 0);    
      bind(iSvrFd, (struct sockaddr*)&sSvrAddr, sizeof(sSvrAddr));    
      listen(iSvrFd, 10);  

      // 初始化base  
      base = event_base_new();  

      struct event evListen;  
      // 设置事件  
      event_set(&evListen, iSvrFd, EV_READ|EV_PERSIST, onAccept, NULL);  
      // 设置为base事件  
      event_base_set(base, &evListen);  
      // 添加事件  
      event_add(&evListen, NULL);  

      // 事件循环  
      event_base_dispatch(base);  

      return 0;  
  }  
 ```
  来自：http://blog.csdn.net/yyyiran/article/details/12219737
 本文由用户 jopen 自行上传分享，仅供网友学习交流。所有权归原作者，若您的权利被侵害，请联系管理员。
 转载本站原创文章，请注明出处，并保留原始链接、图片水印。
 本站是一个以用户分享为主的开源技术平台，欢迎各类分享！
 本文地址：https://www.open-open.com/lib/view/open1386510630330.html
libevent 网络工具包
