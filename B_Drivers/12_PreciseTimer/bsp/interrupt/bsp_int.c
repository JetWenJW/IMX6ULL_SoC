#include "bsp_int.h"


static unsigned int IRQNesting;/* Number of IRQ_Handler waiting to run */

/* IRQ list */
static sys_irq_handle_t IRQ_table[NUMBER_OF_INT_VECTORS];

/* Initial IRQ list */
void system_irq_init(void)
{
    IRQNesting = 0;
    unsigned int i = 0;
    for(i = 0;i < NUMBER_OF_INT_VECTORS; i++)
    {
        IRQ_table[i].irqhandler = default_IRQhandler;
        IRQ_table[i].userParam = NULL;
    }
}

/* Register IRQ Handler */
void system_register_irqhandler(IRQn_Type IRQ, system_irq_handler_t handler, void *userParam)
{
    IRQ_table[IRQ].irqhandler = handler;
    IRQ_table[IRQ].userParam = userParam;

}

/* Initial Interrupt */
void int_init(void)
{
    GIC_Init();
    system_irq_init();    
    /* vector offset */
    __set_VBAR(0x87800000);
}

/* IRQ_Handler call back function */
void system_irqhandler(unsigned int gicciar)
{
    uint32_t intNum = gicciar &0x3FF;

    /* Check IRQ number in the range */
    if((intNum == 1023) || (intNum >= NUMBER_OF_INT_VECTORS))
    {
        return ;
    }

    IRQNesting++;
    /* Follow IRQ number  */
    IRQ_table[intNum].irqhandler(intNum, IRQ_table[intNum].userParam);
    IRQNesting--;
}

/* Default IRQ_Handler */
void default_IRQhandler(unsigned int gicciar, void *userParam)
{
    while(1);
}