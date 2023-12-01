#include "USART.h"
#include "STC12C5A.h"
#define BAUD 9600
//串口初始化
void USART_Init()
{
	PCON &= 0x7f;		//波特率不倍速 SMOD=0
	SCON = 0x50;		//8位数据,可变波特率
	AUXR |= 0x40;		//定时器1时钟为Fosc,即1T
	AUXR &= 0xfe;		//串口1选择定时器1为波特率发生器
	TMOD &= 0x0f;		//清除定时器1模式位
	TMOD |= 0x20;		//设定定时器1为8位自动重装方式
	TL1 = 0xB2;			//设定定时初值
	TH1 = 0xB2;			//设定定时器重装值
	ET1 = 0;			//禁止定时器1中断
	TR1 = 1;			//启动定时器1
	ES = 1;				//开串行中断
	EA = 1;				//开总中断
}	  

//发送一个字符,ASCII码
void SendASC(unsigned char d)
{
	
	SBUF=d;
	while(!TI);	//等待发送完成,TI置位
	TI=0;
}

//发送字符串
void SendString(unsigned char *str)	
{
	while(*str)
	{
		SendASC(*str) ;
		str++;
	}
}


//串口中断服务程序
void USART_Interrupt(void)  interrupt 4	  //放在这里 和放在main（）里面是一样的
{
	unsigned char RxData=0;
	EA=0; 
   	if(RI) 			//必须判断RI(接收中断标志)是否为1
    {
    	RI=0;
    	RxData = SBUF;	//在这里添加你的接收代码
		SendString("The word you send is ");
		SendASC(RxData);
		SendASC('\n');
    }
	EA=1;
}


