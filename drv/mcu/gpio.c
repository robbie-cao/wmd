#include <htc.h>
#include "types.h"
#include "gpio.h"

/* GPIO initialization */
void GPIO_Init(void)
{
}

#ifndef GPIO_OPTIMIZE
/* Setup GPIO (port) direction */
void GPIO_Setup(u8 port, bool direction)
{
    if (direction)
        TRISC |= (1 << port);	/* set bit */
    else
        TRISC &= ~(1 << port);	/* clr bit */
}

/* Setup GPIO (port) as input */
void GPIO_SetInput(u8 port)
{
    TRISC |= (1 << port);	/* set bit */
}

/* Setup GPIO (port) as output */
void GPIO_SetOutput(u8 port)
{
    TRISC &= ~(1 << port);	/* clr bit */
}

/* Read GPIO (port) high/low status */
bool GPIO_Read(u8 port)
{
    return ((PORTC >> port) & 0x1);
}

/* Set GPIO (port) high/low status */
void GPIO_Write(u8 port, bool data)
{
    if (data)
        PORTC |= (1 << port);	/* set bit */
    else
        PORTC &= ~(1 << port);	/* clr bit */
}

/* Set GPIO (port) bit */
void GPIO_Set(u8 port)
{
    PORTC |= (1 << port);	/* set bit */
}

/* Clear GPIO (port) bit */
void GPIO_Clear(u8 port)
{
    PORTC &= ~(1 << port);	/* clr bit */
}

#endif
