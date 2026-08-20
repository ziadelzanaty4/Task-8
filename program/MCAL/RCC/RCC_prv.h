#ifndef RCC_PRV_H
#define RCC_PRV_H

#include "../../lib/STD_TYPES.h"
#include "../../LIB/STM32F401xC_HEADER.h"

#include "RCC_CFG.h"


/*===========================================================
 * RCC Register Structure
 *
 * Base address:
 * 0x40023800
 *===========================================================*/

typedef struct RCC_regs_struct
{
    volatile u32 RCC_CR;              /* 0x00 */
    volatile u32 RCC_PLLCFGR;         /* 0x04 */
    volatile u32 RCC_CFGR;            /* 0x08 */
    volatile u32 RCC_CIR;             /* 0x0C */

    volatile u32 RCC_AHB1RSTR;        /* 0x10 */
    volatile u32 RCC_AHB2RSTR;        /* 0x14 */

    volatile u32 reserved_1;          /* 0x18 */
    volatile u32 reserved_2;          /* 0x1C */

    volatile u32 RCC_APB1RSTR;        /* 0x20 */
    volatile u32 RCC_APB2RSTR;        /* 0x24 */

    volatile u32 reserved_3;          /* 0x28 */
    volatile u32 reserved_4;          /* 0x2C */

    volatile u32 RCC_AHB1ENR;         /* 0x30 */
    volatile u32 RCC_AHB2ENR;         /* 0x34 */

    volatile u32 reserved_5;          /* 0x38 */
    volatile u32 reserved_6;          /* 0x3C */

    volatile u32 RCC_APB1ENR;         /* 0x40 */
    volatile u32 RCC_APB2ENR;         /* 0x44 */

    volatile u32 reserved_7;          /* 0x48 */
    volatile u32 reserved_8;          /* 0x4C */

    volatile u32 RCC_AHB1LPENR;       /* 0x50 */
    volatile u32 RCC_AHB2LPENR;       /* 0x54 */

    volatile u32 reserved_9;          /* 0x58 */
    volatile u32 reserved_10;         /* 0x5C */

    volatile u32 RCC_APB1LPENR;       /* 0x60 */
    volatile u32 RCC_APB2LPENR;       /* 0x64 */

    volatile u32 reserved_11;         /* 0x68 */
    volatile u32 reserved_12;         /* 0x6C */

    volatile u32 RCC_BDCR;            /* 0x70 */
    volatile u32 RCC_CSR;             /* 0x74 */

    volatile u32 reserved_13;         /* 0x78 */
    volatile u32 reserved_14;         /* 0x7C */

    volatile u32 RCC_SSCGR;           /* 0x80 */
    volatile u32 RCC_PLLI2SCFGR;      /* 0x84 */

    volatile u32 reserved_15;         /* 0x88 */

    volatile u32 RCC_DCKCFGR;         /* 0x8C */

} RCC_TYPEDEF;


/*===========================================================
 * RCC instance
 *===========================================================*/

#define RCC_STRUCT    ((RCC_TYPEDEF *)RCC_BASE_ADDR)


/*===========================================================
 * FLASH
 *===========================================================*/

#define FLASH_ACR_ADDR             0x40023C00UL

#define FLASH_ACR                  (*(volatile u32 *)FLASH_ACR_ADDR)

#define FLASH_ACR_LATENCY_MASK     0x00000007UL
#define FLASH_ACR_PRFTEN           (1UL << 8)


/*===========================================================
 * RCC_CR bits
 *===========================================================*/

#define RCC_CR_HSION               0
#define RCC_CR_HSIRDY              1

#define RCC_CR_HSEON               16
#define RCC_CR_HSERDY              17

#define RCC_CR_PLLON               24
#define RCC_CR_PLLRDY              25


/*===========================================================
 * RCC_PLLCFGR fields
 *===========================================================*/

#define RCC_PLLCFGR_PLLM_SHIFT     0
#define RCC_PLLCFGR_PLLM_MASK      (0x3FUL << 0)

#define RCC_PLLCFGR_PLLN_SHIFT     6
#define RCC_PLLCFGR_PLLN_MASK      (0x1FFUL << 6)

#define RCC_PLLCFGR_PLLP_SHIFT     16
#define RCC_PLLCFGR_PLLP_MASK      (0x3UL << 16)

#define RCC_PLLCFGR_PLLSRC         22


/*===========================================================
 * RCC_CFGR fields
 *===========================================================*/

#define RCC_CFGR_SW_SHIFT          0
#define RCC_CFGR_SW_MASK           (0x3UL << 0)

#define RCC_CFGR_SWS_SHIFT         2
#define RCC_CFGR_SWS_MASK          (0x3UL << 2)

#define RCC_CFGR_HPRE_SHIFT        4
#define RCC_CFGR_HPRE_MASK         (0xFUL << 4)

#define RCC_CFGR_PPRE1_SHIFT       10
#define RCC_CFGR_PPRE1_MASK        (0x7UL << 10)

#define RCC_CFGR_PPRE2_SHIFT       13
#define RCC_CFGR_PPRE2_MASK        (0x7UL << 13)


/*===========================================================
 * RCC_CFGR clock switch values
 *===========================================================*/

#define RCC_SW_HSI                 0x0UL
#define RCC_SW_HSE                 0x1UL
#define RCC_SW_PLL                 0x2UL


/*===========================================================
 * Helpers
 *===========================================================*/

#define RCC_MODIFY_FIELD(REG, MASK, SHIFT, VALUE)          \
    do                                                     \
    {                                                      \
        (REG) = ((REG) & ~(MASK)) |                       \
                (((u32)(VALUE) << (SHIFT)) & (MASK));      \
    } while (0)


#endif /* RCC_PRV_H */