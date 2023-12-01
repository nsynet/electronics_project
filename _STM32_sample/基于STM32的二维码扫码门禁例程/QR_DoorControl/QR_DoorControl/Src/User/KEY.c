#include "KEY.h"

uint8_t Get_KEY_Status(void)
{
	return HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0);
}

