#include "stm32f10x_lib.h"

//#define __USE_CH_FONT__

#define nCsPin  GPIO_Pin_5
#define RsPin   GPIO_Pin_1
#define nWrPin  GPIO_Pin_0
#define nRdPin  GPIO_Pin_7
#define nRstPin GPIO_Pin_6
#define Lcd_LightPin GPIO_Pin_1

//硬件相关的子函数  
#define Set_Cs GPIOB->BSRR = nCsPin;
#define Clr_Cs  GPIOB->BRR = nCsPin;

#define Set_Rs  GPIOB->BSRR = RsPin;
#define Clr_Rs  GPIOB->BRR = RsPin;

#define Set_nWr GPIOB->BSRR = nWrPin;
#define Clr_nWr GPIOB->BRR = nWrPin;

#define Set_nRd GPIOB->BSRR = nRdPin;
#define Clr_nRd GPIOB->BRR = nRdPin;

#define Set_Rst GPIOB->BSRR = nRstPin;
#define Clr_Rst GPIOB->BRR = nRstPin;

#define Lcd_Light_ON   GPIOA->BSRR = Lcd_LightPin;
#define Lcd_Light_OFF  GPIOA->BRR = Lcd_LightPin;


/*
#define Set_nWr (*((volatile unsigned long *) 0x40011010) = nWrPin)
#define Clr_nWr (*((volatile unsigned long *) 0x40011014) = nWrPin)

#define Set_Cs  (*((volatile unsigned long *) 0x40011010) = nCsPin)
#define Clr_Cs  (*((volatile unsigned long *) 0x40011014) = nCsPin)

#define Set_Rs  (*((volatile unsigned long *) 0x40011010) = RsPin)
#define Clr_Rs  (*((volatile unsigned long *) 0x40011014) = RsPin)

#define Set_nRd (*((volatile unsigned long *) 0x40011010) = nRdPin)
#define Clr_nRd (*((volatile unsigned long *) 0x40011014) = nRdPin)

#define Set_Rst (*((volatile unsigned long *) 0x40011010) = nRstPin)
#define Clr_Rst (*((volatile unsigned long *) 0x40011014) = nRstPin)

#define Lcd_Light_ON   (*((volatile unsigned long *) 0x40011010) = Lcd_LightPin)
#define Lcd_Light_OFF  (*((volatile unsigned long *) 0x40011014) = Lcd_LightPin)
*/

extern u8 ttybuf[64];

#define lprintf(...) do{char *p = (char *)ttybuf; sprintf(p, __VA_ARGS__);while(*p){ili9320_putc(*p++);}}while(0)

u16 CheckController(void);
void Lcd_Configuration(void);
void ili9320_Initializtion(void);
void ili9320_Reset(void);
void ili9320_WriteRegister(u16 index,u16 dat);
void ili9320_SetCursor(u16 x,u16 y);
void ili9320_SetWindows(u16 StartX,u16 StartY,u16 EndX,u16 EndY);
void ili9320_DrawPicture(u16 StartX,u16 StartY,u16 EndX,u16 EndY,u16 *pic);
void ili9320_FillRect(u16 StartX,u16 StartY,u16 EndX,u16 EndY,u16 clr);
void ili9320_SetPoint(u16 x,u16 y,u16 point);
void ili9320_PutChar_cf(u16 x,u16 y,u8 c,u16 charColor,u16 bkColor);
void ili9320_PutChar_f(u16 x,u16 y,u8 c,u16 charColor,u16 bkColor);
void ili9320_PutChar(u16 x,u16 y,u8 c,u16 charColor,u16 bkColor);
void ili9320_putc(u8 c);
void ili9320_Clear(u16 dat);
void ili9320_Delay(u32 nCount);
void ili9320_Test(void);
u16 ili9320_GetCode(void);
#define ili9320_WriteData(x) GPIOC->ODR = (x)
//void ili9320_WriteData(u16 dat);
void ili9320_WriteIndex(u16 idx);

void ili9320_BackLight(u8 status);

u16 ili9320_BGR2RGB(u16 c);
void ili9320_clrline(u16 y);

u16 ili9320_GetPoint(u16 x,u16 y);
u16 ili9320_ReadData(void);
u16 ili9320_ReadRegister(u16 index);
void ili9320_moveto(u16 x, u16 y);

u16 GUI_Color565(u32 RGB);  // RGB颜色转为16位(565)

void GUI_Text(u16 x, u16 y, u8 *str, u16 len,u16 Color, u16 bkColor);
void GUI_Line(u16 x0, u16 y0, u16 x1, u16 y1,u16 color);  // 画线
void GUI_Circle(u16 cx,u16 cy,u16 r,u16 color,u8 fill);  // 画园
void GUI_Rectangle(u16 x0, u16 y0, u16 x1, u16 y1,u16 color,u8 fill); // 画矩形
void GUI_Square(u16 x0, u16 y0, u16 with, u16 color,u8 fill);  // 画正方形


void Touch_Initializtion(void);
u16  Touch_GetPhyX(void);
u16  Touch_GetPhyY(void);
u16  Touch_MeasurementX(void);
u16  Touch_MeasurementY(void);

void Safe_STDOUT_Init(void);
#define SAFE_STDOUT(x) //do{extern OS_MUT STDOUT_Mutex;Safe_STDOUT_Init();os_mut_wait(STDOUT_Mutex,0xFFFF);x;os_mut_release(STDOUT_Mutex);}while(0)

#define SAFE_STDOUT_BEGIN()// do{extern OS_MUT STDOUT_Mutex;Safe_STDOUT_Init();os_mut_wait(STDOUT_Mutex,0xFFFF)
#define SAFE_STDOUT_END()  // os_mut_release(STDOUT_Mutex);}while(0)

