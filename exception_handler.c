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
#include "initializers.h"




/*
    BL (Branch with Link) instruction in the ARM Cortex-M4 
    is used to call a subroutine or function. It works by 
    branching to a specified target address and simultaneously 
    saving the current program counter (PC) value in the 
    link register (LR). This allows the program to return to 
    the correct location after the subroutine has completed 
    execution.
*/
/*
    // Initialize system clock
    sys_clk_init();
    init psp, msp
    exception priorities
    define idle task
    task queue
    set initial context of tasks with return @ to it's entry func
*/
/*  _ASM _VO ("\
    ldr sp, =_estack;\
    ldr r0, =_sdata_flash;\
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
*/

_ATTR((naked, section(".text.reset_handler"))) void reset_handler(void)
{
    // volatile uint32_t current_msp = 0;
    // volatile uint32_t *new_msp = (uint32_t *)&_estack;
    uint32_t *flash_data_start = (uint32_t *)&_sdata_flash;
    uint32_t *ram_data_start = (uint32_t *)&_sdata;
    uint32_t *ram_data_end = (uint32_t *)&_edata;
    uint32_t *bss_start = (uint32_t *)&_sbss;
    uint32_t *bss_end = (uint32_t *)&_ebss;

    extern uint32_t _estack; // Top of the stack
    extern uint32_t _sdata_flash; // Start of initialized data in flash
    extern uint32_t _sdata; // Start of initialized data in RAM
    extern uint32_t _edata; // End of initialized data in RAM
    extern uint32_t _sbss; // Start of uninitialized data (BSS) in RAM
    extern uint32_t _ebss; // End of uninitialized data (BSS) in RAM

    // __asm volatile("mrs %0, msp" : "=r" (current_msp));

    // Set the Main Stack Pointer (MSP) to the top of the stack
    __asm volatile("msr msp, %0" : : "r" (&_estack) : "memory");

    // Initialize the Process Stack Pointer (PSP) to the top of the stack
    __asm volatile("msr psp, %0" : : "r" (&_estack) : "memory");
    

    // copy initialised variable from flash to sram
    // This section is used for initialized global and static variables
    while (ram_data_start != ram_data_end)
    {
        *ram_data_start = *flash_data_start;
        ram_data_start += 1;
        flash_data_start += 1;
    }
    
    // Zero initialize the BSS section
    // This section is used for uninitialized global and static variables
    while (bss_start != bss_end)
    {
        *bss_start = 0;
        bss_start += 1;
    }
    
    // Initialize the system clock
    sys_clk_init();
    
    // Initialize the systick timer
    systick_init();
    
    // Call the main function
    // This is the entry point of the application
    main();

    // If main returns, enter an infinite loop
    while (1)
    {
        // Optionally, you can add a breakpoint here for debugging
        // or simply halt the system.
    }
}


_ATTR((naked)) void systick_handler(void)
{
    // handle time counter
    // trigger pendsv 
}


_ATTR((naked)) void pendsv_handler(void)
{
    // check whether psp or msp was in use
    // context switch
    // update psp to newly loaded task
}


_ATTR((naked)) void svc_handler(void)
{
    // request kernel services such as task delays,
    // inter-task communication, and resource management
}


/*
    during task creation stack is allocated and psp is set
    to point this stack, this is how user code is forced to use psp
    when exception occurs cpu auto switches to msp
*/






