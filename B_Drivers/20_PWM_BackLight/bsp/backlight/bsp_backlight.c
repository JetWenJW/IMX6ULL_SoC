#include "bsp_backlight.h"
#include "bsp_int.h"


struct backlight_dev_struct backlight_dev;



/* Initial PWM */
void backlight_init(void)
{
    unsigned char i = 0;
    /* Pin IO Initial */
    IOMUXC_SetPinMux(IOMUXC_GPIO1_IO08_PWM1_OUT, 0);        /* Set PWM Pin */
    IOMUXC_SetPinConfig(IOMUXC_GPIO1_IO08_PWM1_OUT,0xB090); /* Configure Electronical Property */

    /* PWM initial */
    /* PWM CLK SRC = 66MHz
     * Divider = 1
     * PWM clock source = 1MHz
     */
    PWM1 -> PWMCR = 0;       /* Clear All (Much Easier to Set bits as 1) */
    PWM1 ->PWMCR |= (1 << 16) | (65 << 4) | (1 << 26);

    pwm1_setperiod_value(1000); /* PWM Freq = 1KHz */

    /* Set Default Duty Cycle */
    backlight_dev.pwm_duty = 50;
    for(i = 0;i < 4; i++)
    {
        pwm1_setduty(backlight_dev.pwm_duty);
    }

    /* Enable Interrupt */
    PWM1 -> PWMIR = (1 << 0);
    GIC_EnableIRQ(PWM1_IRQn);
    system_register_irqhandler(PWM1_IRQn, pwm1_IRQHandler, NULL);
    PWM1 -> PWMSR |= 0xFF;

    /* Enable PWM1 */
    PWM1 -> PWMCR |= (1 << 0);
}


/* Set Period */
void pwm1_setperiod_value(unsigned int value)
{
    unsigned int regvalue = 0;

    if(value < 2)
    {
        regvalue =2;
    }
    else
    {
        regvalue = value - 2;
    }
    PWM1 -> PWMPR = (regvalue & 0xFFFF);
}


/* Set Duty Cycle */
void pwm1_setduty(unsigned char duty)
{
    unsigned short period;
    unsigned short sample;

    backlight_dev.pwm_duty = duty;
    period = PWM1 -> PWMPR + 2;
    sample = (unsigned short)(period * backlight_dev.pwm_duty / 100.0f);
    PWM1 -> PWMSAR  = (sample &0xFFFF);
}
/* PWM1 IRQ Handler back Function */
void pwm1_IRQHandler(unsigned int gicciar, void *param)
{

    if(PWM1 -> PWMSR & (1 << 3))                 /* FIFO Empty Interrupt */
    {
        pwm1_setduty(backlight_dev.pwm_duty);
        PWM1 -> PWMSR |= (1 << 3);               /* Clear Interrupt Flag */
    }   
}