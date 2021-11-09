#!/usr/bin/python3
import paho.mqtt.client as mqtt
from salesforce_api import Salesforce
from sf_creds import SF_USER, SF_PASSWORD, SF_ACCESS_TOKEN
import json

sf = Salesforce(username=SF_USER, password=SF_PASSWORD, security_token=SF_ACCESS_TOKEN)

def on_connect(client, userdata, flags, rc):
    print("Connected with result code "+str(rc))
    client.subscribe("HPIbCG0C72lcw6g/input")

def getGardenForceData(data, client, userdata, msg):    
                plant_id = data
                try:
                    result = sf.sobjects.query("SELECT moist__c,refresh__c,flowMS__c FROM Plant__c WHERE PlantId__c = '"+plant_id+"'")
                    moist = int(result[0]['moist__c'])
                    refresh = int(result[0]['refresh__c'])
                    flow=int(result[0]['flowMS__c'])

                    message = plant_id + ";moist;"+ str(moist) + ";MQTT"
                    client.publish("HPIbCG0C72lcw6g/input", message)
                    message = plant_id + ";read_ms;"+ str(refresh) + ";MQTT"
                    client.publish("HPIbCG0C72lcw6g/input", message)
                    message = plant_id + ";flow_ms;"+ str(flow) + ";MQTT"
                    client.publish("HPIbCG0C72lcw6g/input", message)
                    
                except:
                    print("Error")

def on_message(client, userdata, msg):
    print(msg.topic+" "+str(msg.payload))
    data = msg.payload
    data = data.decode('utf8').replace("'", '"')
    print(data)
    # Evitar el loopback de MQTT
    if  not(";MQTT" in data):
        #TODO: Caso donde se envia PlantId;valor de humedad
        if ";" in data:
            parameters = data.split(";")
            # TODO: reemplazar por Platform Events para evitar el limite de API
            # device_type, device_id, value
            device_type = parameters[0].split('_')[0] #PLANT
            result = sf.sobjects.DeviceMessage__e.insert({"device_Id__c": parameters[0], "device_type": device_type, "value__c": parameters[1]}) #TODO: Platform events instead?
        else:
            # Caso que se recibe desde el ESP-32 para que se devuelva el valor desde SF
            # TODO: Pasar a funcion especifica de gardenforce
            if "PLANT" in data:
                getGardenForceData(data, client, userdata, msg)

def mqtt_publish(client, message):
    client.publish("HPIbCG0C72lcw6g/input", message)

def connect_mqtt():
    client = mqtt.Client()
    client.on_connect = on_connect
    client.on_message = on_message
    client.username_pw_set(username="d2uNYOZq2J5uL3T", password="DwqGBXa7LuVlgBG")
    client.connect("ioticos.org", 1883, 60)
    return client
