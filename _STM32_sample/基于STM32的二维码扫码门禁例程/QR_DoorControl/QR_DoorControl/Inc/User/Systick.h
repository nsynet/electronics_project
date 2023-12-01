#ifndef __SYSTICK_H__
#define __SYSTICK_H__

#include "stm32f1xx_hal.h"

#define SYSTICK_PERIOD    1000       /* 1000Hz ¼´ 1ms */

uint32_t get_systicks(void);
uint8_t query_time(uint32_t old_time, uint32_t delay);
void delayms(uint32_t ms);
void delays(uint32_t s);

#endif
