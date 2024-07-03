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
//  UART初始化
extern void uartInit(void);


//  通过UART发送一个字符
extern void uartPutc(u8 c);

//  通过UART发送字符串
extern void uartPuts(u8 *s);

//  通过UART接收一个字符
extern char uartGetc(void);

//  通过UART接收字符串，回显，退格<Backspace>修改，回车<Enter>结束
extern int uartGets(char *s, int size);
//  通过UART发送字符串
extern void USART_SendString(u8 *str); 
//	通过UART将一个short型的数据拆分成两个char型的数据进行发送
void put_int_in_num(short value);

#endif  //  __UART_GET_PUT_H__

