#include <stdio.h>
#ifndef _MSC_VER
#include <unistd.h>
#endif

#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include "modbus.h"			// 引入libmodbus库

#pragma comment(lib, "modbus.lib")

/******************************************************************************
The goal of this program is to check all major functions of libmodbus:
-wrtie_coil
-read_bits
-write_coils
-write_register
-read_register
-write_registers
-read_registers

All these functions are called with random values on a address range 
defined by the following defines.
*******************************************************************************/

#define LOOP			1				// 循环次数
#define SERVER_ID		17				// 从设备地址
#define ADDRESS_START	0				// 测试寄存器起始地址
#define ADDRESS_END		99				// 测试寄存器结束地址

/************************************************************************************
At each loop,the program works in the range ADDRESS_START to ADDRESS_END then 
ADDRESS_START+1 to ADDRESS_END and so on.
************************************************************************************/
int main(int argc, char *argv[])
{
	modbus_t *ctx;
	int rc;
	int nb_fail;
	int nb_loop;
	int addr;
	int nb;
	uint8_t *tab_rq_bits;					// 用于保存发送或接收数据
	uint8_t *tab_rp_bits;
	uint16_t *tab_rq_registers;
	uint16_t *tab_rw_rq_registers;

	uint16_t *tab_rp_registers;

	// TCP
	ctx = modbus_new_tcp("127.0.0.1", 1502);		// 创建一个TCP类型的容器
	modbus_set_debug(ctx, TRUE);					// 设置Debug模式

	if (modbus_connect(ctx) == -1)					// TCP 模式下链接Server
	{
		fprintf(stderr, "Connection failed:%s\n", modbus_strerror(errno));
		modbus_free(ctx);
		return -1;
	}

	// Allocate and initialize the different memory spaces
	nb = ADDRESS_END - ADDRESS_START;		// 计算需测试的寄存器个数

	// 以下申请内存块，用以保存发送和接收数据
	tab_rq_bits = (uint8_t*)malloc(nb * sizeof(uint8_t));
	memset(tab_rq_bits, 0, nb * sizeof(uint8_t));

	tab_rp_bits = (uint8_t*)malloc(nb * sizeof(uint8_t));
	memset(tab_rq_bits, 0, nb * sizeof(uint8_t));

	tab_rq_registers = (uint16_t*)malloc(nb * sizeof(uint16_t));
	memset(tab_rq_registers, 0, nb * sizeof(uint16_t));

	tab_rp_registers = (uint16_t*)malloc(nb * sizeof(uint16_t));
	memset(tab_rp_registers, 0, nb * sizeof(uint16_t));

	tab_rw_rq_registers = (uint16_t*)malloc(nb * sizeof(uint16_t));
	memset(tab_rw_rq_registers, 0, nb * sizeof(uint16_t));

	nb_loop = nb_fail = 0;
	while (nb_loop++ < LOOP)
	{
		// 从起始地址开始顺序测试
		for (addr = ADDRESS_START; addr < ADDRESS_END; ++addr)
		{
			int i;

			// 生成随机数用于测试
			for (i=0; i < nb; ++i)
			{
				tab_rq_registers[i] = (uint16_t)(65535.0 * rand() / (RAND_MAX + 1.0));
				tab_rw_rq_registers[i] = ~tab_rq_registers[i];
				tab_rq_bits[i] = tab_rq_registers[i] % 2;
			}
			nb = ADDRESS_END - addr;
			
			// 测试线圈寄存器的单个读写
			rc = modbus_write_bit(ctx, addr, tab_rq_bits[0]);			// 写线圈地址addr
			if (rc != 1)
			{
				printf("ERR modbus_write_bit(%d)-%s\n", rc, modbus_strerror(errno));
				printf("Address = %d, Value=%d\n", addr, tab_rq_bits[0]);
				nb_fail++;
			}
			else {
				// 写入之后，再读取并比较
				rc = modbus_read_bits(ctx, addr, 1, tab_rp_bits);
				if (rc != 1 || tab_rq_bits[0] != tab_rp_bits[0])
				{
					printf("ERROR modbus_read_bits single (%d)\n", rc);
					printf("address = %d\n", addr);
					nb_fail++;
				}
			}

			// 测试线圈寄存器的批量读写
			/*测试线圈寄存器的批量读写*/
			rc = modbus_write_bits(ctx, addr, nb, tab_rq_bits);
			if (rc != nb)
			{
				printf("ERROR modbus_write_bits (%d)\n", rc);
				printf("Address =%d, nb =%d\n", addr, nb);
				nb_fail++;
			}
			else
			{
				//写入之后,再读取比较
				rc = modbus_read_bits(ctx, addr, nb, tab_rp_bits);
				if (rc != nb)
				{
					printf("ERROR modbus_read_bits\n");
					printf("Address =%d, nb =%d\n", addr, nb);
					nb_fail++;
				}
				else
				{
					//进行比较
					for (i = 0; i < nb; i++)
					{
						if (tab_rp_bits[i] != tab_rq_bits[i])
						{
							printf("ERROR modbus_read_bits\n");
							printf("Addr=%d, Val=%d (0x%X) != %d (0x%X)\n",
								addr, tab_rq_bits[i], tab_rq_bits[i],
								tab_rp_bits[i], tab_rp_bits[i]);
							nb_fail++;
						}
					}
				}
			}

			/*测试保持寄存器的单个读写*/
			rc = modbus_write_register(ctx, addr, tab_rq_registers[0]);
			if (rc != 1)
			{
				printf("ERROR modbus_write_register (%d)\n", rc);
				printf("Address=%d, Val=%d(0x%X)\n", addr,
					tab_rq_registers[0], tab_rq_registers[0]);
				nb_fail++;
			}
			else
			{
				//写入之后进行读取
				rc = modbus_read_registers(ctx, addr, 1, tab_rp_registers);
				if (rc != 1)
				{
					printf("ERROR modbus_read_registers (%d)\n", rc);
					printf("Address=%d\n", addr);
					nb_fail++;
				}
				else
				{
					//读取后进行比较
					if (tab_rq_registers[0] != tab_rp_registers[0])
					{
						printf("ERROR modbus_read_registers\n");
						printf("Address=%d, Val=%d (0x%X) !=%d (0x%X)\n",
							addr, tab_rq_registers[0], tab_rq_registers[0],
							tab_rp_registers[0], tab_rp_registers[0]);
						nb_fail++;
					}
				}
			}

			/*测试保持寄存器的批量读写*/
			rc = modbus_write_registers(ctx, addr, nb, tab_rq_registers);
			if (rc != nb)
			{
				printf("ERROR modbus_write_registers (%d)\n", rc);
				printf("Address=%d, nb=%d\n", addr, nb);
				nb_fail++;
			}
			else
			{
				//进行读取测试
				rc = modbus_read_registers(ctx, addr, nb, tab_rp_registers);
				if (rc != nb)
				{
					printf("ERROR modbus_read_register (%d)\n", rc);
					printf("Address=%d, nb=%d\n", addr, nb);
					nb_fail++;
				}
				else
				{
					for (i = 0; i < nb; i++)
					{
						if (tab_rp_registers[i] != tab_rq_registers[i])
						{
							printf("ERROR modbus_read_registers\n");
							printf("Address=%d, Value %d (0x%X) != %d (0x%X)\n",
								addr, tab_rq_registers[i], tab_rq_registers[i],
								tab_rp_registers[i], tab_rp_registers[i]);
							nb_fail++;
						}
					}
				}
			}
			/*功能码23(0x17)读写多个寄存器的测试*/
			rc = modbus_write_and_read_registers(ctx, addr, nb,
				tab_rw_rq_registers, addr, nb, tab_rp_registers);
			if (rc != nb)
			{
				printf("ERROR modbus_read_and_write_registers (%d)\n", rc);
				printf("Address=%d, nb=%d\n", addr, nb);
				nb_fail++;
			}
			else
			{
				//读取并比较
				for (i = 0; i < nb; i++)
				{
					if (tab_rp_registers[i] != tab_rw_rq_registers[i])
					{
						printf("ERROR modbus_read_and_write_registers READ\n");
						printf("Address=%d, Value %d (0x%X) != %d (0x%X)\n",
							addr, tab_rp_registers[i], tab_rw_rq_registers[i],
							tab_rp_registers[i], tab_rw_rq_registers[i]);
						nb_fail++;
					}
				}

				// 写入之后，再读取并比较
				rc = modbus_read_registers(ctx, addr, nb, tab_rp_registers);
				if (rc != nb)
				{
					printf("ERROR modbus_read_register (%d)\n", rc);
					printf("Address =%d, nb =%d\n", addr, nb);
					nb_fail++;
				}
				else
				{
					for (i = 0; i < nb; i++)
					{
						if (tab_rw_rq_registers[i] != tab_rp_registers[i])
						{
							printf("ERROR modbus_read_and_write_registers\n");
							printf("Address=%d, Val %d (0x%X) != %d (0x%X)\n",
								addr, tab_rw_rq_registers[i], tab_rw_rq_registers[i],
								tab_rp_registers[i], tab_rp_registers[i]);
							nb_fail++;
						}
					}
				}
			}
		}
		printf("Test:");
		if (nb_fail)
			printf("%d FALLS\n", nb_fail);
		else
			printf("SUCCESS\n");
	}

	/*Free the memory*/
	free(tab_rq_bits);
	free(tab_rp_bits);
	free(tab_rq_registers);
	free(tab_rp_registers);
	free(tab_rw_rq_registers);

	/*Close the connection*/
	modbus_close(ctx);
	modbus_free(ctx);

	return 0;
}

