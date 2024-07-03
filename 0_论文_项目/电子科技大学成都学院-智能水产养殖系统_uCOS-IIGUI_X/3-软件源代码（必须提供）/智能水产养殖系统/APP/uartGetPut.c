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



//  UART初始化
void uartInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	USART_ClockInitTypeDef USART_ClockInitStructure;
	NVIC_InitTypeDef  NVIC_InitStructure;
	/*********GPIO*使能RX/TX所在的GPIO端口*********/
	//RX
	GPIO_InitStructure.GPIO_Pin = USART_GPIO_RX;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(USART_GPIO, &GPIO_InitStructure);
	//TX
	GPIO_InitStructure.GPIO_Pin = USART_GPIO_TX;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(USART_GPIO, &GPIO_InitStructure);
	/********* NVIC*使能RX/TX所在的GPIO端口*********/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	
	/*********USART**********/  //  配置UART端口
	USART_ClockInitStructure.USART_Clock = USART_Clock_Disable  ;	//时钟低电平活动
	USART_ClockInitStructure.USART_CPOL =USART_CPOL_Low  ; //时钟低电平
	USART_ClockInitStructure.USART_CPHA = USART_CPHA_1Edge;	 //时钟第二个边沿进行数据捕获
	USART_ClockInitStructure.USART_LastBit = USART_LastBit_Disable;	//最后一位数据的时钟脉冲不从SCLK 输出
	
	USART_ClockInit(USART, &USART_ClockInitStructure);
	
	USART_InitStructure.USART_BaudRate = 9600;  //  波特率：9600
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;   //  数据位：8
	USART_InitStructure.USART_StopBits = USART_StopBits_1;	    //  停止位：1
	USART_InitStructure.USART_Parity = USART_Parity_No;			 //  校验位：无
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //禁能硬件流控制模式
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//使能接收和发送模式
	USART_Init(USART, &USART_InitStructure);	  //  //USART 配置
	USART_ITConfig(USART, USART_IT_RXNE, ENABLE);//使能USART1接收中断	
	//	USART_ITConfig(USART1, USART_IT_TXE, ENABLE);//使能USART1发送中断
	USART_Cmd(USART, ENABLE);
}


//  通过UART发送一个字符
void uartPutc(u8 c)
{
   USART_SendData(USART, c);
}


//  通过UART发送字符串
void uartPuts(u8  *s)
{  
	while (*(s)!= '\0')
	{
	uartPutc(*(s));
	
	s++;
	}
//	 USART_SendData(USART1, );
}


//  通过UART接收一个字符
char uartGetc(void)
{
    return(USART_ReceiveData(USART));
}

//  通过UART发送字符串
void USART_SendString(u8 *str)
{
	while(*str != 0)
	{
		USART_SendData(USART, *(str));
		str++;
		while(USART_GetFlagStatus(USART, USART_FLAG_TXE) == RESET);
	}  
}

//  功能：通过UART接收字符串，回显，退格<Backspace>修改，回车<Enter>结束
//  参数：*s保存接收数据的缓冲区，只接收可打印字符（ASCII码32～127）
//        size是缓冲区*s的总长度，要求size >= 2（包括末尾'\0'，建议用sizeof()来获取）
//  返回：接收到的有效字符数目
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
		c = uartGetc();                                     //  接收1个字符
		uartPutc(c);                                        //  回显输入的字符
		
		if (c == '\b')                                      //  遇退格<Backspace>修改
		{
			if (n > 0)
			{
				*(--s) = '\0';
				n--;
				uartPuts(" \b");                            //  显示空格和退格<Backspace>
			}
		}
		
		if (c == '\r')                                      //  遇回车<Enter>结束
		{
			uartPuts("\r\n");                               //  显示回车换行<CR><LF>
			break;
		}
		
		if (n < size)                                       //  如果小于长度限制
		{
			if (isprint(c))                                 //  如果接收到的是可打印字符
			{
				*(s++) = c;                                 //  保存接收到的字符到缓冲区
				*s = '\0';
				n++;
			}
		}
	}
	
	return(n);                                              //  返回接收到的有效字符数目
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

//  通过UART接收中断
void USART1_IRQHandler(void)
{
	u8 data;
 //串口接收中断标志位置位
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

	//清除串口中断标志
	USART_ClearITPendingBit(USART, USART_IT_RXNE);
	}
}

