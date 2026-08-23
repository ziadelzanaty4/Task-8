#ifndef GPIO_H
#define GPIO_H

#include "../../LIB/BIT_MATH.h"
#include "../RCC/RCC.h"

// ============
//   Pin Map
// ============
// PA3 -> LED RED     (output)
// PA4 -> LED GREEN   (output)
// PA5 -> LED BLUE    (output)
// PA6 -> Switch 1    (input)
// PA7 -> Switch 2    (input)
// PA8 -> Switch 3    (input)
// _____________________________________________


#define GPIOA_BASE_ADDR      0x40020000

#define GPIO_MODER_OFFSET    0x00
#define GPIO_OTYPER_OFFSET   0x04
#define GPIO_OSPEEDR_OFFSET  0x08
#define GPIO_PUPDR_OFFSET    0x0C
#define GPIO_IDR_OFFSET      0x10
#define GPIO_ODR_OFFSET      0x14
#define GPIO_BSRR_OFFSET     0x18

// gpioa address = base address + offset
#define GPIOA_MODER   (*(volatile unsigned int*)(GPIOA_BASE_ADDR + GPIO_MODER_OFFSET))
#define GPIOA_IDR     (*(volatile unsigned int*)(GPIOA_BASE_ADDR + GPIO_IDR_OFFSET))
#define GPIOA_ODR     (*(volatile unsigned int*)(GPIOA_BASE_ADDR + GPIO_ODR_OFFSET))
#define GPIOA_BSRR    (*(volatile unsigned int*)(GPIOA_BASE_ADDR + GPIO_BSRR_OFFSET))
#define GPIOA_MODER   (*(volatile unsigned int*)(GPIOA_BASE_ADDR + GPIO_MODER_OFFSET))
#define GPIOA_OTYPER  (*(volatile unsigned int*)(GPIOA_BASE_ADDR + GPIO_OTYPER_OFFSET))
#define GPIOA_OSPEEDR (*(volatile unsigned int*)(GPIOA_BASE_ADDR + GPIO_OSPEEDR_OFFSET))
#define GPIOA_PUPDR   (*(volatile unsigned int*)(GPIOA_BASE_ADDR + GPIO_PUPDR_OFFSET))
#define GPIOA_IDR     (*(volatile unsigned int*)(GPIOA_BASE_ADDR + GPIO_IDR_OFFSET))
#define GPIOA_ODR     (*(volatile unsigned int*)(GPIOA_BASE_ADDR + GPIO_ODR_OFFSET))
#define GPIOA_BSRR    (*(volatile unsigned int*)(GPIOA_BASE_ADDR + GPIO_BSRR_OFFSET))


// Register   Bits_per_pin   Pin_N's_bits
// MODER	      2	          N×2, N×2+1
// OTYPER	      1	          N
// OSPEEDR	      2	          N×2, N×2+1
// PUPDR	      2	          N×2, N×2+1


// PA3: lower bit = 3*2 = 6, upper bit = 7
// output mode = 01
void GPIO_InitOutput_PA3(void){
    SET_BIT(GPIOA_MODER, 6);       // mode = output
    CLEAR_BIT(GPIOA_MODER, 7);

    CLEAR_BIT(GPIOA_OTYPER, 3);    // push-pull

    CLEAR_BIT(GPIOA_OSPEEDR, 6);   // low speed
    CLEAR_BIT(GPIOA_OSPEEDR, 7);

    CLEAR_BIT(GPIOA_PUPDR, 6);     // Output, no pull-up/down needed
    CLEAR_BIT(GPIOA_PUPDR, 7);
}

void GPIO_InitOutput_PA4(void){
    SET_BIT(GPIOA_MODER, 8);
    CLEAR_BIT(GPIOA_MODER, 9);

    CLEAR_BIT(GPIOA_OTYPER, 4);

    CLEAR_BIT(GPIOA_OSPEEDR, 8);
    CLEAR_BIT(GPIOA_OSPEEDR, 9);

    CLEAR_BIT(GPIOA_PUPDR, 8);
    CLEAR_BIT(GPIOA_PUPDR, 9);
}

void GPIO_InitOutput_PA5(void){
    SET_BIT(GPIOA_MODER, 10);
    CLEAR_BIT(GPIOA_MODER, 11);

    CLEAR_BIT(GPIOA_OTYPER, 5);

    CLEAR_BIT(GPIOA_OSPEEDR, 10);
    CLEAR_BIT(GPIOA_OSPEEDR, 11);

    CLEAR_BIT(GPIOA_PUPDR, 10);
    CLEAR_BIT(GPIOA_PUPDR, 11);
}

// PA6
// input mode = 00, so clear both bits
void GPIO_InitInput_PA6(void){
    CLEAR_BIT(GPIOA_MODER, 12);    // mode = input
    CLEAR_BIT(GPIOA_MODER, 13);

    CLEAR_BIT(GPIOA_PUPDR, 12);    // pull-down: 10 = bit12=0, bit13=1
    SET_BIT(GPIOA_PUPDR, 13);
}

void GPIO_InitInput_PA7(void){
    CLEAR_BIT(GPIOA_MODER, 14);
    CLEAR_BIT(GPIOA_MODER, 15);

    CLEAR_BIT(GPIOA_PUPDR, 14);
    SET_BIT(GPIOA_PUPDR, 15);
}

void GPIO_InitInput_PA8(void){
    CLEAR_BIT(GPIOA_MODER, 16);
    CLEAR_BIT(GPIOA_MODER, 17);

    CLEAR_BIT(GPIOA_PUPDR, 16);
    SET_BIT(GPIOA_PUPDR, 17);
}

// ===========================
//   BSRR write (LED on/off)
// ===========================
// bits 0-15 of BSRR = SET that pin HIGH
// bits 16-31 of BSRR = CLEAR that pin LOW (bit N+16 clears pin N)
// writing a 0 anywhere in BSRR does nothing, only 1s take effect
void GPIO_Write(unsigned int pin, unsigned int state){
    if (state) {
        SET_BIT(GPIOA_BSRR, pin);          // pin HIGH
    } else {
        SET_BIT(GPIOA_BSRR, (pin + 16));     // pin LOW
    }
}

// ===========================
//   IDR read (switch state)
// ===========================
// each bit of IDR directly reflects that pin's current voltage: 1=HIGH, 0=LOW
unsigned int GPIO_Read(unsigned int pin){
    return GET_BIT(GPIOA_IDR, pin);
}

#endif