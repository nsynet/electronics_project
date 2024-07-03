#include "clock-arch.h"
//#include <lpc213x.h>
//#include "typedefs.h"
//#include "config.h"

U32 timer_counter;

/* Timer0 Compare-Match Interrupt Handler (ISR) */
//void __attribute__ ((interrupt("IRQ"))) tc0_cmp(void)
static void /*RAMFUNC*/ tc0_cmp (void) __irq 
{
//Der Timer wird auf jeden Fall aufgerufen...
//UART0Send( "** Timer **\n\r", strlen("** Timer **\n\r") );
	timer_counter++;
	T0IR = TxIR_MR0_FLAG; // Clear interrupt flag by writing 1 to Bit 0
	VICVectAddr = 0;       // Acknowledge Interrupt (rough?)
}

/* Setup Timer0 Compare-Match Interrupt         */
/* no prescaler timer runs at cclk = FOSC*PLL_M */
void init_timer (void) {
	
	T0MR0 = (FOSC/100)-1;							 // Compare-hit at 1mSec (-1 reset "tick")
	T0MCR = TxMCR_INT_ON_MR0 | TxMCR_RESET_ON_MR0;	 // Interrupt and Reset on MR0
	T0TCR = TxTCR_COUNTER_ENABLE;            		 // Timer0 Enable

    timer_counter=0;

	//VICVectAddr0 = (unsigned long)tc0_cmp;   // set interrupt vector in 0
	//VICVectCntl0 = VICVectCntl0_ENABLE | VIC_Channel_Timer0; // use it for Timer 0 Interrupt:
	//VICIntEnable = (1<<VIC_Channel_Timer0);    // Enable Timer0 Interrupt
	install_irq( TIMER0_INT, (void *)tc0_cmp );
}

unsigned  int SetDelay10ms (int t)
{
  return(timer_counter + t);                                             
}

unsigned char CheckDelay10ms (int t)
{
  return( ((signed int) t - (signed int) timer_counter) < 0 );
}

