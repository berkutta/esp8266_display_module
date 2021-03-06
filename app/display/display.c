#include "esp_common.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "display.h"

#include "images.c"
#include "graphic.h"

#include "wirecube.h"
#include "gameoflife.h"
#include "oscilloscope.h"

LOCAL void display_task(void *pvParameters)
{
	graphic_init();

	wirecube_init();
	gameoflife_init();

	uint32_t last_system_time;
	uint8_t myuptime_string[10];
	uint8_t ms;
	float fps;

	while (1) {
		static int counter;

		switch(myoledstatus) {
			case oled_connecting_wifi:
				switch(counter) {
					case 0:
					graphic_show_image(&loading_0);
					counter++;
					break;

					case 1:
					graphic_show_image(&loading_1);
					counter++;
					break;

					case 2:
					graphic_show_image(&loading_2);
					counter++;
					break;

					case 3:
					graphic_show_image(&loading_3);
					counter = 0;
					break;
					}
				graphic_puts_5x7(30, 45, "Connecting...");
				break;

		case oled_display_cloud:
			graphic_show_image(&cloud);
			graphic_puts_5x7(35, 45, "Connected!");
			graphic_puts_5x7(35, 25, mytext);
			break;

		case oled_display_wirecube:
			ms = ( ( system_get_time() - last_system_time) / 1000 );
			fps = (float)((float)1 / ((float)ms*(float)0.001));

			sprintf(myuptime_string, "%.2f FPS", fps);
			graphic_clear();
			graphic_puts_5x7(75, 5, myuptime_string);
			last_system_time = system_get_time();

			wirecube_render();

			break;

		case oled_display_gameoflife:
			graphic_clear();

			gameoflife_render();

			break;

		case oled_display_chip:
			graphic_show_image(&chip);
			break;
		
		case oled_display_oscilloscope:
			oscilloscope_render();
			break;

		case oled_display_tindie_logo:
			graphic_show_image(&tindie);
			break;

		case oled_display_tindie_gameoflife:
			ms = ( ( system_get_time() - last_system_time) / 1000 );
			fps = (float)((float)1 / ((float)ms*(float)0.001));

			sprintf(myuptime_string, "%.2f FPS", fps);
			graphic_clear();
			graphic_puts_5x7(75, 5, myuptime_string);
			last_system_time = system_get_time();

			gameoflife_render();

			uint8_t mac1[6];
			char mac_string1[17];
			wifi_get_macaddr(0x00, mac1);
			sprintf(mac_string1, "%02X:%02X:%02X:%02X:%02X:%02X", mac1[0], mac1[1], mac1[2], mac1[3], mac1[4], mac1[5]);

			graphic_puts_5x7(5, 45, "Softap WiFi");
			graphic_puts_5x7(5, 55, mac_string1);

			break;

		case oled_display_tindie_wirecube:
			ms = ( ( system_get_time() - last_system_time) / 1000 );
			fps = (float)((float)1 / ((float)ms*(float)0.001));

			sprintf(myuptime_string, "%.2f FPS", fps);
			graphic_clear();
			graphic_puts_5x7(75, 5, myuptime_string);
			last_system_time = system_get_time();


			wirecube_render();

			uint8_t mac[6];
			char mac_string[17];
			wifi_get_macaddr(0x00, mac);
			sprintf(mac_string, "%02X:%02X:%02X:%02X:%02X:%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

			graphic_puts_5x7(5, 45, "Softap WiFi");
			graphic_puts_5x7(5, 55, mac_string);

			break;
	}

	graphic_update();

	vTaskDelay(10 / portTICK_RATE_MS);
}

vTaskDelete(NULL);
printf("delete the display_task\n");
}

void display_start(void)
{
	xTaskCreate(display_task, "display_task", 1024, NULL, 4, NULL);
}
