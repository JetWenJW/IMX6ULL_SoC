#include "bsp_key.h"
#include "bsp_delay.h"


/* Key initial */
void key_init(void)
{
    IOMUXC_SetPinMux(IOMUXC_UART1_CTS_B_GPIO1_IO18 ,0);           /* Set Pin as GPIO_IO18  */
    IOMUXC_SetPinConfig(IOMUXC_UART1_CTS_B_GPIO1_IO18,0xF080);    /* Configure Electronical Property */

    /* GPIO Initial */
    GPIO1 -> GDIR &=~ (1 << 18);                                   /* Set GPIO18 as an input  */
}

/* Read the Key Value 0:Press/1:Release */
int read_key(void)
{
    int ret = 0;
    ret = ((GPIO1 -> DR) >> 18) & 0x01;
    return ret;
}

int key_getvalue(void)
{
    int ret = 0;
    static unsigned char release = 1;  /* KeyValue = 1 means Release */

    if((release == 1) && (read_key() == 0))/* Press key */
    {
        delay(10);                            /* delay 10ms */
        release = 0;
        if(read_key() == 0)                   /* Read Key Value */
        {
            ret = KEY0_VALUE;
        }
    }
    else if(read_key() == 1)
    {
        ret = KEY_NONE;
        release = 1;
    }
    return ret;
}