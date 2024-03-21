#ifndef __BSP_BACKLIGHT_H
#define __BSP_BACKLIGHT_H

#include "imx6u.h"


/* PWM Data structure */
struct backlight_dev_struct
{
    unsigned char pwm_duty; /* 0~100 */
};

extern struct backlight_dev_struct backlight_dev;

/* Function Declare */
void pwm1_setperiod_value(unsigned int value);
void pwm1_setduty(unsigned char duty);
void pwm1_IRQHandler(unsigned int gicciar, void *param);
void backlight_init(void);

#endif