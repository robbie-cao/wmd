#ifndef __USART_H__
#define __USART_H__

#include "types.h"
#include "config.h"

#define USART_OPTIMIZE

#define NINE		0		/* use 9-bit communication? FALSE = 8bit */
#define HIGH_SPEED	1

#define BAUD		9600
#ifdef USART_OPTIMIZE
#if ((FOSC == 4000000L) && (BAUD == 9600))
#define DIVIDER		25
#else
#define DIVIDER		((int)(FOSC / (BAUD << 4) -1))
#endif
#else
#define DIVIDER		((int)(FOSC / (16UL * BAUD) -1))
#endif

#if NINE == 1
#define NINE_BITS	0x40
#else
#define NINE_BITS	0
#endif

#if HIGH_SPEED == 1
#define SPEED		0x4
#else
#define SPEED		0
#endif

#if defined(_16F87) || defined(_16F88)
	#define RX_PIN	TRISB2
	#define TX_PIN	TRISB5
#elif defined(_16F687) || defined(_16F689)|| defined(_16F690)
	#define RX_PIN	TRISB5
	#define TX_PIN	TRISB7
#else
	#define RX_PIN	TRISC7
	#define TX_PIN	TRISC6
#endif

/* USART module initialization */
void	USART_Init(void);
/* Put one byte to USART to transmit */
void	USART_PutByte(u8 byte);
/* Get one byte from USART receiver */
u8		USART_GetByte(void);
/* Put the ninth bit */
#define USART_PutNinth(bitnine)	(TX9D = bitnine ? 1 : 0)
/* Get the ninth bit */
u8		USART_GetNinth(void);
/* Get receiving framing error */
u8		USART_GetFERR(void);
/* Check receiver overrun error */
u8		USART_CheckOERR(void);

/* USART Rx interrupt service routine */
void	USART_Rx_ISR(void);

/* The following functions are for printf in standard C library which may use for debug */
/* Serial initialization */
#define serial_init	USART_Init
#ifdef USART_OPTIMIZE
#define putch(ch)	USART_PutByte(ch)
#define getch()		USART_GetByte()
#else
void	putch(u8);
u8		getch(void);
#endif
u8		getche(void);
void	puthex(u8);
void	putstr(u8 *);

#endif /* __USART_H__ */
