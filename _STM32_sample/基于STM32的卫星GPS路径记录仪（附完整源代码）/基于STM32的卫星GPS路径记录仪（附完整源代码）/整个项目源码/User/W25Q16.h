#ifndef __W25Q_H
#define __W25Q_H

#include "common.h"

#define CODE_WriteEnable                0x06       //дʹ�ܣ�����״̬�Ĵ��� 
#define CODE_WriteDisable               0x04       //д��ֹ 
#define CODE_ReadStatusRegister         0x05       //��״̬�Ĵ���   //ֱ��CS->H,��ѭ�����ͼĴ�����ֵ
#define CODE_ReadStatusRegister2        0x35       //��״̬�Ĵ���   //ֱ��CS->H,��ѭ�����ͼĴ�����ֵ
#define CODE_WriteStatusRegister        0x01       //д״̬�Ĵ��� 
#define CODE_ReadData                   0x03       //��ȡ�洢������ 
#define CODE_FastReadData               0x0B       //���ٶ�ȡ�洢������ 
#define CODE_FastReadDualOutput         0x3B       //����˫�˿������ʽ��ȡ�洢������ 
#define CODE_PageProgram                0x02       //ҳ����--д���� 
#define CODE_BlockErase64K              0xD8       //�����,64kb 
#define CODE_BlockErase32K              0x52       //�����,32kb 
#define CODE_SectorErase                0x20       //�������� ,4kb 
#define CODE_ChipErase                  0xC7       //Ƭ���� 
#define CODE_PowerDown                  0xB9       //����ģʽ 
#define CODE_ReleacePowerDown_DeviceID  0xAB       //�˳�����ģʽ/��ȡ�豸ID��Ϣ  
#define CODE_ReadManuID_DeviceID        0x90       //��ȡ���쳧��ID��Ϣ���豸ID��Ϣ 
#define CODE_ReadUniID                  0x4B       
#define CODE_ReadJedecID                0x9F       //JEDEC��ID��Ϣ 

#define CODE_DummyByte                  0xA5 


/*---------------------------------------

                                    ���� Earse
ҳ: Page = 256 Bytes
     0:  xxx000h-xxx0FFh
    ... ...
    16:  xxxF00h-xxxFFFh

����: Sector = Page * 16 = 4K       W25Q_EraseSector();
     0: xx0000h-xx0FFFh
    ... ...
    16: xxF000h-xxFFFFh

��: Block = Sector * 16 = 64K       W25Q_EraseBlock();
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


#define BLOCK_EARSE_START_ADDR  0x00070000  // 64K �������ʼ��ַ ***


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
