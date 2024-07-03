/**
  * @file    RTC.c 
  * @author  WB R&D Team - openmcu666
  * @version V0.1
  * @date    2015.9.8
  * @brief   Real-Time Clock
  */

#include "rtc.h"
#include "delay.h"
#include "usart.h" 
								  

/**
  * @brief  设置时间
  * @param  Hour,Min,Sec:小时,分钟,秒钟
  * @retval 1:成功; 0:进入初始化模式失败       
  */
ErrorStatus RTC_Set_Time(u8 Hour,u8 Min,u8 Sec,u8 AM_PM)
{
	ErrorStatus status = ERROR;
	RTC_TimeTypeDef RTC_TimeTypeInitStructure;
	
	RTC_TimeTypeInitStructure.RTC_Hours=Hour;
	RTC_TimeTypeInitStructure.RTC_Minutes=Min;
	RTC_TimeTypeInitStructure.RTC_Seconds=Sec;
	RTC_TimeTypeInitStructure.RTC_H12=AM_PM;
	
	status=RTC_SetTime(RTC_Format_BIN,&RTC_TimeTypeInitStructure);
	return status;
}

/**
  * @brief  设置日期
  * @param  Year(0-99),Month,Date,Week:星期(1-7)        
  * @retval 1:成功 0:进入初始化模式失败       
  */
ErrorStatus RTC_Set_Date(u8 Year,u8 Month,u8 Date,u8 Week)
{
	ErrorStatus status = ERROR;
	RTC_DateTypeDef RTC_DateTypeInitStructure;
	
	RTC_DateTypeInitStructure.RTC_Year=Year;
	RTC_DateTypeInitStructure.RTC_Month=Month;
	RTC_DateTypeInitStructure.RTC_Date=Date;
	RTC_DateTypeInitStructure.RTC_WeekDay=Week;
	
  status=RTC_SetDate(RTC_Format_BIN,&RTC_DateTypeInitStructure);
	return status;
}

/**
  * @brief  设置闹钟时间
  * @param  Week:星期;Hour:小时;,Min:分钟;sec:秒钟
  * @retval None    
  */
void RTC_Set_AlarmA(u8 Week,u8 Hour,u8 Min,u8 Sec)
{ 
	NVIC_InitTypeDef   NVIC_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;
	RTC_AlarmTypeDef RTC_AlarmTypeInitStructure;
	RTC_TimeTypeDef RTC_TimeTypeInitStructure;
	
	RTC_AlarmCmd(RTC_Alarm_A,DISABLE);                                    //关闭闹钟A 
	
  RTC_TimeTypeInitStructure.RTC_Hours=Hour;
	RTC_TimeTypeInitStructure.RTC_Minutes=Min;
	RTC_TimeTypeInitStructure.RTC_Seconds=Sec;
	RTC_TimeTypeInitStructure.RTC_H12=RTC_H12_AM;
  
	RTC_AlarmTypeInitStructure.RTC_AlarmDateWeekDay=Week;
	RTC_AlarmTypeInitStructure.RTC_AlarmDateWeekDaySel=RTC_AlarmDateWeekDaySel_WeekDay;//以星期为周期
	RTC_AlarmTypeInitStructure.RTC_AlarmMask=RTC_AlarmMask_None;  
	RTC_AlarmTypeInitStructure.RTC_AlarmTime=RTC_TimeTypeInitStructure;
  RTC_SetAlarm(RTC_Format_BIN,RTC_Alarm_A,&RTC_AlarmTypeInitStructure);
 
	RTC_ClearITPendingBit(RTC_IT_ALRA);
  EXTI_ClearITPendingBit(EXTI_Line17);
	
	RTC_ITConfig(RTC_IT_ALRA,ENABLE);
	RTC_AlarmCmd(RTC_Alarm_A,ENABLE);
	
	EXTI_InitStructure.EXTI_Line = EXTI_Line17;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; 
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = RTC_Alarm_IRQn; 
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/**
  * @brief  RTC唤醒
  * @param  ClockSource:时钟源(详见stm32f4xx_rtc.h 366行)
            Reload:自动重装载值.减到0,产生中断.
  * @retval None    
  */
void RTC_Set_WakeUp(u32 ClockSource,u16 Reload)
{ 
	EXTI_InitTypeDef   EXTI_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;
	RTC_WakeUpCmd(DISABLE);
	
	RTC_WakeUpClockConfig(ClockSource);                     //唤醒时钟选择
	
	RTC_SetWakeUpCounter(Reload);
	
	
	RTC_ClearITPendingBit(RTC_IT_WUT); 
  EXTI_ClearITPendingBit(EXTI_Line22);
	 
	RTC_ITConfig(RTC_IT_WUT,ENABLE);
	RTC_WakeUpCmd( ENABLE);
	
	EXTI_InitStructure.EXTI_Line = EXTI_Line22;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; 
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
 
 
	NVIC_InitStructure.NVIC_IRQChannel = RTC_WKUP_IRQn; 
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}


/**
  * @brief  初始化实时时钟
  * @param  None
  * @retval 0:初始化成功;
            1:LSE开启失败;
  */
u8 RTC_Config(void)
{
	RTC_InitTypeDef RTC_InitStructure;
	u16 retry=0x1FFF; 
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
	PWR_BackupAccessCmd(ENABLE);
	
	if(RTC_ReadBackupRegister(RTC_BKP_DR0)!=0xA5A5)		         //是否第一次配置
	{
		RCC_LSEConfig(RCC_LSE_ON);                               
		while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)	     //等待LSE起振
			{
			  retry++;
			  delay_ms(10);
			}
		if(retry==0)return 1;		                                   //LSE 开启失败. 
			
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);		                  //设置RTC时钟(RTCCLK),选择LSE作为RTC时钟    
		RCC_RTCCLKCmd(ENABLE);	                                    //使能RTC时钟 

    RTC_InitStructure.RTC_AsynchPrediv = 0x7F;
    RTC_InitStructure.RTC_SynchPrediv  = 0xFF;
    RTC_InitStructure.RTC_HourFormat   = RTC_HourFormat_24;      //24小时格式
    RTC_Init(&RTC_InitStructure);
 
		RTC_Set_Time(11,40,9,RTC_H12_AM);               	           //设置时间
		RTC_Set_Date(15,9,8,2);		                                   //设置年月日周
	 
		RTC_WriteBackupRegister(RTC_BKP_DR0,0xA5A5);	               //标记已初始化,下次启动无需配置(带纽扣电池)
	} 
 
	return 0;
}
