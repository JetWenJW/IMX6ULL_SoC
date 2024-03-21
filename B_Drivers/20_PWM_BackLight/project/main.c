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
#include "bsp_backlight.h"



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
	unsigned char key = 0;
	unsigned char duty;

	imx6ul_hardfpu_enable();	/* Enable I.MX6U H/W float point	*/
	int_init(); 				/* Initial Interrupt 				*/
	imx6uclk_init();			/* Initial System Clock				*/
	delay_init();				/* Initial Delay 					*/
	clk_enable();				/* Initial CLK 						*/
	led_init();					/* Initial led 						*/
	beep_init();				/* Initial beep	 					*/
	uart_init();				/* Initial UART, Baud = 115200 		*/
	lcd_init();					/* Initial LCD 						*/
	backlight_init();			/* Initial PWM Back Light			*/

	tftlcd_dev.forecolour = LCD_RED;
	lcd_show_string(50, 10, 400, 24, 24, (char*)"IMX6U PWM BackLight TEST");  
	lcd_show_string(50, 40, 200, 16, 16, (char*)" TEST");  
	lcd_show_string(50, 80, 200, 16, 16, (char*)"2024/3/27");  

	duty = 10;
	pwm1_setduty(duty);
	while(1)					
	{		
		key = key_getvalue();
		if(key == KEY0_VALUE)
		{
			duty += 10;
			if(duty > 100)
			{
				duty = 0;
			}
			printf("PWM1 Duty = %d\r\n", duty);
			pwm1_setduty(duty);
		}
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