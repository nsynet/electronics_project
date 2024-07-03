/**
********************************************************
*
* @file      Hal_infrared.c
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

#include "stm32l0xx_hal.h"
#include "Hal_infrared.h"

void irInit(void)
{

}

bool irHandle(void)
{
    if(HAL_GPIO_ReadPin(Infrared_GPIO_Port, Infrared_Pin))
    {
        return 0;
    }
    else
    {
        return 1;
    }
}
