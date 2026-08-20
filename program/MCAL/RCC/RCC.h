#ifndef RCC_H
#define RCC_H


#include "../../LIB/STD_TYPES.h"
#include "../../LIB/BIT_MATH.h"

#include "../../LIB/STM32F401xC_HEADER.h"
#include "../../LIB/STM32F401xC_OPTIONS.h"

#include "RCC_CFG.h"


/*===========================================================
 * System clock
 *===========================================================*/

extern volatile u32 MCAL_u32_SystemClock;


/*===========================================================
 * Clock source
 *===========================================================*/

RCC_ERRORS_RETURN MCAL_void_RCC_CLK_SRC(RCC_CLOCK_SRC clock_src);

void MCAL_void_RCC_CLK_HSE_ENB(void);

void MCAL_void_RCC_CLK_HSI_ENB(void);

void MCAL_void_RCC_CLK_PLL_ENB(void);

void MCAL_void_RCC_Init(void);


/*===========================================================
 * PLL
 *===========================================================*/

void MCAL_void_PLL_CLK_CONF_SRC(RCC_CLOCK_SRC pll_option);

RCC_ERRORS_RETURN MCAL_void_PLL_CLK_CONF_mul(
    u32 P,
    u8  M,
    u32 N
);

RCC_ERRORS_RETURN MCAL_void_PLL_CLK_mode(
    PLL_MODES speed_mode
);


/*===========================================================
 * Prescalers
 *===========================================================*/

void MCAL_VOID_RCC_AHB_PRESCALE(u16 prescaler);

void MCAL_VOID_RCC_APB1_PRESCALE(u8 prescaler);

void MCAL_VOID_RCC_APB2_PRESCALE(u8 prescaler);


/*===========================================================
 * AHB clock enable
 *===========================================================*/

void MCAL_void_RCC_CLK_ENB_AHB1(void);

void MCAL_void_RCC_CLK_ENB_AHB2(void);

void MCAL_void_RCC_CLK_ENB_DMA(void);

void MCAL_void_RCC_CLK_ENB_GPIO_all(void);

void MCAL_void_RCC_CLK_ENB_GPIO(GPIO_ENUM GPIO_SELECT);


/*===========================================================
 * APB1 / APB2 clock enable
 *===========================================================*/

void MCAL_void_RCC_CLK_ENB_APB1(void);

void MCAL_void_RCC_CLK_ENB_APB2(void);

void MCAL_void_RCC_CLK_ENB_USART_all(void);

void MCAL_void_RCC_CLK_ENB_USART(USART_ENUM USART_SELECT);

void MCAL_void_RCC_CLK_ENB_I2C_all(void);

void MCAL_void_RCC_CLK_ENB_I2C(I2C_ENUM I2C_SELECT);

void MCAL_void_RCC_CLK_ENB_SPI_all(void);

void MCAL_void_RCC_CLK_ENB_SPI(SPI_ENUM SPI_SELECT);

void MCAL_void_RCC_CLK_ENB_TIM(TIMER_ENUM TIM_SELECT);


/*===========================================================
 * Default initialization
 *===========================================================*/

RCC_ERRORS_RETURN MCAL_VOID_RCC_init_default(void);


#endif /* RCC_H */