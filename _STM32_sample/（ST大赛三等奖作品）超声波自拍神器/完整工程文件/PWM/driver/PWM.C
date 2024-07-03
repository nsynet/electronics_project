/*********************************Copyright (c)*********************************                                      
  *                                                  
  ******************************************************************************
  * 文件名称：PWM.C
  * 描    述：PWM模块
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
  #include "PWM.H" 
/* 类型定义 typedef-----------------------------------------------------------*/
/* 预定义字符     ------------------------------------------------------------*/
/* 宏定义        -------------------------------------------------------------*/
/* 变量定义          ---------------------------------------------------------*/ 

unsigned short int    Pwm_period_num = 0; //定时周期

unsigned short int  CCR1_Val = 0;
unsigned short int  CCR2_Val = 0;
unsigned short int  CCR3_Val = 0;


/*******************************************************************************
* 函数名称: Timer1_Init();
* 功能描述: 定时器1初始化
* 输入参数: void
* 返回参数: 无
********************************************************************************/
void PWM_GPIO_Init(void)
{
     GPIO_InitTypeDef GPIO_InitStructure;
     //时钟配置
	   RCC_AHBPeriphClockCmd(F072B_PWM_CH_CLK|F072B_PWM_CHN_CLK , ENABLE);   // 使能GPIOA/GPIOB端口
	  
     //GPIOA通道配置 : TIM1 CH1 (PA8)/TIM1 CH2 (PA9) /TIM1 CH3 (PA10)*/
	   GPIO_InitStructure.GPIO_Pin =   F072B_PWM_CH1_PIN|F072B_PWM_CH2_PIN|F072B_PWM_CH3_PIN;
		 GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_AF;
		 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		 GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		 GPIO_InitStructure.GPIO_PuPd =  GPIO_PuPd_UP ;
		 GPIO_Init(F072B_PWM_CH_P0RT , &GPIO_InitStructure);
	    
     //GPIOB通道配置 : TIM1 CH1N (PB13)/TIM1 CH2N (PB14) /TIM1 CH3N (PB15)*/
	   GPIO_InitStructure.GPIO_Pin =   F072B_PWM_CH1N_PIN|F072B_PWM_CH2N_PIN|F072B_PWM_CH3N_PIN;
		 GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_AF;
		 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		 GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		 GPIO_InitStructure.GPIO_PuPd =  GPIO_PuPd_UP ;
		 GPIO_Init(F072B_PWM_CHN_P0RT , &GPIO_InitStructure);

	   //端口复用功能配置
     GPIO_PinAFConfig(F072B_PWM_CH_P0RT,    F072B_PWM_CH1_SOURCE,  F072B_PWM_CH1_AF);
		 GPIO_PinAFConfig(F072B_PWM_CH_P0RT ,   F072B_PWM_CH2_SOURCE,  F072B_PWM_CH2_AF);
		 GPIO_PinAFConfig(F072B_PWM_CH_P0RT ,   F072B_PWM_CH3_SOURCE,  F072B_PWM_CH3_AF);		
     GPIO_PinAFConfig(F072B_PWM_CHN_P0RT ,  F072B_PWM_CH1N_SOURCE, F072B_PWM_CH1N_AF);
		 GPIO_PinAFConfig(F072B_PWM_CHN_P0RT ,  F072B_PWM_CH2N_SOURCE, F072B_PWM_CH2N_AF);
		 GPIO_PinAFConfig(F072B_PWM_CHN_P0RT ,  F072B_PWM_CH3N_SOURCE, F072B_PWM_CH3N_AF);

}
/*******************************************************************************
* 函数名称: Timer1_Init();
* 功能描述: 定时器1初始化
* 输入参数: void
* 返回参数: 无
********************************************************************************/
void Timer1_Init(unsigned short int arr,unsigned short int psc)
{    
        
        TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;	
	      TIM_OCInitTypeDef  TIM_OCInitStructure;
	      TIM_BDTRInitTypeDef TIM_BDTRInitStructure;
	      
	      PWM_GPIO_Init(); //端口初始化
	      RCC_APB2PeriphClockCmd(F072B_TIMER_CLK , ENABLE);   // 打开定时器1的时钟
	      //TIM_DeInit(F072B_TIMER_PWM); 

       
	      //设置定时器相关参数
	      TIM_TimeBaseInitStructure.TIM_Prescaler = psc;                    // 48MHZ
	      TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; // 向上计数模式
	      TIM_TimeBaseInitStructure.TIM_Period = arr ;         // 自动重装载计数器周期的值
	      TIM_TimeBaseInitStructure.TIM_ClockDivision = 0;                // 时钟分频系数--不分频
        TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	      TIM_TimeBaseInit(F072B_TIMER_PWM, &TIM_TimeBaseInitStructure); 
	
        //PWM模式设置-CH1
				TIM_OCInitStructure.TIM_OCMode      = TIM_OCMode_PWM1;          //PWM模式
        TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;  //输出使能
				TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;//互补端使能输出
				TIM_OCInitStructure.TIM_Pulse =240 ; //设置占空比
        TIM_OCInitStructure.TIM_OCPolarity   =  TIM_OCPolarity_Low;	//设置输出极性
				TIM_OCInitStructure.TIM_OCNPolarity  =  TIM_OCPolarity_High;  //设置输出端互补极性
				TIM_OCInitStructure.TIM_OCIdleState  =  TIM_OCIdleState_Set;   //死区后输出状态
        TIM_OCInitStructure.TIM_OCNIdleState =  TIM_OCIdleState_Reset;  //死区后互补端输出状态
        TIM_OC1Init(F072B_TIMER_PWM, &TIM_OCInitStructure);
				
				TIM_OC1PreloadConfig(F072B_TIMER_PWM, TIM_OCPreload_Enable);
				
        							
				//TIM_ARRPreloadConfig(F072B_TIMER_PWM, ENABLE);
	      TIM_Cmd(F072B_TIMER_PWM, ENABLE);
        TIM_CtrlPWMOutputs(F072B_TIMER_PWM, ENABLE);		
        
}     
/*******************************************************************************
* 函数名称: Timer3_Pwm_Adj();
* 功能描述: 频率可调
* 输入参数: void
* 返回参数: 无
********************************************************************************/
void Timer1_Pwm_Adj(void)
{     
	        
} 

/*******************************************************************************
* 函数名称: Timer1_Duty_Adj();
* 功能描述: 占空比可调
* 输入参数: void
* 返回参数: 无
********************************************************************************/
void Timer1_Duty_Adj(void)
{     
	  
	           
} 




