/**
 * @file exception_handler.c
 * @author Shambhu Kumar
 * @brief  define exception handler functions
 * @version 0.1
 * @date 2025-04-05
 * 
 * @copyright Copyright (c) 2025 Coding Bytes
 * 
 */


#include "exception_handler.h"


_ATTR((naked)) void reset_handler(void)
{
    _ASM _VO(
    "ldr r0, =SYSTICK_CSR @ SYSTICK_CSR \
    ldr r1, =SYSTICK_RVR @ SYSTICK_RVR  \
    ldr r2, =SYSTICK_CVR @ SYSTICK_CVR  \
    ldr r3, =TIMEOUT @ timeout  \
    \
    str r3, [r1] @ set timeout in RVR reg   \
    mov r3, #0  \
    str r3, [r2] @ set initial tick value in CVR reg.   \
    mov r3, #7\
    str r3, [r0] @ enable systick time in CSR reg.  \
    \
    \
    ldr r4, =stacks \
    mov r5, #4  \
    \
    b main  "\
    );

}

