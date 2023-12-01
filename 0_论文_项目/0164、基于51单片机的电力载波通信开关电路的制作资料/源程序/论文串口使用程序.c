/*
此程序已经经本人亲自调试运行过，如有疑问，请与马同学联系
QQ：277039462

*/
#include<reg52.h>	  //预定义头文件
#define uchar unsigned char	//宏定义
sbit keyin=P3^3; //按键定义
sbit led=P3^5; //指示灯定义
uchar a;	//接收数据记存器
void main()	//主函数
{
	keyin=1; //按键口置1
	led=1;	 //灭led灯
	TMOD=0x20; //选择顶时器/计数器1中的方式2
	SP=0x60;   //	设置堆榨地址
	TH1=0xfd; //波特率9600bit/s
	TL1=0xfd;
	SCON=0x50;	//串口通信方式2，允许收发
	TR1=1;		//开启计数器
	while(1)   //循环
	{		
		if(keyin==0) //判断按键是否按下，是则按键处理
		{
			SBUF=0x00;	//接收数据处理
			while(TI==0) ; 	//发送完清除接收标志位
			TI=0;
		}
		if(RI==1)  //是否接收
		{
			RI=0; 	 //接收完接收标志位清零
			a=SBUF;	  //读接收数据
			if(a==0)   //判断接收数据是否正确
			{
				if(led==1)	//正确led灯取反
				led=0;
				else 
				led=1;
			}
		}
	}
}