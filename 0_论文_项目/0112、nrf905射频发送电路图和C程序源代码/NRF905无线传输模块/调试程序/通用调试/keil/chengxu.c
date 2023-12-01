/*2008/11/3
  AT89S52	12M*/
#include<reg52.h>
#include <intrins.h>
#define uint  unsigned int
#define uchar unsigned char
sbit	TXEN	=P2^6;
sbit	TRX_CE	=P2^5;
sbit	PWR 	=P2^4;
sbit	MISO	=P2^3;
sbit	MOSI	=P2^2;
sbit	SCK	=P2^1;
sbit	CSN	=P2^0;

sbit	AM	=P3^2;
sbit	DR	=P3^3;
sbit	CD	=P3^5;
 /*
sbit TRX_CE=P1^0;     //使能芯片接收和发送
sbit TXEN=P1^4;       //收发状态选择：1发射，0接收
sbit PWR=P1^5;        //工作状态控制：1正常工作，0微功耗
sbit AM=P1^1;         //地址匹配
sbit CD=P1^6;         //载波监听
sbit MISO=P1^2;       //SPI 输出,MCU 由此口从RF 芯片读入资料
sbit DR=P1^7;         //接收或发送就绪
sbit CSN=P3^1;        //SPI 使能，低激活
sbit MOSI=P3^0;       //SPI 输入,MCU 由此口向RF 芯片写入资料
sbit SCK=P1^3;        //SPI 时钟 */
sbit LCD=P0^1;		  //指示灯	
sbit jian1=P0^3;	  //控制键1
sbit jian2=P0^2;	  //控制键2
//RF寄存器配置//
unsigned char idata RFConf[11]=
{
    0x00,        //配置命令//
    0x6C,        //CH_NO,频率433.2MHZ
    0x2c,        //功率10db,重发,节电正常模式
    0x44,        //地址宽度,4字节
    0x01,0x01,    //接收发送数据有效长度
    0xE7,0xE7,0xE7,0xE7,    //接收地址
    0xDE,        //CRC允许,16位CRC校验,外部时钟信号使能,16M晶振 0xDE
};

uchar TxRxBuffer;
///////////延时/////////////////
void Delay(uint x)
{
    uint i;
    for(i=0;i<x;i++){
        _nop_();
    }
}

////////////用SPI写数据至NRF905//////////
void SpiWrite(unsigned char b)
{
    unsigned char i=8;
    while (i--)
    {
        Delay(10);
        SCK=0;
        MOSI=(bit)(b&0x80);
        b<<=1 ; 
        Delay(10);
        SCK=1; 
        Delay(10);
    }
    SCK=0;
}
///////////////从nrf905读取数据////////////////
unsigned char SpiRead(void)
{
    register unsigned char i=8;
    unsigned char ddata=0;
    while (i--)
    {
		Delay(10);
        ddata<<=1 ;
        SCK=1;
        ddata|=MISO;
		Delay(10);
        SCK=0 ;
    }
    SCK=0;
    return ddata;
}
///////////////接收数据包/////////////////
void RxPacket(void)
{
    TxRxBuffer= SpiRead();
}

/*
;写发射数据命令:20H
;读发射数据命令:21H
;写发射地址命令:22H
;读发射地址命令:23H
;读接收数据命令:24H
*/
///////////////发送数据包/////////////////
void TxPacket(uchar a)
{
	TRX_CE=0;
    TXEN=1;
    CSN=1;
	Delay(10);
	CSN=0;
    SpiWrite(0x22);        //写发送地址,4位地址//
    SpiWrite(0x00);
    SpiWrite(0x00);
    SpiWrite(0x00);
    SpiWrite(0x00);
    CSN=1;
    Delay(10);
    CSN=0;
    SpiWrite(0x20);        //写发送数据//
    SpiWrite(a);
    CSN=1;
    Delay(10);
    TRX_CE=1;                    //使能发射模式//
	Delay(10000);
    while(!DR);                    //等待发射完成//
    TRX_CE=0;
	TRX_CE=1;
	TXEN=0;
	Delay(1000);
}	   
////////////////等待接收数据包///////////////////
void Wait_Rec_Packet(void)
{ 
	TRX_CE=0;            //数据准备好,进入待机模式
    CSN=1;
	Delay(1000);
	CSN=0;
    SpiWrite(0x24);
    RxPacket();
    CSN=1;
	TRX_CE=1;
   	TXEN=0;
	Delay(10);
}
////////////初始化配置寄存器////////////////
void Ini_System(void)
{
    uchar i;
    LCD=0;
    Delay(1000);
    LCD=1;
    CSN=1;
    SCK=0;
	PWR=1;
    TRX_CE=0;
    TXEN=0;
    Delay(10);
    CSN=0;
    for(i=0;i<11;i++){
        SpiWrite(RFConf[i]);
    }
    CSN=1;
    TRX_CE=1;
    TXEN=0;
    Delay(1000);
	LCD=0;
}
        
void main(void)
{
	Delay(1000);
    Ini_System();
    while(1)
    {
		jian1=jian2=1;
        if(jian1==0){
			TxPacket(0x03);
			Delay(10000);
		}
		if(jian2==0){
			TxPacket(0x02);
			Delay(10000);
		}
		if(DR==1){
			Wait_Rec_Packet();
			Delay(10000);
		}
		if(TxRxBuffer==0x03){
			LCD=0;
		}
		if(TxRxBuffer==0x02){
			LCD=1;
		}		
    }
}