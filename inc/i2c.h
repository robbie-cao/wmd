#ifndef	__I2C_H__
#define __I2C_H__

#include "types.h"
#include "config.h"

#define I2C_OPTIMIZE

/*
 *	SDA (data) and SCL (clock) bits
 *	
 *	Special note!!!
 *	
 *	If the clock and data lines are in the same port, you will need
 *	to beware of the Read/Modify/Write issue in the PIC - since
 *	a bit set or clear on any one bit in a port will read and write
 *	back all other bits. 
 */

/* Uncomment the next line to use the PIC's SSP Module*/
#define I2C_MODULE		1

#ifdef I2C_MODULE
/* I2C module uses PORT B */
#define SCL				RB6			/* clock on port B bit 6 */
#define SCL_DIR			TRISB6
#define SDA				RB4			/* data on port B bit 4 */
#define SDA_DIR			TRISB4
#define I2CTRIS			TRISB

#define I2C_SLAVE7		0b0110		/* I2C slave mode, 7-bit address */
#define I2C_SLAVE10		0b0111		/* I2C slave mode, 10-bit address */
#define I2C_MASTER		0b1011		/* I2C firmware controlled master mode (slave idle) */
#define I2C_SLAVE7_INT	0b1110		/* I2C slave mode, 7-bit address with start/stop bit interrupts */
#define I2C_SLAVE10_INT	0b1111		/* I2C slave mode, 10-bit address with start/stop bit interrupts */
#define SSPMODE(mod)	(SSPCON &= 0xF0, SSPCON |= (mod))

#else
/* Change port as required - defaults to port B */
#define	SCL	            RB6			/* clock on port B bit 6 */
#define	SCL_DIR		    TRISB6

#define SDA             RB4			/* data on port B bit 4 */
#define	SDA_DIR		    TRISB4
#define I2CTRIS         TRISB

#endif

#define M_SDA_INP		0x02
#define M_SDA_OUT		0xFD
#define M_SCL_INP		0x04
#define M_SCL_OUT		0xFB

#define I2C_INPUT		1			/* data direction input */
#define I2C_OUTPUT		0			/* data direction output */

#define I2C_READ		0b1			/* read bit used with address */
#define I2C_WRITE		0b0			/* write bit used with address */

#define I2C_OK			0
#define I2C_ERROR		(-1)

#define I2C_LAST		0			/* SendAck: no more bytes to send */
#define I2C_MORE		1			/* SendAck: more bytes to send */

#define I2C_Start()				I2C_Restart()
#define I2C_WriteTo(address)	I2C_Open((address), I2C_WRITE)
#define I2C_ReadFrom(address)	I2C_Open((address), I2C_READ)

#ifdef I2C_MODULE
#define SCL_HIGH()		SCL_DIR = I2C_INPUT
#define SCL_LOW()		SCL_DIR = I2C_OUTPUT
#define SDA_HIGH()		SDA_DIR = I2C_INPUT
#define SDA_LOW()		SDA_DIR = I2C_OUTPUT
#else
#define SCL_HIGH()		(SCL = 1, SCL_DIR = I2C_OUTPUT)
#define SCL_LOW()		(SCL = 0, SCL_DIR = I2C_OUTPUT)
#define SDA_HIGH()		(SDA = 1, SDA_DIR = I2C_OUTPUT)
#define SDA_LOW()		(SDA = 0, SDA_DIR = I2C_OUTPUT)
#endif

/*
 * Timings for the i2c bus. Times are rounded up to the nearest
 * micro second.
 */

#define I2C_TM_BUS_FREE		5
#define	I2C_TM_START_SU		5
#define I2C_TM_START_HD		4
#define I2C_TM_SCL_LOW		5
#define	I2C_TM_SCL_HIGH		4
#define I2C_TM_DATA_SU		1
#define I2C_TM_DATA_HD		0
#define I2C_TM_SCL_TO_DATA	4	/* SCL low to data valid */
#define	I2C_TM_STOP_SU		4
#define I2C_TM_SCL_TMO		10	/* clock time out */

/*
 *	I2C module initialization
 */
void	I2C_Init(void);
/*
 *	Check for an acknowledge
 *	  - returns ack or ~ack, or ERROR if a bus error
 */
s8		I2C_ReadAcknowledge(void);
/*
 *	Send an address and data direction to the slave
 *	  - 7-bit address
 * 	  - direction (0 = write, 1 = read)
 */
#define I2C_SendAddress(addr, rw)	I2C_SendByte(((addr) << 1) | ((rw) ? 1 : 0))
/*
 *	Send a byte to the slave
 *	  - returns I2C_ERROR on bus error
 */
u8		I2C_SendByte(u8 byte);
/*
 *	Read a byte from the slave
 *	  - returns the byte, or I2C_ERROR if a bus error
 */
s16		I2C_ReadByte(void);
/*
 *	Send (re)start condition
 *	  - ensure data is high then issue a start condition
 *	  - see also I2C_Start() macro
 */
void	I2C_Restart(void);
/*
 *	Send stop condition
 * 	  - data low-high while clock high
 */
void	I2C_Stop(void);
/*
 *	Send an (~)acknowledge to the slave
 *	  - status of I2C_LAST implies this is the last byte to be sent
 */
void	I2C_SendAcknowledge(u8 status);
/*
 *	Send a byte to the slave and acknowledges the transfer
 *	  - returns I2C_ERROR, ack or ~ack
 */
s8		I2C_PutByte(u8 data);
/*
 *	Get a byte from the slave and acknowledges the transfer
 *	  - returns the byte, or I2C_ERROR if a bus error
 */
s16		I2C_GetByte(u8 more);
/*
 *	Opens communication with a device at address. mode
 *	indicates I2C_READ or I2C_WRITE.
 * 	  - returns I2C_ERROR, ack or ~ack
 */
u8		I2C_Open(u8 address, u8 mode);
/*
 *	Reads number bytes from the slave, stores them at str and acknowledges the transfer
 *	  - returns number of bytes not successfully read in
 */
u8		I2C_GetString(u8 *str, u8 number);
/*
 *	Send an array of bytes to the slave and acknowledges the transfer
 *	  - returns number of bytes not successfully transmitted
 */
s16		I2C_PutString(const u8 *str, u8 length);
/*
 *	Wait for the clock line to be released by slow slaves
 *	  - returns I2C_ERROR if SCL was not released after the
 *	time out period.
 *	  - returns I2C_OK if and when SCL released
 */
u8		I2C_WaitForSCL(void);

#endif /* __I2C_H__ */
