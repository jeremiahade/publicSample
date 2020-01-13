/*
 * RTC.h
 *
 * Created: 7/6/2019 11:40:34 PM
 *  Author: Jeremiah.A
 */ 


#ifndef RTC_H_INCLUDED
#define RTC_H_INCLUDED


#ifdef __cplusplus
extern "C" {
	#endif

typedef struct
{
	uint8_t sec;
	uint8_t min;
	int hour;
	uint8_t weekDay;
	uint8_t date;
	uint8_t month;
	uint8_t year;
}rtc_t;

void RTC_Init(void);
void RTC_SetDateTime(rtc_t *rtc);
void RTC_GetDateTime(rtc_t *rtc);
uint8_t rtcisrunning(void);


#ifdef __cplusplus
}
#endif

#endif /* LCD_I2C.H_INCLUDED */
