#ifndef __BSP_EPIR_H
#define __BSP_EPIT_H
#include "imx6u.h"



/* Function Declare */
void epit1_IRQHandler(unsigned int giciar, void *param);
void epit_init(unsigned int frac, unsigned int value);

#endif