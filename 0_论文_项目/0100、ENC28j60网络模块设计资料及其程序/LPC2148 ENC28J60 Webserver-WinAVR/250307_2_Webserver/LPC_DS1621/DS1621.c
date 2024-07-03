#define DEVICEADDR 0x90 

#define STA	0x20
#define SIC	0x08
#define SI	0x08
#define STO	0x10
#define STAC 0x20
#define AA	0x04
#define I2EN	0x40

#define ASCII_PKT	0x2E
#define ASCII_MINUS	0x2D
#define ASCII_PLUS	0x2B

struct stI2C
{
  unsigned char Adr;
  unsigned char DataLen;
  unsigned char Val[10];
};

struct stI2C Slave;
char btI2cFinished;

unsigned char temperatur_low;
unsigned char temperatur_high;

void v_InitI2C(void);
void v_SetNextI2CVal(unsigned char ucVal);
void init_while(void);
void v_StartI2CTrans(void);
void v_WaitI2CFinish(void);
void I2C0IntHandler (void) __irq ;
void init_ds1621(void);
void read_ds1621(void);

void v_InitI2C(void)
{
PINSEL0 &= 0xFFFFFF0F;		//I2C aktivieren
PINSEL0 |= 0x00000050;
I20SCLL = I20SCLH = 400;	//	75kBits @ 58,98 <MHz
I20CONSET = 0x40;		//	enable I2C bus interface

install_irq(I2C0_INT,  (void *)I2C0IntHandler);
	
}

void v_SetNextI2CVal(unsigned char ucVal)
{ 
   Slave.Val[Slave.DataLen] = ucVal;
   Slave.DataLen++;
}

void v_StartI2CTrans(void)
{
   btI2cFinished = 0;
   //I2CON = 0x64;      // Startbedingung setzen  
   
 I20CONCLR = STO | SI ;
 I20CONSET = AA | I2EN | STA;
}

void v_WaitI2CFinish(void)
{
   while (btI2cFinished == 0);
}

void init_ds1621(void)
{
v_InitI2C();
//EA = 1;                // allgemeine Interruptsperre aufheben
Slave.Adr = DEVICEADDR;      // Adresse DS1621 + WR
v_SetNextI2CVal(0xEE); //Command Byte: Start Convert
UART0Send( "** v_StartI2CTrans **\n\r", strlen("** v_StartI2CTrans **\n\r") );
v_StartI2CTrans();
UART0Send( "** v_WaitI2CFinish **\n\r", strlen("** v_WaitI2CFinish **\n\r") );
v_WaitI2CFinish();
}

void read_ds1621(void)
{

  v_SetNextI2CVal(0xAA); //Command Byte:Read Temp
  v_StartI2CTrans();
  UART0Send( "** v_WaitI2CFinish **\n\r", strlen("** v_WaitI2CFinish **\n\r") );
  v_WaitI2CFinish();
  Slave.Adr = DEVICEADDR + 1; // Adresse DS1621 + RD
  Slave.DataLen = 2;    // zwei Bytes werden empfangen
  UART0Send( "** v_StartI2CTrans **\n\r", strlen("** v_StartI2CTrans **\n\r") );
  v_StartI2CTrans();
  UART0Send( "** v_WaitI2CFinish **\n\r", strlen("** v_WaitI2CFinish **\n\r") );
  v_WaitI2CFinish();	
  
  UART0Send( "** Temp.: ", strlen("** Temp.: ") );
   for (int t_help=0;t_help<Slave.DataLen;t_help++)
	{
	UART0Send_Int(Slave.Val[t_help]);
	}
	UART0Send( " **\n\r", strlen(" **\n\r") );
	
temperatur_high = Slave.Val[0];
temperatur_low = Slave.Val[1];

}

unsigned char ucI2COff;
//void vI2C_Interrupt(void) interrupt 6
void I2C0IntHandler (void) __irq 
{
//S0SPINT = SPI0_INT_FLAG;		/* clear interrupt flag */
IENABLE;				/* handles nested interrupt */

//UART0Send_Int(I20STAT);

   switch(I20STAT) // Auswertung des Statusregisters
   {
   case 0x08:  // Status: "START gesendet"
		//UART0Send( "** I2C-Int-START gesendet **\n\r", strlen("** I2C-Int-START gesendet **\n\r") );
   case 0x10:  // Status: "Repeat Start wurde gesendet"
		//UART0Send( "** I2C-Int-REP-START gesendet **\n\r", strlen("** I2C-Int-REP-START gesendet **\n\r") );
     I20DAT = Slave.Adr;
     ucI2COff = 0;
     //I2CON = 0x44;    // (STO, STA, SI = 0, ACK =1)
	 I20CONCLR = STO | STA | SI;
	 I20CONSET = AA | I2EN;
     break;
   case 0x18:  // Status: "SLA+W gesendet; ACK empfangen"
	//UART0Send( "** I2C-Int-SLA+W gesendet ACK **\n\r", strlen("** I2C-Int-SLA+W gesendet ACK **\n\r") );
   case 0x28:  // Status: "byte gesendet; ACK empfangen"
	//UART0Send( "** byte gesendet; ACK empfangen: ", strlen("** byte gesendet; ACK empfangen: ") );
     if (ucI2COff < Slave.DataLen)
     {
	 //UART0Send( "naechster wert **\n\r", strlen("naechster wert **\n\r") );
        I20DAT = Slave.Val[ucI2COff];  // naechsten Wert
        ucI2COff++;                   // schreiben
		 //I2CON = 0x44;    // (STO, STA, SI = 0, ACK =1)
		 I20CONCLR = STO | STA | SI ;
		 I20CONSET = AA | I2EN;
     }
     else
     {
	 //UART0Send( "sto **\n\r", strlen("sto **\n\r") );
        Slave.DataLen = 0;
        btI2cFinished = 1;
        //I2CON = 0x54;    // Stopbedingung setzen (STO =1)
		 I20CONCLR = STA | SI ;
		 I20CONSET = AA | I2EN | STO;
     }
     break;
   case 0x20:   // Status: "SLA+W gesendet, NACK empfangen"
	//UART0Send( "** I2C-Int-SLA+W gesendet NACK **\n\r", strlen("** I2C-Int-SLA+W gesendet NACK **\n\r") );
     //I2CON = 0x54;    // Stopbedingung setzen (STO =1)
		 I20CONCLR = STA | SI ;
		 I20CONSET = AA | I2EN | STO;
     break;
   case 0x40:   // Status: "SLA+R gesendet, ACK empfangen"
	//UART0Send( "** I2C-Int-SLA+W ACK empfangen **\n\r", strlen("** I2C-Int-SLA+W ACK empfangen **\n\r") );
     if (ucI2COff < (Slave.DataLen ))
     {     
        if (ucI2COff < (Slave.DataLen) )
          {
		  //I2CON=0x44; //ACK
			I20CONCLR = STO | STA | SI ;
			I20CONSET = AA | I2EN;
		  }
        else
          {
		  //I2CON=0x40; //NACK
			I20CONCLR = STO | STA | SI | AA ;
			I20CONSET = I2EN;
		  }
     }
     break; 
   case 0x50:  // Status: "Datenbyte empfangen, ACK gesendet"
	//UART0Send( "** I2C-Int-Datenbyte ACK empfangen **\n\r", strlen("** I2C-Int-Datenbyte ACK empfangen **\n\r") );
     if (ucI2COff < (Slave.DataLen ))
     {     
        Slave.Val[ucI2COff] = I20DAT;
        ucI2COff++;
        if (ucI2COff < (Slave.DataLen - 1))
          {
		  //I2CON=0x44; //ACK
			I20CONCLR = STO | STA | SI ;
			I20CONSET = AA | I2EN;
		  }
        else
          {
		  //I2CON=0x40; //NACK
			I20CONCLR = STO | STA | SI | AA ;
			I20CONSET = I2EN;
		  }
     }
     break;
   case 0x58:  // Status: "Datenbyte empfangen, NACK gesendet"
   //UART0Send( "** I2C-Int-Datenbyte NACK empfangen **\n\r", strlen("** I2C-Int-Datenbyte NACK empfangen **\n\r") );
        Slave.Val[ucI2COff] = I20DAT;
        ucI2COff++;
        //I2CON = 0x54;    // Stopbedingung setzen (STO =1)
		 I20CONCLR = STA | SI ;
		 I20CONSET = AA | I2EN | STO;
        btI2cFinished = 1;
        break;   
   default:
   //UART0Send( "** default **\n\r", strlen("** default **\n\r") );
        //I2CON = 0x54;    // Stopbedingung setzen (STO =1)
		 I20CONCLR = STA | SI ;
		 I20CONSET = AA | I2EN | STO;
        btI2cFinished = 1;
     break; 
   } 

IDISABLE;
VICVectAddr = 0;		/* Acknowledge Interrupt */  
}
