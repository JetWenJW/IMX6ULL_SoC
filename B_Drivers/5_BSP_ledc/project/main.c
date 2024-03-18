#include "main.h"
#include "bsp_led.h"
#include "bsp_clk.h"
#include "bsp_delay.h"


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