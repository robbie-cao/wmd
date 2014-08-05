#include "types.h"
#include "kpd.h"
#include "task.h"
#include "timer.h"
#include "bt.h"
#include "sensor.h"
#include "dc.h"
#include "usart.h"
#include "led.h"

//#define DEBUG

void KPD_PressInd(void)
{
#ifdef DEBUG
	putstr((u8 *)"P");
	puthex(MSG_PARM1);
	putch('\r');
#endif
	Timer1_Start(KPD_LP_TIME, MSG_ID_KEYLP_IND, MSG_PARM1, FALSE);
}

void KPD_ReleaseInd(void)
{
#ifdef DEBUG
	putstr((u8 *)"R");
	puthex(MSG_PARM1);
	putch('\r');
#endif
	Timer1_Stop();
}

void KPD_LongPressInd(void)
{
	static bit on_a = 0;
	static bit on_b = 0;

#ifdef DEBUG
	putstr((u8 *)"LP");
	puthex(MSG_PARM1);
	putch('\r');
#endif
	switch (MSG_PARM1)
	{
	case KPD_ID_A:
		on_a = !on_a;
		if (on_a) {
			AccSensor_PwrUp();
			DC_Start();
			LED_On(LED_SENSOR);
		} else {
			LED_Off(LED_SENSOR);
			DC_Stop();
			AccSensor_PwrDn();
		}
		break;
	case KPD_ID_B:
		on_b = !on_b;
		if (on_b) {
			BT_PwrUp();
		#ifdef DEBUG
			LED_On(LED_TEST);
		#endif
		} else {
		#ifdef DEBUG
			LED_Off(LED_TEST);
		#endif
			BT_PwrDn();
		}
		break;
	default:
		break;
	}
}
