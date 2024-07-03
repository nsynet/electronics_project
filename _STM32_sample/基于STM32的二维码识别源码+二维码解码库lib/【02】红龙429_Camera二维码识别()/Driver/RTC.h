#ifndef __RTC_H
#define __RTC_H	 
#include <stm32f4xx.h>
						  
					
ErrorStatus RTC_Set_Time(u8 Hour,u8 Min,u8 Sec,u8 AM_PM);		
ErrorStatus RTC_Set_Date(u8 Year,u8 Month,u8 Date,u8 Week);		
void RTC_Set_AlarmA(u8 Week,u8 Hour,u8 Min,u8 Sec);
void RTC_Set_WakeUp(u32 wksel,u16 cnt);					
u8 RTC_Config(void);	
#endif

















