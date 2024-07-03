#include "usart.h"

#if SYSTEM_SUPPORT_OS
#include "includes.h"					//os ʹ��	  
#endif

//���ڷ��ͻ����� 	
__align(8) u8 ESP8266_TX_BUF[ESP8266_MAX_SEND_LEN]; 	//���ͻ���,���ESP8266_MAX_SEND_LEN�ֽ�  	  
//���ڽ��ջ����� 	
u8 ESP8266_RX_BUF[ESP8266_MAX_RECV_LEN]; 				//���ջ���,���ESP8266_MAX_RECV_LEN���ֽ�.


//ͨ���жϽ�������2���ַ�֮���ʱ������100ms�������ǲ���һ������������.
//���2���ַ����ռ������100ms,����Ϊ����1����������.Ҳ���ǳ���100msû�н��յ�
//�κ�����,���ʾ�˴ν������.
//���յ�������״̬
//[15]:0,û�н��յ�����;1,���յ���һ������.
//[14:0]:���յ������ݳ���
u16 ESP8266_RX_STA=0; 
UART_HandleTypeDef UART1_Handler; //UART���
//u8 USART2_RX_BUF[1];

u8 ESP8266_REC_BUF;

//��ʼ��IO ����1 
//bound:������
void uart_init(u32 bound)
{	
	UART1_Handler.Instance = USART1;
  UART1_Handler.Init.BaudRate = bound;
  UART1_Handler.Init.WordLength = UART_WORDLENGTH_8B;
  UART1_Handler.Init.StopBits = UART_STOPBITS_1;
  UART1_Handler.Init.Parity = UART_PARITY_NONE;
  UART1_Handler.Init.Mode = UART_MODE_TX_RX;
  UART1_Handler.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  UART1_Handler.Init.OverSampling = UART_OVERSAMPLING_16;
  UART1_Handler.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  UART1_Handler.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_RXOVERRUNDISABLE_INIT|UART_ADVFEATURE_DMADISABLEONERROR_INIT;
  UART1_Handler.AdvancedInit.OverrunDisable = UART_ADVFEATURE_OVERRUN_DISABLE;
  UART1_Handler.AdvancedInit.DMADisableonRxError = UART_ADVFEATURE_DMA_DISABLEONRXERROR;
	HAL_UART_Init(&UART1_Handler);					    //HAL_UART_Init()��ʹ��UART1
//	HAL_UART_Receive_IT(&UART1_Handler,ESP8266_RX_BUF, 10);//�ú����Ὺ�������жϣ���־λUART_IT_RXNE���������ý��ջ����Լ����ջ���������������
	
		TIM7_Int_Init(600-1,8000-1);		//60ms�ж�
		ESP8266_RX_STA=0;		//����
		TIM7->CR1&=~(1<<0);        //�رն�ʱ��7
}

//����1,printf ����
//ȷ��һ�η������ݲ�����ESP8266_MAX_SEND_LEN�ֽ�
void esp8266_printf(char* fmt,...)  
{  
	u16 i,j; 
	va_list ap; 
	va_start(ap,fmt);
	vsprintf((char*)ESP8266_TX_BUF,fmt,ap);
	va_end(ap);
	i=strlen((const char*)ESP8266_TX_BUF);		//�˴η������ݵĳ���
	for(j=0;j<i;j++)							//ѭ����������
	{
		while((USART1->ISR&0X40)==0);			//ѭ������,ֱ���������   
		USART1->TDR=ESP8266_TX_BUF[j];  
	} 
}

//����1�����ַ���
void WIFI_SendData(u8* data)
{
	u8 i;
	u16 num;
	num = strlen((const char*)data);
	for(i=0;i<num;i++)
	{
		while((USART1->ISR&0X40)==0);//�ȴ����ͽ���
		USART1->TDR=data[i];
	}
}

//����1�жϺ���
void USART1_IRQHandler(void)
{
	u8 res;
	static u8 i=0;
#if SYSTEM_SUPPORT_OS	 	//ʹ��OS
	OSIntEnter();    
#endif	
	if(__HAL_UART_GET_FLAG(&UART1_Handler,UART_FLAG_RXNE)!=RESET)//���յ�����
	{
		__HAL_UART_CLEAR_FLAG(&UART1_Handler,UART_FLAG_RXNE);
//		HAL_UART_Receive(&UART1_Handler,&res,1,1000);
		res=USART1->RDR; 
//		printf("%c %d\r\n",res,res);	
		if((ESP8266_STA&7) < 5)
		{
			switch (ESP8266_STA&7)
			{
				case 0:if(res=='+')
								{
									ESP8266_STA|=1;
									i=0;
								}
								break;
				case 1:if(res=='I')
								{
									ESP8266_STA&=~7;
									ESP8266_STA|=2;
									memset(ESP8266_Cmd,0,50);
									ESP8266_RX_STA=0;
//								USART1->TDR='2';
								}
								else ESP8266_STA&=~7;
								break;
				case 2:if(res>='0' && res<='3')
								{
									ESP8266_STA&=~7;
									ESP8266_STA|=3;
									ESP8266_STA|=(res-0x30)<<4;
//								USART1->TDR='3';
								}
								else if(res=='4')
								{
									ESP8266_STA&=~7;
									ESP8266_STA|=11;
								}
								break;
				case 3:if(res==':')
								{
									ESP8266_STA&=~7;
									ESP8266_STA|=4;
								USART1->TDR='4';
								}
								break;
				case 4:if(i<49)
								{
									ESP8266_Cmd[i++]=res;
								}
								else
								{
									ESP8266_STA&=~7;
									ESP8266_STA|=5;
								}
								break;
			}
		}
		
		if((ESP8266_RX_STA&(1<<15))==0)//�������һ������,��û�б�����,���ٽ�����������
		{ 
			if(ESP8266_RX_STA<ESP8266_MAX_RECV_LEN)	//�����Խ�������
			{
//				__HAL_TIM_SetCounter(&TIM7_Handler,0);	
				TIM7->CNT=0;         				//���������	
				if(ESP8266_RX_STA==0) 				//ʹ�ܶ�ʱ��7���ж� 
				{
//					__HAL_RCC_TIM7_CLK_ENABLE();            //ʹ��TIM7ʱ��
					TIM7->CR1|=1<<0;     			//ʹ�ܶ�ʱ��7
				}
				ESP8266_RX_BUF[ESP8266_RX_STA++]=res;	//��¼���յ���ֵ	 
			}else 
			{
				ESP8266_RX_STA|=1<<15;				//ǿ�Ʊ�ǽ������
			} 
		}
	}	
#if SYSTEM_SUPPORT_OS	 	//ʹ��OS
	OSIntExit();  											 
#endif	
}   




