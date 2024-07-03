#ifndef __TF_H
#define __TF_H

#include "common.h"


void TF_SPI_Config(u16 Speed);
u8   TF_SendCommand(u8 cmd, u32 arg, u8 crc);
u8   TF_Init(void);
u8   TF_GetCID(u8* cid_data);
u8   TF_ReadBlock(u32 ByteAddr, u8* buffer);
u8   TF_ReadMultiBlocks(u32 ByteAddr, u8* buffer, u8 count);
u8   TF_WriteBlock(u32 ByteAddr, const u8* buffer); 
u8   TF_WriteMultiBlocks(u32 ByteAddr, const u8* buffer, u8 count);

#endif
