#include "GPIO.h"





void GPIO_SELECT_MODE(GPIO_ENUM GPIO_OPTION,u8 pin,u8 mode){
    //change to null
    // volatile u32* MODE_REG = &(GPIOA_STRUCT->MODER);
    volatile u32* MODE_REG = NULL;
    switch (GPIO_OPTION)
    {
    case GPIOA:
         MODE_REG = &(GPIOA_STRUCT->MODER);
        break;
    case GPIOB:
         MODE_REG = &(GPIOB_STRUCT->MODER);
        break;
    case GPIOC:
         MODE_REG = &(GPIOC_STRUCT->MODER);
        break;
    case GPIOD:
         MODE_REG = &(GPIOD_STRUCT->MODER);
        break;
    case GPIOE:
        MODE_REG = &(GPIOE_STRUCT->MODER);
        break;
    case GPIOH:
        MODE_REG = &(GPIOH_STRUCT->MODER);
        break;                                    
    default:
        break;
    }
    switch (mode)
    {
    case OUTPUT:
        (*MODE_REG) &= ~(0b11<<2*pin);
        (*MODE_REG) |= (OUTPUT<<2*pin);       
        break;
    case INPUT:
        (*MODE_REG) &= ~(0b11<<2*pin);
        (*MODE_REG) |= (INPUT<<2*pin);
        break;
    case AF:
        (*MODE_REG) &= ~(0b11<<2*pin);
        (*MODE_REG) |= (AF<<2*pin);
        break;
    case ANALOG:
        // (*MODE_REG) &= ~(0b11<<pin);   //1110011111
        // (*MODE_REG) |= (ANALOG<<pin);
        write_bit_double((*MODE_REG),pin,ANALOG);
        break;            
    default:
        break;
    }
}


void GPIO_void_SELECT_OP_MODE(GPIO_ENUM GPIO_OPTION,u8 pin,u8 mode){
    volatile u32* OTYPE_REG = NULL;
    switch (GPIO_OPTION)
    {
    case GPIOA:
         OTYPE_REG = &(GPIOA_STRUCT->OTYPER);
        break;
    case GPIOB:
         OTYPE_REG = &(GPIOB_STRUCT->OTYPER);
        break;
    case GPIOC:
         OTYPE_REG = &(GPIOC_STRUCT->OTYPER);
        break;
    case GPIOD:
         OTYPE_REG = &(GPIOD_STRUCT->OTYPER);
        break;
    case GPIOE:
        OTYPE_REG = &(GPIOE_STRUCT->OTYPER);
        break;
    case GPIOH:
        OTYPE_REG = &(GPIOH_STRUCT->OTYPER);
        break;                                    
    default:
        break;
    }
    if (mode==OPEN_DRAIN_MODE)
    {
        set_bit((*OTYPE_REG),pin);
    }
    else if (mode==PUSH_PULL_MODE)
    {
        clear_bit((*OTYPE_REG),pin);
    }
}

void GPIO_void_SELECT_SPEED_MODE(GPIO_ENUM GPIO_OPTION,u8 pin,u8 mode){
    volatile u32* OSPEED_REG = NULL;
    switch (GPIO_OPTION)
    {
    case GPIOA:
         OSPEED_REG = &(GPIOA_STRUCT->OSPEEDR);
        break;
    case GPIOB:
         OSPEED_REG = &(GPIOB_STRUCT->OSPEEDR);
        break;
    case GPIOC:
         OSPEED_REG = &(GPIOC_STRUCT->OSPEEDR);
        break;
    case GPIOD:
         OSPEED_REG = &(GPIOD_STRUCT->OSPEEDR);
        break;
    case GPIOE:
        OSPEED_REG = &(GPIOE_STRUCT->OSPEEDR);
        break;
    case GPIOH:
        OSPEED_REG = &(GPIOH_STRUCT->OSPEEDR);
        break;                                    
    default:
        break;
    }
    write_bit_double((*OSPEED_REG),pin,mode);
}

void GPIO_void_SELECT_PULL_MODE(GPIO_ENUM GPIO_OPTION,u8 pin,u8 mode){
    volatile u32* PUPD_REG = NULL;
    switch (GPIO_OPTION)
    {
    case GPIOA:
         PUPD_REG = &(GPIOA_STRUCT->PUPDR);
        break;
    case GPIOB:
         PUPD_REG = &(GPIOB_STRUCT->PUPDR);
        break;
    case GPIOC:
         PUPD_REG = &(GPIOC_STRUCT->PUPDR);
        break;
    case GPIOD:
         PUPD_REG = &(GPIOD_STRUCT->PUPDR);
        break;
    case GPIOE:
        PUPD_REG = &(GPIOE_STRUCT->PUPDR);
        break;
    case GPIOH:
        PUPD_REG = &(GPIOH_STRUCT->PUPDR);
        break;                                    
    default:
        break;
    }
    if(mode==PUPD_RESERVED) return;
    write_bit_double((*PUPD_REG),pin,mode);

}



u8 GPIO_void_digital_Read(GPIO_ENUM GPIO_OPTION,u8 pin){
    volatile u32* ID_REG = NULL;
    switch (GPIO_OPTION)
    {
    case GPIOA:
         ID_REG = &(GPIOA_STRUCT->IDR);
        break;
    case GPIOB:
         ID_REG = &(GPIOB_STRUCT->IDR);
        break;
    case GPIOC:
         ID_REG = &(GPIOC_STRUCT->IDR);
        break;
    case GPIOD:
         ID_REG = &(GPIOD_STRUCT->IDR);
        break;
    case GPIOE:
        ID_REG = &(GPIOE_STRUCT->IDR);
        break;
    case GPIOH:
        ID_REG = &(GPIOH_STRUCT->IDR);
        break;                                    
    default:
        break;
    }
    return get_bit((*ID_REG),pin);
}


void GPIO_void_digital_Write(GPIO_ENUM GPIO_OPTION,u8 pin,u8 mode){
    volatile u32* OD_REG = NULL;
    switch (GPIO_OPTION)
    {
    case GPIOA:
         OD_REG = &(GPIOA_STRUCT->ODR);
        break;
    case GPIOB:
         OD_REG = &(GPIOB_STRUCT->ODR);
        break;
    case GPIOC:
         OD_REG = &(GPIOC_STRUCT->ODR);
        break;
    case GPIOD:
         OD_REG = &(GPIOD_STRUCT->ODR);
        break;
    case GPIOE:
        OD_REG = &(GPIOE_STRUCT->ODR);
        break;
    case GPIOH:
        OD_REG = &(GPIOH_STRUCT->ODR);
        break;                                    
    default:
        break;
    }
    if (mode)
    {
        set_bit((*OD_REG),pin);
    }
    else if (!mode)
    {
        clear_bit((*OD_REG),pin);
    }

}



void GPIO_void_digital_Write_ATOMIC(GPIO_ENUM GPIO_OPTION,u8 pin,u8 mode){
    volatile u32* BSS_REG = NULL;
    switch (GPIO_OPTION)
    {
    case GPIOA:
         BSS_REG = &(GPIOA_STRUCT->BSSR);
        break;
    case GPIOB:
         BSS_REG = &(GPIOB_STRUCT->BSSR);
        break;
    case GPIOC:
         BSS_REG = &(GPIOC_STRUCT->BSSR);
        break;
    case GPIOD:
         BSS_REG = &(GPIOD_STRUCT->BSSR);
        break;
    case GPIOE:
        BSS_REG = &(GPIOE_STRUCT->BSSR);
        break;
    case GPIOH:
        BSS_REG = &(GPIOH_STRUCT->BSSR);
        break;                                    
    default:
        break;
    }
    if(mode==HIGH){
        (*BSS_REG) |= (1U<<pin);
    }
    else if (mode==LOW)
    {
        (*BSS_REG) |= (1U<<(pin+16U));
    } 
}


void GPIO_void_set_AF(GPIO_ENUM GPIO_OPTION,u8 pin, AF_ENUM AF_OPTION){
     volatile u32* AF_REG = NULL;
    if (pin<8)
    {
    switch (GPIO_OPTION)
    {
    case GPIOA:
         AF_REG = &(GPIOA_STRUCT->AFRL);
        break;
    case GPIOB:
         AF_REG = &(GPIOB_STRUCT->AFRL);
        break;
    case GPIOC:
         AF_REG = &(GPIOC_STRUCT->AFRL);
        break;
    case GPIOD:
         AF_REG = &(GPIOD_STRUCT->AFRL);
        break;
    case GPIOE:
        AF_REG = &(GPIOE_STRUCT->AFRL);
        break;
    case GPIOH:
        AF_REG = &(GPIOH_STRUCT->AFRL);
        break;                                    
    default:
        break;
    }
    }
    else if ((pin < 16) && (pin >=8)){
    switch (GPIO_OPTION)
    {
    case GPIOA:
         AF_REG = &(GPIOA_STRUCT->AFRH);
        break;
    case GPIOB:
         AF_REG = &(GPIOB_STRUCT->AFRH);
        break;
    case GPIOC:
         AF_REG = &(GPIOC_STRUCT->AFRH);
        break;
    case GPIOD:
         AF_REG = &(GPIOD_STRUCT->AFRH);
        break;
    case GPIOE:
        AF_REG = &(GPIOE_STRUCT->AFRH);
        break;
    case GPIOH:
        AF_REG = &(GPIOH_STRUCT->AFRH);
        break;                                    
    default:
        break;
    }
    }
    (*AF_REG) &= ~((0b1111)<<4*pin);
    (*AF_REG) |= ((u8)(AF_OPTION)<<4*pin); 
    
}


void GPIO_void_enable_AF(GPIO_ENUM GPIO_OPTION,u8 pin, AF_ENUM AF_OPTION){
    GPIO_void_SELECT_MODE(GPIO_OPTION, pin,AF);
    GPIO_void_set_AF(GPIO_OPTION, pin,AF_OPTION); 
}


void GPIO_init(GPIO_CONFIG CONFIG){
    GPIO_void_SELECT_MODE(CONFIG.port,CONFIG.pin,CONFIG.mode);
    GPIO_void_SELECT_OP_MODE(CONFIG.port,CONFIG.pin,OP_MODE);
    GPIO_void_SELECT_SPEED_MODE(CONFIG.port,CONFIG.pin,SPEED_MODE);
    GPIO_void_SELECT_PULL_MODE(CONFIG.port,CONFIG.pin,PULL_TYPE);
}

void pinMode(u8 pin,u8 mode){
    //PA0 -> 0*16 + 0 =0
    //PB7 -> 1*16 + 7 = 23
    u8 pin_num = pin % 16;
    u8 port = pin /16;
    GPIO_CONFIG GPIO_CFG={(GPIO_ENUM)(port),pin_num,mode};
    GPIO_init(GPIO_CFG);
}
void digitalWrite(u8 pin , u8 output){
    u8 pin_num = pin % 16;
    u8 port = pin /16;
    GPIO_void_digital_Write_ATOMIC((GPIO_ENUM)port,pin_num,output);

}

u8 digitalRead(u8 pin){
    u8 pin_num = pin % 16;
    u8 port = pin /16;
    return GPIO_void_digital_Read((GPIO_ENUM)port,pin_num);
}

