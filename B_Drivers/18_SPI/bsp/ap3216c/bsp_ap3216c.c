#include "bsp_ap3216c.h"
#include "bsp_i2c.h"
#include "bsp_gpio.h"
#include "bsp_delay.h"


/* AP3216C Initial */
unsigned char ap3216c_init(void)
{

    unsigned char value = 0;
    /* Pin IO Initial */
    IOMUXC_SetPinMux(IOMUXC_UART4_TX_DATA_I2C1_SCL ,1);             /* Config as I2C1 SCL           */
    IOMUXC_SetPinMux(IOMUXC_UART4_RX_DATA_I2C1_SDA ,1);             /* Config as I2C1 SDA           */

    IOMUXC_SetPinConfig(IOMUXC_UART4_TX_DATA_I2C1_SCL,0x70B0);      /* Set electronic property(SCL) */
    IOMUXC_SetPinConfig(IOMUXC_UART4_RX_DATA_I2C1_SDA,0x70B0);      /* Set electronic property(SDA) */

    /* I2C Initial */
    i2c_init(I2C1);

    /* AP3216C Initial */
    ap3216c_writeonebyte(AP3216C_ADDR, AP3216C_SYSTEMCONG, 0x4);    /* Reset AP3216C                */
    delay_ms(50);
    ap3216c_writeonebyte(AP3216C_ADDR, AP3216C_SYSTEMCONG, 0x3);    
    value = ap3216c_readonebyte(AP3216C_ADDR, AP3216C_SYSTEMCONG);  

    if(value == 0x3) return 0;
    else             return 1;
}

/* AP3216C Read 1Byte Data */
unsigned char ap3216c_readonebyte(unsigned char addr, unsigned char reg)
{
    unsigned char val = 0;
    struct i2c_transfer masterXfer;

    masterXfer.slaveAddress     = addr;
    masterXfer.direction        = kI2C_Read;
    masterXfer.subaddress       = reg;
    masterXfer.subaddressSize   = 1;
    masterXfer.data             = &val;
    masterXfer.dataSize         = 1;
    i2c_master_transfer(I2C1, &masterXfer);
    return val;
}


/* AP3216C Write 1Byte Data */
unsigned char ap3216c_writeonebyte(unsigned char addr, unsigned char reg, unsigned char data)
{
    unsigned char writedata = data;
    unsigned char status    = 0;
    struct i2c_transfer masterXfer;

    masterXfer.slaveAddress     = addr;
    masterXfer.direction        = kI2C_Write;
    masterXfer.subaddress       = reg;
    masterXfer.subaddressSize   = 1;
    masterXfer.data             = &writedata;
    masterXfer.dataSize         = 1;
    /* Error Happen */
    if(i2c_master_transfer(I2C1, &masterXfer))  status = 1;

    return status;
}

/* AP3216C Read Data */
void ap3216c_readData(unsigned short *ir, unsigned short *ps, unsigned short *als)
{
    unsigned char buf[6];
    unsigned char i = 0;

    for(i = 0;i < 6; i++)
    {
        buf[i] = ap3216c_readonebyte(AP3216C_ADDR,AP3216C_IRDATALOW + i);
    }
    if(buf[0] &0x80)/* Ir & PS data invalid */
    {
        *ir = 0;
        *ps = 0;
    }
    else            /* Ir & PS data Valid */
    {
        *ir = ((unsigned short)buf[1] << 2) | (buf[0] & 0x03);
        *ps = (((unsigned short)buf[5] & 0x3F) << 4) | (buf[4] & 0x0F);
    }
    *als = ((unsigned short)buf[3] << 8) | buf[2] ;
}


















