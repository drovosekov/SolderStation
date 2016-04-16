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

//�������� ����� �� RTC (������� ��� � �������)
void RTC_GetTime(RTCTIME *rtc);

//���������� ����� � RTC
void RTC_SetTime(const RTCTIME *rtc);

//���������� ���������� ���� � ������
u8 RTC_DaysInMonth(u16 year, u8 month);

//������������� ����� �� RTC � ticks
u32 RTC_GetTicksFromTime(const RTCTIME *rtc);

#if 0
//�������� ����� � ����� (������� ��� � �������)
uint32_t RTC_GetTicksFromRTC(uint8_t timetype);

//������ ���� "12:10" "hh:mm" � ��������
uint32_t RTC_ClockHMtoTicks(char *s);

//������ ���� "02:10" "mm:ss" � ��������
uint32_t RTC_ClockMStoTicks(char *s);

//������������ ticks � ��������� RTCTIME
void RTC_TicksToRTC (uint32_t ticks, RTCTIME *rtc);
#endif


#endif /* MCU_RTC_H_ */
