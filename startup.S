/*
    Unified syntax simplifies writing assembly code 
    that can switch between ARM and Thumb states 
    without changing the syntax
*/
.syntax unified
.thumb

# make global access for symbols
.global reset_handler
.global nmi_handler
.global hardfault_handler
.global memmanage_handler
.global busfault_handler
.global usagefault_handler
.global svc_handler
.global debug_monitor_handler
.global pendsv_handler
.global systick_handler
.global default_handler

# linker script symbols
.global _min_stack_size
.global _estack
.global _etext
.global _stext
.global _sdata
.global _edata
.global _sbss
.global _ebss



/* 
    exception vector table
    progbits indicates this section contains
    executable code and initiallized data
*/
.section .isr_vector, "a", %progbits
.align 2
vector_list:

    .word   _estack                    // $sp = 0x20000400
    .word   reset_handler              // Reset_Handler
    .word   nmi_handler                // NMI Handler
    .word   hardfault_handler          // Hard Fault Handler
    .word   memmanage_handler          // MPU Fault Handler
    .word   busfault_handler           // Bus Fault Handler
    .word   usagefault_handler         // Usage Fault Handler
    .word   0                          // Reserved
    .word   0                          // Reserved
    .word   0                          // Reserved
    .word   0                          // Reserved
    .word   svc_handler                // SVCall Handler
    .word   debug_monitor_handler      // Debug Monitor Handler
    .word   0                          // Reserved
    .word   pendsv_handler             // PendSV Handler
    .word   systick_handler            // SysTick Handler
    .word   WWDG_IRQHandler            // window watchdog interrupt handler
    .zero   300

.size vector_list, . - vector_list  /* size of vector table */



@ .section .text.reset_handler
@ .type reset_handler, %function
@ .align 2
@ reset_handler:
@     ldr sp, =_estack; # set stack pointer
    


// undefined exceptions must reach here
.section .text.default_handler
.type default_handler, %function
.align 2
default_handler:
    b .



# make all exception handlers as "weak" function except reset_handler
# also "alias" them to default_handler

.weak default_handler

.weak nmi_handler
.thumb_set nmi_handler, default_handler

.weak hardfault_handler
.thumb_set hardfault_handler, default_handler

.weak memmanage_handler
.thumb_set memmanage_handler, default_handler

.weak busfault_handler
.thumb_set busfault_handler, default_handler

.weak usagefault_handler
.thumb_set usagefault_handler, default_handler

.weak svc_handler
.thumb_set svc_handler, default_handler

.weak debug_monitor_handler
.thumb_set debug_monitor_handler, default_handler

.weak pendsv_handler
.thumb_set pendsv_handler, default_handler

.weak systick_handler
.thumb_set systick_handler, default_handler


