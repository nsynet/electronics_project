#ifndef __W25Q_H
#define __W25Q_H

#include "common.h"

#define CODE_WriteEnable                0x06       //写使能，设置状态寄存器 
#define CODE_WriteDisable               0x04       //写禁止 
#define CODE_ReadStatusRegister         0x05       //读状态寄存器   //直到CS->H,会循环发送寄存器数值
#define CODE_ReadStatusRegister2        0x35       //读状态寄存器   //直到CS->H,会循环发送寄存器数值
#define CODE_WriteStatusRegister        0x01       //写状态寄存器 
#define CODE_ReadData                   0x03       //读取存储器数据 
#define CODE_FastReadData               0x0B       //快速读取存储器数据 
#define CODE_FastReadDualOutput         0x3B       //快速双端口输出方式读取存储器数据 
#define CODE_PageProgram                0x02       //页面编程--写数据 
#define CODE_BlockErase64K              0xD8       //块擦除,64kb 
#define CODE_BlockErase32K              0x52       //块擦除,32kb 
#define CODE_SectorErase                0x20       //扇区擦除 ,4kb 
#define CODE_ChipErase                  0xC7       //片擦除 
#define CODE_PowerDown                  0xB9       //掉电模式 
#define CODE_ReleacePowerDown_DeviceID  0xAB       //退出掉电模式/获取设备ID信息  
#define CODE_ReadManuID_DeviceID        0x90       //读取制造厂商ID信息和设备ID信息 
#define CODE_ReadUniID                  0x4B       
#define CODE_ReadJedecID                0x9F       //JEDEC的ID信息 

#define CODE_DummyByte                  0xA5 


/*---------------------------------------

                                    擦除 Earse
页: Page = 256 Bytes
     0:  xxx000h-xxx0FFh
    ... ...
    16:  xxxF00h-xxxFFFh

扇区: Sector = Page * 16 = 4K       W25Q_EraseSector();
     0: xx0000h-xx0FFFh
    ... ...
    16: xxF000h-xxFFFFh

块: Block = Sector * 16 = 64K       W25Q_EraseBlock();
     0: 000000h-00FFFFh
    ... ...
    31: 1F0000h-1FFFFFh

W25Q16: = Block * 32 = 2048K        W25Q_EraseChip();
---------------------------------------*/
#define FONT16_ADDR             0x00000000  // 
#define FONT16_SIZE             0x00040000
#define FONT12_ADDR             0x00040000
#define FONT12_SIZE             0x00030000
#define FONT_TOTAL_SIZE         0x00070000
#define FONT_TOTAL_CHECKSUM     0x86


#define BLOCK_EARSE_START_ADDR  0x00070000  // 64K 块擦除起始地址 ***


void W25Q_IO_Config(void);
u8   W25Q_ReadID(void);
void W25Q_ReadUniID(u8 *W25Q_uID);
u8   W25Q_ReadReg(void);
u8   W25Q_ReadReg2(void);
void W25Q_WriteEnable(void);
void W25Q_WriteDisable(void);
void W25Q_Busy(void);
void W25Q_EraseSector(u32 SectorAddr);
void W25Q_EraseBlock(u32 BlockAddr);
void W25Q_EraseChip(void);
u8   W25Q_ReadBytes(u32 ReadAddr);
void W25Q_ReadMultiBytes(u8* pBuffer, u32 ReadAddr, u16 ByteNum);
void W25Q_WriteByte(u8 pBuffer, u32 WriteAddr);
void W25Q_WritePageBytes(u8* pBuffer, u32 WriteAddr, u16 ByteNum);


u8   SYS_SheckSumFont(void);
u16  LOG_ScanPageMap(void);
void LOG_ScanPageMapShow(u8* pStr);



#endif
