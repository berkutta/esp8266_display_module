#include "esp_common.h"
#include "string.h"

#include "parameterisation.h"

void parameterisation_scan_done(void *arg, STATUS status) {
    uint8 ssid[33];
    char temp[128];

    if (status == OK)
    {
        struct bss_info *bss_link = (struct bss_info *)arg;

        if(bss_link != NULL) {
            printf("Found configuration WiFi\n");

            struct station_config config;
            memset(&config, 0, sizeof(struct station_config));
            strcpy(config.ssid, parameterisation_ssid);
            strcpy(config.password, parameterisation_password);
            wifi_station_set_config(&config);

            wifi_station_connect();
        } else {
            printf("No configuration WiFi\n");
        }
    }
    else
    {
        printf("Error\n");
    }
}

void parameterisation_task(void *pvParameters) {
    parameterisation_data_t parameterisation_dataset;

    spi_flash_read(0x10000, (uint32 *) &parameterisation_dataset, sizeof(parameterisation_dataset));

    if( (parameterisation_dataset.serial_number == 0) || (parameterisation_dataset.serial_number == 255) ) {
        printf("Dataset is not set\n");
    }

    struct scan_config config;
    memset(&config, 0, sizeof(config));
    config.ssid = parameterisation_ssid;
    wifi_station_scan(&config, parameterisation_scan_done);

    while(1) {
        if(wifi_station_get_connect_status() == STATION_GOT_IP) {
            printf("Got a damned IP!!\n");

            //socket_desc = socket(AF_INET , SOCK_STREAM , 0);
            //if (socket_desc == -1)
            //{
            //    printf("Could not create socket");
            //}

            //server.sin_addr.s_addr = inet_addr(MQTT_SERVER);
            //server.sin_family = AF_INET;
            //server.sin_port = htons( 1883 );

            //Connect to remote server
            //if (connect(socket_desc , (struct sockaddr *)&server , sizeof(server)) < 0)
            //{
            //    puts("Error on socket for HTTP Server");
            //    return;
            //}

        }
        vTaskDelay(100 / portTICK_RATE_MS);
    }
}

void parameterisation_start(void)
{
	xTaskCreate(parameterisation_task, "parameterisation_task", 1024, NULL, 4, NULL);
}
