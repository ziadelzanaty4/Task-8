#ifndef GPIO_H
#define GPIO_H

#include "../../LIB/BIT_MATH.h"

#define RCC_AHB1ENR (*(volatile unsigned int *)0x40023830U)
#define GPIOA_MODER (*(volatile unsigned int *)0x40020000U)
#define GPIOA_IDR   (*(volatile unsigned int *)0x40020010U)
#define GPIOA_BSRR  (*(volatile unsigned int *)0x40020018U)

#define GPIOAEN 0

void gpio_init_output(unsigned int pin) {
    SET_BIT(RCC_AHB1ENR, GPIOAEN);
    unsigned int shift = pin * 2;
    GPIOA_MODER &= ~(0x3U << shift);
    GPIOA_MODER |= (0x1U << shift);
}

void gpio_init_input(unsigned int pin) {
    SET_BIT(RCC_AHB1ENR, GPIOAEN);
    unsigned int shift = pin * 2;
    GPIOA_MODER &= ~(0x3U << shift);
}

void gpio_write(unsigned int pin, unsigned int state) {
    if (state) {
        GPIOA_BSRR = (1U << pin);
    } else {
        GPIOA_BSRR = (1U << (pin + 16));
    }
}

unsigned int gpio_read(unsigned int pin) {
    return GET_BIT(GPIOA_IDR, pin);
}

#endif
