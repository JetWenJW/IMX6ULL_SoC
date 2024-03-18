#include "main.h"
#include "bsp_led.h"
#include "bsp_clk.h"
#include "bsp_delay.h"
#include "bsp_beep.h"


int main(void)
{
    clk_enable();     /* Clock Enable */ 
    led_init();       /* initial LED  */
    beep_init();      /* Beep initaial */

    /* Set Led Blinky */
    while(1)
    {
        led_on();
        beep_switch(ON);
        delay(1000);

        led_off();
        beep_switch(OFF);
        delay(1000);
    }
    return 0;
}