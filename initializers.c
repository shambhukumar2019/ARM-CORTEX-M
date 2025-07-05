#include "initializers.h"


void sys_clk_init(void)
{
    // enable hsi
    RCC->CR |= RCC_CR_HSI_ON;

    // wait till hsi ready
    while (!(RCC->CR & RCC_CR_HSI_RDY));
    
    // pll as sysclk
    RCC->CFGR &= (~RCC_CFGR_SW0); // clear SW0 bit
    RCC->CFGR |= RCC_CFGR_SW1; // set SW1 bit

    // check if pll is selected as sysclk src
    volatile uint32_t sws_status = 0;

    sws_status = (RCC->CFGR & (RCC_CFGR_SWS0 | RCC_CFGR_SWS1));

    // Clock from PREDIV1 in RCC_CFGR2 reg. selected as the PLL input clock
    RCC->CFGR |= RCC_CFGR_PLLSRC;

    
    
}


void systick_init(void)
{

}






