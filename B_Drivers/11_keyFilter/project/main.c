#include "main.h"
#include "bsp_led.h"
#include "bsp_clk.h"
#include "bsp_delay.h"
#include "bsp_beep.h"
#include "bsp_key.h"
#include "bsp_int.h"
#include "bsp_exti.h"
#include "bsp_epit.h"
#include "bsp_keyFilter.h"


int main(void)
{
    int_init();                     /* Initial Interrput    */
    imx6uclk_init();                /* Initial System Clock */
    clk_enable();                   /* Clock Enable         */ 
    led_init();                     /* initial LED          */
    beep_init();                    /* Beep initaial        */
    key_init();                     /* Key initial          */
    keyFilter_init();               /* Initial Key Filter   */


    unsigned char state = OFF;

    /* Set Led Blinky */
    while(1)
    {
        state = !state;
        led_switch(LED0, state);
        delay(500);
    
    }
    return 0;
}