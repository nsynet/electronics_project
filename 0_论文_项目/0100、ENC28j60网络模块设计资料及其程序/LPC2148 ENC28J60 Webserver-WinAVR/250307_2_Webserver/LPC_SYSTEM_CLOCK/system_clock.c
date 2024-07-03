#include "system_clock.h"

/* 
    InitPLL
	
Phase locked loop
PLLCFG (SCB_PLLCFG)  	Das PLL Configuration Register hält den Multiplikator für die 
						interne PLL Schaltung. Diese ermöglicht es die Taktrate des 
						Prozessors zu erhöhen. Zum Beispiel: Ein angeschlossener Quarz 
						mit 10 MHz x 4 (mithilfe von PLL) = 40 MHz Prozessortakt. Bei 
						Bedarf kann auch ein Teiler eingestellt werden.  
PLLCON (SCB_PLLCON)  	Mit dem PLL Control Register kann der PLL aktiviert werden  
PLLSTAT (SCB_PLLSTAT)  	Im PLL Status Register werden Informationen bezüglich des PLL 
						gespeichert, z.B. der aktuelle Multiplikator-Wert etc.  
PLLFEED (SCB_PLLFEED)  	Damit ÿnderungen an PLLCON und PLLCFG übernommen werden muss 
						erst in dieses Register eine "Feed-Sequenz" geschrieben werden. 
						Die Feed-Sequenz wird im Codebeispiel weiter unten dargestellt  

*/
void initPLL(void) 
{

  PLL48CFG = (PLL_M-1)|((PLL_P-1)<<5); // M=4 und P=2 (multiplikatoren von 0 sind nicht erlaubt)
  
  /*
  Nochmal nach rechnen:
  CPU TAKT = PLL_M * FOSC = 4 * 12000000 Hz = 480000000 Hz
  CCO TAKT = 2 * PLL_P * PLL_M * FOSC = 2 * 2 * 4 * 12000000 Hz = 192000000 Hz
  
  F_CCO muss zwischen 156 MHz und 320 MHz liegen
 
  Mit diesen Werten ist alles innerhalb der Spezifikationen aus dem Datenblatt.  
 
  Der CCO (Current Controlled Oscillator) ist ein Bestandteil der PLL.
  */
 
  PLL48CON = 0x01; // PLL aktivieren
 
  PLL48FEED = 0xAA;  //PLL Feed-Sequenz
  PLL48FEED = 0x55;
  while ( !( PLL48STAT & PLOCK ) ); // Darauf warten, dass die ÿnderungen übernommen werden 
 
  /*
  Mit MAMTIM werden die Waitstates beim flash speicherzugriff eingestellt, das Datenblatt empfiehlt folgende Werte:
  1 - bei unter 20 Mhz
  2 - bei 20-40 Mhz 
  3 - bei über 40 Mhz
  */
    
  MAMTIM = 3; 
 
  PLL48CON = 0x03; 		// PLL aktivieren und mit dem internen Taktgeber verbinden
  PLL48FEED = 0xAA;  	// PLL Feed Sequence
  PLL48FEED = 0x55;
}

/*
VLSI Peripheral Bus
// Teiler auf 1 stellen; Prozessor-Takt=Peripherie-Takt
Die gesamte Peripherie ( SPI, UART, etc. ) des ARMs hängt am sogenannten "VLSI Peripheral Bus". 
Mithilfe des VPBDIV Registers kann man die Taktfrequenz dieses Buses einstellen. 

SCB_VPBDIV = 1; // Teiler auf 1 stellen; Prozessor-Takt=Peripherie-Takt Gebrauchen kann man das 
wenn man die gesamte Peripherie des Systems drosseln möchte. Der Peripherie-Takt kann ohne 
Probleme so schnell sein wie der Prozessor-Takt. 
*/
void VLSI_Peripheral_Bus(void)
{VPBDIV = VPBDIV_VAL;}





//PLL-USB Settings!!!




static void feed(void)
{
	PLLFEED = 0xAA;
	PLLFEED = 0x55;
}


void Init_PLL_USB(void)  
{
	
 
	// 				Setting the Phased Lock Loop (PLL)
	//               ----------------------------------
	//
	// Olimex LPC-P2148 has a 12.0000 mhz crystal
	//
	// We'd like the LPC2148 to run at 60 mhz (has to be an even multiple of crystal)
	// 
	// According to the Philips LPC2148 manual:   M = cclk / Fosc	where:	M    = PLL multiplier (bits 0-4 of PLLCFG)
	//																		cclk = 60000000 hz
	//																		Fosc = 12000000 hz
	//
	// Solving:	M = 60000000 / 12000000 = 5           
	//
	//			Note: M - 1 must be entered into bits 0-4 of PLLCFG (assign 4 to these bits)
	//
	//
	// The Current Controlled Oscilator (CCO) must operate in the range 156 mhz to 320 mhz
	//
	// According to the Philips LPC2148 manual:	Fcco = cclk * 2 * P    where:	Fcco = CCO frequency 
	//																			cclk = 60000000 hz
	//																			P = PLL divisor (bits 5-6 of PLLCFG)
	//
	// Solving:	Fcco = 60000000 * 2 * P
	//			P = 2  (trial value)
	//			Fcco = 60000000 * 2 * 2
	//			Fcc0 = 240000000 hz    (good choice for P since it's within the 156 mhz to 320 mhz range)
	//
	// From Table 22 (page 34) of Philips LPC2148 manual    P = 2, PLLCFG bits 5-6 = 1  (assign 1 to these bits)
	//
	// Finally:      PLLCFG = 0  01  00100  =  0x24
	//
	// Final note: to load PLLCFG register, we must use the 0xAA followed 0x55 write sequence to the PLLFEED register
	//             this is done in the short function feed() below
	//
   
	// Setting Multiplier and Divider values
  	PLLCFG = 0x24;
  	feed();
  
	// Enabling the PLL */	
	PLLCON = 0x1;
	feed();
  

  
	// Wait for the PLL to lock to set frequency
	while(!(PLLSTAT & 0x400)) ;
  
	// Connect the PLL as the clock source
	PLLCON = 0x3;
	feed();
  
	// Enabling MAM and setting number of clocks used for Flash memory fetch
	MAMTIM = 0x3;
	MAMCR = 0x2;
  
	// Setting peripheral Clock (pclk) to System Clock (cclk)
	VPBDIV = 0x1;
}




