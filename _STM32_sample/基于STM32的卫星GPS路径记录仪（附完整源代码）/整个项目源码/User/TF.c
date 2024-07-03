/*-------------------------------------------------------
      TF 卡驱动 by wowbanui 2011

 版本历史:
 Rev.201108     SPI 接口, 初始版本, 不支持 SDHC
---------------------------------------------------------*/

#include "tf.h"
#include "GPS.h"
#include "FATFS/ff.h"
#include "FATFS/diskio.h"

// TF 卡 SPI CS 片选
#define SPI_CS_L() (GPIOA->BRR = GPIO_Pin_4)    //0 RESET
#define SPI_CS_H() (GPIOA->BSRR = GPIO_Pin_4)   //1 SET

// TF 读写速度
#define TF_SPEED_HIGH   SPI_BaudRatePrescaler_4
#define TF_SPEED_LOW    SPI_BaudRatePrescaler_256

#define CMD0_IDLE_ERROR 0x01;
	   
// SD卡指令表  	   
#define CMD0    0       //卡复位
#define CMD1    1
#define CMD8    8
#define CMD9    9       // 读 CSD
#define CMD10   10      // 读 CID
#define CMD12   12      // 停止数据传输
#define CMD16   16      // 设置 SectorSize 应返回0x00
#define CMD17   17      // 读 Sector
#define CMD18   18      // 读多个 Sector
#define ACMD23  23      // 预擦除, 设置多sector写入前预先擦除N个block
#define CMD24   24      // 写 Sector
#define CMD25   25      // 写多个 Sector
#define ACMD41  41      //
#define CMD55   55      //
#define CMD58   58      // 读OCR信息
#define CMD59   59      // 使能/禁止CRC，应返回0x00

#define DUMMY_BYTE  0xFF

void TF_SPI_Config(u16 Speed)           // 配置 SPI
{
    SPI_InitTypeDef  SPI_InitStructure;

    //---------------------------------------------
    // SPI1 配置 对应 SD/TF卡
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge; 
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;    
    //SPI_NSS_Hard表示NSS引脚作为一般IO口用
    SPI_InitStructure.SPI_BaudRatePrescaler = Speed;  //APB1上 最高使用2分频
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;      //首位在前
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_Init(SPI1, &SPI_InitStructure);
}

static u8 TF_SendByte(u8 byte)          // 底层发送/接收函数
{
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);   // Loop while DR register in not emplty
    SPI_I2S_SendData(SPI1, byte);                                     // Send byte through the SPI2 peripheral
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);  // Wait to receive a byte
    return SPI_I2S_ReceiveData(SPI1);                                 //Return the byte read from the SPI bus
}

u8 TF_SendCommand(u8 cmd, u32 arg, u8 crc)        //
{
    u8 result,retry = 0;

    SPI_CS_L();                 // 片选 TF卡

    TF_SendByte(cmd | 0x40);    // cmd
    TF_SendByte(arg >> 24);
    TF_SendByte(arg >> 16);
    TF_SendByte(arg >> 8);
    TF_SendByte(arg);
    TF_SendByte(crc);

    //等待响应，或超时退出
    do{
        result = TF_SendByte(DUMMY_BYTE);
        retry++;
        if(retry > 200) break;
    }while(result==0xFF); 

    SPI_CS_H();                 //关闭片选
    TF_SendByte(DUMMY_BYTE);    //额外增加8个时钟

    return result;                  //返回状态值
}


u8 TF_Init(void)                        // IO 配置
{
    GPIO_InitTypeDef GPIO_InitStructure;
    u8 result=0,retry=0,i;

    //--------------------------------------------
    //SPI1 GPIO
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    //SPI1 CS#
    SPI_CS_H();
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    //GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    //---------------------------------------------
    //启动时钟
    //RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);

    // 初始化时使用低速
    TF_SPI_Config(TF_SPEED_LOW);

    SPI_Cmd(SPI1, ENABLE);
    //=============================================

    //=============================================
    // TF 卡初始化开始
    // 延时 > 74 CLK
    for(i=0;i<10;i++)               // 80 CLK
        TF_SendByte(0xFF);

    // 循环发送 CMD0(进入 Idle 模式) 命令, 返回 0x01 则表明成功进入 Idle 模式
    SPI_CS_L();                     // 片选 TF卡
    while(TF_SendCommand(CMD0, 0, 0x95)!=0x01)  
    {
        retry++;
        if (retry>200)
        {
            SPI_CS_H();             // 释放片选             
            return CMD0_IDLE_ERROR;      // 超时, 退出
        }
    }

    // 已经进入 Idle 模式

    // 发送 CMD55+ACMD41
    retry=0;
    do{
        if (TF_SendCommand(CMD55, 0, 0)!=0x01) return result; 
        // SD_HIGH_CAPACITY 0x40000000
        result = TF_SendCommand(ACMD41, 0x40000000, 0);
        retry++;
        if (retry>200) return result;
    }while(result!=0x00);

    // 切换到高速
    TF_SPI_Config(TF_SPEED_HIGH);

    return 0xEE;
}

u8 TF_GetCID(u8* cid_data)              // 读取 CID
{
    u8 result,retry = 0;

    result = TF_SendCommand(CMD10, 0, 0xFF);
    if (result!=0x00) return result;

    SPI_CS_L();

    // 等待 0xFE
    retry=0;
    while(TF_SendByte(DUMMY_BYTE)!=0xFE)
    {
        retry++;
        if(retry > 200) return 0xE2;    
    }

    // 16 Bytes CID 数据
    retry=16;
    while(retry--)
    {
        *cid_data=TF_SendByte(DUMMY_BYTE);
        cid_data++;
    }

    // CRC: 2Bytes
    TF_SendByte(DUMMY_BYTE);
    TF_SendByte(DUMMY_BYTE);

    SPI_CS_H();                 //关闭片选
    TF_SendByte(DUMMY_BYTE);    //额外增加8个时钟

    return 0x00;              //返回状态值
}


u8 TF_ReadBlock(u32 ByteAddr, u8* buffer)                   // 读块
{
    u8 result;
    u16 retry = 0;

    result = TF_SendCommand(CMD17, ByteAddr, 0);
    if (result!=0x00) return result;

    SPI_CS_L();

    // 等待 0xFE
    retry=0;
    while(TF_SendByte(DUMMY_BYTE)!=0xFE)
    {
        retry++;
        if(retry > 200) return 0xE2;    
    }

    // 16 Bytes CID 数据
    retry=512;
    while(retry--)
    {
        *buffer=TF_SendByte(DUMMY_BYTE);
        buffer++;
    }

    // CRC: 2Bytes
    TF_SendByte(DUMMY_BYTE);
    TF_SendByte(DUMMY_BYTE);

    SPI_CS_H();                 //关闭片选
    TF_SendByte(DUMMY_BYTE);    //额外增加8个时钟

    return 0xF0;              //返回状态值
}



u8 TF_ReadMultiBlocks(u32 ByteAddr, u8* buffer, u8 count)   // 读多个块
{
    u8 result;
    u16 retry = 0;

    result = TF_SendCommand(CMD18, ByteAddr, 0);
    if (result!=0x00) return result;

    SPI_CS_L();

    do{
        // 等待 0xFE
        retry=0;
        while(TF_SendByte(DUMMY_BYTE)!=0xFE)
        {
            retry++;
            if(retry > 200) return 0xE2;    
        }
    
        // 512Byte
        retry=512;
        while(retry--)
        {
            *buffer=TF_SendByte(DUMMY_BYTE);
            buffer++;
        }
    
        // CRC: 2Bytes
        TF_SendByte(DUMMY_BYTE);
        TF_SendByte(DUMMY_BYTE);
    }while(--count);

    TF_SendCommand(CMD12, 0, 0);    //发送停止命令

    SPI_CS_H();                     //关闭片选
    TF_SendByte(DUMMY_BYTE);        //额外增加8个时钟

    return 0xF0;                    //返回状态值
}








#define TF_WRITE_OK             0x05
#define TF_WRITE_CRC_ERROR      0x0B
#define TF_WRITE_ERROR          0x0D
#define TF_WRITE_OTHER_ERROR    0xFF

u8 TF_WriteBlock(u32 ByteAddr, const u8* buffer)            // 写入块
{
    u8 result;
    u16 i = 0;

    result = TF_SendCommand(CMD24, ByteAddr, 0);
    if (result!=0x00) return result;

    SPI_CS_L();

    // 延时 3 Byte
    TF_SendByte(DUMMY_BYTE);
    TF_SendByte(DUMMY_BYTE);
    TF_SendByte(DUMMY_BYTE);

    // 起始传输令牌 0xFE
    TF_SendByte(0xFE);
    for (i=0;i<512;i++)
    {
        TF_SendByte(*buffer);
        buffer++;
    }
    // CRC: 2Bytes
    TF_SendByte(DUMMY_BYTE);
    TF_SendByte(DUMMY_BYTE);

    // 等待 TF 卡结束应答
    result = TF_SendByte(DUMMY_BYTE);
    if ((result&0x1F)!=TF_WRITE_OK)    //TF 卡结束应答错误
    {   
        SPI_CS_H();
        return result;
    }
    // TF 卡结束应答正确 

    // 等待 TF 卡忙
    i=0;
    while(!TF_SendByte(DUMMY_BYTE))
    {
        i++;
        if (i>0xFFF0)       // 写入超时退出
        {
            SPI_CS_H();
            return 1;        
        }
    }

    SPI_CS_H();                 //关闭片选
    TF_SendByte(DUMMY_BYTE);    //额外增加8个时钟

    return 0xF0;              //返回状态值
}




u8 TF_WriteMultiBlocks(u32 ByteAddr, const u8* buffer, u8 count)    // 写入多个块
{
    u8 result;
    u16 i = 0,retry16=0;
    u32 retry32=0;

    result = TF_SendCommand(ACMD23, count, 0);      // 预擦除命令, 使接下来的写入更块, 如果超过, 则会每次有新数据时擦除一块

    result = TF_SendCommand(CMD25, ByteAddr, 0);    // CMD25 多块写入
    if (result!=0x00) return result;

    SPI_CS_L();
    // 延时 3 Byte 准备
    TF_SendByte(DUMMY_BYTE);
    TF_SendByte(DUMMY_BYTE);
    TF_SendByte(DUMMY_BYTE);

    do{
        // 起始传输令牌 0xFC: 多块写入
        TF_SendByte(0xFC);
        for (i=0;i<512;i++)
        {
            TF_SendByte(*buffer);
            buffer++;
        }
        // CRC: 2Bytes
        TF_SendByte(DUMMY_BYTE);
        TF_SendByte(DUMMY_BYTE);
    
        // 等待 TF 卡结束应答
        retry16=0;
        do{
            result=TF_SendByte(DUMMY_BYTE)&0x1F;
            if ((result==TF_WRITE_CRC_ERROR)||(result==TF_WRITE_ERROR))
            {
                SPI_CS_H();
                return result;
            } 

            retry16++;
            if (retry16>0xFFF0) //超时
            {
                SPI_CS_H();
                return 1;
            }
        }while(result!=TF_WRITE_OK);        // TF 卡结束应答错误则继续循环
             
        // TF 卡结束应答正确 
        // 等待 TF 写入结束
        retry32=0;
        while(!TF_SendByte(DUMMY_BYTE))
        {
            retry32++;
            if (retry32>0xFFFFFFF0)       // 写入超时退出
            {
                SPI_CS_H();
                return 1;        
            }
        }
    }while(--count);
    
    // 全部传输完毕                    
    result=TF_SendByte(0xFD);   // 结束令牌 0xFD
    TF_SendByte(DUMMY_BYTE);

    while(!TF_SendByte(DUMMY_BYTE))     //结束令牌后 忙
    {
        retry32++;
        if (retry32>0xFFFFFFF0)         //超时退出
        {
            SPI_CS_H();
            return 1;        
        }
    }

    SPI_CS_H();                 //关闭片选
    TF_SendByte(DUMMY_BYTE);    //额外增加8个时钟

    return 0xF0;                //返回状态值
}


//=============================================
// FATFS 接口

DWORD get_fattime (void)            // 写入文件时需要给定的时间, 返回 GPS 时间
{
    u32 TimeNow=0x0000;
    // 返回值 32 bit
    // 3       2    2    1    1
    // 1       4    0    6    2    8    4    0
    // 0000 0000 0000 0000 0000 0000 0000 0000
    // |------||---||----| |----||-----||----|
    // 年-1980  月    日    小时  分钟   秒/2
    //  0-127   1-12 1-31   0-23  0-59   0-29
    u8 buff[6];

    GPS_GetTime(buff);          // 获取时间

    //TimeNow<<=7;
    TimeNow=2000+buff[0]-1980;  // 年

    TimeNow<<=4;
    TimeNow|= buff[1];          // 月

    TimeNow<<=5;
    TimeNow|= buff[2];          // 日

    TimeNow<<=5;
    TimeNow|= buff[3];          // 小时

    TimeNow<<=6;
    TimeNow|= buff[4];          // 分钟

    TimeNow<<=5;
    TimeNow|= (buff[5]/2);      // 秒

    return TimeNow;
}

DSTATUS disk_initialize (BYTE drv)
{
    return RES_OK;
}

DSTATUS disk_status (BYTE drv)
{
    return RES_OK;
}

DRESULT disk_read (BYTE drv, BYTE *buff, DWORD sector, BYTE count)
{
    LED_B_ON();         // TF 读取时 指示灯
    if(count==1)        // 1个 Sector的读操作      
    {

        TF_ReadBlock(sector<<9, buff);

    }
    else                //多个 Sector的读操作     
    {

        TF_ReadMultiBlocks(sector<<9, buff, count);

    }
    LED_B_OFF();
    return RES_OK; //
}

DRESULT disk_write (BYTE drv, const BYTE* buff, DWORD sector, BYTE count)
{
    LED_R_ON();         // TF 写入时 指示灯
    if(count==1)        // 1个 Sector的写操作      
    {

        TF_WriteBlock(sector<<9, buff);

    }
    else                // 多个 Sector的写操作     
    {

        TF_WriteMultiBlocks(sector<<9, buff, count);
    }
    LED_R_OFF();
    return RES_OK; //
}

DRESULT disk_ioctl (BYTE drv, BYTE ctrl, void *buff)
{
    return RES_OK;
}
