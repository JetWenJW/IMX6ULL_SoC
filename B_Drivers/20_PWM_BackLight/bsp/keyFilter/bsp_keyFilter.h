#ifndef __BSP_KEYFILTER_H
#define __BSP_KEYFILTER_H
#include "imx6u.h"


/* Function Declare */
void keyFilter_init(void);
void Filter_Timer_init(unsigned int value);
void FilterTimer_stop(void);
void FilterTimer_restart(unsigned int value);
void Filter_IRQHandler(unsigned int gicciar, void *param);
void gpio1_16_31_IRQHandler(unsigned int gicciar, void *param);

#endif