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
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_5;						//PB5����ΪTIM3��ͨ��2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  
  
	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3 , ENABLE);			 //TIM3�ֲ����ù��ܿ���	 ��TIM3�ľֲ����ÿ���ʱ��PB5�ᱻ����ΪTIM3_CH2
  				
  


	/* Time Base configuration */
	/*-------------------------------------------------------------------
	TIM3CLK=72MHz  Ԥ��Ƶϵ��Prescaler=2 ������Ƶ ��ʱ��ʱ��Ϊ24MHz
	���ݹ�ʽ ͨ�����ռ�ձ�=TIM3_CCR2/(TIM_Period+1),���Եõ�TIM_Pulse�ļ���ֵ	 
	����/�ȽϼĴ���2 TIM3_CCR2= CCR2_Val 
	
	-------------------------------------------------------------------*/
	TIM3_TimeBaseStructure.TIM_Prescaler = 2;						    //Ԥ��Ƶ��TIM3_PSC=63	 
	TIM3_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;		//���������ϼ���ģʽ TIM3_CR1[4]=0
	TIM3_TimeBaseStructure.TIM_Period =48000-1;				           //�Զ���װ�ؼĴ���TIM3_APR  ȷ��Ƶ��Ϊ1KHz 		     
	TIM3_TimeBaseStructure.TIM_ClockDivision = 0x0;					//ʱ�ӷ�Ƶ���� TIM3_CR1[9:8]=00
	TIM3_TimeBaseStructure.TIM_RepetitionCounter = 0x0;
	
	TIM_TimeBaseInit(TIM3,&TIM3_TimeBaseStructure);					//дTIM3���Ĵ�������
	
	TIM3_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; 					  //PWMģʽ2 TIM3_CCMR1[14:12]=111 �����ϼ���ʱ��
																		  //һ��TIMx_CNT<TIMx_CCR1ʱͨ��1Ϊ��Ч��ƽ������Ϊ��Ч��ƽ
	TIM3_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 		  //����/����2�������  OC2�ź��������Ӧ���������PB5
	TIM3_OCInitStructure.TIM_Pulse = 24000-1; 							  //ȷ��ռ�ձȣ����ֵ��������Ч��ƽ��ʱ�䡣
	TIM3_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; 			  //�������  �͵�ƽ��Ч TIM3_CCER[5]=1;
 
	//ȷ��ռ�ձȣ����ֵ��������Ч��ƽ��ʱ�䡣

	TIM3_OCInitStructure.TIM_Pulse = 24000-1; 
	TIM_OC2Init(TIM3, &TIM3_OCInitStructure);
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);

  	TIM3_OCInitStructure.TIM_Pulse= 24000-1;//�����˴�װ�벶��Ƚ���������ֵ
	TIM_OC3Init(TIM3,&TIM3_OCInitStructure);
	TIM_OC3PreloadConfig(TIM3,TIM_OCPreload_Enable);
  
	TIM3_OCInitStructure.TIM_Pulse= 24000-1;//�����˴�װ�벶��Ƚ���������ֵ
	TIM_OC4Init(TIM3,&TIM3_OCInitStructure);
	TIM_OC4PreloadConfig(TIM3,TIM_OCPreload_Enable);
  
	/* TIM1 counter enable */
	TIM_Cmd(TIM3,ENABLE);											//������ʱ��3 TIM3_CR1[0]=1;  
  
 
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
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;						//PB5����ΪTIM3��ͨ��2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
  
	
	//GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3 , ENABLE);			 //TIM3�ֲ����ù��ܿ���	 ��TIM3�ľֲ����ÿ���ʱ��PB5�ᱻ����ΪTIM3_CH2
	
	
	
	
	/* Time Base configuration */
	/*-------------------------------------------------------------------
	TIM3CLK=72MHz  Ԥ��Ƶϵ��Prescaler=2 ������Ƶ ��ʱ��ʱ��Ϊ24MHz
	���ݹ�ʽ ͨ�����ռ�ձ�=TIM3_CCR2/(TIM_Period+1),���Եõ�TIM_Pulse�ļ���ֵ	 
	����/�ȽϼĴ���2 TIM3_CCR2= CCR2_Val 
	
	-------------------------------------------------------------------*/
	TIM4_TimeBaseStructure.TIM_Prescaler = 2;						    //Ԥ��Ƶ��TIM3_PSC=63	 
	TIM4_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;		//���������ϼ���ģʽ TIM3_CR1[4]=0
	TIM4_TimeBaseStructure.TIM_Period =48000-1;				           //�Զ���װ�ؼĴ���TIM3_APR  ȷ��Ƶ��Ϊ1KHz 		     
	TIM4_TimeBaseStructure.TIM_ClockDivision = 0x0;					//ʱ�ӷ�Ƶ���� TIM3_CR1[9:8]=00
	TIM4_TimeBaseStructure.TIM_RepetitionCounter = 0x0;	
	
	TIM_TimeBaseInit(TIM4,&TIM4_TimeBaseStructure);					//дTIM3���Ĵ�������
	
	TIM4_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; 					  //PWMģʽ2 TIM3_CCMR1[14:12]=111 �����ϼ���ʱ��
	//һ��TIMx_CNT<TIMx_CCR1ʱͨ��1Ϊ��Ч��ƽ������Ϊ��Ч��ƽ
	TIM4_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 		  //����/����2�������  OC2�ź��������Ӧ���������PB5
	TIM4_OCInitStructure.TIM_Pulse = 24000-1; 							  //ȷ��ռ�ձȣ����ֵ��������Ч��ƽ��ʱ�䡣
	TIM4_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; 			  //�������  �͵�ƽ��Ч TIM3_CCER[5]=1;
	
	//ȷ��ռ�ձȣ����ֵ��������Ч��ƽ��ʱ�䡣
	
	TIM4_OCInitStructure.TIM_Pulse = 24000-1; 
	TIM_OC1Init(TIM4, &TIM4_OCInitStructure);
	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);

	TIM_Cmd(TIM4,ENABLE);											//������ʱ��3 TIM3_CR1[0]=1;  
}





