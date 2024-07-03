/**
  * @file    ov7670.h 
  * @author  WB R&D Team - openmcu666
  * @version V0.1
  * @date    2015.6.23
  * @brief   OV7670 Camera Driver
  */
 
#ifndef _OV7670_H_
#define _OV7670_H_


#include "stm32f4xx.h"

#define DCMI_DR_ADDRESS       	0x50050028   


uint16_t ov7670_check(void);
uint8_t ov7670_init(void);
void cam_start(void);
void cam_stop(void);

#endif /* _OV7670_H_ */

/******************* (C) COPYRIGHT 2015 WangBao *****END OF FILE****/
