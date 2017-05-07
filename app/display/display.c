#include "esp_common.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "display.h"

#include "images.c"
#include "graphic.h"

LOCAL void display_task(void *pvParameters)
{
  graphic_init();

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

			case oled_display_chip:
				graphic_show_image(&chip);
				break;
		}

    //graphic_clear();
    //graphic_line(0, 0, 127, 63);
    //graphic_line(0, 63, 127, 0);
    //graphic_line(31, 31, 95, 31);
    //graphic_line(63, 0, 63, 63);
    //graphic_circle(63, 31, 32);
    //graphic_rectangle(0, 0, 127, 63);
    //graphic_puts_5x7(20, 40, "Hello World!");
    graphic_update();

		vTaskDelay(100 / portTICK_RATE_MS);
	}

	vTaskDelete(NULL);
	printf("delete the display_task\n");
}

void display_start(void)
{
	xTaskCreate(display_task, "display_task", 1024, NULL, 4, NULL);
}
