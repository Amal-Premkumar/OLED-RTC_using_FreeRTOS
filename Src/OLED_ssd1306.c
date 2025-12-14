/*
 * OLED_ssd1306.c
 *
 *  Created on: Dec 14, 2025
 *      Author: Amal
 */


#include "stm32f4xx.h"
#include "OLED_ssd1306.h"
#include <string.h>
#include <stdint.h>
#include "system_coreclock.h"


#ifndef I2C_WRITE_DECLARED
extern int i2c_write(I2C_TypeDef *i2c, uint8_t addr7, const uint8_t *buf, uint16_t len);
#endif

static uint8_t oled_framebuffer[SSD1306_BUFFER_SIZE];

static int i2c_write_checked(I2C_TypeDef *i2c, uint8_t addr7, const uint8_t *buf, uint16_t len)
{
    const int max_attempts = 3;
    for (int attempt = 0; attempt < max_attempts; ++attempt) {
        if (i2c_write(i2c, addr7, buf, len)) {
            return 1;
        }
        for (volatile int d = 0; d < 2000; ++d) __asm__ volatile ("nop");
    }
    return 0;
}

static int ssd1306_send_command(uint8_t command)
{
    uint8_t pkt[2];
    pkt[0] = 0x00;
    pkt[1] = command;
    return i2c_write_checked(SSD1306_I2C_PERIPH, SSD1306_ADDR_7BIT, pkt, 2);
}


void ssd1306_init(void)
{
    for (volatile uint32_t d = 0; d < 1000000U; d++) __asm volatile ("nop");

    ssd1306_send_command(0xAE);
    ssd1306_send_command(0xD5); ssd1306_send_command(0x80);
    ssd1306_send_command(0xA8); ssd1306_send_command(0x3F);
    ssd1306_send_command(0xD3); ssd1306_send_command(0x00);
    ssd1306_send_command(0x40);
    ssd1306_send_command(0x8D); ssd1306_send_command(0x14);
    ssd1306_send_command(0x20); ssd1306_send_command(0x00);
    ssd1306_send_command(0xA1);
    ssd1306_send_command(0xC8);
    ssd1306_send_command(0xDA); ssd1306_send_command(0x12);
    ssd1306_send_command(0x81); ssd1306_send_command(0x7F);
    ssd1306_send_command(0xD9); ssd1306_send_command(0xF1);
    ssd1306_send_command(0xDB); ssd1306_send_command(0x40);
    ssd1306_send_command(0xA4);
    ssd1306_send_command(0xA6);
    ssd1306_send_command(0x2E);
    ssd1306_send_command(0xAF);
}

void ssd1306_clear(void)
{
    memset(oled_framebuffer, 0x00, SSD1306_BUFFER_SIZE);
}

static void ssd1306_set_pixel(uint8_t x, uint8_t y)
{
    if (x >= SSD1306_WIDTH || y >= SSD1306_HEIGHT) return;
    uint16_t page = (uint16_t)(y >> 3);
    uint16_t index = (uint16_t)(page * SSD1306_WIDTH + x);
    oled_framebuffer[index] |= (uint8_t)(1U << (y & 7U));
}

void ssd1306_update(void)
{
    ssd1306_send_command(0x21);
    ssd1306_send_command(0x00);
    ssd1306_send_command((uint8_t)(SSD1306_WIDTH - 1U));

    ssd1306_send_command(0x22);
    ssd1306_send_command(0x00);
    ssd1306_send_command((uint8_t)((SSD1306_HEIGHT / 8U) - 1U));

    const uint16_t chunk_payload = 16U;
    uint8_t txbuf[1 + chunk_payload];
    txbuf[0] = 0x40;

    uint32_t i = 0;
    while (i < SSD1306_BUFFER_SIZE) {
        uint16_t remaining = (uint16_t)(SSD1306_BUFFER_SIZE - i);
        uint16_t chunk = (remaining < chunk_payload) ? remaining : chunk_payload;

        for (uint16_t n = 0; n < chunk; ++n) {
            txbuf[1 + n] = oled_framebuffer[i + n];
        }

        if (!i2c_write_checked(SSD1306_I2C_PERIPH, SSD1306_ADDR_7BIT, txbuf, (uint16_t)(1 + chunk))) {
            return;
        }

        i += chunk;
    }
}

/* 5x7 digit font + colon */
static const uint8_t font_digits_5x7[][5] = {
    { 0x3E, 0x51, 0x49, 0x45, 0x3E }, /* 0 */
    { 0x00, 0x42, 0x7F, 0x40, 0x00 }, /* 1 */
    { 0x42, 0x61, 0x51, 0x49, 0x46 }, /* 2 */
    { 0x21, 0x41, 0x45, 0x4B, 0x31 }, /* 3 */
    { 0x18, 0x14, 0x12, 0x7F, 0x10 }, /* 4 */
    { 0x27, 0x45, 0x45, 0x45, 0x39 }, /* 5 */
    { 0x3C, 0x4A, 0x49, 0x49, 0x30 }, /* 6 */
    { 0x01, 0x71, 0x09, 0x05, 0x03 }, /* 7 */
    { 0x36, 0x49, 0x49, 0x49, 0x36 }, /* 8 */
    { 0x06, 0x49, 0x49, 0x29, 0x1E }, /* 9 */
};

static const uint8_t font_colon_5x7[5] = { 0x00, 0x36, 0x36, 0x00, 0x00 };

/* 5x7 font for uppercase letters (A-Z) */
static const uint8_t font_letters_5x7[][5] = {
    { 0x7E, 0x11, 0x11, 0x11, 0x7E }, /* A */
    { 0x7F, 0x49, 0x49, 0x49, 0x36 }, /* B */
    { 0x3E, 0x41, 0x41, 0x41, 0x22 }, /* C */
    { 0x7F, 0x41, 0x41, 0x22, 0x1C }, /* D */
    { 0x7F, 0x49, 0x49, 0x49, 0x41 }, /* E */
    { 0x7F, 0x09, 0x09, 0x09, 0x01 }, /* F */
    { 0x3E, 0x41, 0x49, 0x49, 0x7A }, /* G */
    { 0x7F, 0x08, 0x08, 0x08, 0x7F }, /* H */
    { 0x00, 0x41, 0x7F, 0x41, 0x00 }, /* I */
    { 0x20, 0x40, 0x41, 0x3F, 0x01 }, /* J */
    { 0x7F, 0x08, 0x14, 0x22, 0x41 }, /* K */
    { 0x7F, 0x40, 0x40, 0x40, 0x40 }, /* L */
    { 0x7F, 0x02, 0x0C, 0x02, 0x7F }, /* M */
    { 0x7F, 0x04, 0x08, 0x10, 0x7F }, /* N */
    { 0x3E, 0x41, 0x41, 0x41, 0x3E }, /* O */
    { 0x7F, 0x09, 0x09, 0x09, 0x06 }, /* P */
    { 0x3E, 0x41, 0x51, 0x21, 0x5E }, /* Q */
    { 0x7F, 0x09, 0x19, 0x29, 0x46 }, /* R */
    { 0x46, 0x49, 0x49, 0x49, 0x31 }, /* S */
    { 0x01, 0x01, 0x7F, 0x01, 0x01 }, /* T */
    { 0x3F, 0x40, 0x40, 0x40, 0x3F }, /* U */
    { 0x1F, 0x20, 0x40, 0x20, 0x1F }, /* V */
    { 0x3F, 0x40, 0x38, 0x40, 0x3F }, /* W */
    { 0x63, 0x14, 0x08, 0x14, 0x63 }, /* X */
    { 0x07, 0x08, 0x70, 0x08, 0x07 }, /* Y */
    { 0x61, 0x51, 0x49, 0x45, 0x43 }, /* Z */
};

static void draw_char_scaled(uint8_t x, uint8_t y, char ch, uint8_t scale)
{
    const uint8_t *glyph = NULL;

    if (ch >= '0' && ch <= '9') {
        glyph = font_digits_5x7[(uint8_t)(ch - '0')];
    } else if (ch == ':') {
        glyph = font_colon_5x7;
    } else if (ch >= 'A' && ch <= 'Z') {
        glyph = font_letters_5x7[(uint8_t)(ch - 'A')];
    } else if (ch >= 'a' && ch <= 'z') {
        /* Convert lowercase to uppercase */
        glyph = font_letters_5x7[(uint8_t)(ch - 'a')];
    } else {
        return;
    }

    for (uint8_t col = 0; col < 5; ++col) {
        uint8_t bits = glyph[col];
        for (uint8_t row = 0; row < 7; ++row) {
            if (bits & (1U << row)) {
                for (uint8_t sy = 0; sy < scale; ++sy) {
                    for (uint8_t sx = 0; sx < scale; ++sx) {
                        ssd1306_set_pixel((uint8_t)(x + col * scale + sx),
                                          (uint8_t)(y + row * scale + sy));
                    }
                }
            }
        }
    }
}

static void draw_string_scaled(uint8_t x, uint8_t y, const char *s, uint8_t scale)
{
    while (*s) {
        draw_char_scaled(x, y, *s++, scale);
        x = (uint8_t)(x + (5U * scale) + scale);
    }
}

static const char* get_day_string(uint8_t day)
{
    switch(day) {
        case 1: return "SUN";
        case 2: return "MON";
        case 3: return "TUE";
        case 4: return "WED";
        case 5: return "THU";
        case 6: return "FRI";
        case 7: return "SAT";
        default: return "---";
    }
}

/* Draw hh:mm:ss with day of week on OLED */
void draw_time_with_day_on_oled(uint8_t hour, uint8_t minute, uint8_t second, uint8_t day)
{
    char time_buf[9];
    time_buf[0] = (char)('0' + (hour / 10));
    time_buf[1] = (char)('0' + (hour % 10));
    time_buf[2] = ':';
    time_buf[3] = (char)('0' + (minute / 10));
    time_buf[4] = (char)('0' + (minute % 10));
    time_buf[5] = ':';
    time_buf[6] = (char)('0' + (second / 10));
    time_buf[7] = (char)('0' + (second % 10));
    time_buf[8] = '\0';

    ssd1306_clear();

    /* Draw day of week at top (smaller scale) */
    const char* day_str = get_day_string(day);
    draw_string_scaled(50, 8, day_str, 2);  /* Centered, small text */

    /* Draw time below (larger scale) */
    draw_string_scaled(16, 32, time_buf, 2);  /* Centered, large text */
}

void draw_time_on_oled(uint8_t hour, uint8_t minute, uint8_t second)
{
    char buf[9];
    buf[0] = (char)('0' + (hour / 10));
    buf[1] = (char)('0' + (hour % 10));
    buf[2] = ':';
    buf[3] = (char)('0' + (minute / 10));
    buf[4] = (char)('0' + (minute % 10));
    buf[5] = ':';
    buf[6] = (char)('0' + (second / 10));
    buf[7] = (char)('0' + (second % 10));
    buf[8] = '\0';

    ssd1306_clear();
    draw_string_scaled(16, 24, buf, 2);
}
