#include "stm32f10x.h"

u16 CheckController(void);

void lcd_Initializtion(void);
void lcd_Reset(void);
void lcd_WriteRegister(uint16_t index,u16 dat);
void lcd_SetCursor(u16 x,u16 y);
void lcd_SetWindows(u16 StartX,u16 StartY,u16 EndX,u16 EndY);
void lcd_DrawPicture(u16 StartX,u16 StartY,u16 EndX,u16 EndY,u16 *pic);
void lcd_SetPoint(u16 x,u16 y,u16 point);
void lcd_PutChar(u16 x,u16 y,u8 c,u16 charColor,u16 bkColor);
void lcd_Clear(u16 dat);
void lcd_Delay(u32 nCount);
void lcd_Test(void);
u16 lcd_GetCode(void);
void lcd_WriteData(u16 dat);
void lcd_WriteIndex(u16 idx);

void lcd_BackLight(u8 status);

u16 lcd_BGR2RGB(u16 c);

u16 lcd_GetPoint(u16 x,u16 y);
u16 lcd_ReadData(void);
u16 lcd_ReadRegister(u16 index);

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

