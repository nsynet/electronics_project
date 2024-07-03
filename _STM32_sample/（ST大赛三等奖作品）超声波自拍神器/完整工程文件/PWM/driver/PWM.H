/*********************************Copyright (c)*********************************                                      
  *                                           
  ******************************************************************************
  * 文件名称: PWM.H
  * 描    述: PWM头文件
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

#ifndef  __PWM_H__
#define  __PWM_H__

/**********************头文件调用及变量定义声明*********************************/
/*  包含必要的头文件  */
//端口宏定义
// 六路PWM--TIMER1-CH1--PA8 
// 六路PWM--TIMER1-CH2--PA9 
// 六路PWM--TIMER1-CH3--PA10 
// 六路PWM--TIMER1-CH1N--PB13  
// 六路PWM--TIMER1-CH2N--PB14    
// 六路PWM--TIMER1-CH3N--PB15   

#define F072B_TIMER_PWM              TIM1          
#define F072B_TIMER_CLK              RCC_APB2Periph_TIM1

#define F072B_PWM_CH_P0RT            GPIOA  
#define F072B_PWM_CH_CLK             RCC_AHBPeriph_GPIOA

#define F072B_PWM_CHN_P0RT           GPIOB
#define F072B_PWM_CHN_CLK            RCC_AHBPeriph_GPIOB

#define F072B_PWM_CH1_PIN             GPIO_Pin_8         
#define F072B_PWM_CH1_SOURCE          GPIO_PinSource8
#define F072B_PWM_CH1_AF              GPIO_AF_2

#define F072B_PWM_CH2_PIN             GPIO_Pin_9    
#define F072B_PWM_CH2_SOURCE          GPIO_PinSource9
#define F072B_PWM_CH2_AF              GPIO_AF_2

#define F072B_PWM_CH3_PIN             GPIO_Pin_10           
#define F072B_PWM_CH3_SOURCE          GPIO_PinSource10
#define F072B_PWM_CH3_AF              GPIO_AF_2

#define F072B_PWM_CH1N_PIN            GPIO_Pin_13                       
#define F072B_PWM_CH1N_SOURCE         GPIO_PinSource13
#define F072B_PWM_CH1N_AF             GPIO_AF_2

#define F072B_PWM_CH2N_PIN            GPIO_Pin_14                       
#define F072B_PWM_CH2N_SOURCE         GPIO_PinSource14
#define F072B_PWM_CH2N_AF             GPIO_AF_2

#define F072B_PWM_CH3N_PIN            GPIO_Pin_15                       
#define F072B_PWM_CH3N_SOURCE         GPIO_PinSource15
#define F072B_PWM_CH3N_AF             GPIO_AF_2
 
/*  子函数定义        */
extern void PWM_GPIO_Init(void);
extern void Timer1_Init(unsigned short int arr,unsigned short int psc);
extern void Timer1_Pwm_Adj(void);
extern void Timer1_Duty_Adj(void);
#endif  

