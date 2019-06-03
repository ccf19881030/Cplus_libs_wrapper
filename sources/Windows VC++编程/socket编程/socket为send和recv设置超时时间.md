## [socket为send和recv设置超时时间](https://www.cnblogs.com/lidabo/p/3804245.html)
linux和windows下用setsockopt设置SO_SNDTIMEO,SO_RCVTIMEO的参数的一点区别  
UDP的socket在某些情况：如对方关闭时，本地可能sendto不出去数据，然后recvfrom就会被阻塞，这时就需要设置 这两个参数的值提高程序质量。

linux:
```cpp
 struct timeval timeout={3,0};//3s
 int ret=setsockopt(sock_fd,SOL_SOCKET,SO_SNDTIMEO,(const char*)&timeout,sizeof(timeout));
 int ret=setsockopt(sock_fd,SOL_SOCKET,SO_RCVTIMEO,(const char*)&timeout,sizeof(timeout));
 ```
 如果ret==0 则为成功,-1为失败,这时可以查看errno来判断失败原因
 ```cpp
 int recvd=recv(sock_fd,buf,1024,0);
 if(recvd==-1&&errno==EAGAIN)
 {
    printf("timeout\n");
}
```
windows:
```cpp
int timeout = 3000; //3s
int ret=setsockopt(sock_fd,SOL_SOCKET,SO_SNDTIMEO,&timeout,sizeof(timeout));
int ret=setsockopt(sock_fd,SOL_SOCKET,SO_RCVTIMEO,&timeout,sizeof(timeout));
```
而solaris，则不支持。
