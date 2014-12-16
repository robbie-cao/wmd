#include <htc.h>
#include "types.h"
#include "util.h"
#include "pwm.h"


/* PWM initialization */
void PWM_Init(void)
{
}

/* PWM start */
/*
 * Input
 *   - period: PWM period in us
 *   - hr    : Pulse high(duty cycle)
 *   - lr    : Pulse low(duty free)
 * That is, duty cycle ration = hr / (hr + lr)
 */
bool PWM_Start(u8 mask, u8 mode, u16 period, u8 hr, u8 lr)
{
    u8 scale, pr2, prescaler;
    u16 ccpr;

    scale = (period >> 8) & 0x0F;
    if (scale < 1) {
        prescaler = 0b00;		/* 1:1 */
    } else if (scale >= 1 && scale < 4) {
        prescaler = 0b01;		/* 1:4 */
    } else {
        prescaler = 0b10;		/* 1:16 */
    }

    pr2 = (period >> (prescaler << 1)) - 1;
    ccpr = ((pr2 + 1) << 2) * hr / (hr + lr);

    /* disable the PWM pin output driver */
    TRISC |= REVERSE_BITS_4(mask & 0x0F) << 2;
    PR2 = pr2;		/* set the PWM period */
    /* configure the CCP module for the PWM mode */
    /*
     * (PWM Output Config) << 6 | (PWM Duty Cycle Least bits) << 4 | (EECP Mode)
     * PWM Output Config
     *   00 = Single output; P1A modulated; P1B, P1C, P1D assigned as port pins
     *   01 = Full-Bridge output forward; P1D modulated; P1A active; P1B, P1C inactive
     *   10 = Half-Bridge output; P1A, P1B modulated with dead-band control; P1C, P1D assigned as port pins
     *   11 = Full-Bridge output reverse; P1B modulated; P1C active; P1A, P1D inactive
     * EECP Mode
     *   1100 = PWM mode; P1A, P1C active-high; P1B, P1D active-high
     *   1101 = PWM mode; P1A, P1C active-high; P1B, P1D active-low
     *   1110 = PWM mode; P1A, P1C active-low; P1B, P1D active-high
     *   1111 = PWM mode; P1A, P1C active-low; P1B, P1D active-low
     */
    CCP1CON = (0b00 << 6) | ((ccpr & 0x3) << 4) | (0b11 << 2) | (mode & 0x3);
    /* set the PWM duty cycle */
    CCPR1L = (ccpr >> 2) & 0xFF;

    /* set pulse steering mode */
    PSTRCON = (0b1 << 4) | (mask & 0x0F);

    /* config and start timer 2 */
    TMR2IF = 0;		/* clear the TMR2IF interrupt flag */
    TMR2 = 0;
#if 0
    /* set prescaler */
    T2CKPS1 = (prescaler >> 1) & 0x1;
    T2CKPS0 = (prescaler >> 0) & 0x1;
    /* set postscaler to 1:1 */
    TOUTPS3 = 0;
    TOUTPS2 = 0;
    TOUTPS1 = 0;
    TOUTPS0 = 0;
    TMR2ON = 1;		/* enable timer */
#else
    /* set the timer2 prescale value and enable timer2 */
    T2CON = (0b0000 << 3) | (0b1 << 2) | prescaler;
#endif

    /* enable the PWM pin (CCP1) output driver */
    TRISC &= ~(REVERSE_BITS_4(mask & 0x0F) << 2);

    return 0;
}

/* PWM stop */
void PWM_Stop(void)
{
    TRISC5 = 1;
    TMR2ON = 0;
}
