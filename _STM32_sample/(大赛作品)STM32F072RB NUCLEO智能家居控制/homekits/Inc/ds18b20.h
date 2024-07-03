#ifndef __DS18B20_H
#define __DS18B20_H

#include "stm32f0xx_hal.h"

#define DS18B20_PORT              GPIOC
#define DS18B20_PIN               GPIO_PIN_2

#define DS18B20_PIN_H  DS18B20_PORT->BSRR = DS18B20_PIN
#define DS18B20_PIN_L  DS18B20_PORT->BRR = DS18B20_PIN
#define DS18B20_IN  (HAL_GPIO_ReadPin(DS18B20_PORT, DS18B20_PIN))

void DS18B20_PortConfig(uint8_t dir);
void DS18B20_Rst(void);
uint8_t DS18B20_Check(void);

uint8_t DS18B20_ReadBit(void);

uint8_t DS18B20_ReadByte(void);
void DS18B20_WriteByte(uint8_t data);

void DS18B20_Start(void);
float DS18B20_GetTemp(void);

#endif


