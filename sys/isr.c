#include <htc.h>
#include "usart.h"
#include "timer.h"
#include "usart.h"
#include "eint.h"
#include "pmu.h"

static void interrupt ISR(void)
{
	static u8 c = 0;

	if (T0IE && T0IF) {			/* timer overflow */
		Timer0_ISR();
	}
	if (TMR1IE && TMR1IF) {		/* timer overflow */
		Timer1_ISR();
	}
	if (TMR2IE && TMR2IF) {		/* timer overflow */
		Timer2_ISR();
	}
	if (RCIE && RCIF) {			/* usart rx interrupt */
		USART_Rx_ISR();
	}
	if (RABIE && RABIF) {
		EINT_ISR();
	}
	if (INTE && INTF) {
		PMU_ISR();
	}
}
