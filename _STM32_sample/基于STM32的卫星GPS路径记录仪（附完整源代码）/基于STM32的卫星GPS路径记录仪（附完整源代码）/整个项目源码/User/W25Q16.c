#include "W25Q16.h"

#define SPI2_CS_L() (GPIOB->BRR = GPIO_Pin_12)    //0 RESET
#define SPI2_CS_H() (GPIOB->BSRR = GPIO_Pin_12)   //1 SET

void W25Q_IO_Config(void)               // IO 配置
{
    SPI_InitTypeDef  SPI_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    //--------------------------------------------
    //SPI2 GPIO
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    //SPI2 CS#
    SPI2_CS_H();
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    //GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    //---------------------------------------------
    //启动时钟
    //RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO,ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2,ENABLE);

    //---------------------------------------------
    // SPI2 配置
    /* SPI2 设置, 对应W25Q40, 注意NSS引脚选择软件控制, 不开AF, 就能当作一般IO控制了*/
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;            //绝对注意1边沿，注意仔细读参考手册，关于sh
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;               //SPI_NSS_Soft表示NSS引脚由软件决定，NSS脚不能作为一般IO口用
    //SPI_NSS_Hard表示NSS引脚作为一般IO口用
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;  //APB1上 最高使用2分频
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;      //首位在前
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_Init(SPI2, &SPI_InitStructure);
    SPI_Cmd(SPI2, ENABLE);
}

static u8 W25Q_SendByte(u8 byte)        // 底层发送/接收函数
{
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);   // Loop while DR register in not emplty
    SPI_I2S_SendData(SPI2, byte);                                     // Send byte through the SPI2 peripheral
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);  // Wait to receive a byte
    return SPI_I2S_ReceiveData(SPI2);                                 //Return the byte read from the SPI bus
}

u8 W25Q_ReadID(void)                    // 读取 ID
{
    u8 Temp = 0;

    SPI2_CS_L();                          //片选

    W25Q_SendByte(CODE_ReleacePowerDown_DeviceID);

    W25Q_SendByte(CODE_DummyByte);
    W25Q_SendByte(CODE_DummyByte);
    W25Q_SendByte(CODE_DummyByte);

    Temp=W25Q_SendByte(CODE_DummyByte);   //W25Q16: 0x14
    SPI2_CS_H();
    return Temp;
}


void W25Q_ReadUniID(u8 *W25Q_uID)       // 读取 UniID
{
    SPI2_CS_L();                        //片选

    W25Q_SendByte(CODE_ReadUniID);
    W25Q_SendByte(CODE_DummyByte);
    W25Q_SendByte(CODE_DummyByte);
    W25Q_SendByte(CODE_DummyByte);
    W25Q_SendByte(CODE_DummyByte);

    *W25Q_uID=W25Q_SendByte(CODE_DummyByte);
    W25Q_uID++;
    *W25Q_uID=W25Q_SendByte(CODE_DummyByte);
    W25Q_uID++;
    *W25Q_uID=W25Q_SendByte(CODE_DummyByte);
    W25Q_uID++;
    *W25Q_uID=W25Q_SendByte(CODE_DummyByte);

    SPI2_CS_H();
}

u8 W25Q_ReadReg(void)                   // 读取寄存器 1
{
    u8 Temp = 0;

    SPI2_CS_L();      //片选
    W25Q_SendByte(CODE_ReadStatusRegister);
    Temp=W25Q_SendByte(CODE_DummyByte);
    SPI2_CS_H();
    return Temp;
}


u8 W25Q_ReadReg2(void)                  // 读取寄存器 2
{
    u8 Temp = 0;

    SPI2_CS_L();      //片选

    W25Q_SendByte(CODE_ReadStatusRegister2);
    Temp=W25Q_SendByte(CODE_DummyByte);
    SPI2_CS_H();
    return Temp;
}


void W25Q_WriteEnable(void)             // 设置允许写入
{
    SPI2_CS_L();
    W25Q_SendByte(CODE_WriteEnable);
    SPI2_CS_H();
}


void W25Q_WriteDisable(void)            // 设置禁止写入
{
    SPI2_CS_L();
    W25Q_SendByte(CODE_WriteDisable);
    SPI2_CS_H();
}


void W25Q_Busy(void)                    // 等待 Flash 操作完成
{
    u8 SReg = 0;

    SPI2_CS_L();
    W25Q_SendByte(CODE_ReadStatusRegister); //直到CS->H,会循环发送寄存器数值
    do {
        SReg = W25Q_SendByte(CODE_DummyByte);
        //----------------------------
        //bit  7   6   5   4   3   2   1   0
        //                                Busy
    } while ((SReg & 0x01) == 0x01);
    SPI2_CS_H();
}


void W25Q_EraseSector(u32 SectorAddr)   // 扇区擦除
{
    SPI2_CS_L();
    W25Q_SendByte(CODE_WriteEnable);
    SPI2_CS_H();

    SPI2_CS_L();
    W25Q_SendByte(CODE_SectorErase);
    W25Q_SendByte((SectorAddr & 0xFF0000) >> 16);
    W25Q_SendByte((SectorAddr & 0xFF00) >> 8);
    W25Q_SendByte(SectorAddr & 0xFF);
    SPI2_CS_H();
    W25Q_Busy();
}


void W25Q_EraseBlock(u32 BlockAddr)     // 块擦除 64K
{
    SPI2_CS_L();
    W25Q_SendByte(CODE_WriteEnable);
    SPI2_CS_H();

    SPI2_CS_L();
    W25Q_SendByte(CODE_BlockErase64K);
    W25Q_SendByte((BlockAddr & 0xFF0000) >> 16);
    W25Q_SendByte((BlockAddr & 0xFF00) >> 8);
    W25Q_SendByte(BlockAddr & 0xFF);
    SPI2_CS_H();
    W25Q_Busy();
}


void W25Q_EraseChip(void)               // 整片擦除
{
    SPI2_CS_L();
    W25Q_SendByte(CODE_WriteEnable);
    SPI2_CS_H();

    SPI2_CS_L();
    W25Q_SendByte(CODE_ChipErase);
    SPI2_CS_H();
    W25Q_Busy();
}


u8 W25Q_ReadBytes(u32 ReadAddr)         // 读取一个字节
{
    u8 Buffer;

    SPI2_CS_L();

    W25Q_SendByte(CODE_FastReadData);

    W25Q_SendByte((ReadAddr & 0xFF0000) >> 16);
    W25Q_SendByte((ReadAddr& 0xFF00) >> 8);
    W25Q_SendByte(ReadAddr & 0xFF);
    W25Q_SendByte(CODE_DummyByte);

    Buffer= W25Q_SendByte(CODE_DummyByte);

    SPI2_CS_H();

    return Buffer;
}


void W25Q_ReadMultiBytes(u8* pBuffer, u32 ReadAddr, u16 ByteNum)    // 读取多个字节
{

    SPI2_CS_L();

    W25Q_SendByte(CODE_FastReadData);

    W25Q_SendByte((ReadAddr & 0xFF0000) >> 16);
    W25Q_SendByte((ReadAddr& 0xFF00) >> 8);
    W25Q_SendByte(ReadAddr & 0xFF);
    W25Q_SendByte(CODE_DummyByte);

    while (ByteNum--)
    {
        *pBuffer= W25Q_SendByte(CODE_DummyByte);
        pBuffer++;
    }

    SPI2_CS_H();
}


void W25Q_WriteByte(u8 pBuffer, u32 WriteAddr)     // 写入一个字节
{
    SPI2_CS_L();
    W25Q_SendByte(CODE_WriteEnable);
    SPI2_CS_H();

    SPI2_CS_L();
    W25Q_SendByte(CODE_PageProgram);
    W25Q_SendByte((WriteAddr & 0xFF0000) >> 16);
    W25Q_SendByte((WriteAddr & 0xFF00) >> 8);
    W25Q_SendByte(WriteAddr & 0xFF);

    W25Q_SendByte(pBuffer);
    SPI2_CS_H();

    W25Q_Busy();
}


void W25Q_WritePageBytes(u8* pBuffer, u32 WriteAddr, u16 ByteNum)   // 写入多个字节 <256
{
    SPI2_CS_L();
    W25Q_SendByte(CODE_WriteEnable);
    SPI2_CS_H();

    SPI2_CS_L();
    W25Q_SendByte(CODE_PageProgram);
    W25Q_SendByte((WriteAddr & 0xFF0000) >> 16);
    W25Q_SendByte((WriteAddr & 0xFF00) >> 8);
    W25Q_SendByte(WriteAddr & 0xFF);

    while (ByteNum--)
    {
        W25Q_SendByte(*pBuffer);
        pBuffer++;
    }

    SPI2_CS_H();

    W25Q_Busy();
}



u8 SYS_SheckSumFont(void)    // 校验字库
{
    u32 i;
    u8 CheckSum=0x00;

    SPI2_CS_L();

    W25Q_SendByte(CODE_FastReadData);

    W25Q_SendByte(0x00);
    W25Q_SendByte(0x00);
    W25Q_SendByte(0x00);
    W25Q_SendByte(CODE_DummyByte);

    for (i=0;i<0x00070000;i++)
    {
        CheckSum^=W25Q_SendByte(CODE_DummyByte);
    }

    SPI2_CS_H();

    return CheckSum;         // 正常值 = 0x86
}


u16 LOG_ScanPageMap(void)    // 搜索Map
{
//    u16 MapAddr=0,
    u16 i;

    SPI2_CS_L();

    W25Q_SendByte(CODE_FastReadData);
    // 0x070000 - 0x071FFF :    8K 存储空间映射表
    W25Q_SendByte(0x07);
    W25Q_SendByte(0x00);
    W25Q_SendByte(0x00);
    W25Q_SendByte(CODE_DummyByte);

    for (i=0;i<8*1024;i++)
    {
        if (W25Q_SendByte(CODE_DummyByte) == 0xFF)
        {
            SPI2_CS_H();
            return i;
        }
    }

    SPI2_CS_H();

    return 0xFFFF;

}

void LOG_ScanPageMapShow(u8* pStr)    // 搜索Map
{
    u16 i;
    u8 j;

    SPI2_CS_L();

    W25Q_SendByte(CODE_FastReadData);
    // 0x070000 - 0x071FFF :    8K 存储空间映射表
    W25Q_SendByte(0x07);
    W25Q_SendByte(0x00);
    W25Q_SendByte(0x00);
    W25Q_SendByte(CODE_DummyByte);

    for (i=0;i<398;i++)
    {
        *pStr=0x00;

        // *pStr : b 0000 0000
        //                   1:路径起始 = 1
        //                  2: PageUsed = 1
        //                 3: PageFull = 1

        for (j=0;j<16;j++)
        {
            *pStr|=(~(W25Q_SendByte(CODE_DummyByte)));
        }
        pStr++;
    }

    SPI2_CS_H();
}

