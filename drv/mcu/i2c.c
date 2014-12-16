#include <htc.h>
#include "i2c.h"

/*
 * I2C functions for HI-TECH PIC C - master mode only
 */

/*
 * TIMING - see Philips document: THE I2C-BUS SPECIFICATION
 */

void I2C_Init(void)
{
#ifdef I2C_OPTIMIZE
    SCL = 0; SDA = 0;		/* set SDA & SCL to zero. from now on, simply play with tris */
    SCL_DIR = I2C_OUTPUT;	/* set SCL line to an OUPUT */
    SDA_DIR = I2C_INPUT;	/* set SDA line to an INPUT just to be sure! */

    SSPSTAT = 0;			/* make sure SMP and CKE bits be maintained clear */
    SSPCON = 0x20 | I2C_MASTER;	/* config ssp to i2c master mode and enable ssp */
#else
    SMP = 0;				/* make sure this bit be maintained clear */
    CKE = 0;				/* make sure this bit be maintained clear */
    SSPMODE(I2C_MASTER);	/* config ssp to i2c master mode */

    SCL = 0; SDA = 0;		/* set SDA & SCL to zero. from now on, simply play with tris */
    SCL_DIR = I2C_OUTPUT;	/* set SCL line to an OUPUT */
    SDA_DIR = I2C_INPUT;	/* set SDA line to an INPUT just to be sure! */

    SSPEN = 1;				/* synchronous serial port enable */
#endif
}

/*
 * Send stop condition
 * 	  - data low-high while clock high
 */
void I2C_Stop(void)
{
    /* don't assume SCL is high on entry */
    SDA_LOW();					/* ensure data is low first */
    SCL_HIGH();

    __delay_us(I2C_TM_DATA_SU);
    SCL_DIR = I2C_INPUT;		/* float clock high */
    __delay_us(I2C_TM_STOP_SU);
    SDA_HIGH();					/* the low->high data transistion */
    __delay_us(I2C_TM_BUS_FREE);/* bus free time before next start */
    SDA_DIR = I2C_INPUT;		/* float data high */
}

/*
 * Send (re)start condition
 * 	  - ensure data is high then issue a start condition
 * 	  - see also I2C_Start() macro
 */
void I2C_Restart(void)
{
    SCL_LOW();					/* ensure clock is low */
    SDA_HIGH();					/* ensure data is high */

    __delay_us(I2C_TM_DATA_SU);

    SCL_DIR = I2C_INPUT;		/* clock pulse high */
    __delay_us(I2C_TM_SCL_HIGH);

    SDA_LOW();					/* the high->low transition */
    __delay_us(I2C_TM_START_HD);
}

/*
 * Send a byte to the slave
 *	  - returns I2C_ERROR on bus error
 */
u8 I2C_SendByte(u8 byte)
{
    s8 i;

    for (i = 7; i >= 0; i--) {
        SCL_LOW();					/* drive clock low */

        /* data hold time = 0, send data now */
        SDA_DIR = ((byte >> i) & 0x01);
        if ((byte >> i) & 0x01) {	/* bit to send */
            SDA_HIGH();
        } else {
            SDA_LOW();
        }
        __delay_us(I2C_TM_DATA_SU);
        SCL_DIR = I2C_INPUT;		/* float clock high */

        if (I2C_WaitForSCL()) {		/* wait for clock release */
            return I2C_ERROR;		/* bus error */
        }

        __delay_us(I2C_TM_SCL_HIGH);/* clock high time */
    }

    return I2C_OK;
}

/*
 * Check for an acknowledge
 * 	  - returns ack or ~ack, or ERROR if a bus error
 */
s8 I2C_ReadAcknowledge(void)
{
    u8 ack;

    SCL_LOW();						/* make clock is low */
    SDA_DIR = I2C_INPUT;			/* disable data line - listen for ack */
    __delay_us(I2C_TM_SCL_TO_DATA);	/* SCL low to data out valid */
    SCL_DIR = I2C_INPUT;			/* float clock high */
    __delay_us(I2C_TM_DATA_SU);
    ack = SDA;						/* read the acknowledge */

    /* wait for slave to release clock line after processing byte */
    if (I2C_WaitForSCL()) {
        return I2C_ERROR;
    }

    return ack;
}

/*
 * Read a byte from the slave
 * 	  - returns the byte, or I2C_ERROR if a bus error
 */
s16 I2C_ReadByte(void)
{
    s8 i;
    u8 byte = 0;

    for (i = 7; i >= 0; i--) {
        SCL_LOW();					/* drive clock low */
        __delay_us(I2C_TM_SCL_LOW);	/* min clock low  period */
        SDA_DIR = I2C_INPUT;		/* release data line */

        SCL_DIR = I2C_INPUT;		/* float clock high */
        if (I2C_WaitForSCL()) {
            return I2C_ERROR;
        }
        __delay_us(I2C_TM_SCL_HIGH);
        byte = byte << 1;			/* read the next bit */
        byte |= SDA;
    }

    return (s16)byte;
}

/*
 * Send an (~)acknowledge to the slave
 * 	  - status of I2C_LAST implies this is the last byte to be sent
 */
void I2C_SendAcknowledge(u8 status)
{
    SCL_LOW();
    if (status & 0x01) {
        SDA_LOW();					/* drive line low -> more to come */
    } else {
        SDA_HIGH();
    }
    __delay_us(I2C_TM_DATA_SU);
    SCL_DIR = I2C_INPUT;			/* float clock high */
    __delay_us(I2C_TM_SCL_HIGH);
}

/*
 * Send a byte to the slave and acknowledges the transfer
 * 	  - returns I2C_ERROR, ack or ~ack
 */
s8 I2C_PutByte(u8 data)
{
    if (I2C_SendByte(data)) {
        return I2C_ERROR;
    }

    return I2C_ReadAcknowledge();	/* returns ack, ~ack or I2C_ERROR*/
}

/*
 * 	Get a byte from the slave and acknowledges the transfer
 *	  - returns the byte, or I2C_ERROR if a bus error
 */
s16 I2C_GetByte(u8 more)
{
    s16 byte = I2C_ReadByte();

    if (byte == I2C_ERROR) {
        return I2C_ERROR;
    }

    I2C_SendAcknowledge(more);

    return byte;
}

/*
 * Send an array of bytes to the slave and acknowledges the transfer
 * 	  - returns number of bytes not successfully transmitted
 */
s16 I2C_PutString(const u8 *str, u8 length)
{
    s8 error;

    while (length) {
        error = I2C_PutByte(*str);
#ifdef I2C_OPTIMIZE
        if (error) {
            return (error < 0) ? (-(s16)length) : ((s16)length);
        }
#else
        if (error == I2C_ERROR) {
            return -(s16)length;		/* bus error */
        } else {
            if (error) {
                return (s16)length;		/* non acknowledge */
            }
        }
#endif
        str++;
        length--;
    }

    return I2C_OK;						/* everything OK */
}

/*
 * Reads number bytes from the slave, stores them at str and acknowledges the transfer
 * 	  - returns number of bytes not successfully read in
 */
u8 I2C_GetString(u8 *str, u8 number)
{
    s16 byte;

    while (number) {
        byte = I2C_GetByte(number - 1);
        if (byte == I2C_ERROR) {
            return number;				/* bus error */
        } else {
            *str = (u8)byte;
        }
        str++;
        number--;
    }

    return I2C_OK;						/* everything OK */
}

/*
 * Opens communication with a device at address. mode
 * 	indicates I2C_READ or I2C_WRITE.
 * 	  - returns I2C_ERROR, ack or ~ack
 */
u8 I2C_Open(u8 address, u8 mode)
{
    I2C_Start();
    I2C_SendAddress(address, mode);

    return I2C_ReadAcknowledge();	/* returns ack, ~ack or I2C_ERROR*/
}

/*
 * Wait for the clock line to be released by slow slaves
 * 	  - returns I2C_ERROR if SCL was not released after the time out period.
 * 	  - returns I2C_OK if and when SCL released
 */
u8 I2C_WaitForSCL(void)
{
    /* SCL_DIR should be input here */
    if (!SCL) {
        __delay_us(I2C_TM_SCL_TMO);
        /* if the clock is still low -> bus error */
        if (!SCL) {
            return I2C_ERROR;
        }
    }

    return I2C_OK;
}
