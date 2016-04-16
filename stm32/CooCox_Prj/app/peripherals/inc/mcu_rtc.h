/*
 *	File:	mcu_rtc.h
 *	Date:	09.01.2011
 */

#ifndef MCU_RTC_H_
#define MCU_RTC_H_

#include "stm32f10x.h"                  // Device header

typedef struct {
	uint16_t	year;	/* 1970..2106 */
	uint8_t		month;	/* 1..12 */
	uint8_t		mday;	/* 1..31 */
	uint8_t		hour;	/* 0..23 */
	uint8_t		min;	/* 0..59 */
	uint8_t		sec;	/* 0..59 */
	uint8_t		wday;	/* 0..6 (Sun..Sat) */
} RTCTIME;

#define TM_RTC_LEAP_YEAR(year) 			((((year) % 4 == 0) && ((year) % 100 != 0)) || ((year) % 400 == 0))
#define TM_RTC_DAYS_IN_YEAR(x)			TM_RTC_LEAP_YEAR(x) ? 366 : 365


void init_rtc(void);
void configuration_rtc(void);

//получить врем€ из RTC (текущее или в полночь)
void RTC_GetTime(RTCTIME *rtc);

//установить врем€ в RTC
void RTC_SetTime(const RTCTIME *rtc);

//возвращает количество дней в мес€це
u8 RTC_DaysInMonth(u16 year, u8 month);

//преобразовать врем€ из RTC в ticks
u32 RTC_GetTicksFromTime(const RTCTIME *rtc);

#if 0
//получить врем€ в тиках (текущее или в полночь)
uint32_t RTC_GetTicksFromRTC(uint8_t timetype);

//строку вида "12:10" "hh:mm" в таймтики
uint32_t RTC_ClockHMtoTicks(char *s);

//строку вида "02:10" "mm:ss" в таймтики
uint32_t RTC_ClockMStoTicks(char *s);

//конвертирует ticks в структуру RTCTIME
void RTC_TicksToRTC (uint32_t ticks, RTCTIME *rtc);
#endif


#endif /* MCU_RTC_H_ */
