#include "adc.h"

u8 MQ2_Air_Quality,MQ135_Air_Quality;
ADC_HandleTypeDef ADC1_Handler;//ADC���

//��ʼ��ADC
//ch: ADC_channels 
//ͨ��ֵ 0~16ȡֵ��ΧΪ��ADC_CHANNEL_0~ADC_CHANNEL_16
void MY_ADC_Init(void)
{ 
    ADC1_Handler.Instance=ADC1;
    ADC1_Handler.Init.ClockPrescaler=ADC_CLOCK_ASYNC_DIV1;   
    ADC1_Handler.Init.Resolution=ADC_RESOLUTION_12B;             //12λģʽ
    ADC1_Handler.Init.DataAlign=ADC_DATAALIGN_RIGHT;             //�Ҷ���
    ADC1_Handler.Init.ScanConvMode=ADC_SCAN_DISABLE;                      //��ɨ��ģʽ
    ADC1_Handler.Init.EOCSelection=ADC_EOC_SINGLE_CONV;                      //�ر�EOC�ж�
    ADC1_Handler.Init.ContinuousConvMode=DISABLE;                //�ر�����ת��
		ADC1_Handler.Init.LowPowerAutoWait = DISABLE;
    ADC1_Handler.Init.NbrOfConversion=1;                         //1��ת���ڹ��������� Ҳ����ֻת����������1 
    ADC1_Handler.Init.DiscontinuousConvMode=DISABLE;             //��ֹ����������ģʽ
    ADC1_Handler.Init.NbrOfDiscConversion=0;                     //����������ͨ����Ϊ0
    ADC1_Handler.Init.ExternalTrigConv=ADC_SOFTWARE_START;       //�������
    ADC1_Handler.Init.ExternalTrigConvEdge=ADC_EXTERNALTRIGCONVEDGE_NONE;//ʹ���������
    ADC1_Handler.Init.DMAContinuousRequests=DISABLE;             //�ر�DMA����
		ADC1_Handler.Init.Overrun = ADC_OVR_DATA_PRESERVED;
		ADC1_Handler.Init.OversamplingMode = DISABLE;
    HAL_ADC_Init(&ADC1_Handler);                                 //��ʼ�� 
}

//ADC�ײ��������������ã�ʱ��ʹ��
//�˺����ᱻHAL_ADC_Init()����
//hadc:ADC���
void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
{
  GPIO_InitTypeDef GPIO_Initure;
	if(hadc->Instance==ADC1)
	{
    __HAL_RCC_ADC_CLK_ENABLE();            //ʹ��ADC1ʱ��
    __HAL_RCC_GPIOB_CLK_ENABLE();			//����GPIOBʱ��
	
    GPIO_Initure.Pin=GPIO_PIN_0|GPIO_PIN_1;            //PB0
    GPIO_Initure.Mode=GPIO_MODE_ANALOG_ADC_CONTROL;     //ģ��
    GPIO_Initure.Pull=GPIO_NOPULL;          //����������
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);
	}
}

//���ADCֵ
//ch: ͨ��ֵ 0~16��ȡֵ��ΧΪ��ADC_CHANNEL_0~ADC_CHANNEL_16
//����ֵ:ת�����
u16 Get_Adc(u32 ch)   
{
    ADC_ChannelConfTypeDef ADC1_ChanConf;
    
    ADC1_ChanConf.Channel=ch;                                   //ͨ��
    ADC1_ChanConf.Rank=1;                                       //��1�����У�����1
    ADC1_ChanConf.SamplingTime=ADC_SAMPLETIME_640CYCLES_5;        //����ʱ��
    ADC1_ChanConf.Offset=0;                 
    HAL_ADC_ConfigChannel(&ADC1_Handler,&ADC1_ChanConf);        //ͨ������
	
    HAL_ADC_Start(&ADC1_Handler);                               //����ADC
	
    HAL_ADC_PollForConversion(&ADC1_Handler,10);                //��ѯת��
 
	return (u16)HAL_ADC_GetValue(&ADC1_Handler);	        //�������һ��ADC1�������ת�����
}
//��ȡָ��ͨ����ת��ֵ��ȡtimes��,Ȼ��ƽ�� 
//times:��ȡ����
//����ֵ:ͨ��ch��times��ת�����ƽ��ֵ
u16 Get_Adc_Average(u32 ch,u8 times)
{
	u32 temp_val=0;
	u8 t;
	for(t=0;t<times;t++)
	{
		temp_val+=Get_Adc(ch);
//		delay_ms(5);
	}
	return temp_val/times;
} 

//�ɼ�MQ2��AD����AD<0.3Vʱ�жϿ�������Ϊ����A������0.3V-1.0V֮���жϼ���Ϊ��B����1.0V�����ж�Ϊ'C'
u8 MQ2_Collect(u8 times)
{
	u16 adcx;
	adcx=Get_Adc_Average(ADC_CHANNEL_15,times)*3.29/4030;
	if(adcx<0.3) return 'A';
	else if(adcx<1.0) return 'B';
	else return 'C';
}

//�ɼ�MQ135��AD����AD<0.3Vʱ�жϿ�������Ϊ����A������0.3V-1.0V֮���жϼ���Ϊ��B����1.0V�����ж�Ϊ'C'
u8 MQ135_Collect(u8 times)
{
	u16 adcx;
	adcx=Get_Adc_Average(ADC_CHANNEL_16,times)*3.29/4030;
	if(adcx<0.3) return 'A';
	else if(adcx<1.0) return 'B';
	else return 'C';
}



