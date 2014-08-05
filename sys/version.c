#include "types.h"
#include "usart.h"
#include "version.h"

void SYS_Info(void)
{
	putstr((u8 *)MOD_NAME);
	putch('-');
	putstr((u8 *)HW_REV);
	putch('-');
	putstr((u8 *)SW_REV);
	putch('\r');
}
