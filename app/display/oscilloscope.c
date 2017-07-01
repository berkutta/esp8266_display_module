#include "esp_common.h"
#include "math.h"

#include "oscilloscope.h"

//#define ETS_INTR_LOCK taskENTER_CRITICAL;
//#define ETS_INTR_UNLOCK taskEXIT_CRITICAL;

void oscilloscope_render(void) {
    uint16_t adc_buffer[128];
    uint32_t buffer;
    int i;

    //wifi_set_opmode(0);

    taskENTER_CRITICAL();

    for(i = 0; i <= 127; i++) {
        adc_buffer[i] = system_adc_read();

        os_delay_us(10);
    }

    taskEXIT_CRITICAL();


    if((adc_buffer[64] > 500) && (adc_buffer[64] > adc_buffer[63])) {
        //printf("Trigger value: %d \n", adc_buffer[64]);

        graphic_clear();

        for(i = 0; i <= 127; i++) {
            oled_drawpixel(i, ((adc_buffer[i]/64) + 15) * 2, 1);
        }

        char trigger_string[20];
        sprintf(trigger_string, "Triggered at: %d", adc_buffer[64]);
        graphic_puts_5x7(5, 15, trigger_string);
    }
}
