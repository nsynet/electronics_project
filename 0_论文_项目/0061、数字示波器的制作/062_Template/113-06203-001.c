//////////////////////////////////////////////////////////////////////////////
//
// 	Filename:	113-06203-001.c
//	Version:		0.01
//	Data:		2008.08.31
//
//	Author:		Liu, Zemin
//	Company:	zmLAB
//
//-----------------------------------------------------------------------------
//
// 	Target: 		ATmega64
// 	Tool chain: 	WinAvr (GCC-AVR)
//
//	Function Descriptions: Project template based on 06200-00A PCB
//-----------------------------------------------------------------------------
//	Required files:
//
//-----------------------------------------------------------------------------
//
//	Notes:
//		1. Fuse programming configuration: 
//			Extended Byte: 	0b------11 (Non mega603/103 compatible mode)
//					Bit 1		- 1 = M64 mode, 0 = M103 mode
//					Bit 0		- 1 = WDT normal, 0 = WDT always ON
//			High Byte: 	0b11-00001 (CLKOP = 0 for crystal, EEPROM perserved)
//					Bit 7		- 1 = OCD disabled
//					Bit 6		- 1 = JTAG disabled
//					Bit 5		- 1 = SPI program disabled
//					bit 4		- Clock options. See datasheet
//					bit 3		- 1 = EEPROM not preserved, 0 = EEPROM preserved
//					bit [2:1]	- Boot size configration
//								Size				Boot loader Start (word address)
//							11 = 512 words		0x7E00
//							10 = 1024 words		0x7C00
//							01 = 2048 words		0x7800
//							00 = 4096 words		0x7000
//					bit 0		- 1 = Application Reset, 0 = Boot Loader Reset
//			Low Byte:	0b10101110 (Use external crystal osc [20MHz])
//					Bit 7		- BODLEVEL, 1 = 2.7V, 0 = 4.0V
//					Bit 6		- 1 = BOD disabled, 0 = BOD enabled
//					Bit [5:4]	- Start-up Time (see datasheet)
//					bit [3:0]	- Select clock source (See datasheet)
//
//
//-----------------------------------------------------------------------------
//	Revision History:
//		V0.01		2008.08.31
//		1.	First creadted based on 113-06201-040
//
///////////////////////////////////////////////////////////////////////////////

#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

#include "Common.h"
#include "Board.h"
#include "Screen.h"

int	main (void)
{
 // -- Initialize ports
 Port_Init();

 // -- Initialize misc registers
 Misc_Init();

// Timer_Init();
// Comp_Init();
// ADC_Init();
 
 // -- Initialize Uart 0
 Uart0_Init();
// u0putc('0');
 
 // -- Initialize Uart 1
// Uart1_Init();
// u1putc('1');

 uputs((U8 *)"COM0 OK", Uart0);

// sei();

 // Initialize LCD
 LCD_Init();

 // Clear display buffer
 ClearBuf();

// PutChar5x7(10, 10, 0x41);

 // Write a string to display buffer
 LcdPuts(10, 10, "Hello, world!");

 LcdPuts(10, 45, "WWW.JYETECH.COM");

 // Output display buffer to LCD
 Display();


 // Halt
 while(1) {
		}

 return (0);

}

