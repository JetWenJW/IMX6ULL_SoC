.global _start



_start :
    /* Createe Vector Table */
    /* Store Handler address to PC register */
    ldr pc, = Reset_Handler         
    ldr pc, = Undefined_Handler
    ldr pc, = SVC_Handler
    ldr pc, = PreAbort_Handler
    ldr pc, = DataAbort_Handler
    ldr pc, = NotUsed_Handler
    ldr pc, = IRQ_Handler
    ldr pc, = FIQ_Handler

/* Reset_Handler Function Body */
Reset_Handler :
    /* Needs to Reference Arm Cortex-A7 TRM */
    cpsid i                                     /* Disable IRQ                    */
    mrc p15, 0, r0, c1, c0, 0                   /* Read SCTRL data to r0 register */
    bic r0, r0, #(1 << 12)                      /* Close I-Cache                  */
    bic r0, r0, #(1 << 11)                      /* Close Branch predict           */
    bic r0, r0, #(1 << 2)                       /* Close D-Cache                  */
    bic r0, r0, #(1 << 1)                       /* Close Alignment                */
    bic r0, r0, #(1 << 0)                       /* Close MMU                      */
    mcr p15, 0, r0, c1, c0, 0                   /* Write r0 to SCTRL              */

#if 0 //We have used inline assember "Set vecetor offset" in bsp_int.c & bsp_int.h
    /* Set vector offset */
    ldr r0, = 0x87800000
    dsb                                         /* Data Synchronize               */
    isb                                         /* Instruction Synchronize        */
    mcr p15, 0, r0, c12, c0, 0                  /* Set VBAR register = 0x87800000 */
    dsb                                         /* Data Synchronize               */
    isb                                         /* Instruction Synchronize        */
#endif // So We don't need this part any more

.global _bss_start
_bss_start :
    .word __bss_start

.global _bss_end
_bss_end :
    .word __bss_end

    /* Clear bss section */
    ldr r0, _bss_start              /* Store _bss_start address in r0  */
    ldr r1, _bss_end                /* Store _bss_end address in r1    */
    mov r2, #0

bss_loop : 
    stmia r0!, {r2}
    cmp r0, r1                      /* Compare values of r0 & r1 */
    ble bss_loop                    /* Continue to Clean bss section until r0 address < r1 address*/

    /* Set CPU into IRQ mode */
    mrs r0, cpsr                    /* Read CPSR to r0 Register of CPU */
    bic r0, r0, #0x1F               /* Clear CPSR bit[4:0]             */
    orr r0, r0, #0x12               /* In IRQ Mode                     */
    msr cpsr, r0                    /* Write r0 to CPSR                */
    ldr sp, = 0x80600000            /* Set Stack Pointer in IRQ mode   */

    /* Set CPU into SYS mode */
    mrs r0, cpsr                    /* Read CPSR to r0 Register of CPU */
    bic r0, r0, #0x1F               /* Clear CPSR bit[4:0]             */
    orr r0, r0, #0x1F               /* In SYS Mode                     */
    msr cpsr, r0                    /* Write r0 to CPSR                */
    ldr sp, = 0x80400000            /* Set Stack Pointer in SYS mode   */

    /* Set CPU into SVC mode */
    mrs r0, cpsr                    /* Read CPSR to r0 Register of CPU */
    bic r0, r0, #0x1F               /* Clear CPSR bit[4:0]             */
    orr r0, r0, #0x13               /* In SVC Mode                     */
    msr cpsr, r0                    /* Write r0 to CPSR                */
    ldr sp, = 0x80200000            /* Set Stack Pointer in SVC mode   */

    cpsie i                         /* Enable IRQ                      */
    b main                          /* Jump into main function         */


/* Undefined_Handler Function Body */
Undefined_Handler :
    ldr r0, = Undefined_Handler 
    bx r0

/* SVC_Handler Function Body */
SVC_Handler :
    ldr r0, = SVC_Handler 
    bx r0

/* PreAbort_Handler Function Body */
PreAbort_Handler :
    ldr r0, = PreAbort_Handler 
    bx r0

/* DataAbort_Handler Function Body */
DataAbort_Handler :
    ldr r0, = DataAbort_Handler 
    bx r0

/* NotUsed_Handler Function Body */
NotUsed_Handler :
    ldr r0, = NotUsed_Handler 
    bx r0

/* IRQ_Handler Function Body */
IRQ_Handler :
	push {lr}					/* Store le address */
	push {r0-r3, r12}			/* Store r0-r3 & r12 register */

	mrs r0, spsr				/* Read CPSR register */
	push {r0}					/* Save CPSR register */

	mrc p15, 4, r1, c15, c0, 0 
									
	add r1, r1, #0X2000			
	ldr r0, [r1, #0XC]			
								 
	push {r0, r1}				/* Store r0,r1                             */
	
	cps #0x13					/* Get into SVC mode                       */
	
	push {lr}					/* Save SVC mode of lr register            */
	ldr r2, =system_irqhandler	/* Load C language ISR to r2 register      */
	blx r2						/* Run C Language ISR                      */

	pop {lr}					/* When Finished C Language ISR lr POP out */
	cps #0x12					/* Get into IRQ mode                       */
	pop {r0, r1}				
	str r0, [r1, #0X10]			

	pop {r0}						
	msr spsr_cxsf, r0			/* Recover spsr                            */

	pop {r0-r3, r12}			/* r0-r3,r12 pop out                       */
	pop {lr}					/* lr pop out                              */
	subs pc, lr, #4				/* Get lr-4 Value to pc                    */

/* FIQ_Handler Function Body */
FIQ_Handler :
    ldr r0, = FIQ_Handler 
    bx r0


