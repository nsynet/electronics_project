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
  * @brief  ����ʱ��
  * @param  Hour,Min,Sec:Сʱ,����,����
  * @retval 1:�ɹ�; 0:�����ʼ��ģʽʧ��       
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
  * @brief  ��������
  * @param  Year(0-99),Month,Date,Week:����(1-7)        
  * @retval 1:�ɹ� 0:�����ʼ��ģʽʧ��       
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
  * @brief  ��������ʱ��
  * @param  Week:����;Hour:Сʱ;,Min:����;sec:����
  * @retval None    
  */
void RTC_Set_AlarmA(u8 Week,u8 Hour,u8 Min,u8 Sec)
{ 
	NVIC_InitTypeDef   NVIC_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;
	RTC_AlarmTypeDef RTC_AlarmTypeInitStructure;
	RTC_TimeTypeDef RTC_TimeTypeInitStructure;
	
	RTC_AlarmCmd(RTC_Alarm_A,DISABLE);                                    //�ر�����A 
	
  RTC_TimeTypeInitStructure.RTC_Hours=Hour;
	RTC_TimeTypeInitStructure.RTC_Minutes=Min;
	RTC_TimeTypeInitStructure.RTC_Seconds=Sec;
	RTC_TimeTypeInitStructure.RTC_H12=RTC_H12_AM;
  
	RTC_AlarmTypeInitStructure.RTC_AlarmDateWeekDay=Week;
	RTC_AlarmTypeInitStructure.RTC_AlarmDateWeekDaySel=RTC_AlarmDateWeekDaySel_WeekDay;//������Ϊ����
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
  * @brief  RTC����
  * @param  ClockSource:ʱ��Դ(���stm32f4xx_rtc.h 366��)
            Reload:�Զ���װ��ֵ.����0,�����ж�.
  * @retval None    
  */
void RTC_Set_WakeUp(u32 ClockSource,u16 Reload)
{ 
	EXTI_InitTypeDef   EXTI_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;
	RTC_WakeUpCmd(DISABLE);
	
	RTC_WakeUpClockConfig(ClockSource);                     //����ʱ��ѡ��
	
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
  * @brief  ��ʼ��ʵʱʱ��
  * @param  None
  * @retval 0:��ʼ���ɹ�;
            1:LSE����ʧ��;
  */
u8 RTC_Config(void)
{
	RTC_InitTypeDef RTC_InitStructure;
	u16 retry=0x1FFF; 
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
	PWR_BackupAccessCmd(ENABLE);
	
	if(RTC_ReadBackupRegister(RTC_BKP_DR0)!=0xA5A5)		         //�Ƿ��һ������
	{
		RCC_LSEConfig(RCC_LSE_ON);                               
		while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)	     //�ȴ�LSE����
			{
			  retry++;
			  delay_ms(10);
			}
		if(retry==0)return 1;		                                   //LSE ����ʧ��. 
			
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);		                  //����RTCʱ��(RTCCLK),ѡ��LSE��ΪRTCʱ��    
		RCC_RTCCLKCmd(ENABLE);	                                    //ʹ��RTCʱ�� 

    RTC_InitStructure.RTC_AsynchPrediv = 0x7F;
    RTC_InitStructure.RTC_SynchPrediv  = 0xFF;
    RTC_InitStructure.RTC_HourFormat   = RTC_HourFormat_24;      //24Сʱ��ʽ
    RTC_Init(&RTC_InitStructure);
 
		RTC_Set_Time(11,40,9,RTC_H12_AM);               	           //����ʱ��
		RTC_Set_Date(15,9,8,2);		                                   //������������
	 
		RTC_WriteBackupRegister(RTC_BKP_DR0,0xA5A5);	               //����ѳ�ʼ��,�´�������������(��Ŧ�۵��)
	} 
 
	return 0;
}
