#include "types.h"
#include "prot.h"
#include "task.h"
#include "sensor.h"
#include "timer.h"
#include "usart.h"
#include "dc.h"
#include "version.h"

#define CMD_SIZE        sizeof(Cmd_t)

/* task entry for protocol cmd */
void Prot_Cmd_Handler(void)
{
    u8 *cmd_buff = (u8 *)MSG_PARM2;
    u8 cmd, parm;

    if (MSG_PARM1 != CMD_SIZE)
        return;

    cmd = cmd_buff[0];
    parm = cmd_buff[1];

    switch (cmd) {
        case CMD_ID_SYS_VERSION:
            SYS_Info();
            break;
        case CMD_ID_SYS_PARMS:
            break;
        case CMD_ID_SENSOR_PWRUP:
            AccSensor_PwrUp();
            break;
        case CMD_ID_SENSOR_PWRDN:
            AccSensor_PwrDn();
            break;
        case CMD_ID_SENSOR_SLEEP:
            AccSensor_Sleep();
            break;
#ifndef SENSOR_HIDE_FEAT
        case CMD_ID_SENSOR_SELFTEST:
            AccSensor_SelfTest();
            break;
        case CMD_ID_SENSOR_BGTST:
            AccSensor_BGTest();
            break;
        case CMD_ID_SENSOR_TOEN:
            AccSensor_TOEN();
            break;
#endif
        case CMD_ID_SET_DCINTV:
            DC_SetIntv(parm);
            break;
        case CMD_ID_SET_DCDEST:
            DC_SetDest(parm);
            break;
        case CMD_ID_SAMP_START:
            DC_Start();
            break;
        case CMD_ID_SAMP_STOP:
            DC_Stop();
            break;
        default:
            return;
    }

    /* response */
    putch(cmd);
    putch(RESP_OK);
}
