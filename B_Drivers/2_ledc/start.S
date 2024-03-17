.global _start

_start : 
    /* Set CPU into SVC mode */
    mrs r0, cpsr                    /* Read CPSR to r0 Register of CPU */
    bic r0, r0, #0x1F               /* Clear CPSR bit[4:0]             */
    orr r0, r0, #0x13               /* In SVC Mode                     */
    msr cpsr, r0                    /* Write r0 to CPSR                */

    /* Configured SP pointer */
    ldr sp, =0x80200000
    b main                          /* Jump into main function         */