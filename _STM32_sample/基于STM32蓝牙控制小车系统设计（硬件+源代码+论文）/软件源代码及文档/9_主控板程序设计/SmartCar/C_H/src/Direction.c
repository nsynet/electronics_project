#include <Direction.h>
#include <stm32f4xx.h>
void Direction(int direction)
{
	 SysTick_Config(SystemCoreClock / 1000); //ϵͳ�ж���Ϊ1ms
	 RCC->AHB1ENR |= 0x00000005; //ʹ��GPIOA��GPIODʱ��
   RCC->APB2ENR |= (1<<14);  //ʹ��syscfgʱ��
	if(direction==0)
	{
   GPIOA->MODER &= 0xffff0000; //����PA0��1��2��3Ϊ���״̬
   GPIOA->MODER |= 0x00005555; 
   GPIOA->OTYPER &= 0xFFFFff00; //����PA0��1��2��3Ϊ�������
   GPIOA->OSPEEDR &= 0xffff0000; //PA0��1��2��3����ٶ�Ϊ100m
   GPIOA->OSPEEDR |= 0x0000ffff;
   SYSCFG->CMPCR = 0x00000001; //ʹ��IO������Ԫ
   GPIOA->PUPDR &= 0xffffff00;  //PA0��1��2��3��������������
   GPIOA->BSRRH = 0x00ff;  //��λGPIOA_BSRRH�Ĵ���
   GPIOA->BSRRL = 0x0055;		
	 }
		else
		{
			GPIOA->MODER &= 0xffff0000; //����PA0��1��2��3Ϊ���״̬
   GPIOA->MODER |= 0x0000005555; 
   GPIOA->OTYPER &= 0xFFFFff00; //����PA0��1��2��3Ϊ�������
   GPIOA->OSPEEDR &= 0xffff0000; //PA0��1��2��3����ٶ�Ϊ100m
   GPIOA->OSPEEDR |= 0x0000ffff;
   SYSCFG->CMPCR = 0x00000001; //ʹ��IO������Ԫ
   GPIOA->PUPDR &= 0xffffff00;  //PA0��1��2��3��������������
   GPIOA->BSRRH = 0x00ff;  //��λGPIOA_BSRRH�Ĵ���
   GPIOA->BSRRL = 0x00AA;
		}
}
