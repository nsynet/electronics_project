#include "reg52.h"
#include "absacc.h"
#include "string.h"
#include "lcd.h"

//#define WR_COM XBYTE[0x7ff0]
//#define RD_STA XBYTE[0x7ff2]
//#define WR_DAT XBYTE[0x7ff1]
//#define RD_DAT XBYTE[0x7ff3]
bit fTemp;

/************************************************
** 函数名称: Delay
** 功能描述: 延时一段时间
** 输　  入: unsigned char i
** 输　  出: 无
** 全局变量: 
** 设    计: 简小木     时间:2008-10-12
*************************************************/
void Delay(unsigned char i)
{
	while(i--);
}

/************************************************
** 函数名称: LCD_Busy
** 功能描述: 判断LCD是否处于忙状态
** 输　  入: 无
** 输　  出: 无
** 全局变量: 
** 设    计: 简小木     时间:2008-10-12
*************************************************/
void LCD_Busy(void)
{
	bit BusyFlag;
	LCD_DATA = 0xff;
	while(1)
	{
		LCD_RS = 0;
		Delay(1);
		LCD_RW = 1;
		Delay(1);
		LCD_EN = 1;
		BusyFlag = (bit)((LCD_DATA && 0x80)>>7);
		if(!BusyFlag)break;
		LCD_EN =0;
	}
}

/************************************************
** 函数名称: LCD_Wr_Command
** 功能描述: 给LCD写指令
** 输　  入: 命令com
** 输　  出: 无
** 全局变量: 
** 设    计: 简小木     时间:2008-10-12
*************************************************/
void LCD_Wr_Command(unsigned char com)
{
	LCD_Busy();
	LCD_DATA = com;
	LCD_RS = 0;
	Delay(1);
	LCD_RW = 0;
	Delay(1);
	LCD_EN = 0;
}

/************************************************
** 函数名称: LCD_Wr_Data
** 功能描述: 给LCD写数据
** 输　  入: 数据dat
** 输　  出: 无
** 全局变量: 
** 设    计: 简小木     时间:2008-10-12
*************************************************/
void LCD_Wr_Data(unsigned char dat)
{
	LCD_Busy();
	LCD_DATA = dat;
	LCD_RS = 1;
	Delay(1);
	LCD_RW = 0;
	Delay(1);
	LCD_EN = 0;
}

/************************************************
** 函数名称: LCD_Wr_Char
** 功能描述: 写字符
** 输　  入: 列:x, 行:y, 字符:c
** 输　  出: 无
** 全局变量: 
** 设    计: 简小木     时间:2008-10-12
*************************************************/
void LCD_Wr_Char(unsigned char x,unsigned char y,unsigned char c)
{
	if(y==0)
		LCD_Wr_Command(0x80+x);
	else if(y==1)
		LCD_Wr_Command(0xc0+x);

	else if(y>1){}

	LCD_Wr_Data(c);
}

/************************************************
** 函数名称: LCD_Wr_String
** 功能描述: 写字符串
** 输　  入: 列:x, 行:y, 字符串指针:str
** 输　  出: 无
** 全局变量: 
** 设    计: 简小木     时间:2008-10-12
*************************************************/
void LCD_Wr_String(unsigned char x,unsigned char y,unsigned char *str)
{
	unsigned int i;
	i=strlen(str);
	
	while(i!=0)
	{
		if(x>=16)
		{
			y++;
			x = 0;
		}
		if(y>=2)break;

		LCD_Wr_Char(x,y,*str);
		x++;
		str++;
		i--;
	}
}

void Print(unsigned char x,unsigned char y,unsigned char *str)
{
	while(*str!='\0')
	{
		if(x>=16)
		{
			y++;
			x = 0;
		}
		if(y>=2)break;

		LCD_Wr_Char(x,y,*str);
		x++;
		str++;
	}
}

/************************************************
** 函数名称: LCD_Init
** 功能描述: 液晶初始化
** 输　  入: 无
** 输　  出: 无
** 全局变量: 
** 设    计: 简小木     时间:2008-10-12
*************************************************/
void LCD_Init(void)
{
	LCD_RS = 0;
	LCD_RW = 0;
	LCD_EN = 0;
	LCD_Wr_Command(0x30|0x28|0x20);
	Delay(2);
	LCD_Wr_Command(0x0c);
	LCD_Wr_Command(0x01);
	LCD_Wr_Command(0x80);
}

/************************************************
** 函数名称: write_CGRAM
** 功能描述: 向CGRAM写入数据
** 输　  入: add为首地址，size为字符的个数
** 输　  出: 无
** 全局变量: 
** 设    计: 简小木     时间:2009-02-02
*************************************************/	
void write_CGRAM(unsigned char add,unsigned char *p_tab,unsigned char size)
{
	unsigned char lenth=0;			
	lenth=size*8;     		//每个字符，8个码               
	add=0x40+8*add;         //CGRAM的地址D5位要为1                
	while(lenth!=0)			
	{
		if(add>=0x80)                  	//CGRAM的地址到顶
			break;
		LCD_Wr_Command(add);	       		//写CGRAM地址
		LCD_Wr_Data(*p_tab);   			//写CGRAM数据
		lenth--;						//下一个码
		add++;                         	//下个地址                                  
		p_tab++;                   		//下个字符
	}
}

/************************************************
** 函数名称: GotoXY
** 功能描述: 指定坐标的位置
** 输　  入: 无
** 输　  出: 无
** 全局变量: 
** 设    计: 简小木     时间:2008-10-12
*************************************************/
/*
void GoTo_XY(unsigned char x, unsigned char y)
{
	if(y==0)
		LCD_Wr_Command(0x80|x);
	if(y==1)
		LCD_Wr_Command(0x80|(x-0x40));
}
*/
/************************************************
** 函数名称: num_to_char
** 功能描述: 
** 输　  入: 无
** 输　  出: 无
** 全局变量: 
** 设    计: 简小木     时间:2008-10-12
*************************************************/
//unsigned char  num_to_char(unsigned char num)
//{
//	return(num+'0');
//}

/************************************************
** 函数名称: LCD_Clear_ALL
** 功能描述: 
** 输　  入: 无
** 输　  出: 无
** 全局变量: 
** 设    计: 简小木     时间:2008-10-12
*************************************************/
void LCD_Clear_All(void)
{
	unsigned char i,j;

	if(fTemp)
	{
		for(j=0;j<2;j++)
		{
			for(i=0;i<16;i++)
				LCD_Wr_Char(i,j,0x00);
		}
	}
}