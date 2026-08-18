#ifndef STM32_F401XC_OPTIONS_H
#define STM32_F401XC_OPTIONS_H

typedef enum{
    TIM1,
    TIM2,
    TIM3,
    TIM4,
    TIM5,
    TIM9,
    TIM10,
    TIM11
}TIMER_ENUM;



typedef enum{
    USART1,
    USART2,
    USART6
}USART_ENUM;

typedef enum{
    I2C1,
    I2C2,
    I2C3,
}I2C_ENUM;


typedef enum{
    SPI1,
    SPI2,
    SPI3,
    SPI4,
}SPI_ENUM;


typedef enum{
    NOOPTION_GPIO = -1,
    GPIOA,
    GPIOB,
    GPIOC,
    GPIOD,
    GPIOE,
    GPIOH
}GPIO_ENUM;

typedef enum{
    NOOPTION_AF = -1,
    AF0,
    AF1,
    AF2,
    AF3,
    AF4,
    AF5,
    AF6,
    AF7,
    AF8, 
    
}AF_ENUM;


#endif
