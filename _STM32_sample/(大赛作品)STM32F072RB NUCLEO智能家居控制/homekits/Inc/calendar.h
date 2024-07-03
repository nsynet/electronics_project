#ifndef __CALENDAR_H
#define __CALENDAR_H
#include "stm32f0xx_hal.h"





void GetWeek(uint16_t year,uint8_t month,uint8_t day,uint8_t *p);
void GetChinaCalendarStr(uint16_t year,uint8_t month,uint8_t day,uint8_t *str);
uint8_t GetJieQiStr(uint16_t year,uint8_t month,uint8_t day,uint8_t *str);

#endif 
