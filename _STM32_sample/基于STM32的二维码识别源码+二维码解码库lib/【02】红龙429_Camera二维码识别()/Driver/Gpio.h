#ifndef _GPIO_H_
#define _GIPO_H_
#include "stm32f4xx.h"

#define LED_GPIO_PORT                 GPIOD
#define LED1_GPIO_PIN                 GPIO_Pin_11
#define LED2_GPIO_PIN                 GPIO_Pin_12
#define LED_GPIO_CLK                  RCC_AHB1Periph_GPIOD

#define BEEP_GPIO_PORT                GPIOD
#define BEEP_GPIO_PIN                 GPIO_Pin_3
#define BEEP_GPIO_CLK                 RCC_AHB1Periph_GPIOD

void LED_Init(void);
void LED_Toggle(uint8_t led);
void BEEP_Init(void);
void BEEP_Toggle(void);
void Key_Init(void);
#endif
