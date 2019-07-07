## [服务器中判断客户端socket断开连接的方法](https://blog.csdn.net/god2469/article/details/8801356)
下面来罗列一下判断远端已经断开的方法：
### 法一：
当recv()返回值小于等于0时，socket连接断开。但是还需要判断 errno是否等于 EINTR，如果errno == EINTR 则说明recv函数是由于程序接收到信号后返回的，socket连接还是正常的，不应close掉socket连接。

### 法二：
```cpp
  struct tcp_info info; 
  int len=sizeof(info); 
  getsockopt(sock, IPPROTO_TCP, TCP_INFO, &info, (socklen_t *)&len); 
  if((info.tcpi_state==TCP_ESTABLISHED))  则说明未断开  else 断开
```

### 法三：
若使用了select等系统函数，若远端断开，则select返回1，recv返回0则断开。其他注意事项同法一。


### 法四：
```cpp
int keepAlive = 1; // 开启keepalive属性
int keepIdle = 60; // 如该连接在60秒内没有任何数据往来,则进行探测 
int keepInterval = 5; // 探测时发包的时间间隔为5 秒
int keepCount = 3; // 探测尝试的次数.如果第1次探测包就收到响应了,则后2次的不再发.

setsockopt(rs, SOL_SOCKET, SO_KEEPALIVE, (void *)&keepAlive, sizeof(keepAlive));
setsockopt(rs, SOL_TCP, TCP_KEEPIDLE, (void*)&keepIdle, sizeof(keepIdle));
setsockopt(rs, SOL_TCP, TCP_KEEPINTVL, (void *)&keepInterval, sizeof(keepInterval));
setsockopt(rs, SOL_TCP, TCP_KEEPCNT, (void *)&keepCount, sizeof(keepCount));
```
设置后，若断开，则在使用该socket读写时立即失败，并返回ETIMEDOUT错误

### 法五：
自己实现一个心跳检测，一定时间内未收到自定义的心跳包则标记为已断开。
