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
#include "bsp_rtc.h"

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
    rtc_init();



    tftlcd_dev.forecolour = LCD_RED;
    tftlcd_dev.backcolour = LCD_BLACK;
    lcd_show_string(10, 40, 260, 32, 32, (char *)" IMX6U");
    lcd_show_string(10, 80, 240, 24, 24, (char *)" IMX6U");           
    lcd_show_string(10, 110, 240, 16, 16, (char *)" IMX6U");           
    lcd_show_string(10, 130, 260, 32, 32, (char *)"RTC ");           
           
    struct rtc_datatime datatime;
    static unsigned char state = 0;
    /* Set Led Blinky */
    while(1)
    {
        rtc_getdatetime(&datatime);
        state = !state;
        led_switch(LED0,state);
        delay_ms(1000);
    }
    return 0;
}