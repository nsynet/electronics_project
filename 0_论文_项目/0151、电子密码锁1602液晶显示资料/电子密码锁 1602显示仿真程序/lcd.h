#ifndef _lcd_h
#include"at89x51.h"
#define uchar unsigned char
#define uint unsigned int
sbit E=P0^7;
sbit RW=P0^6;
sbit RS=P0^5;	
/***д����***/
void cmd_w(uchar cmd)
{
  RW=0;
  RS=0;
  E=1;
  P2=cmd;
  delay(1);
  E=0;
}
/***д����***/
void dat_w(uchar dat)
{
  RW=0;
  RS=1;
  E=1;
  P2=dat;
  delay(1);
  E=0;
}
/***����****/
void clear(void)
{
  cmd_w(0x01);	  //����
  cmd_w(0x02);	  //�����ص����Ͻ�
}
/***��ʼ��**/
void init(void)
{
  cmd_w(0x38);
  cmd_w(0x0d);	   
}
/***��궨λ****/
void gotoxy(uchar x,uchar y)	//x������,y������
{
 if(x==1)
 {cmd_w(0x80+y);}
 else
 {cmd_w(0xc0+y);}
}
#endif