#ifndef __EINT_H__
#define __EINT_H__

#define EINT_OPTIMIZE

/* external interrupt initialization */
void    EINT_Init(void);
/* external interrupt service routine */
void    EINT_ISR(void);

#endif
