#ifndef STM32F401xC_AF_CFG_H
#define STM32F401xC_AF_CFG_H
#include "STM32F401xC_OPTIONS.h"
#include "STM32F401xC_HEADER.h"
#include "../MCAL/GPIO/GPIO.h"

// #define USART_MODULE_ENABLED
// #define TIM_MODULE_ENABLED
typedef struct 
{
    GPIO_ENUM port;
    u8 pin;
    void* peripheral;
    AF_ENUM AF_OPTION; 
}pinMap_struct;

#ifdef USART_MODULE_ENABLED
//remaining here is rts and cts
const pinMap_struct pinMap_USART_TX[]{
    pinMap_struct{GPIOB,6,USART1_TYPEDEF,AF7},
    pinMap_struct{GPIOA,2,USART2_TYPDEF,AF7},
    pinMap_struct{GPIOA,11,USART6_TYPEDEF,AF8},
    pinMap_struct{NOOPTION_GPIO,-1,NULL,NOOPTION_AF}
}

const pinMap_struct pinMap_USART_RX[]{
    pinMap_struct{GPIOB,7,USART1_TYPEDEF,AF7},
    pinMap_struct{GPIOA,3,USART2_TYPDEF,AF7},
    pinMap_struct{GPIOA,12,USART6_TYPEDEF,AF8},
    pinMap_struct{NOOPTION_GPIO,-1,NULL,NOOPTION_AF}
}
#else
const pinMap_struct pinMap_USART_TX[]{
    pinMap_struct{NOOPTION_GPIO,-1,NULL,NOOPTION_AF}
};
#endif
#ifdef TIM_MODULE_ENABLED
const pinMap_struct  pinMap_TIMER[]{
    pinMap_struct{GPIOA,8,TIM1_TYPEDEF,AF1},  //TIM1 CH1
    pinMap_struct{GPIOA,9,TIM1_TYPEDEF,AF1}, //TIM1 CH2
    pinMap_struct{GPIOA,10,TIM1_TYPEDEF,AF1},  //TIM1 CH3
    pinMap_struct{GPIOA,11,TIM1_TYPEDEF,AF1}, //TIM1 CH4
    pinMap_struct(GPIOB,13,TIM1_TYPEDEF,AF1), //TIM1 CH1N
    pinMap_struct(GPIOB,14,TIM1_TYPEDEF,AF1), //TIM1 CH2N
    pinMap_struct(GPIOB,15,TIM1_TYPEDEF,AF1), //TIM1 CH3N
    pinMap_struct(GPIOB,12,TIM1_TYPEDEF,AF1), //TIM1 CH4N
    
}
#else
const pinMap_struct pinMap_TIMER[]{
    pinMap_struct{NOOPTION_GPIO,-1,NULL,NOOPTION_AF}
};
#endif



u8 STM_AF_CFG_checK(pinMap_struct pinmap , pinMap_struct* pinmap_array){
    u8 i=0;
    u8 flag=0;
    while (pinmap_array[i].port!=NOOPTION_GPIO)
    {
        if ((pinmap_array[i].port==pinmap.port)&&(pinmap_array[i].pin==pinmap.pin)&&(pinmap_array[i].peripheral==pinmap.peripheral)&&(pinmap_array[i].AF_OPTION==pinmap.AF_OPTION)) flag=1;
        i++;      
    }
    return flag;
    
}

#endif