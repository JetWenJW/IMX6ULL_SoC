#include "bsp_i2c.h"


/* I2C Initial */
void i2c_init(I2C_Type *base)
{
    base -> I2CR &= ~(1 << 7);  /* Disable I2C */
    base -> IFDR = 0x15;        /* Set I2C Bus CLK = 103.125KHz */
    base -> I2CR |= (1 << 7);   /* Enable I2C */
}

/* Start Signal */
unsigned char i2c_master_start(I2C_Type *base, unsigned char address, enum i2c_direction direction)
{
    if(base -> I2SR & (1 << 5)) /* I2C Busy */
    {
        return 1;
    }
    /* Master Send Mode */
    base -> I2CR |= (1 << 5) | (1 << 4);

    /* Send (StartSignal & Slave Address) */
    base -> I2DR = ((unsigned int)address << 1) | ((direction == kI2C_Read) ? 1 : 0);
    return 0;
}

/* Stop Signal */
unsigned  char i2c_master_stop(I2C_Type *base)
{
    unsigned short timeout = 0xFFFF;

    /* Clear I2C bit[5:3] */
    base -> I2CR &=~(1 << 5) | (1 << 4) | (1 << 3);

    while((base -> I2SR & (1 << 5)))                /* Wait for I2C IDLE */
    {
        if(timeout == 0) return I2C_STATUS_TIMEOUT; /* Time out Return   */
    }
    return I2C_STATUS_OK;
}

/* Repeat Start */
unsigned char i2c_master_repeat_start(I2C_Type *base, unsigned char address, enum i2c_direction direction)
{
    /* I2C must IDLE & work on Slave */
    if(base -> I2SR & (1 << 5) && (((base -> I2CR) & (1 << 5)) == 0)) return 1;

    base -> I2CR |= (1 << 4) | (1 << 2);
    base -> I2DR = ((unsigned int)address << 1) | ((direction == kI2C_Read) ? 1 : 0);
    return I2C_STATUS_OK;
}

/* Error Check & Clear Function */
unsigned char i2c_check_and_clear_error(I2C_Type *base, unsigned int status)
{
    /* Check Arbitration Lost or not */
    if(status & (1 << 4))
    {
        base -> I2SR &=~(1 << 4);
        base -> I2CR &=~(1 << 7);
        return I2C_STATUS_ARBITRATIONLOST;
    }
    else if(status & (1 << 0))
    {
        return I2C_STATUS_NAK;
    }
    return I2C_STATUS_OK;
}

/* Send Data */
void i2c_master_write(I2C_Type *base, const unsigned char *buf, unsigned int size)
{
    while(!((base ->I2SR) & (1 << 7)));             /* Wait Tranfer complete  */
    base -> I2SR &=~(1 << 1);                       /* Clear Interrupt Flag   */

    while(size--)
    {
        base -> I2DR = *buf++;                      /* Send Data to Buffer    */
        while(!((base -> I2SR) & (1 << 1)));          /* Wait transfer complete */
        base -> I2SR &=~ (1 << 1);

        /* Check ACK (if Error break) */
        if(i2c_check_and_clear_error(base, base ->I2SR))   break;
    }
    base -> I2SR &=~ (1 << 1);
    i2c_master_stop(base);
}

/* Read Data */
void i2c_master_read(I2C_Type *base, unsigned char *buf, unsigned int size)
{
    volatile uint8_t dummy = 0;/* Dummy Read */
    dummy++; 

    while(!(base ->I2SR & (1 << 7))); /* Wait Data Transfer complete */
    base -> I2SR &=~ (1 << 1);
    base -> I2CR &=~ ((1 << 4) | (1 << 3));

    if(size == 1)
    {
        base -> I2CR |= (1 << 3);               /* NACK */
    }

    dummy = base -> I2DR;                       /* DUMMY READ */

    while(size--)
    {
        while(!(base -> I2SR & (1 << 1)));      /* Wait Data transfer Complete */
        base -> I2SR &=~ (1 << 1);

        if(size == 0)                           /* Data tranfer finished       */
        {
            i2c_master_stop(base);
        }
        if(size ==1)                            /* Send NACK to ACK/NACK bit   */
        {
            base -> I2CR |= (1 << 3);           /* NACK */
        }
        *buf++ = base -> I2DR;                  /* Save Data to Buffer         */
    }
}

/* I2C Master Transfer data */
unsigned char i2c_master_transfer(I2C_Type *base, struct i2c_transfer *xfer)
{
	unsigned char ret = 0;
	 enum i2c_direction direction = xfer->direction;	

	base->I2SR &= ~((1 << 1) | (1 << 4));			/* Clear Flag */

	/* Wait Data transfer complete */
	while(!((base->I2SR >> 7) & 0X1)){}; 

    /* MasterRead need to send address,so change direction to write */
    if ((xfer->subaddressSize > 0) && (xfer->direction == kI2C_Read))
    {
        direction = kI2C_Write;
    }

	ret = i2c_master_start(base, xfer->slaveAddress, direction);    /* transfer strt */
    if(ret)
    {	
		return ret;
	}

	while(!(base->I2SR & (1 << 1))){};			                	/* Wait Data transfer complete  */


    ret = i2c_check_and_clear_error(base, base->I2SR);	            /* Check error & clear flag     */
    if(ret)
    {
      	i2c_master_stop(base); 						                /* Error happened,Stop tranfer  */
        return ret;
    }
	
    /* Send address */
    if(xfer->subaddressSize)
    {
        do
        {
			base->I2SR &= ~(1 << 1);			/* Clear flag */
            xfer->subaddressSize--;				/* address Size */
			
            base->I2DR =  ((xfer->subaddress) >> (8 * xfer->subaddressSize)); 
  
			while(!(base->I2SR & (1 << 1)));  	/* Wait Data transfer complete  */

            /* Check error & clear flag */
            ret = i2c_check_and_clear_error(base, base->I2SR);
            if(ret)
            {
             	i2c_master_stop(base); 				/* Send stop bit */
             	return ret;
            }  
        } while ((xfer->subaddressSize > 0) && (ret == I2C_STATUS_OK));

        if(xfer->direction == kI2C_Read) 		/* Read Data */
        {
            base->I2SR &= ~(1 << 1);			/* Clear bit */
            i2c_master_repeat_start(base, xfer->slaveAddress, kI2C_Read);     /* Send Repeat Start & Slave Address */
    		while(!(base->I2SR & (1 << 1))){};/* Wait Data transfer complete  */

            /* Check error & clear flag */
			ret = i2c_check_and_clear_error(base, base->I2SR);
            if(ret)
            {
             	ret = I2C_STATUS_ADDRNAK;
                i2c_master_stop(base); 		/* Send Stop bit */
                return ret;  
            }
           	          
        }
    }	

    /* Send Data */
    if ((xfer->direction == kI2C_Write) && (xfer->dataSize > 0))
    {
    	i2c_master_write(base, xfer->data, xfer->dataSize);
	}

    /* Read Data */
    if ((xfer->direction == kI2C_Read) && (xfer->dataSize > 0))
    {
       	i2c_master_read(base, xfer->data, xfer->dataSize);
	}
	return 0;	
}











