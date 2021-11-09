# IOTForce
Official Framework for IOTForce projects

The stack selected:
* Features in Salesforce:
    * Online monitoring.
    * Scheduling capabilities.
* IOT Proxy:
    * Ioticos(MQTT)
*  Hardware:
    * ESP32
* Software:         
    * Arduino IDE
    * Python3 (Flask, SalesforceAPI)
    * Salesforce developments (see sf folder)
## General Schema
![Schematics](https://github.com/krukmat/GardenForce-Demo/blob/159c5497d52ea9f4a9dd97e8ecaf703c78a2e915/images/GardenForce.jpeg)
##  Hardware
![Schematics](https://github.com/krukmat/GardenForce-Demo/blob/7f40088fdd00eb0f9112b119c42a06c2868b97db/images/version2.jpg)
![Schematics](https://github.com/krukmat/GardenForce-Demo/blob/7f40088fdd00eb0f9112b119c42a06c2868b97db/images/version2_internal.jpg)

## Salesforce
![Config](https://github.com/krukmat/GardenForce-Demo/blob/59f1f45d0b90cbab48707d1845b6b19c51e80a89/images/screenshot_sf.png)
![History](https://github.com/krukmat/GardenForce-Demo/blob/dab6476364e0883ef06dd59ba2e45e5e8e7263f6/images/salesforce-history.png)
![Report](https://github.com/krukmat/GardenForce-Demo/blob/dc64923d7f9524e4d4696be8e0405b1f2916938b/images/report.jpg)


## Reload secuence
* (python folder) nohup python3 ./forever.py > gardenforce.log &
* systemctl reload apache2
* Review errors: 
       * cd /var/log/apache2/
       * cat access.log
       * cat error.log

## More information about the framework: 
-------------------------------------------------------
* https://krukforce.com/our-vision-iotforce/