#include "stm32f10x.h"

#define	CS_SET  GPIOC->BSRR=1<<10    //片选端口  		PC10
#define	RS_SET	GPIOA->BSRR=1<<15    //数据/命令 		PA15  
#define	WR_SET	GPIOC->BSRR=1<<12    //写数据			PC12
#define	RD_SET	GPIOC->BSRR=1<<11    //读数据			PC11
#define	RST_SET	GPIOD->BSRR=1<<2   //复位			PD2
								    
#define	CS_RESET  GPIOC->BRR=1<<10     //片选端口  		PC10
#define	RS_RESET	GPIOA->BRR=1<<15     //数据/命令		PA15	   
#define	WR_RESET	GPIOC->BRR=1<<12    //写数据			PC12
#define	RD_RESET	GPIOC->BRR=1<<11    //读数据			PC11
#define	RST_RESET	GPIOD->BRR=1<<2     //复位			PD2
  
#define DATAOUT(x) GPIOB->ODR=x; //数据输出
#define DATAIN     GPIOB->IDR;   //数据输入	

#define White          0xFFFF
#define Black          0x0000
#define Blue           0x001F
#define Blue2          0x051F
#define Red            0xF800
#define Magenta        0xF81F
#define Green          0x07E0
#define Cyan           0x7FFF
#define Yellow         0xFFE0
#define Browm			 0XBC40 //棕色
#define Brred			 0XFC07 //棕红色
#define Gray  			 0X8430 //灰色
#define DarkBlue      	 0X01CF	//深蓝色
#define LightBlue     	 0X7D7C	//浅蓝色  
#define GrayBlue      	 0X5458 //灰蓝色
#define LightGreen     	 0X841F //浅绿色 
#define LightGray 			 0XC618 //浅灰色

//写数据函数
#define WriteData(data){\
RS_SET;\
CS_RESET;\
DATAOUT(data);\
WR_RESET;\
WR_SET;\
CS_SET;\
} 

//Lcd初始化及其低级控制函数
void LCD_GPIO_Init(void);
void LCD_Init(void);
void LCD_WR_REG(u16 Index,u16 CongfigTemp);
void LCD_WR_Start(void);
//Lcd高级控制函数
void LCD_ColorBox(u16 x,u16 y,u16 xLong,u16 yLong,u16 Color);
void DrawPixel(u16 x, u16 y, u16 Color);
void LCD_Clear(u16 color);
void BlockWrite(unsigned int Xstart,unsigned int Xend,unsigned int Ystart,unsigned int Yend);
void LCD_ShowChar(u16 x,u16 y,u8 num,u8 size,u8 mode,u16 Back_Color,u16 Point_Color);
void LCD_ShowString(u16 x,u16 y,u16 width,u16 height,u8 size,u8 *p,u16 Back_Color,u16 Point_Color);
u16 ReadPoint(u16 x, u16 y);
void LCD_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 color);
void LCD_Color_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 *color);


