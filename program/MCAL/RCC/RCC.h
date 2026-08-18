#ifndef RCC_H
#define RCC_H
#include "RCC_prv.h"
#include "RCC_cfg.h"
#include "../../LIB/STD_TYPES.h"
#include "../../LIB/BIT_MATH.h"
#include "../../LIB/STM32F401xC_HEADER.h" 
#include "../../LIB/STM32F401xC_OPTIONS.h" 


void MCAL_void_RCC_CLK_SRC(RCC_CLOCK_SRC  clock_src);
RCC_ERRORS_RETURN MCAL_void_PLL_CLK_mode(PLL_MODES speed_mode);

void MCAL_void_RCC_CLK_ENB_AHB1();
void MCAL_void_RCC_CLK_ENB_AHB2();
void MCAL_void_RCC_CLK_ENB_APB1();
void MCAL_void_RCC_CLK_ENB_APB2();

void MCAL_void_RCC_CLK_ENB_GPIO_all();
void MCAL_void_RCC_CLK_ENB_USART_all();
void MCAL_void_RCC_CLK_ENB_I2C_all();

void MCAL_void_RCC_CLK_ENB_DMA();


void MCAL_void_RCC_CLK_ENB_GPIO(GPIO_ENUM GPIO_SELECT);
void MCAL_void_RCC_CLK_ENB_TIM(TIMER_ENUM TIM_SELECT);
void MCAL_void_RCC_CLK_ENB_USART(USART_ENUM USART_SELECT);
void MCAL_void_RCC_CLK_ENB_I2C(I2C_ENUM I2C_SELECT);
void MCAL_void_RCC_CLK_ENB_SPI(SPI_ENUM SPI_SELECT);

void MCAL_VOID_RCC_init_default();

void MCAL_VOID_RCC_AHB_PRESCALE(u8 prescaler);
void MCAL_VOID_RCC_APB1_PRESCALE(u8 prescaler);
void MCAL_VOID_RCC_APB2_PRESCALE(u8 prescaler);


#endif