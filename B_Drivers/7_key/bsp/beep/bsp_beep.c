#include "bsp_beep.h"
#include "cc.h"
/* Beep initial */
void beep_init(void)
{
    IOMUXC_SetPinMux(IOMUXC_SNVS_SNVS_TAMPER1_GPIO5_IO01 ,0);       /* Set GPIo Pin */
    IOMUXC_SetPinConfig(IOMUXC_SNVS_SNVS_TAMPER1_GPIO5_IO01,0x10B0);/* Configure Electronical Property */

    /* GPIO Initial */
    GPIO5 -> GDIR|= (1 << 1);                                   /* Set GPIO5 as an output  */
    GPIO5 -> DR &=~ (1 << 1);                                   /* Set Beep Defaut is OFF  */
 
}

void beep_switch(int status)
{
    if(status == ON)
    {
        GPIO5 -> DR &=~ (1 << 1);       /* Clear bit[1] */
    }
    else if(status == OFF)
    {
        
        GPIO5 -> DR |= (1 << 1);        /* Set bit[1]   */
    }
}