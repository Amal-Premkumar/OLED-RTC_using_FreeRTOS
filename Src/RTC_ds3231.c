/*
 * RTC_ds3231.c
 *
 *  Created on: Dec 14, 2025
 *      Author: Amal
 */


#include "RTC_ds3231.h"
#include "stm32f4xx.h"
#include <stddef.h>
#include "system_coreclock.h"


#ifndef I2C_READ_REG1_DECLARED
extern uint8_t i2c_read_reg1(I2C_TypeDef *i2c, uint8_t addr7, uint8_t reg);
#endif
static uint8_t bcd_to_bin(uint8_t x)
{
    return (uint8_t)(((x >> 4U) * 10U) + (x & 0x0FU));
}

static uint8_t bin_to_bcd(uint8_t x)
{
    return (uint8_t)(((x / 10U) << 4U) | (x % 10U));
}

void rtc_set_time(uint8_t hour,
                  uint8_t minute,
                  uint8_t second,
                  uint8_t day)
{
    i2c_write_reg1(I2C_RTC_PERIPH, DS3231_I2C_ADDR_7BIT, 0x00, bin_to_bcd(second));
    i2c_write_reg1(I2C_RTC_PERIPH, DS3231_I2C_ADDR_7BIT, 0x01, bin_to_bcd(minute));
    i2c_write_reg1(I2C_RTC_PERIPH, DS3231_I2C_ADDR_7BIT, 0x02, bin_to_bcd(hour));
    i2c_write_reg1(I2C_RTC_PERIPH, DS3231_I2C_ADDR_7BIT, 0x03, day); // 1=SUN
}

int rtc_read_time(rtc_time_t *t)
{
    if (!t) return -1;

    uint8_t raw[7];

    if (i2c_read_regs(I2C_RTC_PERIPH,
                      DS3231_I2C_ADDR_7BIT,
                      0x00,
                      raw,
                      7) != 0) {
        return -1;
    }

    t->seconds = bcd_to_bin(raw[0] & 0x7F);
    t->minutes = bcd_to_bin(raw[1] & 0x7F);

    uint8_t hour = raw[2];
    if (hour & 0x40) {
        uint8_t h12 = bcd_to_bin(hour & 0x1F);
        t->hours = (hour & 0x20) ?
                   ((h12 == 12) ? 12 : h12 + 12) :
                   ((h12 == 12) ? 0  : h12);
    } else {
        t->hours = bcd_to_bin(hour & 0x3F);
    }

    t->day   = bcd_to_bin(raw[3] & 0x07);
    t->date  = bcd_to_bin(raw[4] & 0x3F);
    t->month = bcd_to_bin(raw[5] & 0x1F);
    t->year  = bcd_to_bin(raw[6]);

    return 0;
}


int rtc_read_regs(uint8_t start_reg, uint8_t *buf, uint8_t len)
{
    if (!buf || len == 0) return -1;

    /* Write start register */
    if (!i2c_write(I2C_RTC_PERIPH,
                   DS3231_I2C_ADDR_7BIT,
                   &start_reg,
                   1)) {
        return -1;
    }

    /* Read data bytes */
    if (!i2c_read_regs(I2C_RTC_PERIPH,
            DS3231_I2C_ADDR_7BIT,
            0x00,   // start register
            buf,
            len));
 {
        return -1;
    }

    return 0;
}
