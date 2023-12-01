//网站http://proteus.5d6d.com
#ifndef _key_h
#define _key_h
#include"at89x51.h"
#define uchar unsigned char 
#define uint unsigned int
uchar ki=0,flag;	 //flag是按键标志 ki是获得的键盘数
/***1ms延时**/
void delay(uint k)
{
  k=k*125;
  while(k--);
}
/***键盘编号程序**/
uchar number(uchar k)
{
  uchar sum;
  switch(k)
  {
    case 1:sum=0;break;
	case 2:sum=1;break;
	case 4:sum=2;break;
	case 8:sum=3;break;
	default:break;
  }
  return sum;
}
/***键盘程序**/
key(void)
{
  uchar m,n;
  P3=0x0f;
  delay(1);
  if(P3!=0x0f)
   {
     P3=0x0f;
	 delay(3);
	 m=P3;
	 if(m!=0x0f)
	  {
	    m=m^0x0f;
		m=number(m);
		m=4*m;
		P3=0xf0;
		delay(1);
		if(P3!=0xf0)
		  {
		    n=P3^0xf0;
		    n=n>>4;
			n=number(n);
			ki=m+n;		  //ki是获得的键盘号
			flag=1;
			do
			 {
			 P3=0x0f; 
			 }
			 while(P3!=0x0f);  //松开按键才可以继续
          }
	  }
   }
}
#endif