/*-------------------------------------------------------
      TF ������ by wowbanui 2011

 �汾��ʷ:
 Rev.201108     SPI �ӿ�, ��ʼ�汾, ��֧�� SDHC
---------------------------------------------------------*/

#include "tf.h"
#include "GPS.h"
#include "FATFS/ff.h"
#include "FATFS/diskio.h"

// TF �� SPI CS Ƭѡ
#define SPI_CS_L() (GPIOA->BRR = GPIO_Pin_4)    //0 RESET
#define SPI_CS_H() (GPIOA->BSRR = GPIO_Pin_4)   //1 SET

// TF ��д�ٶ�
#define TF_SPEED_HIGH   SPI_BaudRatePrescaler_4
#define TF_SPEED_LOW    SPI_BaudRatePrescaler_256

#define CMD0_IDLE_ERROR 0x01;
	   
// SD��ָ���  	   
#define CMD0    0       //����λ
#define CMD1    1
#define CMD8    8
#define CMD9    9       // �� CSD
#define CMD10   10      // �� CID
#define CMD12   12      // ֹͣ���ݴ���
#define CMD16   16      // ���� SectorSize Ӧ����0x00
#define CMD17   17      // �� Sector
#define CMD18   18      // ����� Sector
#define ACMD23  23      // Ԥ����, ���ö�sectorд��ǰԤ�Ȳ���N��block
#define CMD24   24      // д Sector
#define CMD25   25      // д��� Sector
#define ACMD41  41      //
#define CMD55   55      //
#define CMD58   58      // ��OCR��Ϣ
#define CMD59   59      // ʹ��/��ֹCRC��Ӧ����0x00

#define DUMMY_BYTE  0xFF

void TF_SPI_Config(u16 Speed)           // ���� SPI
{
    SPI_InitTypeDef  SPI_InitStructure;

    //---------------------------------------------
    // SPI1 ���� ��Ӧ SD/TF��
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge; 
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;    
    //SPI_NSS_Hard��ʾNSS������Ϊһ��IO����
    SPI_InitStructure.SPI_BaudRatePrescaler = Speed;  //APB1�� ���ʹ��2��Ƶ
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;      //��λ��ǰ
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_Init(SPI1, &SPI_InitStructure);
}

static u8 TF_SendByte(u8 byte)          // �ײ㷢��/���պ���
{
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);   // Loop while DR register in not emplty
    SPI_I2S_SendData(SPI1, byte);                                     // Send byte through the SPI2 peripheral
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);  // Wait to receive a byte
    return SPI_I2S_ReceiveData(SPI1);                                 //Return the byte read from the SPI bus
}

u8 TF_SendCommand(u8 cmd, u32 arg, u8 crc)        //
{
    u8 result,retry = 0;

    SPI_CS_L();                 // Ƭѡ TF��

    TF_SendByte(cmd | 0x40);    // cmd
    TF_SendByte(arg >> 24);
    TF_SendByte(arg >> 16);
    TF_SendByte(arg >> 8);
    TF_SendByte(arg);
    TF_SendByte(crc);

    //�ȴ���Ӧ����ʱ�˳�
    do{
        result = TF_SendByte(DUMMY_BYTE);
        retry++;
        if(retry > 200) break;
    }while(result==0xFF); 

    SPI_CS_H();                 //�ر�Ƭѡ
    TF_SendByte(DUMMY_BYTE);    //��������8��ʱ��

    return result;                  //����״ֵ̬
}


u8 TF_Init(void)                        // IO ����
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
    //����ʱ��
    //RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);

    // ��ʼ��ʱʹ�õ���
    TF_SPI_Config(TF_SPEED_LOW);

    SPI_Cmd(SPI1, ENABLE);
    //=============================================

    //=============================================
    // TF ����ʼ����ʼ
    // ��ʱ > 74 CLK
    for(i=0;i<10;i++)               // 80 CLK
        TF_SendByte(0xFF);

    // ѭ������ CMD0(���� Idle ģʽ) ����, ���� 0x01 ������ɹ����� Idle ģʽ
    SPI_CS_L();                     // Ƭѡ TF��
    while(TF_SendCommand(CMD0, 0, 0x95)!=0x01)  
    {
        retry++;
        if (retry>200)
        {
            SPI_CS_H();             // �ͷ�Ƭѡ             
            return CMD0_IDLE_ERROR;      // ��ʱ, �˳�
        }
    }

    // �Ѿ����� Idle ģʽ

    // ���� CMD55+ACMD41
    retry=0;
    do{
        if (TF_SendCommand(CMD55, 0, 0)!=0x01) return result; 
        // SD_HIGH_CAPACITY 0x40000000
        result = TF_SendCommand(ACMD41, 0x40000000, 0);
        retry++;
        if (retry>200) return result;
    }while(result!=0x00);

    // �л�������
    TF_SPI_Config(TF_SPEED_HIGH);

    return 0xEE;
}

u8 TF_GetCID(u8* cid_data)              // ��ȡ CID
{
    u8 result,retry = 0;

    result = TF_SendCommand(CMD10, 0, 0xFF);
    if (result!=0x00) return result;

    SPI_CS_L();

    // �ȴ� 0xFE
    retry=0;
    while(TF_SendByte(DUMMY_BYTE)!=0xFE)
    {
        retry++;
        if(retry > 200) return 0xE2;    
    }

    // 16 Bytes CID ����
    retry=16;
    while(retry--)
    {
        *cid_data=TF_SendByte(DUMMY_BYTE);
        cid_data++;
    }

    // CRC: 2Bytes
    TF_SendByte(DUMMY_BYTE);
    TF_SendByte(DUMMY_BYTE);

    SPI_CS_H();                 //�ر�Ƭѡ
    TF_SendByte(DUMMY_BYTE);    //��������8��ʱ��

    return 0x00;              //����״ֵ̬
}


u8 TF_ReadBlock(u32 ByteAddr, u8* buffer)                   // ����
{
    u8 result;
    u16 retry = 0;

    result = TF_SendCommand(CMD17, ByteAddr, 0);
    if (result!=0x00) return result;

    SPI_CS_L();

    // �ȴ� 0xFE
    retry=0;
    while(TF_SendByte(DUMMY_BYTE)!=0xFE)
    {
        retry++;
        if(retry > 200) return 0xE2;    
    }

    // 16 Bytes CID ����
    retry=512;
    while(retry--)
    {
        *buffer=TF_SendByte(DUMMY_BYTE);
        buffer++;
    }

    // CRC: 2Bytes
    TF_SendByte(DUMMY_BYTE);
    TF_SendByte(DUMMY_BYTE);

    SPI_CS_H();                 //�ر�Ƭѡ
    TF_SendByte(DUMMY_BYTE);    //��������8��ʱ��

    return 0xF0;              //����״ֵ̬
}



u8 TF_ReadMultiBlocks(u32 ByteAddr, u8* buffer, u8 count)   // �������
{
    u8 result;
    u16 retry = 0;

    result = TF_SendCommand(CMD18, ByteAddr, 0);
    if (result!=0x00) return result;

    SPI_CS_L();

    do{
        // �ȴ� 0xFE
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

    TF_SendCommand(CMD12, 0, 0);    //����ֹͣ����

    SPI_CS_H();                     //�ر�Ƭѡ
    TF_SendByte(DUMMY_BYTE);        //��������8��ʱ��

    return 0xF0;                    //����״ֵ̬
}








#define TF_WRITE_OK             0x05
#define TF_WRITE_CRC_ERROR      0x0B
#define TF_WRITE_ERROR          0x0D
#define TF_WRITE_OTHER_ERROR    0xFF

u8 TF_WriteBlock(u32 ByteAddr, const u8* buffer)            // д���
{
    u8 result;
    u16 i = 0;

    result = TF_SendCommand(CMD24, ByteAddr, 0);
    if (result!=0x00) return result;

    SPI_CS_L();

    // ��ʱ 3 Byte
    TF_SendByte(DUMMY_BYTE);
    TF_SendByte(DUMMY_BYTE);
    TF_SendByte(DUMMY_BYTE);

    // ��ʼ�������� 0xFE
    TF_SendByte(0xFE);
    for (i=0;i<512;i++)
    {
        TF_SendByte(*buffer);
        buffer++;
    }
    // CRC: 2Bytes
    TF_SendByte(DUMMY_BYTE);
    TF_SendByte(DUMMY_BYTE);

    // �ȴ� TF ������Ӧ��
    result = TF_SendByte(DUMMY_BYTE);
    if ((result&0x1F)!=TF_WRITE_OK)    //TF ������Ӧ�����
    {   
        SPI_CS_H();
        return result;
    }
    // TF ������Ӧ����ȷ 

    // �ȴ� TF ��æ
    i=0;
    while(!TF_SendByte(DUMMY_BYTE))
    {
        i++;
        if (i>0xFFF0)       // д�볬ʱ�˳�
        {
            SPI_CS_H();
            return 1;        
        }
    }

    SPI_CS_H();                 //�ر�Ƭѡ
    TF_SendByte(DUMMY_BYTE);    //��������8��ʱ��

    return 0xF0;              //����״ֵ̬
}




u8 TF_WriteMultiBlocks(u32 ByteAddr, const u8* buffer, u8 count)    // д������
{
    u8 result;
    u16 i = 0,retry16=0;
    u32 retry32=0;

    result = TF_SendCommand(ACMD23, count, 0);      // Ԥ��������, ʹ��������д�����, �������, ���ÿ����������ʱ����һ��

    result = TF_SendCommand(CMD25, ByteAddr, 0);    // CMD25 ���д��
    if (result!=0x00) return result;

    SPI_CS_L();
    // ��ʱ 3 Byte ׼��
    TF_SendByte(DUMMY_BYTE);
    TF_SendByte(DUMMY_BYTE);
    TF_SendByte(DUMMY_BYTE);

    do{
        // ��ʼ�������� 0xFC: ���д��
        TF_SendByte(0xFC);
        for (i=0;i<512;i++)
        {
            TF_SendByte(*buffer);
            buffer++;
        }
        // CRC: 2Bytes
        TF_SendByte(DUMMY_BYTE);
        TF_SendByte(DUMMY_BYTE);
    
        // �ȴ� TF ������Ӧ��
        retry16=0;
        do{
            result=TF_SendByte(DUMMY_BYTE)&0x1F;
            if ((result==TF_WRITE_CRC_ERROR)||(result==TF_WRITE_ERROR))
            {
                SPI_CS_H();
                return result;
            } 

            retry16++;
            if (retry16>0xFFF0) //��ʱ
            {
                SPI_CS_H();
                return 1;
            }
        }while(result!=TF_WRITE_OK);        // TF ������Ӧ����������ѭ��
             
        // TF ������Ӧ����ȷ 
        // �ȴ� TF д�����
        retry32=0;
        while(!TF_SendByte(DUMMY_BYTE))
        {
            retry32++;
            if (retry32>0xFFFFFFF0)       // д�볬ʱ�˳�
            {
                SPI_CS_H();
                return 1;        
            }
        }
    }while(--count);
    
    // ȫ���������                    
    result=TF_SendByte(0xFD);   // �������� 0xFD
    TF_SendByte(DUMMY_BYTE);

    while(!TF_SendByte(DUMMY_BYTE))     //�������ƺ� æ
    {
        retry32++;
        if (retry32>0xFFFFFFF0)         //��ʱ�˳�
        {
            SPI_CS_H();
            return 1;        
        }
    }

    SPI_CS_H();                 //�ر�Ƭѡ
    TF_SendByte(DUMMY_BYTE);    //��������8��ʱ��

    return 0xF0;                //����״ֵ̬
}


//=============================================
// FATFS �ӿ�

DWORD get_fattime (void)            // д���ļ�ʱ��Ҫ������ʱ��, ���� GPS ʱ��
{
    u32 TimeNow=0x0000;
    // ����ֵ 32 bit
    // 3       2    2    1    1
    // 1       4    0    6    2    8    4    0
    // 0000 0000 0000 0000 0000 0000 0000 0000
    // |------||---||----| |----||-----||----|
    // ��-1980  ��    ��    Сʱ  ����   ��/2
    //  0-127   1-12 1-31   0-23  0-59   0-29
    u8 buff[6];

    GPS_GetTime(buff);          // ��ȡʱ��

    //TimeNow<<=7;
    TimeNow=2000+buff[0]-1980;  // ��

    TimeNow<<=4;
    TimeNow|= buff[1];          // ��

    TimeNow<<=5;
    TimeNow|= buff[2];          // ��

    TimeNow<<=5;
    TimeNow|= buff[3];          // Сʱ

    TimeNow<<=6;
    TimeNow|= buff[4];          // ����

    TimeNow<<=5;
    TimeNow|= (buff[5]/2);      // ��

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
    LED_B_ON();         // TF ��ȡʱ ָʾ��
    if(count==1)        // 1�� Sector�Ķ�����      
    {

        TF_ReadBlock(sector<<9, buff);

    }
    else                //��� Sector�Ķ�����     
    {

        TF_ReadMultiBlocks(sector<<9, buff, count);

    }
    LED_B_OFF();
    return RES_OK; //
}

DRESULT disk_write (BYTE drv, const BYTE* buff, DWORD sector, BYTE count)
{
    LED_R_ON();         // TF д��ʱ ָʾ��
    if(count==1)        // 1�� Sector��д����      
    {

        TF_WriteBlock(sector<<9, buff);

    }
    else                // ��� Sector��д����     
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
