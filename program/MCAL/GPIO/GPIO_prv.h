#ifndef GPIO_PRV_H
#define GPIO_PRV_H

#include "../../lib/STD_TYPES.h"
#include "../../LIB/STM32F401xC_HEADER.h"


typedef struct RCC_regs_struct
{
    u32 MODER;
    u32 OTYPER;
    u32 OSPEEDR;
    u32 PUPDR;
    u32 IDR;
    u32 ODR;
    u32 BSSR;
    u32 LCKR;
    u32 AFRL;
    u32 AFRH;
}GPIO_TYPEDEF;


#define GPIOA_STRUCT                 ((GPIO_TYPEDEF*)(GPIOA_BASE_ADDR))
#define GPIOB_STRUCT                 ((GPIO_TYPEDEF*)(GPIOB_BASE_ADDR))
#define GPIOC_STRUCT                 ((GPIO_TYPEDEF*)(GPIOC_BASE_ADDR))
#define GPIOD_STRUCT                 ((GPIO_TYPEDEF*)(GPIOD_BASE_ADDR))
#define GPIOE_STRUCT                 ((GPIO_TYPEDEF*)(GPIOE_BASE_ADDR))
#define GPIOH_STRUCT                 ((GPIO_TYPEDEF*)(GPIOH_BASE_ADDR))


#endif