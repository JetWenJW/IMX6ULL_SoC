#include "fsl_iomuxc.h"
#include "MCIMX6Y2.h"

void clk_enable(void)
{
    CCM -> CCGR0 = 0xFFFFFFFF;
    CCM -> CCGR1 = 0xFFFFFFFF;
    CCM -> CCGR2 = 0xFFFFFFFF;
    CCM -> CCGR3 = 0xFFFFFFFF;
    CCM -> CCGR4 = 0xFFFFFFFF;
    CCM -> CCGR5 = 0xFFFFFFFF;
    CCM -> CCGR6 = 0xFFFFFFFF;
}

void led_init(void)
{

    IOMUXC_SetPinMux(IOMUXC_GPIO1_IO03_GPIO1_IO03, 0);              /* Set GPIO1_IO03             */
    IOMUXC_SetPinConfig(IOMUXC_GPIO1_IO03_GPIO1_IO03, 0x10B0);      /* Set GPIO_IO03 Elc Property */


    /* GPIO initial */
    GPIO1 -> GDIR = 0x8;                       /* Set as output              */
    GPIO1 -> DR   = 0x0;                       /* LED ON                     */

}

/* Short delay */
void delay_short(volatile unsigned int n)
{
    while(n--){}
}

/* Delay 1ms */
void delay(volatile unsigned int n)
{
    while(n--)
    {
        delay_short(0x7FF);
    }
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



int main(void)
{
   clk_enable();     /* Clock Enable */ 
   led_init();       /* initial LED  */

    /* Set Led Blinky */
    while(1)
    {
        led_on();
        delay(500);

        led_off();
        delay(500);
    }
    return 0;
}