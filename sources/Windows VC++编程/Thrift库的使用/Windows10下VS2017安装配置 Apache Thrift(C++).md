## 一、Apache Thrift 在Windows10系统下VS2017中的安装与开发
### 1、[Apache Thrift 在Windows下的安装与开发](https://blog.csdn.net/colouroo/article/details/38588297)
### 2、[Windows 10 Visual Studio 2017 安装配置 Apache Thrift （C++）](http://www.cnblogs.com/49er/p/7193829.html)
### 3、[Apache Thrift 在Windows下的安装与开发](https://www.cnblogs.com/lpxblog/p/5382007.html)
### 4、[旭日东升-thrift配置——windows客户端与linux服务端通信（C++）](https://www.cnblogs.com/lpxblog/p/5381816.html)
### 5、[Thrift在Windows及Linux平台下的安装和使用示例](https://www.cnblogs.com/lizhenghn/p/5247266.html)

## [VS2017编译thrift0.12.0记录](https://blog.csdn.net/SHIXINGYA/article/https://www.cnblogs.com/lpxblog/p/5381816.htmltails/89390308)
### 1、[http://thrift.apache.org/download](http://thrift.apache.org/download)
#### [apache thrift各个版本下载](http://archive.apache.org/dist/thrift/)
#### [thrift-0.12.0.tar.gz](http://www.apache.org/http://archive.apache.org/dist/thrift/dyn/closer.cgi?path=/thrift/0.12.0/thrift-0.12.0.tar.gz)
#### [Thrift compiler for Windows (thrift-0.12.0.exe)](http://www.apache.org/dyn/closer.cgi?path=/thrift/0.12.0/thrift-0.12.0.exe)
### 2、libevent
The libevent API provides a mechanism to execute a callback function when a specific event occurs on a file descriptor or after a timeout has been reached. Furthermore, libevent also support callbacks due to signals or regular timeouts.
libevent is meant to replace the event loop found in event driven network servers. An application just needs to call event_dispatch() and then add or remove events dynamically without having to change the event loop.

Currently, libevent supports /dev/poll, kqueue(2), event ports, POSIX select(2), Windows select(), poll(2), and epoll(4). The internal event mechanism is completely independent of the exposed event API, and a simple update of libevent can provide new functionality without having to redesign the applications. As a result, Libevent allows for portable application development and provides the most scalable event notification mechanism available on an operating system. Libevent can also be used for multi-threaded applications, either by isolating each event_base so that only a single thread accesses it, or by locked access to a single shared event_base. Libevent should compile on Linux, *BSD, Mac OS X, Solaris, Windows, and more.

Libevent additionally provides a sophisticated framework for buffered network IO, with support for sockets, filters, rate-limiting, SSL, zero-copy file transmission, and IOCP. Libevent includes support for several useful protocols, including DNS, HTTP, and a minimal RPC framework.

More information about event notification mechanisms for network servers can be found on Dan Kegel's "The C10K problem" web page.
A mailing list for libevent is now available. Libevent is maintained by Nick Mathewson, Azat Khuzhin and Niels Provos.

#### 1、[libevent官网](http://libevent.org)
#### 2、[libevent下载](https://github.com/libevent/libevent/releases/download/release-2.1.8-stable/libevent-2.1.8-stable.tar.gz)
