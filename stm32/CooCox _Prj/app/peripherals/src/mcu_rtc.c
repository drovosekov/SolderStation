/*
 *	File:	mcu_rtc.c
 *	Date:	09.01.2011
 */

#include "main.h"

#include "mcu_rtc.h"


#define TM_RTC_LEAP_YEAR(year) 			((((year) % 4 == 0) && ((year) % 100 != 0)) || ((year) % 400 == 0))
#define TM_RTC_DAYS_IN_YEAR(x)			TM_RTC_LEAP_YEAR(x) ? 366 : 365


static const u8 numofdays[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};


void init_rtc(void)
{
	/* Backup data register value is not correct or not yet programmed (when
       the first time the program is executed) */

	//if (BKP_ReadBackupRegister(BKP_DR1) != 0xA5A5)
	//{
	  /* RTC Configuration */
		configuration_rtc();
	//}
	//else
	//{
		RTC_WaitForSynchro();
	//}
}

void configuration_rtc(void)
{
	/* Allow access to BKP Domain */
	PWR_BackupAccessCmd(ENABLE);
	/* Reset Backup Domain */
	//BKP_DeInit(); //уже вызывается в init_default_values(); повторный вызов сбивает настройки регистров BCKP
	/* Enable LSE */
	RCC_LSEConfig(RCC_LSE_ON);
	/* Wait till LSE is ready */
	while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET);
	/* Select LSE as RTC Clock Source */
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
	/* Enable RTC Clock */
	RCC_RTCCLKCmd(ENABLE);
	/* Wait for RTC registers synchronization */
	RTC_WaitForSynchro();
	/* Wait until last write operation on RTC registers has finished */
	RTC_WaitForLastTask();
	/* Enable the RTC Second */
	//RTC_ITConfig(RTC_IT_SEC, ENABLE);
	/* Wait until last write operation on RTC registers has finished */
	//RTC_WaitForLastTask();
	/* Set RTC prescaler: set RTC period to 1sec */
	RTC_SetPrescaler(32767); /* RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1) */
	/* Wait until last write operation on RTC registers has finished */
	RTC_WaitForLastTask();
    BKP_WriteBackupRegister(BKP_DR1, 0xA5A5);
	PWR_BackupAccessCmd(DISABLE);
}


/*------------------------------------------*/
/* Convert time structure to timeticks      */
/*------------------------------------------*/
u32 RTC_GetTicksFromTime(const RTCTIME *rtc)
{
	uint32_t utc, i, y;
	y = rtc->year - 1970;
	if (y > 2106 || !rtc->month || !rtc->mday) return 0;

	utc = y / 4 * 1461;
	y %= 4;
	utc += y * 365 + (y > 2 ? 1 : 0);
	for (i = 0; i < 12 && i + 1 < rtc->month; i++) {
		utc += numofdays[i];
		if (i == 1 && y == 2) {utc++;}
	}
	utc += rtc->mday - 1;
	utc *= 86400;

	utc += rtc->hour * 3600 + rtc->min * 60 + rtc->sec;
	return utc;
}



/*------------------------------------------*/
/* Get time in calendar form                */
/*------------------------------------------*/
void RTC_GetTime(RTCTIME *rtc)
{
	uint32_t year, month, day, utc;
	utc = RTC_GetCounter();

	/* Compute  hours */
	rtc->sec = (u8)(utc % 60);
	utc /= 60;
	rtc->min = (u8)(utc % 60);
	utc /= 60;
	rtc->hour = (u8)(utc % 24);
	utc /= 24;

	rtc->wday = (u8)((utc + 4) % 7);

	rtc->year = (u16)(1970 + utc / 1461 * 4);
	utc %= 1461;
	year = ((utc >= 1096) ? utc - 1 : utc) / 365;
	rtc->year += year;
	utc -= year * 365 + (year > 2 ? 1 : 0);

	for (month = 0; month < 12; month++) {
		day = numofdays[month];
		if (month == 1 && year == 2) {day++;}
		if (utc < day) {break;}
		utc -= day;
	}
	rtc->month = (u8)(1 + month);
	rtc->mday  = (u8)(1 + utc);
}

/*------------------------------------------*/
/* Set time in calendar form                */
/*------------------------------------------*/
void RTC_SetTime(const RTCTIME* rtc)
{
	u32 utc = RTC_GetTicksFromTime(rtc);

	/* Allow access to BKP Domain */
	PWR_BackupAccessCmd(ENABLE);
	/* Reset Backup Domain */
	//BKP_DeInit();
	/* Enable LSE */
	RCC_LSEConfig(RCC_LSE_ON);
	/* Wait till LSE is ready */
	while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET);
	/* Select LSE as RTC Clock Source */
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
	/* Enable RTC Clock */
	RCC_RTCCLKCmd(ENABLE);
	/* Wait for RTC registers synchronization */
	RTC_WaitForSynchro();

	/* Wait until last write operation on RTC registers has finished */
	RTC_WaitForLastTask();
	/* Change the current time */
	RTC_SetCounter(utc);
	/* Wait until last write operation on RTC registers has finished */
	RTC_WaitForLastTask();

    BKP_WriteBackupRegister(BKP_DR1, 0xA5A5);
	PWR_BackupAccessCmd(DISABLE);
}

u8 RTC_DaysInMonth(u16 year, u8 month){
	u8 d = numofdays[month];
	if(month == 2 && TM_RTC_LEAP_YEAR(2000 + year)) {d++;}
	return d;
}

#if 0 //not used functions
/*------------------------------------------*/
/* Get time ticks from RTC                  */
/*------------------------------------------*/
uint32_t RTC_GetTicksFromRTC(time_count timetype)
{
	uint32_t timeticks = RTC_GetCounter();

    if(timetype==TIME_CURRENT)
	{
		return timeticks;
	}else if(timetype==TIME_MIDNIGHT)
	{
		timeticks /= 86400;
		timeticks *= 86400;
		return timeticks;
	}
	return 0;
}

uint32_t RTC_ClockHMtoTicks(char *s)
{
	unsigned int hours, mins;
	sscanf(s, "%u:%u", &hours, &mins);
	return hours*3600 + mins*60;
}

uint32_t RTC_ClockHMStoTicks(char *s)
{
	unsigned int hours, mins, secs;
	sscanf(s, "%u:%u:%u", &hours, &mins, &secs);
	return hours*3600 + mins*60 + secs;
}

void RTC_TicksToRTC (uint32_t ticks, RTCTIME* rtc)
{
	uint32_t n,i,d,utc;
	utc = ticks;
	/* Compute  hours */
	rtc->sec = (u8)(utc % 60);
	utc /= 60;
	rtc->min = (u8)(utc % 60);
	utc /= 60;
	rtc->hour = (u8)(utc % 24);
	utc /= 24;
	rtc->wday = (u8)((utc + 4) % 7);
	rtc->year = (u16)(1970 + utc / 1461 * 4);
	utc %= 1461;
	n = ((utc >= 1096) ? utc - 1 : utc) / 365;
	rtc->year += n;
	utc -= n * 365 + (n > 2 ? 1 : 0);
	for (i = 0; i < 12; i++) {
		d = numofdays[i];
		if (i == 1 && n == 2) d++;
		if (utc < d) break;
		utc -= d;
	}
	rtc->month = (uint8_t)(1 + i);
	rtc->mday = (uint8_t)(1 + utc);
}

#endif
