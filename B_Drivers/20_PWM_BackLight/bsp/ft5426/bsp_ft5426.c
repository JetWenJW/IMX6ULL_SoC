#include "bsp_ft5426.h"
#include "bsp_gpio.h"
#include "bsp_int.h"
#include "bsp_delay.h"
#include "bsp_i2c.h"
#include "stdio.h"

struct ft5426_dev_struc ft5426_dev;



/* FT5426 Initial */
void ft5426_init(void)
{
    unsigned char reg_val[2];
    /* Pin IO Initial */
    IOMUXC_SetPinMux(IOMUXC_UART5_TX_DATA_I2C2_SCL ,1);             /* Config as I2C2 SCL           */
    IOMUXC_SetPinMux(IOMUXC_UART5_RX_DATA_I2C2_SDA ,1);             /* Config as I2C2 SDA           */

    IOMUXC_SetPinConfig(IOMUXC_UART5_TX_DATA_I2C2_SCL, 0x70B0);     /* Set electronic property(SCL) */
    IOMUXC_SetPinConfig(IOMUXC_UART5_RX_DATA_I2C2_SDA, 0x70B0);     /* Set electronic property(SDA) */
    
    /* Initial int Pin to Ebable interrupt */
    IOMUXC_SetPinMux(IOMUXC_GPIO1_IO09_GPIO1_IO09, 0);              /* Set GPIO1 IO09                  */
    IOMUXC_SetPinConfig(IOMUXC_GPIO1_IO09_GPIO1_IO09, 0xF080);      /* Configure Electronical Property */

    /*  GPIO initial */
    gpio_pin_config_t ctint_config;
    ctint_config.direction = kGPIO_DigitalOutput;
    ctint_config.interruptmode = kGPIO_IntRisingOrFallingEdge;
    gpio_init(GPIO1, 9, &ctint_config); 
    
    /* Enable Interrupt */
    GIC_EnableIRQ(GPIO1_Combined_0_15_IRQn);
    system_register_irqhandler(GPIO1_Combined_0_15_IRQn, gpio1_09_IRQHandler, NULL);
    gpio_enable(GPIO1, 9);

    /* Initial RST Pin Enable Interrupt */
    IOMUXC_SetPinMux(IOMUXC_SNVS_SNVS_TAMPER9_GPIO5_IO09, 0);              /* Set GPIO1 IO09                  */
    IOMUXC_SetPinConfig(IOMUXC_SNVS_SNVS_TAMPER9_GPIO5_IO09, 0x10B0);      /* Configure Electronical Property */
    gpio_pin_config_t ctrst_config;
    ctrst_config.direction = kGPIO_DigitalOutput;
    ctrst_config.outputLogic = 1;
    gpio_init(GPIO5, 9, &ctrst_config); 

    gpio_pinwrite(GPIO5, 9, 0);     /* Reset FT5426       */
    delay_ms(50);
    gpio_pinwrite(GPIO5, 9, 1);     /*Stop Reset FT5426   */
    delay_ms(50);

    /* I2C Initial */
    i2c_init(I2C2);

    /* FT5426 Initial */
    ft5426_read_len(FT5426_ADDR, FT5426_IDGLIB_VERSION, 2, reg_val);
    printf("Touch Firmware Ver:%#X\r\n", ((unsigned short)reg_val[0] << 8) | reg_val[1]);

    ft5426_write_byte(FT5426_ADDR, FT5426_DEVICE_MODE, 0);      /* Configure FT5426 Work on normal Mode */
    ft5426_write_byte(FT5426_ADDR, FT5426_IDG_MODE, 1);         /* FT5426 Work On interupt Mode         */

    ft5426_dev.initflag = FT5426_INIT_FINISHED;
}

/* Call back Function */
void gpio1_09_IRQHandler(unsigned int gicciar, void *param)
{

    if(ft5426_dev.initflag == FT5426_INIT_FINISHED)
    {
        ft5426_touch_tpcoord();
    }
    /* Clear Interrupt Flags */
    gpio_clearIntFlags(GPIO1, 9);
}

/* FT5426 Write 1 Bytes Data */
unsigned char ft5426_write_byte(unsigned char addr, unsigned char reg, unsigned char data)
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
    if(i2c_master_transfer(I2C2, &masterXfer))  status = 1;

    return status;
}

/* FT5426 Read 1 Bytes Data */
unsigned char ft5426_read_byte(unsigned char addr, unsigned char reg)
{
    unsigned char val = 0;
    struct i2c_transfer masterXfer;

    masterXfer.slaveAddress     = addr;
    masterXfer.direction        = kI2C_Read;
    masterXfer.subaddress       = reg;
    masterXfer.subaddressSize   = 1;
    masterXfer.data             = &val;
    masterXfer.dataSize         = 1;
    i2c_master_transfer(I2C2, &masterXfer);
    return val;
}

/* FT5426 Read Data */
void ft5426_read_len(unsigned char addr, unsigned char reg, unsigned char len, unsigned char *buf)
{
    struct i2c_transfer masterXfer;

    masterXfer.slaveAddress     = addr;
    masterXfer.direction        = kI2C_Read;
    masterXfer.subaddress       = reg;
    masterXfer.subaddressSize   = 1;
    masterXfer.data             = buf;
    masterXfer.dataSize         = len;
    i2c_master_transfer(I2C2, &masterXfer);
}

/* Read Touch Coordination */
void ft5426_touch_tpcoord(void)
{
    unsigned char i = 0;
    unsigned char type = 0;
    unsigned char pointbuf[FT5426_XYCOORDREG_NUM];
    ft5426_dev.point_num = ft5426_read_byte(FT5426_ADDR, FT5426_TD_STATUS);

    /* Read Touch Point Data of Register */
    ft5426_read_len(FT5426_ADDR, FT5426_TOUCH1_XH, FT5426_XYCOORDREG_NUM, pointbuf);

    for(i = 0;i < FT5426_XYCOORDREG_NUM; i++)
    {
        unsigned char *buf = &pointbuf[i * 6];
        ft5426_dev.y[i] = ((buf[0] << 8) | buf[1]) & 0xFFF;
        ft5426_dev.x[i] = ((buf[2] << 8) | buf[3]) & 0xFFF;

        type = buf[0] >> 6;

        if(type == FT5426_TOUCH_EVENT_DOWN)
        {

        }
        else if(type == FT5426_TOUCH_EVENT_UP)
        {

        }
        else if(type == FT5426_TOUCH_EVENT_ON)
        {

        }

    }
}