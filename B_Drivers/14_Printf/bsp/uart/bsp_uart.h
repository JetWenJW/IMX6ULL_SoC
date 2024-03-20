#ifndef __BSP_UART_H
#define __BSP_UART_H

#include "imx6u.h"

/* Function Declare */
void uart_io_init(void);
void uart_init(void);
void uart_disable(UART_Type *base);
void uart_enable(UART_Type *base);
void uart_softreset(UART_Type *base);
void put_char(unsigned char c);
void put_string(char *str);
unsigned char get_char(void);
void uart_setbaudrate(UART_Type *base, unsigned int baudrate, unsigned int srcclock_hz);

/* Define This Function avoid cpmpile failed */
void raise(int sig_nr);

#endif