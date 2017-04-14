ESP8266 MQTT
================

This software is a completly stupid proof of concept to just get MQTT running on a ESP8266 with the RTOS on an own sofware implementation.

It currently can't do many things. It connect's to the WiFi in station mode, waits until it get's an IP and then starts the task to publish messages to the MQTT Broker.


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
