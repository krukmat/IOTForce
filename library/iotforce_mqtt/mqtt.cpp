#include <mqtt.h>
#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <PubSubClient.h>

void mqttReconnect(PubSubClient* mqttIPClient, const char* mqtt_ip_user, const char* mqtt_ip_password, const char* mqtt_ip_topic_subscribe, const char* mqtt_ip_topic_publish, const int MQTT_RETRYMS) {

  while (!mqttIPClient->connected()) {
    Serial.print("Intentando conexión Mqtt...");
    // Creamos un cliente ID
    String clientId = "IOTICOS_H_W_";
    clientId += String(random(0xffff), HEX);
    // Intentamos conectar
    if (mqttIPClient->connect(clientId.c_str(),mqtt_ip_user,mqtt_ip_password)) {
      Serial.println("Conectado!");
      // Nos suscribimos
      if(mqttIPClient->subscribe(mqtt_ip_topic_subscribe)){
        Serial.println("Suscripcion ok");
      }else{
        Serial.println("fallo Suscripciión");
      }
    } else {
      Serial.print("falló :( con error -> ");
      Serial.print(mqttIPClient->state());
      Serial.println(" Intentamos de nuevo en 5 segundos");
      delay(MQTT_RETRYMS);
    }
  }
}
void mqttLoop(PubSubClient* mqttIPClient, const char* mqtt_ip_user, const char* mqtt_ip_password, const char* mqtt_ip_topic_subscribe, const char* mqtt_ip_topic_publish, const int MQTT_RETRYMS){
  if (!mqttIPClient->connected()) {
      mqttReconnect(mqttIPClient, mqtt_ip_user, mqtt_ip_password, mqtt_ip_topic_subscribe, mqtt_ip_topic_publish, MQTT_RETRYMS);
   }  
   mqttIPClient->loop();
}

// https://stackoverflow.com/questions/9072320/split-string-into-string-array
String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length()-1;

  for(int i=0; i<=maxIndex && found<=index; i++){
    if(data.charAt(i)==separator || i==maxIndex){
        found++;
        strIndex[0] = strIndex[1]+1;
        strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }

  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}

String mqttBaseCallback(String deviceId, int* value , char* topic, byte* payload, unsigned int length){
  String incoming = "";
  Serial.print("Mensaje recibido desde -> ");
  Serial.print(topic);
  Serial.println("");
  for (int i = 0; i < length; i++) {
    incoming += (char)payload[i];
  }
  incoming.trim();
  Serial.println("Mensaje -> " + incoming); 

  if (getValue(incoming,';',0) == deviceId && getValue(incoming,';',3) == "MQTT"){
    Serial.println("New valid message");
    if (getValue(incoming,';',2).toInt() > 0){
       value = (int*) getValue(incoming,';',2).toInt();
       return getValue(incoming,';',1);
    }else{
      String param = getValue(incoming,';',1);
      Serial.println(param);
      if (param == "execute"){
            return param;
      }
      return "";
    }
  }
}

void bootUp(const String deviceId , PubSubClient* mqttIPClient, const char* mqtt_ip, const int mqtt_ip_port, const char* mqtt_ip_user, const char* mqtt_ip_password, const char* mqtt_ip_topic_subscribe, const char* mqtt_ip_topic_publish, const int MQTT_RETRYMS){
  char msg[12];
  deviceId.toCharArray(msg,deviceId.length()+1);
  if (!mqttIPClient->connected()) {
      mqttReconnect(mqttIPClient, mqtt_ip_user, mqtt_ip_password, mqtt_ip_topic_subscribe, mqtt_ip_topic_publish, MQTT_RETRYMS);
   }
  Serial.println("Connecting to mqtt");
  mqttIPClient->publish(mqtt_ip_topic_subscribe,msg);
}