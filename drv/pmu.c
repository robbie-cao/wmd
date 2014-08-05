#include <htc.h>
#include "types.h"
#include "pmu.h"
#include "i2c.h"
#include "usart.h"
#include "task.h"

//#define DEBUG

/* PMU module initialization */
void PMU_Init(void)
{
#ifdef DEBUG
	u8 res;
	u8 i = 0;
#endif

	TRISA2 = 1;		/* config RA2/INT as input */
	RABPU = 0;		/* PORTA/B pull-up enable */
	WPUA2 = 1;		/* RA2 weak pull-up enable */
	INTEDG = 0;		/* interrupt on falling edge of INT pin */
	INTE = 1;		/* RA2/INT external interrupt enable */
	GIE = 1;		/* enable global interrupt */

#ifndef DEBUG
	/* config AS7100 interrupt enable bits */
	I2C_WriteTo(PMU_I2C_ADDR(OOL1));
	I2C_PutByte(PMU_INT_POWON | PMU_INT_POWOFF | PMU_INT_LVDSTATE | PMU_INT_OPTSTATE);

	/* config AS7100 IRQ mode */
	I2C_WriteTo(PMU_I2C_ADDR(OOL3));
	I2C_PutByte(PMU_IRQ_MODE_PULSE);

	/* enable PORN(RESETN) signal output */
	I2C_WriteTo(PMU_I2C_ADDR(OOL5));
	I2C_PutByte(PMU_PRON_ENABLE);
#else
	/* config AS7100 interrupt enable bits */
	res = I2C_WriteTo(PMU_I2C_ADDR(OOL1));
	puthex(PMU_I2C_ADDR(OOL1));
	putch(':');
	puthex(res);
	putch(',');
	res = I2C_PutByte(PMU_INT_POWON | PMU_INT_POWOFF | PMU_INT_LVDSTATE | PMU_INT_OPTSTATE);
	puthex(res);
	putch('\r');

	/* config AS7100 IRQ mode */
	res = I2C_WriteTo(PMU_I2C_ADDR(OOL3));
	puthex(PMU_I2C_ADDR(OOL3));
	putch(':');
	puthex(res);
	putch(',');
	res = I2C_PutByte(PMU_IRQ_MODE_PULSE);
	puthex(res);
	putch('\r');

	/* enable PORN(RESETN) signal output */
	res = I2C_WriteTo(PMU_I2C_ADDR(OOL5));
	puthex(PMU_I2C_ADDR(OOL5));
	putch(':');
	puthex(res);
	putch(',');
	res = I2C_PutByte(PMU_PRON_ENABLE);
	puthex(res);
	putch('\r');
#endif

#ifdef DEBUG
	/* read all register */
	for (i = 0; i < 0x10; i++) {
		u8 addr = 0x60 | i;

		res = I2C_ReadFrom(addr);
		puthex(addr);
		putch(':');
		puthex(res);
		putch(',');
		res = I2C_GetByte(I2C_LAST);
		puthex(res);
		putch('\r');
	}
#endif
}

/* PMU interrupt service routine */
void PMU_ISR(void)
{
#ifdef DEBUG
	putch('I');
#endif
	/* send msg to main loop */
	Task_Push(MSG_ID_PMU_IND, 0, 0);

	INTF = 0;
}

/* PMU task entry */
void PMU_IndHandler(void)
{
	u8 status;

#ifndef DEBUG
	/* read pmu interrupt status */
	I2C_ReadFrom(PMU_I2C_ADDR(OOL0));
	status = I2C_GetByte(I2C_LAST);

	/* clear pmu interrupt */
	I2C_WriteTo(PMU_I2C_ADDR(OOL2));
	I2C_PutByte(status);
#else
	/* read interrupt status */
	status = I2C_ReadFrom(PMU_I2C_ADDR(OOL0));
	puthex(PMU_I2C_ADDR(OOL0));
	putch(':');
	puthex(status);
	putch(',');
	status = I2C_GetByte(I2C_LAST);
	puthex(status);
	putch('\r');

	/* clear pmu interrupt */
	status = I2C_WriteTo(PMU_I2C_ADDR(OOL2));
	puthex(PMU_I2C_ADDR(OOL2));
	putch(':');
	puthex(status);
	putch(',');
	status = I2C_PutByte(status);
	puthex(status);
	putch('\r');
#endif

	/* send msg to main loop */
	Task_Push(MSG_ID_BATT_IND, status, 0);
}
