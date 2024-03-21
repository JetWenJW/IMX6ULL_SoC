#ifndef __BSP_RTC_H
#define __BSP_RTC_H

#include "imx6u.h"

/* Define Time of Macro */
#define SECONDS_IN_A_DAY            (86400)
#define SECONDS_IN_A_HOUR           (3600)
#define SECONDS_IN_A_MINUTE         (60)
#define DAYS_IN_A_YEAR              (365)
#define YEAR_RANGE_START            (1970)
#define YEAR_RANGE_END              (2099)

/* Definr Time struct */
struct rtc_datatime
{
    unsigned short  year;
    unsigned char   month;
    unsigned char   day;
    unsigned char   hour;
    unsigned char   minute;
    unsigned char   second;
};

/* Function Declare */
void rtc_enable(void);
void rtc_init(void);
uint64_t rtc_coverdate_to_seconds(struct rtc_datatime *datetime);
void rtc_setdatatime(struct rtc_datatime *datatime);
uint64_t rtc_getseconds(void);
void rtc_convertseconds_to_datetime(unsigned int seconds, struct rtc_datatime *datetime);
void rtc_getdatetime(struct rtc_datatime *datatime);
void rtc_disable(void);
unsigned char rtc_isleapyear(unsigned short year);









#endif