#ifndef _STM32F407VG_H_
#define _STM32F407VG_H_

#include "stdint.h"


// systick registers
#define SYSTICK_RVR         (uint32_t *)0xE000E014U   // reload value reg
#define SYSTICK_CSR         (uint32_t *)0xE000E010U   // control & status reg
#define SYSTICK_CVR         (uint32_t *)0xE000E018U   // 
#define SYSTICK_CALIB       (uint32_t *)0xE000E01CU
#define TIMEOUT             0x0000FFFFUL


// (RCC) reset and clock control registers
#define RCC_BASE_ADDR       (uint32_t *)0x40023800
typedef struct rcc
{
    volatile uint32_t * RCC_CR;
};



#define RCC_CR              (RCC_BASE_ADDR + (uint32_t *)0x00)
#define RCC_PLLCFGR         (RCC_BASE_ADDR + (uint32_t *)0x04)
#define RCC_CFGR            (RCC_BASE_ADDR + (uint32_t *)0x08)









#endif