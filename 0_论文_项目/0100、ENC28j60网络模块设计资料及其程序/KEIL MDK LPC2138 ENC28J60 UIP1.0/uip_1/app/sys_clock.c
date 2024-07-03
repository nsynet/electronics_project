#include "uip_includes.h"

volatile uint32_t SYS_TICKS;

__inline void timer0_init(void)
{
    T0TC = 0;//计数器初值清0
    T0PR = 0;//计时器不分频
    T0MCR = 0x03;//与T0MR0匹配后复位T0TC，并产生中断标志
    T0MR0 = 60000000/1/4/100;//每10ms中断一次
    T0TCR = 1;//启动定时器
    SYS_TICKS = 0;
}



__irq void ISR_TIMER0(void)
{
    
    T0IR = 1;//清中断标志

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

