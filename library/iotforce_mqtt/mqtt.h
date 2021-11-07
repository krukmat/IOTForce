#ifndef MQTT_H
#define MQTT_H
    #include <Arduino.h>
    #include <PubSubClient.h>
    void mqttLoop(PubSubClient* mqttIPClient, const char* mqtt_ip_user, const char* mqtt_ip_password, const char* mqtt_ip_topic_subscribe, const char* mqtt_ip_topic_publish, const int MQTT_RETRYMS);
    void bootUp(const String deviceId , PubSubClient* mqttIPClient, const char* mqtt_ip, const int mqtt_ip_port, const char* mqtt_ip_user, const char* mqtt_ip_password, const char* mqtt_ip_topic_subscribe, const char* mqtt_ip_topic_publish, const int MQTT_RETRYMS);
    String getValue(String data, char separator, int index);
#endif