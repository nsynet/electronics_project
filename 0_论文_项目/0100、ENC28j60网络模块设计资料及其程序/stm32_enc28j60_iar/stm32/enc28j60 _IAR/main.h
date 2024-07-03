/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _MAIN_H
#define _MAIN_H


/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_lib.h"
#include "user_uart.h"
#include "string.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

void RCC_HSI_Configuration(void);
void NVIC_Configuration(void);





#endif /* _MAIN_H */
