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




#define _ATTR    __attribute__
#define _ASM     __asm   
#define _VO      volatile

#define SYSTICK_CSR   0xE000E010U
#define SYSTICK_RVR   0xE000E014U
#define SYSTICK_CVR   0xE000E018U
#define SYSTICK_CALIB 0xE000E01CU
#define TIMEOUT       0x0000FFFFU


extern _ATTR((naked)) _VO void systick_handler(void);
extern _ATTR((naked)) _VO void pendsv_handler(void);
extern _ATTR((naked)) _VO void svc_handler(void);







#endif