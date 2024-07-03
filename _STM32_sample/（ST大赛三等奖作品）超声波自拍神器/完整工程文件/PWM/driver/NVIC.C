/*********************************Copyright (c)*********************************                                      
  *                                                  
  ******************************************************************************
  * 文件名称：NVIC.C
  * 描    述：NVIC管理模块
  * 创 建 者: 
  * 创建日期: 
  * 修 改 者:             
  * 修改日期: 
  * 版    本: 
  ******************************************************************************
  * attention
  *
  ******************************************************************************
  */
/*Includes --------------------------------------------------------------------*/
  #include "includes.h"
  #include "NVIC.H"
/* 类型定义 typedef-----------------------------------------------------------*/
/* 预定义字符     ------------------------------------------------------------*/
/* 宏定义        -------------------------------------------------------------*/
/********************************************************************************
函数名称：Nvic_Init(void)
功    能：Nvic_Init配置
参    数：无
返 回 值：无
*********************************************************************************/
void Nvic_Init(void)
{ 
	
   NVIC_InitTypeDef  NVIC_InitStructure;
  
	
  /*1--串口中断管理*/
	 NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
   NVIC_InitStructure.NVIC_IRQChannelPriority = 0x00;
   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
   NVIC_Init(&NVIC_InitStructure); 
   NVIC_InitStructure.NVIC_IRQChannel = KEY1_BUTTON_EXTI_IRQn;
   NVIC_InitStructure.NVIC_IRQChannelPriority = 0x03;
   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
   NVIC_Init(&NVIC_InitStructure);
  
  // 可以继续增加，设置中断优先级

  

}
