// mosquitto_Demo.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>

#include "MQTTService.h"

int main(int argc, char *arg[])
{
	mosqpp::lib_init();

	MQTTService mqttSer("client6");

	int rc;
	char buf[1024] = "This is a mqtt test";
	mqttSer.username_pw_set("mqtt", "mqtt");
	rc = mqttSer.connect("127.0.0.1");	//本地IP
	char err[1024];
	if (rc == MOSQ_ERR_ERRNO)
	{
		cout << "连接错误:" << mosqpp::strerror(rc) << endl;		// 连接错误
	} else if (rc == MOSQ_ERR_SUCCESS)
	{
		rc = mqttSer.loop();
		if (MOSQ_ERR_SUCCESS == rc)
		{
			int mid = 14;
			rc = mqttSer.publish(&mid, "topic/test", strlen(buf), (const void*)buf);
			rc = mqttSer.loop();
		}
		//rc = mqttSer.disconnect();	// 订阅测时试注释该行
		//rc = mqttSer.loop(); 
		mqttSer.subscribe(NULL,g_subTopic.c_str());	// 订阅测试取消注释该行
		rc = mqttSer.loop_forever();					// 订阅测试取消注释该行
	}

		mosqpp::lib_cleanup();

		system("pause");
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门提示: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
