.global _start /* Declare Global variable _start */

_start : 
    /* Initialize All Clock */
    ldr r0, =0x020C4068 /* CCGR0 */
    ldr r1, =0xFFFFFFFF /* The Data that gonna Write to CCGR0 */
    str r1, [r0]        /* Write r1 to r0 */ 
    
    ldr r0, =0x020C406C /* CCGR1 */
    str r1, [r0]        /* Write r1 to r0 */ 

    ldr r0, =0x020C4070 /* CCGR2 */
    str r1, [r0]        /* Write r1 to r0 */ 

    ldr r0, =0x020C4074 /* CCGR3 */
    str r1, [r0]        /* Write r1 to r0 */ 

    ldr r0, =0x020C4078 /* CCGR4 */
    str r1, [r0]        /* Write r1 to r0 */ 

    ldr r0, =0x020C407C /* CCGR5 */
    str r1, [r0]        /* Write r1 to r0 */ 

    ldr r0, =0x020C4080 /* CCGR6 */
    str r1, [r0]        /* Write r1 to r0 */ 

    /* Config GPIO_IO03 Pin to GPIO                         */
    /* Set IOMUX_SW_MUX_CTL_PAD_GPIO_IO03 = 5               */
    /* IOMUX_SW_MUX_CTL_PAD_GPIO_IO03 address is 0x020E0068 */
    ldr r0, =0x020E0068 /* IOMUX_SW_MUX_CTL_PAD_GPIO_IO03                              */
    ldr r1, =0x5        /* The Data that gonna Write to IOMUX_SW_MUX_CTL_PAD_GPIO_IO03 */
    str r1, [r0]        /* Write r1 to r0                                              */ 
    
    /* Config GPIO1_IO03 Electronical Property                   */
    /* Set IOMUX_SW_PAD_CTL_PAD_GPIO_IO03                        */
    /* IOMUX_SW_PAD_CTL_PAD_GPIO_IO03 address is 0x020E02F4      */
    /* bit]0]:  set  0 Low Speed                                 */
    /* bit[5:3] set  110                                         */
    /* bit[7:6] set  10:100MHz speed                             */
    /* bit[11]  set  0:Close Open circuit output                 */
    /* bit[12]  set  1:Enable Pull/Keeper                        */
    /* bit[13]  set  0:Keeper                                    */
    /* bit[16]  set  0:Close Hys                                 */
    ldr r0, =0x020E02F4
    ldr r1, =0x10B0
    str r1, [r0]
    
    
    /* Config GPIO                              */
    /* Set GPIO1_GDIR & GPIO1_GPIO03 as Output  */
    /* GPIO1_GDIR address is 0x0209C004         */
    ldr r0, = 0x0209C004
    ldr r1, = 0x8
    str r1, [r0]

    /* Open LED Set GPIO1_IO03 as 0             */
    /* GPIO1_DR address is 0x0209C000           */
    ldr r0, =0x0209C0000
    ldr r1, = 0x0
    str r1, [r0]

loop:
    b loop