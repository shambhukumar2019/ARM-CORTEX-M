/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Shambhu Kumar
 * @brief          : I2C TestCode 
 * @date           : 11/08/2024
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 CodingBytes.
 * All rights reserved.
 *
 ******************************************************************************
 */

#include "stm32f407vgt_i2c.h"




/*
#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif
*/




/// @brief just waste few million clk cycles for generating delay
/// @param  void
void temp_delay(void)
{
	volatile uint32_t j = 0;

	for(j = 0;j < 2000000;j++)
	{
		__asm("nop");	//inline assembly
	}
}




							/***************  main driver code  ****************/

volatile uint8_t byte[8] = {0};	// store received data

int main(void)
{
	i2c1_gpio_config();
	i2c1_config();
	i2c1_enable();

	//infinte loop
	for(;;)
	{
		i2c1_master(RX,8,7,byte);
		i2c1_master(TX,8,7,byte);
		
		temp_delay();
	}

	return 0;
}

