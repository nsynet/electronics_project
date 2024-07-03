/***********************************************************************/
/*                                                                     */
/*  SERIAL.C:  Low Level Serial Routines                               */
/*                                                                     */
/***********************************************************************/
#include <LPC2103.H>



#define CR     0x0D


int sendchar(int ch){                   /* Write character to Serial Port  */

	if (ch == '\n')  {
    	
		U0THR = CR;	
		while((U0LSR&0x40)==0);						/* output CR */
		}
	U0THR = ch;
    while((U0LSR&0x40)==0);
   	
 
 	return (ch);
}


int getkey (void)  {                      /* Read character from Serial Port */

   	while((U0LSR&0x01)!=1){;}
   	
  	return (U0RBR);
}

