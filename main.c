#include "exception_handler.h"

volatile uint32_t glob_var = 5;

void main(void)
{
    volatile static uint32_t loc_var = 0;
    // define task control blocks and other data structures

    // create tasks

    // start scheduler
        //systick_init();
        // system call to init scheduler

    // Main application code goes here
    while (1)
    {
        // Your application logic
    }
    // This point should never be reached
}

