#include "bsp_epit.h"
#include "bsp_led.h"
#include "bsp_int.h"

/* Inital Epit Timer */
void epit_init(unsigned int frac, unsigned int value)
{
    if(frac > 4095)
    {
        frac = 4095;
    }
    /* Configure EPIT_CR */
    EPIT1 -> CR = 0;
    EPIT1 -> CR = (1 << 1) | (1 << 2) |(1 << 3) | (frac << 4) | (1 << 24);

    EPIT1 -> LR = value; /* Count Down Value */
    EPIT1 ->CMPR = 0;

    GIC_EnableIRQ(EPIT1_IRQn);
    system_register_irqhandler(EPIT1_IRQn, epit1_IRQHandler, NULL);

    /* EPIT timer Enable */
    EPIT1 -> CR |= (1 << 0);
}
/* EPIT1 IRQ_Handler */
void epit1_IRQHandler(unsigned int giciar, void *param)
{
    static unsigned char state = 0;
    if(EPIT1 -> SR & (1 << 0)) /* Interrupt Trigger */
    {
        led_switch(LED0, state);
    }
    /* Clear Interrupt Flags */
    EPIT1 -> SR |= (1 << 0);
}