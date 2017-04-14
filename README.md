ESP8266 MQTT
================

This software is a completly stupid proof of concept to just get MQTT running on a ESP8266 with the RTOS on an own sofware implementation.

It currently can't do more than connect to the MQTT Server and spam it with predefined data.

You have to set your SSID + Password in 'app/user/user_config.h' and configure the paths in 'app/Makefile'

Usage
=================

`make flash`
