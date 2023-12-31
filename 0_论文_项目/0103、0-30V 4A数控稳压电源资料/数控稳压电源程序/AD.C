#include"reg52.h"
#include"function.h"
#include "intrins.h"

/****************AD端口定义****************/
sbit  AD_CLK  = P3^2;
sbit  AD_RST  = P3^3;
sbit  AD_DIN  = P3^7;
sbit  AD_DOUT = P3^6;
sbit  AD_DRDY = P3^5;
 
/************************************************************************************
                              延时  
************************************************************************************/
void  delay(uint i)
{
	for(;i > 0;i--);
} 
/************************************************************************************
                              复位AD7705 
************************************************************************************/
void rst_ad()
{
	AD_DRDY = 1;
	AD_CLK  = 1;
	AD_DIN  = 1;
	AD_DOUT = 1;
	AD_RST  = 0;
	delay(300);
	AD_RST  = 1;     //  复位电路可以接5V电   
	delay(500); 
}

/************************************************************************************
                              写1字节到AD7705
************************************************************************************/
write_ad(uchar i)
{
	uchar a;
	for(a = 8;a > 0;a--)
	{
		
		if((i & 0x80) == 0)
		   {AD_DIN = 0;}
		else
		   {AD_DIN = 1;}
		i <<= 1;
		AD_CLK = 0;
		_nop_();
		_nop_();
		_nop_();
		AD_CLK = 1;	 
	}
}

/************************************************************************************
                              设置AD7705通道 
*  0 为第一通道 
*  1 为第二通道  

    //MD1（0）  工 作 模 式 
	//MD0（0） 	工 作 模 式   
	//G2（0） 	增益选择    0 	  0 	0	 0	  1	   1	1	 1
	//G1（0） 	增益选择  	0 	  0		1	 1	  0	   0	1	 1
	//G0（0） 	增益选择  	0 	  1		0	 1	  0	   1	0	 1
	// 						1	  2		4	 8	  16   32	64	 128
    //B/U（0） 	单极性/双极性工作。“0”表示选择双极性操作，“1”表示选择单极性工作  
	//BUF（0） 	缓冲器控制。“0”表示片内缓冲器短路，缓冲器短路后，电源电流降低  
	//          此位处于高电平时，缓冲器与模拟输入串联，输入端允许处理高阻抗源  
	//FSYNC（1） 滤波器同步  
	  
************************************************************************************/

void set_7705_ch(bit a)
{
    if(a == 0)
	{
		write_ad(0x20);//write_ad(0x20);
		write_ad(0x05);
		write_ad(0x10);//write_ad(0x10);
		write_ad(0x44);//write_ad(0x46);//设置40H 双极性方式 44H 单极性方式
	}
	else
	{
		write_ad(0x20);//write_ad(0x20);
		write_ad(0x05);
		write_ad(0x10);//write_ad(0x10);
		write_ad(0x44);//write_ad(0x46);//设置40H 双极性方式 44H 单极性方式
	}    	
}


/************************************************************************************
                              读1字节到AD7705   
************************************************************************************/
uchar read_ad()
{
    uchar temp2;
	uchar a;
	for(a = 8;a > 0;a--)
	{
		AD_CLK = 0;
		_nop_();
		_nop_();
		_nop_();
		AD_CLK = 1;
    	_nop_();
		_nop_();
		_nop_();
		temp2 <<= 1;
		if(AD_DOUT == 0)
		   {temp2 &= 0xfe;}
		else
		   {temp2 |= 0x01;}
	 }
	 return temp2;
}

/************************************************************************************
                  读AD7705转换后的数据  
// 把DRDY位换为读内部寄存器状态位，显示不正常  
************************************************************************************/

uint read_AD_data(uchar y)
{
    uint  temp;
	uchar i;
	write_ad(y);
	while(AD_DRDY);
	for(i = 16;i > 0;i--)
	{
		AD_CLK = 0;
		_nop_();
		_nop_();
		_nop_();
		AD_CLK = 1;
    	_nop_();
		_nop_();
		_nop_();
		temp <<= 1;
		if(AD_DOUT == 0)
		   {temp &= 0xfffe;}
		else
		   {temp |= 0x0001;}
   }
   return temp;			
}

