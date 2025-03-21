.syntax unified
.thumb

# these variable have global access
.global reset_handler
.global systick_handler
.global task1
.global task2
.global task3
.global stack1
.global stack2
.global stack3

@ .equ SYSTICK_CSR,   0xE000E010
@ .equ SYSTICK_RVR,   0xE000E014
@ .equ SYSTICK_CVR,   0xE000E018
@ .equ SYSTICK_CALIB, 0xE000E01C @read only


# exception vector table
.section .isr_vector
vector_list:

    .word   0x20000800      @ $sp = 0x20000800
    .word   reset_handler

.org 0x0000003C
    .word   systick_handler
    .zero   16


# reset exception handler function
.section .text.reset_handler
.align  2
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

    ldr r4, =stacks
    mov r5, #4

    b .


# systick exception handler function
.section .text.systick_handler
.align  2
.type   systick_handler, %function
systick_handler:

    push {r6-r7}
    mov r0, r8
    mov r1, r9
    mov r2, r10
    mov r3, r11
    push {r0-r3}

    sub r5, r5, #1
    cmp r5, #0
    beq init_sp
 
load_sp:
    ldr sp, [r4]
    add r4, r4, #4
    b break_pnt

init_sp:
    ldr r4, =stacks
    mov r5, #4
    b load_sp
    
# read sp after returning from task1 and with offset to pc write the desired @ there

break_pnt:

    pop {r0-r3}
    mov r8, r0
    mov r9, r1
    mov r10, r2
    mov r11, r3
    pop {r6-r7}

    mrs r0, lr
    msr pc, r0

    @ LR is set to a special value (0xFFFFFFF9), tells the cpu that this is an exception return
    bx lr



# TASK 1
.section .text.task1
.align  2
.type   task1, %function
task1:

    add r0,r0, #1

    b .


# TASK 2
.section .text.task2
.align  2
.type   task2, %function
task2:

    add r1,r1, #2

    b .


# TASK 3
.section .text.task3
.align  2
.type   task3, %function
task3:

    add r2,r2, #3

    b .


/*      *********** STACK START  ***********     */

# STACK 1
.section .data.stack1
.align  2
stack1:

    .word 0x18
    .word 0x19
    .word 0x1a
    .word 0x1b
    .word 0x16
    .word 0x17
    .word 0x10
    .word 0x11
    .word 0x12
    .word 0x13
    .word 0x1c
    .word task1
    .word task1
    .word 0x01000000
    .zero 16


# STACK 2
.section .data.stack2
.align  2
stack2:

    .word 0x28
    .word 0x29
    .word 0x2a
    .word 0x2b
    .word 0x26
    .word 0x27
    .word 0x20
    .word 0x21
    .word 0x22
    .word 0x23
    .word 0x2c
    .word task2
    .word task2
    .word 0x01000000
    .zero 16



# STACK 3
.section .data.stack3
.align  2
stack3:

    .word 0x38
    .word 0x39
    .word 0x3a
    .word 0x3b
    .word 0x36
    .word 0x37
    .word 0x30
    .word 0x31
    .word 0x32
    .word 0x33
    .word 0x3c
    .word task3
    .word task3
    .word 0x01000000
    .zero 16


# save stacks starting address
.section .data
.align 2
stacks: 
    .word sstack1
    .word sstack2
    .word sstack3

