#define  WRled   XBYTE[0xfff0]

sbit UpData= P1^3;
sbit DisSwitch  = P1^4;

sbit RowA=P1^5;
sbit RowB=P1^6;
sbit RowC=P1^7;


sfr   P4=0xe8;
sbit  DS1302_CLK = P4^0;  //实时时钟时钟线引脚 
sbit  DS1302_RST = P4^1;  //实时时钟复位线引脚
sbit  DS1302_IO  = P4^2;  //实时时钟数据线引脚

sbit  ACC0 = ACC^0;
sbit  ACC7 = ACC^7;

delay(unsigned int time)
{
   while(time--);
}
