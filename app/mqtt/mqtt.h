#ifndef MQTT_H
#define MQTT_H

#define MQTT_SERVER "172.16.90.120"

char connack[4];

uint8_t connectMessage[] = {0x10 //Connect
  , 0x0C + 0x04 //Remaining Length
  , 0x00 //0
  , 0x06 //6
  , 0x4d //M
  , 0x51 //Q
  , 0x49 //I
  , 0x73 //S
  , 0x64 //D
  , 0x70 //P
  , 0x03 //Protocol version = 3
  , 0x02 //Clean session only
  , 0x00 //Keepalive MSB
  , 0x3c //Keepaliave LSB = 60
  , 0x00 //String length MSB
  , 0x02 //String length LSB = 2
  , 0x4d //M
  , 0x70 //P .. Let's say client ID = MP
};

uint8_t publishMessage[] = { 0x30 //Publish with QOS 0
  , 0x05 + 0x05 //Remaining length
  , 0x00 //MSB
  , 0x03 //3 bytes of topic
  , 0x61 //a
  , 0x2F ///
  , 0x62 //b (a/b) is the topic
  , 0x48, 0x45 , 0x4c , 0x4c, 0x4f //HELLO is the message
};

uint8_t disconnectMessage[] = { 0x0E //Disconnect
    , 0x00
};

#endif
