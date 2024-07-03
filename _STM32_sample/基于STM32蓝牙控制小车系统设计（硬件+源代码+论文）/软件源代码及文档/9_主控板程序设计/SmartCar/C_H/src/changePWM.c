#include <stm32f4xx.h>
#include <changePWM.h>
void Change_PWM(int duty1,int duty2,int duty3,int duty4)
{
	SCB->AIRCR=0x05AF00;//中断优先级分组 抢占：相应=3:1
  
  RCC->AHB1ENR|=(1<<2);//打开GPIOC时钟
  GPIOC->MODER|=0x000AA000;//pc6789第二功能，推挽输出
  GPIOC->OSPEEDR|=0x000FF000;//速度100m
  GPIOC->PUPDR|=0x00055000;//上拉
  
  GPIOC->AFR[0]|=0x22000000;//pc6789第二功能AF2
  GPIOC->AFR[1]|=0x00000022;
  
  RCC->APB1ENR|=(1<<1);//打开TIM3时钟
  TIM3->PSC=83;//对时钟84M进行84分频,使得计数频率为1M
  TIM3->ARR=10000;//周期为10ms
  TIM3->EGR|=1;//产生一次更新时间
  
  TIM3->CCMR1|=0x6060;//PWM模式1
  TIM3->CCMR2|=0x6060;//PWM模式1
  
  TIM3->CCR1=duty1;//1路PWM
  TIM3->CCR2=duty2;//2路PWM
  TIM3->CCR3=duty3;//3路PWM
  TIM3->CCR4=duty4;//4路PWM
  
  TIM3->CCER|=0x1111;//使能比较输出
  
  TIM3->CCMR1|=0x0808;//启动预装载
  TIM3->CCMR2|=0x8080;
  
  TIM3->CR1|=1;//开始计时
}
