#ifndef	__SENSOR_H__
#define __SENSOR_H__

#include "types.h"
#include "i2c.h"

#define SENSOR_OPTIMIZE

#define SENSOR_HIDE_FEAT

#define SENSOR_ADDR			0x10

#define SENSOR_INTL_REG		0x00

#define SENSOR_CMD_PWRUP	0x00
#define SENSOR_CMD_PWRDN	0x01
#define SENSOR_CMD_ST		0x02
#define SENSOR_CMD_BGTST	0x04
#define SENSOR_CMD_TOEN		0x08

/* Sensor initialization */
void	AccSensor_Init(void);

#ifndef SENSOR_OPTIMIZE
/* Power up sensor */
void 	AccSensor_PwrUp(void);
/* Power down senor */
void 	AccSensor_PwrDn(void);
/* Make senor enter sleep mode */
#define AccSensor_Sleep	AccSensor_PwrDn
#ifndef SENSOR_HIDE_FEAT
/* Sensor self-test*/
void 	AccSensor_SelfTest(void);
/* Sensor bandgap test */
void 	AccSensor_BGTest(void);
/* Senor temperature out enable */
void 	AccSensor_TOEN(void);
#endif

/* Get acceleration data from sensor */
s8		AccSensor_GetData(u8* reg, u16 *ax, u16 *ay, u16 *az);

#else

/* Power up sensor */
#define AccSensor_PwrUp()			\
	I2C_WriteTo(SENSOR_ADDR),		\
	I2C_PutByte(SENSOR_INTL_REG),	\
	I2C_PutByte(SENSOR_CMD_PWRUP)

/* Power down senor */
#define AccSensor_PwrDn()			\
	I2C_WriteTo(SENSOR_ADDR),		\
	I2C_PutByte(SENSOR_INTL_REG),	\
	I2C_PutByte(SENSOR_CMD_PWRDN)

/* Make senor enter sleep mode */
#define AccSensor_Sleep	AccSensor_PwrDn

#ifndef SENSOR_HIDE_FEAT
/* Sensor self-test*/
#define AccSensor_SelfTest()		\
	I2C_WriteTo(SENSOR_ADDR),		\
	I2C_PutByte(SENSOR_INTL_REG),	\
	I2C_PutByte(SENSOR_CMD_ST)

/* Sensor bandgap test */
#define AccSensor_BGTest()			\
	I2C_WriteTo(SENSOR_ADDR),		\
	I2C_PutByte(SENSOR_INTL_REG),	\
	I2C_PutByte(SENSOR_CMD_BGTST)

/* Senor temperature out enable */
#define AccSensor_TOEN()			\
	I2C_WriteTo(SENSOR_ADDR),		\
	I2C_PutByte(SENSOR_INTL_REG),	\
	I2C_PutByte(SENSOR_CMD_TOEN)
#endif


/* Get acceleration data from sensor */
#define AccSensor_GetData(reg, ax, ay, az)	{	\
	u8 data[5];									\
	I2C_WriteTo(SENSOR_ADDR);					\
	I2C_PutByte(SENSOR_INTL_REG);				\
	I2C_ReadFrom(SENSOR_ADDR);					\
	data[0] = I2C_GetByte(I2C_MORE);			\
	data[1] = I2C_GetByte(I2C_MORE);			\
	data[2] = I2C_GetByte(I2C_MORE);			\
	data[3] = I2C_GetByte(I2C_MORE);			\
	data[4] = I2C_GetByte(I2C_LAST);			\
	*reg = data[0];								\
	*ax = data[1] << 8 | data[2];				\
	*ay = data[3] << 8 | data[4];				\
	*az = 0;									\
}

#endif

#endif /* __SENSOR_H__ */
