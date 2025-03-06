/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Shambhu Kumar
 * @brief          : Main program body
 * @date           : 31/07/2024
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 CodingBytes.
 * All rights reserved.
 *
 ******************************************************************************
 */


#include <stm32f407xx.h>

/*
#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif
*/

static volatile uint8_t flag = 0;

int main(void)
{
	volatile uint8_t counter = 0;	// will be used for debug

	RCC->AHB1ENR = 0x00000008;	// enable GPIOD clk
	GPIOD->MODER = 0x01000000;	// make GPIOD green pin o/p

	RCC->AHB1ENR |= (1<<0);	// enable GPIOA clock

	GPIOA->MODER &= ~(3<<0);	// GPIOA pin0 input mode
	GPIOA->PUPDR |= (1<<0);	// GPIOA pin0 pull-up enable

	RCC->APB2ENR |= (1<<14);	// enable syscfg control clock

	SYSCFG->EXTICR[0] &= ~(0xF<<0);	// configure GPIOA pin0 as external interrupt source

	EXTI->IMR |= (1<<0);	// enable external interrupt request from GPIOA pin0
	EXTI->RTSR |= (1<<0);	// enable rising edge trigger on GPIOA pin0
	EXTI->FTSR &= ~(1<<0);	// disable falling edge trigger

	NVIC_SetPriority(EXTI0_IRQn,1);	// set EXTI0 priority to 1
	NVIC_EnableIRQ(EXTI0_IRQn);	// enable exti0 interrupt



	for(;;)
	{
		if(flag == 1)
		{
			flag = 0;
			counter += 1;	// inreament counter at each Rising clk on GPIOA pin0

			GPIOD->ODR ^= (1U<<12);	// toggle the pin o/p
		}
	}

	return 0;
}

/// @brief EXTI0 IRQ Handler function
/// @param  void
void EXTI0_IRQHandler(void)
{
	// when interrupt occurs on respected pin, Pending request
	// bit in EXTI controller register EXTI->PR
	if((EXTI->PR & 0x1) == 1)
	{
		EXTI->PR |= (1<<0);	// write 1 to clear this bit
		flag = 1;
	}

}
