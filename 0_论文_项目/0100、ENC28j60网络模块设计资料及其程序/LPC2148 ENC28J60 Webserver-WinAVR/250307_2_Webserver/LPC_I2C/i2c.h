#ifndef _I2C_H
#define _I2C_H

#define STA	0x20
#define SIC	0x08
#define SI	0x08
#define STO	0x10
#define STAC 0x20
#define AA	0x04

#define R 1// Read Bit +1

// STOP Makro
#define STOPI2C		I20CONCLR = SIC; I20CONSET=STO; while((I20CONSET&STO));


void i2c_init();
static void wait_for_SI(void);
int i2c_start(int addr);
int i2c_write(unsigned char *buf, unsigned count);
int i2c_read();
int i2c_readlast();
void i2c_stop(void);

#endif
