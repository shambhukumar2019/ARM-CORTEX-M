/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Shambhu Kumar
 * @brief          : Main program body
 * @date           : 25/07/2024
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

int main(void)
{
	volatile uint32_t cnt = 0;	// for use in delay loop
	RCC->AHB1ENR = 0x00000008; // enable AHB1 bus clock connecte to GPIOD
  	GPIOD->MODER = 0x01000000;	// set GPIOD pin 12 as o/p
    /* Loop forever */
	for(;;)
	{
		cnt = 5;
		cnt = 4;
		for(cnt = 2000000;cnt>0;cnt--);
		GPIOD->ODR ^= (1U<<12);	// toggle GPIOD pin 12
	}

	return 0;
}
