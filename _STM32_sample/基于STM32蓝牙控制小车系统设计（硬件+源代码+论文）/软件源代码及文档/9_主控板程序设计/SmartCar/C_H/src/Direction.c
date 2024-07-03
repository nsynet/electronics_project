#include <Direction.h>
#include <stm32f4xx.h>
void Direction(int direction)
{
	 SysTick_Config(SystemCoreClock / 1000); //系统中断设为1ms
	 RCC->AHB1ENR |= 0x00000005; //使能GPIOA和GPIOD时钟
   RCC->APB2ENR |= (1<<14);  //使能syscfg时钟
	if(direction==0)
	{
   GPIOA->MODER &= 0xffff0000; //设置PA0，1，2，3为输出状态
   GPIOA->MODER |= 0x00005555; 
   GPIOA->OTYPER &= 0xFFFFff00; //设置PA0，1，2，3为推挽输出
   GPIOA->OSPEEDR &= 0xffff0000; //PA0，1，2，3输出速度为100m
   GPIOA->OSPEEDR |= 0x0000ffff;
   SYSCFG->CMPCR = 0x00000001; //使用IO补偿单元
   GPIOA->PUPDR &= 0xffffff00;  //PA0，1，2，3无上拉，无下拉
   GPIOA->BSRRH = 0x00ff;  //复位GPIOA_BSRRH寄存器
   GPIOA->BSRRL = 0x0055;		
	 }
		else
		{
			GPIOA->MODER &= 0xffff0000; //设置PA0，1，2，3为输出状态
   GPIOA->MODER |= 0x0000005555; 
   GPIOA->OTYPER &= 0xFFFFff00; //设置PA0，1，2，3为推挽输出
   GPIOA->OSPEEDR &= 0xffff0000; //PA0，1，2，3输出速度为100m
   GPIOA->OSPEEDR |= 0x0000ffff;
   SYSCFG->CMPCR = 0x00000001; //使用IO补偿单元
   GPIOA->PUPDR &= 0xffffff00;  //PA0，1，2，3无上拉，无下拉
   GPIOA->BSRRH = 0x00ff;  //复位GPIOA_BSRRH寄存器
   GPIOA->BSRRL = 0x00AA;
		}
}
