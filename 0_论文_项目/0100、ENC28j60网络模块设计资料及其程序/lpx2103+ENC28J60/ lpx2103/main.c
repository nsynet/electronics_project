#include<stdio.h>
#include <LPC2103.H>



 #define LED  ( 1 << 26 )        //P0.26控制LED


#define FPCLK 66355200L

#define CS (1<<7)
#define reset (1<<9);

extern int ENC_main(void);




void uart_int(unsigned char ch)
{	

	if(ch==0)
	{

	PINSEL0 |= 0x05;
	PINSEL1 = 0x00;
	U0LCR = 0x80;  //SET DLAB
	U0DLM = 0x00;  //115200bps @66.35520M
	U0DLL = 0x24;
	U0LCR = 0x03;
	}
}


void spi_int(void)
{
	PINSEL0 |= 0x1500;
	IODIR |= CS;  //P0.7
	S0SPCCR = 0x08;  //0a 6.6m 0c 5.5m 0e 4.7m
	S0SPCR = 0x20;
}


do_spi(unsigned char dat)
{
	S0SPDR = dat;
	while( 0==(S0SPSR&0x80));
	return(S0SPDR);
}
	





int main (void) 
{  
   unsigned int ii;
   IODIR = 0x04000000;		   //设置IO口为输出口
   IODIR |= (1<<9);			   //set p0.9 as reset
   IOCLR |= reset;
   while(ii<200)
   {
   	ii++;
   }
   
   IOSET|=reset;
	ii=0;
	 while(ii<35535)
   {
   	ii++;
   }


    uart_int(0);
	spi_int();
	printf("Ready!"	);
	ENC_main();
  
}

