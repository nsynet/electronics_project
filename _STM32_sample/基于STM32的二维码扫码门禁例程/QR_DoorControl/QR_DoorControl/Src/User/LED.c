#include "LED.h"

void LED_Control(uint8_t sta)
{
	if(sta)
	{
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);		// ��LED��
	}
	else
	{
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);			// �ر�LED��
	}
}
