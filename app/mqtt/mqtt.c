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

	puts("Created socket to MQTT Server\n");
}

LOCAL void mqtt_connect(void) {
	if( send(socket_desc , connectMessage , connectMessageLength, 0) < 0)
	{
		puts("Connect message failed");
		return;
	}

	if( recv(socket_desc, connack , 4 , 0) < 0)
	{
		puts("recv failed");
	}

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

LOCAL void mqtt_publish(void) {
	if( send(socket_desc , publishMessage , publishMessageLength , 0) < 0)
	{
	puts("Push message failed");
	return;
	}
	puts("Push message Send\n");
}

LOCAL void mqtt_task(void *pvParameters)
{
	mqtt_create_socket();
	mqtt_connect();

	while (1) {
		mqtt_publish();

		vTaskDelay(500 / portTICK_RATE_MS);
	}

	vTaskDelete(NULL);
	printf("delete the mqtt_task\n");
}

void mqtt_start(void)
{
	xTaskCreate(mqtt_task, "mqtt_task", 1024, NULL, 4, NULL);
}
