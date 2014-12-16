#include <htc.h>
#include "usart.h"
#include "task.h"

//#define DEBUG

#define USART_RXBUFF_SIZE	4

static u8 rx_buff[USART_RXBUFF_SIZE];

/* USART module initialization */
void USART_Init(void)
{
    RX_PIN = 1;                         /* set RX_PIN as input */
    TX_PIN = 0;                         /* set TX_PIN as output */
    ANSEL = 0x00;                       /* config analog chanel as digital input */
    SPBRG = DIVIDER;                    /* baudrate */
#ifdef USART_OPTIMIZE
    TXSTA = (SPEED | NINE_BITS | 0x20); /* config speed, 9-bit transmit, enable TX */
    RCSTA = (NINE_BITS | 0x90);         /* 9-bit receive, enable RX, serial port enable */
#else
    RX9 = NINE;                         /* 9-bit receive */
    TX9 = NINE;                         /* 9-bit transmit */
    BRGH = HIGH_SPEED;                  /* config speed */
    SYNC = 0;                           /* asynchronous */
    CREN = 1;                           /* enable RX */
    TXEN = 1;                           /* enable TX */
    SPEN = 1;                           /* serial port enable */
#endif

    RCIE = 1;                           /* enable usart rx interrupt */
    PEIE = 1;                           /* enables all unmasked interrupts */
    GIE = 1;                            /* make sure global interrupts enable */
}

/* Put one byte to USART to transmit */
void USART_PutByte(u8 byte)
{
    while (!TXIF)	/* set when register is empty */
        continue;

    TXREG = byte;
}

/* Get one byte from USART receiver */
u8 USART_GetByte(void)
{
    while (!RCIF)	/* set when register is not empty */
        continue;

    return RCREG;	/* RXD9 and FERR are gone now */
}

/* Get the ninth bit */
u8 USART_GetNinth(void)
{
    while (!RCIF)
        continue;

    return RX9D;	/* RCIF is not cleared until RCREG is read */
}

/* Get receiving framing error */
u8 USART_GetFERR(void)
{
    while (!RCIF)
        continue;

    return FERR;	/* RCIF is not cleared until RCREG is read */
}

/* Check receiver overrun error */
u8 USART_CheckOERR(void)
{
    if (OERR)	/* re-enable after overrun error */
    {
        CREN = 0;
        CREN = 1;

        return 1;
    }

    return 0;
}

/* USART Rx interrupt service routine */
void USART_Rx_ISR(void)
{
    static u8 idx = 0;

    USART_CheckOERR();
#ifdef DEBUG
    /* echo received byte back */
    putch(RCREG);
#endif

    if (RCREG == 0x0D) {
        /* send msg to main loop */
        Task_Push(MSG_ID_CMDRCV_IND, idx, (u16)rx_buff);
        idx = 0;
        return;
    }

    if (idx >= USART_RXBUFF_SIZE)
        return;

    rx_buff[idx++] = RCREG;
}


#ifndef USART_OPTIMIZE
void putch(u8 byte)
{
    /* output one byte */
    while (!TXIF)	/* set when register is empty */
        continue;

    TXREG = byte;
}

u8 getch(void)
{
    /* retrieve one byte */
    while (!RCIF)	/* set when register is not empty */
        continue;

    return RCREG;
}
#endif

u8 getche(void)
{
    u8 c;

    putch(c = getch());

    return c;
}

void puthex(u8 byte)
{
    u8 c;

    c = byte >> 4;
#ifdef USART_OPTIMIZE
    c +=  (c <= 9) ? '0' : 0x37;	/* 0x37 = 'A' - 0xA */
#else
    c +=  ((c <= 9) ? '0' : ('A' - 0xA));
#endif
    putch(c);

    c = byte & 0xF;
#ifdef USART_OPTIMIZE
    c +=  (c <= 9) ? '0' : 0x37;	/* 0x37 = 'A' - 0xA */
#else
    c +=  ((c <= 9) ? '0' : ('A' - 0xA));
#endif
    putch(c);
}

void putstr(u8 *str)
{
    while (*str) {
        putch(*str++);
    }
}
