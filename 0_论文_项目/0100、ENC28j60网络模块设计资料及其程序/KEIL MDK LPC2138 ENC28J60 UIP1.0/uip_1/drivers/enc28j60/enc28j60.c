/*********************************************
 * vim:sw=8:ts=8:si:et
 * To use the above modeline in vim you must have "set modeline" in your .vimrc
 * Author: Guido Socher 
 * Copyright: GPL V2
 * http://www.gnu.org/licenses/gpl.html
 *
 * Based on the enc28j60.c file from the AVRlib library by Pascal Stang.
 * For AVRlib See http://www.procyonengineering.com/
 * Used with explicit permission of Pascal Stang.
 *
 * Title: Microchip ENC28J60 Ethernet Interface Driver
 * Chip type           : ATMEGA88 with ENC28J60
 *********************************************/

#include <LPC213x.H>
#include "enc28j60.h"
#include "enc28j60_hal.h"

static uint8_t enc28j60_bank;
static uint16_t next_packet_ptr;

void _delay_us(uint32_t us)
{
	uint32_t i;
	for (;us > 0; us --)
		for (i = 0; i < 20; i++ );
}

void _delay_ms(uint32_t ms) 
{
	uint32_t i;
	for (;ms > 0; ms --)
		for (i = 0; i < 100; i++ );
}

void enc28j60_set_bank(uint8_t address)
{
        // set the bank (if needed)
        if((address & BANK_MASK) != enc28j60_bank)
        {
                // set the bank
                enc28j60_write_op(ENC28J60_BIT_FIELD_CLR, ECON1, (ECON1_BSEL1|ECON1_BSEL0));
                enc28j60_write_op(ENC28J60_BIT_FIELD_SET, ECON1, (address & BANK_MASK)>>5);
                enc28j60_bank = (address & BANK_MASK);
        }
}

uint8_t enc28j60_read(uint8_t address)
{
        // set the bank
        enc28j60_set_bank(address);
        // do the read
        return enc28j60_read_op(ENC28J60_READ_CTRL_REG, address);
}

void enc28j60_write(uint8_t address, uint8_t __data)
{
        // set the bank
        enc28j60_set_bank(address);
        // do the write
        enc28j60_write_op(ENC28J60_WRITE_CTRL_REG, address, __data);
}

// read upper 8 bits
uint32_t enc28j60_phy_read_H(uint8_t address)
{

	// Set the right address and start the register read operation
	enc28j60_write(MIREGADR, address);
	enc28j60_write(MICMD, MICMD_MIIRD);
    _delay_us(15);

	// wait until the PHY read completes
	while(enc28j60_read(MISTAT) & MISTAT_BUSY);

	// reset reading bit
	enc28j60_write(MICMD, 0x00);
	
	return (enc28j60_read(MIRDH));
}



void enc28j60_phy_write(uint8_t address, uint16_t __data)
{
        // set the PHY register address
        enc28j60_write(MIREGADR, address);
        // write the PHY data
        enc28j60_write(MIWRL, __data);
        enc28j60_write(MIWRH, __data>>8);
        // wait until the PHY write completes
        while(enc28j60_read(MISTAT) & MISTAT_BUSY)
        {
                _delay_us(15);
        }
}

void enc28j60_clk_out(uint8_t clk)
{
        //setup clkout: 2 is 12.5MHz:
	enc28j60_write(ECOCON, clk & 0x7);
}

void enc28j60_init(uint8_t* macaddr)
{
	// initialize I/O
        // ss as output:

    enc28j60_IO_init();

	//IO0SET = LCD_CS;
	//CSPASSIVE; // ss=0
    ENC28J60_DISABLED;//��ֹENC28J60

	//IO1CLR = (1<<ENC28J60_CONTROL_RESET);
	//IO1CLR = 0x01000000;

    //��λENC28J60
    clr_ENC28J60_RESET;
	_delay_ms(200);
	//IO1SET = (1<<ENC28J60_CONTROL_RESET);
	//IO1SET = 0x01000000;
    set_ENC28J60_RESET;

	PINSEL0 |= 0x1500;
	S0SPCCR = 0x0E;  //0a 6.6m 0c 5.5m 0e 4.7m
	S0SPCR = 0x20;

	_delay_ms(400);

/*        //	
	DDRB  |= 1<<PB3 | 1<<PB5; // mosi, sck output
	cbi(DDRB,PINB4); // MISO is input
        //
        cbi(PORTB,PB3); // MOSI low
        cbi(PORTB,PB5); // SCK low
	//
	// initialize SPI interface
	// master mode and Fosc/2 clock:
        SPCR = (1<<SPE)|(1<<MSTR);
        SPSR |= (1<<SPI2X);
*/
	// perform system reset
	enc28j60_write_op(ENC28J60_SOFT_RESET, 0, ENC28J60_SOFT_RESET);
	_delay_ms(50);
	// check CLKRDY bit to see if reset is complete
        // The CLKRDY does not work. See Rev. B4 Silicon Errata point. Just wait.
	//while(!(enc28j60Read(ESTAT) & ESTAT_CLKRDY));
	// do bank 0 stuff
	// initialize receive buffer
	// 16-bit transfers, must write low byte first
	// set receive buffer start address
	next_packet_ptr = RXSTART_INIT;
        // Rx start
	enc28j60_write(ERXSTL, RXSTART_INIT&0xFF);
	enc28j60_write(ERXSTH, RXSTART_INIT>>8);
	// set receive pointer address
	enc28j60_write(ERXRDPTL, RXSTART_INIT&0xFF);
	enc28j60_write(ERXRDPTH, RXSTART_INIT>>8);
	// RX end
	enc28j60_write(ERXNDL, RXSTOP_INIT&0xFF);
	enc28j60_write(ERXNDH, RXSTOP_INIT>>8);
	// TX start
	enc28j60_write(ETXSTL, TXSTART_INIT&0xFF);
	enc28j60_write(ETXSTH, TXSTART_INIT>>8);
	// TX end
	enc28j60_write(ETXNDL, TXSTOP_INIT&0xFF);
	enc28j60_write(ETXNDH, TXSTOP_INIT>>8);
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
	enc28j60_write(ERXFCON, ERXFCON_UCEN|ERXFCON_CRCEN|ERXFCON_PMEN);
	enc28j60_write(EPMM0, 0x3f);
	enc28j60_write(EPMM1, 0x30);
	enc28j60_write(EPMCSL, 0xf9);
	enc28j60_write(EPMCSH, 0xf7);
        //
        //
	// do bank 2 stuff
	// enable MAC receive
	enc28j60_write(MACON1, MACON1_MARXEN|MACON1_TXPAUS|MACON1_RXPAUS);
	// bring MAC out of reset
	enc28j60_write(MACON2, 0x00);
	// enable automatic padding to 60bytes and CRC operations
	enc28j60_write_op(ENC28J60_BIT_FIELD_SET, MACON3, MACON3_PADCFG0|MACON3_TXCRCEN|MACON3_FRMLNEN);
	// set inter-frame gap (non-back-to-back)
	enc28j60_write(MAIPGL, 0x12);
	enc28j60_write(MAIPGH, 0x0C);
	// set inter-frame gap (back-to-back)
	enc28j60_write(MABBIPG, 0x12);
	// Set the maximum packet size which the controller will accept
        // Do not send packets longer than MAX_FRAMELEN:
	enc28j60_write(MAMXFLL, MAX_FRAMELEN&0xFF);	
	enc28j60_write(MAMXFLH, MAX_FRAMELEN>>8);
	// do bank 3 stuff
        // write MAC address
        // NOTE: MAC address in ENC28J60 is byte-backward
        enc28j60_write(MAADR5, macaddr[0]);
        enc28j60_write(MAADR4, macaddr[1]);
        enc28j60_write(MAADR3, macaddr[2]);
        enc28j60_write(MAADR2, macaddr[3]);
        enc28j60_write(MAADR1, macaddr[4]);
        enc28j60_write(MAADR0, macaddr[5]);
	// no loopback of transmitted frames
	enc28j60_phy_write(PHCON2, PHCON2_HDLDIS);
	// switch to bank 0
	enc28j60_set_bank(ECON1);
	// enable interrutps
	enc28j60_write_op(ENC28J60_BIT_FIELD_SET, EIE, EIE_INTIE|EIE_PKTIE);
	// enable packet reception
	enc28j60_write_op(ENC28J60_BIT_FIELD_SET, ECON1, ECON1_RXEN);
}

// read the revision of the chip:
uint8_t enc28j60_getrev(void)
{
	return(enc28j60_read(EREVID));
}

// link status
uint8_t enc28j60_linkup(void)
{
        // bit 10 (= bit 3 in upper reg)
	return(enc28j60_phy_read_H(PHSTAT2) && 4);
}

void enc28j60_packet_send( uint8_t* packet, uint32_t len)
{
	// Set the write pointer to start of transmit buffer area
	enc28j60_write(EWRPTL, TXSTART_INIT&0xFF);
	enc28j60_write(EWRPTH, TXSTART_INIT>>8);
	// Set the TXND pointer to correspond to the packet size given
	enc28j60_write(ETXNDL, (TXSTART_INIT+len)&0xFF);
	enc28j60_write(ETXNDH, (TXSTART_INIT+len)>>8);
	// write per-packet control byte (0x00 means use macon3 settings)
	enc28j60_write_op(ENC28J60_WRITE_BUF_MEM, 0, 0x00);
	// copy the packet into the transmit buffer
	enc28j60_write_buffer(packet, len);
	// send the contents of the transmit buffer onto the network
	enc28j60_write_op(ENC28J60_BIT_FIELD_SET, ECON1, ECON1_TXRTS);
        // Reset the transmit logic problem. See Rev. B4 Silicon Errata point 12.
	if( (enc28j60_read(EIR) & EIR_TXERIF) ){
                enc28j60_write_op(ENC28J60_BIT_FIELD_CLR, ECON1, ECON1_TXRTS);
        }
}

// Gets a packet from the network receive buffer, if one is available.
// The packet will by headed by an ethernet header.
//      maxlen  The maximum acceptable length of a retrieved packet.
//      packet  Pointer where packet data should be stored.
// Returns: Packet length in bytes if a packet was retrieved, zero otherwise.
uint32_t enc28j60_packet_receive( uint8_t* packet, uint32_t maxlen)
{
	uint32_t rxstat;
	uint32_t len;
	// check if a packet has been received and buffered
	//if( !(enc28j60Read(EIR) & EIR_PKTIF) ){
        // The above does not work. See Rev. B4 Silicon Errata point 6.
	if( enc28j60_read(EPKTCNT) ==0 ){
		return(0);
        }

	// Set the read pointer to the start of the received packet
	enc28j60_write(ERDPTL, (next_packet_ptr));
	enc28j60_write(ERDPTH, (next_packet_ptr)>>8);
	// read the next packet pointer
	next_packet_ptr  = enc28j60_read_op(ENC28J60_READ_BUF_MEM, 0);
	next_packet_ptr |= enc28j60_read_op(ENC28J60_READ_BUF_MEM, 0)<<8;
	// read the packet length (see datasheet page 43)
	len  = enc28j60_read_op(ENC28J60_READ_BUF_MEM, 0);
	len |= enc28j60_read_op(ENC28J60_READ_BUF_MEM, 0)<<8;
        len-=4; //remove the CRC count
	// read the receive status (see datasheet page 43)
	rxstat  = enc28j60_read_op(ENC28J60_READ_BUF_MEM, 0);
	rxstat |= ((uint16_t)enc28j60_read_op(ENC28J60_READ_BUF_MEM, 0))<<8;
	// limit retrieve length
        if (len>maxlen-1){
                len=maxlen-1;
        }
        // check CRC and symbol errors (see datasheet page 44, table 7-3):
        // The ERXFCON.CRCEN is set by default. Normally we should not
        // need to check this.
        if ((rxstat & 0x80)==0){
                // invalid
                len=0;
        }else{
                // copy the packet from the receive buffer
                enc28j60_read_buffer( packet, len);
        }
	// Move the RX read pointer to the start of the next received packet
	// This frees the memory we just read out
	enc28j60_write(ERXRDPTL, (next_packet_ptr));
	enc28j60_write(ERXRDPTH, (next_packet_ptr)>>8);
	// decrement the packet counter indicate we are done with this packet
	enc28j60_write_op(ENC28J60_BIT_FIELD_SET, ECON2, ECON2_PKTDEC);
	return(len);
}

