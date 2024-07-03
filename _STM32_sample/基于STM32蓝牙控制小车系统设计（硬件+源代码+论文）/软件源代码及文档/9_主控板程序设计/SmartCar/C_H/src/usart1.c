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
	
	   //Usart1 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器USART1
	
	
	       //IO口设置
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
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;  //与M3不一样,M3是浮空输入
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_InitStructure);  	

  GPIO_PinAFConfig(GPIOB,GPIO_PinSource6,GPIO_AF_USART1);
  GPIO_PinAFConfig(GPIOB,GPIO_PinSource7,GPIO_AF_USART1);

	
           //USART 初始化设置
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); 
	USART_InitStructure.USART_BaudRate = bound;//一般设置为9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//8位数据
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//收发模式
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_Init(USART1, &USART_InitStructure);
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);  //开启串口接收中断
	USART_Cmd(USART1, ENABLE);                    //使能串口 

}
//串口中断程序必须在接收第二个数据之前出中断，否则有些数据没有接收

void USART1_IRQHandler(void)                	//串口1中断服务程序
{
	
	u8 Res;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
		{
		Res =USART_ReceiveData(USART1);//(USART1->DR);	//读取接收到的数据
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
			case 'f':Direction(1);//正转
			Change_PWM(2000,2000,2000,2000);
			//printf(" start!");
			printf(" right direction");
			break;
			case 'b':Direction(0);//反转
			Change_PWM(2000,2000,2000,2000);
			printf(" contray direction!");
			break;
		}
}

//加入以下代码,支持printf函数,而不需要选择use MicroLIB
#if 1
#pragma import(__use_no_semihosting)   
//标准库需要的支持函数   
struct __FILE 
{ 
	int handle; 
}; 
FILE __stdout;      
//定义_sys_exit()以避免使用半主机模式  
_sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{
  	USART_SendData(USART1, (u8) ch);
  	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
  	return ch;
}
#endif
