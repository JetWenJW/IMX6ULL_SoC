#ifndef __BSP_EXTI_H
#define __BSP_EXTI_H
#include "imx6u.h"


/* Function Declare */
void gpio1_18_IRQHandler(unsigned int gicciar, void *param);
void exti_init(void);



#endif