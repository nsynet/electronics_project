/*
 * Copyright (c) 2001, Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
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
 * Author: Adam Dunkels <adam@sics.se>
 *
 * $Id: tapdev.c,v 1.8 2006/06/07 08:39:58 adam Exp $
 */


#include "config.h"


#ifdef ENC28J60_RX_ISR
volatile unsigned int plen;
#endif

/*
;*****************************************************************************************************
;* 函数名称 : tapdev_init
;* 描    述 : 网络设备初始化
;* 输　	 入 : 无
;*        
;* 输　	 出 : 无
;*****************************************************************************************************
;*/
void tapdev_init(void)
{
#if UIP_FIXEDETHADDR == 0
	extern char avr_mac[];
#else
	char avr_mac[] = {UIP_ETHADDR0,UIP_ETHADDR1,UIP_ETHADDR2, UIP_ETHADDR3, UIP_ETHADDR4, UIP_ETHADDR5};
#endif

	enc28j60_init(avr_mac);
	plen = 0;
	
#ifdef ENC28J60_RX_ISR	
	GICR |= 0x01 << 6;
#endif
}

/*
;*****************************************************************************************************
;* 函数名称 : tapdev_read
;* 描    述 : 从网络设备读回一个数据报
;* 输　	 入 : 无
;*        
;* 输　	 出 : 数据报长度
;*****************************************************************************************************
;*/
unsigned int tapdev_read(void)
{
#ifdef ENC28J60_RX_ISR
	unsigned int temp;
	extern volatile clock_time_t UIP_Ticks;
	
	temp = UIP_Ticks;
	
	while (plen == 0 && temp == UIP_Ticks)
	;
	
	temp = plen;
	
	plen = 0;

	return temp;
#else
	return enc28j60_packet_receive(uip_buf, UIP_BUFSIZE );	
#endif
}

/*
;*****************************************************************************************************
;* 函数名称 : SIGNAL(SIG_INTERRUPT0) 
;* 描    述 : 网络设备接收到一个数据报后的中断处理程序
;* 输　	 入 : 无
;*        
;* 输　	 出 : 无
;*****************************************************************************************************
;*/
#ifdef ENC28J60_RX_ISR
SIGNAL(SIG_INTERRUPT0)  
{
	// get new packet
	plen = enc28j60_packet_receive(uip_buf, UIP_BUFSIZE );	
}
#endif

/*
;*****************************************************************************************************
;* 函数名称 : tapdev_send
;* 描    述 : 利用网络设备发送一个数据报到网络
;* 输　	 入 : 无
;*        
;* 输　	 出 : 无
;*****************************************************************************************************
;*/
void tapdev_send(void)
{
	enc28j60_packet_send(uip_buf, uip_len);
}

/*
;*****************************************************************************************************
;*                            			End Of File
;*****************************************************************************************************
;*/
