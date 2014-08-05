#ifndef	__BT_H__
#define __BT_H__

#include "types.h"
#include "gpio.h"

#define BT_OPTIMIZE

#define GPIO_BTEN		0

#ifdef BT_OPTIMIZE
#define BT_Init()		GPIO_SetOutput(GPIO_BTEN), GPIO_Clear(GPIO_BTEN)
#define BT_PwrUp()		GPIO_Set(GPIO_BTEN)
#define BT_PwrDn()		GPIO_Clear(GPIO_BTEN)
#else
/* BT module initialization */
void	BT_Init(void);
/* Power up BT module */
void 	BT_PwrUp(void);
/* Power off BT module */
void 	BT_PwrDn(void);
#endif

/* Receive data from BT serial port */
s8		BT_Recv(u8 *data, s16 len);
/* Send data to BT serial port to transmit */
s8		BT_Transmit(u8 *data, s16 len);

#endif /* __BT_H__ */
