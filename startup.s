.syntax unified
.thumb

.global reset_handler

.section .isr_vector
vector_list:
    .word   0xABC0
    .word   reset_handler
    .zero   400


.section .text
.align  1
.type   reset_handler, %function
reset_handler:
    movs r0, #2;
    movs r1, #3;
    add r2,r1,r0;
    bl .

