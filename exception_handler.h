/**
 * @file exception_handler.h
 * @author Shambhu Kumar
 * @brief header file for exception handlers
 * @version 0.1
 * @date 2025-04-05
 * 
 * @copyright Copyright (c) 2025 Coding Bytes
 * 
 */


#ifndef EXCEPTION_HANDLER_H
#define EXCEPTION_HANDLER_H

#include "stm32f407vg.h"

#define _ATTR    __attribute__




extern _ATTR((naked, section(".text.reset_handler"))) void reset_handler(void);
extern _ATTR((naked)) void systick_handler(void);
extern _ATTR((naked)) void pendsv_handler(void);
extern _ATTR((naked)) void svc_handler(void);


extern void main(void);
extern void sys_clk_init(void);
extern void systick_init(void);


extern uint32_t _estack; // Top of the stack
extern uint32_t _sdata_flash; // Start of initialized data in flash
extern uint32_t _sdata; // Start of initialized data in RAM
extern uint32_t _edata; // End of initialized data in RAM
extern uint32_t _sbss; // Start of uninitialized data (BSS) in RAM
extern uint32_t _ebss; // End of uninitialized data (BSS) in RAM


/*

NOTE: special registers cann't be accessed directly in C/C++.
       We need to use inline assembly to access them.

For example, to read and write the Main Stack Pointer (MSP) register,
we can use the following inline assembly code:
// Declare variables to hold the current and new MSP values
volatile uint32_t current_msp;
volatile uint32_t new_msp = 0x20000000; // Example new value for MSP

// Read current MSP value using MRS instruction
__asm volatile ("MRS %0, MSP" : "=r" (current_msp));

// Write new value to MSP using MSR instruction
__asm volatile ("MSR MSP, %0" : : "r" (new_msp) : "memory");

*/




#endif