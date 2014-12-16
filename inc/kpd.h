#ifndef __KPD_H__
#define __KPD_H__

#define KPD_ID_A            0x00
#define KPD_ID_B            0x01
#define KPD_ID_ON           0x02

#define KPD_EVENT_PRESS     0x00
#define KPD_EVENT_RELEASE   0x01
#define KPD_EVENT_LONGPRESS 0x02
#define KPD_EVENT_REPEAT    0x03

#define KPD_LP_TIME         (TICKS_1_S * 2)

void    KPD_PressInd(void);
void    KPD_ReleaseInd(void);
void    KPD_LongPressInd(void);

#endif /* __KPD_H__ */
