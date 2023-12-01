//////////////////////////////////////////////////////////////////////////////
//
// 	Filename:	Screen.c
//	Version:		0.01
//	Data:		2005.12.20
//
//	Author:		Liu, Zemin
//	Company:	zmLAB
//
//-----------------------------------------------------------------------------
//
// 	Target: 		ATmega64
// 	Tool chain: 	WinAvr (GCC-AVR)
//
//
//-----------------------------------------------------------------------------
//	Required files:
//
//-----------------------------------------------------------------------------
//	Notes:
//
//
//-----------------------------------------------------------------------------
//	Revision History:
//		V0.01		2005.12.20
//		1. First created.
//
///////////////////////////////////////////////////////////////////////////////
//
//-----------------------------------------------------------------------------
// 	Includes
//-----------------------------------------------------------------------------
#include "avr/io.h"
#include	"avr/Pgmspace.h"
#include <avr/interrupt.h>
#include	"Common.h"
#include	"Board.h"
#include 	"Screen.h"


// ==========================================================
//	File Scope Variables
// ==========================================================
//


PROGMEM	U8	Font5X7[(128 - 32)*5]={
	0x00, 0x00, 0x00, 0x00, 0x00,// (space)
	0x00, 0x00, 0x5F, 0x00, 0x00,// !
	0x00, 0x07, 0x00, 0x07, 0x00,// "
	0x14, 0x7F, 0x14, 0x7F, 0x14,// #
	0x24, 0x2A, 0x7F, 0x2A, 0x12,// $
	0x23, 0x13, 0x08, 0x64, 0x62,// %
	0x36, 0x49, 0x55, 0x22, 0x50,// &
	0x00, 0x05, 0x03, 0x00, 0x00,// '
	0x00, 0x1C, 0x22, 0x41, 0x00,// (
	0x00, 0x41, 0x22, 0x1C, 0x00,// )
	0x08, 0x2A, 0x1C, 0x2A, 0x08,// *
	0x08, 0x08, 0x3E, 0x08, 0x08,// +
	0x00, 0x50, 0x30, 0x00, 0x00,// ,
	0x08, 0x08, 0x08, 0x08, 0x08,// -
	0x00, 0x60, 0x60, 0x00, 0x00,// .
	0x20, 0x10, 0x08, 0x04, 0x02,// /
	0x3E, 0x51, 0x49, 0x45, 0x3E,// 0
	0x00, 0x42, 0x7F, 0x40, 0x00,// 1
	0x42, 0x61, 0x51, 0x49, 0x46,// 2
	0x21, 0x41, 0x45, 0x4B, 0x31,// 3
	0x18, 0x14, 0x12, 0x7F, 0x10,// 4
	0x27, 0x45, 0x45, 0x45, 0x39,// 5
	0x3C, 0x4A, 0x49, 0x49, 0x30,// 6
	0x01, 0x71, 0x09, 0x05, 0x03,// 7
	0x36, 0x49, 0x49, 0x49, 0x36,// 8
	0x06, 0x49, 0x49, 0x29, 0x1E,// 9
	0x00, 0x36, 0x36, 0x00, 0x00,// :
	0x00, 0x56, 0x36, 0x00, 0x00,// ;
	0x00, 0x08, 0x14, 0x22, 0x41,// <
	0x14, 0x14, 0x14, 0x14, 0x14,// =
	0x41, 0x22, 0x14, 0x08, 0x00,// >
	0x02, 0x01, 0x51, 0x09, 0x06,// ?
	0x32, 0x49, 0x79, 0x41, 0x3E,// @
	0x7E, 0x11, 0x11, 0x11, 0x7E,// A
	0x7F, 0x49, 0x49, 0x49, 0x36,// B
	0x3E, 0x41, 0x41, 0x41, 0x22,// C
	0x7F, 0x41, 0x41, 0x22, 0x1C,// D
	0x7F, 0x49, 0x49, 0x49, 0x41,// E
	0x7F, 0x09, 0x09, 0x01, 0x01,// F
	0x3E, 0x41, 0x41, 0x51, 0x32,// G
	0x7F, 0x08, 0x08, 0x08, 0x7F,// H
	0x00, 0x41, 0x7F, 0x41, 0x00,// I
	0x20, 0x40, 0x41, 0x3F, 0x01,// J
	0x7F, 0x08, 0x14, 0x22, 0x41,// K
	0x7F, 0x40, 0x40, 0x40, 0x40,// L
	0x7F, 0x02, 0x04, 0x02, 0x7F,// M
	0x7F, 0x04, 0x08, 0x10, 0x7F,// N
	0x3E, 0x41, 0x41, 0x41, 0x3E,// O
	0x7F, 0x09, 0x09, 0x09, 0x06,// P
	0x3E, 0x41, 0x51, 0x21, 0x5E,// Q
	0x7F, 0x09, 0x19, 0x29, 0x46,// R
	0x46, 0x49, 0x49, 0x49, 0x31,// S
	0x01, 0x01, 0x7F, 0x01, 0x01,// T
	0x3F, 0x40, 0x40, 0x40, 0x3F,// U
	0x1F, 0x20, 0x40, 0x20, 0x1F,// V
	0x7F, 0x20, 0x18, 0x20, 0x7F,// W
	0x63, 0x14, 0x08, 0x14, 0x63,// X
	0x03, 0x04, 0x78, 0x04, 0x03,// Y
	0x61, 0x51, 0x49, 0x45, 0x43,// Z
	0x00, 0x00, 0x7F, 0x41, 0x41,// [
	0x02, 0x04, 0x08, 0x10, 0x20,// "\"
	0x41, 0x41, 0x7F, 0x00, 0x00,// ]
	0x04, 0x02, 0x01, 0x02, 0x04,// ^
	0x40, 0x40, 0x40, 0x40, 0x40,// _
	0x00, 0x01, 0x02, 0x04, 0x00,// `
	0x20, 0x54, 0x54, 0x54, 0x78,// a
	0x7F, 0x48, 0x44, 0x44, 0x38,// b
	0x38, 0x44, 0x44, 0x44, 0x20,// c
	0x38, 0x44, 0x44, 0x48, 0x7F,// d
	0x38, 0x54, 0x54, 0x54, 0x18,// e
	0x08, 0x7E, 0x09, 0x01, 0x02,// f
	0x08, 0x14, 0x54, 0x54, 0x3C,// g
	0x7F, 0x08, 0x04, 0x04, 0x78,// h
	0x00, 0x44, 0x7D, 0x40, 0x00,// i
	0x20, 0x40, 0x44, 0x3D, 0x00,// j
	0x00, 0x7F, 0x10, 0x28, 0x44,// k
	0x00, 0x41, 0x7F, 0x40, 0x00,// l
	0x7C, 0x04, 0x18, 0x04, 0x78,// m
	0x7C, 0x08, 0x04, 0x04, 0x78,// n
	0x38, 0x44, 0x44, 0x44, 0x38,// o
	0x7C, 0x14, 0x14, 0x14, 0x08,// p
	0x08, 0x14, 0x14, 0x18, 0x7C,// q
	0x7C, 0x08, 0x04, 0x04, 0x08,// r
	0x48, 0x54, 0x54, 0x54, 0x20,// s
	0x04, 0x3F, 0x44, 0x40, 0x20,// t
	0x3C, 0x40, 0x40, 0x20, 0x7C,// u
	0x1C, 0x20, 0x40, 0x20, 0x1C,// v
	0x3C, 0x40, 0x30, 0x40, 0x3C,// w
	0x44, 0x28, 0x10, 0x28, 0x44,// x
	0x0C, 0x50, 0x50, 0x50, 0x3C,// y
	0x44, 0x64, 0x54, 0x4C, 0x44,// z
	0x00, 0x08, 0x36, 0x41, 0x00,// {
	0x00, 0x00, 0x7F, 0x00, 0x00,// |
	0x00, 0x41, 0x36, 0x08, 0x00,// }
	0x08, 0x08, 0x2A, 0x1C, 0x08,// ->
	0x08, 0x1C, 0x2A, 0x08, 0x08 // <-	};
};

#define	DispSize		1024
U8	DispBuf[DispSize];


// ==========================================================
//	Function Definitions
// ==========================================================
//
void	ClearBuf(void)
{
 U8 	*ptmp;
 U16	count;

 // Clear Display buffer
 ptmp = DispBuf;
 count = 0;
 while(count < 1024) {
 	*ptmp++ = 0x00;
	count++;
 	}
 
}

void	Display(void)
{
 U8	*ptmp;
 U8	tmp1, tmp2;

 ptmp = DispBuf;

 // Set display OFF
// while(!(LCD_Write(0, 0b00111110, 0))) {
// 	}
// while(!(LCD_Write(0, 0b00111110, 1))) {
// 	}

// cli();
 
 // Set display start line to 0
 while(!(LCD_Write(LCD_I_type, LCD_I_StartLn, LCD_Block_Left))) {
 	}
 while(!(LCD_Write(LCD_I_type, LCD_I_StartLn, LCD_Block_Right))) {
 	}
 
 // Set block left X & Yaddress to 0
 while(!(LCD_Write(LCD_I_type, LCD_I_Page0, LCD_Block_Left))) {
 	}
 while(!(LCD_Write(LCD_I_type, LCD_I_Y0, LCD_Block_Left))) {
 	}

 // Set block right X & Y address to 0
 while(!(LCD_Write(LCD_I_type, LCD_I_Page0, LCD_Block_Right))) {
 	}
 while(!(LCD_Write(LCD_I_type, LCD_I_Y0, LCD_Block_Right))) {
 	}


 // Transfer Block left & right data
 tmp1 = 0;
 while(tmp1 < 8) {
 	tmp2 = 0;
	while(tmp2 < 64) {
		while(!(LCD_Write(LCD_D_type, *ptmp, LCD_Block_Left))) {
			}
		ptmp++;
		tmp2++;
		}
 	tmp2 = 0;
	while(tmp2 < 64) {
		while(!(LCD_Write(LCD_D_type, *ptmp, LCD_Block_Right))) {
			}
		ptmp++;
		tmp2++;
		}
	 // Increase X address
	tmp1++;
 	while(!(LCD_Write(LCD_I_type, LCD_I_Page0|tmp1, LCD_Block_Left))) {
		}
 	while(!(LCD_Write(LCD_I_type, LCD_I_Page0|tmp1, LCD_Block_Right))) {
		}
	
 	}
// sei();
}

// 将一个字符写到（x,y）处
//
void	PutChar5x7(U8 x, U8 y, U8 ch)
{
 U8 tmp0, tmp1, tmp2, tmp4;
 U8 *ptmp, *pfont;
 U16	tmp3;

 // Byte address at DispBuf
 ptmp = DispBuf + (y >> 3) * Hmax + x;

 // Font address
 pfont = Font5X7 + (ch - 32) * 5;

 tmp0 = y & 0x07;
 if(tmp0 != 0) {
 	// Square located at position where x coordinate is not dividable by 8
 	tmp2 = 0xFF;
	tmp2 <<= tmp0;
	// Get font
	tmp1 = 0;
	while(tmp1 < 5) {
		tmp3 = pgm_read_byte(pfont);
		// Shift font to the right position
		tmp3 <<= tmp0;
		// Write font to DispBuf
		tmp4 = *ptmp;
		tmp4 &= ~tmp2;
		tmp4 |= (tmp3 & 0x00FF);
		*ptmp = tmp4;
		tmp4 = *(ptmp + Hmax);
		tmp4 &= tmp2;
		tmp4 |=  (tmp3 >> 8);
		*(ptmp + Hmax) = tmp4;
		ptmp++;
		pfont++;
		tmp1++;
		}
 	}
 else {
 	// Square located at position where x coordinate is dividable by 8
	// Get font
	tmp1 = 0;
	while(tmp1 < 5) {
		tmp3 = pgm_read_byte(pfont);
		// Write font to DispBuf
		tmp4 = *ptmp;
		tmp4 &= 0x80;
		tmp4 |= (tmp3 & 0x00FF);
		*ptmp = tmp4;
		ptmp++;
		pfont++;
		tmp1++;
		}
 	}

 
}

// 在(x,y)处写字符串（字符之间间隔CharSpcec定）
//
void	LcdPuts(U8 x, U8 y, U8* s) 
{
 while(*s != 0) {
	PutChar5x7(x, y, *s);
	s++;
	x += CharSpace;
 	}

}


void	Delay(volatile U16 count)
{
 while(count) {
 	count--;
 	}


}


void	LCD_Init(void)
{
 // Reset LCD panel
 BitClr8(LCD_RES_Port, (1<<LCD_RES_Bit));
 Delay(20000);

 BitSet8(LCD_RES_Port, (1<<LCD_RES_Bit));
 Delay(10000);

 // Set display ON
 while(!(LCD_Write(LCD_I_type, LCD_I_On, LCD_Block_Left))) {
 	}
 while(!(LCD_Write(LCD_I_type, LCD_I_On, LCD_Block_Right))) {
 	}


}

U8	LCD_Read(U8 type, U8 block)
{
 U8 tmp;

 // Select block
 if(block == LCD_Block_Left) {
 	// Left block
 	BitSet8(LCD_CS1_Port, (1<<LCD_CS1_Bit));
 	BitClr8(LCD_CS2_Port, (1<<LCD_CS2_Bit));
 	}
 else {
 	// Right block
 	BitSet8(LCD_CS2_Port, (1<<LCD_CS2_Bit));
 	BitClr8(LCD_CS1_Port, (1<<LCD_CS1_Bit));
 	} 	
 
 if(type == LCD_I_type) {
 	// Read status
	BitClr8(LCD_DI_Port, (1<<LCD_DI_Bit));
 	}
 else {
 	// Read data
	BitSet8(LCD_DI_Port, (1<<LCD_DI_Bit));
 	}
 
 	BitSet8(LCD_WR_Port, (1<<LCD_WR_Bit));

 	// Set E signal to HIGH
 	BitSet8(LCD_E_Port, (1<<LCD_E_Bit));
 
 	// Set Port A to input mode
 	LCD_Data_Dir = 0x00;
	// Delay
 	Delay(LCD_Delay);

 	// Read
 	tmp = LCD_Data_In;
//	tmp = pgm_read_byte((U8*)(BitRevrese + tmp));
 	Delay(LCD_Delay);

 	// Set Port A to output mode
 	LCD_Data_Dir = 0xFF;
 	BitClr8(LCD_CS1_Port, (1<<LCD_CS1_Bit));
	BitClr8(LCD_CS2_Port, (1<<LCD_CS2_Bit));
 	// Set E signal to LOW
 	BitClr8(LCD_E_Port, (1<<LCD_E_Bit));
 	Delay(LCD_Delay);

 	return(tmp);
 
}

U8	LCD_Write(U8 type, U8 data, U8 block)
{
 U8 tmp;
 
 // Check for status
 tmp = LCD_Read(LCD_I_type, block);
 if( tmp & 0x80) {
 	// LCD busy
 	return(0x00);
 	}
 
 // -- type = 0/1 for control/date
 // 
 if(type == LCD_I_type) {
 	// Write instruction
 	BitClr8(LCD_DI_Port, (1<<LCD_DI_Bit));
	BitClr8(LCD_WR_Port, (1<<LCD_WR_Bit));
 	}
 else {
 	// Write data
 	BitClr8(LCD_WR_Port, (1<<LCD_WR_Bit));
 	BitSet8(LCD_DI_Port, (1<<LCD_DI_Bit));
 	} 	

 // Select block
 if(block == LCD_Block_Left) {
 	// Left block
 	BitSet8(LCD_CS1_Port, (1<<LCD_CS1_Bit));
 	BitClr8(LCD_CS2_Port, (1<<LCD_CS2_Bit));
 	}
 else {
 	// Right block
 	BitSet8(LCD_CS2_Port, (1<<LCD_CS2_Bit));
 	BitClr8(LCD_CS1_Port, (1<<LCD_CS1_Bit));
 	} 	

 // Set E signal to HIGH
 BitSet8(LCD_E_Port, (1<<LCD_E_Bit));

// data =  pgm_read_byte((U8*)(BitRevrese + data));
 LCD_Data_Port = data;
 Delay(LCD_Delay);

// Delay(2);
 // Set E signal to LOW
 BitClr8(LCD_E_Port, (1<<LCD_E_Bit));
 BitClr8(LCD_CS1_Port, (1<<LCD_CS1_Bit));
 BitClr8(LCD_CS2_Port, (1<<LCD_CS2_Bit));
 Delay(LCD_Delay);

 return (0xFF);
	
}


