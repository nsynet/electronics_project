#include "timer.h"

TIM_HandleTypeDef TIM7_Handler;      //��ʱ����� 
extern vu16 USART3_RX_STA;

//��ʱ��7��ʼ��
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
void TIM7_Int_Init(u16 arr,u16 psc)
{
		TIM7_Handler.Instance=TIM7;                          //ͨ�ö�ʱ��3
    TIM7_Handler.Init.Prescaler=psc;                     //��Ƶϵ��
    TIM7_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    //���ϼ�����
    TIM7_Handler.Init.Period=arr;                        //�Զ�װ��ֵ
    TIM7_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;//ʱ�ӷ�Ƶ����
    HAL_TIM_Base_Init(&TIM7_Handler);
    
    HAL_TIM_Base_Start_IT(&TIM7_Handler); //ʹ�ܶ�ʱ��3�Ͷ�ʱ��3�����жϣ�TIM_IT_UPDATE									 
}
//��ʱ���ײ�����������ʱ�ӣ������ж����ȼ�
//�˺����ᱻHAL_TIM_Base_Init()��������
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
  if(htim->Instance==TIM7)
	{
		__HAL_RCC_TIM7_CLK_ENABLE();            //ʹ��TIM7ʱ��
		HAL_NVIC_SetPriority(TIM7_IRQn,1,3);    //�����ж����ȼ�����ռ���ȼ�0�������ȼ�1
		HAL_NVIC_EnableIRQ(TIM7_IRQn);          //����ITM7�ж�   
	}
}
//��ʱ��7�жϷ������		    
void TIM7_IRQHandler(void)
{
	ESP8266_RX_STA|=1<<15;	//��ǽ������
	ESP8266_RX_BUF[ESP8266_RX_STA&0X7FFF]=0;//��ӽ�����
	if((ESP8266_STA&7) == 4)  //�������������
		{
			ESP8266_STA&=~7;
			ESP8266_STA|=5;
		}
	__HAL_TIM_CLEAR_FLAG(&TIM7_Handler,TIM_EventSource_Update );       //���TIM7�����жϱ�־  
	TIM7->CR1&=~(1<<0);     			//�رն�ʱ��7    
} 

