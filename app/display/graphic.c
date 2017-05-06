#include "esp_common.h"

#include "oled.h"

void graphic_init(void) {
  oled_init();
  system_update_cpu_freq(160);
}

void graphic_show_image(uint8_t image[64][16]) {
  uint8_t x, y, x_pos;

  for(x = 0; x < 16; x++) {
    for(y = 0; y < 65; y++) {
      uint8_t image_byte = image[y][x];
      for(x_pos = 0; x_pos < 8; x_pos++) {
        oled_drawpixel((x*8) + x_pos, y, (image_byte >> (7-x_pos)) & 0x01);
      }
    }
  }
}

void graphic_update(void) {
  oled_update();
}
