/*******************************************************************************                                    
  *                                                
  ******************************************************************************
  * 文件名称：UART.H
  * 描    述：串口头文件
  * 创 建 者: 
  * 创建日期:   
  * 修 改 者:             
  * 修改日期:      
  * 版    本: 
  ******************************************************************************
  * attention
  *
  ******************************************************************************
  */

#ifndef  __UART_H__ 
#define  __UART_H__
/**********************头文件调用及变量定义声明****************************************/
/*  包含必要的头文件  */
  
  #include   "stm32f0xx_conf.h"
  #include <stdarg.h>
  #include <stdio.h>


//板载串口1的宏定义

#define F072RB_COM                           USART2
#define F072RB_COM_CLK                       RCC_APB1Periph_USART2

#define F072RB_COM_TX_PIN                    GPIO_Pin_2
#define F072RB_COM_RX_PIN                    GPIO_Pin_3
#define F072RB_COM_PORT           	         GPIOA
#define F072RB_COM_PORT_CLK            	     RCC_AHBPeriph_GPIOA


#define F072RB_COM_TX_SOURCE                 GPIO_PinSource2
#define F072RB_COM_TX_AF                     GPIO_AF_1

#define F072RB_COM_RX_SOURCE                 GPIO_PinSource3
#define F072RB_COM_RX_AF                     GPIO_AF_1


/*  子函数定义        */




extern  void Uart_Init(void);  
extern  void Uart_Put_Char(unsigned char c);
extern  unsigned char Uart_Get_Char(void);
extern  int  fputc(int ch, FILE *f);

#endif  

