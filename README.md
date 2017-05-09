ESP8266 Display Module
================

This software is primarly made for my [OLED Display Module](https://www.tindie.com/products/kilobyte/oled-esp8285-module/). It currently just displays it's status on the display and send's some status informations to a predefined MQTT Server.

My goal is to get a full CLI based system over MQTT. The plan is to be able to e.g. set GPIO's to high/low or even do advanced stuff like integrated WS2812/SK6812 driving capability. Drawing things on the OLED Displays with simple graphic commands is already mostly implemented. All that should be controllable over MQTT. For first time configuration I'm planning to implement an WiFi Ad-Hoc mode on which you can connect from your computer and then configure it via Webinterface (WiFi credentials, MQTT Server etc.).

All that features aren't implemented within 5 minutes. So this is really a long term project to finish. I love to design new hardware and thus probably never finish this software to 100%.

![Module](https://kilobyte.ch/stuff/tindie/OLED_ESP8285_Module/display_module_case_small.png "Module")

The whole software is designed to run as fast as possible. Thus the whole system has absolute no issues with spamming the MQTT Server and changing the Display at the same time:

![fast](https://raw.githubusercontent.com/berkutta/esp8266_display_module/master/images/system_speed.gif "fast")

You can even do amazing stuff like drawing an animated cube!:

![cube](https://raw.githubusercontent.com/berkutta/esp8266_display_module/master/images/system_cube.gif "cube")

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
