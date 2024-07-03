#ifndef __SYSTEM_CLOCK_H
#define __SYSTEM_CLOCK_H

#define FOSC	12000000			// Die Frequenz der Taktquelle
#define PLL_M	4					// Der Multiplikator für den CPU-Takt
#define PLL_P	2					// Der Teiler für F_CCO (muss zwischen 156 MHz und 320 MHz liegen)
 
#define VPBDIV_VAL 1 // Teiler des Peripherie-Takts 

#define CCLK	(PLL_M * FOSC) 		//Die CPU frequenz nochmal als Zahlenwert definiert
 
// Das PLOCK Bit im PLLSTAT register gibt an, ob der PLL auf die konfigurierte  Frequenz eingestellt ist
#define PLOCK (1<<10)		//-> siehe C-File


/* System configuration: Fosc, Fcclk, Fcco, Fpclk must be defined */
/* Crystal frequence,10MHz~25MHz should be the same as actual status. */
//#define Fosc	12000000

/* System frequence,should be (1~32)multiples of Fosc,and should be equal or 
less than 60MHz. */
#define Fcclk	(Fosc * 5)

/* CCO frequence,should be 2/4/8/16 multiples of Fcclk, ranged from 156MHz to 
320MHz. */
#define Fcco	(Fcclk * 4)

/* VPB clock frequence , must be 1/2/4 multiples of (Fcclk / 4). */
#define Fpclk	(Fcclk / 4) * 1



void initPLL(void);
void VLSI_Peripheral_Bus(void);
void Init_PLL_USB(void);
static void feed(void);

#endif

