## [libevent源码深度剖析一](https://blog.csdn.net/sparkliang/article/details/4957667)
## [libevent源码深度剖析二](https://blog.csdn.net/sparkliang/article/details/4957744)
## [libevent – an event notification library](http://libevent.org)
The libevent API provides a mechanism to execute a callback function when a specific event occurs on a file descriptor or after a timeout has been reached. Furthermore, libevent also support callbacks due to signals or regular timeouts.
libevent is meant to replace the event loop found in event driven network servers. An application just needs to call event_dispatch() and then add or remove events dynamically without having to change the event loop.

Currently, libevent supports /dev/poll, kqueue(2), event ports, POSIX select(2), Windows select(), poll(2), and epoll(4). The internal event mechanism is completely independent of the exposed event API, and a simple update of libevent can provide new functionality without having to redesign the applications. As a result, Libevent allows for portable application development and provides the most scalable event notification mechanism available on an operating system. Libevent can also be used for multi-threaded applications, either by isolating each event_base so that only a single thread accesses it, or by locked access to a single shared event_base. Libevent should compile on Linux, *BSD, Mac OS X, Solaris, Windows, and more.

Libevent additionally provides a sophisticated framework for buffered network IO, with support for sockets, filters, rate-limiting, SSL, zero-copy file transmission, and IOCP. Libevent includes support for several useful protocols, including DNS, HTTP, and a minimal RPC framework.

More information about event notification mechanisms for network servers can be found on Dan Kegel's "The C10K problem" web page.
A mailing list for libevent is now available. Libevent is maintained by Nick Mathewson, Azat Khuzhin and Niels Provos.

libevent源码深度剖析一
——序幕
张亮

 

### 1 前言
Libevent是一个轻量级的开源高性能网络库，使用者众多，研究者更甚，相关文章也不少。写这一系列文章的用意在于，一则分享心得；二则对libevent代码和设计思想做系统的、更深层次的分析，写出来，也可供后来者参考。

附带一句：Libevent是用c语言编写的（MS大牛们都偏爱c语言哪），而且几乎是无处不函数指针，学习其源代码也需要相当的c语言基础。

### 2 Libevent简介
上来当然要先夸奖啦，Libevent 有几个显著的亮点：
事件驱动（event-driven），高性能;
轻量级，专注于网络，不如ACE那么臃肿庞大；
源代码相当精炼、易读；
跨平台，支持Windows、Linux、*BSD和Mac Os；
支持多种I/O多路复用技术， epoll、poll、dev/poll、select和kqueue等；
支持I/O，定时器和信号等事件；
注册事件优先级；

Libevent已经被广泛的应用，作为底层的网络库；比如memcached、Vomit、Nylon、Netchat等等。
Libevent当前的最新稳定版是1.4.13；这也是本文参照的版本。

### 3 学习的好处
学习libevent有助于提升程序设计功力，除了网络程序设计方面外，Libevent的代码里有很多有用的设计技巧和基础数据结构，比如信息隐藏、函数指针、c语言的多态支持、链表和堆等等，都有助于提升自身的程序功力。
程序设计不止要了解框架，很多细节之处恰恰也是事关整个系统成败的关键。只对libevent本身的框架大概了解，那或许仅仅是一知半解，不深入代码分析，就难以了解其设计的精巧之处，也就难以为自己所用。

事实上Libevent本身就是一个典型的Reactor模型，理解Reactor模式是理解libevent的基石；因此下一节将介绍典型的事件驱动设计模式——Reactor模式。

### 参考资料：
* Libevent: http://monkey.org/~provos/libevent/

