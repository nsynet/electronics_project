/******************************************************************************************\ 
============================================================================================
* T905.c
*
* This file is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTYT; 
* without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*
*
* NOTES:
* This program is for the nRF905 with 232 interface;
* $write date: 2004.4.7 ;time 11:00$
* $Revision: 1 $
*
/*******************************************************************************************/
#include <reg51.h>
#include <intrins.h>

#define uchar unsigned char 
#define uint  unsigned int

#define WC		0x00			// Write configuration register command
#define RC		0x10 			// Read  configuration register command
#define WTP		0x20 			// Write TX Payload  command
#define RTP		0x21			// Read  TX Payload  command
#define WTA		0x22			// Write TX Address  command
#define RTA		0x23			// Read  TX Address  command
#define RRP		0x24			// Read  RX Payload  command
/*******************************************************************************************/
typedef struct RFConfig
{
	uchar n;
	uchar buf[10];
}RFConfig;

code RFConfig RxTxConf =
{
	10,
	0x01, 0x0c, 0x44, 0x20, 0x20, 0xcc, 0xcc, 0xcc,0xcc, 0x58	
};
// The content of this struct is nRF905's initialize data.
// CH_NO=1;433MHZ;Normal Opration,No Retrans;RX,TX Address is 4 Bytes
// RX TX Payload Width is 32 Bytes;Disable Extern Clock;Fosc=16MHZ
// 8 Bits CRC And enable
/*******************************************************************************************/
uchar data TxBuf[32];
uchar data RxBuf[32];
/*******************************************************************************************/
uchar bdata DATA_BUF;
sbit	flag	=DATA_BUF^7;
sbit	flag1	=DATA_BUF^0;
/*******************************************************************************************/
sbit	TX_EN	=P2^6;
sbit	TRX_CE	=P2^5;
sbit	PWR_UP	=P2^4;
sbit	MISO	=P2^3;
sbit	MOSI	=P2^2;
sbit	SCK		=P2^1;
sbit	CSN		=P2^0;

sbit	AM		=P3^2;
sbit	DR		=P3^3;
sbit	CD		=P3^5;

/*******************************************************************************************/
void InitIO(void);				// Initialize IO port
void Inituart(void);			// initialize 232 uart
void Config905(void);			// Config nRF905 module
void SetTxMode(void);			// Set nRF905 in Tx mode
void SetRxMode(void);			// Set nRF905 in Rx mode
void TxPacket(void);			// Send data by nRF905
void RxPacket(void);			// Recive data by nRF905
void SpiWrite(uchar);			// Write data to nRF905
uchar SpiRead(void);			// Read data to nRF905
void Delay(uchar n);			// Delay 100us
void Scankey(void);				// Scan key
void TxData (uchar x);			// Send key_value to CRT display
/*******************************************************************************************/
//function main();
/*******************************************************************************************/
void main(void)
{	
	InitIO(); 					// Initialize IO port
	Inituart();					// initialize 232 uart
	Config905();				// Config nRF905 module
	SetTxMode();				// Set Tx Mode
	TxPacket();					// Transmit Tx buffer data
	Delay(500);					// delay for led light	
	P0=0xff;					// led close
	SetRxMode();				// Set nRF905 in Rx mode
	while(1)					// circulation
	{	
		Scankey();				// Scan key
		if (DR)					// If recive data ready... 
			RxPacket();			// ... recive data
	}	
}
/*******************************************************************************************/
//function InitIO();
/*******************************************************************************************/
void InitIO(void)
{
	P0=0x0f;					// led light
	CSN=1;						// Spi 	disable						
	SCK=0;						// Spi clock line init high
	DR=1;						// Init DR for input
	AM=1;						// Init AM for input
	PWR_UP=1;					// nRF905 power on
	TRX_CE=0;					// Set nRF905 in standby mode
	TX_EN=0;					// set radio in Rx mode	
}
/*******************************************************************************************/
//function Inituart();
/*******************************************************************************************/
void Inituart(void)
{
	TMOD = 0x20;				//timer1 working mode 1
	TL1 = 0xfd;					//f7=9600 for 16mhz Fosc,and ... 
	TH1 = 0xfd;					//...fd=19200 for 11.0592mhz Fosc
	SCON = 0xd8;				//uart mode 3,ren==1
	PCON = 0x80;				//smod=0
	TR1 = 1;					//start timer1
}
/*******************************************************************************************/
//function Config905();
/*******************************************************************************************/
void Config905(void)
{
	uchar i;					
	CSN=0;						// Spi enable for write a spi command
	SpiWrite(WC);				// Write config command
	for (i=0;i<RxTxConf.n;i++)	// Write configration words
	{
		SpiWrite(RxTxConf.buf[i]);
	}
	CSN=1;						// Disable Spi
}
/*******************************************************************************************/
//function Delay100us();Delay 100us
/*******************************************************************************************/
void Delay(uchar n)
{
	uint i;
	while(n--)
	for(i=0;i<80;i++);	
}
/*******************************************************************************************/
//function SpiWrite();
/*******************************************************************************************/
void SpiWrite(uchar  byte)
{
	uchar i;	
	DATA_BUF=byte;				// Put function's parameter into a bdata variable		
	for (i=0;i<8;i++)			// Setup byte circulation bits
	{	
		
		if (flag)				// Put DATA_BUF.7 on data line
			MOSI=1;
		else
			MOSI=0;
		SCK=1;					// Set clock line high
		DATA_BUF=DATA_BUF<<1;	// Shift DATA_BUF
		SCK=0;					// Set clock line low 
	}	
}
/*******************************************************************************************/
//function SpiRead();
/*******************************************************************************************/
uchar SpiRead(void)
{
	uchar i;	
	for (i=0;i<8;i++)			// Setup byte circulation bits
	{	
		DATA_BUF=DATA_BUF<<1;	// Right shift DATA_BUF
		SCK=1;					// Set clock line high
		if (MISO)
			flag1=1;			// Read data
		else
			flag1=0;
							
		
		SCK=0;					// Set clock line low
	}
	return DATA_BUF;			// Return function parameter
}
/*******************************************************************************************/
//function TxPacket();
/*******************************************************************************************/
void TxPacket(void)
{
	uchar i;
	//Config905();				
	CSN=0;						// Spi enable for write a spi command	
	SpiWrite(WTP);				// Write payload command
	for (i=0;i<32;i++)
	{
		SpiWrite(TxBuf[i]);		// Write 32 bytes Tx data
	}
	CSN=1;						// Spi disable						
	Delay(1);
	CSN=0;						// Spi enable for write a spi command	
	SpiWrite(WTA);				// Write address command
	for (i=0;i<4;i++)			// Write 4 bytes address
	{
		SpiWrite(RxTxConf.buf[i+5]);
	}	
	CSN=1;						// Spi disable
	TRX_CE=1;					// Set TRX_CE high,start Tx data transmission
	Delay(1);					// while (DR!=1);
	TRX_CE=0;					// Set TRX_CE low
}
/*******************************************************************************************/
//function RxPacket();
/*******************************************************************************************/
void RxPacket(void)
{
	uchar i,xx;	
	TRX_CE=0;					// Set nRF905 in standby mode	
	CSN=0;						// Spi enable for write a spi command
	SpiWrite(RRP);				// Read payload command	
	for (i=0;i<32;i++)
	{
		RxBuf[i]=SpiRead();		// Read data and save to buffer		
	}
	CSN=1;						// Disable spi
	while(DR||AM);	
	P0=	RxBuf[0];				// Buffer[0] output from P0 port...
	TRX_CE=1;
	xx=(RxBuf[0]>>4)&0x0f;
	TxData(xx);
	Delay(500);					// ...light led
	P0=0xff;					// Close led								
}
/*******************************************************************************************/
//function SetTxMode();
/*******************************************************************************************/
void SetTxMode(void)				
{	
	TX_EN=1;
	TRX_CE=0;
	Delay(1); 					// delay for mode change(>=650us)
}				
/*******************************************************************************************/
//function SetRxMode();
/*******************************************************************************************/
void SetRxMode(void)
{
	TX_EN=0;
	TRX_CE=1;
	Delay(1); 					// delay for mode change(>=650us)				
}
/*******************************************************************************************/
//Functon Scankey();
/*******************************************************************************************/	
void Scankey()
{
	uchar Temp,xx;
	P0=0xff;
	Temp=P0&0x0f;				// Read key value from port P0
	if (Temp!=0x0f)
	{	Delay(10);
		Temp=P0&0x0f;				// Read key value from port P0
		if (Temp!=0x0f)
		{
			xx=Temp;	
			Temp=Temp<<4;		// Left shift 4 bits	
			TxBuf[0]=Temp;		// Turn On the LED
			P0=Temp;			// Save to Tx buffer[0]
			SetTxMode();		// Set Tx Mode
			TxPacket();			// Transmit Tx buffer data
			TxData (xx);
			Delay(500);			// Delay for LED light			
			P0=0xff;			// Close LED display
			SetRxMode();		// Set Rx Mode
			
			while((P0&0x0f)!=0x0f);
		 }
	}
}
/*******************************************************************************************/
//Function TxData ();
/*******************************************************************************************/
void TxData (uchar x)
{
	SBUF=x;	
	while(TI==0);
		TI=0;

}
/*******************************************************************************************/
