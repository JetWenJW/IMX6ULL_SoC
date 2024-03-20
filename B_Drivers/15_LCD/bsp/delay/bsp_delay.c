#include "bsp_delay.h"
#include "bsp_int.h"
#include "bsp_led.h"



/* Precise Timer Initial */
void delay_init(void)
{
    GPT1 -> CR = 0;
    GPT1 -> CR = (1 << 15);
    while((GPT1 -> CR >> 15) & 0x01);       /*  Software Resrt */

    /* Set GPT1 CLK SRC = ipg_clk = 66MHz */
    GPT1 -> CR |= (1 << 1) | (1 << 6); 

    /* Set Divider */
    GPT1 -> PR = 65;                        /* Divider = 66,Freq = 66MHz/6 = 11MHz */

    GPT1 -> OCR[0] = 0xFFFFFFFF;
#if 0
    /* Set Output Compare Channel 1 */
    GPT1 -> OCR[0] = 1000000/2;             /* Set Intrrupt Priodic = 500ms */
    
    /* Enable Output Compare Channel 1 Interrupt */
    GPT1 ->IR = (1 << 0);

    GIC_EnableIRQ(GPT1_IRQn);
    system_register_irqhandler(GPT1_IRQn, gpt1_IRQHandler, NULL);
    
    /* Enable GPT1 Timer */
    GPT1 -> CR |= (1 << 0);
#endif
    /* Enable GPT1 Timer */
    GPT1 -> CR |= (1 << 0);
}
#if 0
/*GPT1 IRQ Handler */
void gpt1_IRQHandler(unsigned int gicciar, void *param)
{
    static unsigned char state = 0;
    if((GPT1 -> SR) & (1 << 0))
    {
        state = !state;
        led_switch(LED0, state);
    }
    GPT1 -> SR |= (1 << 0);                 /* Clear Interrput Flags*/
}
#endif

/*  delay us */
void delay_us(unsigned int us)
{
    unsigned long oldcnt, newcnt;
    unsigned long tcntvalue = 0;
    oldcnt = GPT1 -> CNT;
    while(1)
    {
        newcnt = GPT1 -> CNT;
        if(newcnt != oldcnt)
        {
            if(newcnt > oldcnt)
            {
                tcntvalue += newcnt - oldcnt;
            }
            else
            {
                tcntvalue += 0xFFFFFFFF - oldcnt + newcnt;
            }
            oldcnt = newcnt;
            if(tcntvalue > us)  break; 
        }
    }
}

/* delay ms */
void delay_ms(unsigned int ms)
{
    int i = 0;
    for( i = 0; i < ms; i++)
    {
        delay_us(1000);
    }
}


/* Short delay */
void delay_short(volatile unsigned int n)
{
    while(n--){}
}

/* Delay 1ms */
void delay(volatile unsigned int n)
{
    while(n--)
    {
        delay_short(0x7FF);
    }
}