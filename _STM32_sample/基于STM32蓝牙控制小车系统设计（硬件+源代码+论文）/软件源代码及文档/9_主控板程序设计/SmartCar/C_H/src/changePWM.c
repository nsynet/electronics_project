#include <stm32f4xx.h>
#include <changePWM.h>
void Change_PWM(int duty1,int duty2,int duty3,int duty4)
{
	SCB->AIRCR=0x05AF00;//�ж����ȼ����� ��ռ����Ӧ=3:1
  
  RCC->AHB1ENR|=(1<<2);//��GPIOCʱ��
  GPIOC->MODER|=0x000AA000;//pc6789�ڶ����ܣ��������
  GPIOC->OSPEEDR|=0x000FF000;//�ٶ�100m
  GPIOC->PUPDR|=0x00055000;//����
  
  GPIOC->AFR[0]|=0x22000000;//pc6789�ڶ�����AF2
  GPIOC->AFR[1]|=0x00000022;
  
  RCC->APB1ENR|=(1<<1);//��TIM3ʱ��
  TIM3->PSC=83;//��ʱ��84M����84��Ƶ,ʹ�ü���Ƶ��Ϊ1M
  TIM3->ARR=10000;//����Ϊ10ms
  TIM3->EGR|=1;//����һ�θ���ʱ��
  
  TIM3->CCMR1|=0x6060;//PWMģʽ1
  TIM3->CCMR2|=0x6060;//PWMģʽ1
  
  TIM3->CCR1=duty1;//1·PWM
  TIM3->CCR2=duty2;//2·PWM
  TIM3->CCR3=duty3;//3·PWM
  TIM3->CCR4=duty4;//4·PWM
  
  TIM3->CCER|=0x1111;//ʹ�ܱȽ����
  
  TIM3->CCMR1|=0x0808;//����Ԥװ��
  TIM3->CCMR2|=0x8080;
  
  TIM3->CR1|=1;//��ʼ��ʱ
}
