//********************************************************************************************
//
// File : enc28j60.c Microchip ENC28J60 Ethernet Interface Driver
//
//********************************************************************************************
//
// Copyright (C) 2007
//
// This program is free software; you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free Software
// Foundation; either version 2 of the License, or (at your option) any later
// version.
// This program is distributed in the hope that it will be useful, but
//
// WITHOUT ANY WARRANTY;
//
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
// PURPOSE. See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along with
// this program; if not, write to the Free Software Foundation, Inc., 51
// Franklin St, Fifth Floor, Boston, MA 02110, USA
//
// http://www.gnu.de/gpl-ger.html
//
//********************************************************************************************

#include "p24HJ64GP504.h"
#include "enc28j60.h"
#include "spi.h"

static unsigned char Enc28j60Bank;
static WORD_BYTES next_packet_ptr;

//*******************************************************************************************
//
// Function : icmp_send_request
// Description : Send ARP request packet to destination.
//
//*******************************************************************************************
unsigned char enc28j60ReadOp(unsigned char op, unsigned char address)
{
	unsigned char dat = 0; 
	// activate CS
	CSACTIVE;
	dat = op | (address & ADDR_MASK);
	SPI1_ReadWrite(dat);
	dat = SPI1_ReadWrite(0xFF);
	// do dummy read if needed (for mac and mii, see datasheet page 29)
	if(address & 0x80)
	    {
		dat = SPI1_ReadWrite(0xFF);
	    }
	// release CS
	CSPASSIVE;
	return dat;
}

//*******************************************************************************************
//
// Function : icmp_send_request
// Description : Send ARP request packet to destination.
//
//*******************************************************************************************
void enc28j60WriteOp(unsigned char op, unsigned char address, unsigned char data)
{
	unsigned char dat = 0; 
	// activate CS
	CSACTIVE;
	// issue write command
	dat = op | (address & ADDR_MASK);
	SPI1_ReadWrite(dat);
	// write data
	dat = data;
	SPI1_ReadWrite(dat);
	// release CS
	CSPASSIVE;
}

//*******************************************************************************************
//
// Function : icmp_send_request
// Description : Send ARP request packet to destination.
//
//*******************************************************************************************
void enc28j60SetBank(unsigned char address)
{
	// set the bank (if needed)
	if((address & BANK_MASK) != Enc28j60Bank)
	{
		// set the bank
		enc28j60WriteOp(ENC28J60_BIT_FIELD_CLR, ECON1, (ECON1_BSEL1|ECON1_BSEL0));
		enc28j60WriteOp(ENC28J60_BIT_FIELD_SET, ECON1, (address & BANK_MASK)>>5);
		Enc28j60Bank = (address & BANK_MASK);
	}
}

//*******************************************************************************************
//
// Function : icmp_send_request
// Description : Send ARP request packet to destination.
//
//*******************************************************************************************
unsigned char enc28j60Read(unsigned char address)
{
	// select bank to read
	enc28j60SetBank(address);
	
	// do the read
	return enc28j60ReadOp(ENC28J60_READ_CTRL_REG, address);
}

//*******************************************************************************************
//
// Function : icmp_send_request
// Description : Send ARP request packet to destination.
//
//*******************************************************************************************
void enc28j60Write(unsigned char address, unsigned char data)
{
	// select bank to write
	enc28j60SetBank(address);

	// do the write
	enc28j60WriteOp(ENC28J60_WRITE_CTRL_REG, address, data);
}

//*******************************************************************************************
//
// Function : icmp_send_request
// Description : Send ARP request packet to destination.
//
//*******************************************************************************************
WORD enc28j60_read_phyreg(unsigned char address)
{
	WORD data;
	
	// set the PHY register address
	enc28j60Write(MIREGADR, address);
	enc28j60Write(MICMD, MICMD_MIIRD);
	
	// Loop to wait until the PHY register has been read through the MII
	// This requires 10.24us
	while( (enc28j60Read(MISTAT) & MISTAT_BUSY) );
	
	// Stop reading
	enc28j60Write(MICMD, MICMD_MIIRD);
	
	// Obtain results and return
	data = enc28j60Read ( MIRDL );
	data |= enc28j60Read ( MIRDH );

	return data;
}

//*******************************************************************************************
//
// Function : icmp_send_request
// Description : Send ARP request packet to destination.
//
//*******************************************************************************************
void enc28j60PhyWrite(unsigned char address, WORD_BYTES data)
{
    int i;
	// set the PHY register address
	enc28j60Write(MIREGADR, address);
	// write the PHY data
	enc28j60Write(MIWRL, data.byte.low);
	enc28j60Write(MIWRH, data.byte.high);
	// wait until the PHY write completes
	while(enc28j60Read(MISTAT) & MISTAT_BUSY)
	{
		//_delay_us(15);
		for(i=0;i<1000;i++);
	}
}

//*******************************************************************************************
//
// Function : icmp_send_request
// Description : Send ARP request packet to destination.
//
//*******************************************************************************************
void enc28j60_init( unsigned char *pic_mac)
{
	unsigned int i;
	asm volatile ("MOV #OSCCON, w1 \n"       //端口重映射解锁
                  "MOV #0x46, w2 \n"
                  "MOV #0x57, w3 \n"
                  "MOV.b w2, [w1] \n"
                  "MOV.b w3, [w1] \n"
                  "BCLR OSCCON,#6");

	_SDI1R=21;   //RP21----SDI1
	_RP20R=7;    //RP20----SDO1 b00111
	_RP19R=8;    //RP19----SCK1 b01000
	
	asm volatile ("MOV #OSCCON, w1 \n"       //端口重映射锁定
	              "MOV #0x46, w2 \n"
	              "MOV #0x57, w3 \n"
	              "MOV.b w2, [w1] \n"
	              "MOV.b w3, [w1] \n"
	              "BSET OSCCON,#6");

    _TRISB5=0;
    _TRISB6=0;

    SPI1_Init();

    ENC28J60_RESET();
    for(i=0;i<10000;i++);
	ENC28J60_UNRESET();

	// perform system reset
	enc28j60WriteOp(ENC28J60_SOFT_RESET, 0, ENC28J60_SOFT_RESET);
	while(!(enc28j60Read(ESTAT) & ESTAT_CLKRDY));

	// check CLKRDY bit to see if reset is complete
	// The CLKRDY does not work. See Rev. B4 Silicon Errata point. Just wait.
	// while(!(enc28j60Read(ESTAT) & ESTAT_CLKRDY));
	// do bank 0 stuff
	// initialize receive buffer
	// 16-bit transfers, must write low byte first
	// set receive buffer start address
	next_packet_ptr.word = RXSTART_INIT;
    // Rx start
	enc28j60Write(ERXSTL, RXSTART_INIT&0xFF);
	enc28j60Write(ERXSTH, RXSTART_INIT>>8);
	// set receive pointer address
	enc28j60Write(ERXRDPTL, RXSTART_INIT&0xFF);
	enc28j60Write(ERXRDPTH, RXSTART_INIT>>8);
	// RX end
	enc28j60Write(ERXNDL, RXSTOP_INIT&0xFF);
	enc28j60Write(ERXNDH, RXSTOP_INIT>>8);
	// TX start
	enc28j60Write(ETXSTL, TXSTART_INIT&0xFF);
	enc28j60Write(ETXSTH, TXSTART_INIT>>8);
	// TX end
	enc28j60Write(ETXNDL, TXSTOP_INIT&0xFF);
	enc28j60Write(ETXNDH, TXSTOP_INIT>>8);

	// do bank 2 stuff
	// enable MAC receive
	enc28j60Write(MACON1, MACON1_MARXEN|MACON1_TXPAUS|MACON1_RXPAUS);
	
	// bring MAC out of reset
	//enc28j60Write(MACON2, 0x00);

	// enable automatic padding to 60bytes and CRC operations
	enc28j60Write(MACON3, MACON3_PADCFG0|MACON3_TXCRCEN|MACON3_FRMLNEN);

	// Allow infinite deferals if the medium is continuously busy 
    // (do not time out a transmission if the half duplex medium is 
    // completely saturated with other people's data)
    enc28j60Write(MACON4, MACON4_DEFER);

	// Late collisions occur beyond 63+8 bytes (8 bytes for preamble/start of frame delimiter)
	// 55 is all that is needed for IEEE 802.3, but ENC28J60 B5 errata for improper link pulse 
	// collisions will occur less often with a larger number.
    enc28j60Write(MACLCON2, 63);
	
	// Set non-back-to-back inter-packet gap to 9.6us.  The back-to-back 
	// inter-packet gap (MABBIPG) is set by MACSetDuplex() which is called 
	// later.
	enc28j60Write(MAIPGL, 0x12);
	enc28j60Write(MAIPGH, 0x0C);
	
	// Set the maximum packet size which the controller will accept
    // Do not send packets longer than MAX_FRAMELEN:
	enc28j60Write(MAMXFLL, MAX_FRAMELEN&0xFF);	
	enc28j60Write(MAMXFLH, MAX_FRAMELEN>>8);
	
	// do bank 3 stuff
    // write MAC address
	// NOTE: MAC address in ENC28J60 is byte-backward
	// ENC28J60 is big-endian avr gcc is little-endian
	enc28j60Write(MAADR5, pic_mac[0]);
	enc28j60Write(MAADR4, pic_mac[1]);
	enc28j60Write(MAADR3, pic_mac[2]);
	enc28j60Write(MAADR2, pic_mac[3]);
	enc28j60Write(MAADR1, pic_mac[4]);
	enc28j60Write(MAADR0, pic_mac[5]);
	
	// no loopback of transmitted frames
	enc28j60PhyWrite(PHCON2, (WORD_BYTES){PHCON2_HDLDIS});
	
	// Magjack leds configuration, see enc28j60 datasheet, page 11
	// 0x476 is PHLCON LEDA=links status, LEDB=receive/transmit
	// enc28j60PhyWrite(PHLCON,0b0000 0100 0111 00 10);
	enc28j60PhyWrite(PHLCON,(WORD_BYTES){0x0472});

	// do bank 1 stuff, packet filter:
	// For broadcast packets we allow only ARP packtets
	// All other packets should be unicast only for our mac (MAADR)
	//
	// The pattern to match on is therefore
	// Type     ETH.DST
	// ARP      BROADCAST
 	// 06 08 -- ff ff ff ff ff ff -> ip checksum for theses bytes=f7f9
	// in binary these poitions are:11 0000 0011 1111
	// This is hex 303F->EPMM0=0x3f,EPMM1=0x30
	enc28j60Write(ERXFCON, ERXFCON_UCEN|ERXFCON_CRCEN|ERXFCON_PMEN);
	enc28j60Write(EPMM0, 0x3f);
	enc28j60Write(EPMM1, 0x30);
	enc28j60Write(EPMCSL, 0xf9);
	enc28j60Write(EPMCSH, 0xf7);

	// set inter-frame gap (back-to-back)
	enc28j60Write(MABBIPG, 0x12);
	
	// switch to bank 0
	enc28j60SetBank(ECON1);

	// enable interrutps
	enc28j60WriteOp(ENC28J60_BIT_FIELD_SET, EIE, EIE_INTIE|EIE_PKTIE);

	// enable packet reception
	enc28j60WriteOp(ENC28J60_BIT_FIELD_SET, ECON1, ECON1_RXEN);

	//_delay_ms(20);
}

//*******************************************************************************************
//
// Function : enc28j60getrev
// Description : read the revision of the chip.
//
//*******************************************************************************************
unsigned char enc28j60getrev(void)
{
	return(enc28j60Read(EREVID));
}

//*******************************************************************************************
//
// Function : enc28j60_packet_send
// Description : Send packet to network.
//
//*******************************************************************************************
void enc28j60_packet_send ( unsigned char *buffer, WORD length )
{
	//Set the write pointer to start of transmit buffer area
	enc28j60Write(EWRPTL, LOW(TXSTART_INIT) );
	enc28j60Write(EWRPTH, HIGH(TXSTART_INIT) );

	// Set the TXND pointer to correspond to the packet size given
	enc28j60Write(ETXNDL, LOW((TXSTART_INIT+length)) );
	enc28j60Write(ETXNDH, HIGH((TXSTART_INIT+length)) );

	// write per-packet control byte (0x00 means use macon3 settings)
	enc28j60WriteOp(ENC28J60_WRITE_BUF_MEM, 0, 0x00);

	CSACTIVE;
	// issue write command
    SPI1_ReadWrite(ENC28J60_WRITE_BUF_MEM);
	while(length)
	{
		length--;
		// write data
		SPI1_ReadWrite(*buffer++); 
	}
	CSPASSIVE;
	
	// send the contents of the transmit buffer onto the network
	enc28j60WriteOp(ENC28J60_BIT_FIELD_SET, ECON1, ECON1_TXRTS);

	// Reset the transmit logic problem. See Rev. B4 Silicon Errata point 12.
	if( (enc28j60Read(EIR) & EIR_TXERIF) )
	{
		enc28j60WriteOp(ENC28J60_BIT_FIELD_CLR, ECON1, ECON1_TXRTS);
	}
}

//*******************************************************************************************
//
// Function : enc28j60_packet_receive
// Description : check received packet and return length of data
//
//*******************************************************************************************
//WORD data_length;
WORD enc28j60_packet_receive ( unsigned char *rxtx_buffer, unsigned int max_length )
{
	WORD_BYTES rx_status, data_length;
	
	// check if a packet has been received and buffered
	// if( !(enc28j60Read(EIR) & EIR_PKTIF) ){
	// The above does not work. See Rev. B4 Silicon Errata point 6.
	if( enc28j60Read(EPKTCNT) == 0 )
	{
		return 0;
	}

	// Set the read pointer to the start of the received packet
	enc28j60Write(ERDPTL, next_packet_ptr.bytes[0]);
	enc28j60Write(ERDPTH, next_packet_ptr.bytes[1]);

	// read the next packet pointer
	next_packet_ptr.bytes[0] = enc28j60ReadOp(ENC28J60_READ_BUF_MEM, 0);
	next_packet_ptr.bytes[1] = enc28j60ReadOp(ENC28J60_READ_BUF_MEM, 0);

	// read the packet length (see datasheet page 43)
	data_length.bytes[0] = enc28j60ReadOp(ENC28J60_READ_BUF_MEM, 0);
	data_length.bytes[1] = enc28j60ReadOp(ENC28J60_READ_BUF_MEM, 0);
	data_length.word -=4; //remove the CRC count
	
	// read the receive status (see datasheet page 43)
	rx_status.bytes[0] = enc28j60ReadOp(ENC28J60_READ_BUF_MEM, 0);
	rx_status.bytes[1] = enc28j60ReadOp(ENC28J60_READ_BUF_MEM, 0);
	
	if ( data_length.word > (max_length-1) )
	{
		data_length.word = max_length-1;
	}
	
	// check CRC and symbol errors (see datasheet page 44, table 7-3):
	// The ERXFCON.CRCEN is set by default. Normally we should not
	// need to check this.
	if ( (rx_status.word & 0x80)==0 )
	{
		// invalid
		data_length.word = 0;
	}
	else
	{
		// read data from rx buffer and save to rxtx_buffer
		rx_status.word = data_length.word;
		CSACTIVE;
		// issue read command
     	SPI1_ReadWrite(ENC28J60_READ_BUF_MEM);
		while(rx_status.word)
		{
			rx_status.word--;
			*rxtx_buffer++ = SPI1_ReadWrite(0); 
		}
		CSPASSIVE;
	}
	
	// Move the RX read pointer to the start of the next received packet
	// This frees the memory we just read out
	enc28j60Write(ERXRDPTL, next_packet_ptr.bytes[0]);
	enc28j60Write(ERXRDPTH, next_packet_ptr.bytes[1]);

	// decrement the packet counter indicate we are done with this packet
	enc28j60WriteOp(ENC28J60_BIT_FIELD_SET, ECON2, ECON2_PKTDEC);

	return( data_length.word );
}


