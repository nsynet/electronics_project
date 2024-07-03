#include <REG932.H>

struct stI2C
{
  unsigned char Adr;
  unsigned char DataLen;
  unsigned char Val[10];
};

struct stI2C Slave;
bit btI2cFinished;

v_InitI2C(void)
{
   P1M1 |= 0x0C;      // SDA und SCL als Open-Drain
   P1M2 |= 0x0C;
   I2SCLH = 19;       // I2C-Geschwindigkeit ~100 KHz  
   I2SCLL = 19;
   I2CON = 0x44;      // I2C-Hardware aktivieren (STO, STA, SI = 0, ACK =1)
   EI2C = 1;          // I2C Interrupt freigeben
}

v_SetNextI2CVal(unsigned char ucVal)
{ 
   Slave.Val[Slave.DataLen] = ucVal;
   Slave.DataLen++;
}

v_StartI2CTrans(void)
{
   btI2cFinished = 0;
   I2CON = 0x64;      // Startbedingung setzen  
}

void v_WaitI2CFinish(void)
{
   while (btI2cFinished == 0);
}

main()
{
  v_InitI2C();
  EA = 1;                // allgemeine Interruptsperre aufheben
  Slave.Adr = 0x92;      // Adresse DS1621 + WR
  v_SetNextI2CVal(0xEE); //Command Byte: Start Convert
  v_StartI2CTrans();
  v_WaitI2CFinish();
  v_SetNextI2CVal(0xAA); //Command Byte:Read Temp
  v_StartI2CTrans();
  v_WaitI2CFinish();
  Slave.Adr = 0x92 + 1; // Adresse DS1621 + RD
  Slave.DataLen = 2;    // zwei Bytes werden empfangen
  v_StartI2CTrans();
  v_WaitI2CFinish();
  while(1);
}

unsigned char ucI2COff;
void vI2C_Interrupt(void) interrupt 6
{
   switch(I2STAT) // Auswertung des Statusregisters
   {
   case 0x08:  // Status: "START gesendet"
   case 0x10:  // Status: "Repeat Start wurde gesendet"
     I2DAT = Slave.Adr;
     ucI2COff = 0;
     I2CON = 0x44;    // (STO, STA, SI = 0, ACK =1)
     break;
   case 0x18:  // Status: "SLA+W gesendet; ACK empfangen"
   case 0x28:  // Status: "byte gesendet; ACK empfangen"
     if (ucI2COff < Slave.DataLen)
     {
        I2DAT = Slave.Val[ucI2COff];  // naechsten Wert
        ucI2COff++;                   // schreiben
        I2CON = 0x44; 
     }
     else
     {
        Slave.DataLen = 0;
        btI2cFinished = 1;
        I2CON = 0x54;    // Stopbedingung setzen (STO =1)
     }
     break;
   case 0x20:   // Status: "SLA+W gesendet, NACK empfangen"
     I2CON = 0x54;    // Stopbedingung setzen (STO =1)
     break;
   case 0x40:   // Status: "SLA+R gesendet, ACK empfangen"
     if (ucI2COff < (Slave.DataLen ))
     {     
        if (ucI2COff < (Slave.DataLen) )
          I2CON=0x44; //ACK
        else
          I2CON=0x40; //NACK
     }
     break; 
   case 0x50:  // Status: "Datenbyte empfangen, ACK gesendet"
     if (ucI2COff < (Slave.DataLen ))
     {     
        Slave.Val[ucI2COff] = I2DAT;
        ucI2COff++;
        if (ucI2COff < (Slave.DataLen - 1))
          I2CON=0x44; // ACK gesendet
        else
          I2CON=0x40; // NACK gesendet
     }
     break;
   case 0x58:  // Status: "Datenbyte empfangen, NACK gesendet"
        Slave.Val[ucI2COff] = I2DAT;
        ucI2COff++;
        I2CON = 0x54;    // Stopbedingung setzen (STO =1)
        btI2cFinished = 1;
        break;   
   default:
        I2CON = 0x54;    // Stopbedingung setzen (STO =1)
        btI2cFinished = 1;
     break; 
   }   
}