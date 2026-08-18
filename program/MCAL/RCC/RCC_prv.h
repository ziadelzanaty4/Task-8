#ifndef RCC_PRV_H
#define RCC_PRV_H

#include "../../lib/STD_TYPES.h"
#include "../../LIB/STM32F401xC_HEADER.h"

typedef struct RCC_regs_struct
{
   volatile u32 RCC_CR;
   volatile u32 RCC_PLLCFGR;
   volatile u32 RCC_CFGR;
   volatile u32 RCC_CIR;
   volatile u32 RCC_AHB1RSTR;
   volatile u32 RCC_AHB2RSTR;
   volatile u32 reserved_1;
   volatile u32 reserved_2;
   volatile u32 RCC_APB1RSTR;
   volatile u32 RCC_APB2RSTR;
   volatile u32 reserved_3;
   volatile u32 reserved_4;
   volatile u32 RCC_AHB1ENR;
   volatile u32 RCC_AHB2ENR;
   volatile u32 reserved_5;
   volatile u32 reserved_6;
   volatile u32 RCC_APB1ENR;
   volatile u32 RCC_APB2ENR;
   volatile u32 reserved_7;
   volatile u32 reserved_8;
   volatile u32 RCC_AHB1LPENR;
   volatile u32 RCC_AHB2LPENR;
   volatile u32 reserved_9;
   volatile u32 reserved_10;
   volatile u32 RCC_APB1LPENR;
   volatile u32 RCC_APB2LPENR;
   volatile u32 reserved_11;
   volatile u32 reserved_12;
   volatile u32 RCC_BDCR;
   volatile u32 RCC_CSR;
   volatile u32 reserved_13;
   volatile u32 reserved_14;
   volatile u32 RCC_SSCGR;
   volatile u32 RCC_PLLI2SCFGR;
   volatile u32 reserved_15;
   volatile u32 RCC_DCKCFGR;
    
}RCC_TYPEDEF;


#define RCC_STRUCT     ((RCC_TYPEDEF*)(RCC_BASE_ADDR))








#endif