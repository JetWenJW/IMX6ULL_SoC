#ifndef __BSP_LCD_H
#define __BSP_LCD_H

#include "imx6u.h"

/* Define Screen ID */
#define ATK4342     0x4342
#define ATK4384     0x4384
#define ATK7084     0x7084
#define ATK7016     0x7016
#define ATK1018     0x1018

/* Colour */
#define LCD_BLUE		  0x000000FF
#define LCD_GREEN		  0x0000FF00
#define LCD_RED 		  0x00FF0000
#define LCD_CYAN		  0x0000FFFF
#define LCD_MAGENTA 	  0x00FF00FF
#define LCD_YELLOW		  0x00FFFF00
#define LCD_LIGHTBLUE	  0x008080FF
#define LCD_LIGHTGREEN	  0x0080FF80
#define LCD_LIGHTRED	  0x00FF8080
#define LCD_LIGHTCYAN	  0x0080FFFF
#define LCD_LIGHTMAGENTA  0x00FF80FF
#define LCD_LIGHTYELLOW   0x00FFFF80
#define LCD_DARKBLUE	  0x00000080
#define LCD_DARKGREEN	  0x00008000
#define LCD_DARKRED 	  0x00800000
#define LCD_DARKCYAN	  0x00008080
#define LCD_DARKMAGENTA   0x00800080
#define LCD_DARKYELLOW	  0x00808000
#define LCD_WHITE		  0x00FFFFFF
#define LCD_LIGHTGRAY	  0x00D3D3D3
#define LCD_GRAY		  0x00808080
#define LCD_DARKGRAY	  0x00404040
#define LCD_BLACK		  0x00000000
#define LCD_BROWN		  0x00A52A2A
#define LCD_ORANGE		  0x00FFA500
#define LCD_TRANSPARENT   0x00000000

/* LCD Address Start */
#define LCD_FRAMBUF_ADDR       (0x89000000)
/* LCD type Define struct */
struct tftlcd_typedef
{
    unsigned short height;      /* Screen height        */
    unsigned short width;       /* Screen height        */
    unsigned char  pixsize;     /* Pixal Size           */
    unsigned short vspw;
    unsigned short vbpd;
    unsigned short vfpd;
    unsigned short hspw;
    unsigned short hbpd;
    unsigned short hfpd;
    unsigned int   framebuffer; /* Panel Initial Address */
    unsigned int   forecolour;   /* Front colour          */
    unsigned int   backcolour;  /* Backgroung colour     */
};

extern struct tftlcd_typedef tftlcd_dev;

/* Function Declare */
unsigned short lcd_read_panelid(void);
void lcd_init(void);
void lcdgpio_init(void);
void lcd_enable(void);
void lcd_reset(void);
void lcd_noreset(void);
void lcdclk_init(unsigned char loopDiv, unsigned char prediv, unsigned char div);
void lcd_drawpoint(unsigned short x, unsigned short y, unsigned int colour);
unsigned int lcd_readpoint(unsigned short x, unsigned short y);
void lcd_clear(unsigned int colour);
void lcd_fill(unsigned short x0, unsigned short y0, unsigned short x1, unsigned short y1, unsigned int color);






#endif