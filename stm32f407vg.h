
/********  write now it is for cortex m3, stm32f100rb  *********/


#ifndef _STM32F407VG_H_
#define _STM32F407VG_H_


#include "stdint.h"



// peripherals base address
#define RCC_BASE_ADDR       0x40021000UL
#define SYSTICK_BASE_ADDR   0xE000E010UL


typedef struct rcc_t
{
    volatile uint32_t CR;
    volatile uint32_t CFGR;
    volatile uint32_t CIR;
}rcc_t;

typedef struct systick_t
{
    volatile uint32_t CSR;
    volatile uint32_t RVR;
    volatile uint32_t CVR;
    volatile uint32_t CALIB;
}systick_t;


#define RCC                     ((rcc_t *)RCC_BASE_ADDR)
#define SYSTICK                 ((systick_t *)SYSTICK_BASE_ADDR)


#define RCC_CR_HSI_ON           (0x1UL << 0U)
#define RCC_CR_HSI_RDY          (0x1UL << 1U)
#define RCC_CR_PLL_ON           (0x1UL << 24U)
#define RCC_CR_PLL_RDY          (0x1UL << 25U)

#define RCC_CFGR_SW0            (0x1UL << 0U)
#define RCC_CFGR_SW1            (0x1UL << 1U)
#define RCC_CFGR_SWS0           (0x1UL << 2U)
#define RCC_CFGR_SWS1           (0x1UL << 3U)
#define RCC_CFGR_HPRE0          (0x1UL << 4U)
#define RCC_CFGR_HPRE1          (0x1UL << 5U)
#define RCC_CFGR_HPRE2          (0x1UL << 6U)
#define RCC_CFGR_HPRE3          (0x1UL << 7U)
#define RCC_CFGR_PLLSRC         (0x1UL << 16U)
#define RCC_CFGR_PLLMUL0        (0x1UL << 18U)
#define RCC_CFGR_PLLMUL1        (0x1UL << 19U)
#define RCC_CFGR_PLLMUL2        (0x1UL << 20U)
#define RCC_CFGR_PLLMUL3        (0x1UL << 21U)












#endif