#include "systick.h"

#ifndef TRUE
#define TRUE	1
#endif

#ifndef FALSE
#define FALSE	0
#endif

static volatile uint32_t    SysTicks = 0;

//void SysTick_Handler(void)
//{
//	SysTicks++;
//}

//void HAL_SYSTICK_Callback(void)
//{
//	
//}

/* 获取当前滴答 */
uint32_t get_systicks(void)
{
	return HAL_GetTick();
	//return SysTicks;
}
/* 超时查询 */
uint8_t query_time(uint32_t old_time, uint32_t delay)
{
	uint32_t new_time = get_systicks();
	
	if(new_time >= old_time)
	{
		return (new_time - old_time) > delay ? TRUE : FALSE;
	}
	else
	{
		/* SysTicks 的值溢出0xFFFFFFFF */
		return (new_time + (0xFFFFFFFF - old_time)) > delay ? TRUE : FALSE;
	}
}
/* 延时ms */
void delayms(uint32_t ms)
{
	uint32_t cur_ticks = get_systicks();
	
	while(!query_time(cur_ticks, ms))
	{
		;
	}
}
/* 延时 s */
void delays(uint32_t s)
{
	uint32_t i;
	
	for(i=0;i<s;i++)
	{
		delayms(1000);
	}
}
