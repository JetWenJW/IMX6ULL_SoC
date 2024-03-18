.global _start
.global _bss_start
_bss_start :
    .word __bss_start

.global _bss_end
_bss_end :
    .word __bss_end

_start : 
    /* Set CPU into SVC mode */
    mrs r0, cpsr                    /* Read CPSR to r0 Register of CPU */
    bic r0, r0, #0x1F               /* Clear CPSR bit[4:0]             */
    orr r0, r0, #0x13               /* In SVC Mode                     */
    msr cpsr, r0                    /* Write r0 to CPSR                */

    /* Clear bss section */
    ldr r0, _bss_start              /* Store _bss_start address in r0  */
    ldr r1, _bss_end                /* Store _bss_end address in r1    */
    mov r2, #0

bss_loop : 
    stmia r0!, {r2}
    cmp r0, r1                      /* Compare values of r0 & r1 */
    ble bss_loop                    /* Continue to Clean bss section until r0 address < r1 address*/



    /* Configured SP pointer */
    ldr sp, =0x80200000
    b main                          /* Jump into main function         */