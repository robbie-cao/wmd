#include "types.h"
#include "sensor.h"

/* Sensor initialization */
void AccSensor_Init(void)
{
    /* do nothing by now */
}

#ifndef SENSOR_OPTIMIZE
/* Power up sensor */
void AccSensor_PwrUp(void)
{
    I2C_WriteTo(SENSOR_ADDR);		/* talk to device 0010000w */
    I2C_PutByte(SENSOR_INTL_REG);	/* internal address to be written into */
    I2C_PutByte(SENSOR_CMD_PWRUP);	/* wake up call */
}

/* Power down senor */
void AccSensor_PwrDn(void)
{
    I2C_WriteTo(SENSOR_ADDR);		/* talk to device 0010000w */
    I2C_PutByte(SENSOR_INTL_REG);	/* internal address to be written into */
    I2C_PutByte(SENSOR_CMD_PWRDN);	/* power down call */
}

#ifndef SENSOR_HIDE_FEAT
/* Sensor self-test*/
void AccSensor_SelfTest(void)
{
    I2C_WriteTo(SENSOR_ADDR);		/* talk to device 0010000w */
    I2C_PutByte(SENSOR_INTL_REG);	/* internal address to be written into */
    I2C_PutByte(SENSOR_CMD_ST);		/* self-test call */
}

/* Sensor bandgap test */
void AccSensor_BGTest(void)
{
    I2C_WriteTo(SENSOR_ADDR);		/* talk to device 0010000w */
    I2C_PutByte(SENSOR_INTL_REG);	/* internal address to be written into */
    I2C_PutByte(SENSOR_CMD_BGTST);	/* bandgap test call */
}

/* Senor temperature out enable */
void AccSensor_TOEN(void)
{
    I2C_WriteTo(SENSOR_ADDR);		/* talk to device 0010000w */
    I2C_PutByte(SENSOR_INTL_REG);	/* internal address to be written into */
    I2C_PutByte(SENSOR_CMD_TOEN);	/* TOEN call */
}
#endif

/* Get acceleration data from sensor */
s8 AccSensor_GetData(u8* reg, u16 *ax, u16 *ay, u16 *az)
{
    u8 data[5];

    I2C_WriteTo(SENSOR_ADDR);        /* talk to device 0010000w */
    I2C_PutByte(SENSOR_INTL_REG);    /* starting address to be read */

    I2C_ReadFrom(SENSOR_ADDR);       /* talk to device 0010000r */
    data[0] = I2C_GetByte(I2C_MORE); /* read one byte */
    data[1] = I2C_GetByte(I2C_MORE); /* read another */
    data[2] = I2C_GetByte(I2C_MORE); /* read another */
    data[3] = I2C_GetByte(I2C_MORE); /* read another */
    data[4] = I2C_GetByte(I2C_LAST); /* read last */

    *reg = data[0];
    *ax = data[1] << 8 | data[2];
    *ay = data[3] << 8 | data[4];
    *az = 0;

    return 0;
}
#endif
