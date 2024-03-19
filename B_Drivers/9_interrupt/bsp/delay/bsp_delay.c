#include "bsp_delay.h"


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