/*
 * rtc+oled_driver.c
 *
 *  Created on: Dec 14, 2025
 *      Author: Amal
 */


#include "stm32f4xx.h"
#include <stdint.h>


/* I2C1 on PB6/PB7 (AF4) */
#define I2C1_PORT       (GPIOB)
#define I2C1_SCL_PIN    6U
#define I2C1_SDA_PIN    7U
#define I2C1_AF         4U  /* AF4 for I2C on STM32F4 */

/* I2C2 on PB10/PB11 (AF4) */
#define I2C2_PORT       (GPIOB)
#define I2C2_SCL_PIN    10U
#define I2C2_SDA_PIN    11U
#define I2C2_AF         4U

#define GPIO_MODE_INPUT    0U
#define GPIO_MODE_OUTPUT   1U
#define GPIO_MODE_ALTFN    2U
#define GPIO_MODE_ANALOG   3U

/* OTYPER */
#define GPIO_OTYPE_PP      0U
#define GPIO_OTYPE_OD      1U

/* PUPDR mapping for convenience */
#define GPIO_PUPD_NONE     0U
#define GPIO_PUPD_PULLUP   1U
#define GPIO_PUPD_PULLDOWN 2U

/* Speed (00 low, 01 medium, 10 fast, 11 high) */
#define GPIO_SPEED_LOW     0U
#define GPIO_SPEED_MEDIUM  1U
#define GPIO_SPEED_FAST    2U
#define GPIO_SPEED_HIGH    3U

static inline void gpio_enable_clock(GPIO_TypeDef *port)
{
    if (port == GPIOA) { RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; }
    else if (port == GPIOB) { RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN; }
    else if (port == GPIOC) { RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN; }
    else if (port == GPIOD) { RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN; }
    else if (port == GPIOE) { RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN; }
    else if (port == GPIOF) { RCC->AHB1ENR |= RCC_AHB1ENR_GPIOFEN; }
    else if (port == GPIOG) { RCC->AHB1ENR |= RCC_AHB1ENR_GPIOGEN; }
    else if (port == GPIOH) { RCC->AHB1ENR |= RCC_AHB1ENR_GPIOHEN; }
    else if (port == GPIOI) { RCC->AHB1ENR |= RCC_AHB1ENR_GPIOIEN; }

    (void)RCC->AHB1ENR;
}


static inline void set_pin_mode(GPIO_TypeDef *port, uint8_t pin, uint8_t mode)
{
    uint32_t pos = pin * 2U;
    port->MODER &= ~(0x3U << pos);
    port->MODER |= ((uint32_t)(mode & 0x3U) << pos);
}

static inline void set_pin_otype(GPIO_TypeDef *port, uint8_t pin, uint8_t otype)
{
    port->OTYPER &= ~(1U << pin);
    port->OTYPER |= ((uint32_t)(otype & 0x1U) << pin);
}

static inline void set_pin_speed(GPIO_TypeDef *port, uint8_t pin, uint8_t speed)
{
    uint32_t pos = pin * 2U;
    port->OSPEEDR &= ~(0x3U << pos);
    port->OSPEEDR |= ((uint32_t)(speed & 0x3U) << pos);
}

static inline void set_pin_pupd(GPIO_TypeDef *port, uint8_t pin, uint8_t pupd)
{
    uint32_t pos = pin * 2U;
    port->PUPDR &= ~(0x3U << pos);
    port->PUPDR |= ((uint32_t)(pupd & 0x3U) << pos);
}

static inline void set_pin_af(GPIO_TypeDef *port, uint8_t pin, uint8_t af)
{
    uint32_t index = (pin >> 3U); /* 0 => AFR[0], 1 => AFR[1] */
    uint32_t shift = (pin & 0x7U) * 4U;
    port->AFR[index] &= ~(0xFU << shift);
    port->AFR[index] |= ((uint32_t)(af & 0xFU) << shift);
}


void GPIO_ConfigI2C_Pins(GPIO_TypeDef *port,
                         uint8_t sclPin,
                         uint8_t sdaPin,
                         uint8_t altFn,
                         uint8_t pull,
                         uint8_t speed)
{
    if (!port) return;

    gpio_enable_clock(port);

    /* SCL */
    set_pin_mode(port, sclPin, GPIO_MODE_ALTFN);
    set_pin_otype(port, sclPin, GPIO_OTYPE_OD);
    set_pin_speed(port, sclPin, speed);
    set_pin_pupd(port, sclPin, pull);
    set_pin_af(port, sclPin, altFn);

    /* SDA */
    set_pin_mode(port, sdaPin, GPIO_MODE_ALTFN);
    set_pin_otype(port, sdaPin, GPIO_OTYPE_OD);
    set_pin_speed(port, sdaPin, speed);
    set_pin_pupd(port, sdaPin, pull);
    set_pin_af(port, sdaPin, altFn);
}


void gpio_init_i2c1_pins(void)
{
    GPIO_ConfigI2C_Pins(I2C1_PORT, I2C1_SCL_PIN, I2C1_SDA_PIN, I2C1_AF,
                        GPIO_PUPD_NONE, GPIO_SPEED_HIGH);
    RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
    (void)RCC->APB1ENR;
}



static int timeout_count(void)
{
    return 100000;
}

static void delay_us(uint32_t us)
{
    volatile uint32_t count = us * 10;
    while (count--) {
        __asm volatile ("nop");
    }
}



void i2c_init(I2C_TypeDef *I2Cx)
{

    if (I2Cx == I2C1) {
        RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
        (void)RCC->APB1ENR;
    }
    else if (I2Cx == I2C2) {
        RCC->APB1ENR |= RCC_APB1ENR_I2C2EN;
        (void)RCC->APB1ENR;
    }

    i2c_bus_recovery(I2Cx);

    if (I2Cx == I2C1) {
        RCC->APB1RSTR |= RCC_APB1RSTR_I2C1RST;
        delay_us(10);
        RCC->APB1RSTR &= ~RCC_APB1RSTR_I2C1RST;
    }
    delay_us(100);

    I2Cx->CR1 &= ~I2C_CR1_PE;

    I2Cx->CR2 = 42U;

    I2Cx->CCR = 20U;

    I2Cx->TRISE = 43U;

    I2Cx->CR1 |= I2C_CR1_ACK;

    I2Cx->CR1 |= I2C_CR1_PE;

    delay_us(100);
}

int i2c_write(I2C_TypeDef *I2Cx, uint8_t addr7, const uint8_t *buf, uint16_t len)
{
    if (!I2Cx || (!buf && len)) return 0;
    if (len == 0U) return 1;

    int t;

    t = timeout_count();
    while ((I2Cx->SR2 & I2C_SR2_BUSY) && --t) {}
    if (t == 0) {
        i2c_bus_recovery(I2Cx);
        return 0;
    }

    /* Generate START */
    I2Cx->CR1 |= I2C_CR1_START;

    /* Wait for SB (start bit) */
    t = timeout_count();
    while (((I2Cx->SR1 & I2C_SR1_SB) == 0U) && --t) {}
    if (t == 0) {
        I2Cx->CR1 |= I2C_CR1_STOP;
        return 0;
    }

    /* Send address (write: R/W = 0) */
    I2Cx->DR = (uint8_t)(addr7 << 1U);

    /* Wait for ADDR set */
    t = timeout_count();
    while (((I2Cx->SR1 & I2C_SR1_ADDR) == 0U) && --t) {
        /* Check for NACK */
        if (I2Cx->SR1 & I2C_SR1_AF) {
            I2Cx->SR1 &= ~I2C_SR1_AF;
            I2Cx->CR1 |= I2C_CR1_STOP;
            return 0;
        }
    }
    if (t == 0) {
        I2Cx->CR1 |= I2C_CR1_STOP;
        return 0;
    }

    /* Clear ADDR by reading SR1 followed by SR2 */
    (void)I2Cx->SR1;
    (void)I2Cx->SR2;

    /* Write bytes */
    for (uint16_t i = 0; i < len; ++i) {
        /* Wait for TXE */
        t = timeout_count();
        while (((I2Cx->SR1 & I2C_SR1_TXE) == 0U) && --t) {}
        if (t == 0) {
            I2Cx->CR1 |= I2C_CR1_STOP;
            return 0;
        }

        I2Cx->DR = buf[i];
    }

    /* Wait until transfer finished (BTF) */
    t = timeout_count();
    while (((I2Cx->SR1 & I2C_SR1_BTF) == 0U) && --t) {}

    /* Send STOP */
    I2Cx->CR1 |= I2C_CR1_STOP;

    delay_us(10);

    return (t == 0) ? 0 : 1;
}

uint8_t i2c_read_reg1(I2C_TypeDef *I2Cx, uint8_t addr7, uint8_t reg)
{
    if (!I2Cx) return 0xFFU;

    int t;

    /* Wait until bus is not busy */
    t = timeout_count();
    while ((I2Cx->SR2 & I2C_SR2_BUSY) && --t) {}
    if (t == 0) {
        i2c_bus_recovery(I2Cx);
        return 0xFFU;
    }

    /* --- Write register address --- */
    I2Cx->CR1 |= I2C_CR1_START;
    t = timeout_count();
    while (((I2Cx->SR1 & I2C_SR1_SB) == 0U) && --t) {}
    if (t == 0) return 0xFFU;

    I2Cx->DR = (uint8_t)(addr7 << 1U);
    t = timeout_count();
    while (((I2Cx->SR1 & I2C_SR1_ADDR) == 0U) && --t) {}
    if (t == 0) {
        I2Cx->CR1 |= I2C_CR1_STOP;
        return 0xFFU;
    }

    /* Clear ADDR */
    (void)I2Cx->SR1;
    (void)I2Cx->SR2;

    /* Wait TXE */
    t = timeout_count();
    while (((I2Cx->SR1 & I2C_SR1_TXE) == 0U) && --t) {}
    if (t == 0) {
        I2Cx->CR1 |= I2C_CR1_STOP;
        return 0xFFU;
    }

    /* Send register index */
    I2Cx->DR = reg;

    /* Wait for BTF */
    t = timeout_count();
    while (((I2Cx->SR1 & I2C_SR1_BTF) == 0U) && --t) {}
    if (t == 0) {
        I2Cx->CR1 |= I2C_CR1_STOP;
        return 0xFFU;
    }

    /* --- Repeated START for read --- */
    I2Cx->CR1 |= I2C_CR1_START;
    t = timeout_count();
    while (((I2Cx->SR1 & I2C_SR1_SB) == 0U) && --t) {}
    if (t == 0) {
        I2Cx->CR1 |= I2C_CR1_STOP;
        return 0xFFU;
    }

    I2Cx->DR = (uint8_t)((addr7 << 1U) | 0x01U);

    /* Wait for ADDR */
    t = timeout_count();
    while (((I2Cx->SR1 & I2C_SR1_ADDR) == 0U) && --t) {}
    if (t == 0) {
        I2Cx->CR1 |= I2C_CR1_STOP;
        return 0xFFU;
    }

    I2Cx->CR1 &= ~I2C_CR1_ACK;

    /* Clear ADDR */
    (void)I2Cx->SR1;
    (void)I2Cx->SR2;

    /* Generate STOP */
    I2Cx->CR1 |= I2C_CR1_STOP;

    /* Wait until RXNE */
    t = timeout_count();
    while (((I2Cx->SR1 & I2C_SR1_RXNE) == 0U) && --t) {}
    if (t == 0) return 0xFFU;

    uint8_t val = (uint8_t)I2Cx->DR;

    /* Re-enable ACK */
    I2Cx->CR1 |= I2C_CR1_ACK;

    delay_us(10);

    return val;
}

void i2c_write_reg1(I2C_TypeDef *I2Cx, uint8_t device_addr, uint8_t reg_addr, uint8_t data)
{
    uint32_t timeout;

    // Wait for bus free
    timeout = 100000;
    while ((I2Cx->SR2 & I2C_SR2_BUSY) && --timeout) {}
    if (timeout == 0) return;

    // Generate START
    I2Cx->CR1 |= I2C_CR1_START;
    timeout = 100000;
    while (!(I2Cx->SR1 & I2C_SR1_SB) && --timeout) {}
    if (timeout == 0) return;

    // Send device address (write mode)
    I2Cx->DR = (device_addr << 1);  // LSB = 0 for write
    timeout = 100000;
    while (!(I2Cx->SR1 & I2C_SR1_ADDR) && --timeout) {}
    if (timeout == 0) {
        I2Cx->CR1 |= I2C_CR1_STOP;
        return;
    }

    // Clear ADDR flag by reading SR1 then SR2
    (void)I2Cx->SR1;
    (void)I2Cx->SR2;

    // Send register address
    I2Cx->DR = reg_addr;
    timeout = 100000;
    while (!(I2Cx->SR1 & I2C_SR1_TXE) && --timeout) {}
    if (timeout == 0) {
        I2Cx->CR1 |= I2C_CR1_STOP;
        return;
    }

    // Send data byte
    I2Cx->DR = data;
    timeout = 100000;
    while (!(I2Cx->SR1 & I2C_SR1_TXE) && --timeout) {}
    if (timeout == 0) {
        I2Cx->CR1 |= I2C_CR1_STOP;
        return;
    }

    // Wait for BTF (Byte Transfer Finished)
    timeout = 100000;
    while (!(I2Cx->SR1 & I2C_SR1_BTF) && --timeout) {}

    // Generate STOP
    I2Cx->CR1 |= I2C_CR1_STOP;
}

void i2c_bus_recovery(I2C_TypeDef *I2Cx)
{
   /* Disable peripheral */
   I2Cx->CR1 &= ~I2C_CR1_PE;

   delay_us(100);

   I2Cx->CR1 |= I2C_CR1_SWRST;
   delay_us(100);
   I2Cx->CR1 &= ~I2C_CR1_SWRST;

   delay_us(100);
}

int i2c_read_regs(I2C_TypeDef *I2Cx,
                  uint8_t addr7,
                  uint8_t start_reg,
                  uint8_t *buf,
                  uint16_t len)
{
    if (!I2Cx || !buf || len == 0) return -1;

    int t;

    /* Wait until bus free */
    t = 100000;
    while ((I2Cx->SR2 & I2C_SR2_BUSY) && --t) {}
    if (t == 0) return -1;

    /* --- Write register address --- */
    I2Cx->CR1 |= I2C_CR1_START;
    t = 100000;
    while (!(I2Cx->SR1 & I2C_SR1_SB) && --t) {}
    if (t == 0) return -1;

    I2Cx->DR = (addr7 << 1);   // write
    t = 100000;
    while (!(I2Cx->SR1 & I2C_SR1_ADDR) && --t) {}
    if (t == 0) goto stop_fail;

    (void)I2Cx->SR1;
    (void)I2Cx->SR2;

    t = 100000;
    while (!(I2Cx->SR1 & I2C_SR1_TXE) && --t) {}
    if (t == 0) goto stop_fail;

    I2Cx->DR = start_reg;

    t = 100000;
    while (!(I2Cx->SR1 & I2C_SR1_BTF) && --t) {}
    if (t == 0) goto stop_fail;

    /* --- Repeated START for READ --- */
    I2Cx->CR1 |= I2C_CR1_START;
    t = 100000;
    while (!(I2Cx->SR1 & I2C_SR1_SB) && --t) {}
    if (t == 0) goto stop_fail;

    I2Cx->DR = (addr7 << 1) | 1;  // read
    t = 100000;
    while (!(I2Cx->SR1 & I2C_SR1_ADDR) && --t) {}
    if (t == 0) goto stop_fail;

    if (len == 1) {
        I2Cx->CR1 &= ~I2C_CR1_ACK;
        (void)I2Cx->SR1;
        (void)I2Cx->SR2;
        I2Cx->CR1 |= I2C_CR1_STOP;
        t = 100000;
        while (!(I2Cx->SR1 & I2C_SR1_RXNE) && --t) {}
        buf[0] = I2Cx->DR;
    } else {
        I2Cx->CR1 |= I2C_CR1_ACK;
        (void)I2Cx->SR1;
        (void)I2Cx->SR2;

        for (uint16_t i = 0; i < len; i++) {
            if (i == len - 2) {
                I2Cx->CR1 &= ~I2C_CR1_ACK;
            }
            t = 100000;
            while (!(I2Cx->SR1 & I2C_SR1_RXNE) && --t) {}
            buf[i] = I2Cx->DR;
        }
        I2Cx->CR1 |= I2C_CR1_STOP;
    }

    I2Cx->CR1 |= I2C_CR1_ACK;
    return 0;

stop_fail:
    I2Cx->CR1 |= I2C_CR1_STOP;
    I2Cx->CR1 |= I2C_CR1_ACK;
    return -1;
}

