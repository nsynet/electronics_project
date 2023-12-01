#include <iom16v.h>			//包含型号头文件
#include <macros.h>			//包含"位"操作头文件
#include <stdio.h>
#include <DS18B20.h>
#define uchar unsigned char	
#define uint  unsigned int
#define SEGLK PA3	//数码管段选锁存器控制端
#define BITLK PA4	//数码管位选锁存器控制端
#define LEDLK PA2	//LED锁存器控制端

const SEGMENT[10]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d, 0x07,
		           0x7f,0x6f};
/*******************************************
函数名称: Delayms
功    能: 延时指定毫秒（8M晶振）
参    数: MS--延时的毫秒数
返回值  : 无
********************************************/
void Delayms(uint MS)		  
{
 uint i,j;
 for( i=0;i<MS;i++)
 for(j=0;j<1141;j++);	//1141是在8MHz晶振下，通过软件仿真反复实验得到的数值
}
/*******************************************
函数名称: Board_init
功    能: 初始化实验板，关闭全部功能
参    数: 无
返回值  : 无
/********************************************/
void Board_init(void)
{
	DDRA=0xFF;	//将四个IO口全部配置为输出
	DDRB=0xFF;
	DDRC=0xFF;
	DDRD=0xFF;

	SPCR=0x00;	//还原SPI口，使其为正常端口	

	PORTA|=BIT(PA6);//(BEEP)关闭蜂鸣器
	PORTC&=~BIT(PC6);	  //(RELAY)初始化继电器为常闭连接状态	
	PORTA&=~BIT(PA1);	//关闭SPI总线DA
	PORTB=0xFF;	//关闭LED
	PORTA|=BIT(LEDLK);	//锁存数据，使LED熄灭
 	Delayus(5);		
 	PORTA&=~BIT(LEDLK);
	
	PORTB=0x00;	   //输出段选
 	PORTA|=BIT(SEGLK);		  
 	Delayus(50);			  
 	PORTA&=~BIT(SEGLK);	    
 	
 	PORTB=0xFF;	   //输出位选
 	PORTA|=BIT(BITLK);		   
 	Delayus(50);				   
 	PORTA&=~BIT(BITLK);	   
	
	PORTD=0xFF;
	PORTA|=BIT(PA7);
	Delayus(50);
	PORTA&=~BIT(PA7);

	
}

/*******************************************
函数名称: One_smg_display
功    能: 指定的数码管显示指定的内容
参    数: data--显示的内容(0-15)
	  	  number--指定的数码管(1-6)
返回值  : 无
/********************************************/
void One_smg_display(uchar data,uchar number)
{ 
 PORTB|=0x3F;			   //输出位选
 PORTA|=BIT(BITLK);		   //更新位选
 Delayus(50);			   //调整时序
 PORTA&=~BIT(BITLK);	   //锁存位选

 PORTB=SEGMENT[data];	   //输出段选
 PORTA|=BIT(SEGLK);		   //更新段选
 Delayus(50);			   //调整时序
 PORTA&=~BIT(SEGLK);	   //锁存段选
 
 number=~BIT(number-1);	   //调整位选（指定时是1~6，而操作的时候是0~5）
 PORTB=number;			   //输出位选
 PORTA|=BIT(BITLK);		   //更新位选
 Delayus(50);				   //调整时序
 PORTA&=~BIT(BITLK);	   //锁存位选
}

/********************************************/
void uart0_init(void) 
{
  UCSRB = 0x00;  //禁止发送
  UCSRA = 0x02;  //倍速
  UCSRC = 0x06;  //8位数据位，1个停止位
  UBRRL = 0x67;  //波特率为9600
  UBRRH = 0x00; 
  UCSRB = 0x18;  //允许发送
}
/********************************************************/
void uart0_send(uchar i)  //发送一个字符
{
  while(!(UCSRA&(1<<UDRE))); //等待发送缓冲区为空
    UDR=i;                    //发送数据
  while(!(UCSRA&(1<<TXC)));		 //等待发送完毕
    UCSRA|=1<<TXC;	           //清除发送完毕状态位
}
/*******************************************/
void str_send(char *s) //发送一串字符
{
 while(*s)
 {
  uart0_send(*s); //字符串未结束则继续发送
  s++;
 }
}
/*********************************************/
uchar uart0_receive(void) //接收一个字符
{
  while(!(UCSRA&(1<<RXC)));//等待接收数据
  return UDR;               //返回数据
}

/*******************************************
函数名称: main
功    能: 6个数码管循环滚动显示指定的内容
参    数: 无
返回值  : 无
/********************************************/
void main(void)
{	 uint t=0;
     uchar temh_d,teml_d,temm_d;
	 //uchar RDATA[3];
	 uart0_init();
	 Board_init();	   //初始化开发板
	 while(1)
	 {
	 
	  t=Read_temp()*0.625;//得到转换温度值
      Delayms(2);
	  temh_d=t/100;
	  temm_d=(t/10)%10;
	  teml_d=t%10;
	 
	   
		//uart0_send(temh_d+48);
		//Delayms(1);
	   // uart0_send(temm_d+48);
		//Delayms(1);
	    uart0_send(temh_d+48);
		uart0_send(temm_d+48);
		str_send(".");
		uart0_send(teml_d+48);
		uart0_send(32);
		uart0_send(32);
		uart0_send(32);
		//Delayms(1);
		//str_send(" ");
		
	  One_smg_display(teml_d,6);
	  Delayms(2);
	  One_smg_display(temm_d,5);
	  Delayms(2);
	  One_smg_display(temh_d,4);
	  Delayms(2);
	
	 }
}