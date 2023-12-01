//////////////////////////////////////////////////////////////////////////////
//
// 	Filename:	Board.h
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

#ifndef Screen_h

#define	Screen_h

#include	"Common.h"

// ------------ Display parameters -----------------------
// Overall display parameters
#define		Hmax			128					// Horizontal maximun points
#define		Vmax			64					// Vertical maximun points
#define		DBUFsize		((Hmax * Vmax)/8)

#define		CharSpace		6

// LCD definitions
// NOTE: TG12864D-04 bit order: LSB at top, MSB at bottom
#define	LCD_Data_Port	PORTC
#define	LCD_Data_Dir	DDRC
#define	LCD_Data_In	PINC
#define	LCD_WR_Port	PORTB
#define	LCD_WR_Bit		PB0
#define	LCD_CS1_Port	PORTG
#define	LCD_CS1_Bit		PG0
#define	LCD_CS2_Port	PORTG
#define	LCD_CS2_Bit		PG1
#define	LCD_RES_Port	PORTG
#define	LCD_RES_Bit		PG2
#define	LCD_DI_Port		PORTG
#define	LCD_DI_Bit		PG3
#define	LCD_E_Port		PORTG
#define	LCD_E_Bit		PG4

#define	LCD_Delay		500

#define	LCD_I_Page0	0b10111000
#define	LCD_I_Y0		0b01000000
#define	LCD_I_On		0b00111111
#define	LCD_I_StartLn	0b11000000

#define	LCD_I_type		0
#define	LCD_D_type		1

#define	LCD_Block_Left	0
#define	LCD_Block_Right 1


//	Display Panel
// =======================================================
//


// ===========================================================
//	Function Prototype Declarations
// ===========================================================
//
void	ClearBuf(void);
void	Display(void);
void	PutChar5x7(U8 x, U8 y, U8 ch);
void	LcdPuts(U8 x, U8 y, U8* s);
void	Delay(volatile U16 count);
void	LCD_Init(void);
U8	LCD_Read(U8 type, U8 block);
U8	LCD_Write(U8 type, U8 date, U8 block);

#endif

