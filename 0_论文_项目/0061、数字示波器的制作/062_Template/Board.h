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
//

#ifndef	Board_h

#define	Board_h

#include	"Common.h"

// CPU frequency
//#define F_CPU      8000000L          	

// ===========================================
//	Uart parameters
//
#define	Uart0_Baud 	38400L
#define	Uart1_Baud 	38400L

#define	Uart0 	0
#define	Uart1	1


// ====================================================
//	Macros
//


// ====================================================
//	Function Prototype Declarations
//
void SYSCLK_Init (void);
void	SYS_Init(void);
void	Misc_Init(void);
void Port_Init(void);
void Timer_Init (void);
void Comp_Init(void);
void ADC_Init (void);
void DelayTimer(U16 ms);
void Uart0_Init (void);
void Uart1_Init (void);

void	u0putc(U8 ch);
void	u1putc(U8 ch);
U8	u0getc(void);
U8	u1getc(void);
void	uputs(U8 *s, U8 UartNum);
U8	u0rcheck(void);
U8	u1rcheck(void);
U8	u1rcheck_and_get(void);
void	UartTest(U8 UartNumber);




#endif	// Board_h


