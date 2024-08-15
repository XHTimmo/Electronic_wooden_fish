#ifndef __SSD1306_H__
#define __SSD1306_H__

#include "stdint.h"
#define SSD1306_ADDR (0x3C)

void SSD1306_init();

void SSD1306_fill(uint8_t *data_buf);
#endif