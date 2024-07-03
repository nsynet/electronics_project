/*==========================================================
//ENC28J60芯片底层驱动，使用avr m16芯片，硬件SPI连接
//http://www.embed.org.cn		hanembed@126.com
==========================================================*/

#include "board.h"
#include "reg.h"
#include "ne2000.h"

union NetNode myNode;
union netcard rxdnetbuf;
union netcard txdnetbuf;

unsigned char Enc28j60Bank;
unsigned int NextPacketPtr;

void DelayIni(void)
{
	TCCR0 = 0x00; //stop
	TCNT0 = 0xE1; //set count
}

void delay_ms(unsigned char ms)
{
	TCCR0 = 0x04; //start timer
	while(ms--)
	{
		while((TIFR & (1<<TOV0)) != (1<<TOV0));
		TIFR |= (1<<TOV0);
		TCNT0 = 0xE1;
	}
	TCCR0 = 0x00; //stop
}

void portIni(void)
{
	PORTB = MSK_ENJ;				//high
	DDRB = MSK_ENJ;					//port out
	PORTB = MSK_ENJ;				//high
	SPCR = (1 << SPE) | (1 << MSTR);
	SPSR = (1 << SPI2X);			//1/2 MCK
	DelayIni();
}

/*==========================================================
//	function:	write data to add
//	Parameter:	op			command code
//				add			parameter regsiter address
//				data		send data
//	return:		void
//	date:		2006/11/26
//	note:		datasheet page 28
//	write:		han
==========================================================*/
void encWriteOp(unsigned char op,unsigned char add,unsigned char data)
{
	PORTB &= ~CS_ENJ;

	// send write command
	SPDR = op | (add & ADDR_MASK);
	while(!(SPSR & (1<<SPIF)));
	// write data
	SPDR = data;
	while(!(SPSR & (1<<SPIF)));

	PORTB |= CS_ENJ;
}

/*==========================================================
//	function:	read data from add
//	Parameter:	op			command code
//				add			parameter regsiter address
//	return:		unsigned char	read data
//	date:		2006/11/26
//	note:		datasheet page 28
//	write:		han
==========================================================*/
unsigned char encReadOp(unsigned char op,unsigned char add)
{
	unsigned char data;
   
	PORTB &= ~CS_ENJ;
	
	// send read command
	SPDR = op | (add & ADDR_MASK);
	while(!(SPSR & (1<<SPIF)));
	// read data
	SPDR = 0x00;
	while(!(SPSR & (1<<SPIF)));
	// do dummy read if needed
	if(add & 0x80)
	{
		SPDR = 0x00;
		while(!(SPSR & (1<<SPIF)));
	}
	data = SPDR;
	
	PORTB |= CS_ENJ;

	return data;
}

/*==========================================================
//	function:	read data buffer form enc28j60
//	Parameter:	len			read size
//				*data		data buffer pointer
//	return:		void
//	date:		2006/11/26
//	note:		none
//	write:		han
==========================================================*/
void encReadBuffer(unsigned int len,unsigned char *data)
{
	PORTB &= ~CS_ENJ;
	
	// send read command
	SPDR = ENC28J60_READ_BUF_MEM;
	while(!(SPSR & (1<<SPIF)));
	while(len--)
	{
		// read data
		SPDR = 0x00;
		while(!(SPSR & (1<<SPIF)));
		*data++ = SPDR;
	}
		
	PORTB |= CS_ENJ;
}

/*==========================================================
//	function:	write data to enc28j60
//	Parameter:	len			write size
//				*data		data buffer pointer
//	return:		void
//	date:		2006/11/26
//	note:		none
//	write:		han
==========================================================*/
void encWriteBuffer(unsigned int len,unsigned char *data)
{
	PORTB &= ~CS_ENJ;
	
	// send write command
	SPDR = ENC28J60_WRITE_BUF_MEM;
	while(!(SPSR & (1<<SPIF)));
	while(len--)
	{
		// write data
		SPDR = *data++;
		while(!(SPSR & (1<<SPIF)));
	}
		
	PORTB |= CS_ENJ;
}

/*==========================================================
//	function:	set enc28j60 register bank
//	Parameter:	add			register address
//	return:		void
//	date:		2006/11/27
//	note:		datasheet page 17
//	write:		han
==========================================================*/
void encSetBank(unsigned char add)
{
	// set the bank (if needed)
	if((add & BANK_MASK) != Enc28j60Bank)
	{
		// clear the set
		encWriteOp(ENC28J60_BIT_FIELD_CLR, ECON1, (ECON1_BSEL1|ECON1_BSEL0));
		// set the bank
		encWriteOp(ENC28J60_BIT_FIELD_SET, ECON1, (add & BANK_MASK)>>5);
		Enc28j60Bank = (add & BANK_MASK);
	}
}

/*==========================================================
//	function:	read regsiter
//	Parameter:	add			register address
//	return:		unsigned char	return data
//	date:		2006/11/27
//	note:		datasheet page 28
//	write:		han
==========================================================*/
unsigned char encRead(unsigned char add)
{
	// set the bank
	encSetBank(add);
	// do the read
	return encReadOp(ENC28J60_READ_CTRL_REG, add);
}

/*==========================================================
//	function:	write regsiter
//	Parameter:	add			register address
//				data		data to write
//	return:		void
//	date:		2006/11/27
//	note:		datasheet page 28
//	write:		han
==========================================================*/
void encWrite(unsigned char add,unsigned char data)
{
	// set the bank
	encSetBank(add);
	// do the write
	encWriteOp(ENC28J60_WRITE_CTRL_REG, add, data);
}

/*==========================================================
//	function:	set MAC address
//	Parameter:	void		data in myNode
//	return:		void
//	date:		2006/11/26
//	note:		datasheet page 28
//	write:		han
==========================================================*/
void nicSetMacAddress(void)
{
	// write MAC address
	// NOTE: MAC address in ENC28J60 is byte-backward
	encWrite(MAADR0, myNode.node.mac[5]);
	encWrite(MAADR1, myNode.node.mac[4]);
	encWrite(MAADR2, myNode.node.mac[3]);
	encWrite(MAADR3, myNode.node.mac[2]);
	encWrite(MAADR4, myNode.node.mac[1]);
	encWrite(MAADR5, myNode.node.mac[0]);
}

/*==========================================================
//	function:	write PHY regsiter
//	Parameter:	add			register address
//				data		data to write
//	return:		void
//	date:		2006/11/27
//	note:		datasheet page 21
//	write:		han
==========================================================*/
void encPhyWrite(unsigned char add, unsigned int data)
{
	// set the PHY register address
	encWrite(MIREGADR, add);
	
	// write the PHY data
	encWrite(MIWRL, data);	
	encWrite(MIWRH, data>>8);

	// wait until the PHY write completes
	while(encRead(MISTAT) & MISTAT_BUSY);
}

/*==========================================================
//	function:	chip init
//	Parameter:	void
//	return:		void
//	date:		2006/11/27
//	note:		datasheet page 35
//	write:		han
==========================================================*/
void encInit(void)
{
	// initialize I/O

	portIni();	

	// perform system reset
	encWriteOp(ENC28J60_SOFT_RESET, 0, ENC28J60_SOFT_RESET);
	// check CLKRDY bit to see if reset is complete
	delay_ms(10);
	while(!(encRead(ESTAT) & ESTAT_CLKRDY));

	// do bank 0 stuff
	// initialize receive buffer
	// 16-bit transfers, must write low byte first
	// set receive buffer start address
	NextPacketPtr = RXSTART_INIT;
	encWrite(ERXSTL, RXSTART_INIT&0xFF);
	encWrite(ERXSTH, RXSTART_INIT>>8);
	// set receive pointer address
	encWrite(ERXRDPTL, RXSTART_INIT&0xFF);
	encWrite(ERXRDPTH, RXSTART_INIT>>8);
	// set receive buffer end
	// ERXND defaults to 0x1FFF (end of ram)
	encWrite(ERXNDL, RXSTOP_INIT&0xFF);
	encWrite(ERXNDH, RXSTOP_INIT>>8);
	// set transmit buffer start
	// ETXST defaults to 0x0000 (beginnging of ram)
	encWrite(ETXSTL, TXSTART_INIT&0xFF);
	encWrite(ETXSTH, TXSTART_INIT>>8);

	// do bank 2 stuff
	// enable MAC receive
	encWrite(MACON1, MACON1_MARXEN|MACON1_TXPAUS|MACON1_RXPAUS);
	// bring MAC out of reset
	encWrite(MACON2, 0x00);
	// enable automatic padding and CRC operations
	encWriteOp(ENC28J60_BIT_FIELD_SET, MACON3, MACON3_PADCFG0|MACON3_TXCRCEN|MACON3_FRMLNEN);

	// set inter-frame gap (non-back-to-back)
	encWrite(MAIPGL, 0x12);
	encWrite(MAIPGH, 0x0C);
	// set inter-frame gap (back-to-back)
	encWrite(MABBIPG, 0x12);
	// Set the maximum packet size which the controller will accept
	encWrite(MAMXFLL, MAX_FRAMELEN&0xFF);	
	encWrite(MAMXFLH, MAX_FRAMELEN>>8);

	// do bank 3 stuff
	// write MAC address
	// NOTE: MAC address in ENC28J60 is byte-backward
	nicSetMacAddress();

	// no loopback of transmitted frames
	encPhyWrite(PHCON2, PHCON2_HDLDIS);

	// switch to bank 0
	encSetBank(ECON1);
	// enable interrutps
	encWriteOp(ENC28J60_BIT_FIELD_SET, EIE, EIE_INTIE|EIE_PKTIE);
	// enable packet reception
	encWriteOp(ENC28J60_BIT_FIELD_SET, ECON1, ECON1_RXEN);

}

void encPacketSend(union ethernet_address_type *pDestAddr, union netcard *txdnet, 
                  unsigned int buffer_length, unsigned int packet_type)
{
	unsigned char i;
	for(i = 0; i < 6; i++)
	{
    	txdnet->etherframe.sourcenodeid[i] = myNode.node.mac[i];
   	}
	// Step 3b: Load the destination address
    for(i = 0; i < 6; i++)
    {
    	txdnet->etherframe.destnodeid[i] = pDestAddr->bytes[i];
    }
    txdnet->etherframe.protocal = packet_type;

	// Set the write pointer to start of transmit buffer area
	encWrite(EWRPTL, TXSTART_INIT);
	encWrite(EWRPTH, TXSTART_INIT>>8);
	// Set the TXND pointer to correspond to the packet size given
	encWrite(ETXNDL, (TXSTART_INIT+buffer_length));
	encWrite(ETXNDH, (TXSTART_INIT+buffer_length)>>8);

	// write per-packet control byte
	encWriteOp(ENC28J60_WRITE_BUF_MEM, 0, 0x00);

	// copy the packet into the transmit buffer
	encWriteBuffer(buffer_length, txdnet->bytedata.bytebuf+4);
	
	// send the contents of the transmit buffer onto the network
	encWriteOp(ENC28J60_BIT_FIELD_SET, ECON1, ECON1_TXRTS);
}

unsigned int encPacketReceive(union netcard *rxdnet)
{
	unsigned int rxstat;
	unsigned int len;

	// check if a packet has been received and buffered
	if( !(encRead(EIR) & EIR_PKTIF) )
		return 0;
	
	// Make absolutely certain that any previous packet was discarded	
	//if( WasDiscarded == FALSE)
	//	MACDiscardRx();

	// Set the read pointer to the start of the received packet
	encWrite(ERDPTL, (NextPacketPtr));
	encWrite(ERDPTH, (NextPacketPtr)>>8);
	// read the next packet pointer
	NextPacketPtr  = encReadOp(ENC28J60_READ_BUF_MEM, 0);
	NextPacketPtr |= encReadOp(ENC28J60_READ_BUF_MEM, 0)<<8;
	// read the packet length
	len  = encReadOp(ENC28J60_READ_BUF_MEM, 0);
	len |= encReadOp(ENC28J60_READ_BUF_MEM, 0)<<8;
	// read the receive status
	rxstat  = encReadOp(ENC28J60_READ_BUF_MEM, 0);
	rxstat |= encReadOp(ENC28J60_READ_BUF_MEM, 0)<<8;

	// limit retrieve length
	// (we reduce the MAC-reported length by 4 to remove the CRC)
	if(len > 336)
		len = 336;

	// copy the packet from the receive buffer
	encReadBuffer(len, rxdnet->bytedata.bytebuf+4);

	// Move the RX read pointer to the start of the next received packet
	// This frees the memory we just read out
	encWrite(ERXRDPTL, (NextPacketPtr));
	encWrite(ERXRDPTH, (NextPacketPtr)>>8);

	// decrement the packet counter indicate we are done with this packet
	encWriteOp(ENC28J60_BIT_FIELD_SET, ECON2, ECON2_PKTDEC);

	return len;
}
