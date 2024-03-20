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
#include "bsp_uart.h"
#include "stdio.h"



int main(void)
{
    int_init();                     /* Initial Interrput    */
    imx6uclk_init();                /* Initial System Clock */
    delay_init();                   /* Initial Delay Func   */
    uart_init();                    /* Initial UART         */
    clk_enable();                   /* Clock Enable         */ 
    led_init();                     /* initial LED          */
    beep_init();                    /* Beep initaial        */
    key_init();                     /* Key initial          */

    /* Set Led Blinky */
    while(1)
    {
        /* Use Printf in main (note: Not support float point) */
        int a ,b;
        printf("Please Enter 2 numbers: ");
        scanf("%d %d",&a, &b);
        printf("\r\n %d + %d = %d\r\n", &a, &b, a+b);
    }
    return 0;
}