#include "bsp_clk.h"
#include "bsp_delay.h"
#include "bsp_led.h"
#include "bsp_beep.h"
#include "bsp_key.h"
#include "bsp_int.h"
#include "bsp_uart.h"
#include "bsp_lcd.h"
#include "bsp_rtc.h"
#include "bsp_icm20608.h"
#include "bsp_spi.h"
#include "bsp_lcdapi.h"
#include "stdio.h"
#include "bsp_ft5426.h"



/* Enable IMX6ULL NEON & FPU */
 void imx6ul_hardfpu_enable(void)
{
	uint32_t cpacr;
	uint32_t fpexc;

	cpacr = __get_CPACR();
	cpacr = (cpacr & ~(CPACR_ASEDIS_Msk | CPACR_D32DIS_Msk))
		   |  (3UL << CPACR_cp10_Pos) | (3UL << CPACR_cp11_Pos);
	__set_CPACR(cpacr);
	fpexc = __get_FPEXC();
	fpexc |= 0x40000000UL;	
	__set_FPEXC(fpexc);
}


int main(void)
{
	unsigned char state = OFF;
	unsigned char i = 0;


	imx6ul_hardfpu_enable();	/* Enable I.MX6U H/W float point	*/
	int_init(); 				/* Initial Interrupt 				*/
	imx6uclk_init();			/* Initial System Clock				*/
	delay_init();				/* Initial Delay 					*/
	clk_enable();				/* Initial CLK 						*/
	led_init();					/* Initial led 						*/
	beep_init();				/* Initial beep	 					*/
	uart_init();				/* Initial UART, Baud = 115200 		*/
	lcd_init();					/* Initial LCD 						*/
	ft5426_init();				/* Initial Touch Screen FT5426		*/	

	tftlcd_dev.forecolour = LCD_RED;
	lcd_show_string(50, 10, 400, 24, 24, (char*)"IMX6U Touch Screen TEST");  
	lcd_show_string(50, 40, 200, 16, 16, (char*)"TEST");  
	lcd_show_string(50, 80, 200, 16, 16, (char*)"2024/3/27");  
#if 0	
	lcd_show_string(50, 110, 400, 16, 16,	(char*)"TP Num	:");  
	lcd_show_string(50, 130, 200, 16, 16,	(char*)"Point0 X:");  
	lcd_show_string(50, 150, 200, 16, 16,	(char*)"Point0 Y:");  
	lcd_show_string(50, 170, 200, 16, 16,	(char*)"Point1 X:");  
	lcd_show_string(50, 190, 200, 16, 16,	(char*)"Point1 Y:");  
	lcd_show_string(50, 210, 200, 16, 16,	(char*)"Point2 X:");  
	lcd_show_string(50, 230, 200, 16, 16,	(char*)"Point2 Y:");  
	lcd_show_string(50, 250, 200, 16, 16,	(char*)"Point3 X:");  
	lcd_show_string(50, 270, 200, 16, 16,	(char*)"Point3 Y:");  
	lcd_show_string(50, 290, 200, 16, 16,	(char*)"Point4 X:");  
	lcd_show_string(50, 310, 200, 16, 16,	(char*)"Point4 Y:"); 
#endif
	tftlcd_dev.forecolour = LCD_BLUE;

	while(1)					
	{		
#if 0
		unsigned char i = 0;

		lcd_shownum(50 + 72, 110, ft5426_dev.point_num , 1, 16);
		lcd_shownum(50 + 72, 130, ft5426_dev.x[0], 5, 16);
		lcd_shownum(50 + 72, 150, ft5426_dev.y[0], 5, 16);
		lcd_shownum(50 + 72, 170, ft5426_dev.x[1], 5, 16);
		lcd_shownum(50 + 72, 190, ft5426_dev.y[1], 5, 16);
		lcd_shownum(50 + 72, 210, ft5426_dev.x[2], 5, 16);
		lcd_shownum(50 + 72, 230, ft5426_dev.y[2], 5, 16);
		lcd_shownum(50 + 72, 250, ft5426_dev.x[3], 5, 16);
		lcd_shownum(50 + 72, 270, ft5426_dev.y[3], 5, 16);
		lcd_shownum(50 + 72, 290, ft5426_dev.x[4], 5, 16);
		lcd_shownum(50 + 72, 310, ft5426_dev.y[4], 5, 16);
#endif
		lcd_drawpoint(ft5426_dev.x[0], ft5426_dev.y[0], LCD_RED);

		delay_ms(10);
		i++;
	
		if(i == 50)
		{	
			i = 0;
			state = !state;
			led_switch(LED0,state); 
		}
	}
	return 0;
}