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
sbit TRX_CE=P1^0;     //ʹ��оƬ���պͷ���
sbit TXEN=P1^4;       //�շ�״̬ѡ��1���䣬0����
sbit PWR=P1^5;        //����״̬���ƣ�1����������0΢����
sbit AM=P1^1;         //��ַƥ��
sbit CD=P1^6;         //�ز�����
sbit MISO=P1^2;       //SPI ���,MCU �ɴ˿ڴ�RF оƬ��������
sbit DR=P1^7;         //���ջ��;���
sbit CSN=P3^1;        //SPI ʹ�ܣ��ͼ���
sbit MOSI=P3^0;       //SPI ����,MCU �ɴ˿���RF оƬд������
sbit SCK=P1^3;        //SPI ʱ�� */
sbit LCD=P0^1;		  //ָʾ��	
sbit jian1=P0^3;	  //���Ƽ�1
sbit jian2=P0^2;	  //���Ƽ�2
//RF�Ĵ�������//
unsigned char idata RFConf[11]=
{
    0x00,        //��������//
    0x6C,        //CH_NO,Ƶ��433.2MHZ
    0x2c,        //����10db,�ط�,�ڵ�����ģʽ
    0x44,        //��ַ���,4�ֽ�
    0x01,0x01,    //���շ���������Ч����
    0xE7,0xE7,0xE7,0xE7,    //���յ�ַ
    0xDE,        //CRC����,16λCRCУ��,�ⲿʱ���ź�ʹ��,16M���� 0xDE
};

uchar TxRxBuffer;
///////////��ʱ/////////////////
void Delay(uint x)
{
    uint i;
    for(i=0;i<x;i++){
        _nop_();
    }
}

////////////��SPIд������NRF905//////////
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
///////////////��nrf905��ȡ����////////////////
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
///////////////�������ݰ�/////////////////
void RxPacket(void)
{
    TxRxBuffer= SpiRead();
}

/*
;д������������:20H
;��������������:21H
;д�����ַ����:22H
;�������ַ����:23H
;��������������:24H
*/
///////////////�������ݰ�/////////////////
void TxPacket(uchar a)
{
	TRX_CE=0;
    TXEN=1;
    CSN=1;
	Delay(10);
	CSN=0;
    SpiWrite(0x22);        //д���͵�ַ,4λ��ַ//
    SpiWrite(0x00);
    SpiWrite(0x00);
    SpiWrite(0x00);
    SpiWrite(0x00);
    CSN=1;
    Delay(10);
    CSN=0;
    SpiWrite(0x20);        //д��������//
    SpiWrite(a);
    CSN=1;
    Delay(10);
    TRX_CE=1;                    //ʹ�ܷ���ģʽ//
	Delay(10000);
    while(!DR);                    //�ȴ��������//
    TRX_CE=0;
	TRX_CE=1;
	TXEN=0;
	Delay(1000);
}	   
////////////////�ȴ��������ݰ�///////////////////
void Wait_Rec_Packet(void)
{ 
	TRX_CE=0;            //����׼����,�������ģʽ
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
////////////��ʼ�����üĴ���////////////////
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