#ifndef _lcd_h
#include"at89x51.h"
#define uchar unsigned char
#define uint unsigned int
sbit E=P0^7;
sbit RW=P0^6;
sbit RS=P0^5;	
/***写命令***/
void cmd_w(uchar cmd)
{
  RW=0;
  RS=0;
  E=1;
  P2=cmd;
  delay(1);
  E=0;
}
/***写数据***/
void dat_w(uchar dat)
{
  RW=0;
  RS=1;
  E=1;
  P2=dat;
  delay(1);
  E=0;
}
/***清屏****/
void clear(void)
{
  cmd_w(0x01);	  //清屏
  cmd_w(0x02);	  //清屏回到左上角
}
/***初始化**/
void init(void)
{
  cmd_w(0x38);
  cmd_w(0x0d);	   
}
/***光标定位****/
void gotoxy(uchar x,uchar y)	//x是行数,y是列数
{
 if(x==1)
 {cmd_w(0x80+y);}
 else
 {cmd_w(0xc0+y);}
}
#endif