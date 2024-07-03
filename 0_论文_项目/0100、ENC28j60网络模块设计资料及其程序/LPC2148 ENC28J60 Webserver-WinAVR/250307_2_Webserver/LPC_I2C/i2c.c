//*************************************************
//** Polling I2C-Master Library
//** Created by Mike Schaub
//** Modified by Patrick Mania <pmania_at_tzi.de>
//** Version: 1.0
//** Description:
//** This Library provides I2C-Master Functions on LPC2000 Microcontrollers.
//** The Code was tested on a LPC2124 and LPC2194 
//** 
//** Compiled with WinARM
//*************************************************

//#include <arch/philips/lpc2114.h> // Replace this file with your own header file
#include "lpc_i2c/i2c.h"

// Initialize I2C
void i2c_init(){
	I20SCLL = I20SCLH = 400;	//	75kBits @ 58,98 <MHz
	I20CONSET = 0x40;		//	enable I2C bus interface
}

// Handle timeouts and interrupt action
static void wait_for_SI(void){
	unsigned long timeout = 1600000;

	I20CONCLR = 8;								//	clear SI starts action
	while (timeout-- && !(I20CONSET & 8));		//	check SI with timeout
}


//Send "START" condition
int i2c_start(int addr){
	I20CONCLR = 0x14;								//	clear STO, AA
	I20CONSET = 0x28;								//	set STA, SI
	wait_for_SI();
	I20CONCLR = 0x20;								//	clear STA
	if (I20STAT > 0x10) return(-1);				//	bus is busy
	I20DAT = addr;									//	set address
	wait_for_SI();
	return (I20STAT != 0x40 && I20STAT != 0x18);	//  no acknowledge
}

//Write on I2C bus
int i2c_write(unsigned char *buf, unsigned count){
	while (count--)
	{
		I20DAT = *buf++;						//	load data into I2DAT-Register
		wait_for_SI();
		if (I20STAT != 0x28) return 1;		//	no acknowledge
	}
	return 0;
}

//Read from I2C bus
int i2c_read(){
  I20CONSET=AA; // Assert Acknowledge
  wait_for_SI();
  if(I20STAT!=0x50 && I20STAT!=0x40){
    return -1; // Not ok
  }else{
    return I20DAT;
  }
}

//Read last byte from I2C Bus
int i2c_readlast(){
  I20CONCLR=AA; // Assert Acknowledge
  wait_for_SI();
  if(I20STAT!=0x58){
    return -1; // Not ok
  }else{
    return I20DAT;
  }
}

//Send "STOP" condition
void i2c_stop(void){
	/*I2C_I2CONSET = 0x10;		//	set STO
	I2C_I2CONCLR = 8;			//	clear SI*/
	STOPI2C
	I20CONSET = 0x40;		//	Re-enable I2C bus interface
}
