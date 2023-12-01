#include "stm32f10x.h"
#include "LCD.h"
#include "Font.h" 

void LCD_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB |RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD | RCC_APB2Periph_AFIO,ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE); //禁止JTAG功能
	//DB00--DB15
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//最高输出速率50MHz
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//推挽输出
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化外设GPIOx寄存器
	//RS
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//最高输出速率50MHz
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//推挽输出
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化外设GPIOx寄存器
	//RST
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//最高输出速率50MHz
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//推挽输出
	GPIO_Init(GPIOD, &GPIO_InitStructure);//初始化外设GPIOx寄存器
	//CS  RD  WR
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//最高输出速率50MHz
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//推挽输出
	GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化外设GPIOx寄存器
	
	GPIO_SetBits(GPIOC, GPIO_Pin_10);//CS
	GPIO_SetBits(GPIOA, GPIO_Pin_15);//RS
	GPIO_SetBits(GPIOC, GPIO_Pin_12);//WR
	GPIO_SetBits(GPIOC, GPIO_Pin_11);//RD
	GPIO_SetBits(GPIOD, GPIO_Pin_2);//RST
}

volatile static void Delay(__IO u32 nCount)
{	
	volatile int i;
	for(i=0;i<7200;i++)
    for(; nCount != 0; nCount--);
} 

volatile static void LCD_Rst(void)
{			
    RST_RESET;
    Delay(10000);					   
    RST_SET;		 	 
    Delay(10000);	
}
static void WriteComm(u16 CMD)
{			
	RS_RESET;
	CS_RESET;
	DATAOUT(CMD);
	WR_RESET;
	WR_SET;
	CS_SET;
}

void LCD_Init(void)
{	

Delay(20);
LCD_Rst();

WriteComm(0xB0);
WriteData(0x04);
 
WriteComm(0x36);    
WriteData(0x60);    
 
WriteComm(0x3A);    
WriteData(0x55);   
 
WriteComm(0xB4);    
WriteData(0x00); 
 
WriteComm(0xC0);
WriteData(0x03);//0013
WriteData(0xDF);//480
WriteData(0x40);
WriteData(0x12);
WriteData(0x00);
WriteData(0x01);
WriteData(0x00);
WriteData(0x43);
 
 
WriteComm(0xC1);//frame frequency
WriteData(0x05);//BCn,DIVn[1:0
WriteData(0x2f);//RTNn[4:0] 
WriteData(0x08);// BPn[7:0]
WriteData(0x08);// FPn[7:0]
WriteData(0x00);
 
 
 
WriteComm(0xC4);
WriteData(0x63);
WriteData(0x00);
WriteData(0x08);
WriteData(0x08);
 
WriteComm(0xC8);//Gamma
WriteData(0x06);
WriteData(0x0c);
WriteData(0x16);
WriteData(0x24);//26
WriteData(0x30);//32 
WriteData(0x48);
WriteData(0x3d);
WriteData(0x28);
WriteData(0x20);
WriteData(0x14);
WriteData(0x0c);
WriteData(0x04);
 
WriteData(0x06);
WriteData(0x0c);
WriteData(0x16);
WriteData(0x24);
WriteData(0x30);
WriteData(0x48);
WriteData(0x3d);
WriteData(0x28);
WriteData(0x20);
WriteData(0x14);
WriteData(0x0c);
WriteData(0x04);
 
 
 
WriteComm(0xC9);//Gamma
WriteData(0x06);
WriteData(0x0c);
WriteData(0x16);
WriteData(0x24);//26
WriteData(0x30);//32 
WriteData(0x48);
WriteData(0x3d);
WriteData(0x28);
WriteData(0x20);
WriteData(0x14);
WriteData(0x0c);
WriteData(0x04);
 
WriteData(0x06);
WriteData(0x0c);
WriteData(0x16);
WriteData(0x24);
WriteData(0x30);
WriteData(0x48);
WriteData(0x3d);
WriteData(0x28);
WriteData(0x20);
WriteData(0x14);
WriteData(0x0c);
WriteData(0x04);
 
 
 
WriteComm(0xCA);//Gamma
WriteData(0x06);
WriteData(0x0c);
WriteData(0x16);
WriteData(0x24);//26
WriteData(0x30);//32 
WriteData(0x48);
WriteData(0x3d);
WriteData(0x28);
WriteData(0x20);
WriteData(0x14);
WriteData(0x0c);
WriteData(0x04);
 
WriteData(0x06);
WriteData(0x0c);
WriteData(0x16);
WriteData(0x24);
WriteData(0x30);
WriteData(0x48);
WriteData(0x3d);
WriteData(0x28);
WriteData(0x20);
WriteData(0x14);
WriteData(0x0c);
WriteData(0x04);
 
 
WriteComm(0xD0);
WriteData(0x95);
WriteData(0x06);
WriteData(0x08);
WriteData(0x10);
WriteData(0x3f);
 
 
WriteComm(0xD1);
WriteData(0x02);
WriteData(0x28);
WriteData(0x28);
WriteData(0x40);
 
WriteComm(0xE1);    
WriteData(0x00);    
WriteData(0x00);    
WriteData(0x00);    
WriteData(0x00);    
WriteData(0x00);   
WriteData(0x00);   
 
WriteComm(0xE2);    
WriteData(0x80);    
 
WriteComm(0x2A);    
WriteData(0x00);    
WriteData(0x00);    
WriteData(0x01);    
WriteData(0x3F);    
 
WriteComm(0x2B);    
WriteData(0x00);    
WriteData(0x00);    
WriteData(0x01);    
WriteData(0xDF);    
 
WriteComm(0x11);
 
Delay(120);
 
WriteComm(0x29);
 
WriteComm(0xC1);//frame frequency
WriteData(0x05);//BCn,DIVn[1:0
WriteData(0x2f);//RTNn[4:0] 
WriteData(0x08);// BPn[7:0]
WriteData(0x08);// FPn[7:0]
WriteData(0x00);

WriteComm(0x20);
// while(1);
		//LCD_Fill_Pic(0,0,480,320, gImage_Boot);
		//Delay(50000000);
}

/******************************************
函数名：Lcd写命令函数
功能：向Lcd指定位置写入应有命令或数据
入口参数：Index 要寻址的寄存器地址
          ConfigTemp 写入的数据或命令值
******************************************/
void LCD_WR_REG(u16 Index,u16 CongfigTemp)
{
	WriteComm(Index);
	WriteData(CongfigTemp);
}

/************************************************
函数名：Lcd写开始函数
功能：控制Lcd控制引脚 执行写操作
************************************************/
void LCD_WR_Start(void)
{
  WriteComm(0x2C);
}

/**********************************************
函数名：Lcd块选函数
功能：选定Lcd上指定的矩形区域

注意：xStart、yStart、Xend、Yend随着屏幕的旋转而改变，位置是矩形框的四个角

入口参数：xStart x方向的起始点
          ySrart y方向的起始点
          Xend   y方向的终止点
          Yend   y方向的终止点
返回值：无
***********************************************/
void BlockWrite(unsigned int Xstart,unsigned int Xend,unsigned int Ystart,unsigned int Yend) 
{
	//HX8352-C

	WriteComm(0x2a);   
	WriteData(Xstart>>8);
	WriteData(Xstart&0xff);
	WriteData(Xend>>8);
	WriteData(Xend&0xff);

	WriteComm(0x2b);   
	WriteData(Ystart>>8);
	WriteData(Ystart&0xff);
	WriteData(Yend>>8);
	WriteData(Yend&0xff);


	WriteComm(0x2c);
}

/**********************************************
函数名：Lcd块选函数
功能：选定Lcd上指定的矩形区域

注意：xStart和 yStart随着屏幕的旋转而改变，位置是矩形框的四个角

入口参数：xStart x方向的起始点
          ySrart y方向的终止点
          xLong 要选定矩形的x方向长度
          yLong  要选定矩形的y方向长度
返回值：无
***********************************************/
void LCD_ColorBox(u16 xStart,u16 yStart,u16 xLong,u16 yLong,u16 Color)
{
	u32 temp;

	BlockWrite(xStart,xStart+xLong-1,yStart,yStart+yLong-1);
	for (temp=0; temp<xLong*yLong; temp++)
	{
		WriteData(Color);
	}
}
//在指定区域内填充指定颜色
//区域大小:(xend-xsta+1)*(yend-ysta+1)
//xsta
//color:要填充的颜色
void LCD_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 color)
{          
	u16 i,j;
	BlockWrite(sx,ex,sy,ey);
	j=(ex-sx+1)*(ey-sy+1);
		for(i=0;i<j;i++)
		{
		 	WriteData(color);	  
		}
} 
//在指定区域内填充指定颜色块			 
//(sx,sy),(ex,ey):填充矩形对角坐标,区域大小为:(ex-sx+1)*(ey-sy+1)   
//color:要填充的颜色
void LCD_Color_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 *color)
{  
	u16 height,width;
	u16 i;
	width=ex-sx+1; 			//得到填充的宽度
	height=ey-sy+1;			//高度
	BlockWrite(sx,ex,sy,ey);
 	for(i=0;i<height*width;i++)
	{
 		WriteData(color[i]);
	}	  
} 
void LCD_Clear(u16 color)
{
	u32 index=0;      
	u32 totalpoint=320*480;//得到总点数
	BlockWrite(0,479,0,319); 	//设置光标位置 	  	  
	for(index=0;index<totalpoint;index++)WriteData(color);	
}  

void DrawPixel(u16 x, u16 y, u16 Color)
{
	BlockWrite(x,x+1,y,y+1);
	WriteData(Color);
}

u16 ReadPoint(u16 x, u16 y)
{
	u16 data;
	if(x>=480||y>=320)return 0;
	
	WriteComm(0x2a);   
	WriteData(x>>8);
	WriteData(x&0xff);
	WriteData((x+1)>>8);
	WriteData((x+1)&0xff);

	WriteComm(0x2b);   
	WriteData(y>>8);
	WriteData(y&0xff);
	WriteData((y+1)>>8);
	WriteData((y+1)&0xff);
	
	WriteComm(0x2E);
	
	GPIOB->CRL=0X88888888; 							//PB0-7  上拉输入
	GPIOB->CRH=0X88888888; 							//PB8-15 上拉输入
	GPIOB->ODR=0X0000;     							//全部输出高
	
	RS_SET;
	CS_RESET;
	RD_RESET;
	Delay(1);
	data=DATAIN; 
	RD_SET;
	//Dummy Read
	RD_RESET;
	Delay(1);
	data=DATAIN; 
	RD_SET;
	CS_SET;
	
	GPIOB->CRL=0X33333333; 		//PB0-7  上拉输出
	GPIOB->CRH=0X33333333; 		//PB8-15 上拉输出
	GPIOB->ODR=0X0000;    		//全部输出高  
	
	return data;
}
//在指定位置显示一个字符
//x,y:起始坐标
//num:要显示的字符:" "--->"~"
//size:字体大小 12/16/24
//mode:叠加方式(1)还是非叠加方式(0)
//Back_Color:背景颜色
//Point_Color:字体颜色
void LCD_ShowChar(u16 x,u16 y,u8 num,u8 size,u8 mode,u16 Back_Color,u16 Point_Color)
{  							  
    u8 temp,t1,t;
	u16 y0=y;
	u8 csize=(size/8+((size%8)?1:0))*(size/2);		//得到字体一个字符对应点阵集所占的字节数	
 	num=num-' ';//得到偏移后的值（ASCII字库是从空格开始取模，所以-' '就是对应字符的字库）
	for(t=0;t<csize;t++)
	{   
		if(size==12)temp=asc2_1206[num][t]; 	 	//调用1206字体
		else if(size==16)temp=asc2_1608[num][t];	//调用1608字体
		else if(size==24)temp=asc2_2412[num][t];	//调用2412字体
		else return;								//没有的字库
		for(t1=0;t1<8;t1++)
		{			    
			if(temp&0x80)DrawPixel(x,y,Point_Color);
			else if(mode==0)DrawPixel(x,y,Back_Color);
			temp<<=1;
			y++;
			if(y>=320)return;		//超区域了
			if((y-y0)==size)
			{
				y=y0;
				x++;
				if(x>=480)return;	//超区域了
				break;
			}
		}  	 
	}  	    	   	 	  
}   
//显示字符串
//x,y:起点坐标
//width,height:区域大小  
//size:字体大小
//*p:字符串起始地址		  
void LCD_ShowString(u16 x,u16 y,u16 width,u16 height,u8 size,u8 *p,u16 Back_Color,u16 Point_Color)
{         
	u8 x0=x;
	width+=x;
	height+=y;
    while((*p<='~')&&(*p>=' '))//判断是不是非法字符!
    {       
        if(x>=width){x=x0;y+=size;}
        if(y>=height)break;//退出
        LCD_ShowChar(x,y,*p,size,1,Back_Color,Point_Color);
        x+=size/2;
        p++;
    }  
}






