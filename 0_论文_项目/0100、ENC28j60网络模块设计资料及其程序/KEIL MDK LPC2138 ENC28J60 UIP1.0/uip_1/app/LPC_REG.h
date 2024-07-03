/*****************************************************
***(c)Ancheel 2008.9
****************************************************/
#ifndef __LPC_REG_H__
#define __LPC_REG_H__


/*中断*/
#define TIMER0_INT_NUM       0x04
#define TIMER1_INT_NUM       0x05
#define UART0_INT_NUM        0x06
#define UART1_INT_NUM        0x07
#define PWM0_INT_NUM        0x08
#define I2C0_INT_NUM           0x09
#define SPI0_INT_NUM           0x0a
#define SPI1_INT_NUM           0x0b
#define PLL_INT_NUM             0x0c
#define RTC_INT_NUM            0x0d
#define EINT0_INT_NUM         0x0e
#define ENINT1_INT_NUM      0x0f
#define EINT2_INT_NUM         0x10
#define ENINT3_INT_NUM      0x11
#define ADC0_INT_NUM         0x12

/*I2C*/
#define I2EN        6
#define STA          5
#define STO         4
#define SI             3
#define AA           2

//UART


/*SPI REG BIT*/

//SPCR
#define CPHA       3
#define CPOL       4
#define MSTR       5
#define LSBF        6
#define SPIE         7

//SPSR
#define ABRT    3
#define MODF  4
#define ROVR   5
#define WCOL  6
#define SPIF     7

//SP

/*SPI PIN*/

//SSPSR
#define SSPBSY 4//忙
#define SSPRFF 3//接收FIFO满
#define SSPRNE 2//接收FIFO不为空
#define SSPTNF 1//发送FIFO未满
#define SSPTFE 0 //发送FIFO空

//RTC
#define CLKEN 0
#define CLKSRC 4
#define CTCRST 1

#endif
