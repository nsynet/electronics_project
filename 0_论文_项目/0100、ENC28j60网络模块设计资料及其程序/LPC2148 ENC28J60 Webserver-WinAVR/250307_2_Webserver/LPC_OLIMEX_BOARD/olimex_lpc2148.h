#ifndef __OLIMEX_LPC2148_H
#define __OLIMEX_LPC2148_H

#define nop()	asm volatile("nop")
#define NOP()	asm volatile("nop")

//LEDs
#define LED_IODIR	IODIR0
#define LED_IOSET	IOSET0
#define LED_IOCLR	IOCLR0
#define LED_IOPIN	IOPIN0
#define LED1		10
#define	LED2		11

//Buttons
#define B_IODIR		IODIR0
#define B_IOSET		IOSET0
#define B_IOCLR		IOCLR0
#define B_IOPIN		IOPIN0
#define B1			15
#define	B2			16

//Buzzer
#define	BUZZ_IODIR	IODIR0
#define	BUZZ_IOSET	IOSET0
#define	BUZZ_IOCLR	IOCLR0
#define BUZZ_IOPIN	IOPIN0
#define	BUZZ1		12
#define	BUZZ2		13

//MMC-SD-Karte
#define	MMC_IODIR	IODIR1
#define	MMC_IOSET	IOSET1
#define	MMC_IOCLR	IOCLR1
#define MMC_IOPIN	IOPIN1
#define	MMC_SWITCH		25

//SPI0
#define SPI0_IODIR	IODIR0
#define SPI0_IOPIN	IOPIN0
#define SPI0_IOSET	IOSET0
#define SPI0_IOCLR	IOCLR0
#define SPI0_CLK	4
#define SPI0_MISO	5
#define SPI0_MOSI	6
#define SPI0_CS		7

#endif

	


