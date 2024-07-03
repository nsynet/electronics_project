#include "usart1.h"
#include <stdio.h>
#include "delay.h"
#include <changePWM.h>
#include <Direction.h>

void uart_init(u32 bound)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	   //Usart1 NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���USART1
	
	
	       //IO������
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	 //USART1_TX   PB.6
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	//USART1_RX	  PB.7
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;  //��M3��һ��,M3�Ǹ�������
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_InitStructure);  	

  GPIO_PinAFConfig(GPIOB,GPIO_PinSource6,GPIO_AF_USART1);
  GPIO_PinAFConfig(GPIOB,GPIO_PinSource7,GPIO_AF_USART1);

	
           //USART ��ʼ������
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); 
	USART_InitStructure.USART_BaudRate = bound;//һ������Ϊ9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//8λ����
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//�շ�ģʽ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_Init(USART1, &USART_InitStructure);
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);  //�������ڽ����ж�
	USART_Cmd(USART1, ENABLE);                    //ʹ�ܴ��� 

}
//�����жϳ�������ڽ��յڶ�������֮ǰ���жϣ�������Щ����û�н���

void USART1_IRQHandler(void)                	//����1�жϷ������
{
	
	u8 Res;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
		{
		Res =USART_ReceiveData(USART1);//(USART1->DR);	//��ȡ���յ�������
		USART_SendData(USART1,Res);  
		}
		switch(Res)
		{
			case 'l':Change_PWM(2000,1000,2000,1000);
			printf(" turn left!");
			break;
			case 'r':Change_PWM(1000,2000,1000,2000);
			printf(" turn right!");
			break;
			case 's':Change_PWM(0,0,0,0);
			printf(" stop!");
			break;
			case 'f':Direction(1);//��ת
			Change_PWM(2000,2000,2000,2000);
			//printf(" start!");
			printf(" right direction");
			break;
			case 'b':Direction(0);//��ת
			Change_PWM(2000,2000,2000,2000);
			printf(" contray direction!");
			break;
		}
}

//�������´���,֧��printf����,������Ҫѡ��use MicroLIB
#if 1
#pragma import(__use_no_semihosting)   
//��׼����Ҫ��֧�ֺ���   
struct __FILE 
{ 
	int handle; 
}; 
FILE __stdout;      
//����_sys_exit()�Ա���ʹ�ð�����ģʽ  
_sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{
  	USART_SendData(USART1, (u8) ch);
  	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
  	return ch;
}
#endif
