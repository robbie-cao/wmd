#include <htc.h>
#include "types.h"
#include "timer.h"
#include "task.h"

//#define DEBUG
#ifdef DEBUG
#include "usart.h"
#endif

typedef struct {
	u8			repeat		: 1;
	u8			prescaler	: 3;
	u8			postscaler	: 4;
	u8			tmr_l		: 8;
	u8			tmr_h		: 8;
	u8			load		: 8;
	u8			reload		: 8;
	u8			busy		: 1;
	u8			reserved	: 3;
	u8			msg			: 4;
	u8			parm		: 8;
} timer_isr_block_t;

static timer_isr_block_t	tb[3];

/* Timer 0 initialization */
void Timer0_Init(void)
{
	tb[0].busy = 0;
}

/* Start timer 0 */
s8 Timer0_Start(u16 timeout, u8 msg, u8 parm, bool repeat)
{
	if (tb[0].busy)
		return -1;

	tb[0].busy = 1;
	tb[0].msg = msg;
	tb[0].parm = parm;
	tb[0].repeat = repeat;

	tb[0].reload = timeout >> 8;
	tb[0].load = tb[0].reload;
	tb[0].tmr_l = ~(timeout & 0xFF) + 1;

	TMR0 = tb[0].load ? 0 : tb[0].tmr_l;
	T0CS = 0;			/* select internal clock */
	T0IE = 1;			/* enable timer interrupt */
	GIE = 1;			/* make sure global interrupts enable */

	return 0;
}

/* Stop timer (id) */
s8 Timer0_Stop(void)
{
	T0IE = 0;			/* disable timer interrupt */

	tb[0].busy = 0;

	return 0;
}

/* Timer 0 interrupt service routine */
void Timer0_ISR(void)
{
	if (!tb[0].load) {
		/* send msg to main loop */
		Task_Push(tb[0].msg, tb[0].parm, 0);
	#ifdef DEBUG
		putch('0');
	#endif
		if (!tb[0].repeat) {
			T0IE = 0;	/* disable timer interrupt */
			return;
		}
		tb[0].load = tb[0].reload + 1;
	}

	tb[0].load--;
	TMR0 = tb[0].load ? 0:  tb[0].tmr_l;
	T0IF = 0;	/* clear interrupt flag, ready for next */
}

/* Timer 1 initialization */
void Timer1_Init(void)
{
	tb[1].busy = 0;
}

/* Start timer 1 */
s8 Timer1_Start(u32 timeout, u8 msg, u8 parm, bool repeat)
{
	u32 remain = timeout & 0x7FFFF;
	u8 scale = (remain >> 16) & 0x07;

	if (tb[1].busy)
		return -1;

	tb[1].busy = 1;
	tb[1].msg = msg;
	tb[1].parm = parm;
	tb[1].repeat = repeat;

	tb[1].reload = (timeout >> 19) & 0xFF;
	tb[1].load = tb[1].reload;

	switch (scale)
	{
	case 0:  tb[1].prescaler = 0b00; /* 1:1 */ break;
	case 1:  tb[1].prescaler = 0b01; /* 1:2 */ break;
	case 2:
	case 3:  tb[1].prescaler = 0b10; /* 1:4 */ break;
	case 4:
	case 5:
	case 6:
	case 7:  tb[1].prescaler = 0b11; /* 1:8 */ break;
	default: tb[1].prescaler = 0b00; /* 1:1 */ break;
	}

	remain = ~remain + 1;
	tb[1].tmr_h = (remain >> (8 + tb[1].prescaler)) & 0xFF;
	tb[1].tmr_l = (remain >> tb[1].prescaler) & 0xFF;

	TMR1H = tb[1].load ? 0 : tb[1].tmr_h;
	TMR1L = tb[1].load ? 0 : tb[1].tmr_l;
#if 0
	T1CKPS1 = ((tb[1].load ? 0b11 : tb[1].prescaler) << 1) & 0x1;	/* set prescaler */
	T1CKPS0 = (tb[1].load ? 0b11 : tb[1].prescaler) & 0x1;
	TMR1CS = 0;		/* select internal clock */
	TMR1ON = 1;		/* enable timer */
#else
	T1CON = ((tb[1].load ? 0b11 : tb[1].prescaler) << 4) | 0b01;
#endif
	TMR1IE = 1;		/* enable timer interrupt */
	PEIE = 1;		/* enables all unmasked interrupts */
	GIE = 1;		/* make sure global interrupts enable */

	return 0;
}

/* Stop timer 1 */
s8 Timer1_Stop(void)
{
	TMR1ON = 0;		/* disable timer */
	TMR1IE = 0;		/* disable timer interrupt */

	tb[1].busy = 0;

	return 0;
}

/* Timer 1 interrupt service routine */
void Timer1_ISR(void)
{
	if (!tb[1].load) {
		/* send msg to main loop */
		Task_Push(tb[1].msg, tb[1].parm, 0);
	#ifdef DEBUG
		putch('1');
	#endif

		if (!tb[1].repeat) {
			TMR1ON = 0;		/* disable timer */
			TMR1IE = 0;		/* disable timer interrupt */
			return;
		}
		tb[1].load = tb[1].reload + 1;
	}

	tb[1].load--;
	TMR1H = tb[1].load ? 0 : tb[1].tmr_h;
	TMR1L = tb[1].load ? 0 : tb[1].tmr_l;
#if 0
	T1CKPS1 = ((tb[1].load ? 0b11 : tb[1].prescaler) << 1) & 0x1;	/* set prescaler */
	T1CKPS0 = (tb[1].load ? 0b11 : tb[1].prescaler) & 0x1;
	TMR1CS = 0;		/* select internal clock */
	TMR1ON = 1;		/* enable timer */
#else
	T1CON = ((tb[1].load ? 0b11 : tb[1].prescaler) << 4) | 0b01;
#endif

	TMR1IF = 0;	/* clear interrupt flag, ready for next */
}

/* Timer 2 initialization */
void Timer2_Init(void)
{
	tb[2].busy = 0;
}

/* Start timer 2 */
s8 Timer2_Start(u32 timeout, u8 msg, u8 parm, bool repeat)
{
	u16 remain = timeout & 0xFFFF;
	u8 scale = (remain >> 8) & 0xFF;

	if (tb[2].busy)
		return -1;

	tb[2].busy = 1;
	tb[2].msg = msg;
	tb[2].parm = parm;
	tb[2].repeat = repeat;

	tb[2].reload = (timeout >> 16) & 0xFF;
	tb[2].load = tb[2].reload;

	if (scale < 16) {
		tb[2].prescaler = 0b00;	/* 1:1 */
		tb[2].postscaler = scale;
	} else if (scale >= 16 && scale < 64) {
		tb[2].prescaler = 0b01;	/* 1:4 */
		tb[2].postscaler = scale >> 2;
	} else {
		tb[2].prescaler = 0b10;	/* 1:16 */
		tb[2].postscaler = scale >> 4;
	}
	tb[2].tmr_h = (remain >> (8 - (tb[2].prescaler << 1))) / (tb[2].postscaler + 1);

	TMR2 = 0;
	PR2 = tb[2].load ? 0xFF : tb[2].tmr_h;
	T2CON = ((tb[2].load ? 0xF : tb[2].postscaler) << 3) | (0b1 << 2) | (tb[2].load ? 0b10 : tb[2].prescaler);
	TMR2IE = 1;		/* enable timer interrupt */
	PEIE = 1;		/* enables all unmasked interrupts */
	GIE = 1;		/* make sure global interrupts enable */

	return 0;
}

/* Stop timer 2 */
s8 Timer2_Stop(void)
{
	TMR2ON = 0;		/* disable timer */
	TMR2IE = 0;		/* disable timer interrupt */

	tb[2].busy = 0;

	return 0;
}

/* Timer 2 interrupt service routine */
void Timer2_ISR(void)
{
	if (!tb[2].load) {
		/* send msg to main loop */
		Task_Push(tb[2].msg, tb[2].parm, 0);
	#ifdef DEBUG
		putch('2');
	#endif
	
		if (!tb[2].repeat) {
			TMR2ON = 0;		/* disable timer */
			TMR2IE = 0;		/* disable timer interrupt */
			return;
		}
		tb[2].load = tb[2].reload + 1;
	}

	tb[2].load--;
	TMR2 = 0;
	PR2 = tb[2].load ? 0xFF : tb[2].tmr_h;
	T2CON = ((tb[2].load ? 0xF : tb[2].postscaler) << 3) | (0b1 << 2) | (tb[2].load ? 0b10 : tb[2].prescaler);

	TMR2IF = 0;	/* clear interrupt flag, ready for next */
}
