ESP8266 Display Module
================

This software is primarly made for my [OLED Display Module](https://www.tindie.com/products/kilobyte/oled-esp8285-module/): . It currently just displays it's status on the display and send's some status informations to a predefined MQTT Server.

My goal is to get a full CLI based system over MQTT. The plan is to be able to e.g. set GPIO's or drawing things on the OLED Displays with simple commands over MQTT.

![Module](https://kilobyte.ch/stuff/tindie/OLED_ESP8285_Module/display_module_case_small.png "Module")

The whole software is designed to run as fast as possible. Thus the whole system has absolute no issues with spamming the MQTT Server and changing the Display at the same time:

![fast](https://raw.githubusercontent.com/berkutta/esp8266_display_module/master/images/system_speed.gif "fast")

Usage
=================
Set your SSID + Password in 'app/user/user_config.h'

Set your MQTT Server in 'app/mqtt/mqtt.h"

Install the compiler + sdk and configure the paths in 'app/Makefile'

Then just connect your ESP8266 via USB and run the following command:

`make flash`


Environment
=================
I used a simple environment with an https://mosquitto.org/ MQTT Broker with the standard configuration under Debian. As a client on the PC side I've used the MQTT.fx software.

![Screenshot](https://raw.githubusercontent.com/berkutta/ESP8266_MQTT/master/images/mqttfx.jpeg "Screenshot")
