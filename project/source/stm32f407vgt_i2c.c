/**
 * @file stm32f407vgt_i2c.c
 * @author shambhu kumar
 * @brief stm32f407vgtx I2C device driver source code
 * @version 0.1
 * @date 2025-03-12
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#include "stm32f407vgt_i2c.h"

// store garbage values
static volatile uint16_t dump_reg = 0;

 					/************** Configure GPIO and I2C controls ************/

/**
 * @brief confugure gpio settings for I2C1
 * 
 */
void i2c1_gpio_config(void)
{
	RCC->AHB1ENR |= (1<<1);	//enable GPIOB clk
	GPIOB->MODER |= (1<<13) | (1<<15);	//bit 12, bit 14 = 0, set PB6 and PB7 for alternate function mode
	GPIOB->PUPDR |= (1<<12) | (1<<14);	//bit 13, bit 15 = 0, pullup enable for PB6 and PB7
	GPIOB->OTYPER |= (1<<6) | (1<<7);	//open drain both I2C1_SDA (PB7), I2C1_SCL (PB6)
	GPIOB->AFR[0] |= (1<<26) | (1<<30);	//for PB6, PB7 set AF4 (I2C1)

}


/**
 * @brief configure I2C1 control and clock settings
 * 
 */
void i2c1_config(void)
{
	I2C1->CR1 |= (1<<15);		//i2c under software reset
	__NOP();					//no operation
	I2C1->CR1 &= ~(1<<15);		//i2c out of reset condition
	RCC->APB1ENR |= (1<<21);	//enable i2c1 clk
	I2C1->CR2 |= (1<<4);		//FREQ= 0b010000 = 16 MHz for i2c1 peripheral

	// by default standard mode (100 KHz) selected
	I2C1->CCR |= (1<<4) | (1<<6);	//set i2c1_scl frequency = 100 KHz
	I2C1->TRISE = 0;				// clear TRISE reg
	I2C1->TRISE |= (1<<0) | (1<<4);	//set scl rise time to 1000ns
}


/**
 * @brief enable I2C1 peripheral
 * 
 */
void i2c1_enable(void)
{
	I2C1->CR1 |= (1<<0);	//enable i2c peripheral (set PE bit)
}


/**
 * @brief disable I2C1 peripheral
 * 
 */
void i2c1_disable(void)
{
	I2C1->CR1 &= ~(1<<0);	//disable i2c peripheral (clear PE bit)
}




 					/**************  I2C Master Mode Start **************/

/**
 * @brief master send start bit to slave
 * 
 */
void i2c1_master_send_start(void)
{
	I2C1->CR1 &= ~(1<<11);	// clear POS bit
	I2C1->CR1 |= (1<<10);	//enable acknowledgement bit (ACK)
	I2C1->CR1 |= (1<<8);	//send start condition
	while((I2C1->SR1 & (1<<0)) == 0);	//wait until start bit generated
	dump_reg = I2C1->SR1;				//read SR1 reg to clear SB
}


/**
 * @brief master send slave address in read/write mode
 * 
 * @param slave_addr slave address to connect
 * @param rw_bit master READ or WRITE mode
 */
void i2c1_master_tx_slave_addr(uint8_t slave_addr, uint8_t rw_bit)
{
	// LSB must be 0 for write operation and 1 for read
	I2C1->DR = ((slave_addr << 1) | rw_bit);	//send slave address to SDA line, this step also requied to clear SB

	while((I2C1->SR1 & (1<<1)) == 0);		//wait until ADDR bit set (sets when @ sent)
	if(rw_bit == WRITE)
	{
		dump_reg = I2C1->SR1;				//read SR1 reg to clear ADDR bit
		dump_reg = I2C1->SR2;				//read SR2 reg to clear ADDR bit
	}
}




 						/************** I2C MASTER Transmitter MODE ************/

/**
 * @brief master send byte to slave
 * 
 * @param data byte to be sent
 */
void i2c1_master_tx_data(uint8_t data)
{
	// data shift reg not empty
	while((I2C1->SR1 & (1<<7)) == 0);	//wait until TxE bit sets, data reg empty

	I2C1->DR = data;	//data to send, clears TxE bit

	
}


/**
 * @brief master transmitter stop the communication with slave
 * 
 */
void i2c1_master_tx_stop(void)
{
	while(((I2C1->SR1 & (1<<7)) && (I2C1->SR1 & (1<<2))) == 0);	//wait until TxE and BTF bit sets

	I2C1->CR1 |= (1<<9);				// send stop condition
	while((I2C1->SR2 & (1<<1)) == 0);	//wait until BUSY bit clears, indicates communication terminated
}




 						/************** I2C Master Receiver Mode Start **************/

/**
 * @brief master receive data of given size from slave
 * 
 * @param size no of bytes to receive
 * @param data[] store received bytes in this array
 */
void i2c1_master_rx_data(uint8_t size, uint8_t *data)
{
	volatile uint8_t i = 0;

	if(size == 1)
	{
		I2C1->CR1 &= ~(1<<10);	// clear ACK bit 

		dump_reg = I2C1->SR1;	//read SR1 reg to clear ADDR bit
		dump_reg = I2C1->SR2;	//read SR2 reg to clear ADDR bit

		while((I2C1->SR1 & (1<<6)) == 0);	// wait until RxNE sets
		data[0] = I2C1->DR;

		I2C1->CR1 |= (1<<9);	// send stop condition

		I2C1->CR1 |= (1<<10);	// set ACK bit
	}
	else if(size == 2)
	{
		I2C1->CR1 &= ~(1<<10);	// clear ACK bit 
		I2C1->CR1 |= (1<<11);	// set POS bit

		dump_reg = I2C1->SR1;	//read SR1 reg to clear ADDR bit
		dump_reg = I2C1->SR2;	//read SR2 reg to clear ADDR bit

		while(((I2C1->SR1 & (1<<6)) && (I2C1->SR1 & (1<<2))) == 0);	// wait until RxNE and BTF sets
		I2C1->CR1 |= (1<<9);	// send stop condition

		data[1] = I2C1->DR;
		data[0] = I2C1->DR;

		I2C1->CR1 |= (1<<10);	// set ACK bit
		I2C1->CR1 &= ~(1<<11);	// clear POS bit
	}
	else if (size > 2)
	{
		dump_reg = I2C1->SR1;	//read SR1 reg to clear ADDR bit
		dump_reg = I2C1->SR2;	//read SR2 reg to clear ADDR bit

		// read first size-4 bytes
		for(i = 0; i < size-3; i += 1)
		{
			while((I2C1->SR1 & (1<<6)) == 0);	// wait until RxNE sets
			data[i] = I2C1->DR;
		}

		while((I2C1->SR1 & (1<<2)) == 0);	// wait until BTF sets
		I2C1->CR1 &= ~(1<<10);				// clear ACK bit 

		data[i++] = I2C1->DR;	// read third last byte

		while((I2C1->SR1 & (1<<2)) == 0);	// wait until BTF sets
		I2C1->CR1 |= (1<<9);				// send stop condition

		data[i++] = I2C1->DR; 	// read second last byte
		data[i] = I2C1->DR; 	// read last byte

		I2C1->CR1 |= (1<<10);	// set ACK bit
	}

	while((I2C1->SR2 & (1<<1)) == 0);	//wait until BUSY bit clears, indicates communication terminated
}



 									/************* I2C SLAVE MODE Start **************/

/**
 * @brief configure I2C1 as slave and set slave address 
 * 
 * @param addr address of slave
 */
void i2c1_slave_config(uint8_t addr)
{
	I2C1->OAR1 = ((addr << 1) | (1<<14));	//slave @ = 0x08, SET SLAVE ADDRESS
	I2C1->CR1 &= ~(1<<11);	//clear POS bit	
	I2C1->CR1 |= (1<<10);	//enable acknowledgement bit (ACK)
}

/**
 * @brief match the received slave address from master
 * 
 */
void i2c1_slave_rx_address(void)
{
	
	while((I2C1->SR1 & (1<<1)) == 0);	//wait until ADDR bit set (sets when @ received)
	dump_reg = I2C1->SR1;	//read SR1 reg to clear ADDR bit
	dump_reg = I2C1->SR2;	//read SR2 reg to clear ADDR bit

}


					/************* I2C SLAVE Receiver MODE Start **************/

/**
 * @brief read the received data byte from master
 * 
 * @return uint8_t received byte
 */
uint8_t i2c1_slave_rx_data(void)
{
	volatile uint8_t data = 0;

	while((I2C1->SR1 & (1<<6)) == 0);	//wait until RxNE bit sets, data shift reg empty
	data = I2C1->DR;

	return data;
}



					/************* I2C SLAVE Transmitter MODE Start **************/

/**
 * @brief send a byte to master
 * 
 * @param data byte to be sent
 */
void i2c1_slave_tx_data(uint8_t data)
{
	while((I2C1->SR1 & (1<<7)) == 0);	// wait until TxE in SR1 reg sets
	I2C1->DR = data;
}


/**
 * @brief stop slave from communicating with master
 * 
 *      master sends stop bit which is detected by the slave 
 *      then it ends the connection
 * 
 */
void i2c1_slave_stop(void)
{
	if (((I2C1->SR1) & (1<<10)) == 1UL)
	{
		I2C1->SR1 &= ~(1<<10);	// clear AF (acknowledge failure) bit
	}

	while((I2C1->SR1 & (1<<4)) == 0);	//wait until STOPF bit sets
	dump_reg = I2C1->SR1;	//read SR1 reg
	
	if((I2C1->SR1 & (1<<1)) == 1UL)
	{
		dump_reg = I2C1->SR1;	//read SR1 reg to clear ADDR bit
		dump_reg = I2C1->SR2;	//read SR2 reg to clear ADDR bit
	}
	if((I2C1->SR1 & (1<<4)) == 1UL)
	{
		dump_reg = I2C1->SR1;	//read SR1 to clear STOPF bit
		I2C1->CR1 = 0;	//write CR1 to clear STOPF bit
	}

	while((I2C1->SR2 & (1<<1)) == 0);	//wait until BUSY bit clears, indicates communication terminated
}

/**
 * @brief master send chunk of bytes to slave
 * 
 * @param slave_addr address of the slave to connect
 * @param len no of bytes to send
 * @param buf[] buffer from which bytes would be sent
 */
void i2c1_master_transmit(uint8_t slave_addr, uint8_t len, uint8_t *buf)
{
	volatile uint8_t i = 0;

	i2c1_master_send_start();
	i2c1_master_tx_slave_addr(slave_addr, WRITE);

	for(i=0;i<len;i++)
	{
		i2c1_master_tx_data(buf[i]);
	}
	i2c1_master_tx_stop();
}

/**
 * @brief master receive data from slave
 * 
 * @param slave_addr slave address from which data to be received
 * @param len no of bytes to receive
 * @param buf buffer in which to store received bytes
 */
void i2c1_master_receive(uint8_t slave_addr, uint8_t len, uint8_t *buf)
{
	i2c1_master_send_start();
	i2c1_master_tx_slave_addr(slave_addr, READ);
	i2c1_master_rx_data(len,buf);
}

/**
 * @brief I2C master rx/tx bytes to slave
 * 
 * 		wrapper function for master rx/tx mode
 * 
 * @param tx_rx_mode set TX or RX mode 
 * @param slave_add slave address
 * @param len no of bytes to rx/tx
 * @param buf buffer of bytes
 */
void i2c1_master(uint8_t tx_rx_mode, uint8_t slave_add, uint8_t len, uint8_t *buf)
{
	if(tx_rx_mode == TX)
	{
		i2c1_master_transmit(slave_add,len,buf);
	}
	else if (tx_rx_mode == RX)
	{
		i2c1_master_receive(slave_add,len,buf);
	}
}
