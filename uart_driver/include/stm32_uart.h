/**
 * @file stm32_uart.h
 * @author shambhu kumar
 * @brief uart header file
 * @version 0.1
 * @date 2025-04-14
 * 
 * @copyright Copyright (c) 2025
 * 
 */


#ifndef STM32_UART_H
#define STM32_UART_H


#include "stm32f407xx.h"


#define MAX_BUF_SIZE   25
#define DISABLE         0
#define Enable          1
#define BAUD_RATE       9600
#define FRAME_SIZE      8
#define STOP_BIT_SIZE   1
#define PARITY_BIT      DISABLE


typedef struct rs232
{
    char rx_buf[MAX_BUF_SIZE + 1];
    char tx_buf[MAX_BUF_SIZE + 1];
    uint8_t rx_buf_char_num;
    uint8_t tx_buf_char_num;

}rs232_obj;








#endif
