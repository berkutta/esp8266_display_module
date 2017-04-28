#include "esp_common.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include "lwip/netdb.h"
#include "lwip/dns.h"

#include "mqtt.h"

int socket_desc;
struct sockaddr_in server;

LOCAL void mqtt_create_socket(void) {
	socket_desc = socket(AF_INET , SOCK_STREAM , 0);
	if (socket_desc == -1)
	{
		printf("Could not create socket");
	}

	server.sin_addr.s_addr = inet_addr(MQTT_SERVER);
	server.sin_family = AF_INET;
	server.sin_port = htons( 1883 );

	//Connect to remote server
	if (connect(socket_desc , (struct sockaddr *)&server , sizeof(server)) < 0)
	{
		puts("Error on socket for MQTT Server");
		return;
	}

	int fred = 1;
	ioctlsocket(socket_desc,FIONBIO,&fred);

	puts("Created socket to MQTT Server\n");
}

LOCAL void mqtt_connect(void) {
	if( send(socket_desc , connectMessage , connectMessageLength, 0) < 0)
	{
		puts("Connect message failed");
		return;
	}

	while( !(recv(socket_desc, connack , 4 , 0) > 0) );

	if(connack[0] == 0x20 && connack[2] == 0x00) {
		puts("Succesfully connected to MQTT Server\n");
	} else {
		puts("Reeived wrong Connack from MQTT Server\n");
	}
}

LOCAL void mqtt_disconnect(void) {
	if( send(socket_desc , disconnectMessage , disconnectMessageLength , 0) < 0)
	{
	puts("Disconnect message failed");
	return;
	}
	puts("Disconnect message Send\n");
}

LOCAL void mqtt_publish(char *topic, uint8_t topic_length, char *payload, uint8_t payload_length) {
	uint8_t mqtt_publish_buffer[50];

	// MQTT Control Packet type (3), No DUP, QOS 0, No Retain
	mqtt_publish_buffer[0] = 0x30;

	// Calculate length of topic + payload + topic length byte + payload msb length byte
	uint16_t data_length = topic_length + payload_length + 1 + 1;

	mqtt_publish_buffer[1] = data_length & 0xFF;
	mqtt_publish_buffer[2] = (data_length & 0xFF00) >> 8;

	mqtt_publish_buffer[3] = topic_length;

	memcpy(mqtt_publish_buffer + 4, topic, topic_length);
	memcpy(mqtt_publish_buffer + topic_length + 4, payload, payload_length);

	//printf("topic length: %d, payload length: %d\n", topic_length, payload_length);

	int i = 0;
	for(i = 0; i <= topic_length + payload_length + 3; i++) {
		//printf("%x ", mqtt_publish_buffer[i]);
	}

	if( send(socket_desc, mqtt_publish_buffer, topic_length + payload_length + 4, 0) < 0)
	{
		puts("Push message failed");
		return;
	}
		//puts("Push message Send\n");
}

void mqtt_subscribe(char *topic, uint8_t topic_length) {
	uint8_t mqtt_subscribe_buffer[50];

	// https://image.slidesharecdn.com/mqtt-130828021319-phpapp01/95/mqtt-mq-telemetry-transport-for-message-queueing-19-638.jpg?cb=1465367894
	// MQTT fixed header
	mqtt_subscribe_buffer[0] = 0b10000010;
	mqtt_subscribe_buffer[1] = 4 + 1 + strlen(topic);
	// MQTT variable header
	mqtt_subscribe_buffer[2] = 0x00;
	mqtt_subscribe_buffer[3] = 10;
	// MQTT list of topics
	mqtt_subscribe_buffer[4] = (topic_length & 0xFF00) >> 8;
	mqtt_subscribe_buffer[5] = topic_length & 0xFF;

	memcpy(mqtt_subscribe_buffer + 6, topic, topic_length);
	mqtt_subscribe_buffer[topic_length + 6] = 0x00;

	if( send(socket_desc, mqtt_subscribe_buffer, topic_length + 7, 0) < 0)
	{
		puts("Subscribe failed");
		return;
	}
}

void mqtt_receive(void) {
	uint8_t buffer[1500];
	int err, length;

	length = recv(socket_desc, buffer, sizeof(buffer), 0);

	if( length > 0) {
    switch(buffer[0] & 0xF0) {
      case CONNECT:
        printf("Connect\n");
        break;

      case CONNACK:
        printf("Connack\n");
        break;

      case PUBLISH:
        printf("Publish\n");
        break;

      case PUBACK:
        printf("Puback\n");
        break;

      case PUBREC:
        printf("Pubrec\n");
        break;

      case PUBREL:
        printf("Pubrel\n");
        break;

      case PUBCOMP:
        printf("Pubcomp\n");
        break;

      case SUBSCRIBE:
        printf("Subscribe");
        break;

      case SUBACK:
        printf("Suback\n");
        break;

      case UNSUBSCRIBE:
        printf("Unsubscribe");
        break;

      case UNSUBACK:
        printf("Unsuback\n");
        break;

      case PINGREQ:
        printf("Pingreq\n");
        break;

      case PINGRESP:
        printf("Pingresp\n");
        break;

      case DISCONNECT:
        printf("Disconnect\n");
        break;
    }

    if((buffer[0] & 0b00110000) == 0x30) {
      printf("Received PUBLISH mess hallo\n");
    }

	}
}

void mqtt_ping(void) {
	uint8_t mqtt_ping_buffer[2];

	mqtt_ping_buffer[0] = 0b11010000;
	mqtt_ping_buffer[1] = 0x00;

	if( send(socket_desc, mqtt_ping_buffer, 2, 0) < 0)
	{
		puts("Ping failed");
		return;
	}
}

LOCAL void mqtt_task(void *pvParameters)
{
	// Put MAC from STATION_IF to string and generate Topic path
	uint8_t mac[6];
	char mac_string[17]; char topic[25];
	wifi_get_macaddr(0x00, mac);
	sprintf(mac_string, "%X:%X:%X:%X:%X:%X", mac[5], mac[4], mac[3], mac[2], mac[1], mac[0]);
	sprintf(topic, "esp/%s", mac_string);

	connectMessage[17] = mac[0];

	mqtt_create_socket();
	mqtt_connect();

	mqtt_publish(topic, strlen(topic), "sali", strlen("sali"));

	mqtt_subscribe("hoi/#", strlen("hoi/#"));

	while (1) {
		mqtt_ping();

		char payload[] = "Test!";
    sprintf(payload, "%s, Uptime: %lu", payload, system_get_time());
		mqtt_publish(topic, strlen(topic), payload, strlen(payload));

		mqtt_receive();

		vTaskDelay(100 / portTICK_RATE_MS);
	}

	vTaskDelete(NULL);
	printf("delete the mqtt_task\n");
}

void mqtt_start(void)
{
	xTaskCreate(mqtt_task, "mqtt_task", 1024, NULL, 4, NULL);
}
