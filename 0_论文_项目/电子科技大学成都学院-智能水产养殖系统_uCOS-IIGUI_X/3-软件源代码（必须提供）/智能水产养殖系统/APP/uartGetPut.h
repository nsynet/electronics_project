#ifndef  __UART_GET_PUT_H__
#define  __UART_GET_PUT_H__
#include "stm32f10x.h"
//#include "stm32f10x_usart.h"
extern volatile char Sta1[10],Sta2[10],Sta3[10],Sta4[10];
extern volatile  short status1,status2,status3;


#define  USART              USART1
#define  USART_GPIO 		GPIOA
#define	 USART_GPIO_RX      GPIO_Pin_10
#define	 USART_GPIO_TX	    GPIO_Pin_9
//  UART��ʼ��
extern void uartInit(void);


//  ͨ��UART����һ���ַ�
extern void uartPutc(u8 c);

//  ͨ��UART�����ַ���
extern void uartPuts(u8 *s);

//  ͨ��UART����һ���ַ�
extern char uartGetc(void);

//  ͨ��UART�����ַ��������ԣ��˸�<Backspace>�޸ģ��س�<Enter>����
extern int uartGets(char *s, int size);
//  ͨ��UART�����ַ���
extern void USART_SendString(u8 *str); 
//	ͨ��UART��һ��short�͵����ݲ�ֳ�����char�͵����ݽ��з���
void put_int_in_num(short value);

#endif  //  __UART_GET_PUT_H__

