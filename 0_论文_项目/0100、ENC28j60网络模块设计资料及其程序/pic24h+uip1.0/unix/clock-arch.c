/*
 * Copyright (c) 2006, Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * This file is part of the uIP TCP/IP stack
 *
 * $Id: clock-arch.c,v 1.2 2006/06/12 08:00:31 adam Exp $
 */

/**
 * \file
 *         Implementation of architecture-specific clock functionality
 * \author
 *         Adam Dunkels <adam@sics.se>
 */


#include "p24HJ64GP504.h"
#include "clock-arch.h"
	
/* 当前系统嗒嘀 */	
volatile clock_time_t UIP_Ticks;


/*
;*****************************************************************************************************
;* 函数名称 : clock_time
;* 描    述 : 获得当前系统嗒嘀
;* 输　	 入 : 无
;*        
;* 输　	 出 : 当前系统嗒嘀
;*****************************************************************************************************
;*/
clock_time_t clock_time(void)
{
  return UIP_Ticks;
}

/*
;*****************************************************************************************************
;* 函数名称 : SIGNAL(SIG_OVERFLOW0)
;* 描    述 : 系统定时器中断服务子程序
;* 输　	 入 : 无
;*        
;* 输　	 出 : 无
;*****************************************************************************************************
;*/
void __attribute__((interrupt, no_auto_psv)) _T1Interrupt(void) 
{
	UIP_Ticks++;							/* 系统嗒嘀加一	*/			
    _T1IF=0;
}

/*
;*****************************************************************************************************
;* 函数名称 : UIP_Clock_Init
;* 描    述 : 初始化系统定时器
;* 输　	 入 : 无
;*        
;* 输　	 出 : 无
;*****************************************************************************************************
;*/
void UIP_Clock_Init(void)   //10ms
{
	TMR1 = 0x0000;
    T1CONbits.TCS = 0;      //使用内部时钟源
    T1CONbits.TGATE = 0;  
    T1CONbits.TCKPS = 2;    //64分频指令周期
    PR1 = 0xa0 ;          	//设置周期寄存器20ms
    
    IPC0bits.T1IP = 4;      //中断优先级设置为4
    IFS0bits.T1IF = 0;      //清中断标志位
    IEC0bits.T1IE = 1;      //使能中断
    SRbits.IPL = 0;         //设置CPU中断为可屏蔽模式
    T1CONbits.TON = 1;      //启动系统时钟
}

/*
;*****************************************************************************************************
;*                            			End Of File
;*****************************************************************************************************
;*/

