#include <stdio.h>
#ifndef _MSC_VER
#include <unistd.h>
#endif
#include <stdlib.h>
#include <errno.h>

#include "modbus.h"

#define SERVER_ID 17

int main(void)
{
	modbus_t *ctx;
	modbus_mapping_t *mb_mapping;
	
	/*RTU*/
	ctx = modbus_new_rtu("COM4", 19200, 'N', 8, 1);    //创建一个RTU类型的容器
	modbus_set_slave(ctx, SERVER_ID);                  //设置从端地址
	
	if(modbus_connect(ctx) == -1)                      //RTU模式下表示打开连接到串口
	{
		fprintf(stderr, "Connection failed: %s\n",modbus_strerror(errno));
		modbus_free(ctx);
		return -1;
	}
	
	modbus_set_debug(ctx, TRUE);                       //设置debug模式
	
	//申请4块内存区用以存放寄存器数据,这里各申请500各寄存器地址
	mb_mapping = modbus_mapping_new(500, 500, 500, 500);
	if(mb_mapping == NULL)
	{
		fprintf(stderr, "Error mapping: %s\n", modbus_strerror(errno));
		modbus_free(ctx);
		return -1;
	}
	
	//循环接收查询帧并回复消息
	for(;;)
	{
		uint8_t query[MODBUS_TCP_MAX_ADU_LENGTH];
		int rc;
		
		rc = modbus_receive(ctx, query);               //获取查询报文
		if(rc >= 0)
		{
			/*rc is the query size*/
			modbus_reply(ctx, query, rc, mb_mapping);  //回复响应报文
		}
		else
		{
			//Connection closed by the client or error
			printf("Connection Closed\n");
		}
	}
	
	printf("Quit the loop: %s\n",modbus_strerror(errno));
	
	//释放内存
	modbus_mapping_free(mb_mapping);
	modbus_close(ctx);
	modbus_free(ctx);
	
	return 0;
}