#include "RCC.h"
#include "RCC_PRV.h"


/*===========================================================
 * Global system clock
 *===========================================================*/

volatile u32 MCAL_u32_SystemClock = RCC_HSI_VALUE;


/*===========================================================
 * FLASH configuration
 *===========================================================*/

/*
 * STM32F401:
 *
 * 0 WS : up to 30 MHz
 * 1 WS : 30 - 60 MHz
 * 2 WS : 60 - 84 MHz
 *
 * At 84 MHz and VDD = 2.7 - 3.6 V,
 * 2 wait states are required.
 */

static void MCAL_void_FLASH_SetLatency(u8 latency)
{
    FLASH_ACR =
        (FLASH_ACR & ~FLASH_ACR_LATENCY_MASK) |
        ((u32)latency & FLASH_ACR_LATENCY_MASK);

    /*
     * Prefetch enable
     */
    SET_BIT(FLASH_ACR, 8);

    /*
     * Make sure the programmed value has taken effect.
     */
    while ((FLASH_ACR & FLASH_ACR_LATENCY_MASK) != latency)
    {
        /* wait */
    }
}


/*===========================================================
 * HSE
 *===========================================================*/

void MCAL_void_RCC_CLK_HSE_ENB(void)
{
    /*
     * HSE ON
     */
    SET_BIT(RCC_STRUCT->RCC_CR, RCC_CR_HSEON);

    /*
     * Wait HSE ready
     */
    while (!GET_BIT(RCC_STRUCT->RCC_CR, RCC_CR_HSERDY))
    {
        /* wait */
    }

    /*
     * Flash latency for 16 MHz HSE
     */
    MCAL_void_FLASH_SetLatency(0);

    /*
     * Select HSE as SYSCLK
     *
     * SW = 01
     */
    RCC_MODIFY_FIELD(
        RCC_STRUCT->RCC_CFGR,
        RCC_CFGR_SW_MASK,
        RCC_CFGR_SW_SHIFT,
        RCC_SW_HSE
    );

    /*
     * Wait until HSE is actually used as SYSCLK
     *
     * SWS = 01
     */
    while (
        ((RCC_STRUCT->RCC_CFGR & RCC_CFGR_SWS_MASK)
        >> RCC_CFGR_SWS_SHIFT) != RCC_SW_HSE
    )
    {
        /* wait */
    }

    MCAL_u32_SystemClock = RCC_HSE_VALUE;
}


/*===========================================================
 * HSI
 *===========================================================*/

void MCAL_void_RCC_CLK_HSI_ENB(void)
{
    /*
     * HSI ON
     */
    SET_BIT(RCC_STRUCT->RCC_CR, RCC_CR_HSION);

    /*
     * Wait HSI ready
     */
    while (!GET_BIT(RCC_STRUCT->RCC_CR, RCC_CR_HSIRDY))
    {
        /* wait */
    }

    /*
     * Flash latency for 16 MHz
     */
    MCAL_void_FLASH_SetLatency(0);

    /*
     * SW = 00
     */
    RCC_MODIFY_FIELD(
        RCC_STRUCT->RCC_CFGR,
        RCC_CFGR_SW_MASK,
        RCC_CFGR_SW_SHIFT,
        RCC_SW_HSI
    );

    /*
     * Wait until HSI becomes SYSCLK
     *
     * SWS = 00
     */
    while (
        ((RCC_STRUCT->RCC_CFGR & RCC_CFGR_SWS_MASK)
        >> RCC_CFGR_SWS_SHIFT) != RCC_SW_HSI
    )
    {
        /* wait */
    }

    MCAL_u32_SystemClock = RCC_HSI_VALUE;
}


/*===========================================================
 * PLL source
 *===========================================================*/

void MCAL_void_PLL_CLK_CONF_SRC(RCC_CLOCK_SRC pll_option)
{
    switch (pll_option)
    {
        case HSE:

            /*
             * PLLSRC = 1
             */
            SET_BIT(
                RCC_STRUCT->RCC_PLLCFGR,
                RCC_PLLCFGR_PLLSRC
            );

            break;


        case HSI:

            /*
             * PLLSRC = 0
             */
            CLEAR_BIT(
                RCC_STRUCT->RCC_PLLCFGR,
                RCC_PLLCFGR_PLLSRC
            );

            break;


        default:

            break;
    }
}


/*===========================================================
 * PLL M / N / P configuration
 *===========================================================*/

RCC_ERRORS_RETURN MCAL_void_PLL_CLK_CONF_mul(
    u32 P,
    u8  M,
    u32 N
)
{
    u32 PLL_INPUT_FREQ;
    u32 VCO_INPUT_FREQ;
    u32 VCO_OUTPUT_FREQ;
    u32 SYSCLK_IN;


    /*-------------------------------------------------------
     * Select input frequency
     *-------------------------------------------------------*/

#ifdef PLL_SRC_HSE

    PLL_INPUT_FREQ = RCC_HSE_VALUE;

#elif defined(PLL_SRC_HSI)

    PLL_INPUT_FREQ = RCC_HSI_VALUE;

#else

#error "Define exactly one PLL source: PLL_SRC_HSE or PLL_SRC_HSI"

#endif


    /*-------------------------------------------------------
     * Validate M
     *
     * STM32F401:
     * PLLM = 2 ... 63
     *-------------------------------------------------------*/

    if ((M < 2U) || (M > 63U))
    {
        return M_WRONG_CONF;
    }


    /*-------------------------------------------------------
     * Validate N
     *
     * STM32F401:
     * PLLN = 192 ... 432
     *-------------------------------------------------------*/

    if ((N < 192U) || (N > 432U))
    {
        return N_WRONG_CONF;
    }


    /*-------------------------------------------------------
     * Validate P BEFORE division
     *-------------------------------------------------------*/

    if ((P != 2U) &&
        (P != 4U) &&
        (P != 6U) &&
        (P != 8U))
    {
        return P_NOT_VALID;
    }


    /*-------------------------------------------------------
     * VCO input
     *-------------------------------------------------------*/

    VCO_INPUT_FREQ = PLL_INPUT_FREQ / (u32)M;


    if ((VCO_INPUT_FREQ < 1000000UL) ||
        (VCO_INPUT_FREQ > 2000000UL))
    {
        return VCO_IN_NOT_IN_RANGE;
    }


    /*-------------------------------------------------------
     * VCO output
     *-------------------------------------------------------*/

    VCO_OUTPUT_FREQ = VCO_INPUT_FREQ * N;


    if ((VCO_OUTPUT_FREQ < 192000000UL) ||
        (VCO_OUTPUT_FREQ > 432000000UL))
    {
        return VCO_OUT_NOT_IN_RANGE;
    }


    /*-------------------------------------------------------
     * SYSCLK
     *-------------------------------------------------------*/

    SYSCLK_IN = VCO_OUTPUT_FREQ / P;


    if (SYSCLK_IN > RCC_MAX_SYSCLK)
    {
        return SYSCLK_EXCEED_84;
    }


    /*-------------------------------------------------------
     * Configure PLLM
     *
     * PLLM = bits 5:0
     *-------------------------------------------------------*/

    RCC_STRUCT->RCC_PLLCFGR &=
        ~RCC_PLLCFGR_PLLM_MASK;

    RCC_STRUCT->RCC_PLLCFGR |=
        ((u32)M << RCC_PLLCFGR_PLLM_SHIFT);


    /*-------------------------------------------------------
     * Configure PLLN
     *
     * PLLN = bits 14:6
     *-------------------------------------------------------*/

    RCC_STRUCT->RCC_PLLCFGR &=
        ~RCC_PLLCFGR_PLLN_MASK;

    RCC_STRUCT->RCC_PLLCFGR |=
        ((u32)N << RCC_PLLCFGR_PLLN_SHIFT);


    /*-------------------------------------------------------
     * Configure PLLP
     *
     * Encoding:
     *
     * 00 -> /2
     * 01 -> /4
     * 10 -> /6
     * 11 -> /8
     *-------------------------------------------------------*/

    RCC_STRUCT->RCC_PLLCFGR &=
        ~RCC_PLLCFGR_PLLP_MASK;


    switch (P)
    {
        case 2U:

            RCC_STRUCT->RCC_PLLCFGR |=
                (0UL << RCC_PLLCFGR_PLLP_SHIFT);

            break;


        case 4U:

            RCC_STRUCT->RCC_PLLCFGR |=
                (1UL << RCC_PLLCFGR_PLLP_SHIFT);

            break;


        case 6U:

            RCC_STRUCT->RCC_PLLCFGR |=
                (2UL << RCC_PLLCFGR_PLLP_SHIFT);

            break;


        case 8U:

            RCC_STRUCT->RCC_PLLCFGR |=
                (3UL << RCC_PLLCFGR_PLLP_SHIFT);

            break;


        default:

            return P_NOT_VALID;
    }


    return PLL_CFG_TRUE;
}


/*===========================================================
 * PLL speed modes
 *===========================================================*/

RCC_ERRORS_RETURN MCAL_void_PLL_CLK_mode(
    PLL_MODES speed_mode
)
{
    RCC_ERRORS_RETURN error;


#ifdef PLL_SRC_HSE

    switch (speed_mode)
    {
        case LOW_SPEED:

            /*
             * 25 MHz / 25 * 192 / 8
             * = 24 MHz
             */
            error = MCAL_void_PLL_CLK_CONF_mul(
                8U,
                25U,
                192U
            );

            if (error == PLL_CFG_TRUE)
            {
                MCAL_u32_SystemClock = 24000000UL;
            }

            break;


        case MEDIUM_SPEED:

            /*
             * 25 MHz / 25 * 192 / 4
             * = 48 MHz
             */
            error = MCAL_void_PLL_CLK_CONF_mul(
                4U,
                25U,
                192U
            );

            if (error == PLL_CFG_TRUE)
            {
                MCAL_u32_SystemClock = 48000000UL;
            }

            break;


        case HIGH_SPEED:

            /*
             * 25 MHz / 25 * 256 / 4
             * = 64 MHz
             */
            error = MCAL_void_PLL_CLK_CONF_mul(
                4U,
                25U,
                256U
            );

            if (error == PLL_CFG_TRUE)
            {
                MCAL_u32_SystemClock = 64000000UL;
            }

            break;


        case MAX_SPEED:

            /*
             * 25 MHz / 25 * 336 / 4
             * = 84 MHz
             */
            error = MCAL_void_PLL_CLK_CONF_mul(
                4U,
                25U,
                336U
            );

            if (error == PLL_CFG_TRUE)
            {
                MCAL_u32_SystemClock = 84000000UL;
            }

            break;


        default:

            error = P_NOT_VALID;

            break;
    }


#elif defined(PLL_SRC_HSI)

    switch (speed_mode)
    {
        case LOW_SPEED:

            /*
             * 16 MHz / 8 * 192 / 8
             * = 24 MHz
             */
            error = MCAL_void_PLL_CLK_CONF_mul(
                8U,
                8U,
                192U
            );

            if (error == PLL_CFG_TRUE)
            {
                MCAL_u32_SystemClock = 24000000UL;
            }

            break;


        case MEDIUM_SPEED:

            /*
             * 16 MHz / 8 * 192 / 4
             * = 48 MHz
             */
            error = MCAL_void_PLL_CLK_CONF_mul(
                4U,
                8U,
                192U
            );

            if (error == PLL_CFG_TRUE)
            {
                MCAL_u32_SystemClock = 48000000UL;
            }

            break;


        case HIGH_SPEED:

            /*
             * 16 MHz / 8 * 256 / 4
             * = 64 MHz
             */
            error = MCAL_void_PLL_CLK_CONF_mul(
                4U,
                8U,
                256U
            );

            if (error == PLL_CFG_TRUE)
            {
                MCAL_u32_SystemClock = 64000000UL;
            }

            break;


        case MAX_SPEED:

            /*
             * 16 MHz / 8 * 336 / 4
             * = 84 MHz
             */
            error = MCAL_void_PLL_CLK_CONF_mul(
                4U,
                8U,
                336U
            );

            if (error == PLL_CFG_TRUE)
            {
                MCAL_u32_SystemClock = 84000000UL;
            }

            break;


        default:

            error = P_NOT_VALID;

            break;
    }


#else

#error "Define exactly one PLL source"

#endif


    return error;
}


/*===========================================================
 * PLL enable
 *===========================================================*/

void MCAL_void_RCC_CLK_PLL_ENB(void)
{
    /*
     * PLL ON
     */
    SET_BIT(
        RCC_STRUCT->RCC_CR,
        RCC_CR_PLLON
    );

    /*
     * Wait PLL ready
     */
    while (!GET_BIT(
        RCC_STRUCT->RCC_CR,
        RCC_CR_PLLRDY
    ))
    {
        /* wait */
    }


    /*
     * SW = 10
     *
     * Select PLL as SYSCLK
     */
    RCC_MODIFY_FIELD(
        RCC_STRUCT->RCC_CFGR,
        RCC_CFGR_SW_MASK,
        RCC_CFGR_SW_SHIFT,
        RCC_SW_PLL
    );


    /*
     * Wait for SWS = 10
     */
    while (
        ((RCC_STRUCT->RCC_CFGR & RCC_CFGR_SWS_MASK)
        >> RCC_CFGR_SWS_SHIFT) != RCC_SW_PLL
    )
    {
        /* wait */
    }
}


/*===========================================================
 * Select SYSCLK
 *===========================================================*/

RCC_ERRORS_RETURN MCAL_void_RCC_CLK_SRC(
    RCC_CLOCK_SRC clock_src
)
{
    RCC_ERRORS_RETURN error;


    switch (clock_src)
    {
        /*---------------------------------------------------
         * HSI
         *---------------------------------------------------*/

        case HSI:

            MCAL_void_RCC_CLK_HSI_ENB();

            return PLL_CFG_TRUE;


        /*---------------------------------------------------
         * HSE
         *---------------------------------------------------*/

        case HSE:

            MCAL_void_RCC_CLK_HSE_ENB();

            return PLL_CFG_TRUE;


        /*---------------------------------------------------
         * PLL
         *---------------------------------------------------*/

        case PLL:

#ifdef PLL_SRC_HSE

            /*
             * HSE must be running before PLL uses it.
             */
            SET_BIT(
                RCC_STRUCT->RCC_CR,
                RCC_CR_HSEON
            );

            while (!GET_BIT(
                RCC_STRUCT->RCC_CR,
                RCC_CR_HSERDY
            ))
            {
                /* wait */
            }

            MCAL_void_PLL_CLK_CONF_SRC(HSE);

#elif defined(PLL_SRC_HSI)

            /*
             * HSI must be running before PLL uses it.
             */
            SET_BIT(
                RCC_STRUCT->RCC_CR,
                RCC_CR_HSION
            );

            while (!GET_BIT(
                RCC_STRUCT->RCC_CR,
                RCC_CR_HSIRDY
            ))
            {
                /* wait */
            }

            MCAL_void_PLL_CLK_CONF_SRC(HSI);

#endif


            /*
             * Configure PLL
             */
            error = MCAL_void_PLL_CLK_mode(
                MAX_SPEED
            );

            if (error != PLL_CFG_TRUE)
            {
                return error;
            }


            /*
             * For 84 MHz
             *
             * 2 Flash wait states required at
             * VDD = 2.7V ... 3.6V.
             */
            MCAL_void_FLASH_SetLatency(2);


            /*
             * HCLK = SYSCLK
             */
            MCAL_VOID_RCC_AHB_PRESCALE(1);


            /*
             * APB1 = HCLK / 2
             *
             * 84 MHz / 2 = 42 MHz
             */
            MCAL_VOID_RCC_APB1_PRESCALE(2);


            /*
             * APB2 = HCLK
             *
             * 84 MHz
             */
            MCAL_VOID_RCC_APB2_PRESCALE(1);


            /*
             * Enable PLL and switch SYSCLK to PLL
             */
            MCAL_void_RCC_CLK_PLL_ENB();


            return PLL_CFG_TRUE;


        default:

            return P_NOT_VALID;
    }
}


/*===========================================================
 * AHB prescaler
 *===========================================================*/

void MCAL_VOID_RCC_AHB_PRESCALE(u16 prescaler)
{
    u32 encoding;


    switch (prescaler)
    {
        case 1U:

            encoding = 0b0000;
            break;


        case 2U:

            encoding = 0b1000;
            break;


        case 4U:

            encoding = 0b1001;
            break;


        case 8U:

            encoding = 0b1010;
            break;


        case 16U:

            encoding = 0b1011;
            break;


        case 64U:

            encoding = 0b1100;
            break;


        case 128U:

            encoding = 0b1101;
            break;


        case 256U:

            encoding = 0b1110;
            break;


        case 512U:

            encoding = 0b1111;
            break;


        default:

            /*
             * Invalid value.
             * Keep current configuration.
             */
            return;
    }


    RCC_MODIFY_FIELD(
        RCC_STRUCT->RCC_CFGR,
        RCC_CFGR_HPRE_MASK,
        RCC_CFGR_HPRE_SHIFT,
        encoding
    );
}


/*===========================================================
 * APB1 prescaler
 *===========================================================*/

void MCAL_VOID_RCC_APB1_PRESCALE(u8 prescaler)
{
    u32 encoding;


    switch (prescaler)
    {
        case 1U:

            encoding = 0b000;
            break;


        case 2U:

            encoding = 0b100;
            break;


        case 4U:

            encoding = 0b101;
            break;


        case 8U:

            encoding = 0b110;
            break;


        case 16U:

            encoding = 0b111;
            break;


        default:

            return;
    }


    RCC_MODIFY_FIELD(
        RCC_STRUCT->RCC_CFGR,
        RCC_CFGR_PPRE1_MASK,
        RCC_CFGR_PPRE1_SHIFT,
        encoding
    );
}


/*===========================================================
 * APB2 prescaler
 *===========================================================*/

void MCAL_VOID_RCC_APB2_PRESCALE(u8 prescaler)
{
    u32 encoding;


    switch (prescaler)
    {
        case 1U:

            encoding = 0b000;
            break;


        case 2U:

            encoding = 0b100;
            break;


        case 4U:

            encoding = 0b101;
            break;


        case 8U:

            encoding = 0b110;
            break;


        case 16U:

            encoding = 0b111;
            break;


        default:

            return;
    }


    RCC_MODIFY_FIELD(
        RCC_STRUCT->RCC_CFGR,
        RCC_CFGR_PPRE2_MASK,
        RCC_CFGR_PPRE2_SHIFT,
        encoding
    );
}


/*===========================================================
 * AHB1 clock
 *===========================================================*/

void MCAL_void_RCC_CLK_ENB_AHB1(void)
{
    /*
     * Enable:
     *
     * GPIOA
     * GPIOB
     * GPIOC
     * GPIOD
     * GPIOE
     * GPIOH
     * DMA1
     * DMA2
     */

    SET_BIT(RCC_STRUCT->RCC_AHB1ENR, 0);
    SET_BIT(RCC_STRUCT->RCC_AHB1ENR, 1);
    SET_BIT(RCC_STRUCT->RCC_AHB1ENR, 2);
    SET_BIT(RCC_STRUCT->RCC_AHB1ENR, 3);
    SET_BIT(RCC_STRUCT->RCC_AHB1ENR, 4);
    SET_BIT(RCC_STRUCT->RCC_AHB1ENR, 7);

    SET_BIT(RCC_STRUCT->RCC_AHB1ENR, 21);
    SET_BIT(RCC_STRUCT->RCC_AHB1ENR, 22);
}


/*===========================================================
 * AHB2 clock
 *===========================================================*/

void MCAL_void_RCC_CLK_ENB_AHB2(void)
{
    /*
     * OTG FS
     */
    SET_BIT(RCC_STRUCT->RCC_AHB2ENR, 7);
}


/*===========================================================
 * DMA
 *===========================================================*/

void MCAL_void_RCC_CLK_ENB_DMA(void)
{
    SET_BIT(RCC_STRUCT->RCC_AHB1ENR, 21);
    SET_BIT(RCC_STRUCT->RCC_AHB1ENR, 22);
}


/*===========================================================
 * All GPIO clocks
 *===========================================================*/

void MCAL_void_RCC_CLK_ENB_GPIO_all(void)
{
    SET_BIT(RCC_STRUCT->RCC_AHB1ENR, 0); /* GPIOA */
    SET_BIT(RCC_STRUCT->RCC_AHB1ENR, 1); /* GPIOB */
    SET_BIT(RCC_STRUCT->RCC_AHB1ENR, 2); /* GPIOC */
    SET_BIT(RCC_STRUCT->RCC_AHB1ENR, 3); /* GPIOD */
    SET_BIT(RCC_STRUCT->RCC_AHB1ENR, 4); /* GPIOE */
    SET_BIT(RCC_STRUCT->RCC_AHB1ENR, 7); /* GPIOH */
}


/*===========================================================
 * Individual GPIO clock
 *===========================================================*/

void MCAL_void_RCC_CLK_ENB_GPIO(
    GPIO_ENUM GPIO_SELECT
)
{
    switch (GPIO_SELECT)
    {
        case GPIOA:

            SET_BIT(
                RCC_STRUCT->RCC_AHB1ENR,
                0
            );

            break;


        case GPIOB:

            SET_BIT(
                RCC_STRUCT->RCC_AHB1ENR,
                1
            );

            break;


        case GPIOC:

            SET_BIT(
                RCC_STRUCT->RCC_AHB1ENR,
                2
            );

            break;


        case GPIOD:

            SET_BIT(
                RCC_STRUCT->RCC_AHB1ENR,
                3
            );

            break;


        case GPIOE:

            SET_BIT(
                RCC_STRUCT->RCC_AHB1ENR,
                4
            );

            break;


        case GPIOH:

            SET_BIT(
                RCC_STRUCT->RCC_AHB1ENR,
                7
            );

            break;


        default:

            break;
    }
}


/*===========================================================
 * APB1 clock
 *
 * This function enables the commonly-used APB1
 * peripherals from your original driver.
 *===========================================================*/

void MCAL_void_RCC_CLK_ENB_APB1(void)
{
    /*
     * TIM2
     * TIM3
     * TIM4
     * TIM5
     * USART2
     * SPI2
     * SPI3
     * I2C1
     * I2C2
     * I2C3
     */

    SET_BIT(RCC_STRUCT->RCC_APB1ENR, 0);
    SET_BIT(RCC_STRUCT->RCC_APB1ENR, 1);
    SET_BIT(RCC_STRUCT->RCC_APB1ENR, 2);
    SET_BIT(RCC_STRUCT->RCC_APB1ENR, 3);

    SET_BIT(RCC_STRUCT->RCC_APB1ENR, 14);
    SET_BIT(RCC_STRUCT->RCC_APB1ENR, 15);

    SET_BIT(RCC_STRUCT->RCC_APB1ENR, 17);

    SET_BIT(RCC_STRUCT->RCC_APB1ENR, 21);
    SET_BIT(RCC_STRUCT->RCC_APB1ENR, 22);
    SET_BIT(RCC_STRUCT->RCC_APB1ENR, 23);
}


/*===========================================================
 * APB2 clock
 *===========================================================*/

void MCAL_void_RCC_CLK_ENB_APB2(void)
{
    /*
     * USART1
     * USART6
     * SPI1
     * SPI4
     * TIM1
     * TIM9
     * TIM10
     * TIM11
     */

    SET_BIT(RCC_STRUCT->RCC_APB2ENR, 0);   /* TIM1  */
    SET_BIT(RCC_STRUCT->RCC_APB2ENR, 4);   /* USART1 */
    SET_BIT(RCC_STRUCT->RCC_APB2ENR, 5);   /* USART6 */
    SET_BIT(RCC_STRUCT->RCC_APB2ENR, 12);  /* SPI1  */
    SET_BIT(RCC_STRUCT->RCC_APB2ENR, 13);  /* SPI4  */

    SET_BIT(RCC_STRUCT->RCC_APB2ENR, 16);  /* TIM9  */
    SET_BIT(RCC_STRUCT->RCC_APB2ENR, 17);  /* TIM10 */
    SET_BIT(RCC_STRUCT->RCC_APB2ENR, 18);  /* TIM11 */
}


/*===========================================================
 * USART all
 *===========================================================*/

void MCAL_void_RCC_CLK_ENB_USART_all(void)
{
    SET_BIT(RCC_STRUCT->RCC_APB2ENR, 4);  /* USART1 */
    SET_BIT(RCC_STRUCT->RCC_APB1ENR, 17); /* USART2 */
    SET_BIT(RCC_STRUCT->RCC_APB2ENR, 5);  /* USART6 */
}


/*===========================================================
 * USART individual
 *===========================================================*/

void MCAL_void_RCC_CLK_ENB_USART(
    USART_ENUM USART_SELECT
)
{
    switch (USART_SELECT)
    {
        case USART1:

            SET_BIT(
                RCC_STRUCT->RCC_APB2ENR,
                4
            );

            break;


        case USART2:

            SET_BIT(
                RCC_STRUCT->RCC_APB1ENR,
                17
            );

            break;


        case USART6:

            SET_BIT(
                RCC_STRUCT->RCC_APB2ENR,
                5
            );

            break;


        default:

            break;
    }
}


/*===========================================================
 * I2C all
 *===========================================================*/

void MCAL_void_RCC_CLK_ENB_I2C_all(void)
{
    SET_BIT(RCC_STRUCT->RCC_APB1ENR, 21);
    SET_BIT(RCC_STRUCT->RCC_APB1ENR, 22);
    SET_BIT(RCC_STRUCT->RCC_APB1ENR, 23);
}


/*===========================================================
 * I2C individual
 *===========================================================*/

void MCAL_void_RCC_CLK_ENB_I2C(
    I2C_ENUM I2C_SELECT
)
{
    switch (I2C_SELECT)
    {
        case I2C1:

            SET_BIT(
                RCC_STRUCT->RCC_APB1ENR,
                21
            );

            break;


        case I2C2:

            SET_BIT(
                RCC_STRUCT->RCC_APB1ENR,
                22
            );

            break;


        case I2C3:

            SET_BIT(
                RCC_STRUCT->RCC_APB1ENR,
                23
            );

            break;


        default:

            break;
    }
}


/*===========================================================
 * SPI all
 *===========================================================*/

void MCAL_void_RCC_CLK_ENB_SPI_all(void)
{
    SET_BIT(RCC_STRUCT->RCC_APB2ENR, 12); /* SPI1 */
    SET_BIT(RCC_STRUCT->RCC_APB1ENR, 14); /* SPI2 */
    SET_BIT(RCC_STRUCT->RCC_APB1ENR, 15); /* SPI3 */
    SET_BIT(RCC_STRUCT->RCC_APB2ENR, 13); /* SPI4 */
}


/*===========================================================
 * SPI individual
 *===========================================================*/

void MCAL_void_RCC_CLK_ENB_SPI(
    SPI_ENUM SPI_SELECT
)
{
    switch (SPI_SELECT)
    {
        case SPI1:

            SET_BIT(
                RCC_STRUCT->RCC_APB2ENR,
                12
            );

            break;


        case SPI2:

            SET_BIT(
                RCC_STRUCT->RCC_APB1ENR,
                14
            );

            break;


        case SPI3:

            SET_BIT(
                RCC_STRUCT->RCC_APB1ENR,
                15
            );

            break;


        case SPI4:

            SET_BIT(
                RCC_STRUCT->RCC_APB2ENR,
                13
            );

            break;


        default:

            break;
    }
}


/*===========================================================
 * TIMER individual
 *===========================================================*/

void MCAL_void_RCC_CLK_ENB_TIM(
    TIMER_ENUM TIM_SELECT
)
{
    switch (TIM_SELECT)
    {
        case TIM1:

            SET_BIT(
                RCC_STRUCT->RCC_APB2ENR,
                0
            );

            break;


        case TIM2:

            SET_BIT(
                RCC_STRUCT->RCC_APB1ENR,
                0
            );

            break;


        case TIM3:

            SET_BIT(
                RCC_STRUCT->RCC_APB1ENR,
                1
            );

            break;


        case TIM4:

            SET_BIT(
                RCC_STRUCT->RCC_APB1ENR,
                2
            );

            break;


        case TIM5:

            SET_BIT(
                RCC_STRUCT->RCC_APB1ENR,
                3
            );

            break;


        case TIM9:

            SET_BIT(
                RCC_STRUCT->RCC_APB2ENR,
                16
            );

            break;


        case TIM10:

            SET_BIT(
                RCC_STRUCT->RCC_APB2ENR,
                17
            );

            break;


        case TIM11:

            SET_BIT(
                RCC_STRUCT->RCC_APB2ENR,
                18
            );

            break;


        default:

            break;
    }
}


/*===========================================================
 * Default RCC initialization
 *===========================================================*/

RCC_ERRORS_RETURN MCAL_VOID_RCC_init_default(void)
{
    RCC_ERRORS_RETURN error;


    /*-------------------------------------------------------
     * Configure system clock
     *
     * PLL -> 84 MHz
     *-------------------------------------------------------*/

    error = MCAL_void_RCC_CLK_SRC(PLL);

    if (error != PLL_CFG_TRUE)
    {
        return error;
    }


    /*-------------------------------------------------------
     * Enable GPIO clocks
     *-------------------------------------------------------*/

    MCAL_void_RCC_CLK_ENB_GPIO_all();


    /*-------------------------------------------------------
     * Enable USART clocks
     *-------------------------------------------------------*/

    MCAL_void_RCC_CLK_ENB_USART_all();


    /*-------------------------------------------------------
     * Enable I2C clocks
     *-------------------------------------------------------*/

    MCAL_void_RCC_CLK_ENB_I2C_all();


    return PLL_CFG_TRUE;
}


void MCAL_void_RCC_Init(void)
{
    *(volatile unsigned int *)(0x40023800UL + 0x30) |= (1U << 0);
}