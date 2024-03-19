#include "main.h"
#include "bsp_led.h"
#include "bsp_clk.h"
#include "bsp_delay.h"
#include "bsp_beep.h"
#include "bsp_key.h"
#include "bsp_int.h"
#include "bsp_exti.h"
#include "bsp_epit.h"


int main(void)
{
    int_init();                     /* Initial Interrput    */
    imx6uclk_init();                /* Initial System Clock */
    clk_enable();                   /* Clock Enable         */ 
    led_init();                     /* initial LED          */
    beep_init();                    /* Beep initaial        */
    key_init();                     /* Key initial          */
    epit_init(0, 66000000/2);       /* Epit Timer Initial   */    


    /* Set Led Blinky */
    while(1)
    {
      
        delay(500);
    
    }
    return 0;
}