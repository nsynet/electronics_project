/*****************************************************************************
 *   spi.c:  SPI C file for Philips LPC214x Family Microprocessors
 *
 *   Copyright(C) 2006, Philips Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2005.10.01  ver 1.00    Prelimnary version, first Release
 *
*****************************************************************************/

#include "spi_polling.h"


/*****************************************************************************
** Function name:		SPIInit
**
** Descriptions:		SPI port initialization routine
**				
** parameters:			None
** Returned value:		None
** 
*****************************************************************************/
void SPIInit( void )
{

    S0SPCR = 0x00;
    PINSEL0 &= 0xFFFF00FF;
	//PINSEL0 |= 0x00005500;
    PINSEL0 |= 0x00001500;		//SSEL0 als normaler IO-PORT
    IODIR0 |= SPI0_SEL;
    IOSET0 = SPI0_SEL;

    //SPI-Clock
    S0SPCCR = 0x08;		//SPI_CLK = PCLK / S0SPCCR
						//60/8 = 7,5 -> nahe dran an 8Mhz...?!?

    S0SPCR = SPI0_MSTR;

	//Zur sicherheit nochmal...
    IODIR0 |= SPI0_SEL;
    IOSET0 = SPI0_SEL;

    return;
}

/*****************************************************************************
** Function name:		SPISend
**
** Descriptions:		Send one byte over spi0
**
** parameters:			byte to send
** Returned value:		None
** 
*****************************************************************************/

void SPISend( BYTE buf )
{
BYTE Dummy;

S0SPDR = buf;

while ( !(S0SPSR & SPIF) );

Dummy = S0SPDR;		// Dummy read
		
return; 
}

/*****************************************************************************
** Function name:		SPIReceiveByte
**
** Descriptions:		Receive one byte of data from the SPI port
**				Write a dummy byte, wait until SPI transfer
**				complete, then, read the data register to
**				get the SPI data.
**
** parameters:			None
** Returned value:		the data byte received
** 
*****************************************************************************/
BYTE SPIReceive( void )
{
    BYTE data;

    /* wrtie dummy byte out to generate clock, then read data from MISO */
    S0SPDR = 0xFF;
    /* Wait for transfer complete, SPIF bit set */
    while ( !(S0SPSR & SPIF) );
		data = S0SPDR;
    
	return ( data ); 
}

/******************************************************************************
**                            End Of File
******************************************************************************/

