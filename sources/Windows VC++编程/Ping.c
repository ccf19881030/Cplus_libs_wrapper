/******************************************************************************\
* ping.c - Simple ping utility using SOCK_RAW
*
* This is a part of the Microsoft Source Code Samples.
* Copyright 1996-1997 Microsoft Corporation.
* All rights reserved.
* This source code is only intended as a supplement to
* Microsoft Development Tools and/or WinHelp documentation.
* See these sources for detailed information regarding the
* Microsoft samples programs.
\******************************************************************************/

#pragma pack(4) 

#define WIN32_LEAN_AND_MEAN 
#include <winsock2.h> 
#include <stdio.h> 
#include <stdlib.h> 
#pragma comment(lib,"ws2_32.lib")

#define ICMP_ECHO 8											 // ICMP 类型码，回送请求的类型码为8
#define ICMP_ECHOREPLY 0 

#define ICMP_MIN 8                                           // minimum 8 byte icmp packet (just header) 

/* The IP header */
typedef struct iphdr
{
	unsigned int h_len : 4; // length of the header 
	unsigned int version : 4; // Version of IP 
	unsigned char tos; // Type of service 
	unsigned short total_len; // total length of the packet 
	unsigned short ident; // unique identifier 
	unsigned short frag_and_flags; // flags 
	unsigned char ttl;
	unsigned char proto; // protocol (TCP, UDP etc) 
	unsigned short checksum; // IP checksum 
	unsigned int sourceIP;
	unsigned int destIP;
}IpHeader;

// 
// ICMP header 
// ICMP的数据报是封装在IP报头里的。而它本身也是由报头加上数据部分构成的，
// 其报头的格式可以用一个结构体表示。
typedef struct _ihdr {
	BYTE i_type;     // ICMP 类型码，回送请求的类型码为8
	BYTE i_code;     // 子类型码，保存与特定ICMP报文类型相关的细节信息
	USHORT i_cksum;  // 校验和
	USHORT i_id;     // ICMP数据报的ID号
	USHORT i_seq;    // ICMP数据报的序列号
	ULONG timestamp; //时间戳  /* This is not the std header, but we reserve space for time */ 
}IcmpHeader;         //ICMP报文  包括报头和数据

#define STATUS_FAILED 0xFFFF 
#define DEF_PACKET_SIZE 32 
#define MAX_PACKET 1024 

#define xmalloc(s) HeapAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,(s)) 
#define xfree(p) HeapFree (GetProcessHeap(),0,(p)) 

void fill_icmp_data(char *, int);
USHORT checksum(USHORT *, int);
void decode_resp(char *, int, struct sockaddr_in *);

int main(int argc, char **argv)
{
	WSADATA wsaData;
	SOCKET sockRaw;
	struct sockaddr_in dest;
	struct hostent * hp;
	int bread, datasize;
	int timeout = 1000;		// 设置延时为1000ms
	char *dest_ip;
	char *icmp_data;
	char *recvbuf;
	unsigned int addr = 0;
	USHORT seq_no = 0;
	struct sockaddr_in from;	// 数据报的源IP地址
	int fromlen = sizeof(from);

	if (WSAStartup(MAKEWORD(2, 1), &wsaData) != 0)
	{
		fprintf(stderr, "WSAStartup failed: %d\n", GetLastError());
		ExitProcess(STATUS_FAILED);
	}

	/*
	为了使用发送接收超时设置(即设置SO_RCVTIMEO, SO_SNDTIMEO)，
//    必须将标志位设为WSA_FLAG_OVERLAPPED !
	*/
	sockRaw = WSASocket(AF_INET, SOCK_RAW, IPPROTO_ICMP, NULL, 0, WSA_FLAG_OVERLAPPED);                    //建立一个原始套接字
	//sockRaw = WSASocket (AF_INET, SOCK_RAW, IPPROTO_ICMP, NULL, 0,0);

	if (sockRaw == INVALID_SOCKET)
	{
		fprintf(stderr, "WSASocket() failed: %d\n", WSAGetLastError());
		ExitProcess(STATUS_FAILED);
	}

	timeout = 1000;   //设置接收超时时间为1000毫秒
	bread = setsockopt(sockRaw, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout)); //RECVTIMEO是接收超时时间
	if (bread == SOCKET_ERROR)
	{
		fprintf(stderr, "failed to set recv timeout: %d\n", WSAGetLastError());
		ExitProcess(STATUS_FAILED);
	}

	timeout = 1000;   //设置发送超时时间
	bread = setsockopt(sockRaw, SOL_SOCKET, SO_SNDTIMEO, (char*)&timeout, sizeof(timeout)); //SNDTIMEO是发送超时时间 
	if (bread == SOCKET_ERROR)
	{
		fprintf(stderr, "failed to set send timeout: %d\n", WSAGetLastError());
		ExitProcess(STATUS_FAILED);
	}
	memset(&dest, 0, sizeof(dest));            //目标地址清零

	hp = gethostbyname("www.baidu.com");      //通过域名或者主机名获取IP地址
	if (!hp)  //失败返回NULL
	{
		ExitProcess(STATUS_FAILED);
	}
	else
	{
		addr = inet_addr("14.215.177.37");    //www.baidu.com的ip地址
	}

	if ((!hp) && (addr == INADDR_NONE))        //既不是域名也不是点分十进制的IP地址 
	{
		ExitProcess(STATUS_FAILED);
	}

	if (hp != NULL)                           //获取的是域名
		memcpy(&(dest.sin_addr), hp->h_addr, hp->h_length);   //从hostent得到的对方ip地址
	else
		dest.sin_addr.s_addr = addr;

	if (hp)
		dest.sin_family = hp->h_addrtype;    
	else
		dest.sin_family = AF_INET;

	dest_ip = inet_ntoa(dest.sin_addr);        //目标IP地址

	// 生成ICMP回送请求报文
	datasize = DEF_PACKET_SIZE;             // ICMP包数据大小设定为32

	datasize += sizeof(IcmpHeader);         // 另外加上ICMP包的包头 其实包头占12个字节

	icmp_data = (char *)xmalloc(MAX_PACKET);// 发送icmp_data数据包内存
	recvbuf = (char *)xmalloc(MAX_PACKET);  // 存放接收到的数据

	if (!icmp_data)                         //分配内存
	{
		ExitProcess(STATUS_FAILED);
	}

	memset(icmp_data, 0, MAX_PACKET);
	fill_icmp_data(icmp_data, datasize);         // 只填充了ICMP包

	fprintf(stdout, "\nPinging %s ....\n\n", dest_ip);

	while (1)
	{
		int bwrote;

		((IcmpHeader*)icmp_data)->i_cksum = 0;		// 将校验和置0
		((IcmpHeader*)icmp_data)->timestamp = GetTickCount(); // 设置时间戳

		((IcmpHeader*)icmp_data)->i_seq = seq_no++;           //ICMP的序列号
		((IcmpHeader*)icmp_data)->i_cksum = checksum((USHORT*)icmp_data, datasize);   //icmp校验位  

		//下面这个函数的问题是 发送数据只是ICMP数据包，而接收到的数据时包含ip头的 也就是发送和接收不对等
		//问题是sockRaw 设定了协议为 IPPROTO_ICMP
		// 发送数据报文
		bwrote = sendto(sockRaw, icmp_data, datasize, 0, (struct sockaddr*)&dest, sizeof(dest));
		if (bwrote == SOCKET_ERROR)
		{
			if (WSAGetLastError() == WSAETIMEDOUT)     //发送时间超时
			{
				printf("timed out\n");
				continue;
			}

			fprintf(stderr, "sendto failed: %d\n", WSAGetLastError());
			ExitProcess(STATUS_FAILED);
		}

		// 没有把所有的数据发送出去，也出错了。
		if (bwrote < datasize)
		{
			fprintf(stdout, "Wrote %d bytes\n", bwrote);
		}

		// 接收响应报文
		bread = recvfrom(sockRaw, recvbuf, MAX_PACKET, 0, (struct sockaddr*)&from, &fromlen);
		if (bread == SOCKET_ERROR)
		{
			if (WSAGetLastError() == WSAETIMEDOUT)
			{
				printf("timed out\n");
				continue;
			}
			fprintf(stderr, "recvfrom failed: %d\n", WSAGetLastError());
			ExitProcess(STATUS_FAILED);
		}
		
		// 接收到原始的IP报文到recvbuf中，先剥去外面的IP包头得到ICMP的报文部分
		decode_resp(recvbuf, bread, &from);

		Sleep(1000);
	}

	WSACleanup();
	system("pause");

	return 0;
}

/*
The response is an IP packet. We must decode the IP header to locate
the ICMP data
*/
void decode_resp(char *buf, int bytes, struct sockaddr_in *from)
{
	IpHeader *iphdr;
	IcmpHeader *icmphdr;
	unsigned short iphdrlen;

	iphdr = (IpHeader *)buf;      //接收到的数据就是原始的IP数据报

	iphdrlen = iphdr->h_len * 4; // IP报头的长度(number of 32-bit words *4 = bytes)

	if (bytes < iphdrlen + ICMP_MIN)
	{
		printf("Too few bytes from %s\n", inet_ntoa(from->sin_addr));
	}

	icmphdr = (IcmpHeader*)(buf + iphdrlen);	// 跳过IP报头

	if (icmphdr->i_type == 3)
	{
		printf("network unreachable -- Response from %s.\n", inet_ntoa(from->sin_addr));
		return;
	}

	if (icmphdr->i_id != (USHORT)GetCurrentProcessId())	// ID不相符，丢弃
	{
		fprintf(stderr, "someone else's packet!\n");
		return;
	}
	printf("%d bytes from %s:", bytes, inet_ntoa(from->sin_addr));
	printf(" icmp_seq = %d ", icmphdr->i_seq);
	printf(" time: %d ms ", GetTickCount() - icmphdr->timestamp);
	printf(" ttl: %d", iphdr->ttl);
	printf("\n");
}

//完成ICMP的校验
USHORT checksum(USHORT *buffer, int size)
{
	unsigned long cksum = 0;

	while (size > 1)
	{
		cksum += *buffer++;
		size -= sizeof(USHORT);
	}

	if (size)
	{
		cksum += *(UCHAR*)buffer;
	}

	cksum = (cksum >> 16) + (cksum & 0xffff);
	cksum += (cksum >> 16);
	return (USHORT)(~cksum);
}

/*
Helper function to fill in various stuff in our ICMP request.
*/
void fill_icmp_data(char * icmp_data, int datasize) {

	IcmpHeader *icmp_hdr;
	char *datapart;

	icmp_hdr = (IcmpHeader*)icmp_data;

	icmp_hdr->i_type = ICMP_ECHO;                   // ICMP_ECHO要求收到包的主机回复此ICMP包
	icmp_hdr->i_code = 0;
	icmp_hdr->i_id = (USHORT)GetCurrentProcessId(); // id填当前进程的id
	icmp_hdr->i_cksum = 0;
	icmp_hdr->i_seq = 0;

	datapart = icmp_data + sizeof(IcmpHeader);
	// 
	// Place some junk in the buffer. 
	// 
	memset(datapart, 'E', datasize - sizeof(IcmpHeader));  //填充了一些废物
}
