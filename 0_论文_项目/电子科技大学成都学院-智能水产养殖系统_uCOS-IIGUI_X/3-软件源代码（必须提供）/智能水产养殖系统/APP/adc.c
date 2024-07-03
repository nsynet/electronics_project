#include "stm32f10x.h"
#include "adc.h"
#include "demo.h"	 
void adc_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef  ADC_InitStructure;
	
	/* Enable  clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC  , ENABLE);
	
	/* Configure Ports */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1|RCC_APB2Periph_AFIO, ENABLE);
	
	/* ADC1 Configuration ------------------------------------------------------*/
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//ѡ��ADCģʽ���˴�Ϊ����ģʽ
	ADC_InitStructure.ADC_ScanConvMode =DISABLE;		//��ͨ��ʹ�ܣ��˴�Ϊ��ͨ���ɼ�
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE ;	 //����ת��ʹ�ܡ��˴�����ת����ת����һ�����ݺ��ֹͣ
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; //ѡ���ⲿ����ģʽ
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//���ݶ���ģʽ
	ADC_InitStructure.ADC_NbrOfChannel = 1; //ͨ������
	ADC_Init(ADC1, &ADC_InitStructure);

}

#define V_REF 3300
void adc_read(void)
{
  int i;
   for(i=0; i<4; i++)
   {
        switch (i)
        {
            case 0: ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_239Cycles5); break;
            case 1: ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 1, ADC_SampleTime_239Cycles5); break;
            case 2: ADC_RegularChannelConfig(ADC1, ADC_Channel_12, 1, ADC_SampleTime_239Cycles5); break;
            case 3: ADC_RegularChannelConfig(ADC1, ADC_Channel_13, 1, ADC_SampleTime_239Cycles5); break;
        }
		/* Enable ADC1 */
		ADC_Cmd(ADC1, ENABLE);
		
		/* Enable ADC1 reset calibaration register */   
		ADC_ResetCalibration(ADC1);
		/* Check the end of ADC1 reset calibration register */
		while(ADC_GetResetCalibrationStatus(ADC1));
		
		/* Start ADC1 calibaration */
		ADC_StartCalibration(ADC1);
		/* Check the end of ADC1 calibration */
		while(ADC_GetCalibrationStatus(ADC1));

		/* Start ADC1 Software Conversion */ 
		ADC_SoftwareStartConvCmd(ADC1, ENABLE); //ʹ��ADC�����������
		Delay(10000); //3����4ѡ��ͬͨ���ɼ����
		AD_value[i]= ADC_GetConversionValue(ADC1);	//�������һ��ADCx�������ת�����
		AD_value[i]=AD_value[i]* V_REF /(0xfff);				 
	}
}
