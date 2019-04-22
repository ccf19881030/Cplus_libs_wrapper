#include "pch.h"
#include "MQTTService.h"



void MQTTService::on_subscribe(int mid, int qos_count, const int *granted_qos)
{
	cout << "订阅 mid:" << mid << endl;
}

void MQTTService::on_message(const struct mosquitto_message *message)
{
	bool res = false;
	mosqpp::topic_matches_sub(g_subTopic.c_str(), message->topic, &res);
	if (res)
	{
		string strRecv = (char*)message->payload;
		cout << "来自" << message->topic << ">的消息" << strRecv.c_str() << endl;
	}
}




