#include "bsp_key.h"
#include "bsp_delay.h"
#include "bsp_gpio.h"


/* Key initial */
void key_init(void)
{
    gpio_pin_config_t key_config;
    IOMUXC_SetPinMux(IOMUXC_UART1_CTS_B_GPIO1_IO18 ,0);           /* Set Pin as GPIO_IO18  */
    IOMUXC_SetPinConfig(IOMUXC_UART1_CTS_B_GPIO1_IO18,0xF080);    /* Configure Electronical Property */

    /*  GPIO initial */
    key_config.direction = kGPIO_DigitalInput;
    gpio_init(GPIO1, 18, &key_config);
}

/* Read the Key Value 0:Press/1:Release */
int read_key(void)
{
    int ret = 0;
    ret = gpio_pinread(GPIO1, 18);
    return ret;
}

int key_getvalue(void)
{
    int ret = 0;
    static unsigned char release = 1;  /* KeyValue = 1 means Release */

    if((release == 1) && (gpio_pinread(GPIO1, 18) == 0))/* Press key */
    {
        delay(10);                            /* delay 10ms */
        release = 0;
        if(gpio_pinread(GPIO1, 18) == 0)                   /* Read Key Value */
        {
            ret = KEY0_VALUE;
        }
    }
    else if(gpio_pinread(GPIO1, 18) == 1)
    {
        ret = KEY_NONE;
        release = 1;
    }
    return ret;
}