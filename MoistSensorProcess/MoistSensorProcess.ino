#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <PubSubClient.h>
#include <WebServer.h>
#include "GardenForce_consts.h"

const String deviceId = "PLANT_1";
WiFiClient mqttIPClientWifi;
PubSubClient mqttIPClient( mqttIPClientWifi );

#include <mqtt.h>


typedef struct shoot_on_demand {
  bool shoot;
  String deviceId;
};
shoot_on_demand esp_now_data;

volatile int moistValue = 3200;

TaskHandle_t taskSendStatus;



void mqttCallback(char* topic, byte* payload, unsigned int length){
      int value = getIntValue(deviceId, payload, length);
      String parameter = getCommand(deviceId, topic, payload, length);
      
      if (value > 0){
        Serial.println("New value: "+ String(value));
        if (parameter == "moist"){
            moistValue = value;
         }
         if (parameter == "read_ms")
            readMs = value;
         if (parameter == "flow_ms"){
            Serial.println("New flow_ms:" + String(value));
            flowMS = value;
            defaultFlowMS = flowMS;
         }        
      }
      if (parameter == "hidrate"){
            Serial.println("execute");
            handle_hidrate();
            delay(flowMS);
      }

}

void taskSendStatusMethod( void * parameter) {
  String statusMsg;
  for(;;) {
    mqttLoop(&mqttIPClient, mqtt_ip_user, mqtt_ip_password, mqtt_ip_topic_subscribe, mqtt_ip_topic, MQTT_RETRYMS);
  
    
    if (sensorStatus > 0){
      statusMsg = deviceId+";"+String(sensorStatus);
      statusMsg.toCharArray(msg,statusMsg.length()+1);
      mqttIPClient.publish(mqtt_ip_topic_subscribe,msg);
      delay(readMs);      
    } else {
      delay(300);
    }
  }
}

// it wil set the static IP address to 192, 168, 10, 47
IPAddress local_IP(192, 168, 1, 135);
//it wil set the gateway static IP address to 192, 168, 2,2
IPAddress gateway(192, 168, 1 ,1);

// Following three settings are optional
IPAddress subnet(255, 255, 0, 0);
IPAddress primaryDNS(8, 8, 8, 8); 
IPAddress secondaryDNS(8, 8, 4, 4);


void setup() {
  // put your setup code here, to run once:
  //WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); //disable brownout detector
  Serial.begin(115200);
  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
    Serial.println("STA Failed to configure");
  }
  pinMode(MOIST_SENSOR,INPUT);
  pinMode(RELAY,OUTPUT);
  digitalWrite(RELAY, 1); // Disable by default
  
  Serial.println("The device started, now you can pair it with bluetooth!");

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  Serial.print("SensorForce Ready!");
  Serial.println(WiFi.macAddress());
  Serial.print(WiFi.localIP());

  mqttIPClient.setServer(mqtt_ip, mqtt_ip_port);
  mqttIPClient.setCallback(mqttCallback);
  bootUp(deviceId, &mqttIPClient, mqtt_ip, mqtt_ip_port, mqtt_ip_user, mqtt_ip_password, mqtt_ip_topic_subscribe, mqtt_ip_topic, MQTT_RETRYMS);

  xTaskCreatePinnedToCore(
      taskSendStatusMethod, 
      "taskSendStatus", 
      10000,  
      NULL,  
      0,  
      &taskSendStatus,  
      0);
 //Blynk.begin(blynk_auth, ssid, password);
}


// This function will be called every time Slider Widget
// in Blynk app writes values to the Virtual Pin 1
void handle_hidrate(void)
{
  Serial.println(flowMS);
  digitalWrite(RELAY, 0);
  delay(flowMS);
  // 1 minuto - flowMS parado
  digitalWrite(RELAY,1);
}



void loop() {
  mqttLoop(&mqttIPClient, mqtt_ip_user, mqtt_ip_password, mqtt_ip_topic_subscribe, mqtt_ip_topic, MQTT_RETRYMS);
  sensorStatus = analogRead(MOIST_SENSOR);
}
