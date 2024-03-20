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
#include "bsp_lcd.h"
#include "bsp_lcdapi.h"

#define lcd_readpoint_function_test     0

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
    lcd_init();                     /* LCD Initial          */


    static unsigned char state = 0;
    lcd_drawpoint(0, 0, LCD_RED);                                           /* Left Up side    */
    lcd_drawpoint(tftlcd_dev.width - 1, 0, LCD_RED);                        /* Right Up side   */
    lcd_drawpoint(0, tftlcd_dev.height - 1, LCD_RED);                       /* Left Down side  */
    lcd_drawpoint(tftlcd_dev.width - 1, tftlcd_dev.height - 1, LCD_RED);    /* Right Down side */

#if lcd_readpoint_function_test
    unsigned int colour = 0;
    colour = lcd_readpoint(0, 0);
    printf("Right Up = %#x\r\n", colour);
    colour = lcd_readpoint(tftlcd_dev.width - 1, 0);
    printf("Left Up = %#x\r\n", colour);
        
    colour = lcd_readpoint(0, tftlcd_dev.height - 1);
    printf("Right Down = %#x\r\n", colour);
    colour = lcd_readpoint(tftlcd_dev.width - 1, tftlcd_dev.height - 1);
    printf("Left Down = %#x\r\n", colour);
#endif
    tftlcd_dev.forecolour = LCD_RED;
    tftlcd_dev.backcolour = LCD_BLACK;
    lcd_show_string(10, 40, 260, 32, 32, (char *)"ALPHA IMX6U");
    lcd_show_string(10, 80, 240, 24, 24, (char *)"ALPHA IMX6U");           
    lcd_show_string(10, 110, 240, 16, 16, (char *)"ALPHA IMX6U");           
    lcd_show_string(10, 130, 260, 32, 32, (char *)"ALPHA IMX6U");           
           

    /* Set Led Blinky */
    while(1)
    {
        state = !state;
        led_switch(LED0, state);
        delay_ms(1000);
    }
    return 0;
}