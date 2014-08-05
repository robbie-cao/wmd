#ifndef	__DC_H__
#define __DC_H__

#include "types.h"
#include "timer.h"
#include "task.h"

#define DC_OPTIMIZE

#define DC_DEST_USART		0x00
#define DC_DEST_BT			0x01
#define DC_DEST_ALGO		0x02

#define DC_DEFAULT_INTV		30		/* ms */

#ifndef DC_OPTIMIZE
#define DC_Start()			(dc_intv ? Timer0_Start(dc_intv * TICKS_1_MS, MSG_ID_SAMP_IND, 0, TRUE) : 0)
#else
s8		DC_Start(void);
#endif

/* set data collecting interval, unit: ms */
#define DC_SetIntv(intv)	dc_intv = (intv)
/* set collected data destination */
#define DC_SetDest(dest)	dc_dest = (dest)
/* data collecting stop */
#define DC_Stop()			Timer0_Stop()

/* task entry for dispatching collected data to destination */
void	DC_Dispatch(void);
/* task entry for data sampling */
void 	DC_Sampling(void);

extern u8 dc_intv;
extern u8 dc_dest;

#endif /* __DC_H__ */
