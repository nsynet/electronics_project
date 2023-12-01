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
** ��������: Delay
** ��������: ��ʱһ��ʱ��
** �䡡  ��: unsigned char i
** �䡡  ��: ��
** ȫ�ֱ���: 
** ��    ��: ��Сľ     ʱ��:2008-10-12
*************************************************/
void Delay(unsigned char i)
{
	while(i--);
}

/************************************************
** ��������: LCD_Busy
** ��������: �ж�LCD�Ƿ���æ״̬
** �䡡  ��: ��
** �䡡  ��: ��
** ȫ�ֱ���: 
** ��    ��: ��Сľ     ʱ��:2008-10-12
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
** ��������: LCD_Wr_Command
** ��������: ��LCDдָ��
** �䡡  ��: ����com
** �䡡  ��: ��
** ȫ�ֱ���: 
** ��    ��: ��Сľ     ʱ��:2008-10-12
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
** ��������: LCD_Wr_Data
** ��������: ��LCDд����
** �䡡  ��: ����dat
** �䡡  ��: ��
** ȫ�ֱ���: 
** ��    ��: ��Сľ     ʱ��:2008-10-12
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
** ��������: LCD_Wr_Char
** ��������: д�ַ�
** �䡡  ��: ��:x, ��:y, �ַ�:c
** �䡡  ��: ��
** ȫ�ֱ���: 
** ��    ��: ��Сľ     ʱ��:2008-10-12
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
** ��������: LCD_Wr_String
** ��������: д�ַ���
** �䡡  ��: ��:x, ��:y, �ַ���ָ��:str
** �䡡  ��: ��
** ȫ�ֱ���: 
** ��    ��: ��Сľ     ʱ��:2008-10-12
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
** ��������: LCD_Init
** ��������: Һ����ʼ��
** �䡡  ��: ��
** �䡡  ��: ��
** ȫ�ֱ���: 
** ��    ��: ��Сľ     ʱ��:2008-10-12
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
** ��������: write_CGRAM
** ��������: ��CGRAMд������
** �䡡  ��: addΪ�׵�ַ��sizeΪ�ַ��ĸ���
** �䡡  ��: ��
** ȫ�ֱ���: 
** ��    ��: ��Сľ     ʱ��:2009-02-02
*************************************************/	
void write_CGRAM(unsigned char add,unsigned char *p_tab,unsigned char size)
{
	unsigned char lenth=0;			
	lenth=size*8;     		//ÿ���ַ���8����               
	add=0x40+8*add;         //CGRAM�ĵ�ַD5λҪΪ1                
	while(lenth!=0)			
	{
		if(add>=0x80)                  	//CGRAM�ĵ�ַ����
			break;
		LCD_Wr_Command(add);	       		//дCGRAM��ַ
		LCD_Wr_Data(*p_tab);   			//дCGRAM����
		lenth--;						//��һ����
		add++;                         	//�¸���ַ                                  
		p_tab++;                   		//�¸��ַ�
	}
}

/************************************************
** ��������: GotoXY
** ��������: ָ�������λ��
** �䡡  ��: ��
** �䡡  ��: ��
** ȫ�ֱ���: 
** ��    ��: ��Сľ     ʱ��:2008-10-12
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
** ��������: num_to_char
** ��������: 
** �䡡  ��: ��
** �䡡  ��: ��
** ȫ�ֱ���: 
** ��    ��: ��Сľ     ʱ��:2008-10-12
*************************************************/
//unsigned char  num_to_char(unsigned char num)
//{
//	return(num+'0');
//}

/************************************************
** ��������: LCD_Clear_ALL
** ��������: 
** �䡡  ��: ��
** �䡡  ��: ��
** ȫ�ֱ���: 
** ��    ��: ��Сľ     ʱ��:2008-10-12
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