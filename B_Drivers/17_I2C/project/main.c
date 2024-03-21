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
#include "bsp_ap3216c.h"


int main(void)
{


	int_init(); 				/* Initial Interrupt             */
	imx6uclk_init();			/* Initial System Clock		  	 */
	delay_init();				/* Initial Delay 			     */
	clk_enable();				/* Initial Clock 		      	 */
	led_init();					/* Initial led 			         */
	beep_init();				/* Initial beep	 		         */
	uart_init();				/* UART Inital                   */
	lcd_init();					/* LCD Initial			         */

	tftlcd_dev.forecolour = LCD_RED;	
	lcd_show_string(30, 50, 200, 16, 16, (char*)"IMX6U IIC TEST");  
	lcd_show_string(30, 70, 200, 16, 16, (char*)"AP3216C TEST");  
	lcd_show_string(30, 110, 200, 16, 16, (char*)"2024/3/26");  
	
	while(ap3216c_init())		/* Cannot detect AP3216C */
	{
		lcd_show_string(30, 130, 200, 16, 16, (char*)"AP3216C Check Failed!");
		delay_ms(500);
		lcd_show_string(30, 130, 200, 16, 16, (char*)"Please Check!        ");
		delay_ms(500);
	}	
	
	lcd_show_string(30, 130, 200, 16, 16, (char*)"AP3216C Ready!");  
    lcd_show_string(30, 160, 200, 16, 16, (char*)" IR:");	 
	lcd_show_string(30, 180, 200, 16, 16, (char*)" PS:");	
	lcd_show_string(30, 200, 200, 16, 16, (char*)"ALS:");	
	tftlcd_dev.forecolour = LCD_BLUE;	


    unsigned short ir, als, ps;
	unsigned char state = OFF;
	while(1)					
	{
		ap3216c_readData(&ir, &ps, &als);		/* Read Data	    	*/
		lcd_shownum(30 + 32, 160, ir, 5, 16);	/* Show IR Data 		*/
        lcd_shownum(30 + 32, 180, ps, 5, 16);	/* Show PS Data 		*/
        lcd_shownum(30 + 32, 200, als, 5, 16);	/* Show ALS Data 	    */ 
		delay_ms(120);
		state = !state;
		led_switch(LED0,state);	
	}
	return 0;
}