#!/usr/bin/python3
import sys, os
sys.path.append('/rest /var/www/html/IOTForce/python/')
from flask import Flask, request
import mqtt_proxy
app = Flask(__name__)

@app.route('/setup')
def index():
    deviceid = request.args.get('device_id')
    for key in request.args.keys():
        if key!='device_id' and deviceid and request.args[key]:
            message = deviceid +  ";" +  key +';'+request.args[key] + ";MQTT"
            client = mqtt_proxy.connect_mqtt()
            mqtt_proxy.mqtt_publish(client, message)
            return 'OK'
    return 'KO' 

@app.route('/execute')
def execute_task():
    device_id = request.args.get('device_id')
    message = device_id + ";execute;0;MQTT"
    if device_id:
        client = mqtt_proxy.connect_mqtt()
        mqtt_proxy.mqtt_publish(client, message)
        return 'OK'
    return 'KO'


    