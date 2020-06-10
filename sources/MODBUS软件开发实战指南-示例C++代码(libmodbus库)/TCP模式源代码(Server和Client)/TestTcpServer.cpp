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
	ctx = modbus_new_tcp(NULL, 1502);    //����һ��TCP���͵�����
	modbus_set_debug(ctx, TRUE);                       //����debugģʽ

	//����4���ڴ������Դ�żĴ�������,���������500���Ĵ�����ַ
	mb_mapping = modbus_mapping_new(500, 500, 500, 500);
	if (mb_mapping == NULL)
	{
		fprintf(stderr, "Error mapping: %s\n", modbus_strerror(errno));
		modbus_free(ctx);
		return -1;
	}

	// ���������˿�
	server_socket = modbus_tcp_listen(ctx, 1);
	if (server_socket == -1)
	{
		fprintf(stderr, "Unable to listen TCP\n");
		modbus_free(ctx);

		return -1;
	}

	// ��ʼ��������
	modbus_tcp_accept(ctx, &server_socket);

	//ѭ�����ղ�ѯ֡���ظ���Ϣ
	for (;;)
	{
		uint8_t query[MODBUS_TCP_MAX_ADU_LENGTH];
		int rc;

		rc = modbus_receive(ctx, query);               //��ȡ��ѯ����
		if (rc >= 0)
		{
			/*rc is the query size*/
			modbus_reply(ctx, query, rc, mb_mapping);  //�ظ���Ӧ����
		}
		else
		{
			//Connection closed by the client or error
			printf("Connection Closed\n");
			// Close ctx
			modbus_close(ctx);
			// �ȴ���һ���ͻ��˱���
			modbus_tcp_accept(ctx, &server_socket);
		}
	}

	printf("Quit the loop: %s\n", modbus_strerror(errno));

	//�ͷ��ڴ�
	modbus_mapping_free(mb_mapping);
	modbus_close(ctx);
	modbus_free(ctx);

	system("pause");

	return 0;
}