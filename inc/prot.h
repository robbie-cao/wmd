#ifndef	__PROT_H__
#define __PROT_H__

#include "types.h"
#include "sensor.h"

typedef struct {
    u8  id;
    u8  parm;
} Cmd_t;

typedef struct {
    u8  id;
    u8  ack;
} Resp_t;

#define CMD_ID_SYS_VERSION      0x20
#define CMD_ID_SYS_PARMS        0x21

#define CMD_ID_SENSOR_PWRUP     0x30
#define CMD_ID_SENSOR_PWRDN     0x31
#define CMD_ID_SENSOR_SLEEP     0x32
#ifndef SENSOR_HIDE_FEAT
#define CMD_ID_SENSOR_SELFTEST  0x33
#define CMD_ID_SENSOR_BGTST     0x34
#define CMD_ID_SENSOR_TOEN      0x35
#endif

#define CMD_ID_SET_DCINTV       0x40
#define CMD_ID_SET_DCDEST       0x41

#define CMD_ID_SAMP_START       0x50
#define CMD_ID_SAMP_STOP        0x51

#define CMD_ID_VALID(id)        (id >= 0x20 && id <= 0x60)

#define RESP_OK                 0x00
#define RESP_FAIL               0x01

#define ACC_DTAT_HEADER         0x55

typedef struct {
    u8 header;
    u8 intv;
    u16 ax;
    u16 ay;
    u16 az;
} AccData_t;

/* task entry for protocol cmd */
void    Prot_Cmd_Handler(void);

#endif /* __PROT_H__ */
