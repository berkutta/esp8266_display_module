#include "esp_common.h"
#include "user_config.h"
#include "../display/display.h"
#include "../parameterisation/parameterisation.h"

uint32_t saved_mode;

uint32 user_rf_cal_sector_set(void)
{
    flash_size_map size_map = system_get_flash_size_map();
    uint32 rf_cal_sec = 0;

    switch (size_map) {
        case FLASH_SIZE_4M_MAP_256_256:
            rf_cal_sec = 128 - 5;
            break;

        case FLASH_SIZE_8M_MAP_512_512:
            rf_cal_sec = 256 - 5;
            break;

        case FLASH_SIZE_16M_MAP_512_512:
        case FLASH_SIZE_16M_MAP_1024_1024:
            rf_cal_sec = 512 - 5;
            break;

        case FLASH_SIZE_32M_MAP_512_512:
        case FLASH_SIZE_32M_MAP_1024_1024:
            rf_cal_sec = 1024 - 5;
            break;

        default:
            rf_cal_sec = 0;
            break;
    }

    return rf_cal_sec;
}

void ICACHE_FLASH_ATTR wifi_event_handler_cb(System_Event_t *event)
{
    printf("[WiFi] event %u\n", event->event_id);

    switch (event->event_id) {
        case EVENT_STAMODE_DISCONNECTED:
            printf("Disconnected!\n");
            break;
        case EVENT_STAMODE_CONNECTED:
            //myoledstatus = oled_display_cloud;
            printf("Connected!\n");
            break;
        case EVENT_STAMODE_GOT_IP:
            printf("Got IP!\n");
            //mqtt_start();
            break;
        default:
            break;
    }
}

void wifi_station_mode_init(void) {
    wifi_set_opmode(STATION_MODE);

    struct station_config config;
    memset(&config, 0, sizeof(struct station_config));
    strcpy(config.ssid, myssid);
    strcpy(config.password, mypassword);
    wifi_station_set_config(&config);

    wifi_station_connect();
}

void user_init(void)
{
    uart_div_modify(0, UART_CLK_FREQ / 115200);

    os_printf("SDK version:%s\n", system_get_sdk_version());

    display_start();

    #ifdef tindie_mode
    wifi_set_opmode(SOFTAP_MODE);

    myoledstatus = oled_display_tindie_logo;

    vTaskDelay(2500 / portTICK_RATE_MS);

    spi_flash_read(0x70000, (uint32 *)&saved_mode, sizeof(saved_mode));

    if(saved_mode == 1) {
        myoledstatus = oled_display_tindie_wirecube;
        saved_mode = 0;
    } else {
        myoledstatus = oled_display_tindie_gameoflife;
        saved_mode = 1;
    }

    spi_flash_erase_sector(0x70);
    spi_flash_write(0x70000, (uint32 *)&saved_mode, sizeof(saved_mode));
    
    #else

    wifi_set_event_handler_cb(wifi_event_handler_cb);

    wifi_station_mode_init();

    myoledstatus = oled_display_gameoflife;
    #endif
}
