#ifndef _RTC_H
#define _RTC_H
#include "config.h"

void RTC_Config(void);
void RTC_set(u32 count);
void RTC_UpdateTime(void);
void RTC_GetCalendar(u32 timecount,TIME *time);
u8 RTC_GetWeek(u16 _year,u8 _month,u8 _day);



#endif

