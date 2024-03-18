#include "main.h"
#include "bsp_led.h"
#include "bsp_clk.h"
#include "bsp_delay.h"
#include "bsp_beep.h"
#include "bsp_key.h"


int main(void)
{
    imx6uclk_init();  /* Initial System Clock */
    clk_enable();     /* Clock Enable         */ 
    led_init();       /* initial LED          */
    beep_init();      /* Beep initaial        */
    key_init();       /* Key initial          */

    
    int i = 0;
    unsigned char led_state = OFF;
    unsigned char beep_state = OFF;
    int keyvalue = 0;

    /* Set Led Blinky */
    while(1)
    {
        /* Use Key to Control Beep */
        keyvalue = key_getvalue();
        if(keyvalue)/* if the value is positive, the value is valid */
        {
            switch(keyvalue)
            {
                case KEY0_VALUE :
                    beep_state = !beep_state;
                    beep_switch(beep_state);
                    break;
            }
        }
        i++;
        if(i == 50)/* 50*10 = 500ms */
        {
            i = 0;
            led_state = !led_state;
            led_switch(LED0,led_state);
        }
        delay(10);
    
    }
    return 0;
}