## [Windows 和 Linux下使用socket下载网页页面内容（可设置接收/发送超时）的代码](https://www.cnblogs.com/lidabo/p/3804238.html)
```cpp
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <string.h>
 
#ifdef _WIN32 	///包含win socket相关头文件
#include <winsock.h>
#pragma comment(lib,"ws2_32.lib")
#else  		///包含linux socket相关头文件
#include <unistd.h>
#include <strings.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>
#include <stdint.h>
#endif
 
#ifdef _WIN32
#ifdef __cplusplus
extern "C"{
#endif
 
int strcasecmp(const char *s1, const char *s2)
{
	while ((*s1 != '\0')
		&& (tolower(*(unsigned char *) s1) ==
		tolower(*(unsigned char *) s2))) 
	{
		s1++;
		s2++;
	}
	return tolower(*(unsigned char *) s1) - tolower(*(unsigned char *) s2);
}
int strncasecmp(const char *s1, const char *s2, unsigned int n)
{
	if (n == 0)
		return 0;
	while ((n-- != 0)
		&& (tolower(*(unsigned char *) s1) ==
		tolower(*(unsigned char *) s2))) {
			if (n == 0 || *s1 == '\0' || *s2 == '\0')
				return 0;
			s1++;
			s2++;
	}
	return tolower(*(unsigned char *) s1) - tolower(*(unsigned char *) s2);
}
#ifdef __cplusplus
}
#endif
 
#endif
/**********************************
*功能:Base64编码
*参数:
	src_data:待编码的字符串
	coded_data:编码后的字符串
*返回值:-1,失败;0,成功
***********************************/
int base64encode(const char * src_data/*in,待编码的字符串*/,char * coded_data/*out,编码后的字符串*/)
{
	const char EncodeTable[]="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	int src_data_len = strlen(src_data);
	int i;
	int lineLength=0;
	int mod=src_data_len % 3;
	unsigned char tmp[4]={0};
	char buff[5]={0};
	for(i=0;i<(int)(src_data_len / 3);i++)
	{
		tmp[1] = *src_data++;
		tmp[2] = *src_data++;
		tmp[3] = *src_data++;
		sprintf(buff,"%c%c%c%c", EncodeTable[tmp[1] >> 2],EncodeTable[((tmp[1] << 4) | (tmp[2] >> 4)) & 0x3F],EncodeTable[((tmp[2] << 2) | (tmp[3] >> 6)) & 0x3F],EncodeTable[tmp[3] & 0x3F]);
		strcat(coded_data,buff);
		if(lineLength+=4,lineLength==76) 
		{
			strcat(coded_data,"\r\n");
			lineLength=0;
		}
	}	
	if(mod==1)
	{
		tmp[1] = *src_data++;
		sprintf(buff,"%c%c==",EncodeTable[(tmp[1] & 0xFC) >> 2],EncodeTable[((tmp[1] & 0x03) << 4)]);
		strcat(coded_data,buff);
	}
	else if(mod==2)
	{
		tmp[1] = *src_data++;
		tmp[2] = *src_data++;
		sprintf(buff,"%c%c%c=",EncodeTable[(tmp[1] & 0xFC) >> 2],EncodeTable[((tmp[1] & 0x03) << 4) | ((tmp[2] & 0xF0) >> 4)],EncodeTable[((tmp[2] & 0x0F) << 2)]);
		strcat(coded_data,buff);
	}
	return 0;
}
 
 
//格式化http头,返回值:-1失败,-2用户名或密码无效;>=0 成功
int format_http_header(const char * webserverip,
			unsigned short httpport/*web server 端口*/,
			const char * url/*页面相对url,下载的页面为:http://ip/url"*/,
			const char * username/*网站认证用户*/,
			const char * password/*认证密码*/,
			const char * ext_param/*访问网页附加参数*/,
			int net_timeout/*超时时间,秒*/,
			char header[512]/*out*/)
{
	int len =0;	
	char buf_auth[100]={0},auth[100]={0};
	sprintf(buf_auth,"%s:%s",username,password);
	base64encode(buf_auth,auth);
	if(ext_param) 
	{
		len = strlen(ext_param);
	}
	if(len)
	{
		//GET
		return sprintf(header,
				"GET /%s?%s HTTP/1.1\r\n"
				"Host:%s:%u\r\n"
				"Content-Type: application/x-www-form-urlencoded\r\n"
				"Keep-Alive: Keep-Alive: timeout=%d\r\n"
				"Connection: keep-alive\r\n"
				"Accept:text/html\r\n"
				"Authorization: Basic %s\r\n"
				"\r\n"
				,url,ext_param,webserverip,httpport,net_timeout,auth
				);
	}
	//GET
	return sprintf(header,
			"GET /%s HTTP/1.1\r\n"
			"Host:%s:%u\r\n"
			"Content-Type: application/x-www-form-urlencoded\r\n"
			"Keep-Alive: timeout=%d\r\n"
			"Connection: keep-alive\r\n"
			"Accept:text/html\r\n"
			"Authorization: Basic %s\r\n"
			"\r\n"
			,url,webserverip,httpport,net_timeout,auth
			);
	/*POST /login.php HTTP/1.1 必有字段
	Host: www.webserver.com:80 必有字段
	User-Agent: Mozilla/5.0 (Windows; U; Windows NT 5.1; zh-CN; rv:1.9) Gecko/2008052906 Firefox/3.0
	Accept: text/html,application/xhtml+xml,application/xml;q=0.9,**; q=.2\r\n");  必有字段
	Accept-Language: zh-cn,zh;q=0.5
	Accept-Encoding: gzip,deflate
	Accept-Charset: gb2312,utf-8;q=0.7,*;q=0.7
	Keep-Alive: 300
	Connection: keep-alive
	Referer: http://www.vckbase.com/
	Cookie: ASPSESSIONIDCSAATTCD=DOMMILABJOPANJPNNAKAMCPK
	Content-Type: application/x-www-form-urlencoded 必有字段
	Content-Length: 79 post方式时必有字段*/
	
 
	/*GET方式HTTP头写法*/
	/*sprintf(header,
			"GET /ipnc/php/ipnc.php?%s HTTP/1.1\r\n"
			"Host:%s\r\n"
			"Content-Type:application/x-www-form-urlencoded\r\n"
			"Accept:text/html\r\n"
			"\r\n"
			,parm,serverip
			);*/
}
int parse_response_http_header(const char * all_contents/*接收到的所有内容,包含http头*/,char ** contents/*返回自己需要的内容*/)
{
	/**
	*根据需求分析网页的内容
	**/
	return 0;
}
//分析返回的内容的长度
int parse_respose_contents_length(const char * header/*in,http头*/)
{
	char * p = (char *)header;
	int tmp = 0;
#if 1
	if(p)
	{
		//获取内容长度
		while(*p)
		{
			if(*p == '\r')
			{
				if(strncasecmp(p,"\r\n\r\n",4) != 0)//http头没有结束
				{
					p+=2;//过滤\n
					if(strncasecmp(p,"Content-Length",14) == 0)
					{
						while(*p)
						{
							if(*p == ':')
							{
								p++;
								tmp = atoi(p);
								break;
							}
							p++;
						}
						break;
					}
				}
				else
				{
					break;
				}
			}
			p++;
		}
		if(!tmp)//没有Content-Length字段
		{
			
			for(p = (char*)header;*p;p++)
			{
				if(*p == '\r')
				{
					if(strncmp(p,"\r\n\r\n",4) == 0)
					{
						p+=4;
						tmp = strlen(p);
						break;
					}
				}
			}
		}
	}
#endif
	return tmp;
}
 
#define HTTP_RECV_BUFFER_SIZE 1024*1024*3 //3MB的接收缓存
#define RECV_BUFF_SIZE	1024
 
int download_web_page(const char * ipv4/*web server ip地址*/,
			unsigned short httpport/*web server 端口*/,
			const char * url/*页面相对url,下载的页面为:http://ip/url"*/,
			const char * username/*网站认证用户*/,
			const char * password/*认证密码*/,
			const char * ext_param/*访问网页附加参数*/,
			int net_timeout/*网络超时时间,秒*/,
			char ** contents/*out:返回的实际内容,无http头,需要使用free函数手动释放空间*/
			)
{
#ifdef _WIN32
	WSADATA wsaData;          //指向WinSocket信息结构的指针
#endif
	struct sockaddr_in server_addr;
	int sockfd = -1;
	char szHttpHeader[1024]={0};
	char * pszBuffer	=	NULL;///堆栈溢出，所以使用堆空间
	char szRecvBuffer[RECV_BUFF_SIZE+1]={0};
	int len = 0,total_recv_len=0,total_contents_len = 0,re=-1;
	unsigned long flags;
	fd_set fs;
	char * pHttpHeaderEnd = NULL;
#ifdef _WIN32
	/*
	*这里请注意
	*windows下设置接收/发送超时时间时，setsockopt函数对应的超时时间为int型（且超时时间的值的单位为毫秒，当时我直接填写为秒，老是接收超时）
	*linux下为struct timeval结构
	*/
	int timeout = net_timeout*1000;
	struct timeval select_timeout={0};
	select_timeout.tv_sec=net_timeout;
#else
	struct timeval timeout={.tv_sec=net_timeout,.tv_usec=0};
#endif
 
#ifdef _WIN32
    if(WSAStartup(MAKEWORD( 1, 1 ), &wsaData )!=0)//进行WinSocket的初始化
    {
		WSACleanup();
        return -1;//Can't initiates windows socket!初始化失败
    }
#endif
 
	if((sockfd = socket(AF_INET,SOCK_STREAM,0)) <= 0)
	{
#if defined CONSOLE || defined LINUX
		printf("创建socket失败.错误代码:%d,错误原因:%s\n",errno,strerror(errno));
#endif
		return -1;//create socket fd failed
	}
	///设置接收超时时间
	if(setsockopt(sockfd,SOL_SOCKET,SO_RCVTIMEO,(char *)&timeout,sizeof(timeout)) != 0)
	{
#if defined CONSOLE || defined LINUX
		printf("设置socket发送超时时间失败.错误代码:%d,错误原因:%s\n",errno,strerror(errno));
#endif
#ifdef _WIN32
		closesocket(sockfd);
#else
		close(sockfd);
#endif
		return -1;
	}
	///设置发送超时时间
	if(setsockopt(sockfd,SOL_SOCKET,SO_SNDTIMEO,(char *)&timeout,sizeof(timeout)) != 0)
	{
#if defined CONSOLE || defined LINUX
		printf("设置socket接收超时时间失败.错误代码:%d,错误原因:%s\n",errno,strerror(errno));
#endif
#ifdef _WIN32
		closesocket(sockfd);
#else
		close(sockfd);
#endif
		return -1;
	}
	///设置非阻塞方式,使用select来判断connect是否超时
#ifdef _WIN32
	flags=1;
	if( ioctlsocket(sockfd,FIONBIO,&flags) != 0)
#else
	flags=fcntl(sockfd,F_GETFL,0);
	flags |= O_NONBLOCK;
	if( fcntl(sockfd,F_SETFL,flags) != 0)
#endif
	{
#if defined CONSOLE || defined LINUX
		printf("设置socket为非阻塞失败.错误代码:%d,错误原因:%s\n",errno,strerror(errno));
#endif
#ifdef _WIN32
		closesocket(sockfd);
#else
		close(sockfd);
#endif
		return -1;
	}
	///设置连接参数
#ifdef _WIN32
	memset(&server_addr,0,sizeof(struct sockaddr_in));
#else
	bzero(&server_addr,sizeof(struct sockaddr_in));
#endif
	server_addr.sin_family 		= AF_INET;
	server_addr.sin_port 		= htons(httpport);
	server_addr.sin_addr.s_addr = inet_addr(ipv4);
	///连接服务器
	if( connect(sockfd,(struct sockaddr *)&server_addr,sizeof(struct sockaddr_in)) < 0)
	{
		int ret = 0;
		///判断是否超时
		FD_ZERO(&fs);
		FD_SET(sockfd,&fs);
#ifdef _WIN32
		ret = select(sockfd+1,NULL,&fs,NULL,&select_timeout);
#else
		ret = select(sockfd+1,NULL,&fs,NULL,&timeout);
#endif
		if(ret == 0)//超时
		{
#if defined CONSOLE || defined LINUX
			printf("链接服务器超时.错误代码:%d,错误原因:%s\n",errno,strerror(errno));
#endif
#ifdef _WIN32
			closesocket(sockfd);
#else
			close(sockfd);
#endif
			return -1;//连接超时
		}
		else if(ret < 0)///错误
		{
#if defined CONSOLE || defined LINUX
			printf("链接服务器时发生错误.错误代码:%d,错误原因:%s\n",errno,strerror(errno));
#endif
#ifdef _WIN32
			closesocket(sockfd);
#else
			close(sockfd);
#endif
			return -1;
		}
	}
	///设置为阻塞方式发送和接收数据
#ifdef _WIN32
	flags=0;
	if( ioctlsocket(sockfd,FIONBIO,&flags) != 0)
#else
	flags=fcntl(sockfd,F_GETFL,0);
	flags &= ~O_NONBLOCK;
	if( fcntl(sockfd,F_SETFL,flags) != 0)
#endif
	{
#if defined CONSOLE || defined LINUX
		printf("设置socket为阻塞失败.错误代码:%d,错误原因:%s\n",errno,strerror(errno));
#endif
#ifdef _WIN32
		closesocket(sockfd);
#else
		close(sockfd);
#endif
		return -1;//ioctlsocket() error
	}
	format_http_header(ipv4,httpport,url,username,password,ext_param,net_timeout,szHttpHeader);
 
	len = strlen(szHttpHeader);
	///发送http头
	if(send(sockfd,szHttpHeader,len,0) != len)
	{
#if defined CONSOLE || defined LINUX
		printf("发送http头失败.错误代码:%d,错误原因:%s\nhttp头:\n%s\n",errno,strerror(errno),szHttpHeader);
#endif
#ifdef _WIN32
		closesocket(sockfd);
#else
		close(sockfd);
#endif
		return -1;//发送数据失败
	}
	///准备接收数据
	pszBuffer = (char *)malloc(HTTP_RECV_BUFFER_SIZE);
	if(!pszBuffer)
	{
#if defined CONSOLE || defined LINUX
		printf("内存分配失败\n");
#endif
#ifdef _WIN32
		closesocket(sockfd);
#else
		close(sockfd);
#endif
		return -1;//outof memory
	}
 
#ifdef _WIN32
	memset(pszBuffer,0,HTTP_RECV_BUFFER_SIZE);
#else
	bzero(pszBuffer,HTTP_RECV_BUFFER_SIZE);
#endif
 
	while(1)
	{
#ifdef _WIN32
		len = recv(sockfd,szRecvBuffer,RECV_BUFF_SIZE,0);
#else
		len = recv(sockfd,szRecvBuffer,RECV_BUFF_SIZE,MSG_WAITALL);
#endif
		if(len == 0)
		{
#if defined CONSOLE || defined LINUX
			printf("接收数据超时,超时时间:%d s\n",net_timeout);
#endif
#ifdef _WIN32
			closesocket(sockfd);
#else
			close(sockfd);
#endif
			free(pszBuffer);
			return -1;//接收数据超时
		}
		if(len < 0 )
		{
#if defined CONSOLE || defined LINUX
			printf("接收数据错误,recv返回值:%d \n",len);
#endif
#ifdef _WIN32
			closesocket(sockfd);
#else
			close(sockfd);
#endif
			free(pszBuffer);
			return -1;//timeout
		}
		//printf("%s",szBuffer);
		total_recv_len += len;
		if(total_recv_len > (HTTP_RECV_BUFFER_SIZE-1) )
		{
#if defined CONSOLE || defined LINUX
			printf("接收数据buffer空间不足,当前buffer大小:%d B\n",HTTP_RECV_BUFFER_SIZE-1);
#endif
#ifdef _WIN32
			closesocket(sockfd);
#else
			close(sockfd);
#endif
			free(pszBuffer);
			return -1;//not enough buffer size
		}
		strcat(pszBuffer,szRecvBuffer);
		if(len < RECV_BUFF_SIZE)
		{
			pHttpHeaderEnd = strstr(pszBuffer,"\r\n\r\n");
			if(pHttpHeaderEnd )
			{
				if(!total_contents_len)///http返回头中标示的内容大小
				{
					total_contents_len = parse_respose_contents_length(pszBuffer);
				}
				pHttpHeaderEnd += 4;
				//如果接收到的内容长度已经达到http返回头中标示的内容大小,停止接收
				if( total_contents_len && strlen( pHttpHeaderEnd) >= total_contents_len )
					break;
				pHttpHeaderEnd = NULL;
			}			
		}
#ifdef _WIN32
		memset(szRecvBuffer,0,sizeof(szRecvBuffer));
#else
		bzero(szRecvBuffer,sizeof(szRecvBuffer));
#endif
		len = 0;
	}
	if(strcmp(pszBuffer,"") == 0)
	{
#ifdef _WIN32
		closesocket(sockfd);
#else
		close(sockfd);
#endif
		free(pszBuffer);
		return -1;//recv data error
	}
	//printf("%s\n",szBuffer);
	* contents = NULL;
	re = parse_response_http_header(pszBuffer,contents);
	if( re != 0 || !(*contents))
	{
		if(*contents)
		{
			free(*contents);
		}
#if defined CONSOLE || defined LINUX
		printf("分析服务器返回内容失败.返回内容为:\n%s\n",pszBuffer);
#endif
#ifdef _WIN32
		closesocket(sockfd);
#else
		close(sockfd);
#endif
		free(pszBuffer);
		if( -401 == re)
			return -1;//用户名/密码无效
		return -1;//unknown error
	}
#ifdef _WIN32
	closesocket(sockfd);
#else
	close(sockfd);
#endif
	free(pszBuffer);
 
 
#ifdef _WIN32
	WSACleanup();
#endif
	return 0;
}
```
