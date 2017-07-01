#ifndef DISPLAY_H
#define DISPLAY_H

typedef enum oled_status {
  oled_connecting_wifi,
  oled_display_cloud,
  oled_display_chip,
  oled_display_wirecube,
  oled_display_gameoflife,
  oled_display_oscilloscope,
  oled_display_tindie_logo,
  oled_display_tindie_gameoflife,
  oled_display_tindie_wirecube,
} oled_status_t;

oled_status_t myoledstatus;

uint8_t mytext[20];
#endif
