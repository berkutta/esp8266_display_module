#include "esp_common.h"
#include "i2c_master.h"

#include "oled.h"

void oled_write_command(unsigned char command)
{
	unsigned char ack = 0;

	i2c_master_start();

	i2c_master_writeByte(SSD1306_address << 1);

	ack = i2c_master_checkAck();

	i2c_master_writeByte(0x00);

	ack = i2c_master_checkAck();

	i2c_master_writeByte(command);

	ack = i2c_master_checkAck();

	i2c_master_stop();
}

void oled_write_data(unsigned char data)
{
	unsigned char ack = 0;

	i2c_master_start();

	i2c_master_writeByte(SSD1306_address << 1);

	ack = i2c_master_checkAck();

	i2c_master_writeByte(0x40);

	ack = i2c_master_checkAck();

	i2c_master_writeByte(data);

	ack = i2c_master_checkAck();

	i2c_master_stop();
}

void oled_write_data_n(unsigned char *data, unsigned char length)
{
	unsigned char ack = 0;

	i2c_master_start();

	i2c_master_writeByte(SSD1306_address << 1);

	ack = i2c_master_checkAck();

	i2c_master_writeByte(0x40);

	ack = i2c_master_checkAck();

	unsigned char i;
	for(i = 0; i < length; i++) {
		i2c_master_writeByte(data[i]);
		ack = i2c_master_checkAck();
	}

	i2c_master_stop();
}

void oled_init(void) {
	i2c_master_gpio_init();
	i2c_master_init();

	// OFF
	oled_write_command(SSD1306_DISPLAYOFF);                    // 0xAE

	// Set MUX Ratio
	oled_write_command(SSD1306_SETMULTIPLEX);                  // 0xA8
	oled_write_command(0x3F);

	// Set Display Offset
	oled_write_command(SSD1306_SETDISPLAYOFFSET);              // 0xD3
	oled_write_command(0x0);                                   // no offset

	// Set Display Start Line
	oled_write_command(SSD1306_SETSTARTLINE | 0x0);            // line #0

	// Set Memory Addressing Mode
	oled_write_command(SSD1306_MEMORYMODE);                    // 0x20
	oled_write_command(0x10);                                  // 0x0 act like ks0108

	// Set Segment  re-map
	oled_write_command(SSD1306_SEGREMAP | 0x1);

	// Set COM Output Scan Direction
	oled_write_command(SSD1306_COMSCANDEC);

	// Set COM Pins hardware configuration
	oled_write_command(SSD1306_SETCOMPINS);                    // 0xDA
	oled_write_command(0x12);

	// Set Contrast Control
	oled_write_command(SSD1306_SETCONTRAST);                   // 0x81
	oled_write_command(0xF0);

	// Disable Entire Display On
	oled_write_command(SSD1306_DISPLAYALLON_RESUME);           // 0xA4

	// Set Normal Display
	oled_write_command(SSD1306_NORMALDISPLAY);                 // 0xA6

	// Set Osc Frequency
	oled_write_command(SSD1306_SETDISPLAYCLOCKDIV);            // 0xD5
	oled_write_command(0x80);

	oled_write_command(SSD1306_SETPRECHARGE);                  // 0xd9
	oled_write_command(0xF1);
	oled_write_command(SSD1306_SETVCOMDETECT);                 // 0xDB
	oled_write_command(0x40);

	// Enable charge pump regulator
	oled_write_command(SSD1306_CHARGEPUMP);                    // 0x8D
	oled_write_command(0x14);								  // Enable Charge Pump

	// Display On
	oled_write_command(SSD1306_DISPLAYON);

	// Set column range
	oled_write_command(SSD1306_COLUMNADDR);
	oled_write_command(0);   // Column start address (0 = reset)
	oled_write_command(SSD1306_LCDWIDTH - 1); // Column end address (127 = reset)

	// Set page range
	oled_write_command(SSD1306_PAGEADDR);
	oled_write_command(0); // Page start address (0 = reset)

	oled_write_command(7); // Page end address
}

void oled_goto_x_y(uint8_t x, uint8_t y) {
		// Set column (x) address
		oled_write_command(0x00 | (x & 0x0F));
		oled_write_command(0x10 | ((x & 0xF0) >> 4));

		// Set page (y) address
		oled_write_command(0xB0 | y);
}

void oled_update() {
	uint8_t line, width;

	for(line = 0; line < 8; line++) {
		oled_goto_x_y(0, line);

		oled_write_data_n(&SSD1306_buffer[SSD1306_LCDWIDTH * line], SSD1306_LCDWIDTH);
	}
}

void oled_drawpixel(uint8_t x, uint8_t y, uint8_t bit) {
	if( x >= SSD1306_LCDWIDTH || y >= SSD1306_LCDHEIGHT) {
		return;
	}

	if(bit == true) {
		SSD1306_buffer[x + (y/8) * SSD1306_LCDWIDTH] |= 1 << (y % 8);
	} else {
		SSD1306_buffer[x + (y/8) * SSD1306_LCDWIDTH] &= ~(1 << (y % 8));
	}
}

/* end-of-file-found */
