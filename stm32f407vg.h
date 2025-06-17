#ifndef _STM32F407VG_H_
#define _STM32F407VG_H_

#include "stdint.h"

/********  write now it is for cortex m3, stm32f100rb  *********/


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


#define RCC                 ((rcc_t *)RCC_BASE_ADDR)
#define SYSTICK             ((systick_t *)SYSTICK_BASE_ADDR)








#endif