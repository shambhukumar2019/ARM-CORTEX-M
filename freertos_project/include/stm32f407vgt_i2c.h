/**
 * @file stm32f407vgt_i2c.h
 * @author shambhu kumar
 * @brief stm32f407vgtx I2C device driver header file
 * @version 0.1
 * @date 2025-03-12
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#ifndef STM_I2C_H
#define STM_I2C_H

#include "stm32f407xx.h"


#define	TRUE	1U
#define	FALSE	0U

#define	READ	TRUE
#define	WRITE	FALSE

#define	TX		FALSE
#define	RX		TRUE



void i2c1_gpio_config(void);
void i2c1_config(void);
void i2c1_enable(void);
void i2c1_disable(void);
void i2c1_master_send_start(void);
void i2c1_master_tx_slave_addr(uint8_t, uint8_t);
void i2c1_master_tx_data(uint8_t);
void i2c1_master_tx_stop(void);
void i2c1_master_rx_data(uint8_t, uint8_t *);
void i2c1_slave_config(uint8_t);
void i2c1_slave_rx_address(void);
uint8_t i2c1_slave_rx_data(void);
void i2c1_slave_tx_data(uint8_t);
void i2c1_slave_stop(void);
void i2c1_master_transmit(uint8_t, uint8_t, uint8_t *);
void i2c1_master_receive(uint8_t, uint8_t, uint8_t *);
void i2c1_master(uint8_t, uint8_t, uint8_t, uint8_t *);


#endif