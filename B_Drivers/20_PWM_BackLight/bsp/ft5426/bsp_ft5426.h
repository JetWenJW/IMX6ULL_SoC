#ifndef __BSP_FT5426_H
#define __BSP_FT5426_H

#include "imx6u.h"

/* Define FT5426 Macro */
#define FT5426_ADDR				0X38	/* FT5426 Address		        */

#define FT5426_DEVICE_MODE		0X00 	/* Mode Register 			    */
#define FT5426_IDGLIB_VERSION	0XA1 	/* Firmware Version 			*/
#define FT5426_IDG_MODE			0XA4	/* Interrupt Mode				*/
#define FT5426_TD_STATUS		0X02	/* Touch Status			        */
#define FT5426_TOUCH1_XH		0X03	/* Touch Position
										 * One Touch Point use 4 Register Save position
                                         */

#define FT5426_XYCOORDREG_NUM	30		/* Numerous Register of Touch Point         */
#define FT5426_INIT_FINISHED	1		/* Touch Screen Initial Complete 			*/
#define FT5426_INIT_NOTFINISHED	0		/* Touch Screen Initial incomplete  		*/

#define FT5426_TOUCH_EVENT_DOWN			0x00	/* Press 		*/
#define FT5426_TOUCH_EVENT_UP			0x01	/* Release		*/
#define FT5426_TOUCH_EVENT_ON			0x02	/* Touch 		*/
#define FT5426_TOUCH_EVENT_RESERVED		0x03	/* No Event     */


/* FT5426 Touch Data */
struct ft5426_dev_struc
{
    unsigned char initflag;             /* Initial Touch Screen Status  */
    unsigned char point_num;            /* Numerous of Touch Point      */
    unsigned short x[5];                /* X-axis 5 point               */
    unsigned short y[5];                /* y-axis 5 point               */
};

extern struct ft5426_dev_struc ft5426_dev;

/* Function Declare */
void ft5426_init(void);
void gpio1_09_IRQHandler(unsigned int gicciar, void *param);
void ft5426_read_len(unsigned char addr, unsigned char reg, unsigned char len, unsigned char *buf);
unsigned char ft5426_read_byte(unsigned char addr, unsigned char reg);
unsigned char ft5426_write_byte(unsigned char addr, unsigned char reg, unsigned char data);
void ft5426_touch_tpcoord(void);

#endif