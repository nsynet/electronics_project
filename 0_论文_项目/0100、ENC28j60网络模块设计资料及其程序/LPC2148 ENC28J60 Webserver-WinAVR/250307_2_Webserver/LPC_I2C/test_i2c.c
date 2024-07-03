//*************************************************
//** I2C-Master Polling Library Example
//*************************************************

#include <arch/philips/lpc2114.h> // Replace this file with your own header file
#include "i2c.h"

#define DEVICEADDR 112

int main (void){
  unsigned char i2c_messages[5],readbyte;
  
  i2c_init();
  
  i2c_messages[0]=55;
  i2c_messages[1]=44;
  i2c_messages[2]=99;

  //Write 3 bytes
  i2c_start(DEVICEADDR);
  i2c_write(i2c_messages,3);
  i2c_stop();
  
  //Read 1 byte
  i2c_start(DEVICEADDR+1);
  readbyte=i2c_readlast();
  i2c_stop();
  
  //Read 3 bytes
  i2c_start(DEVICEADDR+1);
  i2c_read();
  i2c_read();
  i2c_readlast();
  i2c_stop();
  
  while(1){
    asm volatile("nop");         
  }
}
