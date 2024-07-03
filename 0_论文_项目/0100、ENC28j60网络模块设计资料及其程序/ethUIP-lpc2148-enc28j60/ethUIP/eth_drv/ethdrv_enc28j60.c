/******************************************************************************
 *
 * Copyright:
 *    (C) 2000 - 2005 Embedded Artists AB
 *
 * Description:
 *    Ethernet driver for Davicom DM9000E
 *
 *****************************************************************************/

/******************************************************************************
 * Includes 
 *****************************************************************************/

#include "general.h"
#include "ethdrv_enc28j60.h"
#include <lpc2xxx.h>
#include <string.h>

#include <printf_P.h>

/******************************************************************************
 * Local variables
 *****************************************************************************/

/* Ethernet address (MAC) */
static tU8 macAddr[6];

/******************************************************************************
 * Defines and typedefs
 *****************************************************************************/


#define m_eth_dis_int
#define m_eth_ena_int

#define m_nic_read(reg) encReadReg(reg)
#define m_nic_write(reg, data) encWriteReg(reg, data)
#define m_nic_read_data(len, buf) encReadBuff((len), (buf))
#define m_nic_write_data(len, buf) encWriteBuff((len), (buf))


// bit field set
#define m_nic_bfs(reg, data) encBitSet(reg, data)

// bit field clear
#define m_nic_bfc(reg, data) encBitClr(reg, data)

/*
 * Controller memory layout:
 *
 * 0x0000 - 0x0bff  3k bytes transmit buffer
 * 0x0c00 - 0x1fff  5k bytes receive buffer
 */
#define ENC_TX_BUF_START 0x0000
#define ENC_RX_BUF_START 0x0C00

/* maximum frame length */
#define ENC_MAX_FRM_LEN 1518


#define CRYSTAL_FREQUENCY FOSC
#define PLL_FACTOR        PLL_MUL
#define VPBDIV_FACTOR     PBSD

#define SPI_SLAVE_CS 0x00002000  //pin P0.13
#define ENC_RESET    0x00001000  //pin P0.12
#define FAILSAFE_VALUE 5000


static tU8 bank = 0;

/******************************************************************************
 * External Function Prototypes
 *****************************************************************************/


/******************************************************************************
 * Implementation of local functions
 *****************************************************************************/
void ethIf_init(tU8* pEthAddr);


/*****************************************************************************
 *
 * Description:
 *    Delay execution by a specified number of milliseconds by using
 *    timer #1. A polled implementation.
 *
 * Params:
 *    [in] delayInMs - the number of milliseconds to delay.
 *
 ****************************************************************************/
static void
delayMs(tU16 delayInMs)
{
  /*
   * setup timer #1 for delay
   */
  TIMER1_TCR = 0x02;          //stop and reset timer
  TIMER1_PR  = 0x00;          //set prescaler to zero
  TIMER1_MR0 = delayInMs * ((CRYSTAL_FREQUENCY * PLL_FACTOR)/ (1000 * VPBDIV_FACTOR));
  TIMER1_IR  = 0xff;          //reset all interrrupt flags
  TIMER1_MCR = 0x04;          //stop timer on match
  TIMER1_TCR = 0x01;          //start timer
  
  //wait until delay time has elapsed
  while (TIMER1_TCR & 0x01)
    ;
}

/*****************************************************************************
 *
 * Description:
 *    Sends and received one byte over the SPI serial channel.
 *    A polled implementation.
 *
 * Params:
 *    [in] byte - the byte to send.
 *
 * Return:
 *    The received byte.
 *
 ****************************************************************************/
static tU8
sendSpi(tU8 byte)
{
  tU32 failsafe;
  tU8  receivedByte;

//  IOCLR0 = SPI_SLAVE_CS;  //activate SPI

  SPI_SPDR = byte;
  failsafe = 0;
  while (((SPI_SPSR & 0x80) == 0) && (failsafe < FAILSAFE_VALUE))
    failsafe++;

  receivedByte = SPI_SPDR;

//  IOSET0 = SPI_SLAVE_CS;  //deactivate SPI

  //reinitialize SPI if timeout
  if (failsafe >= FAILSAFE_VALUE)
  {
    printf("\nSPI-error: SSEL signal must be high!\n");
    receivedByte = 0x00;
    SPI_SPCCR = 0x08;    
    SPI_SPCR  = 0x20;  //no irq, MSB first, master, cpol=0, cpha=0
  }

  return receivedByte;
}
static tU8 encReadReg(tU8 regNo);


static void
encWriteReg(tU8 regNo, tU8 data)
{
  IOCLR0 = SPI_SLAVE_CS;  //activate SPI
  sendSpi(0x40 | regNo);  //write in regNo
  sendSpi(data);
  IOSET0 = SPI_SLAVE_CS;  //deactivate SP

  IOCLR0 = SPI_SLAVE_CS;  //activate SPI
  sendSpi(0x1f);         //read reg 0x1f
//printf("\nwrite reg = %x (bank = %d), value=%x", regNo, sendSpi(0) & 0x3, data);

  IOSET0 = SPI_SLAVE_CS;  //deactivate SP

}

static tU8
encReadReg(tU8 regNo)
{
  tU8 rxByte;

  IOCLR0 = SPI_SLAVE_CS;  //activate SPI
  sendSpi(0x1f);         //read reg 0x1f

  bank = sendSpi(0) & 0x3;
  IOSET0 = SPI_SLAVE_CS;  //deactivate SP


  IOCLR0 = SPI_SLAVE_CS;  //activate SPI
  sendSpi(regNo);         //read reg i
  rxByte = sendSpi(0);

  //check if MAC or MII register
  if (((bank == 2) && (regNo <= 0x1a)) ||
      ((bank == 3) && (regNo <= 0x05 || regNo == 0x0a)))
  //ignore first byte and read another byte
  {
    rxByte = sendSpi(0);
    printf("\nRead MAC or MII reg = %x value=%x", regNo, rxByte);
  }
  IOSET0 = SPI_SLAVE_CS;  //deactivate SP

  return rxByte;
}

static void
encReadBuff(tU16 length, tU8 *pBuff)
{
//    tU8 rxByte;
  IOCLR0 = SPI_SLAVE_CS;  //activate SPI
  sendSpi(0x20 | 0x1a);         //read buffer memory

  while (length--)
  {
    if (pBuff != NULL)
      *pBuff++ = sendSpi(0);
    else
      sendSpi(0);
  }

  IOSET0 = SPI_SLAVE_CS;  //deactivate SP
//  return rxByte;
}

static void
encWriteBuff(tU16 length, tU8 *pBuff)
{
//    tU8 rxByte;
  IOCLR0 = SPI_SLAVE_CS;  //activate SPI
  sendSpi(0x60 | 0x1a);         //write buffer memory

  while (length--)
    sendSpi(*pBuff++);

  IOSET0 = SPI_SLAVE_CS;  //deactivate SP
//  return rxByte;
}

static void
encBitSet(tU8 regNo, tU8 data)
{
  IOCLR0 = SPI_SLAVE_CS;  //activate SPI
  sendSpi(0x80 | regNo);  //bit field set 
  sendSpi(data);
  IOSET0 = SPI_SLAVE_CS;  //deactivate SP
}

static void
encBitClr(tU8 regNo, tU8 data)
{
  IOCLR0 = SPI_SLAVE_CS;  //activate SPI
  sendSpi(0xA0 | regNo);  //bit field clear 
  sendSpi(data);
  IOSET0 = SPI_SLAVE_CS;  //deactivate SP
}

static void
encReset(void)
{
  IOCLR0 = SPI_SLAVE_CS;  //activate SPI
  sendSpi(0xff);  //soft reset
  IOSET0 = SPI_SLAVE_CS;  //deactivate SP
}

static void
initSpi(void)
{
  //connect SPI bus to IO-pins
  PINSEL0 |= 0x00005500;

  //initialize SPI interface
  SPI_SPCCR = 0x08;    
  SPI_SPCR  = 0x20;  //no irq, MSB first, master, cpol=0, cpha=0

  //make SPI slave chip select an output and set signal high
  IODIR |= SPI_SLAVE_CS;
  IOSET  = SPI_SLAVE_CS;

  //reset ENC28J60
  IODIR |= ENC_RESET;
  IOSET  = ENC_RESET;
  IOCLR  = ENC_RESET;
  delayMs(1);              //reset pulse width > 2 us
  IOSET  = ENC_RESET;
  delayMs(1);              //wait >300us after reset
}

/*****************************************************************************
 *
 * Description:
 *    Read PHY registers.
 *
 *    NOTE! This function will change to Bank 2.
 *
 * Params:
 *    [in] addr address of the register to read
 *
 * Returns:
 *    The value in the register
 *
 ****************************************************************************/
static tU16 
phyRead(tU8 addr)
{
  tU16 ret = 0;

  /* move to bank 2 */
  m_nic_bfc(CTL_REG_ECON1, ENC_ECON1_BSEL0);
  m_nic_bfs(CTL_REG_ECON1, ENC_ECON1_BSEL1);

  /* write address to MIREGADR */
  m_nic_write(CTL_REG_MIREGADR, addr);

  /* set MICMD.MIIRD */
  m_nic_write(CTL_REG_MICMD, ENC_MICMD_MIIRD);

  /* poll MISTAT.BUSY bit until operation is complete */
  while ((m_nic_read(CTL_REG_MISTAT) & ENC_MISTAT_BUSY) != 0)
  {
    static int cnt = 0;

    if (cnt++ >= 1000)
    {
      printf("#");
      cnt = 0;
    }
  }


  /* clear MICMD.MIIRD */
  m_nic_write(CTL_REG_MICMD, 0);

  ret  = (m_nic_read(CTL_REG_MIRDH) << 8);
  ret |= (m_nic_read(CTL_REG_MIRDL) & 0xFF);

  return ret;
}

/*****************************************************************************
 *
 * Description:
 *    Get the next packet from the receive ring buffer. 
 *
 * Params:
 *    [in/out] pBuf - data is copied to this buffer
 *    [in]     len  - length of buffer
 *
 * Returns:
 *    number of copied bytes to the supplied buffer
 *
 ****************************************************************************/
static tU16
getPacket(tU8* pBuf, 
          tU16 len)
{
  tU8 nextpL = 0;
  tU8 nextpH = 0;
  tU16 nextPointer;

  tU16 count = 0;
  tU16 status = 0;
  tU16 cpyLen = 0;

  tU8 buffer[20];

  // move to bank 0
  m_nic_bfc(CTL_REG_ECON1, (ENC_ECON1_BSEL1 | ENC_ECON1_BSEL0));

//printf("\nCTL_REG_ERDPT = 0x%x", (tU16)m_nic_read(CTL_REG_ERDPTL) | ((tU16)m_nic_read(CTL_REG_ERDPTH) << 8));
//printf("\nCTL_REG_ERXRDPT = 0x%x", (tU16)m_nic_read(CTL_REG_ERXRDPTL) | ((tU16)m_nic_read(CTL_REG_ERXRDPTH) << 8));

  m_nic_read_data(6, buffer);
//printf("\nByte= [%x, %x, %x, %x, %x, %x, %x, %x, %x, %x]\n", buffer[0], buffer[1], buffer[2], buffer[3], buffer[4], buffer[5], buffer[6], buffer[7], buffer[8], buffer[9]);
//printf("\nByte= [%x, %x, %x, %x, %x, %x, %x, %x, %x, %x]\n", buffer[10], buffer[11], buffer[12], buffer[13], buffer[14], buffer[15], buffer[16], buffer[17], buffer[18], buffer[19]);

#if 0

  /* read the next packet pointer */
  m_nic_read_data(1, &nextpL);
  m_nic_read_data(1, &nextpH);
  printf("\nNextp = 0x%x\n", (tU16)nextpL | ((tU16)nextpH << 8));

  /* 
   * Get the receive byte count. The count includes destination and
   * source addresses, type/length, data, padding and CRC
   */
  m_nic_read_data(2, (tU8*)&count);
#endif
  nextpL = buffer[0];
  nextpH = buffer[1];
  nextPointer = (tU16)nextpL | (tU16)nextpH << 8;
  count = buffer[2];
  count |= (tU16)buffer[3] << 8;
  status = buffer[4];
  status |= (tU16)buffer[5] << 8;



  // TODO: Remove this code.  In revision A0 beta silicon, a bug exists 
  // which can cause the receive buffer to become corrupt when moving the 
  // ERXRDPT value.  They do not update as a WORD, but rather as two 
  // separate BYTEs.  In future silicon revisions, receive buffer corruption
  // should never be possible.
  if (nextPointer > 0x1fff || 
      nextPointer < ENC_RX_BUF_START ||
      count > 1518)
  {
    // Reset the part.  We've gotten corrupt.
    ethIf_init(macAddr);
    printf("\n\nEth reset!!!\n\n");
    return 0;
  }


//  printf("getPacket: count = %d\n", count);

  /* read rest of status vector (2 bytes) bits [23:16][31:24]*/
//  m_nic_read_data(2, (tU8*)&status);

//  printf("getPacket: status = %x, Ok = %d\n", status, ((status & 0x0080) != 0));


  if (pBuf != NULL)
  {
    if (count <= len)
      cpyLen = count;
    else
      cpyLen = len;
  }

  /* read data into buffer */
  m_nic_read_data(cpyLen, pBuf);

  /* skip any remaining data */
  m_nic_read_data((count-cpyLen), NULL);

  /* free buffer space */

  // move to bank 0
  m_nic_bfc(CTL_REG_ECON1, ENC_ECON1_BSEL1);
  m_nic_bfc(CTL_REG_ECON1, ENC_ECON1_BSEL0);


  /*
   * Set to next packet pointer
   */
  m_nic_write(CTL_REG_ERXRDPTL, nextpL);
  m_nic_write(CTL_REG_ERXRDPTH, nextpH);
  m_nic_write(CTL_REG_ERDPTL, nextpL);
  m_nic_write(CTL_REG_ERDPTH, nextpH);

  /* decrease packet counter */
  m_nic_bfs(CTL_REG_ECON2, ENC_ECON2_PKTDEC);

  return cpyLen;

}

/******************************************************************************
 * Implementation of public functions
 *****************************************************************************/

/*****************************************************************************
 *
 * Description:
 *    Resets the NIC and initializes all required hardware registers. 
 *
 * Params:
 *    [in] pEthAddr - the ethernet address (MAC) that should be assigned to 
 *                    the driver. 
 *
 ****************************************************************************/
void
ethIf_init(tU8* pEthAddr)
{
  tU16 phid1 = 0;
  tU16 phid2 = 0;

  memcpy(macAddr, pEthAddr, 6);

  initSpi();

  /* 
   * Setup the buffer space. Only start pointer is needed since the default
   * value of the end pointer (ERXND) is 0x1fff.
   */  
  m_nic_write(CTL_REG_ERXSTL, (ENC_RX_BUF_START & 0xFF));
  m_nic_write(CTL_REG_ERXSTH, (ENC_RX_BUF_START >> 8));
  m_nic_write(CTL_REG_ERDPTL, (ENC_RX_BUF_START & 0xFF));
  m_nic_write(CTL_REG_ERDPTH, (ENC_RX_BUF_START >> 8));

  /*
   * For tracking purposes, the ERXRDPT registers should be programmed with 
   * the same value. This is the read pointer.
   */
  m_nic_write(CTL_REG_ERXRDPTL, (ENC_RX_BUF_START & 0xFF));
  m_nic_write(CTL_REG_ERXRDPTH, (ENC_RX_BUF_START >> 8));

  /* Setup receive filters. */

  // move to bank 1
  m_nic_bfc(CTL_REG_ECON1, ENC_ECON1_BSEL1);
  m_nic_bfs(CTL_REG_ECON1, ENC_ECON1_BSEL0);

  // OR-filtering, Unicast, CRC-check and broadcast
  m_nic_write(CTL_REG_ERXFCON, (ENC_RFR_UCEN|ENC_RFR_CRCEN|ENC_RFR_BCEN));

  /* Wait for Oscillator Start-up Timer (OST). */

  while ((m_nic_read(CTL_REG_ESTAT) & ENC_ESTAT_CLKRDY) == 0)
  {
    static int cnt = 0;

    if (cnt++ >= 1000)
    {
      printf(".");
      cnt = 0;
    }
  }



  /* verify identification */
  phid1 = phyRead(PHY_REG_PHID1);
  phid2 = phyRead(PHY_REG_PHID2);

  if (phid1 != ENC_PHID1_VALUE 
      || (phid2 & ENC_PHID2_MASK) != ENC_PHID2_VALUE)
  {
    printf("ERROR: failed to identify controller\n");
    printf("phid1 = %x, phid2 = %x\n", phid1, (phid2&ENC_PHID2_MASK));
    printf("should be phid1 = %x, phid2 = %x\n", ENC_PHID1_VALUE, ENC_PHID2_VALUE);
  }

  //
  // --- MAC Initialization ---
  //

  /* Pull MAC out of Reset */

  // switch to bank 2
  m_nic_bfc(CTL_REG_ECON1, ENC_ECON1_BSEL0);
  m_nic_bfs(CTL_REG_ECON1, ENC_ECON1_BSEL1);
  // clear MARTST
  m_nic_write(CTL_REG_MACON2, 0);

  /* enable MAC to receive frames */
  m_nic_write(CTL_REG_MACON1, ENC_MACON1_MARXEN);

  /* configure pad, tx-crc and duplex */ 
  // TODO maybe enable FRMLNEN
  m_nic_write(CTL_REG_MACON3, (ENC_MACON3_PADCFG0|ENC_MACON3_TXCRCEN));

  /* set maximum frame length */
  m_nic_write(CTL_REG_MAMXFLL, (ENC_MAX_FRM_LEN & 0xff));
  m_nic_write(CTL_REG_MAMXFLH, (ENC_MAX_FRM_LEN >> 8));

  /* 
   * Set MAC back-to-back inter-packet gap. Recommended 0x12 for half duplex
   * and 0x15 for full duplex.
   */
  m_nic_write(CTL_REG_MABBIPG, 0x12);

  /* Set (low byte) Non-Back-to_Back Inter-Packet Gap. Recommended 0x12 */
  m_nic_write(CTL_REG_MAIPGL, 0x12);

  /* 
   * Set (high byte) Non-Back-to_Back Inter-Packet Gap. Recommended 
   * 0x0c for half-duplex. Nothing for full-duplex
   */
  m_nic_write(CTL_REG_MAIPGH, 0x0C);

  /* set MAC address */

  // switch to bank 3
  m_nic_bfs(CTL_REG_ECON1, (ENC_ECON1_BSEL0|ENC_ECON1_BSEL1));

  m_nic_write(CTL_REG_MAADR0, macAddr[5]);
  m_nic_write(CTL_REG_MAADR1, macAddr[4]);
  m_nic_write(CTL_REG_MAADR2, macAddr[3]);
  m_nic_write(CTL_REG_MAADR3, macAddr[2]);
  m_nic_write(CTL_REG_MAADR4, macAddr[1]);
  m_nic_write(CTL_REG_MAADR5, macAddr[0]);

  //
  // Receive settings
  //

  /* auto-increment RX-pointer when reading a received packet */
  m_nic_bfs(CTL_REG_ECON2, ENC_ECON2_AUTOINC);

  // TODO interrupts can be enabled EIE.PKTIE, EIE.INTIE


  /* enable reception */
  m_nic_bfs(CTL_REG_ECON1, ENC_ECON1_RXEN);


  m_eth_ena_int;
}

/*****************************************************************************
 *
 * Description:
 *    Send an ethernet packet. 
 *
 * Params:
 *    [in] pData - the data to send
 *    [in] len   - length of the data to send
 *
 ****************************************************************************/
void
ethIf_send(tU8* pData1,
           tU16 len1,
           tU8* pData2,
           tU16 len2)

{
  tU8 byteVal = 0;
//printf("\nEnter ethIf_send()");
  // TODO wait for ECON1.TXRTS to be cleared ????
  while ((m_nic_read(CTL_REG_ECON1) & 0x08) == 0x08)
    printf("?");

  /* move to bank 0 */
  m_nic_bfc(CTL_REG_ECON1, (ENC_ECON1_BSEL1 | ENC_ECON1_BSEL0));

  /* set start of transmit buffer */
  m_nic_write(CTL_REG_EWRPTL, (ENC_TX_BUF_START & 0xff));
  m_nic_write(CTL_REG_EWRPTH, (ENC_TX_BUF_START >> 8));
  m_nic_write(CTL_REG_ETXSTL, (ENC_TX_BUF_START & 0xff));
  m_nic_write(CTL_REG_ETXSTH, (ENC_TX_BUF_START >> 8));

  /* set end of transmit buffer, data + control byte */
  m_nic_write(CTL_REG_ETXNDL, ((ENC_TX_BUF_START+len1+len2) & 0xff));
  m_nic_write(CTL_REG_ETXNDH, ((ENC_TX_BUF_START+len1+len2) >> 8));

  /* write control byte */
  byteVal = 0x0E;
  m_nic_write_data(1, &byteVal);

  /* write data */
  m_nic_write_data(len1, pData1);

  if (len2 > 0)
    m_nic_write_data(len2, pData2);


  /* clear transmit interrupt flag */
  m_nic_bfc(CTL_REG_EIR, ENC_EIR_TXIF);

  // TODO set EIE.TXIE o EIE:INTIE if interrupts...

  /* start transmission */
  m_nic_bfs(CTL_REG_ECON1, ENC_ECON1_TXRTS);
//printf("... and start transmission!!!\n");
}


/*****************************************************************************
 *
 * Description:
 *    Poll the driver
 *
 * Params:
 *    [in] pBuf - allocated buffer to which data will be copied
 *    [in] len  - length of buffer
 *
 * Returns:
 *    Number of copied bytes to the supplied buffer
 *
 ****************************************************************************/
tU16
ethIf_poll(tU8* pBuf, 
           tU16 len)
{
  tU16 recvLen = 0;
  tU8 eir = 0;

  eir = m_nic_read(CTL_REG_EIR);

  /* receive error */
  if (eir & ENC_EIR_RXERIF)
  {
    printf("ethIf_poll: Receive Error\n");
    m_nic_bfc(CTL_REG_EIR, ENC_EIR_RXERIF);
  }

  /* transmit error */
  if (eir & ENC_EIR_TXERIF)
  {
    printf("ethIf_poll: Transmit Error\n");
    m_nic_bfc(CTL_REG_EIR, ENC_EIR_TXERIF);
  }

  /* wake-up on LAN */
  if (eir & ENC_EIR_WOLIF)
  {
    printf("ethIf_poll: Wake-up on LAN\n");
  }

  /* Transmit interrupt -> a packet has been sent. */
  if ((eir & ENC_EIR_TXIF) != 0)
  {
    printf("ethIf_poll: Transmit done\n");
    m_nic_bfc(CTL_REG_EIR, ENC_EIR_TXIF);


  }

  /* Link change */
  if ((eir & ENC_EIR_LINKIF) != 0)
  {
    printf("\nethIf_poll: Link Change\n");
    m_nic_bfc(CTL_REG_EIR, ENC_EIR_LINKIF);
  }

  /* DMA completed */
  if ((eir & ENC_EIR_DMAIF) != 0)
  {
    printf("\nethIf_poll: DMA Completed\n");
    m_nic_bfc(CTL_REG_EIR, ENC_EIR_DMAIF);
  }

  /* a packet has been received */
  if ((eir & ENC_EIR_PKTIF) != 0)
  {
    recvLen = getPacket(pBuf, len);
    printf("ethIf_poll: Packet received\n");
  }

  return recvLen;
}
