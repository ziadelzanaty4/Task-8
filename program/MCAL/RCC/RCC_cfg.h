#ifndef RCC_CFG_H
#define RCC_CFG_H


typedef enum{
    PLL_CFG_TRUE=0,
    SYSCLK_EXCEED_84,
    VCO_OUT_NOT_IN_RANGE,
    VCO_IN_NOT_IN_RANGE,
    N_WRONG_CONF,
    M_WRONG_CONF,
    P_NOT_VALID
}RCC_ERRORS_RETURN;

typedef enum{
    HSI,
    HSE,
    PLL
}RCC_CLOCK_SRC;


typedef enum{
    LOW_SPEED,
    MEDIUM_SPEED,
    HIGH_SPEED,
    MAX_SPEED
}PLL_MODES;


#define PLL_SRC_HSE

#endif