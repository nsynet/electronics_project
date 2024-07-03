#include "led.h"
	    
//LED��ʼ��
void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_Initure;
	__HAL_RCC_GPIOB_CLK_ENABLE();           //����GPIOBʱ��
	__HAL_RCC_GPIOA_CLK_ENABLE();

	GPIO_Initure.Pin=GPIO_PIN_3|GPIO_PIN_4; 
	GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //�������
	GPIO_Initure.Pull=GPIO_PULLUP;          //����
	GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //����
	HAL_GPIO_Init(GPIOB,&GPIO_Initure);

	GPIO_Initure.Pin=GPIO_PIN_8|GPIO_PIN_11; 
	GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //�������
	GPIO_Initure.Pull=GPIO_PULLUP;          //����
	GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //����
	HAL_GPIO_Init(GPIOA,&GPIO_Initure);	
}

//�ֶ�����״̬��1Ϊ��Ӧ�ĵ��������ֶ����ƣ�0Ϊ�������ֶ����ƣ��ֶ��������ȼ������Զ����ƣ�ֻ���ֶ����ƺ��Զ����ƶ��ر�ʱ����������ȫ�ر�
//������ֻ�õ������������ͳ������Զ����ֶ����ܣ��Ƶ��ֶ����ܣ�����λΪ����λ
//λ7���յ�  λ6������������  λ5������ λ4���� λ3���� 
u8 Manual_STA;
//�Զ�����״̬  1Ϊ��Ӧ�ĵ��������Զ����ƣ�0Ϊ���Զ�����
//λ7���յ�  λ6������������  λ5������ λ4���� λ3���� 
u8 AUTO_STA;
//PM2.5��������
u8 PM2_5_Quality;

//�������ƺ���
void Control()
{
	if(Manual_STA&(1<<6))
	{
		AIR_CLEAR_ON;
	}
	else if(AUTO_STA&(1<<6))
	{
		if(MQ2_Air_Quality>='B' || MQ135_Air_Quality>='B' || PM2_5_Quality>='B')
		{
			AIR_CLEAR_ON;
		}
		else
		{
			AIR_CLEAR_OFF;
		}
	}
	else
	{
		AIR_CLEAR_OFF;
	}
	
	if(Manual_STA&(1<<5))
	{
		EXHAUST_FAN_ON;
	}
	else if(AUTO_STA&(1<<5))
	{
		if(MQ2_Air_Quality>='B' || MQ135_Air_Quality>='B' || PM2_5_Quality>='B')
		{
			EXHAUST_FAN_ON;
		}
		else
		{
			EXHAUST_FAN_OFF;
		}
	}
	else
	{
		EXHAUST_FAN_OFF;
	}
	
	if(Manual_STA&(1<<3))
	{
		LIGHT_ON;
	}
	else
	{
		LIGHT_OFF;
	}
}

