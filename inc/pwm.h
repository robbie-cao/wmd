#ifndef	__PWM_H__
#define __PWM_H__

#include "types.h"

#define PWM_PIN_P1A			0x01
#define PWM_PIN_P1B			0x02
#define PWM_PIN_P1C			0x04
#define PWM_PIN_P1D			0x08

/*
 * PWM mode
 *   00 = P1A, P1C active-high; P1B, P1D active-high
 *   01 = P1A, P1C active-high; P1B, P1D active-low
 *   10 = P1A, P1C active-low; P1B, P1D active-high
 *   11 = P1A, P1C active-low; P1B, P1D active-low
 */
#define PWM_MODE_0			0b00
#define PWM_MODE_1			0b01
#define PWM_MODE_2			0b10
#define PWM_MODE_3			0b11

/*
 * Tosc = 1 / Fosc
 * Tpwm = (PR2 + 1) * 4 * Tosc * TMR2_Prescaler		- unit (s)
 * ==>
 * PR2 = (Tpwm * Fosc) / (4 * TMR2_Prescaler) - 1
 * Tpwm_us = 1000000 * Tpwm							- unit (us)
 * IPERIOD = (4 * 1000000 / Fosc)					- period of instruction clock in 1 us
 * ==>
 * PR2 = Tpwm_us / (IPERIOD * TMR2_Prescaler) - 1
 */

/* 
 * Duty = (CCPR1L:CCP1CON<5:4>) / (4* (PR2 + 1))
 * Duty = [0 ~ 100%]
 * ==>
 * (CCPR1L:CCP1CON<5:4>) = Duty * 4 * (PR2 + 1)
 * duty = 1 / Duty
 * ==>
 * (CCPR1L:CCP1CON<5:4>) = 4 * (PR2 + 1) / duty
 */

/* PWM initialization */
void	PWM_Init(void);
/* PWM start */
/*
 * Input
 *   - mask  : bit 0 - P1A, bit 1 - P1B, bit 2 - P1C, bit 3 - P1D
 *   - mode  : pwm mode 
 *   - period: PWM period in us
 *   - hr    : Pulse high(duty cycle)
 *   - lr    : Pulse low(duty free)
 * That is, duty cycle ration = hr / (hr + lr)
 */
bool	PWM_Start(u8 mask, u8 mode, u16 period, u8 hr, u8 lr);
/* PWM stop */
void	PWM_Stop(void);

#endif /* __PWM_H__ */
