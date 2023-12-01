#ifndef _LCD_H
#define _LCD_H

#define LCD_DATA   P0
sbit LCD_RS = P2^0;
sbit LCD_RW = P2^1;
sbit LCD_EN = P2^2;

void LCD_Busy(void);
void LCD_Wr_Command(unsigned char);
void LCD_Wr_Data(unsigned char);
void LCD_Wr_Char(unsigned char,unsigned char,unsigned char);
void LCD_Wr_String(unsigned char,unsigned char,unsigned char *);
void Print(unsigned char x,unsigned char y,unsigned char *str);
void LCD_Init(void);
void GoTo_XY(unsigned char x, unsigned char y);
void LCD_Clear_All(void);
void write_CGRAM(unsigned char add,unsigned char *p_tab,unsigned char size);

#endif