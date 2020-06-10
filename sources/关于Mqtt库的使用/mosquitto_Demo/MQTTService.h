#pragma once

#include <mosquittopp.h>
#include <mosquitto.h>
#include <iostream>

using namespace std;


static std::string g_subTopic = "subTopic";

class MQTTService: public mosqpp::mosquittopp
{
public:
	MQTTService(const char *id):mosquittopp(id){ }
public:
	void on_connect(int rc) { cout << "on_connect" << endl; }
	void on_disconnect() { std::cout << "on_disconnect" << std::endl; }
	void on_publish(int mid) { std::cout << "on_publish" << std::endl; }
	void on_subscribe(int mid, int qos_count, const int *granted_qos);//订阅回调函数
	void on_message(const struct mosquitto_message *message);//订阅主题接收到消息
	
};