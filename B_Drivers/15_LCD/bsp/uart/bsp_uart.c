#include "bsp_uart.h"
#include "bsp_gpio.h"


/* Uart initial ,Baud = 115200 */
void uart_init(void)
{
    /* UART IO Iniitial */
    uart_io_init();

    /* Initial UART1 */
    uart_disable(UART1);            /* Disable UART1    */
    uart_softreset(UART1);          /* Soft Reset UART1 */

    UART1 -> UCR1 = 0;              /* Clear UCR1 Register */

    UART1 -> UCR1 &=~ (1 << 14);    /* Disable UART Baud AutoSet */

    /* Configure UART1 DATA, Parity, Stop bit */
    UART1 -> UCR2 |= (1 << 1) | (1 << 2) | (1 << 5) | (1 << 14); 
    UART1 -> UCR3 |= (1 << 2);

#if 0    
    /* Set Baudrate = 115200 */
    UART1 -> UFCR &=~ (7 << 7);     /* Clear FRDIV */
    UART1 -> UFCR = (5 << 7);       /* Divider = 1 */
    UART1 -> UBIR = 71;
    UART1 -> UBMR = 3124;
#endif

    uart_setbaudrate(UART1,115200,80000000);

    /* Enable UART1 */
    uart_enable(UART1);
}

/* This Fintion Provide by NXP SDK*/
void uart_setbaudrate(UART_Type *base, unsigned int baudrate, unsigned int srcclock_hz)
{
    uint32_t numerator = 0u;		
    uint32_t denominator = 0U;		
    uint32_t divisor = 0U;
    uint32_t refFreqDiv = 0U;
    uint32_t divider = 1U;
    uint64_t baudDiff = 0U;
    uint64_t tempNumerator = 0U;
    uint32_t tempDenominator = 0u;

    /* get the approximately maximum divisor */
    numerator = srcclock_hz;
    denominator = baudrate << 4;
    divisor = 1;

    while (denominator != 0)
    {
        divisor = denominator;
        denominator = numerator % denominator;
        numerator = divisor;
    }

    numerator = srcclock_hz / divisor;
    denominator = (baudrate << 4) / divisor;

    /* numerator ranges from 1 ~ 7 * 64k */
    /* denominator ranges from 1 ~ 64k */
    if ((numerator > (UART_UBIR_INC_MASK * 7)) || (denominator > UART_UBIR_INC_MASK))
    {
        uint32_t m = (numerator - 1) / (UART_UBIR_INC_MASK * 7) + 1;
        uint32_t n = (denominator - 1) / UART_UBIR_INC_MASK + 1;
        uint32_t max = m > n ? m : n;
        numerator /= max;
        denominator /= max;
        if (0 == numerator)
        {
            numerator = 1;
        }
        if (0 == denominator)
        {
            denominator = 1;
        }
    }
    divider = (numerator - 1) / UART_UBIR_INC_MASK + 1;

    switch (divider)
    {
        case 1:
            refFreqDiv = 0x05;
            break;
        case 2:
            refFreqDiv = 0x04;
            break;
        case 3:
            refFreqDiv = 0x03;
            break;
        case 4:
            refFreqDiv = 0x02;
            break;
        case 5:
            refFreqDiv = 0x01;
            break;
        case 6:
            refFreqDiv = 0x00;
            break;
        case 7:
            refFreqDiv = 0x06;
            break;
        default:
            refFreqDiv = 0x05;
            break;
    }
    /* Compare the difference between baudRate_Bps and calculated baud rate.
     * Baud Rate = Ref Freq / (16 * (UBMR + 1)/(UBIR+1)).
     * baudDiff = (srcClock_Hz/divider)/( 16 * ((numerator / divider)/ denominator).
     */
    tempNumerator = srcclock_hz;
    tempDenominator = (numerator << 4);
    divisor = 1;
    /* get the approximately maximum divisor */
    while (tempDenominator != 0)
    {
        divisor = tempDenominator;
        tempDenominator = tempNumerator % tempDenominator;
        tempNumerator = divisor;
    }
    tempNumerator = srcclock_hz / divisor;
    tempDenominator = (numerator << 4) / divisor;
    baudDiff = (tempNumerator * denominator) / tempDenominator;
    baudDiff = (baudDiff >= baudrate) ? (baudDiff - baudrate) : (baudrate - baudDiff);

    if (baudDiff < (baudrate / 100) * 3)
    {
        base->UFCR &= ~UART_UFCR_RFDIV_MASK;
        base->UFCR |= UART_UFCR_RFDIV(refFreqDiv);
        base->UBIR = UART_UBIR_INC(denominator - 1);  
        base->UBMR = UART_UBMR_MOD(numerator / divider - 1);
    }
}

/* Initial UART IO Pin port */
void uart_io_init(void)
{
    IOMUXC_SetPinMux(IOMUXC_UART1_TX_DATA_UART1_TX, 0);             /* Set UART1_TX */
    IOMUXC_SetPinMux(IOMUXC_UART1_RX_DATA_UART1_RX, 0);             /* Set UART1_RX */

    IOMUXC_SetPinConfig(IOMUXC_UART1_TX_DATA_UART1_TX, 0x10B0);     /* Set UART_I TX IO Property  */
    IOMUXC_SetPinConfig(IOMUXC_UART1_RX_DATA_UART1_RX, 0x10B0);     /* Set UART_I RX IO Property  */
}

/* Disable UART */
void uart_disable(UART_Type *base)
{
    base -> UCR1 &=~ (1 << 0);
}

/* Enable UART */
void uart_enable(UART_Type *base)
{
    base -> UCR1 |= (1 << 0);
}

/* UART Software Reset */
void uart_softreset(UART_Type *base)
{
    base -> UCR2 &=~ (1 << 0);                  /* Clear UART-> UCR2 to Reset UART */
    while((base -> UCR2 & 0x1 ) == 0);         /* Wait Reset Finished */
}

/* Send a Char */
void put_char(unsigned char c)
{
    while(((UART1 -> USR2 >> 3) & 0x01) == 0);  /* Wait for sending complete */
    UART1 ->UTXD = c & 0xFF;                    /* Send Data */
}

/* Send a String */
void put_string(char *str)
{
    char *p = str;
    while(*p)   put_char(*p++);
}

/* Receive a Char */
unsigned char get_char(void)
{
    while((UART1 -> USR2 & 0x01) == 0);       /* Wait for Recive complete */
    return UART1 -> URXD;                       /* return Data              */
}

/* Define This Function avoid cpmpile failed */
void raise(int sig_nr)
{

}