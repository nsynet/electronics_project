#include <mydef.h>
#ifndef __SYS_CLOCK_H__
#define __SYS_CLOCK_H__

extern volatile uint32_t SYS_TICKS;

extern sys_clock_init(void);

__irq void ISR_TIMER0(void);

#endif
