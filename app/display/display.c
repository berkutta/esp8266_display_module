#include "esp_common.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "oled.h"

LOCAL void display_task(void *pvParameters)
{
  oled_init();

  oled_drawpixel(0, 0, 1);
  oled_drawpixel(0, 7, 1);

  //oled_drawpixel(10, 1, 0);

  //oled_drawpixel(50, 2, 0);

  oled_update();

  while(1) {

  }
  /*
	i2c_master_gpio_init();
	i2c_master_init();

	system_update_cpu_freq(160);

	oled_init();

  uint8_t myThing = 0;

	oled_clear_buffer();

	while (1) {
		//oled_noise_generator();

		static int counter;

		switch(myoledstatus) {
			case oled_connecting_wifi:
				switch(counter) {
					case 0:
						oled_write_buffer(loading_0);
						counter++;
						break;

					case 1:
						oled_write_buffer(loading_1);
						counter++;
						break;

					case 2:
						oled_write_buffer(loading_2);
						counter++;
						break;

					case 3:
						oled_write_buffer(loading_3);
						counter = 0;
						break;
				}
				break;

			case oled_display_cloud:
				oled_write_buffer(cloud);
				break;

			case oled_display_chip:
				oled_write_buffer(chip);
				break;
		}

		vTaskDelay(100 / portTICK_RATE_MS);
	}
  */

	vTaskDelete(NULL);
	printf("delete the json_task\n");
}

void display_start(void)
{
	xTaskCreate(display_task, "display_task", 1024, NULL, 4, NULL);
}
