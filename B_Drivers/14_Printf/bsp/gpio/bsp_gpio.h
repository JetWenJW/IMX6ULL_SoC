#ifndef __BSP_GPIO_H
#define __BSP_GPIO_H

#include "fsl_common.h"
#include "fsl_iomuxc.h"
#include "MCIMX6Y2.h"
#include "cc.h"




/*
 * GPIO Interrupt Type define
 */
typedef enum _gpio_interrupt_mode
{
    kGPIO_NoIntmode      = 0,			/* No Use interrupt     */
    kGPIO_IntLowLevel    = 1, 			/* Low Voltage Trigger	*/
    kGPIO_IntHighLevel   = 2, 			/* High Voltage Trigger	*/
    kGPIO_IntRisingEdge  = 3, 			/* Rising Edge Trigger	*/
    kGPIO_IntFallingEdge = 4, 			/* Falling Edge Trigger	*/
    kGPIO_IntRisingOrFallingEdge = 5U, 	/* Both EdgeTrigger     */
} gpio_interrupt_mode_t;	

/* Type define GPIO Handle */
typedef enum _gpio_pin_direction
{
    kGPIO_DigitalInput = 0,
    kGPIO_DigitalOutput = 1
} gpio_pin_direction_t;

typedef struct _gpio_pin_config
{
    gpio_pin_direction_t direction;
    uint8_t outputLogic;
    gpio_interrupt_mode_t interruptmode;  /* Interrupt Trigger Mode */
} gpio_pin_config_t;

/* Function Declare */
void gpio_init(GPIO_Type * base, int pin, gpio_pin_config_t *config);
void gpio_pinwrite(GPIO_Type *base, int pin, int value);
int gpio_pinread(GPIO_Type *base, int pin);
void gpio_clearIntFlags(GPIO_Type *base, unsigned int pin);
void gpio_disable(GPIO_Type *base, unsigned int pin);
void gpio_enable(GPIO_Type *base, unsigned int pin);
void gpio_intconfig(GPIO_Type *base, unsigned int pin, gpio_interrupt_mode_t pin_int_mode);



#endif