

#include "fsmc_sram.h"

#define Bank1_LCD_D    ((uint32_t)0x60020000)    //显示缓存区地址
#define Bank1_LCD_C    ((uint32_t)0x60000000)	 //显示寄存器地址



void LCD_Init1(void);
void LCD_WR_REG(unsigned int index);
void LCD_WR_CMD(unsigned int index,unsigned int val);

void LCD_WR_Data(unsigned int val);

void LCD_test(void);
void LCD_clear(unsigned int p);

void Delay(__IO uint32_t nCount);

unsigned short LCD_RD_data(void);
extern void lcd_rst(void);
extern void Delay(__IO uint32_t nCount);


/****************************************************************************
* 名    称：LCD_WR_REG(unsigned int index)
* 功    能：FSMC写显示器寄存器地址函数
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/ 
void LCD_WR_REG(unsigned int index)
{
	*(__IO uint16_t *) (Bank1_LCD_C)= index;

}			 

/****************************************************************************
* 名    称：void LCD_WR_CMD(unsigned int index,unsigned int val)
* 功    能：FSMC写显示器寄存器数据函数
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/ 
void LCD_WR_CMD(unsigned int index,unsigned int val)
{	
	*(__IO uint16_t *) (Bank1_LCD_C)= index;	
	*(__IO uint16_t *) (Bank1_LCD_D)= val;
}


/****************************************************************************
* 名    称：unsigned short LCD_RD_data(void)
* 功    能：FSMC读显示区16位数据函数
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/ 
unsigned short LCD_RD_data(void){
	unsigned int a=0;
	a=*(__IO uint16_t *) (Bank1_LCD_D);   //空操作
	a=*(__IO uint16_t *) (Bank1_LCD_D);   //读出的实际16位像素数据	  
	return(a);	
}



/****************************************************************************
* 名    称：LCD_WR_Data(unsigned int val)
* 功    能：FSMC写16位数据函数
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/ 
void LCD_WR_Data(unsigned int val)
{   
	*(__IO uint16_t *) (Bank1_LCD_D)= val; 	
}


/****************************************************************************
* 名    称：void Delay(__IO uint32_t nCount)
* 功    能：延时函数
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/ 
void Delay(__IO uint32_t nCount)
{
  for(; nCount != 0; nCount--);
}

/****************************************************************************
* 名    称：void LCD_Init(void)
* 功    能：3 寸TFT(R61509A驱动）初始化函数
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/ 
void LCD_Init1(void)
{	  
    GPIO_ResetBits(GPIOB, GPIO_Pin_12);
    Delay(0x1AFFf);					   
    GPIO_SetBits(GPIOB, GPIO_Pin_12 );		 //液晶复位
	Delay(0x1AFFf);	
     LCD_WR_CMD(0x0083,0x0002);//打开测试模式，TEST MODE=1      
        LCD_WR_CMD(0x0085,0x0003);//启用3个vddd，不知道什么意思，网上说是电容
        LCD_WR_CMD(0x008b,0x0000);//电阻0欧姆,GS_RES1=0    
        LCD_WR_CMD(0x008c,0x013); //GS_RES0=0                   

        LCD_WR_CMD(0x0091,0x01 );//DCDC_CLK和CLK同步显示
        LCD_WR_CMD(0x0083,0x00); //关闭测试模式，TEST MODE=0

//		LCD_WriteReg(0x005a,0x01); //开启Gamma LPT修正
		//Gamma修正                                   
        LCD_WR_CMD(0x003e,0xb0);	           
        LCD_WR_CMD(0x003f,0x0003);
		LCD_WR_CMD(0x0040,0x0010);
		LCD_WR_CMD(0x0041,0x0056);
		LCD_WR_CMD(0x0042,0x0013);
		LCD_WR_CMD(0x0043,0x0046);
		LCD_WR_CMD(0x0044,0x0023);		
		LCD_WR_CMD(0x0045,0x0076);
		LCD_WR_CMD(0x0046,0x0000);
		LCD_WR_CMD(0x0047,0x005e);
		LCD_WR_CMD(0x0048,0x004f);								
		LCD_WR_CMD(0x0049,0x0040);

		//Power On sequence
		LCD_WR_CMD(0x0017,0x0091);//设置晶振频率为100%，即7.5Mhz，使能晶振
		//LCD_WR_CMD(0x0023,0x0001);//
		LCD_WR_CMD(0x002B,0x00f9);//N_DC=0xf9
		//delay(10); // delay 10 ms
		LCD_WR_CMD(0x001b,0x0014);//BT=B0001,AP=B100
		LCD_WR_CMD(0x001A,0x0011);
		LCD_WR_CMD(0x001c,0x000d);//VRH=3.6times,1.25*3.6=4.5 0x0006 0x000d  
		LCD_WR_CMD(0x001F,0x0027);//0x0042  0x0027
		//delay(20); // delay 20 ms

		LCD_WR_CMD(0x0019,0x000A);
		LCD_WR_CMD(0x0019,0x001A);
	//delay(40); // delay 40 ms
		LCD_WR_CMD(0x0019,0x0012); 
	//delay(40); // delay 40 ms
		LCD_WR_CMD(0x001E,0x002c);//0x0027  0x002c
		//delay(100); // delay 100 ms
//*******************官方手册中的代码**********
		LCD_WR_CMD(0x003c,0x00c0);
		LCD_WR_CMD(0x003D,0x00c0);
		LCD_WR_CMD(0x0034,0x0038);
		LCD_WR_CMD(0x0035,0x0038);
		LCD_WR_CMD(0x0024,0x0038);
		LCD_WR_CMD(0x0024,0x003C); 
		LCD_WR_CMD(0x0001,0x0006);//正常显示模式，若赋值0x0002则反演模式
		LCD_WR_CMD(0x0055,0x0000);	
		//DRIVE TABLE(寄存器 16H)
		LCD_WR_CMD(0x0016,0x0068);//横屏模式2，  MY=0 MX=1 MV =1 --- GS=0 RGB=1 ---  ---   SM=0   	RGB格式.
				
				LCD_WR_CMD(0x0002,0x0000);	//行起点的高八位						         
        LCD_WR_CMD(0x0003,0x0000);	//行起点        
        LCD_WR_CMD(0x0004,(399)>>8);//行终点高八位		          
        LCD_WR_CMD(0x0005,399);		//行终点，低八位		         
        LCD_WR_CMD(0x0006,0x0000);	//列起点
        LCD_WR_CMD(0x0007,0x0000);	//列起点   
        LCD_WR_CMD(0x0008,(239)>>8);//列终点
        LCD_WR_CMD(0x0009,239);		//列终点	
// lcd_SetWindows(0,0,399,239);

}



