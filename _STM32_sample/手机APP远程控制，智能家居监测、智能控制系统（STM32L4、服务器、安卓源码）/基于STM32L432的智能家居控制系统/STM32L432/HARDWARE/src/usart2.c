#include "usart2.h"
////////////////////////////////////////////////////////////////////////////////// 	 
//���ʹ��os,����������ͷ�ļ�����.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//os ʹ��	  
#endif
  
//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
//#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)	
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{ 	
	while((USART2->ISR&0X40)==0);//ѭ������,ֱ���������   
	USART2->TDR = (u8) ch;      
	return ch;
}
#endif 

#if EN_USART2_RX   //���ʹ���˽���
//����1�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���   	
u8 USART_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
#endif	
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
//u16 USART_RX_STA=0;       //����״̬���	


//u8 aRxBuffer[RXBUFFERSIZE];//HAL��ʹ�õĴ��ڽ��ջ���
UART_HandleTypeDef UART2_Handler; //UART���

//��ʼ��IO ����2 
//bound:������
void uart2_init(u32 bound)
{	
	//UART ��ʼ������
	UART2_Handler.Instance=USART2;					    //USART1
	UART2_Handler.Init.BaudRate = bound;
  UART2_Handler.Init.WordLength = UART_WORDLENGTH_8B;
  UART2_Handler.Init.StopBits = UART_STOPBITS_1;
  UART2_Handler.Init.Parity = UART_PARITY_NONE;
  UART2_Handler.Init.Mode = UART_MODE_TX_RX;
  UART2_Handler.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  UART2_Handler.Init.OverSampling = UART_OVERSAMPLING_16;
  UART2_Handler.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  UART2_Handler.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_RXOVERRUNDISABLE_INIT|UART_ADVFEATURE_DMADISABLEONERROR_INIT;
  UART2_Handler.AdvancedInit.OverrunDisable = UART_ADVFEATURE_OVERRUN_DISABLE;
  UART2_Handler.AdvancedInit.DMADisableonRxError = UART_ADVFEATURE_DMA_DISABLEONRXERROR;
	HAL_UART_Init(&UART2_Handler);					    //HAL_UART_Init()��ʹ��UART1
	
//	HAL_UART_Receive_IT(&UART2_Handler, (u8 *)aRxBuffer, RXBUFFERSIZE);//�ú����Ὺ�������жϣ���־λUART_IT_RXNE���������ý��ջ����Լ����ջ���������������
  
}

//UART�ײ��ʼ����ʱ��ʹ�ܣ��������ã��ж�����
//�˺����ᱻHAL_UART_Init()����
//huart:���ھ��
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
    //GPIO�˿�����
	if(huart==(&UART1_Handler))
	{
    //GPIO�˿�����
		GPIO_InitTypeDef GPIO_Initure;
	
		__HAL_RCC_GPIOA_CLK_ENABLE();			//ʹ��GPIOAʱ��
		__HAL_RCC_USART1_CLK_ENABLE();			//ʹ��USART1ʱ��
	
		GPIO_Initure.Pin = GPIO_PIN_9|GPIO_PIN_10;
    GPIO_Initure.Mode = GPIO_MODE_AF_PP;
    GPIO_Initure.Pull = GPIO_PULLUP;
    GPIO_Initure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_Initure.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(GPIOA, &GPIO_Initure);
	
//    __HAL_UART_DISABLE_IT(huart,UART_IT_TC);
#if ESP8266_RX_EN
		__HAL_UART_ENABLE_IT(huart,UART_IT_RXNE);		//���������ж�
    HAL_NVIC_SetPriority(USART1_IRQn, 2, 2);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
#endif	
	}
	if(huart==(&UART2_Handler))
	{
		  //GPIO�˿�����
		GPIO_InitTypeDef GPIO_Initure;
	
		__HAL_RCC_GPIOA_CLK_ENABLE();			//ʹ��GPIOAʱ��
		__HAL_RCC_USART2_CLK_ENABLE();			//ʹ��USART2ʱ��
	
		GPIO_Initure.Pin = GPIO_PIN_2;
    GPIO_Initure.Mode = GPIO_MODE_AF_PP;
    GPIO_Initure.Pull = GPIO_PULLUP;
    GPIO_Initure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_Initure.Alternate = GPIO_AF7_USART2;
    HAL_GPIO_Init(GPIOA, &GPIO_Initure);
		
//		GPIO_Initure.Pin = GPIO_PIN_15;
//		GPIO_Initure.Alternate = GPIO_AF3_USART2;
		GPIO_Initure.Pin = GPIO_PIN_3;
		GPIO_Initure.Alternate = GPIO_AF7_USART2;
    HAL_GPIO_Init(GPIOA, &GPIO_Initure);
	
//		__HAL_UART_DISABLE_IT(huart,UART_IT_TC);
#if EN_USART2_RX 
		__HAL_UART_ENABLE_IT(huart,UART_IT_RXNE);		//���������ж�
		HAL_NVIC_SetPriority(USART2_IRQn,2,3);			//��ռ���ȼ�2�������ȼ�3
		HAL_NVIC_EnableIRQ(USART2_IRQn);				//ʹ��USART2�ж�
#endif
	}
}

//GP2Y1050ת������
u8 GP2Y1050_Times;
//PM2.5ʵʱŨ��
float GP2Y1050_Quality;
//GP2Y1050���ջ���
u8 GP2Y1050_buf[5];
//GP2Y1050����״̬
//bit7 ���յ�����ͷ0XAA��˵�����Կ�ʼ��������
//bit6~0 ����������Ŀ
u8 GP2Y1050_STA=0;

//����2�жϷ������
void USART2_IRQHandler(void)                	
{ 
	u8 Res;
#if SYSTEM_SUPPORT_OS	 	//ʹ��OS
	OSIntEnter();    
#endif
	#if EN_USART2_RX 
	if((__HAL_UART_GET_FLAG(&UART2_Handler,UART_FLAG_RXNE)!=RESET))  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{
//    HAL_UART_Receive(&UART2_Handler,&Res,1,1000); 
		Res=USART2->RDR;
		
		//GP2Y1050���ճ���
		if(GP2Y1050_STA&(1<<7) && GP2Y1050_Times<GP2Y1050_COLLECT_TIMES)
		{
			if((GP2Y1050_STA&0x7f)<5)
			{
				GP2Y1050_buf[GP2Y1050_STA&0x7f]=Res;
				GP2Y1050_STA++;
			}
			else if(Res==0xff)
			{
				if(GP2Y1050_buf[4]==GP2Y1050_buf[0]+GP2Y1050_buf[1]+GP2Y1050_buf[2]+GP2Y1050_buf[3])  //����У��
				{
					GP2Y1050_Times++;
					GP2Y1050_Quality += (GP2Y1050_buf[0]<<8) + GP2Y1050_buf[1];
					if(GP2Y1050_Times==GP2Y1050_COLLECT_TIMES) //�ﵽ�ɼ�������ر��ж�
					{
						__HAL_UART_DISABLE_IT(&UART2_Handler,UART_IT_RXNE);		//�رս����ж�
						GP2Y1050_Quality = GP2Y1050_Quality/GP2Y1050_COLLECT_TIMES/1024.0*5;
						GP2Y1050_Quality = GP2Y1050_Quality*600;
					}
				}
				GP2Y1050_STA=0;
			}
			else
			{
				GP2Y1050_STA=0;
			}
		}
		else if(Res==0xaa && GP2Y1050_Times<GP2Y1050_COLLECT_TIMES)
		{
			GP2Y1050_STA|=1<<7;
		}
	 		 
	}
	#endif	
#if SYSTEM_SUPPORT_OS	 	//ʹ��OS
	OSIntExit();  											 
#endif
} 

 




