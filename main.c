#include "exception_handler.h"


_ATTR((noreturn)) void main(void)
{
    // define task control blocks and other data structures

    // create tasks

    // start scheduler
        systick_init();
        // system call to init scheduler

    // Main application code goes here
    while (1)
    {
        // Your application logic
    }
    // This point should never be reached
}

