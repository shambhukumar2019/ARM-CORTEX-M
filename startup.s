.syntax unified
.thumb

.global reset_handler

.section .isr_vector
vector_list:
    .word   0x20000100
    .word   reset_handler
    .zero   64


.section .text
.align  1
.type   reset_handler, %function
reset_handler:
    mov r0, #1
    mov r1, #2
    mov r2, #3
    mov r3, #4

    push {r0-r3}
    
    mov r0, #11
    mov r1, #22
    mov r2, #33
    mov r3, #44

    pop {r3,r2,r1,r0}
    
    b .

