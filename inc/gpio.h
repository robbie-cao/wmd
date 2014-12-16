#ifndef __GPIO_H__
#define __GPIO_H__

#include <htc.h>
#include "types.h"

#define GPIO_OPTIMIZE

#define GPIO_INPUT      1
#define GPIO_OUTPUT     0

/*
 * PORTC in PIC16F690 is an 8-bit wide, bidirectional port,
 * We use this PORTC as GPIO purpose. When we say GPIO (#x),
 * the actual port we are using is RCx.
 */

/* GPIO initialization */
void    GPIO_Init(void);

#ifdef GPIO_OPTIMIZE
#if 1
#define GPIO_Setup(port, dir)   ((dir) ? (TRISC |= (1 << (port))) : (TRISC &= ~(1 << (port))))
#define GPIO_SetInput(port)     (TRISC |= (1 << (port)))
#define GPIO_SetOutput(port)    (TRISC &= ~(1 << (port)))
#define GPIO_Read(port)         ((PORTC >> port) & 0x1)
#define GPIO_Write(port, data)  ((data) ? (PORTC |= (1 << (port))) : (PORTC &= ~(1 << (port))))
#define GPIO_Set(port)          (PORTC |= (1 << (port)))
#define GPIO_Clear(port)        (PORTC &= ~(1 << (port)))
#else
#define GPIO_Setup(port, dir)   (TRISC##port = dir)
#define GPIO_SetInput(port)     (TRISC##port = 1)
#define GPIO_SetOutput(port)    (TRISC##port = 0)
#define GPIO_Read(port)         (RC##port)
#define GPIO_Write(port, data)  (RC##port = data)
#define GPIO_Set(port)          (RC##port = 1)
#define GPIO_Clear(port)        (RC##port = 0)
#endif
#else
/* Setup GPIO (port) direction */
void    GPIO_Setup(u8 port, bool direction);
/* Setup GPIO (port) as input */
void    GPIO_SetInput(u8 port);
/* Setup GPIO (port) as output */
void    GPIO_SetOutput(u8 port);
/* Read GPIO (port) high/low status */
bool    GPIO_Read(u8 port);
/* Set GPIO (port) high/low status */
void    GPIO_Write(u8 port, bool data);
/* Set GPIO (port) bit */
void    GPIO_Set(u8 port);
/* Clear GPIO (port) bit */
void    GPIO_Clear(u8 port);
#endif

#endif /* __GPIO_H__ */
