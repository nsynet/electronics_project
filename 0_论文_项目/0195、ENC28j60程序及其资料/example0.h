#ifndef __example0_H__
#define __example0_H__

#include <reg52.h>

sbit P1_4 = P1^4;
sbit P1_5 = P1^5;

extern void example0_init(void);
extern void example0_app(void);

#endif 
