#ifndef __BSP_I2C_H
#define __BSP_I2C_H
#include "imx6u.h"

/* Define I2C STATE Macro */
#define I2C_STATUS_OK                   (0)
#define I2C_STATUS_BUSY                 (1)
#define I2C_STATUS_IDLE                 (2)
#define I2C_STATUS_NAK                  (3)
#define I2C_STATUS_ARBITRATIONLOST      (4)
#define I2C_STATUS_TIMEOUT              (5)
#define I2C_STATUS_ADDRNAK              (6)

/* Transfer Direction */
enum i2c_direction
{
    kI2C_Write = 0x0,   /* Master Write */
    kI2C_Read  = 0x1    /* Master Read  */
};

/* Master Transfer */
struct i2c_transfer
{
    unsigned char slaveAddress;      	/* 7位7bit Slave Address            */
    enum i2c_direction direction; 		/* Data transfer direction 			*/
    unsigned int subaddress;       		/* Register Address			        */
    unsigned char subaddressSize;    	/* Register Address	Size 			*/
    unsigned char *volatile data;    	/* Data Buffer 			            */
    volatile unsigned int dataSize;  	/* Data Buffer Size 			    */
};

/* Function Declare */
void i2c_init(I2C_Type *base);
unsigned  char i2c_master_stop(I2C_Type *base);
unsigned char i2c_master_start(I2C_Type *base, unsigned char address, enum i2c_direction direction);
unsigned char i2c_master_repeat_start(I2C_Type *base, unsigned char address, enum i2c_direction direction);
unsigned char i2c_check_and_clear_error(I2C_Type *base, unsigned int status);
void i2c_master_read(I2C_Type *base, unsigned char *buf, unsigned int size);
unsigned char i2c_master_transfer(I2C_Type *base, struct i2c_transfer *xfer);

#endif