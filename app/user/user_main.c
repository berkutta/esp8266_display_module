#include "esp_common.h"
#include "user_config.h"
#include "../display/display.h"
#include "../parameterisation/parameterisation.h"

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
            myoledstatus = oled_display_cloud;
            printf("Connected!\n");
            break;
        case EVENT_STAMODE_GOT_IP:
            printf("Got IP!\n");
            mqtt_start();
            break;
        default:
            break;
    }
}

void user_init(void)
{
    uart_div_modify(0, UART_CLK_FREQ / 115200);

    os_printf("SDK version:%s\n", system_get_sdk_version());

    display_start();

    wifi_set_event_handler_cb(wifi_event_handler_cb);

    #ifdef tindie_mode
    wifi_set_opmode(SOFTAP_MODE);

    myoledstatus = oled_display_tindie_logo;

    vTaskDelay(2500 / portTICK_RATE_MS);

    myoledstatus = oled_display_tindie_wirecube;
    #else
    myoledstatus = oled_display_wirecube;
    #endif
}
