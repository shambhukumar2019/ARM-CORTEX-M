## This branch is for practice of cortex-m3 on qemu emulator.


*$* Implemented a basic schedular to switch between three tasks depending on systick interrupt.
> Thanks to **Piyush Itankar** Sir and his great **Team** at [Pyjamabrah](https://pyjamabrah.com/) and [Embedded Systems, inPyjama](https://inpyjama.com/),  for providing great lectures on ARM Cortex-M


## Basic Concepts

#### __ attribute __((naked)) function attribute

This attribute tells the compiler that the function is an embedded assembly function. You can write the body of the function entirely in assembly code using __asm statements.

The compiler does not generate prologue and epilogue sequences for functions with __ attribute __((naked)).

The compiler only supports basic __asm statements in __ attribute __((naked)) functions. Using extended assembly, parameter references or mixing C code with __asm statements might not work reliably.