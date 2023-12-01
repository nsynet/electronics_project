#ifndef __COMMON_H
#define __COMMON_H

#include "stm32f10x.h"

// Boolean type
typedef enum { FALSE = 0, TRUE } BOOL;
#define NULL            (0)

// ��Ļ�Ϸ�LED, Green, Yellow
#define LED_Y_ON()  (GPIOA->BRR = GPIO_Pin_11)    //0 RESET
#define LED_Y_OFF() (GPIOA->BSRR = GPIO_Pin_11)   //1 SET
#define LED_G_ON()  (GPIOA->BRR = GPIO_Pin_12)    //0 RESET
#define LED_G_OFF() (GPIOA->BSRR = GPIO_Pin_12)   //1 SET

// ���� LED, Blue, Red
#define LED_R_ON()  (GPIOB->BRR = GPIO_Pin_8)    //0 RESET
#define LED_R_OFF() (GPIOB->BSRR = GPIO_Pin_8)   //1 SET
#define LED_B_ON()  (GPIOB->BRR = GPIO_Pin_9)    //0 RESET
#define LED_B_OFF() (GPIOB->BSRR = GPIO_Pin_9)   //1 SET

// ��ǰ����ģʽ
#define MODE_SATINFO      0x01
#define MODE_SPEED        0x02
#define MODE_LOG          0x03

// ��¼ģʽ
#define LOG_STOP          0x01
#define LOG_START         0x02




void DelayUS(u32 nus);
void RCC_Config(void);
void NVIC_Config(void);
void TIM_Config(void);
void USART_Config(void);
void LED_IO_Config(void);

#endif /* __COMMON_H */

