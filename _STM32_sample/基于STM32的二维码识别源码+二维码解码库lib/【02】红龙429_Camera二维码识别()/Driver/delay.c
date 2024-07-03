#include "delay.h"

__IO u16 time;								    
/**
  * @brief  ��ʱnms*1 ms
  * @param  nms:��ʱ����
  * @retval None
  */
void delay_ms(u16 nms)
{	 		  	  
	time=nms;
	SysTick_Config(180000);//1ms����һ���ж�
	while(time);
	SysTick->CTRL=0x00;			  	    
}   

/**
  * @brief  ��ʱnus*1 us
  * @param  nus:��ʱ����
  * @retval None
  */
void delay_us(u32 nus)
{		
	time=nus;
	SysTick_Config(180);//1us����һ���ж�
	while(time);
	SysTick->CTRL=0x00;
}

