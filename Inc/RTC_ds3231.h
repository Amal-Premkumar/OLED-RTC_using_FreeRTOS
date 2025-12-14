/*
 * RTC_ds3231.h
 *
 *  Created on: Dec 14, 2025
 *      Author: Amal
 */

#ifndef RTC_DS3231_H_
#define RTC_DS3231_H_

#include <stdint.h>
#include "stm32f4xx.h"

#ifndef DS3231_I2C_ADDR_7BIT
#define DS3231_I2C_ADDR_7BIT           (0x68U)
#endif
#ifndef DS3231_ADDR_7BIT
#define DS3231_ADDR_7BIT               DS3231_I2C_ADDR_7BIT
#endif

#ifndef I2C_RTC_PERIPH
#define I2C_RTC_PERIPH I2C1
#endif

typedef struct {
    uint8_t seconds;
    uint8_t minutes;
    uint8_t hours;
    uint8_t day;
    uint8_t date;
    uint8_t month;
    uint8_t year;
} rtc_time_t;

int rtc_read_time(rtc_time_t *t);
int rtc_read_regs(uint8_t start_reg, uint8_t *buf, uint8_t len);
int i2c_read_regs(I2C_TypeDef *I2Cx,
                  uint8_t addr7,
                  uint8_t start_reg,
                  uint8_t *buf,
                  uint16_t len);

#endif /* RTC_DS3231_H_ */
