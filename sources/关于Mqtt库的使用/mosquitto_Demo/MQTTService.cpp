#include "pch.h"
#include "MQTTService.h"



void MQTTService::on_subscribe(int mid, int qos_count, const int *granted_qos)
{
	cout << "���� mid:" << mid << endl;
}

void MQTTService::on_message(const struct mosquitto_message *message)
{
	bool res = false;
	mosqpp::topic_matches_sub(g_subTopic.c_str(), message->topic, &res);
	if (res)
	{
		string strRecv = (char*)message->payload;
		cout << "����" << message->topic << ">����Ϣ" << strRecv.c_str() << endl;
	}
}




