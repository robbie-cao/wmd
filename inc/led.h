#ifndef	__LED_H__
#define __LED_H__

#include <htc.h>
#include "types.h"
#include "util.h"
#include "gpio.h"
#include "timer.h"
#include "task.h"

#define LED_OPTIMIZE

#define LED_ID_A		5
#define LED_ID_B		4
#define LED_ID_C		3

#define LID2PID(id)		(1 << (5 - (id)))

#define LED_MASK_A		(1 << 5)
#define LED_MASK_B		(1 << 4)
#define LED_MASK_C		(1 << 3)

#define LMSK2PMSK(mask)	REVERSE_BITS_4((mask) >> 2)

#define LED_TEST		LED_MASK_A
#define LED_BATT		LED_MASK_B
#define LED_SENSOR		LED_MASK_C

#define LED_FLASH_INTV	(TICKS_1_MS * 50)

#ifdef LED_OPTIMIZE
#define LED_Init()		((TRISC &= ~0b00111000), (PORTC |= 0b00111000))

#define LED_On(mask)	(led_mask &= ~(mask), PORTC &= ~(mask))
#define LED_Off(mask)	(led_mask &= ~(mask), PORTC |= (mask))
#define LED_Flash(mask)	(led_mask |= (mask), Timer2_Start(LED_FLASH_INTV, MSG_ID_LED_FLASH, led_mask, TRUE))

#else

/* LED module initialization */
void	LED_Init(void);
/* Turn LED (mask) on */
void	LED_On(u8 mask);
/* Turn LED (mask) off */
void	LED_Off(u8 mask);
/* Let LED (mask) flash */
void	LED_Flash(u8 mask);
#endif

/* task entry for led flash */
void	LED_FlashInd(void);

extern u8 led_mask;

#endif /* __LED_H__ */
