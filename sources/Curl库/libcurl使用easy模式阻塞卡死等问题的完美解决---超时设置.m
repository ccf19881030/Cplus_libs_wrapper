## [libcurl使用easy模式阻塞卡死等问题的完美解决---超时设置](https://www.cnblogs.com/fnlingnzb-learner/p/6018433.html)
### libcurl使用时疑难问题：
在使用libcurl时, jwisp发现, curl_easy_perform是阻塞的方式进行下载的, curl_easy_perform执行后,程序会在这里阻塞等待下载结束(成功结束或者失败结束).此时若正常下载一段时间后,进行网络中断, curl_easy_perform并不会返回失败,而是阻塞整个程序卡在这里,此时即使网络连接重新恢复, curl_easy_perform也无法恢复继续下载,导致整个程序出现”死机”状态.

但是若先断网, 然后进行curl_easy_perform的话,会直接返回失败,不会阻塞

在网上搜索后发现, 大家在网上遇到这个问题的很多,但是解决方法很少,下面jwisp就把网上建议的可以使用的解决方法罗列:

1. 使用multi模式下载,而不使用easy模式,此方法的唯一好处就是multi并不会阻塞,而是立即返回.但是缺点是带来了问题,其一就是需要自己去阻塞,当我们需要返回时再返回,其二还需要启动一个线程,需要自己控制整个过程的节奏.

2. 在下载中,另起一个线程,若发现下载状态卡死(可以通过定期检查文件大小来实现),则从外部中断下载线程.此方法需另起线程,而且直接中断线程,会给整个程序带来不稳定.

主要的设计思路如下, 下载过程中,设置超时时间为30秒, 30秒后若下载未完成就重新连接进行下载(这个可解决卡死问题)。

超时代码设计如下：
```c
curl_easy_setopt( curl, CURLOPT_VERBOSE, 1L ); //在屏幕打印请求连接过程和返回http数据
curl_easy_setopt( curl, CURLOPT_TIMEOUT, 10 );//接收数据时超时设置，如果10秒内数据未接收完，直接退出
curl_easy_setopt(curl, CURLOPT_AUTOREFERER, 1); // 以下3个为重定向设置
curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1); //返回的头部中有Location(一般直接请求的url没找到)，则继续请求Location对应的数据 
curl_easy_setopt(curl, CURLOPT_MAXREDIRS, 1);//查找次数，防止查找太深
curl_easy_setopt( curl, CURLOPT_CONNECTTIMEOUT, 3 );//连接超时，这个数值如果设置太短可能导致数据请求不到就断开了
```
