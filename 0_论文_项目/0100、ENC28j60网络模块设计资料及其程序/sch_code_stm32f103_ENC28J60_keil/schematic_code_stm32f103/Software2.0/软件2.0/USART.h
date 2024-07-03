/******************** (C) COPYRIGHT 2009 Robotell ********************
* File Name          : USART.h
* Author             : 郭盖华
* Version            : V2.0.1
* Date               : 2009-4-23
* Description        : 串口通讯主文件
**********************************************************************/

//----------------------------------------------------------------------------
// Define to prevent recursive inclusion
//----------------------------------------------------------------------------
#ifndef __USART_H
#define __USART_H

//----------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------
#include "stm32f10x_lib.h"
#include "string.h"
#include "RTL.h"

//----------------------------------------------------------------------------
// macros
//----------------------------------------------------------------------------
#define USART_FRAMECTRL 0xA5                                                  
#define USART_FRAMEHEAD 0xAA
#define USART_FRAMETAIL 0x55

#define RX_BUFFER_LEN 128
#define TX_BUFFER_LEN 128

#define TX_EVENT (1<< 1)
#define RX_EVENT (1<< 2)

//----------------------------------------------------------------------------
// types
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Exported variables
//----------------------------------------------------------------------------
extern u8 UASRT_Signal;

//----------------------------------------------------------------------------
// Exported functions
//----------------------------------------------------------------------------
void USART_Initial(u32 baudrate);
u16 USART_RecvPackage(u8 *packid, u8 *data , u16 timeout);
OS_RESULT USART_SendPackage(s16 packid, u8 *data,u16 n , u16 timeout);

u16 USART_RecvBuf(u8 *buf, u16 n , u16 timeout);
OS_RESULT USART_SendBuf(u8 *buf,u16 n, u16 timeout);

u16 USART_SendPackageWithACK(u8 *txdata,u16 txn, u8 *ackdata, u16 timeout);
u16 USART_RecvPackageWithACK(u8 *rxdata, u8 *ackdata, u16 ackn, u16 timeout);

bool USART_RxCallBack(u8 data);
void USART_TxCallBack(void);

#endif
