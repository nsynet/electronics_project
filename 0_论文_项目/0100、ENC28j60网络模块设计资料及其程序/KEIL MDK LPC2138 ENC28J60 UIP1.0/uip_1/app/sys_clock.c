#include "uip_includes.h"

volatile uint32_t SYS_TICKS;

__inline void timer0_init(void)
{
    T0TC = 0;//��������ֵ��0
    T0PR = 0;//��ʱ������Ƶ
    T0MCR = 0x03;//��T0MR0ƥ���λT0TC���������жϱ�־
    T0MR0 = 60000000/1/4/100;//ÿ10ms�ж�һ��
    T0TCR = 1;//������ʱ��
    SYS_TICKS = 0;
}



__irq void ISR_TIMER0(void)
{
    
    T0IR = 1;//���жϱ�־

    SYS_TICKS++;
    VICVectAddr = 0;
}


__inline void timer0_int_init(void)
{
    VICIntSelect = 0;
    VICVectCntl0 = 0x20 | 0x04;
    VICVectAddr0 = (unsigned int) ISR_TIMER0;
    VICIntEnable = 1<<4;
}


sys_clock_init(void)
{
    timer0_init();
    timer0_int_init();
}

