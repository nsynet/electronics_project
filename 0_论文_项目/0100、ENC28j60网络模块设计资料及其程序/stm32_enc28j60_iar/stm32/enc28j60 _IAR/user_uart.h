/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef _USER_UART_H
#define _USER_UART_H

/* Includes ------------------------------------------------------------------*/

#include "main.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

void UART1_NVIC_Configuration(void);
void UART1_Configuration(void);
void UART1_GPIO_Configuration(void);
u8 Uart1_PutChar(u8 ch);
void Uart1_PutString(u8* buf , u8 len);

#endif /* _USER_UART_H */
