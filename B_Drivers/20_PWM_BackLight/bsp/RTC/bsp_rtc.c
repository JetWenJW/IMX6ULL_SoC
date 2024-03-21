#include "bsp_rtc.h"

/* RTC Initial */
void rtc_init(void)
{
    struct rtc_datatime rtcData;

    SNVS -> HPCOMR |= (1 << 31) | (1 << 8);

    rtcData.year   = 2024;
    rtcData.month  = 3;
    rtcData.hour   = 17;
    rtcData.minute = 6;
    rtcData.second = 0;

    rtc_setdatatime(&rtcData);
    /* RTC Enable */
    rtc_enable();
}

/* RTC Enable  */
void rtc_enable(void)
{
    SNVS -> LPCR |= (1 << 0);
    while((SNVS -> LPCR & 0x01) == 0); /* Wait Complete */
}

/* RTC Disable  */
void rtc_disable(void)
{
    SNVS -> LPCR &=~ (1 << 0);
    while((SNVS -> LPCR & 0x01) == 1); /* Wait Complete */
}

/* Algorithm transfer month,date,day into seconds */
uint64_t rtc_coverdate_to_seconds(struct rtc_datatime *datetime)
{	
	unsigned short i = 0;
	uint64_t seconds = 0;
	unsigned int days = 0;
	unsigned short monthdays[] = {0U, 0U, 31U, 59U, 90U, 120U, 151U, 181U, 212U, 243U, 273U, 304U, 334U};
	
	for(i = 1970; i < datetime -> year; i++)
	{
		days += DAYS_IN_A_YEAR; 		    /* 365days a year */
		if(rtc_isleapyear(i)) days += 1;    /* Leap year	  */
	}

	days += monthdays[datetime -> month];
	if(rtc_isleapyear(i) && (datetime -> month >= 3)) days += 1;/* 闰年，并且当前月份大于等于3月的话加一天 */

	days += datetime->day - 1;

	seconds = days * SECONDS_IN_A_DAY + 
				datetime -> hour * SECONDS_IN_A_HOUR +
				datetime -> minute * SECONDS_IN_A_MINUTE +
				datetime -> second;

	return seconds;	
}

/* Set Data Time */
void rtc_setdatatime(struct rtc_datatime *datatime)
{
    uint64_t second = 0;
    unsigned int tmp = SNVS -> LPCR;
    rtc_disable();

    second = rtc_coverdate_to_seconds(datatime);

	SNVS -> LPSRTCMR = (unsigned int)(second >> 17); /* 设置高16位 */
	SNVS -> LPSRTCLR = (unsigned int)(second << 15); /* 设置地16位 */

    if(tmp & 0x01)/* RTC has already enabled */
    {
        rtc_enable();
    }
}
/* Leap Year or not */
unsigned char rtc_isleapyear(unsigned short year)
{	
	unsigned char value=0;
	
	if(year % 400 == 0)
		value = 1;
	else 
	{
		if((year % 4 == 0) && (year % 100 != 0))
			value = 1;
		else 
			value = 0;
	}
	return value;
}
void rtc_convertseconds_to_datetime(unsigned int seconds, struct rtc_datatime *datetime)
{
    unsigned int x;
    unsigned int  secondsRemaining, days;
    unsigned short daysInYear;

    /* Each days of month       */
    unsigned char daysPerMonth[] = {0U, 31U, 28U, 31U, 30U, 31U, 30U, 31U, 31U, 30U, 31U, 30U, 31U};

    secondsRemaining = seconds; 
    days = secondsRemaining / SECONDS_IN_A_DAY + 1; 		
    secondsRemaining = secondsRemaining % SECONDS_IN_A_DAY; 

	/* Calculate Hour,Minute Seconds */
    datetime->hour = secondsRemaining / SECONDS_IN_A_HOUR;
    secondsRemaining = secondsRemaining % SECONDS_IN_A_HOUR;
    datetime -> minute = secondsRemaining / 60;
    datetime -> second = secondsRemaining % SECONDS_IN_A_MINUTE;

    /* Calculate Year */
    daysInYear = DAYS_IN_A_YEAR;
    datetime -> year = YEAR_RANGE_START;
    while(days > daysInYear)
    {
    
        days -= daysInYear;
        datetime -> year++;

        /* Leap Year */
        if (!rtc_isleapyear(datetime -> year))
            daysInYear = DAYS_IN_A_YEAR;
        else	/*Leap Year + 1 */
            daysInYear = DAYS_IN_A_YEAR + 1;
    }
	
    if(rtc_isleapyear(datetime -> year)) /* Leap year Feburary + 1 day */
        daysPerMonth[2] = 29;

    for(x = 1; x <= 12; x++)
    {
        if (days <= daysPerMonth[x])
        {
            datetime -> month = x;
            break;
        }
        else
        {
            days -= daysPerMonth[x];
        }
    }

    datetime -> day = days;

}

/* Read Seconds */
uint64_t rtc_getseconds(void)
{
    uint64_t seconds = 0;

    seconds = (SNVS -> LPSRTCMR << 17) | (SNVS -> LPSRTCLR >> 15);
    return seconds;
}

/* Get time Function */
void rtc_getdatetime(struct rtc_datatime *datatime)
{
    uint64_t seconds = 0;
    seconds = rtc_getseconds();
    rtc_convertseconds_to_datetime(seconds, datatime);
}







