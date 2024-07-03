/***********************************************************************
**LPC2xxx RTC
**(c)ANCHEY.COM
**************************************************************************/

#include <lpc213x.h>
#include <mydef.h>
#include <lpc_reg.h>

#ifndef __RTC_H__
#define __RTC_H__

typedef __packed struct _rtc_time_format
{
    uint16_t year;
    uint8_t month;
    uint8_t day;
    uint8_t week;

    uint8_t hour;
    uint8_t minute;
    uint8_t second;
} rtc_time_t;

extern void rtc_start(void);

extern void rtc_stop(void);

extern void rtc_init(void);

extern void rtc_set_time(rtc_time_t *_time);

extern void rtc_read_time(rtc_time_t *_time);

#endif
