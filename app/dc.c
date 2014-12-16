#include "types.h"
#include "dc.h"
#include "sensor.h"
#include "usart.h"
#include "prot.h"
#include "task.h"

//#define DC_FORMAT_HEX

typedef struct {
    u8 reg;
    u16 ax;
    u16 ay;
    u16 az;
} dc_data_t;

u8 dc_intv = DC_DEFAULT_INTV;
u8 dc_dest = DC_DEST_USART;

static dc_data_t dc_data;

#ifdef DC_OPTIMIZE
/* data collecting start */
s8 DC_Start(void)
{
    if (!dc_intv)
        return 0;

    return Timer0_Start(dc_intv * TICKS_1_MS, MSG_ID_SAMP_IND, 0, TRUE);
}
#endif

/* task entry for dispatching collected data to destination */
void DC_Dispatch(void)
{
    static u8 idx = 0;
    dc_data_t *data = (dc_data_t *)MSG_PARM2;

    switch (MSG_PARM1)
    {
        case DC_DEST_USART:
        case DC_DEST_BT:
#ifdef DC_FORMAT_HEX
            puthex(idx);
            putch(',');
            puthex(dc_intv);
            putch(',');
            puthex((data->ax >> 8) & 0xFF);
            puthex(data->ax & 0xFF);
            putch(',');
            puthex((data->ay >> 8) & 0xFF);
            puthex(data->ay & 0xFF);
            putch('\r');
#else
            putch(ACC_DTAT_HEADER);
            putch(dc_intv);
            putch((data->ax >> 8) & 0xFF);
            putch(data->ax & 0xFF);
            putch((data->ay >> 8) & 0xFF);
            putch(data->ay & 0xFF);
#if 0
            putch((data->az >> 8) & 0xFF);
            putch(data->az & 0xFF);
#else
            /* set z-axis to reg and idx as we don't have z-axis in current sensor */
            putch(data->reg);
            putch(idx);
#endif
#endif
            idx++;
            break;
        case DC_DEST_ALGO:
        default:
            break;
    }
}


/* task entry for data sampling */
void DC_Sampling(void)
{
    AccSensor_GetData(&dc_data.reg, &dc_data.ax, &dc_data.ay, &dc_data.az);
    Task_Push(MSG_ID_DC_IND, dc_dest, (u16)&dc_data);
}
