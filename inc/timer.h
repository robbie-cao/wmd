#ifndef	__TIMER_H__
#define __TIMER_H__

#include "types.h"
#include "config.h"

#define TIMER_UNIT_TICK     0
#define TIMER_UNIT_US       1
#define TIMER_UNIT_MS       2
#define TIMER_UNIT_S        3

typedef void (*timer_cb_t)(void);

/* Timer 0 initialization */
void    Timer0_Init(void);
/* Start timer 0 */
/*
 * Input
 *   - timeout: 0~2^16 IPERIOD
 *   - msg:     msg #id which will be handled in main control loop
 *   - parm:    parm for msg
 *   - repeat:  true/false
 * Return
 *   - 0, success
 *   - other, fail
 */
s8      Timer0_Start(u16 timeout, u8 msg, u8 parm, bool repeat);
/* Stop timer 0 */
s8      Timer0_Stop(void);
/* Timer 0 interrupt service routine */
void    Timer0_ISR(void);

/* Timer 1 initialization */
void    Timer1_Init(void);
/* Start timer 1 */
/*
 * Input
 *   - timeout: 0~2^27 IPERIOD
 *   - msg:     msg #id which will be handled in main control loop
 *   - parm:    parm for msg
 *   - repeat:  true/false
 * Return
 *   - 0, success
 *   - other, fail
 */
s8      Timer1_Start(u32 timeout, u8 msg, u8 parm, bool repeat);
/* Stop timer 1 */
s8      Timer1_Stop(void);
/* Timer 1 interrupt service routine */
void    Timer1_ISR(void);

/* Timer 2 initialization */
void    Timer2_Init(void);
/* Start timer 2 */
/*
 * Input
 *   - timeout: 0~2^24 IPERIOD
 *   - msg:     msg #id which will be handled in main control loop
 *   - parm:    parm for msg
 *   - repeat:  true/false
 * Return
 *   - 0, success
 *   - other, fail
 */
s8      Timer2_Start(u32 timeout, u8 msg, u8 parm, bool repeat);
/* Stop timer 2 */
s8      Timer2_Stop(void);
/* Timer 2 interrupt service routine */
void    Timer2_ISR(void);

#endif /* __TIMER_H__ */
