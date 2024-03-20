#ifndef __BSP_INT_H
#define __BSP_INT_H

#include "imx6u.h"

/* Type define system_IRQ */
typedef void(*system_irq_handler_t)(unsigned int gicciar, void *param);

/* Typr define */
typedef struct _sys_irq_handle
{
    system_irq_handler_t irqhandler;    /* IRQ Handler   */
    void *userParam;                    /* IRQ Parameter */
}sys_irq_handle_t;




/* Function Declare */
void int_init(void);
void default_IRQhandler(unsigned int gicciar, void *userParam);
void system_register_irqhandler(IRQn_Type IRQ, system_irq_handler_t handler, void *userParam);
void system_irq_init(void);


#endif