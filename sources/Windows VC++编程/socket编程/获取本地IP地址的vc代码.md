## [获取本地IP地址的vc代码](https://blog.csdn.net/clever101/article/details/52893643#)
### 获取本地IP地址有两种做法。
#### 1、一种是使用gethostname函数，代码如下：
```cpp
bool CSocketComm::GetLocalAddress(std::string& strAddress)
{
	char strHost[HOSTNAME_SIZE] = { 0 };
 
	// get host name, if fail, SetLastError is called
	if (SOCKET_ERROR != gethostname(strHost, sizeof(strHost)))
	{
		struct hostent* hp;
		hp = gethostbyname(strHost);
		if (hp != NULL && hp->h_addr_list[0] != NULL)
		{
			// IPv4: Address is four bytes (32-bit)
			if ( hp->h_length < 4)
				return false;
 
			// Convert address to . format
			strHost[0] = 0;
 
			// IPv4: Create Address string
			sprintf(strHost, "%u.%u.%u.%u",
				(UINT)(((PBYTE) hp->h_addr_list[0])[0]),
				(UINT)(((PBYTE) hp->h_addr_list[0])[1]),
				(UINT)(((PBYTE) hp->h_addr_list[0])[2]),
				(UINT)(((PBYTE) hp->h_addr_list[0])[3]));
 
			strAddress = strHost;
			return true;
		}
	}
	else
		SetLastError(ERROR_INVALID_PARAMETER);
 
	return false;
}
```
这种方法有一个弊端，就是只能获取一个网卡的ip地址，而且当你的机器上装有以太网卡和无线网卡，它优先获取的是无线网卡的地址。

### 2、第二种做法可以获取多个网卡的IP地址，代码如下：
```cpp
#include <Iphlpapi.h>
#pragma comment(lib,"Iphlpapi.lib") //需要添加Iphlpapi.lib库
 
bool CSocketComm::GetLocalAddress()
{
std::string strAddress;
int nCardNo = 1;
	//PIP_ADAPTER_INFO结构体指针存储本机网卡信息
	PIP_ADAPTER_INFO pIpAdapterInfo = new IP_ADAPTER_INFO();
	//得到结构体大小,用于GetAdaptersInfo参数
	unsigned long stSize = sizeof(IP_ADAPTER_INFO);
	//调用GetAdaptersInfo函数,填充pIpAdapterInfo指针变量;其中stSize参数既是一个输入量也是一个输出量
	int nRel = GetAdaptersInfo(pIpAdapterInfo,&stSize);
	//记录网卡数量
	int netCardNum = 0;
	//记录每张网卡上的IP地址数量
	int IPnumPerNetCard = 0;
	if (ERROR_BUFFER_OVERFLOW == nRel)
	{
		//如果函数返回的是ERROR_BUFFER_OVERFLOW
		//则说明GetAdaptersInfo参数传递的内存空间不够,同时其传出stSize,表示需要的空间大小
		//这也是说明为什么stSize既是一个输入量也是一个输出量
		//释放原来的内存空间
		delete pIpAdapterInfo;
		//重新申请内存空间用来存储所有网卡信息
		pIpAdapterInfo = (PIP_ADAPTER_INFO)new BYTE[stSize];
		//再次调用GetAdaptersInfo函数,填充pIpAdapterInfo指针变量
		nRel=GetAdaptersInfo(pIpAdapterInfo,&stSize);    
	}
	if (ERROR_SUCCESS == nRel)
	{
		//输出网卡信息
		//可能有多网卡,因此通过循环去判断
		while (pIpAdapterInfo)
		{
			//可能网卡有多IP,因此通过循环去判断
			IP_ADDR_STRING *pIpAddrString =&(pIpAdapterInfo->IpAddressList);
			switch(pIpAdapterInfo->Type)
			{
			case MIB_IF_TYPE_OTHER:
			case MIB_IF_TYPE_ETHERNET:
			case MIB_IF_TYPE_TOKENRING:
			case MIB_IF_TYPE_FDDI:
			case MIB_IF_TYPE_PPP:
			case MIB_IF_TYPE_LOOPBACK:
			case MIB_IF_TYPE_SLIP:
				{
					strAddress = pIpAddrString->IpAddress.String;
               // 需要注意的是有时可能获取的IP地址是0.0.0.0，这时需要过滤掉
					if(std::string("0.0.0.0")==strAddress)
						break;
           std::cout<<_T("第")<< nCardNo<<_T("张网卡的IP地址是")<< strAddress<<std::endl;
                     nCardNo++;
					 break;
				}
			default:
                // 未知类型网卡就跳出
				break;
			}
			pIpAdapterInfo = pIpAdapterInfo->Next;
		}
	}
	//释放内存空间
	if (pIpAdapterInfo)
	{
		delete pIpAdapterInfo;
	}
}
```


