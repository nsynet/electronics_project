#include "stm32f4xx.h"
#include "delay.h"
#include "usart1.h"
#include<stdio.h>
#include "changePWM.h"
#include <Direction.h>
//static u8 rev;
//�����жϣ����ڵ������ַ�ʲô����Ƭ�����պ��ٷ���ȥ
int main(void)
{   
	delay_init(168);//�ӳ�ʱ���ʼ��
	uart_init(9600);//���ڳ�ʼ��
	Direction(1);//��ʼ�����ת��
	delay_ms(10);//�ӳ�10ms
	printf(" welcome to control the smart car!:\n\r");//���ֻ����뻶ӭ��
	while(1);
}
