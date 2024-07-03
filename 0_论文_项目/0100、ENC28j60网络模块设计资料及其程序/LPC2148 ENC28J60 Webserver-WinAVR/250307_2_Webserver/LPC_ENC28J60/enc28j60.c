/*! \file enc28j60.c \brief Microchip ENC28J60 Ethernet Interface Driver. */

#include "enc28j60.h"
//#include "lpc213x.h"
//#include "typedefs.h"
//#include "ssp_master.h"

U08 Enc28j60Bank;
U16 NextPacketPtr;

void delay(unsigned short us) 
{
    int a;
    for(a=0;a<100;a++);
} 

void nicSetMacAddress(U08* macaddr)
{
	// write MAC address
	// NOTE: MAC address in ENC28J60 is byte-backward
	enc28j60Write(MAADR5, *macaddr++);
	enc28j60Write(MAADR4, *macaddr++);
	enc28j60Write(MAADR3, *macaddr++);
	enc28j60Write(MAADR2, *macaddr++);
	enc28j60Write(MAADR1, *macaddr++);
	enc28j60Write(MAADR0, *macaddr++);
}

U08 enc28j60ReadOp(U08 op, U08 address)
{

    U08 data;
    // release CS
    //IOPIN0 &=~P0_21;
	SPI0_IOCLR = (1<<SPI0_CS);
   	delay(10000);
    
    S0SPDR = op | (address & ADDR_MASK);
    while(!(S0SPSR & SPIF)){};
	
    S0SPDR = 0x00;
    while(!(S0SPSR & SPIF)){};
    // do dummy read if needed
	if(address & 0x80)
	{
		S0SPDR = 0x00;
		while(!(S0SPSR & SPIF)){};
	}
	data = S0SPDR; 
    
    //IOPIN0 |= P0_21; //CS auf High
	SPI0_IOSET = (1<<SPI0_CS);
	return data;
}

void enc28j60WriteOp(U08 op, U08 address, U08 data)
{
    // release CS
    //IOPIN0 &= ~P0_21;
    SPI0_IOCLR = (1<<SPI0_CS);
	delay(10000);
    // issue write command
	S0SPDR = op | (address & ADDR_MASK);
	while(!(S0SPSR & SPIF)){};
	// write data
	S0SPDR = data;
	while(!(S0SPSR & SPIF)){};
    // release CS
    //IOPIN0 |= P0_21; //CS auf High
	SPI0_IOSET = (1<<SPI0_CS);
}

void enc28j60ReadBuffer(U16 len, U08* data)
{
	// assert CS
    //IOPIN0 &= ~P0_21;
    SPI0_IOCLR = (1<<SPI0_CS);
	delay(10000);
	// issue read command
	S0SPDR = ENC28J60_READ_BUF_MEM;
	while(!(S0SPSR & SPIF)){};
	while(len--)
	{
		// read data
		S0SPDR = 0x00;
		while(!(S0SPSR & SPIF)){};
		*data++ = S0SPDR;
	}	
	// release CS
	//IOPIN0 |= P0_21;
	SPI0_IOSET = (1<<SPI0_CS);
}

void enc28j60WriteBuffer(U16 len, U08* data)
{
	// assert CS
	//IOPIN0 &= ~P0_21; //CS auf Low
    SPI0_IOCLR = (1<<SPI0_CS);
	
	delay(10000);
	// issue write command
	S0SPDR = ENC28J60_WRITE_BUF_MEM;
	while(!(S0SPSR & SPIF)){};
	while(len--)
	{
		// write data
		S0SPDR = *data++;
		while(!(S0SPSR & SPIF)){};
	}	
	// release CS
	//IOPIN0 |= P0_21;
	SPI0_IOSET = (1<<SPI0_CS);
}

void enc28j60SetBank(U08 address)
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

U08 enc28j60Read(U08 address)
{
	// set the bank
	enc28j60SetBank(address);
	// do the read
	return enc28j60ReadOp(ENC28J60_READ_CTRL_REG, address);
}

void enc28j60Write(U08 address, U08 data)
{
	// set the bank
	enc28j60SetBank(address);
	// do the write
	enc28j60WriteOp(ENC28J60_WRITE_CTRL_REG, address, data);
}

void enc28j60PhyWrite(U08 address, U16 data)
{
	// set the PHY register address
	enc28j60Write(MIREGADR, address);
	
	// write the PHY data
	enc28j60Write(MIWRL, data);	
	enc28j60Write(MIWRH, data>>8);

	// wait until the PHY write completes
	while(enc28j60Read(MISTAT) & MISTAT_BUSY);
}

void enc28j60Init(void)
{
    U32 a;
    U32 timeout;
    // Port and SPI init
	/*
    IODIR0 |= P0_21; //Direction for CS Pin
    IOPIN0 |= P0_21; //CS auf High

	// SPI init
	PINSEL0 |= SPI0_IOSET_MASK;
	S0SPCCR = 8;        //low speed
	S0SPCR = SPI0_MSTR;  //Enable SPI, SPI in Master Mode	
    */
	SPIInit();
	
    do{
        timeout = 0;
        // perform system reset
        enc28j60WriteOp(ENC28J60_SOFT_RESET, 0, ENC28J60_SOFT_RESET);
        // check CLKRDY bit to see if reset is complete
        for(a=0;a<1000000;a++);
        while(!(enc28j60Read(ESTAT) & ESTAT_CLKRDY)){
            timeout++;
            if (timeout > 100000)
                break;
            };
    }while(timeout > 100000);

	// do bank 0 stuff
	// initialize receive buffer
	// 16-bit transfers, must write low byte first
	// set receive buffer start address
	NextPacketPtr = RXSTART_INIT;
	enc28j60Write(ERXSTL, RXSTART_INIT&0xFF);
	enc28j60Write(ERXSTH, RXSTART_INIT>>8);
	// set receive pointer address
	enc28j60Write(ERXRDPTL, RXSTART_INIT&0xFF);
	enc28j60Write(ERXRDPTH, RXSTART_INIT>>8);
	// set receive buffer end
	// ERXND defaults to 0x1FFF (end of ram)
	enc28j60Write(ERXNDL, RXSTOP_INIT&0xFF);
	enc28j60Write(ERXNDH, RXSTOP_INIT>>8);
	// set transmit buffer start
	// ETXST defaults to 0x0000 (beginnging of ram)
	enc28j60Write(ETXSTL, TXSTART_INIT&0xFF);
	enc28j60Write(ETXSTH, TXSTART_INIT>>8);

	// do bank 2 stuff
	// enable MAC receive
	enc28j60Write(MACON1, MACON1_MARXEN|MACON1_TXPAUS|MACON1_RXPAUS);
	// bring MAC out of reset
	enc28j60Write(MACON2, 0x00);
	// enable automatic padding and CRC operations
	enc28j60WriteOp(ENC28J60_BIT_FIELD_SET, MACON3, MACON3_PADCFG0|MACON3_TXCRCEN|MACON3_FRMLNEN);

	// set inter-frame gap (non-back-to-back)
	enc28j60Write(MAIPGL, 0x12);
	enc28j60Write(MAIPGH, 0x0C);
	// set inter-frame gap (back-to-back)
	enc28j60Write(MABBIPG, 0x12);
	// Set the maximum packet size which the controller will accept
	enc28j60Write(MAMXFLL, MAX_FRAMELEN&0xFF);	
	enc28j60Write(MAMXFLH, MAX_FRAMELEN>>8);

	// do bank 3 stuff
	// write MAC address
	// NOTE: MAC address in ENC28J60 is byte-backward
	enc28j60Write(MAADR5, ENC28J60_MAC0);
	enc28j60Write(MAADR4, ENC28J60_MAC1);
	enc28j60Write(MAADR3, ENC28J60_MAC2);
	enc28j60Write(MAADR2, ENC28J60_MAC3);
	enc28j60Write(MAADR1, ENC28J60_MAC4);
	enc28j60Write(MAADR0, ENC28J60_MAC5);

	// no loopback of transmitted frames
	enc28j60PhyWrite(PHCON2, PHCON2_HDLDIS);

	// switch to bank 0
	enc28j60SetBank(ECON1);
	// enable interrutps
	enc28j60WriteOp(ENC28J60_BIT_FIELD_SET, EIE, EIE_INTIE|EIE_PKTIE);
	// enable packet reception
	enc28j60WriteOp(ENC28J60_BIT_FIELD_SET, ECON1, ECON1_RXEN);

}

void enc28j60PacketSend(U16 len, U08* packet)
{
	// Set the write pointer to start of transmit buffer area
	enc28j60Write(EWRPTL, TXSTART_INIT);
	enc28j60Write(EWRPTH, TXSTART_INIT>>8);
	// Set the TXND pointer to correspond to the packet size given
	enc28j60Write(ETXNDL, (TXSTART_INIT+len));
	enc28j60Write(ETXNDH, (TXSTART_INIT+len)>>8);

	// write per-packet control byte
	enc28j60WriteOp(ENC28J60_WRITE_BUF_MEM, 0, 0x00);

	// copy the packet into the transmit buffer
	enc28j60WriteBuffer(len, packet);
	
	// send the contents of the transmit buffer onto the network
	enc28j60WriteOp(ENC28J60_BIT_FIELD_SET, ECON1, ECON1_TXRTS);
}

unsigned int enc28j60PacketReceive(U16 maxlen, U08* packet)
{
	U16 rxstat;
	U16 len;
    U32 timeout = 0;

	// check if a packet has been received and buffered
	if( !(enc28j60Read(EIR) & EIR_PKTIF) || (timeout++ > 100000))
        {
		return 0;
        }
        
	// Set the read pointer to the start of the received packet
	enc28j60Write(ERDPTL, (NextPacketPtr));
	enc28j60Write(ERDPTH, (NextPacketPtr)>>8);
	// read the next packet pointer
	NextPacketPtr  = enc28j60ReadOp(ENC28J60_READ_BUF_MEM, 0);
	NextPacketPtr |= enc28j60ReadOp(ENC28J60_READ_BUF_MEM, 0)<<8;
	// read the packet length
	len  = enc28j60ReadOp(ENC28J60_READ_BUF_MEM, 0);
	len |= enc28j60ReadOp(ENC28J60_READ_BUF_MEM, 0)<<8;
	// read the receive status
	rxstat  = enc28j60ReadOp(ENC28J60_READ_BUF_MEM, 0);
	rxstat |= enc28j60ReadOp(ENC28J60_READ_BUF_MEM, 0)<<8;

	// limit retrieve length
	// (we reduce the MAC-reported length by 4 to remove the CRC)
	len = IFMIN(len, maxlen);

	// copy the packet from the receive buffer
	enc28j60ReadBuffer(len, packet);

	// Move the RX read pointer to the start of the next received packet
	// This frees the memory we just read out
	enc28j60Write(ERXRDPTL, (NextPacketPtr));
	enc28j60Write(ERXRDPTH, (NextPacketPtr)>>8);

	// decrement the packet counter indicate we are done with this packet
	enc28j60WriteOp(ENC28J60_BIT_FIELD_SET, ECON2, ECON2_PKTDEC);

	return len;
}




