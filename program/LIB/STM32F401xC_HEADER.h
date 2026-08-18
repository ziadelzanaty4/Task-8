#ifndef STM32F401xC_HEADER
#define STM32F401xC_HEADER

#include "STD_TYPES.h"





#define GPIOA_BASE_ADDR     *(volatile u32*)(0x40020000U)
#define GPIOB_BASE_ADDR     *(volatile u32*)(0x40020400U)
#define GPIOC_BASE_ADDR     *(volatile u32*)(0x40020800U)
#define GPIOD_BASE_ADDR     *(volatile u32*)(0x40020C00U)
#define GPIOE_BASE_ADDR     *(volatile u32*)(0x40021000U)
#define GPIOH_BASE_ADDR     *(volatile u32*)(0x40021C00U)

#define RCC_BASE_ADDR       *(volatile u32*)(0X40023800U)





#endif