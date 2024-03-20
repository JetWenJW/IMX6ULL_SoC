#include "bsp_exti.h"
#include "bsp_gpio.h"
#include "bsp_int.h"
#include "bsp_beep.h"
#include "bsp_delay.h"



/* Initial GPIO1_IO18 as external Interrupt */
void exti_init(void)
{
    gpio_pin_config_t key_config;
    IOMUXC_SetPinMux(IOMUXC_UART1_CTS_B_GPIO1_IO18 ,0);           /* Set Pin as GPIO_IO18  */
    IOMUXC_SetPinConfig(IOMUXC_UART1_CTS_B_GPIO1_IO18,0xF080);    /* Configure Electronical Property */

    /*  GPIO initial */
    key_config.direction = kGPIO_DigitalInput;
    key_config.interruptmode = kGPIO_IntFallingEdge;
    gpio_init(GPIO1, 18, &key_config);  

    GIC_EnableIRQ(GPIO1_Combined_16_31_IRQn);
    system_register_irqhandler(GPIO1_Combined_16_31_IRQn, gpio1_18_IRQHandler, NULL);

    gpio_enable(GPIO1, 18);


}
/* Call back Function */
void gpio1_18_IRQHandler(unsigned int gicciar, void *param)
{
    static unsigned char state = 0;
    
    delay(10);
    if(gpio_pinread(GPIO1, 18) == 0)
    {
        state = !state;
        beep_switch(state);
    }

    /* Clear Interrupt Flags */
    gpio_clearIntFlags(GPIO1, 18);
}