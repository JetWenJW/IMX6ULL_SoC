#include "bsp_keyFilter.h"
#include "bsp_gpio.h"
#include "bsp_int.h"
#include "bsp_beep.h"


/* Key Initial */
void keyFilter_init(void)
{
    gpio_pin_config_t key_config;
    IOMUXC_SetPinMux(IOMUXC_UART1_CTS_B_GPIO1_IO18 ,0);           /* Set Pin as GPIO_IO18  */
    IOMUXC_SetPinConfig(IOMUXC_UART1_CTS_B_GPIO1_IO18,0xF080);    /* Configure Electronical Property */

    /*  GPIO initial */
    key_config.direction = kGPIO_DigitalInput;
    key_config.interruptmode = kGPIO_IntFallingEdge;
    gpio_init(GPIO1, 18, &key_config);  

    GIC_EnableIRQ(GPIO1_Combined_16_31_IRQn);
    system_register_irqhandler(GPIO1_Combined_16_31_IRQn, gpio1_16_31_IRQHandler, NULL);

    gpio_enable(GPIO1, 18);
    
    /* Filter timer initial */
    Filter_Timer_init(66000000/10);
}


/* Initial EPIT1 Timer */
void Filter_Timer_init(unsigned int value)
{
    EPIT1 -> CR = 0;
    EPIT1 -> CR = (1 << 1) | (1 << 2) |(1 << 3) | (1 << 24);

    EPIT1 -> LR = value; /* Count Down Value */
    EPIT1 ->CMPR = 0;

    GIC_EnableIRQ(EPIT1_IRQn);
    system_register_irqhandler(EPIT1_IRQn, Filter_IRQHandler, NULL);

    /* EPIT timer Enable */
    EPIT1 -> CR |= (1 << 0);
}

/* Disable  EPIT1 Timer */
void FilterTimer_stop(void)
{
    EPIT1 -> CR &=~ (1 << 0);
}

/* Restart EPIT1 Timer */
void FilterTimer_restart(unsigned int value)
{
    EPIT1 -> CR &=~ (1 << 0);
    EPIT1 -> LR = value;
    EPIT1 -> CR |=  (1 << 0);
}

/* EPIT1 interrupt IRQHandler */
void Filter_IRQHandler(unsigned int gicciar, void *param)
{
    static unsigned char state = OFF;
    if(EPIT1 -> SR & (1 << 0))
    {
        /* Disable Timer */
        FilterTimer_stop();
        if(gpio_pinread(GPIO1, 18) == 0)
        {
            state = !state;
            beep_switch(state);
        }
    }
    EPIT1 -> SR |= (1 << 0);/* Clear EPIT interrupt Flags */

}

/* Key Interrput IRQ Handler */
void gpio1_16_31_IRQHandler(unsigned int gicciar, void *param)
{
    /* Enable  EPIT1 Timer */
    FilterTimer_restart(66000000/100);

    /* Clear GPIO interrupt Flags */
    gpio_clearIntFlags(GPIO1, 18);
}