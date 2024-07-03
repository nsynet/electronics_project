/**
********************************************************
*
* @file      Hal_motor.c
* @author    Gizwtis
* @version   V03010100
* @date      2016-07-05
*
* @brief     机智云.只为智能硬件而生
*            Gizwits Smart Cloud  for Smart Products
*            链接|增值ֵ|开放|中立|安全|自有|自由|生态
*            www.gizwits.com
*
*********************************************************/
#include <math.h>
#include "Hal_motor.h"


void motorInit(void)
{
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);

	TIM3->CCR1 = 0;
	TIM3->CCR2 = 0;
    motorStatus(0);
}


void motorControl(uint8_t m1,uint8_t m2)
{
			uint16_t temp = (MOTOR_ARR+1) / MOTOR_MAX;
				
		  TIM3->CCR1 = m1*temp;
    	TIM3->CCR2 = m2*temp;
}

void motorStatus(motor_t status)
{
    if(0 == status)
    {
        motorControl(0,0);
    }
    else
    {
        if(status > 0)
        {
            motorControl((abs(status)+5)*10, 0);
        }
        else
        {
            motorControl(0, (abs(status)+5)*10);
        }
    }  
}


