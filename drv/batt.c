#include "types.h"
#include "pmu.h"
#include "batt.h"
#include "task.h"
#include "usart.h"
#include "led.h"

//#define DEBUG

void Batt_StatusHandler(void)
{
	if (MSG_PARM1 & PMU_INT_POWON) {
		/* power on */
#ifdef DEBUG
		putstr((u8 *)"Power On\r");
#endif
	}
	if (MSG_PARM1 & PMU_INT_POWOFF) {
		/* power off */
#ifdef DEBUG
		putstr((u8 *)"Power Off\r");
#endif
	}
	if (MSG_PARM1 & PMU_INT_LVDSTATE) {
		/* low voltage */
#ifdef DEBUG
		putstr((u8 *)"Low Voltage\r");
#endif
		LED_On(LED_BATT);
	}
	if (MSG_PARM1 & PMU_INT_OPTSTATE) {
		/* temperature run away */
#ifdef DEBUG
		putstr((u8 *)"Temperature run away\r");
#endif
		LED_Flash(LED_BATT);
	}
}
