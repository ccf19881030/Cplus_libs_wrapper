#include <stdio.h>
#ifndef _MSC_VER
#include <unistd.h>
#endif
#include <stdlib.h>
#include <errno.h>

#include "modbus.h"

#pragma comment(lib,"modbus.lib")

int main(void)
{
	int server_socket = -1;
	modbus_t *ctx;
	modbus_mapping_t *mb_mapping;

	/*TCP*/
	ctx = modbus_new_tcp(NULL, 1502);    //创建一个TCP类型的容器
	modbus_set_debug(ctx, TRUE);                       //设置debug模式

	//申请4块内存区用以存放寄存器数据,这里各申请500各寄存器地址
	mb_mapping = modbus_mapping_new(500, 500, 500, 500);
	if (mb_mapping == NULL)
	{
		fprintf(stderr, "Error mapping: %s\n", modbus_strerror(errno));
		modbus_free(ctx);
		return -1;
	}

	// 开启监听端口
	server_socket = modbus_tcp_listen(ctx, 1);
	if (server_socket == -1)
	{
		fprintf(stderr, "Unable to listen TCP\n");
		modbus_free(ctx);

		return -1;
	}

	// 开始接收数据
	modbus_tcp_accept(ctx, &server_socket);

	//循环接收查询帧并回复消息
	for (;;)
	{
		uint8_t query[MODBUS_TCP_MAX_ADU_LENGTH];
		int rc;

		rc = modbus_receive(ctx, query);               //获取查询报文
		if (rc >= 0)
		{
			/*rc is the query size*/
			modbus_reply(ctx, query, rc, mb_mapping);  //回复响应报文
		}
		else
		{
			//Connection closed by the client or error
			printf("Connection Closed\n");
			// Close ctx
			modbus_close(ctx);
			// 等待下一个客户端报文
			modbus_tcp_accept(ctx, &server_socket);
		}
	}

	printf("Quit the loop: %s\n", modbus_strerror(errno));

	//释放内存
	modbus_mapping_free(mb_mapping);
	modbus_close(ctx);
	modbus_free(ctx);

	system("pause");

	return 0;
}