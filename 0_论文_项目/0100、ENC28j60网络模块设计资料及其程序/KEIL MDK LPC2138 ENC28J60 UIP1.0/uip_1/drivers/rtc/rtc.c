/***********************************************************************
**LPC2xxx RTC
**(c)ANCHEY.COM
**************************************************************************/

#include <lpc213x.h>
#include <mydef.h>
#include <lpc_reg.h>

#define CTC_STOP clr_bit(CCR, CLKEN)
#define CTC_START set_bit(CCR, CLKEN)

#define CTC_RST set_bit(CCR, CTCRST)
#define CTC_KEEP clr_bit(CCR, CTCRST)



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


void rtc_stop(void)
{
    CTC_STOP;
    CTC_RST;
}

void rtc_start(void)
{
    CTC_START;
    CTC_KEEP;
}
/*----------------------------------------------------------*\
 | Realtime Initial                                         |
\*----------------------------------------------------------*/
void rtc_init(void)
{
	PCONP  |= 0x00000200;   
	CCR     = 0x00000002;
	CIIR    = 0x00000000;
	AMR     = 0x000000FF;
	ILR     = 0x00000003;
	//PREINT  = FPCLK / 32768 - 1;
	//PREFRAC = FPCLK - (FPCLK / 32768) * 32768;
	CCR     = 0x00000011;
}
/*----------------------------------------------------------*\
 | Set current time                                         |
\*----------------------------------------------------------*/
void rtc_set_time(rtc_time_t *_time)
{
    rtc_stop();

	YEAR  = _time->year ;
	MONTH = _time->month ;
	DOM   = _time->day;
	HOUR  = _time->hour;
	MIN   = _time->minute;
	SEC   = _time->second ;

    rtc_start();

}

void rtc_read_time(rtc_time_t *_time)
{

    _time->year = YEAR;
    _time->month = MONTH;
    _time->day = DOM;
    _time->hour = HOUR;
    _time->minute = MIN;
    _time->second = SEC;

}
