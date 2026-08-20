#ifndef RCC_CFG_H
#define RCC_CFG_H

#include "../../lib/STD_TYPES.h"


/*===========================================================
 * Clock source
 *===========================================================*/

typedef enum
{
    HSI = 0,
    HSE,
    PLL

} RCC_CLOCK_SRC;


/*===========================================================
 * PLL speed modes
 *===========================================================*/

typedef enum
{
    LOW_SPEED = 0,      /* 24 MHz  */
    MEDIUM_SPEED,       /* 48 MHz  */
    HIGH_SPEED,         /* 64 MHz  */
    MAX_SPEED           /* 84 MHz  */

} PLL_MODES;


/*===========================================================
 * PLL configuration error codes
 *===========================================================*/

typedef enum
{
    PLL_CFG_TRUE = 0,

    SYSCLK_EXCEED_84,
    VCO_OUT_NOT_IN_RANGE,
    VCO_IN_NOT_IN_RANGE,
    N_WRONG_CONF,
    M_WRONG_CONF,
    P_NOT_VALID

} RCC_ERRORS_RETURN;


/*===========================================================
 * Hardware clock values
 *===========================================================*/

#define RCC_HSI_VALUE       16000000UL

/*
 * Change this if your external crystal/clock is different.
 *
 * Example:
 *      #define RCC_HSE_VALUE 8000000UL
 */
#define RCC_HSE_VALUE       25000000UL


/*===========================================================
 * Select PLL source
 *
 * Define ONLY ONE of these.
 *===========================================================*/

#define PLL_SRC_HSE


/*===========================================================
 * Maximum system clock
 *===========================================================*/

#define RCC_MAX_SYSCLK      84000000UL




#endif /* RCC_CFG_H */