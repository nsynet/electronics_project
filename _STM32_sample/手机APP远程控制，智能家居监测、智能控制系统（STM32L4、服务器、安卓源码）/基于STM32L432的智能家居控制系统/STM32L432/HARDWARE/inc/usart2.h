#ifndef _USART2_H
#define _USART2_H
#include "sys.h"
#include "stdio.h"	
#include "usart.h"
#include "timer.h"

#define USART_REC_LEN  			200  	//�����������ֽ��� 200
#define EN_USART2_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����
#define GP2Y1050_COLLECT_TIMES  30  //PM2.5ģ��ɼ�ƽ��������ÿ�ɼ��ô�����һ��ƽ��ֵ����ֵ���Ϊ64���������������ݲ�׼

extern UART_HandleTypeDef UART2_Handler; //UART���

#define RXBUFFERSIZE   1 //�����С
extern u8 aRxBuffer[RXBUFFERSIZE];//HAL��USART����Buffer

extern u8 GP2Y1050_Times;
extern float GP2Y1050_Quality;

//����봮���жϽ��գ��벻Ҫע�����º궨��
void uart2_init(u32 bound);
void uart1_init(u32 pclk2,u32 bound);

#endif
