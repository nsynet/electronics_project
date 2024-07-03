#include "misc.h"



void delay(uint32_t t)
{
	uint32_t nTimingDelay = 0;

	nTimingDelay = t + TIM2->CNT;
	
	while(TIM2->CNT < nTimingDelay);
	
}


