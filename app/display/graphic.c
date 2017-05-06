#include "esp_common.h"

#include "oled.h"
#include "font_5x7.h"
#include "font_10x8.h"

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

// https://de.wikipedia.org/wiki/Bresenham-Algorithmus
void graphic_line(int x0, int y0, int x1, int y1) {
  int dx =  abs(x1-x0), sx = x0<x1 ? 1 : -1;
  int dy = -abs(y1-y0), sy = y0<y1 ? 1 : -1;
  int err = dx+dy, e2; /* error value e_xy */

  while(1){
    oled_drawpixel(x0, y0, 1);
    if (x0==x1 && y0==y1) break;
    e2 = 2*err;
    if (e2 > dy) { err += dy; x0 += sx; } /* e_xy+e_x > 0 */
    if (e2 < dx) { err += dx; y0 += sy; } /* e_xy+e_y < 0 */
  }
}

// http://www.thecrazyprogrammer.com/2016/12/bresenhams-midpoint-circle-algorithm-c-c.html
void graphic_circle(int x0, int y0, int radius)
{
  int x = radius;
  int y = 0;
  int err = 0;

  while (x >= y)
  {
    oled_drawpixel(x0 + x, y0 + y, 1);
    oled_drawpixel(x0 + y, y0 + x, 1);
    oled_drawpixel(x0 - y, y0 + x, 1);
    oled_drawpixel(x0 - x, y0 + y, 1);
    oled_drawpixel(x0 - x, y0 - y, 1);
    oled_drawpixel(x0 - y, y0 - x, 1);
    oled_drawpixel(x0 + y, y0 - x, 1);
    oled_drawpixel(x0 + x, y0 - y, 1);

    if (err <= 0)
    {
      y += 1;
      err += 2*y + 1;
    }

    if (err > 0)
    {
      x -= 1;
      err -= 2*x + 1;
    }
  }
}

void graphic_rectangle(int x1, int y1, int x2, int y2, unsigned char fill)
{
  unsigned int xmin, xmax, ymin, ymax, i;

  if(fill == 1)
  {
    if(x1 < x2)
    {
      xmin = x1;
      xmax = x2;
    }
    else
    {
      xmin = x2;
      xmax = x1;
    }

    if(y1 < y2)
    {
      ymin = y1;
      ymax = y2;
    }
    else
    {
      ymin = y2;
      ymax = y1;
    }

    for( ;xmin<=xmax; xmin++) {
      for(i=ymin; i<=ymax; i++) {
        oled_drawpixel(xmin,i);
      }
    }
  } else {
      graphic_line(x1,y1,x2,y1);
      graphic_line(x1,y2,x2,y2);
      graphic_line(x1,y1,x1,y2);
      graphic_line(x2,y1,x2,y2);
  }
}

void graphic_putc_5x7(unsigned char x0, unsigned char y0, char ch) {
  uint8_t x, y;

  for(x = 0; x <= 4; x++) {
    for(y = 0; y <= 7; y++) {
      if(font_5x7[((ch - 32) * 5) + x] & (0x01<<y)) {
        oled_drawpixel(x + x0, y + y0, 1);
      }
    }
  }
}

void graphic_puts_5x7(unsigned char x0, unsigned char y0, char *str) {
  unsigned char i;
  unsigned char cur_x = x0;
	for (i=0; str[i] != '\0'; i++)
	{
	  graphic_putc_5x7(cur_x, y0, str[i]);
	  cur_x += 6;
	}
}

void graphic_putc_10x8(unsigned char x0, unsigned char y0, char ch) {
  uint8_t x, y;

  for(y = 0; y <= 8; y++) {
    for(x = 0; x <= 7; x++) {
      if(font_10x8[((ch-33) * 9) + y] & (0x80>>x)) {
        oled_drawpixel(x + x0, y + y0, 1);
      }
    }
  }
}

void graphic_puts_10x8(unsigned char x0, unsigned char y0, char *str) {
  unsigned char i;
  unsigned char cur_x = x0;
	for (i=0; str[i] != '\0'; i++)
	{
	  graphic_putc_10x8(cur_x, y0, str[i]);
	  cur_x += 6;
	}
}

void graphic_clear(void) {
  uint8_t x, y;

  for(x = 0; x < 129; x++) {
    for(y = 0; y < 65; y++) {
        oled_drawpixel(x, y, 0x00);
    }
  }
}

void graphic_update(void) {
  oled_update();
}
