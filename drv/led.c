#include "types.h"
#include "led.h"

u8 led_mask = 0;

#ifndef LED_OPTIMIZE
/* LED module initialization */
void LED_Init(void)
{
	GPIO_SetOutput(LED_ID_A);
	GPIO_SetOutput(LED_ID_B);
	GPIO_SetOutput(LED_ID_C);

	GPIO_Set(LED_ID_A);
	GPIO_Set(LED_ID_B);
	GPIO_Set(LED_ID_C);
}

/* Turn LED (mask) on */
void LED_On(u8 mask)
{
	led_mask &= ~mask;
	PORTC &= ~mask;
}

/* Turn LED (mask) off */
void LED_Off(u8 mask)
{
	led_mask &= ~mask;
	PORTC |= mask;
}

/* Let LED (mask) flash */
void LED_Flash(u8 mask)
{
	led_mask |= mask;
	Timer2_Start(LED_FLASH_INTV, MSG_ID_LED_FLASH, led_mask, TRUE);
}

#endif

/* task entry for led flash */
void LED_FlashInd(void)
{
	static bool on = 0;

	if (on)
		PORTC &= ~MSG_PARM1;
	else
		PORTC |= MSG_PARM1;

	on = !on;
}