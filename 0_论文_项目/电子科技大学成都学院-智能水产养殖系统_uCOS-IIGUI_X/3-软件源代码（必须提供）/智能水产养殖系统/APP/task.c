#include "stm32f10x.h"
#include "pid.h"

GPIO_InitTypeDef GPIO_InitStructure;
#define LED1_OFF GPIO_SetBits(GPIOA, GPIO_Pin_0);  
#define LED1_ON GPIO_ResetBits(GPIOA, GPIO_Pin_0); 

#define LED2_OFF GPIO_SetBits(GPIOA, GPIO_Pin_1);  
#define LED2_ON GPIO_ResetBits(GPIOA, GPIO_Pin_1); 

#define LED3_OFF GPIO_SetBits(GPIOA, GPIO_Pin_2);  
#define LED3_ON GPIO_ResetBits(GPIOA, GPIO_Pin_2);

#define LED4_OFF GPIO_SetBits(GPIOA, GPIO_Pin_3);  
#define LED4_ON GPIO_ResetBits(GPIOA, GPIO_Pin_3);

#define LED5_OFF GPIO_SetBits(GPIOA, GPIO_Pin_4);  
#define LED5_ON GPIO_ResetBits(GPIOA, GPIO_Pin_4); 

#define LED6_OFF GPIO_SetBits(GPIOA, GPIO_Pin_5);  
#define LED6_ON GPIO_ResetBits(GPIOA, GPIO_Pin_5); 
		   
#define LED7_OFF GPIO_SetBits(GPIOA, GPIO_Pin_6);  
#define LED7_ON GPIO_ResetBits(GPIOA, GPIO_Pin_6);  

#define LED8_OFF GPIO_SetBits(GPIOA, GPIO_Pin_7);  
#define LED8_ON GPIO_ResetBits(GPIOA, GPIO_Pin_7);     

#define LED_OFF_ALL GPIO_SetBits(GPIOA, GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7); //关闭所有LED  

void LED_Config(void);
void KEY_Config(void);


/****************************************************************************
* 名    称：void LED_Config(void)
* 功    能：LED 控制初始化函数
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/ 
void LED_Config(void)
{
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3
  |GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12;				     //8个LED
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			 //口线翻转速度为50MHz
  GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/****************************************************************************
* 名    称：void KEY_Config(void)
* 功    能：LED 控制初始化函数
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/ 
void KEY_Config(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3; //4个按键
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	GPIO_SetBits(GPIOE, GPIO_Pin_All);					 
}
