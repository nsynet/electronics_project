#include "reg51.h"
#include "string.h"
sbit P3_2=P3^2;
char receive_data[96];
char flag;
char converse=0;
unsigned char code table[]={
0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};
//串口初始化 晶振为11.0592M　方式1 波特率300－57600
void InitCom(unsigned char BaudRate)
{
unsigned char THTL;
switch (BaudRate)
{
case 1: THTL = 64; break; //波特率300
case 2: THTL = 160; break; //600
case 3: THTL = 208; break; //1200
case 4: THTL = 232; break; //2400
case 5: THTL = 244; break; //4800
case 6: THTL = 250; break; //9600
case 7: THTL = 253; break; //19200
case 8: THTL = 255; break; //57600
default: THTL = 208;
}
SCON = 0x50; //串口方式1,允许接收
TMOD = 0x20; //定时器1定时方式2
TCON = 0x40; //设定时器1开始计数
TH1 = THTL;
TL1 = THTL;
PCON = 0x80; //波特率加倍控制,SMOD位
RI = 0; //清收发标志
TI = 0;
TR1 = 1; //启动定时器
}

//从串口接收一个定长字符串(非中断方式,只用于字符需有一个的idata字符串数组)
//如unsigned char *Str;
//unsigned char i[]="abcd";
//InHandStr = &i;
void ComInStr(unsigned char *Str)
{
while(*Str !='@') //判断是否到了字符串的尾部
{
while(!RI); //空语句判断字符是否收完
*Str = SBUF; //保存字符
RI = 0; //清RI
Str++; //字符串指针加一
converse=1;
}
}

void delay(int c)
    {
    int i,j;
    for(i=0;i<c;i++)
        for(j=0;j<10;j++)
            ;
     }
void delay1s(void) 

{ 

  unsigned char i,j,k; 

  for(k=60;k>0;k--) 

  for(i=20;i>0;i--) 

  for(j=248;j>0;j--); 

} 
void main()
{
    unsigned char i,j;    //i:每个字的显示循环；j每个字的显示码除以2�
    unsigned int b=0;      //显示偏移控制，char类型最多只能显示14个汉字+一个空白位字符
    unsigned char a;//控制移动间隔时间
   InitCom(6);
      EA=1;
      ES=1;
      ET1=0;
   while(1)
   {  char i;
      ComInStr(receive_data);
while(converse)
    {
    j=0;
    if(a>3) //移动间隔时间；取值0--255
        {
        a=0;
        b+=2;
        if(b>=96)  //显示到最后一个字，回头显示，判断值=字数*32
            {
            b=0;
            }   
        }
     P3_2=1;
     for(i=0;i<16;i++)
        {
        P1=i;
        P0=receive_data[j+b];
        P2=receive_data[j+b+1];
        delay(5);
        P0=0x00;
        P2=0x00;
        j+=2;
        }

     P3_2=0;
     for(i=0;i<16;i++)
        {
        P1=i;
        P0=receive_data[j+b];
        P2=receive_data[j+b+1];
        delay(5);
        P0=0x00;
        P2=0x00;
        j+=2;
        }
        a++;

    }
	  
  }


}

//串口接收中断函数 
//void serial () interrupt 4 using 3
//{
   //ComInStr(receive_data);
   //flag=1;
//}
