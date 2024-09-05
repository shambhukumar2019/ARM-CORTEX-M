# $$ STEPS TO CONFIGURE STM32F407VGT6 uC I2C PERIPHERAL 


### => Steps to configure GPIO for I2C

	1. Configure respective GPIO clock in RCC AHB1ENR.
	2. set respective GPIO for alternate mode in MODER reg .
	3. set pull-up on SDA and SCA pins, PUPDR reg.
	4. set open drain on both pins, OTYPER reg.
	5. set alternate func reg. for I2C,set AFRL to AF4.
	6. output speed in low speed mode (reset condition), OSPEEDR.
	
> ***************************************************************************************************
	
### => Configure I2C Controls

	1. reset then unreset I2C peripheral (SWRST in CR1 reg), so that initially I2C is in known state.
	2. configure respective I2C peripheral clock in RCC APB1ENR.
	3. set FREQ[0:5] bits in CR2 reg with same frequency as of APB1, 16MHz default HSI = APB1.
	4. select Sm/Fm mode in CCR reg, by default in Sm.
	5. configure CCR reg. value in I2C_CCR for SM or FM I2C, configure when i2c disabled only.
		example: In Sm mode, f= 100kHz, Ton=Toff= 0.5 us
					APB1 =>  f1 = 16MHz, t = 0.0625 us
				CCR value = 16MHz / (2 * 100KHz) => 80 = 0b01010000 = 0x50
	6. set max. rise time of SCL in TRISE reg, 
			Trise = {1000ns (Trscl) / 62.5ns (Tapb1) } + 1
					= 17 = 0b00010001 = 0x11
	7. enable I2C peripheral in CR1 reg.
	
> ****************************************************************************************

# I2C Master Mode

### => Start Condition

> reset POS bit in CR1. use in master rx mode for more than 2 byte only.

	1. enable ACK bit in CR1, return ACK if a byte received.
	2. set start bit in CR1 to generate start condition (generates high to low pulse).
	3. wait until SB bit in SR1 sets.
	4. read SR1 reg. to clear SB bit in SR1.

> **********************************************************************************

## $ Transmit Data

#### => Send slave address

	1. write slave address to DR reg.
	2. wait until ADDR bit in SR1 reg. sets.
			LSB = 0 for master transmitter mode
	3. read SR1 and SR2 reg. to clear ADDR bit.
	4. wait for TxE bit in SR1 reg to set.


#### => Send data

	1. Now put data in DR reg of I2C.
	2. wait for TxE bit in SR1 reg to set.
	3. repeat from step 1 to send multiple bytes.


#### => End I2C communication

	1. after sending last byte, wait until both TxE and BTF in SR1 sets.
	2. set STOP bit in CR1 to end I2C communication.
	3. wait until BUSY bit in SR2 clears, indicates communication terminated.


> ***********************************************************************************

## $ Receive Data

#### => Send slave address

	1. write slave address to DR reg.
	2. wait until ADDR bit in SR1 reg. sets.
			LSB = 1 for master receiver mode
		

#### => receive 1 byte data

	1. clear ACK bit in CR1.
	2. read SR1 and SR2 reg.
	3. wait until RxNE and BTF bit in SR1 sets.
	4. set STOP bit in SR1.
	5. read data from DR.


#### => receive 2 byte data

	1. clear ACK bit in CR1.
	2. read SR1 and SR2 reg.
	3. Wait until RxNE and BTF = 1 (Data 1 in DR, Data2 in shift register, SCL stretched low until data1 is read).
	4. Set STOP high.
	5. Read DR two times for data 1 and 2 continously.


#### => receive more than 2 byte data

	1. before reading 3rd data from last, clear ACK bit.
	2. read data(N-2)
	3. wait until BTF sets.
	4. set STOP bit.
	5. read data two times (N-1), N.

> In general, before reading each data wait until BTF sets. $$$

> ****************************************************************************************

# I2C Slave

## Slave Receiver

### => Slave Rx mode Initialization

> reset POS bit in CR1. use in master rx mode for more than 2 byte only.

	1. enable ACK bit in CR1, return ACK if a byte received.
	2. set slave address in OAR1.
	3. wait until ADDR bit in SR1 reg. sets.
	4. read SR1 and SR2 reg.
	5. read received data from DR .
	6. wait until RxNE bit in SR1 reg sets.
	7. read SR1 reg.
	8. wait until STOPF bit in SR1 sets.
	9. if ADDR bit in SR1 set, read SR1 then SR2 reg.
	10. if STOPF bit in SR1 set, read SR1 then write CR1 reg.



