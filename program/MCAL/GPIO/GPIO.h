#ifndef GPIO_HEADER
#define GPIO_HEADER

#include "GPIO_prv.h"
#include "GPIO_cfg.h"
#include "../../LIB/STD_TYPES.h"
#include "../../LIB/BIT_MATH.h"
#include "../../LIB/STM32F401xC_HEADER.h" 
#include "../../LIB/STM32F401xC_OPTIONS.h" 
#include "../../LIB/PINS.h"

typedef struct 
{
    GPIO_ENUM port;
    u8 pin;
    u8 mode;
}GPIO_CONFIG;



void pinMode(u8 pin,u8 mode);
void digitalWrite(u8 pin , u8 output);
u8 digitalRead(u8 pin);





void GPIO_init(GPIO_CONFIG CONFIG);

void GPIO_void_SELECT_MODE(GPIO_ENUM GPIO_OPTION,u8 pin,u8 mode);

void GPIO_void_SELECT_OP_MODE(GPIO_ENUM GPIO_OPTION,u8 pin,u8 mode);

void GPIO_void_SELECT_SPEED_MODE(GPIO_ENUM GPIO_OPTION,u8 pin,u8 mode);


void GPIO_void_SELECT_PULL_MODE(GPIO_ENUM GPIO_OPTION,u8 pin,u8 mode);

u8 GPIO_void_digital_Read(GPIO_ENUM GPIO_OPTION,u8 pin);

void GPIO_void_digital_Write(GPIO_ENUM GPIO_OPTION,u8 pin,u8 mode);

void GPIO_void_digital_Write_ATOMIC(GPIO_ENUM GPIO_OPTION,u8 pin,u8 mode);

void GPIO_void_set_AF(GPIO_ENUM GPIO_OPTION,u8 pin, AF_ENUM AF_OPTION);

void GPIO_void_enable_AF(GPIO_ENUM GPIO_OPTION,u8 pin, AF_ENUM AF_OPTION);



#endif