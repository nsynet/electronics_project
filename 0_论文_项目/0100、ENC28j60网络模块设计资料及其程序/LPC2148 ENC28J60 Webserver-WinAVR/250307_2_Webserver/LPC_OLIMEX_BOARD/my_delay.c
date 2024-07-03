/*
		Andreas Weschenfelder				14.03.07
		
		Delay-Fkten entwickeln
		
		fuer 60 Mhz-Takt, reine Warte-Fkten

*/


#include "LPC_OLIMEX_BOARD\my_delay.h"


void _delay_us_60mhz(U32 my_wait)
{
for (U32 tmp_i=0; tmp_i<my_wait; tmp_i++)
	{
	for (U32 tmp_j=0; tmp_j<(60 /6); tmp_j++)
		{
		nop();
		}
	}
}

void _delay_ms_60mhz(U32 my_wait)
{
for(U32 tmp_i=0; tmp_i<my_wait; tmp_i++)
	{
	_delay_us_60mhz(1000);
	}
}
