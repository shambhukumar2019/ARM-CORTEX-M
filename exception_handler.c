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



_ATTR((naked,section(".text.reset_handler"))) void reset_handler(void)
{
    _ASM _VO ("\
        ldr sp, =_estack;\
        ldr r0, =_sidata;\
        ldr r1, =_sdata;\
        ldr r2, =_edata;\
        movs r3, #0;\
        b loopcpydatainit;\
        \
        loopcpydata:\
            ldr r4, [r0, r3];\
            str r4, [r1, r3];\
            adds r3, r3, #4;\
        \
        loopcpydatainit:\
            adds   r4, r1, r3;\
            cmp r4, r2;\
            bcc loopcpydata;\
        \
        \
        ldr r0, =_sbss;\
        ldr r1, =_ebss;\
        movs r2, #0;\
        b loop_fillzero_bss_init;\
        \
        loop_fillzero_bss:\
            str r2, [r0];\
            adds r0, r0, #4;\
        \
        loop_fillzero_bss_init:\
            cmp r0, r1;\
            bcc loop_fillzero_bss;\
        \
        \
        bl main;\
        ");
        /*
             BL (Branch with Link) instruction in the ARM Cortex-M4 
             is used to call a subroutine or function. It works by 
             branching to a specified target address and simultaneously 
             saving the current program counter (PC) value in the 
             link register (LR). This allows the program to return to 
             the correct location after the subroutine has completed 
             execution.
        */
}


_ATTR((naked)) void systick_handler(void)
{
    
}


_ATTR((naked)) void pendsv_handler(void)
{
    
}


_ATTR((naked)) void svc_handler(void)
{
    
}







