//////////////////////////////////////////////////////////////////////////////
//
// 	Filename:	Common.h
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

#ifndef Common_h

#define	Common_h

typedef	unsigned char		U8;
typedef	signed char		S8;
typedef	unsigned short int	U16;
typedef	signed short int	S16;
typedef	unsigned long		U32;
typedef	signed long		S32;

typedef	void	(*FuncPointer)(U8); 
typedef	void	(*CmdFuncPointer)(void); 

// -- Control debug code generation
//#define	_Debug_

// ============= Macro definitions ===========================
#define	BitSet8(word, bit_mask)		((word) |= (bit_mask))
#define	BitClr8(word, bit_mask)		((word) &= ~(bit_mask))
#define	BitTest8(word, bit_mask)		((word) & (bit_mask))


// ===========================================================
//	Function Prototype Declarations
// ===========================================================
//

#endif // Common_h 

