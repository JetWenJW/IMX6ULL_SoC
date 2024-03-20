#include "bsp_led.h"
#include "cc.h"


void led_init(void)
{

    IOMUXC_SetPinMux(IOMUXC_GPIO1_IO03_GPIO1_IO03, 0);              /* Set GPIO1_IO03             */
    IOMUXC_SetPinConfig(IOMUXC_GPIO1_IO03_GPIO1_IO03, 0x10B0);      /* Set GPIO_IO03 Elc Property */


    /* GPIO initial */
    GPIO1 -> GDIR = 0x8;                       /* Set as output     */
    GPIO1 -> DR  |= (1 << 3);                  /* Default LED OFF   */

}

/* LED on */
void led_on(void)
{
    GPIO1 -> DR &= ~(1 << 3);           /* Clear bit[3] */
}

/* LED off */
void led_off(void)
{
    GPIO1 -> DR |= (1 << 3);            /* Set bit[3] */
}

/* LED Switch */
void led_switch(int led, int status)
{
    switch(led)
    {
        case LED0 :
            if(status == ON)
            {
                GPIO1 -> DR &=~ (1 << 3);       /* Clear bit[3] */
            }
            else if(status == OFF)
            {
                GPIO1 -> DR |= (1 << 3);        /* Set bit[3] */
            }
            break;
    }

}