#include  "uartGetPut.h"
#include "stm32f10x_usart.h"
#include "stm32f10x.h"
#include  <ctype.h>
#include "lcd.h"
#include "stdio.h"
#include "delay.h"


volatile char Sta1[10],Sta2[10],Sta3[10],Sta4[10] ;
volatile short status1,status2,status3,status4;

int k=0;



//  UART��ʼ��
void uartInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	USART_ClockInitTypeDef USART_ClockInitStructure;
	NVIC_InitTypeDef  NVIC_InitStructure;
	/*********GPIO*ʹ��RX/TX���ڵ�GPIO�˿�*********/
	//RX
	GPIO_InitStructure.GPIO_Pin = USART_GPIO_RX;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(USART_GPIO, &GPIO_InitStructure);
	//TX
	GPIO_InitStructure.GPIO_Pin = USART_GPIO_TX;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(USART_GPIO, &GPIO_InitStructure);
	/********* NVIC*ʹ��RX/TX���ڵ�GPIO�˿�*********/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	
	/*********USART**********/  //  ����UART�˿�
	USART_ClockInitStructure.USART_Clock = USART_Clock_Disable  ;	//ʱ�ӵ͵�ƽ�
	USART_ClockInitStructure.USART_CPOL =USART_CPOL_Low  ; //ʱ�ӵ͵�ƽ
	USART_ClockInitStructure.USART_CPHA = USART_CPHA_1Edge;	 //ʱ�ӵڶ������ؽ������ݲ���
	USART_ClockInitStructure.USART_LastBit = USART_LastBit_Disable;	//���һλ���ݵ�ʱ�����岻��SCLK ���
	
	USART_ClockInit(USART, &USART_ClockInitStructure);
	
	USART_InitStructure.USART_BaudRate = 9600;  //  �����ʣ�9600
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;   //  ����λ��8
	USART_InitStructure.USART_StopBits = USART_StopBits_1;	    //  ֹͣλ��1
	USART_InitStructure.USART_Parity = USART_Parity_No;			 //  У��λ����
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //����Ӳ��������ģʽ
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//ʹ�ܽ��պͷ���ģʽ
	USART_Init(USART, &USART_InitStructure);	  //  //USART ����
	USART_ITConfig(USART, USART_IT_RXNE, ENABLE);//ʹ��USART1�����ж�	
	//	USART_ITConfig(USART1, USART_IT_TXE, ENABLE);//ʹ��USART1�����ж�
	USART_Cmd(USART, ENABLE);
}


//  ͨ��UART����һ���ַ�
void uartPutc(u8 c)
{
   USART_SendData(USART, c);
}


//  ͨ��UART�����ַ���
void uartPuts(u8  *s)
{  
	while (*(s)!= '\0')
	{
	uartPutc(*(s));
	
	s++;
	}
//	 USART_SendData(USART1, );
}


//  ͨ��UART����һ���ַ�
char uartGetc(void)
{
    return(USART_ReceiveData(USART));
}

//  ͨ��UART�����ַ���
void USART_SendString(u8 *str)
{
	while(*str != 0)
	{
		USART_SendData(USART, *(str));
		str++;
		while(USART_GetFlagStatus(USART, USART_FLAG_TXE) == RESET);
	}  
}

//  ���ܣ�ͨ��UART�����ַ��������ԣ��˸�<Backspace>�޸ģ��س�<Enter>����
//  ������*s����������ݵĻ�������ֻ���տɴ�ӡ�ַ���ASCII��32��127��
//        size�ǻ�����*s���ܳ��ȣ�Ҫ��size >= 2������ĩβ'\0'��������sizeof()����ȡ��
//  ���أ����յ�����Ч�ַ���Ŀ
int uartGets(char *s, int size)
{
	char c;
	int n = 0;
	
	*s = '\0';
	if (size < 2)
	{
		return(0);
	}
	size--;
	
	for (;;)
	{
		c = uartGetc();                                     //  ����1���ַ�
		uartPutc(c);                                        //  ����������ַ�
		
		if (c == '\b')                                      //  ���˸�<Backspace>�޸�
		{
			if (n > 0)
			{
				*(--s) = '\0';
				n--;
				uartPuts(" \b");                            //  ��ʾ�ո���˸�<Backspace>
			}
		}
		
		if (c == '\r')                                      //  ���س�<Enter>����
		{
			uartPuts("\r\n");                               //  ��ʾ�س�����<CR><LF>
			break;
		}
		
		if (n < size)                                       //  ���С�ڳ�������
		{
			if (isprint(c))                                 //  ������յ����ǿɴ�ӡ�ַ�
			{
				*(s++) = c;                                 //  ������յ����ַ���������
				*s = '\0';
				n++;
			}
		}
	}
	
	return(n);                                              //  ���ؽ��յ�����Ч�ַ���Ŀ
}
//----
void put_int_in_num(short value)
{
	union{
	struct{
	unsigned char a;
	unsigned char b;
	}ch;
	short c; 
	}un;

	un.c=value;
	USART_SendString(&un.ch.a);
	USART_SendString(&un.ch.b);
}

//  ͨ��UART�����ж�
void USART1_IRQHandler(void)
{
	u8 data;
 //���ڽ����жϱ�־λ��λ
	if(USART_GetITStatus(USART,USART_IT_RXNE)!= RESET) 
	{
		data=uartGetc();
		if(data == 49)
		{
			put_int_in_num(200);
			put_int_in_num(500);
			put_int_in_num(200);
			put_int_in_num(24);
		}

	//��������жϱ�־
	USART_ClearITPendingBit(USART, USART_IT_RXNE);
	}
}

