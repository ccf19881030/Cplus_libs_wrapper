#include "ping.h"
#include <stdio.h>

void showPingResult(PingResult pingResult);
int main(int argc,char *argv[])
{
	if (argc != 2) {
		printf("usage: ping <host or ip>\n");
		return 0;
	}
	char * hostOrIp =  argv[1];
	int nsend = 0, nreceived = 0;
	bool ret;
	PingResult pingResult;
	Ping ping = Ping();
	for (int count = 1; count <= 4; count++) {
		ret = ping.ping(hostOrIp, 1, pingResult);
		if (count == 1) {
			printf("PING %s(%s): %d bytes data in ICMP packets.\n",hostOrIp, pingResult.ip.c_str(), pingResult.dataLen);
		}
		if (!ret) {
			printf("%s\n", pingResult.error.c_str());
			break;
		}
		showPingResult(pingResult);
		nsend += pingResult.nsend;
		nreceived += pingResult.nreceived;
	}
	if (ret) {
		printf("%d packets transmitted, %d received , %%%d lost\n", nsend, nreceived,
			(nsend - nreceived) / nsend * 100);
	}
	return 0;
}

void showPingResult(PingResult pingResult) {
	for (unsigned int icmpEchoReplyIndex = 0; icmpEchoReplyIndex < pingResult.icmpEchoReplys.size(); icmpEchoReplyIndex++) {
		IcmpEchoReply icmpEchoReply = pingResult.icmpEchoReplys.at(icmpEchoReplyIndex);
		if (icmpEchoReply.isReply) {
			printf("%d byte from %s: icmp_seq=%u ttl=%d rtt=%.3f ms\n",
					icmpEchoReply.icmpLen,
					icmpEchoReply.fromAddr.c_str(),
					icmpEchoReply.icmpSeq,
					icmpEchoReply.ipTtl,
					icmpEchoReply.rtt);
		} else {
			printf("request timeout\n");
		}
	}
}
