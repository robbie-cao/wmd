#include "types.h"
#include "bt.h"

#ifndef BT_OPTIMIZE
/* BT module initialization */
void BT_Init(void)
{
	GPIO_SetOutput(GPIO_BTEN);
	GPIO_Clear(GPIO_BTEN);
}

/* Power up BT module */
void BT_PwrUp(void)
{
	GPIO_Set(GPIO_BTEN);
}

/* Power off BT module */
void BT_PwrDn(void)
{
	GPIO_Clear(GPIO_BTEN);
}
#endif

/* Receive data from BT serial port */
s8 BT_Recv(u8 *data, s16 len)
{
	return 0;
}

/* Send data to BT serial port to transmit */
s8 BT_Transmit(u8 *data, s16 len)
{
	return 0;
}
