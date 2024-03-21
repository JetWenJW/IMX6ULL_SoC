#include "bsp_clk.h"

/*
 * You need to follow Reference Manual
 * to Set Clock Tree.
 */

/* Enable Clock */
void clk_enable(void)
{
    CCM -> CCGR0 = 0xFFFFFFFF;
    CCM -> CCGR1 = 0xFFFFFFFF;
    CCM -> CCGR2 = 0xFFFFFFFF;
    CCM -> CCGR3 = 0xFFFFFFFF;
    CCM -> CCGR4 = 0xFFFFFFFF;
    CCM -> CCGR5 = 0xFFFFFFFF;
    CCM -> CCGR6 = 0xFFFFFFFF;
}

/* Clock initial */
void imx6uclk_init(void)
{  
    unsigned int reg = 0;
    
    /* Set imx6ull as 528MHz */
    if(((CCM -> CCSR >> 2) & 0x1) == 0)
    {
        CCM -> CCSR &=~ (1 << 8);       /* Set step_clk = osc_clk =24MHz    */
        CCM -> CCSR |=  (1 << 2);       /* pll1_sw_clk = step_clk = 24MHz   */
    }

    /* Set PLL 0 1056MHz */
    CCM_ANALOG -> PLL_ARM = (1 << 13) | ((88 << 0) & 0x7F);
    CCM -> CACRR = 1;                   /* Set Divider = 2 */
    CCM -> CCSR &=~ (1 << 2);           /* Set pll1_sw_clk = pll1_main_clk = 1056MHz */

    /* Set Total 4 PFD of PLL2 (Accorging to RM) */
    reg = CCM_ANALOG -> PFD_528;
    reg &= ~(0x3F3F3F3F);
    reg |= (32 << 24);                  /* PLL2_PFD3 = 297MHz */
    reg |= (24 << 16);                  /* PLL2_PFD2 = 396MHz */
    reg |= (16 << 8);                   /* PLL2_PFD1 = 594MHz */
    reg |= (27 << 0);                   /* PLL2_PFD0 = 352MHz */
    CCM_ANALOG -> PFD_528 = reg;

    /* Set Total 4 PFD of PLL3 (According to RM) */
    reg = 0;
    reg = CCM_ANALOG -> PFD_480;
    reg &= ~(0x3F3F3F3F);
    reg |= (19 << 24);                  /* PLL3_PFD3 = 454.7MHz */
    reg |= (17 << 16);                  /* PLL3_PFD2 = 508.2MHz */
    reg |= (16 << 8);                   /* PLL3_PFD1 = 540MHz   */
    reg |= (12 << 0);                   /* PLL3_PFD0 = 720MHz   */
    CCM_ANALOG -> PFD_480 = reg;

    /* Set AHB_CLK_ROOT = 132MHz */
    CCM -> CBCMR &=~ (3 << 18);
    CCM -> CBCMR |=  (1 << 18);         /* Set pre_periph clock = PLL2_PFD2 = 396MHz */
    CCM -> CBCDR &=~ (1 << 25);
    while(CCM -> CDHIPR & (1 << 5));    /* According to RM needs to waiting Hand-shake signal  */

#if 0
    CCM -> CBCDR &=~ (7 << 10);         
    CCM -> CBCDR |=  (2 << 10);         /* Divider is 3 */
    while(CCM -> CDHIPR & (1 << 1));    /* According to RM needs to waiting Hand-shake signal  */
#endif

    /* IPG_CLK_ROOT = 66MHz */
    CCM -> CBCDR &=~ (3 << 8);
    CCM -> CBCDR |=  (1 << 8);          /* IPG_CLK_ROOT = AHB _CLK_ROOT/2 = 132/2 = 66MHz */

    /* PERCLK_CLK_ROOT = 66MHz */
    CCM -> CSCMR1 &=~(1 << 6);          /* PER_CLK_ROOT CLK SRC is IPG_CLK = 66MHz        */
    CCM -> CSCMR1 &=~(0x3F << 0);       /* Divider is 1,PERCLK_CLK_ROOT = 66MHz           */

    /* Set UART CLK */
    CCM -> CSCDR1 &=~ (1 << 6);         /* UART_CLK_ROOT = 80MHz */
    CCM -> CSCDR1 &=~ 0x3F;             /* Divider = 1           */


}