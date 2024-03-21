#include "bsp_lcd.h"
#include "bsp_gpio.h"
#include "stdio.h"
#include "bsp_delay.h"


/* Panel variable */
struct tftlcd_typedef tftlcd_dev;
/* TFTLCD Initial */
void lcd_init(void)
{
    unsigned short lcdid = 0;
    lcdid = lcd_read_panelid();
    printf("LCD ID = %#X\r\n",lcdid);

    /* Initial LCD_IO  */
    lcdgpio_init();
    lcd_reset();        /* Reset    */
    delay_ms(10);
    lcd_noreset();      /* No-reset */

    /* Each Panel ID */
    if(lcdid == ATK4342)
    {
        tftlcd_dev.height    = 272;
        tftlcd_dev.width     = 480;
        tftlcd_dev.vspw      = 0;
        tftlcd_dev.vbpd      = 8;
        tftlcd_dev.vfpd      = 8;
        tftlcd_dev.hspw      = 0;
        tftlcd_dev.hbpd      = 40;
        tftlcd_dev.hfpd      = 5;
        lcdclk_init(27, 8, 8);                    /* Set Pixal Clock 10.1MHz  */    

    }
    else if(lcdid == ATK4384)
    {
        tftlcd_dev.height    = 480;
        tftlcd_dev.width     = 800;
        tftlcd_dev.vspw      = 3;
        tftlcd_dev.vbpd      = 32;
        tftlcd_dev.vfpd      = 13;
        tftlcd_dev.hspw      = 48;
        tftlcd_dev.hbpd      = 88;
        tftlcd_dev.hfpd      = 40;
        lcdclk_init(42, 4, 8);                    /* Set Pixal Clock 31.5MHz  */    
        
    }
    else if(lcdid == ATK7084)
    {
        tftlcd_dev.height    = 480;
        tftlcd_dev.width     = 800;
        tftlcd_dev.vspw      = 1;
        tftlcd_dev.vbpd      = 23;
        tftlcd_dev.vfpd      = 22;
        tftlcd_dev.hspw      = 1;
        tftlcd_dev.hbpd      = 46;
        tftlcd_dev.hfpd      = 210;   
        lcdclk_init(34, 3, 7);                    /* Set Pixal Clock 34.2MHz  */    
     
    }
    else if(lcdid == ATK7016)
    {
        tftlcd_dev.height    = 600;
        tftlcd_dev.width     = 1024;
        tftlcd_dev.vspw      = 3;
        tftlcd_dev.vbpd      = 20;
        tftlcd_dev.vfpd      = 12;
        tftlcd_dev.hspw      = 20;
        tftlcd_dev.hbpd      = 140;
        tftlcd_dev.hfpd      = 160;    
        lcdclk_init(32, 3, 5);                    /* Set Pixal Clock 51.2MHz  */    
    }
    tftlcd_dev.pixsize = 4;                       /* Each Pixel is 4 Bytes    */
    tftlcd_dev.framebuffer = LCD_FRAMBUF_ADDR;
    tftlcd_dev.forecolour  = LCD_WHITE;           /* Front Colour is white    */
    tftlcd_dev.backcolour  = LCD_BLACK;           /* Back Colour is Black     */

    /* Configure LCDIF Control Port */
    LCDIF -> CTRL = 0;
    LCDIF -> CTRL |= (1 << 5) | (3 << 8) | (3 << 10) | (1 << 17) | (1 << 19);

    LCDIF ->CTRL1 = 0;
    LCDIF ->CTRL1 |= (7 << 16);

    LCDIF -> TRANSFER_COUNT = 0;
    LCDIF -> TRANSFER_COUNT |= (tftlcd_dev.height << 16) | (tftlcd_dev.width << 0);

    LCDIF -> VDCTRL0 = 0;
    LCDIF -> VDCTRL0 |= (tftlcd_dev.vspw << 0) | (1 << 20) | (1 << 21)
                       | (1 << 24) | (0 << 25) | (0 << 26) | (0 << 27)
                       | (1 << 28) | (0 << 29);
    LCDIF -> VDCTRL1 = tftlcd_dev.vspw + tftlcd_dev.height 
                     + tftlcd_dev.vfpd + tftlcd_dev.vbpd;
     
    LCDIF -> VDCTRL2 = (tftlcd_dev.width + tftlcd_dev.hspw + tftlcd_dev.hfpd + tftlcd_dev.hbpd)
                    | (tftlcd_dev.hspw << 18);

    LCDIF -> VDCTRL3 = (tftlcd_dev.vspw + tftlcd_dev.vbpd) 
                    | ((tftlcd_dev.hbpd + tftlcd_dev.hspw ) << 16);

    LCDIF -> VDCTRL4 = (1 << 18) | (tftlcd_dev.width);

    LCDIF -> CUR_BUF  = (unsigned int)tftlcd_dev.framebuffer;
    LCDIF -> NEXT_BUF = (unsigned int)tftlcd_dev.framebuffer;

    /** LCD Enable **/
    lcd_enable();
    delay_ms(10);
    lcd_clear(LCD_WHITE);
}

/* Pixal ClocK Initial */
/*
 * loopDiv Set DIV_SELECT(Range : 27-54)
 * prediv (Range : 1-8)  
 * div (range : 1-8)
 * LCD CLK = 24 * loopDiv / prediv / div
 */
void lcdclk_init(unsigned char loopDiv, unsigned char prediv, unsigned char div)
{
    /* No Use Float Point Divider */
    CCM_ANALOG -> PLL_VIDEO_NUM     = 0;
    CCM_ANALOG -> PLL_VIDEO_DENOM   = 0;
    CCM_ANALOG -> PLL_VIDEO = (1 << 13) | (2 << 19) | (loopDiv << 0);

    CCM_ANALOG -> MISC2 &=~ (3 << 30);

    CCM -> CSCDR2 &=~ (7 << 15);
    CCM -> CSCDR2 |= (2 << 15);

    CCM -> CSCDR2 &=~ (7 << 12);
    CCM -> CSCDR2 |= ((prediv - 1) << 12);   

    CCM -> CBCMR &=~ (7 << 23);
    CCM -> CBCMR |= ((div - 1) << 23);

    CCM -> CSCDR2 &=~ (7 << 9);
    CCM -> CSCDR2 |= (0 << 9);
}

/* Reset TFTLCD Panel */
void lcd_reset(void)
{
    LCDIF -> CTRL |= (1 << 31);
}

/* No-Reset TFTLCD Panel */
void lcd_noreset(void)
{
    LCDIF -> CTRL &=~ (1 << 31);
}

/* Enable TFTLCD Panel */
void lcd_enable(void)
{
    LCDIF -> CTRL |= (1 << 0);
}



/* Read Screen ID */
unsigned short lcd_read_panelid(void)
{
    unsigned char idx = 0;

    /* Open Switch */
    gpio_pin_config_t lcdio_config;
    IOMUXC_SetPinMux(IOMUXC_LCD_VSYNC_GPIO3_IO03, 0);           /* Set GPIO_IO03Pin */
    IOMUXC_SetPinConfig(IOMUXC_LCD_VSYNC_GPIO3_IO03, 0x10B0);   /* Configure Electronical Property */

    /*  GPIO initial */
    lcdio_config.direction = kGPIO_DigitalOutput;
    lcdio_config.outputLogic = 1;
    gpio_init(GPIO3, 3, &lcdio_config); 

    /* Read Screen ID */
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA07_GPIO3_IO12, 0);           
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA15_GPIO3_IO20, 0);           
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA23_GPIO3_IO28, 0);           

    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA07_GPIO3_IO12, 0x10B0);  
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA15_GPIO3_IO20, 0x10B0);  
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA23_GPIO3_IO28, 0x10B0);  

    lcdio_config.direction = kGPIO_DigitalInput;
    gpio_init(GPIO3, 12, &lcdio_config); 
    gpio_init(GPIO3, 20, &lcdio_config); 
    gpio_init(GPIO3, 28, &lcdio_config); 

    idx = (unsigned char)gpio_pinread(GPIO3, 28);    /* M0 */
    idx |= (unsigned char)gpio_pinread(GPIO3, 20);   /* M1 */
    idx |= (unsigned char)gpio_pinread(GPIO3, 12);   /* M2 */

    if(idx == 0)        return ATK4342;
    else if(idx == 1)   return ATK7084;
    else if(idx == 2)   return ATK7016;
    else if(idx == 4)   return ATK4384;
    else if(idx == 5)   return ATK1018;
    return 0;
}

/*  Panel IO Initial */
void lcdgpio_init(void)
{
    /* IO Setting */
	IOMUXC_SetPinMux(IOMUXC_LCD_DATA00_LCDIF_DATA00,0);
	IOMUXC_SetPinMux(IOMUXC_LCD_DATA01_LCDIF_DATA01,0);
	IOMUXC_SetPinMux(IOMUXC_LCD_DATA02_LCDIF_DATA02,0);
	IOMUXC_SetPinMux(IOMUXC_LCD_DATA03_LCDIF_DATA03,0);
	IOMUXC_SetPinMux(IOMUXC_LCD_DATA04_LCDIF_DATA04,0);
	IOMUXC_SetPinMux(IOMUXC_LCD_DATA05_LCDIF_DATA05,0);
	IOMUXC_SetPinMux(IOMUXC_LCD_DATA06_LCDIF_DATA06,0);
	IOMUXC_SetPinMux(IOMUXC_LCD_DATA07_LCDIF_DATA07,0);
	IOMUXC_SetPinMux(IOMUXC_LCD_DATA08_LCDIF_DATA08,0);
	IOMUXC_SetPinMux(IOMUXC_LCD_DATA09_LCDIF_DATA09,0);
	IOMUXC_SetPinMux(IOMUXC_LCD_DATA10_LCDIF_DATA10,0);
	IOMUXC_SetPinMux(IOMUXC_LCD_DATA11_LCDIF_DATA11,0);
	IOMUXC_SetPinMux(IOMUXC_LCD_DATA12_LCDIF_DATA12,0);
	IOMUXC_SetPinMux(IOMUXC_LCD_DATA13_LCDIF_DATA13,0);
	IOMUXC_SetPinMux(IOMUXC_LCD_DATA14_LCDIF_DATA14,0);
	IOMUXC_SetPinMux(IOMUXC_LCD_DATA15_LCDIF_DATA15,0);
	IOMUXC_SetPinMux(IOMUXC_LCD_DATA16_LCDIF_DATA16,0);
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA17_LCDIF_DATA17,0);
	IOMUXC_SetPinMux(IOMUXC_LCD_DATA18_LCDIF_DATA18,0);
	IOMUXC_SetPinMux(IOMUXC_LCD_DATA19_LCDIF_DATA19,0);
	IOMUXC_SetPinMux(IOMUXC_LCD_DATA20_LCDIF_DATA20,0);
	IOMUXC_SetPinMux(IOMUXC_LCD_DATA21_LCDIF_DATA21,0);
	IOMUXC_SetPinMux(IOMUXC_LCD_DATA22_LCDIF_DATA22,0);
	IOMUXC_SetPinMux(IOMUXC_LCD_DATA23_LCDIF_DATA23,0);

	IOMUXC_SetPinMux(IOMUXC_LCD_CLK_LCDIF_CLK,0);	
	IOMUXC_SetPinMux(IOMUXC_LCD_ENABLE_LCDIF_ENABLE,0);	
	IOMUXC_SetPinMux(IOMUXC_LCD_HSYNC_LCDIF_HSYNC,0);
	IOMUXC_SetPinMux(IOMUXC_LCD_VSYNC_LCDIF_VSYNC,0);          
    

    /* Setting  Electronical Property*/
	IOMUXC_SetPinConfig(IOMUXC_LCD_DATA00_LCDIF_DATA00,0xB9);
	IOMUXC_SetPinConfig(IOMUXC_LCD_DATA01_LCDIF_DATA01,0xB9);
	IOMUXC_SetPinConfig(IOMUXC_LCD_DATA02_LCDIF_DATA02,0xB9);
	IOMUXC_SetPinConfig(IOMUXC_LCD_DATA03_LCDIF_DATA03,0xB9);
	IOMUXC_SetPinConfig(IOMUXC_LCD_DATA04_LCDIF_DATA04,0xB9);
	IOMUXC_SetPinConfig(IOMUXC_LCD_DATA05_LCDIF_DATA05,0xB9);
	IOMUXC_SetPinConfig(IOMUXC_LCD_DATA06_LCDIF_DATA06,0xB9);
	IOMUXC_SetPinConfig(IOMUXC_LCD_DATA07_LCDIF_DATA07,0xB9);
	IOMUXC_SetPinConfig(IOMUXC_LCD_DATA08_LCDIF_DATA08,0xB9);
	IOMUXC_SetPinConfig(IOMUXC_LCD_DATA09_LCDIF_DATA09,0xB9);
	IOMUXC_SetPinConfig(IOMUXC_LCD_DATA10_LCDIF_DATA10,0xB9);
	IOMUXC_SetPinConfig(IOMUXC_LCD_DATA11_LCDIF_DATA11,0xB9);
	IOMUXC_SetPinConfig(IOMUXC_LCD_DATA12_LCDIF_DATA12,0xB9);
	IOMUXC_SetPinConfig(IOMUXC_LCD_DATA13_LCDIF_DATA13,0xB9);
	IOMUXC_SetPinConfig(IOMUXC_LCD_DATA14_LCDIF_DATA14,0xB9);
	IOMUXC_SetPinConfig(IOMUXC_LCD_DATA15_LCDIF_DATA15,0xB9);
	IOMUXC_SetPinConfig(IOMUXC_LCD_DATA16_LCDIF_DATA16,0xB9);
	IOMUXC_SetPinConfig(IOMUXC_LCD_DATA17_LCDIF_DATA17,0xB9);
	IOMUXC_SetPinConfig(IOMUXC_LCD_DATA18_LCDIF_DATA18,0xB9);
	IOMUXC_SetPinConfig(IOMUXC_LCD_DATA19_LCDIF_DATA19,0xB9);
	IOMUXC_SetPinConfig(IOMUXC_LCD_DATA20_LCDIF_DATA20,0xB9);
	IOMUXC_SetPinConfig(IOMUXC_LCD_DATA21_LCDIF_DATA21,0xB9);
	IOMUXC_SetPinConfig(IOMUXC_LCD_DATA22_LCDIF_DATA22,0xB9);
	IOMUXC_SetPinConfig(IOMUXC_LCD_DATA23_LCDIF_DATA23,0xB9);

	IOMUXC_SetPinConfig(IOMUXC_LCD_CLK_LCDIF_CLK,0xB9);
	IOMUXC_SetPinConfig(IOMUXC_LCD_ENABLE_LCDIF_ENABLE,0xB9);
	IOMUXC_SetPinConfig(IOMUXC_LCD_HSYNC_LCDIF_HSYNC,0xB9);
	IOMUXC_SetPinConfig(IOMUXC_LCD_VSYNC_LCDIF_VSYNC,0xB9);
 
    /* Panel LCD Back Light */
 	IOMUXC_SetPinMux(IOMUXC_GPIO1_IO08_GPIO1_IO08, 0);          
	IOMUXC_SetPinConfig(IOMUXC_GPIO1_IO08_GPIO1_IO08, 0x10B0);         /* Back light GPIO pin */
    
    /* Back Light GPIO Initial */
    gpio_pin_config_t BL_config;
    BL_config.direction = kGPIO_DigitalOutput;
    BL_config.outputLogic = 1;
    gpio_init(GPIO1, 8, &BL_config); 
}

/* Draw Point Function */
void lcd_drawpoint(unsigned short x, unsigned short y, unsigned int colour)
{
    *(unsigned int *)((unsigned int)tftlcd_dev.framebuffer +
                      tftlcd_dev.pixsize * (tftlcd_dev.width * y + x)) = colour;
}
/* Read Point  Function */
unsigned int lcd_readpoint(unsigned short x, unsigned short y)
{
    return *(unsigned int *)((unsigned int)tftlcd_dev.framebuffer +
                  tftlcd_dev.pixsize * (tftlcd_dev.width * y + x)); 
}

/* Clear LCD Colour */
void lcd_clear(unsigned int colour)
{
    unsigned int num;
    unsigned int i = 0;
    unsigned int *startaddr = (unsigned int *)tftlcd_dev.framebuffer;
    num = (unsigned int)tftlcd_dev.width + tftlcd_dev.height;

    for(i = 0;i < num; i++)
    {
        startaddr[i] = colour;
    }
}

void lcd_fill(unsigned short x0, unsigned short y0, 
            unsigned short x1, unsigned short y1, unsigned int color)
{ 
    unsigned short x, y;

	if(x0 < 0) x0 = 0;
	if(y0 < 0) y0 = 0;
	if(x1 >= tftlcd_dev.width) x1 = tftlcd_dev.width - 1;
	if(y1 >= tftlcd_dev.height) y1 = tftlcd_dev.height - 1;
	
    for(y = y0; y <= y1; y++)
    {
        for(x = x0; x <= x1; x++)
			lcd_drawpoint(x, y, color);
    }
}
