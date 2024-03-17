#include "main.h"


void clk_enable(void)
{
    CCM_CCGR0 = 0xFFFFFFFF;
    CCM_CCGR1 = 0xFFFFFFFF;
    CCM_CCGR2 = 0xFFFFFFFF;
    CCM_CCGR3 = 0xFFFFFFFF;
    CCM_CCGR4 = 0xFFFFFFFF;
    CCM_CCGR5 = 0xFFFFFFFF;
    CCM_CCGR6 = 0xFFFFFFFF;
}

void led_init(void)
{
    SW_MUX_GPIO1_IO03 = 0x5;                /* Set GPIO1_IO03             */
    SW_PAD_GPIO1_IO03 = 0x10B0;             /* Set GPIO_IO03 Elc Property */

    /* GPIO initial */
    GPIO1_GDIR = 0x8;                       /* Set as output              */
    GPIO1_DR   = 0x0;                       /* LED ON                     */

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
    GPIO1_DR &= ~(1 << 3);           /* Clear bit[3] */
}
/* LED off */
void led_off(void)
{
    GPIO1_DR |= (1 << 3);            /* Set bit[3] */
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