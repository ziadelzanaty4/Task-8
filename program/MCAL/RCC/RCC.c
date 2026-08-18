#include "RCC.h"





void MCAL_void_RCC_CLK_HSE_ENB();
void MCAL_void_RCC_CLK_HSI_ENB();
void MCAL_void_RCC_CLK_PLL_ENB();

void MCAL_void_PLL_CLK_CONF(RCC_CLOCK_SRC pll_option);


RCC_ERRORS_RETURN MCAL_void_PLL_CLK_CONF_mul(u32 P , u8 M ,u32 N);


void MCAL_void_RCC_CLK_SRC(RCC_CLOCK_SRC clock_src){
    switch (clock_src)
    {
    case HSE:
        #define SYSTEM_CLK   16000000U
        MCAL_void_RCC_CLK_HSE_ENB();
        break;
    case HSI:
        #define SYSTEM_CLK   16000000U
        MCAL_void_RCC_CLK_HSI_ENB();
        break;
    case PLL:
       
        #ifdef PLL_SRC_HSE
        //hse set as a src here
        MCAL_void_PLL_CLK_CONF(HSE);
        // pll configuration method
        MCAL_void_PLL_CLK_mode(MAX_SPEED);
        
        
        #elif PLL_SRC_HSI
        //hsi set as a src here
        MCAL_void_PLL_CLK_CONF(HSI);
         // pll configuration method
        MCAL_void_PLL_CLK_mode(MAX_SPEED);
        #endif
        // pll enable method
        MCAL_void_RCC_CLK_PLL_ENB();
        break;    
    default:
        break;
    }
};

void MCAL_void_RCC_CLK_HSE_ENB(){
    set_bit(RCC_STRUCT->RCC_CR,16);
    while(!get_bit(RCC_STRUCT->RCC_CR,17));
    set_bit(RCC_STRUCT->RCC_CFGR,0);
    clear_bit(RCC_STRUCT->RCC_CFGR,1);
};

void MCAL_void_RCC_CLK_HSI_ENB(){
    set_bit(RCC_STRUCT->RCC_CR,0);
    while(!get_bit(RCC_STRUCT->RCC_CR,1));
    clear_bit(RCC_STRUCT->RCC_CFGR,0);
    clear_bit(RCC_STRUCT->RCC_CFGR,1);
};

void MCAL_void_RCC_CLK_PLL_ENB(){
    set_bit(RCC_STRUCT->RCC_CR,24);
    while(!get_bit(RCC_STRUCT->RCC_CR,25));
    clear_bit(RCC_STRUCT->RCC_CFGR,0);
    set_bit(RCC_STRUCT->RCC_CFGR,1);
};


void MCAL_void_PLL_CLK_CONF_SRC(RCC_CLOCK_SRC pll_option){
    switch (pll_option)
    {
    case HSE:
        set_bit(RCC_STRUCT->RCC_PLLCFGR,22);
        break;
    case HSI:
        clear_bit(RCC_STRUCT->RCC_PLLCFGR,22);
        break;
    default:
        break;
    }
};

RCC_ERRORS_RETURN MCAL_void_PLL_CLK_CONF_mul(u32 P , u8 M ,u32 N){
    #ifdef PLL_SRC_HSE
    //16mhz  -> needs to be checked
    u32 PLL_INPUT_FREQ=25000000U;
    #elif  PLL_SRC_HSI
    //16mhz
    u32 PLL_INPUT_FREQ=8000000U;
    #endif
    if(N<192 || N>432 ){
        return N_WRONG_CONF;
    }
    if(M<2  || M>63){
        return M_WRONG_CONF;
    }
    u32 VCO_INPUT_FREQ=PLL_INPUT_FREQ/M;
    if(VCO_INPUT_FREQ > 2000000 || VCO_INPUT_FREQ < 1000000 ){
        return VCO_IN_NOT_IN_RANGE;
    }
    u32 VCO_OUTPUT_FREQ=VCO_INPUT_FREQ*N;
    if(VCO_OUTPUT_FREQ>432000000 || VCO_OUTPUT_FREQ < 192000000){
        return VCO_OUT_NOT_IN_RANGE;
    }
    u32 SYSTEM_CLOCK_IN=VCO_OUTPUT_FREQ/P;
    if (SYSTEM_CLOCK_IN > 84000000)
    {
        return SYSCLK_EXCEED_84;
    }
    RCC_STRUCT->RCC_PLLCFGR |= (M<<5);
    RCC_STRUCT->RCC_PLLCFGR |= (N<<6);
    switch (P)
    {
    case 2:
        clear_bit(RCC_STRUCT->RCC_PLLCFGR,16);
        clear_bit(RCC_STRUCT->RCC_PLLCFGR,17);
        break;
    case 4:
        set_bit(RCC_STRUCT->RCC_PLLCFGR,16);
        clear_bit(RCC_STRUCT->RCC_PLLCFGR,17);
        break;
    case 6:
        clear_bit(RCC_STRUCT->RCC_PLLCFGR,16);
        set_bit(RCC_STRUCT->RCC_PLLCFGR,17);
        break;
    case 8:
        set_bit(RCC_STRUCT->RCC_PLLCFGR,16);
        set_bit(RCC_STRUCT->RCC_PLLCFGR,17);
        break;
    default:
        return P_NOT_VALID;
        break;
    }
    return PLL_CFG_TRUE;
    
};

RCC_ERRORS_RETURN MCAL_void_PLL_CLK_mode(PLL_MODES speed_mode){
    #ifdef PLL_SRC_HSE
    switch (speed_mode)
    {
    // 84 max , low speed 24 mhz 
    case LOW_SPEED:
        return MCAL_void_PLL_CLK_CONF_mul(8,25,192);
        #define SYSTEM_CLK   24000000U
        break;
    //medium speed 48 mhz    
    case MEDIUM_SPEED:
        return MCAL_void_PLL_CLK_CONF_mul(4,25,192);
        #define SYSTEM_CLK   48000000U
        break;
    //high speed 64 mhz    
    case HIGH_SPEED:
        return MCAL_void_PLL_CLK_CONF_mul(4,25,256);
        #define SYSTEM_CLK   64000000U
        break;
    // max speed 84 mhz    
    case MAX_SPEED:
        return MCAL_void_PLL_CLK_CONF_mul(4,25,336);
        #define SYSTEM_CLK   84000000U    
        break;       
    default:
        return MCAL_void_PLL_CLK_CONF_mul(4,25,336);
        #define SYSTEM_CLK   84000000U
        break;
    }
    #elif PLL_SRC_HSI
    switch (speed_mode)
    {
    // 84 max , low speed 24 mhz 
    case LOW_SPEED:
        return MCAL_void_PLL_CLK_CONF_mul(8,8,192);
        #define SYSTEM_CLK   24000000U
        break;
    //medium speed 48 mhz    
    case MEDIUM_SPEED:
        return MCAL_void_PLL_CLK_CONF_mul(4,8,192);
        #define SYSTEM_CLK   48000000U
        break;
    //high speed 64 mhz    
    case HIGH_SPEED:
        return MCAL_void_PLL_CLK_CONF_mul(4,8,256);
        #define SYSTEM_CLK   64000000U
        break;
    // max speed 84 mhz    
    case MAX_SPEED:
        return MCAL_void_PLL_CLK_CONF_mul(4,8,336);
        #define SYSTEM_CLK   84000000U    
        break;       
    default:
        return MCAL_void_PLL_CLK_CONF_mul(4,8,336);
        #define SYSTEM_CLK   84000000U
        break;
    }
    #endif

}


void MCAL_void_RCC_CLK_ENB_AHB1(){
    RCC_STRUCT->RCC_AHB1ENR=(u32)0x0060109F;
}

void MCAL_void_RCC_CLK_ENB_AHB2(){
    set_bit(RCC_STRUCT->RCC_AHB2ENR,7);
}
void MCAL_void_RCC_CLK_ENB_APB1(){
    RCC_STRUCT->RCC_APB1ENR=(u32)0b10000111000101100100000001111;

}
void MCAL_void_RCC_CLK_ENB_APB2(){
    RCC_STRUCT->RCC_APB2ENR=(u32)0b1110111100100110001;
    
}

void MCAL_void_RCC_CLK_ENB_GPIO_all(){
    set_bit(RCC_STRUCT->RCC_AHB1ENR,0);
    set_bit(RCC_STRUCT->RCC_AHB1ENR,1);
    set_bit(RCC_STRUCT->RCC_AHB1ENR,2);
        
}

void MCAL_void_RCC_CLK_ENB_USART_all(){
    set_bit(RCC_STRUCT->RCC_APB2ENR,4);
    set_bit(RCC_STRUCT->RCC_APB1ENR,17);
    set_bit(RCC_STRUCT->RCC_APB2ENR,5);
    
}
void MCAL_void_RCC_CLK_ENB_I2C_all(){
    set_bit(RCC_STRUCT->RCC_APB1ENR,21);
    set_bit(RCC_STRUCT->RCC_APB1ENR,22);
    set_bit(RCC_STRUCT->RCC_APB1ENR,23);
}


void MCAL_void_RCC_CLK_ENB_SPI_all(){
    set_bit(RCC_STRUCT->RCC_APB2ENR,12);
    set_bit(RCC_STRUCT->RCC_APB1ENR,14);
    set_bit(RCC_STRUCT->RCC_APB1ENR,15);
    set_bit(RCC_STRUCT->RCC_APB2ENR,13);
}
void MCAL_void_RCC_CLK_ENB_DMA(){
    set_bit(RCC_STRUCT->RCC_AHB1ENR,21);
    set_bit(RCC_STRUCT->RCC_AHB1ENR,22);
}

void MCAL_void_RCC_CLK_ENB_GPIO(GPIO_ENUM GPIO_SELECT){
    switch (GPIO_SELECT)
    {
    case GPIOA:
        set_bit(RCC_STRUCT->RCC_AHB1ENR,0);
        break;
    case GPIOB:
        set_bit(RCC_STRUCT->RCC_AHB1ENR,1);
        break;
    case GPIOC:
        set_bit(RCC_STRUCT->RCC_AHB1ENR,2);
        break;
    case GPIOD:
        set_bit(RCC_STRUCT->RCC_AHB1ENR,3);
        break;
    case GPIOE:
        set_bit(RCC_STRUCT->RCC_AHB1ENR,4);
        break;
    case GPIOH:
        set_bit(RCC_STRUCT->RCC_AHB1ENR,7);
        break;                
        
    default:
        break;
    }
}

void MCAL_void_RCC_CLK_ENB_TIM(TIMER_ENUM TIM_SELECT){
    switch (TIM_SELECT)
    {
    case TIM1:
        set_bit(RCC_STRUCT->RCC_APB2ENR,0);
        break;
    case TIM2:
        set_bit(RCC_STRUCT->RCC_APB1ENR,0);
        break;
    case TIM3:
        set_bit(RCC_STRUCT->RCC_APB1ENR,1);
        break;
    case TIM4:
        set_bit(RCC_STRUCT->RCC_APB1ENR,2);
        break;
    case TIM5:
        set_bit(RCC_STRUCT->RCC_APB1ENR,3);
        break;
    case TIM9:
        set_bit(RCC_STRUCT->RCC_APB2ENR,16);
        break;
    case TIM10:
        set_bit(RCC_STRUCT->RCC_APB2ENR,17);
        break;
    case TIM11:
        set_bit(RCC_STRUCT->RCC_APB2ENR,18);
        break;
                                                                
    default:
        break;
    }
}

void MCAL_void_RCC_CLK_ENB_USART(USART_ENUM USART_SELECT){
    switch (USART_SELECT)
    {
    case USART1:
        set_bit(RCC_STRUCT->RCC_APB2ENR,4);
        break;
    case USART2:
        set_bit(RCC_STRUCT->RCC_APB1ENR,17);
        break;
    case USART6:
        set_bit(RCC_STRUCT->RCC_APB2ENR,5);
        break;
    default:
        break;
    }
}

void MCAL_void_RCC_CLK_ENB_I2C(I2C_ENUM I2C_SELECT){
    switch (I2C_SELECT)
    {
    case I2C1:
        set_bit(RCC_STRUCT->RCC_APB1ENR,21);
        break;
    case I2C2:
        set_bit(RCC_STRUCT->RCC_APB1ENR,22);
        break;
    case I2C3:
        set_bit(RCC_STRUCT->RCC_APB1ENR,23);
        break;    
    default:
        break;
    }
}

void MCAL_void_RCC_CLK_ENB_SPI(SPI_ENUM SPI_SELECT){
    switch (SPI_SELECT)
    {
    case SPI1:
        set_bit(RCC_STRUCT->RCC_APB2ENR,12);
        break;
    case SPI2:
        set_bit(RCC_STRUCT->RCC_APB1ENR,14);
        break;
    case SPI3:
        set_bit(RCC_STRUCT->RCC_APB1ENR,15);
        break;
    case SPI4:
        set_bit(RCC_STRUCT->RCC_APB2ENR,13);
        break;
    default:
        break;
    }

}

void MCAL_VOID_RCC_APB1_PRESCALE(u8 prescaler){
    switch (prescaler)
    {
    case 1:
        clear_bit(RCC_STRUCT->RCC_CFGR,12);
        break;
    case 2:
        set_bit(RCC_STRUCT->RCC_CFGR,12);
        clear_bit(RCC_STRUCT->RCC_CFGR,11);
        clear_bit(RCC_STRUCT->RCC_CFGR,10); 
    case 4:
        set_bit(RCC_STRUCT->RCC_CFGR,12);
        clear_bit(RCC_STRUCT->RCC_CFGR,11);
        set_bit(RCC_STRUCT->RCC_CFGR,10);
    case 8:
        set_bit(RCC_STRUCT->RCC_CFGR,12);
        set_bit(RCC_STRUCT->RCC_CFGR,11);
        clear_bit(RCC_STRUCT->RCC_CFGR,10);
    case 16:
        set_bit(RCC_STRUCT->RCC_CFGR,12);
        set_bit(RCC_STRUCT->RCC_CFGR,11);
        set_bit(RCC_STRUCT->RCC_CFGR,10); 
    default:
        break;
    }   
}

void MCAL_VOID_RCC_APB2_PRESCALE(u8 prescaler){
    switch (prescaler)
    {
    case 1:
        clear_bit(RCC_STRUCT->RCC_CFGR,15);
        break;
    case 2:
        set_bit(RCC_STRUCT->RCC_CFGR,15);
        clear_bit(RCC_STRUCT->RCC_CFGR,14);
        clear_bit(RCC_STRUCT->RCC_CFGR,13);
    case 4:
        set_bit(RCC_STRUCT->RCC_CFGR,15);
        clear_bit(RCC_STRUCT->RCC_CFGR,14);
        set_bit(RCC_STRUCT->RCC_CFGR,13);
    case 8:
        set_bit(RCC_STRUCT->RCC_CFGR,15);
        set_bit(RCC_STRUCT->RCC_CFGR,14);
        clear_bit(RCC_STRUCT->RCC_CFGR,13);
    case 16:
        set_bit(RCC_STRUCT->RCC_CFGR,15);
        set_bit(RCC_STRUCT->RCC_CFGR,14);
        set_bit(RCC_STRUCT->RCC_CFGR,13); 
    default:
        break;
    }   
}

void MCAL_VOID_RCC_AHB_PRESCALE(u8 prescaler){
    switch (prescaler)
    {
    case 1:
        clear_bit(RCC_STRUCT->RCC_CFGR,20);
        break;
    case 2:
        set_bit(RCC_STRUCT->RCC_CFGR,20);
        clear_bit(RCC_STRUCT->RCC_CFGR,19);
        clear_bit(RCC_STRUCT->RCC_CFGR,18);
        clear_bit(RCC_STRUCT->RCC_CFGR,17);
    case 4:
        set_bit(RCC_STRUCT->RCC_CFGR,20);
        clear_bit(RCC_STRUCT->RCC_CFGR,19);
        clear_bit(RCC_STRUCT->RCC_CFGR,18);
        set_bit(RCC_STRUCT->RCC_CFGR,17);
    case 8:
        set_bit(RCC_STRUCT->RCC_CFGR,20);
        clear_bit(RCC_STRUCT->RCC_CFGR,19);
        set_bit(RCC_STRUCT->RCC_CFGR,18);
        clear_bit(RCC_STRUCT->RCC_CFGR,17);
    case 16:
        set_bit(RCC_STRUCT->RCC_CFGR,20);
        clear_bit(RCC_STRUCT->RCC_CFGR,19);
        set_bit(RCC_STRUCT->RCC_CFGR,18);
        set_bit(RCC_STRUCT->RCC_CFGR,17); 

    // TO BE CONTIUED
    
    default:
        break;
    }
}


void MCAL_VOID_RCC_init_default(){
    MCAL_void_RCC_CLK_SRC(PLL);
    MCAL_VOID_RCC_APB1_PRESCALE(2);
    MCAL_void_RCC_CLK_ENB_GPIO_all();
    MCAL_void_RCC_CLK_ENB_USART_all();  //continue
    MCAL_void_RCC_CLK_ENB_I2C_all();  //continue 
}


