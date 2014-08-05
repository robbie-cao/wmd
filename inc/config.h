#ifndef	__CONFIG_H__
#define __CONFIG_H__

#define FOSC				4000000L		/* 4MHz system frequency */

#ifndef _XTAL_FREQ
#define _XTAL_FREQ			FOSC
#endif

#define IPERIOD				(4 * 1000000 / _XTAL_FREQ)		/* period of instruction clock in 1 us */
#define ICYCLES				(_XTAL_FREQ / (4 * 1000000))	/* instruction cycles in 1 us */

#if IPERIOD >= 1
#define TICKS_1_US			(1 / IPERIOD)
#define TICKS_1_MS			(1000 / IPERIOD)
#define TICKS_1_S			(1000000 / IPERIOD)
#else
#define TICKS_1_US			(1 * ICYCLES)
#define TICKS_1_MS			(1000 * ICYCLES)
#define TICKS_1_S			(1000000 * ICYCLES)
#endif

#endif /* __CONFIG_H__ */
