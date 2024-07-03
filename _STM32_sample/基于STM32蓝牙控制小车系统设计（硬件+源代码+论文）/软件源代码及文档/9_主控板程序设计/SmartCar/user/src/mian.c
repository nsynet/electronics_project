#include "stm32f4xx.h"
#include "delay.h"
#include "usart1.h"
#include<stdio.h>
#include "changePWM.h"
#include <Direction.h>
//static u8 rev;
//串口中断：串口调试助手发什么，单片机接收后再发回去
int main(void)
{   
	delay_init(168);//延迟时间初始化
	uart_init(9600);//串口初始化
	Direction(1);//初始化电机转向
	delay_ms(10);//延迟10ms
	printf(" welcome to control the smart car!:\n\r");//给手机输入欢迎语
	while(1);
}
