#include "stdafx.h"
#include<stdio.h>
#include<windows.h>
#include<process.h>
 
#pragma comment( lib, "ws2_32.lib" )
 
#define SEND_SIZE 32 
#define PACKET_SIZE 4096
#define ICMP_ECHO 8
#define ICMP_ECHOREPLY 0
 
struct icmp
{
	unsigned char icmp_type;
	unsigned char icmp_code;
	unsigned short icmp_cksum;
	unsigned short icmp_id;
	unsigned short icmp_seq;
	unsigned long icmp_data; 
};
 
struct ip
{
	unsigned char ip_hl:4;      
	unsigned char ip_v:4;       
	unsigned char ip_tos;          
	unsigned short ip_len;        
	unsigned short ip_id;         
	unsigned short ip_off;        
	unsigned char ip_ttl;          
	unsigned char ip_p;         
	unsigned short ip_sum;        
	unsigned long ip_src;
	unsigned long ip_dst; 
};
 
/*unsigned */char sendpacket[PACKET_SIZE];
/*unsigned */char recvpacket[PACKET_SIZE];
struct sockaddr_in dest_addr;
struct sockaddr_in from_addr;
int sockfd;
int pid;
 
unsigned short cal_chksum(unsigned short *addr,int len);
int pack(int pack_no);
int unpack(/*unsigned*/ char *buf,int len);
void send_packet(void);
void recv_packet(void);
 
void main(int argc,char *argv[])
{      
	struct hostent *host;
	struct protoent *protocol;
	WSADATA wsaData;
	int timeout=1000;
	int SEND_COUNT=4;
	int i;
	char* par_host = "www.baidu.com";
 
/*
	par_host=argv[argc-1];
	switch(argc)
	{
	case 2: break;
	case 3: if(strcmp(argv[1],"-t")==0)
			{
				SEND_COUNT=10000;
				break;
			}
			//fall through
	default:
		printf("usage: %s [-t] Host name or IP address\n",argv[0]);
		exit(1);       
	}*/
 
 
	if(WSAStartup(0x1010,&wsaData)!=0)
	{
		printf("wsastartup error\n");
		exit(1);
	}
	if( (protocol=getprotobyname("icmp") )==NULL)
	{
		printf("getprotobyname error\n");
		exit(1);
	}
	if( (sockfd=socket(AF_INET,SOCK_RAW,protocol->p_proto) )<0)
	{  
		printf("socket error\n");
		exit(1);
	}
	if(setsockopt(sockfd,SOL_SOCKET,SO_RCVTIMEO,(char*)&timeout,sizeof(timeout))<0) 
		fprintf(stderr,"failed to set recv timeout: %d\n",WSAGetLastError());
	if(setsockopt(sockfd,SOL_SOCKET,SO_SNDTIMEO,(char*)&timeout,sizeof(timeout))<0)
		fprintf(stderr,"failed to set send timeout: %d\n",WSAGetLastError());       
 
	memset(&dest_addr,0,sizeof(dest_addr));
	dest_addr.sin_family=AF_INET;
	if(host=gethostbyname(par_host) )
	{
		memcpy( (char *)&dest_addr.sin_addr,host->h_addr,host->h_length);
		//resolve address to hostname
		if(host=gethostbyaddr(host->h_addr,4,PF_INET))
			par_host=host->h_name;
	}
	else if( dest_addr.sin_addr.s_addr=inet_addr(par_host)==INADDR_NONE)
	{
		printf("Unkown host %s\n",par_host);
		exit(1);
	}
 
 
	pid = _getpid();
	printf("Pinging %s [%s]: with %d bytes of data:\n\n",par_host,inet_ntoa(dest_addr.sin_addr),SEND_SIZE);                   
	for(i=0;i<SEND_COUNT;i++)                       
	{
		send_packet();
		recv_packet();
		Sleep(1000);
	}
}
 
//this algorithm is referenced from other's
unsigned short cal_chksum(unsigned short *addr,int len)d
//打包
int pack(int pack_no)
{      
	int packsize;
	struct icmp *icmp;
 
	packsize=8+SEND_SIZE;
	icmp=(struct icmp*)sendpacket;
	icmp->icmp_type=ICMP_ECHO;
	icmp->icmp_code=0;
	icmp->icmp_cksum=0;
	icmp->icmp_seq=pack_no;
	icmp->icmp_id=pid;
	icmp->icmp_data=GetTickCount();
	icmp->icmp_cksum=cal_chksum( (unsigned short *)icmp,packsize); /*校验算法*/
	return packsize;
}
 
//解包
int unpack(/*unsigned*/ char *buf,int len)
{     
	struct ip *ip;
	struct icmp *icmp;
	double rtt;
	int iphdrlen;
 
	ip=(struct ip *)buf;
	iphdrlen=ip->ip_hl*4;   
	icmp=(struct icmp *)(buf+iphdrlen);          
	if( (icmp->icmp_type==ICMP_ECHOREPLY) && (icmp->icmp_id==pid) )
	{
		len=len-iphdrlen-8;  
		rtt=GetTickCount()-icmp->icmp_data; 
		printf("Reply from %s: bytes=%d time=%.0fms TTL=%d icmp_seq=%u\n",            
			inet_ntoa(from_addr.sin_addr),
			len,
			rtt,
			ip->ip_ttl,
			icmp->icmp_seq);
		return 1;
	}
	return 0;
}
 
//发送
void send_packet()
{  
	int packetsize;
	static int pack_no=0;
 
	packetsize=pack(pack_no++);
	if( sendto(sockfd,sendpacket,packetsize,0,(struct sockaddr *)&dest_addr,sizeof(dest_addr) )<0  )
		printf("Destination host unreachable.\n");
	// printf("send NO %d\n",pack_no-1);
}
 
//接收
void recv_packet()
{   
	int n,fromlen;
	int success;
 
	fromlen=sizeof(from_addr);
	do
	{
		if( (n=recvfrom(sockfd,recvpacket,sizeof(recvpacket),0,(struct sockaddr *)&from_addr,&fromlen)) >=0)
			success=unpack(recvpacket,n);
		else if (WSAGetLastError() == WSAETIMEDOUT)
		{
			printf("Request timed out.\n");
			return;
		}
	}while(!success);
 
}
