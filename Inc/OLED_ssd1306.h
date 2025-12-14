/*
 * OLED_ssd1306.h
 *
 *  Created on: Dec 14, 2025
 *      Author: Amal
 */

#ifndef OLED_SSD1306_H_
#define OLED_SSD1306_H_

#include "stm32f4xx.h"
#include <stdint.h>

#define SSD1306_I2C_PERIPH    I2C1      /* I2C peripheral used for OLED */
#define SSD1306_ADDR_7BIT     0x3C
#define SSD1306_WIDTH         128       /* Display width in pixels */
#define SSD1306_HEIGHT        64        /* Display height in pixels */
#define SSD1306_BUFFER_SIZE   ((SSD1306_WIDTH * SSD1306_HEIGHT) / 8)


void ssd1306_init(void);

void ssd1306_clear(void);

void ssd1306_update(void);

void draw_time_on_oled(uint8_t hour, uint8_t minute, uint8_t second);

void draw_time_with_day_on_oled(uint8_t hour, uint8_t minute, uint8_t second, uint8_t day);

#endif /* OLED_SSD1306_H_ */
