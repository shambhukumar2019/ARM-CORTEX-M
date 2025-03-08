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


#include <stm32f407xx.h>
#include <stdio.h>

/*
#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif
*/

volatile uint16_t dump_reg = 0;

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

/// @brief configure gpio for i2c1
/// @param  void
void i2c1_gpio_config(void)
{
	RCC->AHB1ENR |= (1<<1);	//enable GPIOB clk
	GPIOB->MODER |= (1<<13) | (1<<15);	//bit 12, bit 14 = 0, set PB6 and PB7 for alternate function mode
	GPIOB->PUPDR |= (1<<12) | (1<<14);	//bit 13, bit 15 = 0, pullup enable for PB6 and PB7
	GPIOB->OTYPER |= (1<<6) | (1<<7);	//open drain both I2C1_SDA (PB7), I2C1_SCL (PB6)
	GPIOB->AFR[0] |= (1<<26) | (1<<30);	//for PB6, PB7 set AF4 (I2C1)

}

/// @brief configure i2c1 peripheral settings
/// @param  void
void i2c1_config(void)
{
	I2C1->CR1 |= (1<<15);	//i2c under software reset
	__NOP();	//no operation
	I2C1->CR1 &= ~(1<<15);	//i2c out of reset condition
	RCC->APB1ENR |= (1<<21);	//enable i2c1 clk
	I2C1->CR2 |= (1<<4);	//FREQ= 0b010000 = 16 MHz for i2c1 peripheral
	// by default standard mode (100 KHz) selected
	I2C1->CCR |= (1<<4) | (1<<6);	//set i2c1_scl frequency = 100 KHz
	I2C1->TRISE = 0;	// clear TRISE reg
	I2C1->TRISE |= (1<<0) | (1<<4);	//set scl rise time to 1000ns
}

/// @brief enable i2c1 peripheral
/// @param  void
void i2c1_enable(void)
{
	I2C1->CR1 |= (1<<0);	//enable i2c peripheral (set PE bit)
}

/// @brief disable i2c1 peripheral
/// @param  void
void i2c1_disable(void)
{
	I2C1->CR1 &= ~(1<<0);	//disable i2c peripheral (clear PE bit)
}

// ************** I2C MASTER MODE ************

/// @brief transmit start condition to sda line
/// @param  void
void i2c1_master_send_start(void)
{
	I2C1->CR1 &= ~(1<<11);	// clear POS bit
	I2C1->CR1 |= (1<<10);	//enable acknowledgement bit (ACK)
	I2C1->CR1 |= (1<<8);	//send start condition
	while((I2C1->SR1 & (1<<0)) == 0);	//wait until start bit generated
	dump_reg = I2C1->SR1;	//read SR1 reg to clear SB
}

/// @brief transmit slave address @ to for i2c connection with it
/// @param  slave_addr address of the slave to select
void i2c1_master_tx_slave_addr(uint8_t slave_addr)
{
	// LSB must be 0 for write operation
	I2C1->DR = (slave_addr << 1);	//send slave address to SDA line, this step also requied to clear SB
	while((I2C1->SR1 & (1<<1)) == 0);	//wait until ADDR bit set (sets when @ sent)
	dump_reg = I2C1->SR1;	//read SR1 reg to clear ADDR bit
	dump_reg = I2C1->SR2;	//read SR2 reg to clear ADDR bit
	while((I2C1->SR1 & (1<<7)) == 0);	//wait until TxE bit sets, data shift reg empty
}

/// @brief transmit byte size data to sda line
/// @param data byte to be transmitted via i2c1
void i2c1_master_tx_data(uint8_t data)
{
	I2C1->DR = data;	//data to send
	while((I2C1->SR1 & (1<<7)) == 0);	//wait until TxE bit sets, data shift reg empty
}

/// @brief transmit stop condition to sda line to end communication
/// @param  void
void i2c1_master_tx_stop(void)
{
	while((I2C1->SR1 & (1<<7)) && (I2C1->SR1 & (1<<2)) == 0);	//wait until TxE and BTF bit sets
	I2C1->CR1 |= (1<<9);
	while((I2C1->SR2 & (1<<1)) == 0);	//wait until BUSY bit clears, indicates communication terminated
}


// ************* I2C SLAVE MODE Start **************

void i2c1_slave_config(void)
{
	I2C1->OAR1 |= (1<<4) | (1<<14);	//slave @ = 0x08
	I2C1->CR1 &= ~(1<<11);	//clear POS bit	
	
}

void i2c1_slave_rx_address(void)
{
	I2C1->CR1 |= (1<<10);	//enable acknowledgement bit (ACK)
	while((I2C1->SR1 & (1<<1)) == 0);	//wait until ADDR bit set (sets when @ received)
	dump_reg = I2C1->SR1;	//read SR1 reg to clear ADDR bit
	dump_reg = I2C1->SR2;	//read SR2 reg to clear ADDR bit

}

uint8_t i2c1_slave_rx_data(void)
{
	volatile uint8_t data = 0;

	while((I2C1->SR1 & (1<<6)) == 0);	//wait until RxNE bit sets, data shift reg empty
	data = I2C1->DR;

	return data;
}

void i2c1_slave_rx_stop(void)
{
	dump_reg = I2C1->SR1;	//read SR1 reg
	while((I2C1->SR1 & (1<<4)) == 0);	//wait until STOPF bit sets
	if((I2C1->SR1 & (1<<1)) == 1)
	{
		dump_reg = I2C1->SR1;	//read SR1 reg to clear ADDR bit
		dump_reg = I2C1->SR2;	//read SR2 reg to clear ADDR bit
	}
	if((I2C1->SR1 & (1<<4)) == 1)
	{
		dump_reg = I2C1->SR1;	//read SR1 to clear STOPF bit
		I2C1->CR1 = 0;	//write CR1 to clear STOPF bit
	}
}

// ************* I2C SLAVE MODE End **************

volatile uint8_t data = 0;	// store received data

// main driver code, slave rx mode
int main(void)
{
	i2c1_gpio_config();
	i2c1_config();
	i2c1_slave_config();
	i2c1_enable();
	
	// i2c1_disable();

	//eternal loop
	for(;;)
	{
		i2c1_slave_rx_address();
		data = i2c1_slave_rx_data();
		i2c1_slave_rx_stop();
	}

	return 0;
}

