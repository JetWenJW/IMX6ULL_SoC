#include "bsp_spi.h"

/* SPI Initial */
void spi_init(ECSPI_Type *base)
{
    base -> CONREG = 0;     /* Clear Register */
    base -> CONREG |= (1 << 0) | (1 << 3) | (1 << 4) | (7 << 20);
    
    base -> CONFIGREG = 0;  /* Clear Register */

    base -> PERIODREG  = 0x2000;
    /* SPI Clock Config (SPI CLK = 6MHz)  */
    base -> CONREG &=~ ((0xF << 12) | (0xF << 8));      /* Clear bit[15:12] & bit[11:8] as 0 */
    base -> CONREG |= (9 << 12);                        /* Divider = 10, 60MHz/10 = 6MHz     */

}

/* SPI Read Write */
unsigned char spich0_readwrite_byte(ECSPI_Type *base, unsigned char txdata)
{
    uint32_t spirxdata = 0;
    uint32_t spitxdata = txdata;

    /* Select SPI Channel 0 */
    base -> CONREG &=~ (3 << 18);                       /* Clear bits */
    base -> CONREG |= (0 << 18);        

    /* Send Data */
    while((base -> STATREG & (1 << 0)) == 0);           /* Wait Send Data Complete */
    base -> TXDATA = spitxdata;

    /* Receive Data */
    while((base -> STATREG & (1 << 3)) == 0);           /* Wait Receive Data Complete */
    spirxdata = base -> RXDATA;

    return spirxdata;
}