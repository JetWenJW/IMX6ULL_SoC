#include "bsp_gpio.h"

/* GPIO initial */
void gpio_init(GPIO_Type * base, int pin, gpio_pin_config_t *config)
{
    if(config -> direction == kGPIO_DigitalInput)/* GPIO as Input */
    {
        base -> GDIR &=~(1 << pin); 
    }
    else
    {
        base -> GDIR |= (1 << pin);
        gpio_pinwrite(base, pin, config -> outputLogic);
    }
}

/* Control GPIO Voltage */
void gpio_pinwrite(GPIO_Type *base, int pin, int value)
{
    if(value == 0)
    {
        base -> DR &=~(1 << pin);
    }
    else
    {
        base -> DR |= (1 << pin);
    }
}

/* Read IO voltage */
int gpio_pinread(GPIO_Type *base, int pin)
{
    return (((base -> DR) >> pin) & 0x01);
}