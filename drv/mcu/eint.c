#include <htc.h>
#include "eint.h"
#include "kpd.h"
#include "task.h"
#include "usart.h"

//#define DEBUG

/* external interrupt initialization */
void EINT_Init(void)
{
#ifdef EINT_OPTIMIZE
    TRISA |= 0x03;	/* config RA0/1 as input */
    ANSELH = 0x00;	/* config analog chanel as digital input */
    ANSEL = 0x00;	/* config analog chanel as digital input */
    RABPU = 0;		/* PORTA/B pull-up enable */
    WPUA |= 0x03;	/* RA0/1 weak pull-up enable */
    IOCA |= 0x03;	/* RA0/1 interrupt-on-change enable */
    RABIE = 1;		/* PORTA/B change interrupt enable */
    GIE = 1;		/* enable global interrupt */
#else
    TRISA0 = 1;		/* config RA0 as input */
    TRISA1 = 1;		/* config RA1 as input */

    ANSELH = 0x00;	/* config analog chanel as digital input */
    ANSEL = 0x00;	/* config analog chanel as digital input */

    RABPU = 0;		/* PORTA/B pull-up enable */
    WPUA0 = 1;		/* RA0 weak pull-up enable */
    WPUA1 = 1;		/* RA1 weak pull-up enable */

    IOCA0 = 1;		/* RA0 interrupt-on-change enable */
    IOCA1 = 1;		/* RA1 interrupt-on-change enable */
    RABIE = 1;		/* PORTA/B change interrupt enable */
    GIE = 1;		/* enable global interrupt */
#endif
}

/* external interrupt service routine */
void EINT_ISR(void)
{
    static u8 p0 = 1, p1 = 1;

    if (RA0 != p0) {
#ifdef DEBUG
        putch('A');
        putch('0');
        putch(':');
        puthex(RA0);
        putch('\r');
#endif
        /* send msg to main loop */
        Task_Push(RA0 ? MSG_ID_KEYREL_IND : MSG_ID_KEYPR_IND, KPD_ID_A, 0);
        p0 = RA0;
    }
    if (RA1 != p1){
#ifdef DEBUG
        putch('A');
        putch('1');
        putch(':');
        puthex(RA1);
        putch('\r');
#endif
        /* send msg to main loop */
        Task_Push(RA1 ? MSG_ID_KEYREL_IND : MSG_ID_KEYPR_IND, KPD_ID_B, 0);
        p1 = RA1;
    }
    RABIF = 0;		/* clear interrupt flag */
}
