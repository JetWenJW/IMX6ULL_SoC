#ifndef __BSP_DELAY_H
#define __BSP_DELAY_H


#include "fsl_common.h"
#include "fsl_iomuxc.h"
#include "MCIMX6Y2.h"

/* Function Declare */
void delay_short(volatile unsigned int n);
void delay(volatile unsigned int n);
void delay_init(void);
void gpt1_IRQHandler(unsigned int gicciar, void *param);
void delay_ms(unsigned int ms);
void delay_us(unsigned int us);



#endif