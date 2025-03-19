.syntax unified
.thumb

.global reset_handler

@ .equ SYSTICK_CSR,   0xE000E010
@ .equ SYSTICK_RVR,   0xE000E014
@ .equ SYSTICK_CVR,   0xE000E018
@ .equ SYSTICK_CALIB, 0xE000E01C @read only


.section .isr_vector
vector_list:
    .word   0x20000100
    .word   reset_handler

.org 0x0000003C
    .word systick_handler

    .zero   64


.section .text
.align  1
.type   reset_handler, %function
reset_handler:
    @ mov only works for data upto 255
    ldr r0, =0xE000E010 @ SYSTICK_CSR
    ldr r1, =0xE000E014 @ SYSTICK_RVR
    ldr r2, =0xE000E018 @ SYSTICK_CVR
    ldr r3, =0x00ffffff @ timeout

    str r3, [r1] @ set timeout in RVR reg.
    mov r3, #0
    str r3, [r2] @ set initial tick value in CVR reg.
    mov r3, #7
    str r3, [r0] @ enable systick time in CSR reg.

    b .


.section .text
.align  1
.type   systick_handler, %function
systick_handler:
    mov r0, #1;
    mov r1, #1;
    mov r2, #1;
    mov r3, #1;

    @ LR is set to a special value (0xFFFFFFF9), tells the cpu that this is an exception return
    bx lr



