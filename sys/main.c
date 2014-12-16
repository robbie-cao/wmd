#include <htc.h>
#include "types.h"
#include "config.h"
#include "usart.h"
#include "i2c.h"
#include "sensor.h"
#include "gpio.h"
#include "bt.h"
#include "timer.h"
#include "pwm.h"
#include "task.h"
#include "eint.h"
#include "pmu.h"
#include "led.h"
#include "version.h"

#define DEBUG

__CONFIG(WDTDIS & UNPROTECT & INTCLK & MCLREN);

void main(void)
{
    INTCON = 0;		/* purpose of disabling the interrupts */

    USART_Init();	/* set up the USART */
    I2C_Init();		/* set up I2C */

    Timer0_Init();	/* init timer 0 */
    Timer1_Init();	/* init timer 1 */
    Timer2_Init();	/* init timer 2 */

    EINT_Init();	/* setup external interrupt */
    PMU_Init();		/* pmu must be init after i2c initialization */

    LED_Init();		/* led init */
    BT_Init();		/* bt module init */

    Task_Init();

#ifdef DEBUG
    LED_On(LED_TEST);
#endif
    /* system information */
    SYS_Info();

    for (;;) {
        s8 msg_id;

        if ((msg_id = Task_GetCurrent()) < 0)
            continue;

        Task_Run(msg_id);
        Task_Pop();
    }
}
