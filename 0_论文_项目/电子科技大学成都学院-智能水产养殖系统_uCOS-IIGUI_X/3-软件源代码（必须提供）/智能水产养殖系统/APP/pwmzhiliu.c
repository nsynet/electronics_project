#include "pwmzhiliu.h"
#include "stm32f10x.h"
#include "time.h"
void function(int c,int d)
{
	if(c>0)
	{
		TIM_SetCompare3(TIM3,0);
	}
	else
	{
		TIM_SetCompare3(TIM3,-c);
	}
	if(d>0)
	{
		TIM_SetCompare4(TIM3,0);
	}
	else
	{
		TIM_SetCompare4(TIM3,-d);
	}
}
void PWM1_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM3_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM3_OCInitStructure;
	
	
	
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO , ENABLE); 	   
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
  

  //CCR2_Val=12000;

  /* GPIOA Configuration: Channel 1 Output */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_5;						//PB5复用为TIM3的通道2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  
  
	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3 , ENABLE);			 //TIM3局部复用功能开启	 在TIM3的局部复用开启时，PB5会被复用为TIM3_CH2
  				
  


	/* Time Base configuration */
	/*-------------------------------------------------------------------
	TIM3CLK=72MHz  预分频系数Prescaler=2 经过分频 定时器时钟为24MHz
	根据公式 通道输出占空比=TIM3_CCR2/(TIM_Period+1),可以得到TIM_Pulse的计数值	 
	捕获/比较寄存器2 TIM3_CCR2= CCR2_Val 
	
	-------------------------------------------------------------------*/
	TIM3_TimeBaseStructure.TIM_Prescaler = 2;						    //预分频器TIM3_PSC=63	 
	TIM3_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;		//计数器向上计数模式 TIM3_CR1[4]=0
	TIM3_TimeBaseStructure.TIM_Period =48000-1;				           //自动重装载寄存器TIM3_APR  确定频率为1KHz 		     
	TIM3_TimeBaseStructure.TIM_ClockDivision = 0x0;					//时钟分频因子 TIM3_CR1[9:8]=00
	TIM3_TimeBaseStructure.TIM_RepetitionCounter = 0x0;
	
	TIM_TimeBaseInit(TIM3,&TIM3_TimeBaseStructure);					//写TIM3各寄存器参数
	
	TIM3_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; 					  //PWM模式2 TIM3_CCMR1[14:12]=111 在向上计数时，
																		  //一旦TIMx_CNT<TIMx_CCR1时通道1为无效电平，否则为有效电平
	TIM3_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 		  //输入/捕获2输出允许  OC2信号输出到对应的输出引脚PB5
	TIM3_OCInitStructure.TIM_Pulse = 24000-1; 							  //确定占空比，这个值决定了有效电平的时间。
	TIM3_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; 			  //输出极性  低电平有效 TIM3_CCER[5]=1;
 
	//确定占空比，这个值决定了有效电平的时间。

	TIM3_OCInitStructure.TIM_Pulse = 24000-1; 
	TIM_OC2Init(TIM3, &TIM3_OCInitStructure);
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);

  	TIM3_OCInitStructure.TIM_Pulse= 24000-1;//设置了待装入捕获比较器的脉冲值
	TIM_OC3Init(TIM3,&TIM3_OCInitStructure);
	TIM_OC3PreloadConfig(TIM3,TIM_OCPreload_Enable);
  
	TIM3_OCInitStructure.TIM_Pulse= 24000-1;//设置了待装入捕获比较器的脉冲值
	TIM_OC4Init(TIM3,&TIM3_OCInitStructure);
	TIM_OC4PreloadConfig(TIM3,TIM_OCPreload_Enable);
  
	/* TIM1 counter enable */
	TIM_Cmd(TIM3,ENABLE);											//启动定时器3 TIM3_CR1[0]=1;  
  
 
}
void PWM2_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM4_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM4_OCInitStructure;
	
	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE); 	   
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	
	
	//CCR2_Val=12000;
	
	/* GPIOA Configuration: Channel 1 Output */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;						//PB5复用为TIM3的通道2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
  
	
	//GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3 , ENABLE);			 //TIM3局部复用功能开启	 在TIM3的局部复用开启时，PB5会被复用为TIM3_CH2
	
	
	
	
	/* Time Base configuration */
	/*-------------------------------------------------------------------
	TIM3CLK=72MHz  预分频系数Prescaler=2 经过分频 定时器时钟为24MHz
	根据公式 通道输出占空比=TIM3_CCR2/(TIM_Period+1),可以得到TIM_Pulse的计数值	 
	捕获/比较寄存器2 TIM3_CCR2= CCR2_Val 
	
	-------------------------------------------------------------------*/
	TIM4_TimeBaseStructure.TIM_Prescaler = 2;						    //预分频器TIM3_PSC=63	 
	TIM4_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;		//计数器向上计数模式 TIM3_CR1[4]=0
	TIM4_TimeBaseStructure.TIM_Period =48000-1;				           //自动重装载寄存器TIM3_APR  确定频率为1KHz 		     
	TIM4_TimeBaseStructure.TIM_ClockDivision = 0x0;					//时钟分频因子 TIM3_CR1[9:8]=00
	TIM4_TimeBaseStructure.TIM_RepetitionCounter = 0x0;	
	
	TIM_TimeBaseInit(TIM4,&TIM4_TimeBaseStructure);					//写TIM3各寄存器参数
	
	TIM4_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; 					  //PWM模式2 TIM3_CCMR1[14:12]=111 在向上计数时，
	//一旦TIMx_CNT<TIMx_CCR1时通道1为无效电平，否则为有效电平
	TIM4_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 		  //输入/捕获2输出允许  OC2信号输出到对应的输出引脚PB5
	TIM4_OCInitStructure.TIM_Pulse = 24000-1; 							  //确定占空比，这个值决定了有效电平的时间。
	TIM4_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; 			  //输出极性  低电平有效 TIM3_CCER[5]=1;
	
	//确定占空比，这个值决定了有效电平的时间。
	
	TIM4_OCInitStructure.TIM_Pulse = 24000-1; 
	TIM_OC1Init(TIM4, &TIM4_OCInitStructure);
	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);

	TIM_Cmd(TIM4,ENABLE);											//启动定时器3 TIM3_CR1[0]=1;  
}





