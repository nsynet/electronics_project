#include "SPI_45db161.h"


void AT45DB161_init (void)
{
  df_SPI_init();
  //AT45DB161_REST_LOWD();
  //Delay_Ms(1);
  //AT45DB161_REST_HIGH();
  //AT45DB161_CS_HIGH();
  //Delay_Ms(1);
  //AT45DB161_CS_LOWD();
  //AT45DB161_WP_HIGH();
  //AT45DB161_WP_LOWD();
}

/*****************************************************************************
*
*   Function name : df_SPI_init
*
*   Returns :     None
*
*   Parameters :   None
*
*   Purpose :     Sets up the HW SPI in Master mode, Mode 3
*             Note -> Uses the SS line to control the DF CS-line.
*
******************************************************************************/
void df_SPI_init (void)
{
  SPI_InitTypeDef  SPI_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Enable SPI2 and GPIO clocks */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
    
  /* Configure SPI2 pins: SCK, MISO and MOSI */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//推挽式复用功能
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//推挽式输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  /* SPI2 configuration */
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; //SPI设置为双线双向全双工 
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master; //设置为主 SPI 
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b; //SPI发送接收 8 位帧结构 
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;  //时钟悬空高 
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge; //数据捕获于第二个时钟沿 
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;//内部 NSS 信号有 SSI位控制 
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;//波特率预分频值为 256 
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;//SPI_FirstBit_LSB;//数据传输从 MSB 位开始 
  SPI_InitStructure.SPI_CRCPolynomial = 7;//定义了用于 CRC值计算的多项式 7
  SPI_Init(SPI2, &SPI_InitStructure);

  /* Enable SPI2  */
  SPI_Cmd(SPI2, ENABLE);
}
/*****************************************************************************
*
*   Function name : DF_SPI_RW
*
*   Returns :     Byte read from SPI data register (any value)
*
*   Parameters :   Byte to be written to SPI data register (any value)
*
*   Purpose :     Read and writes one byte from/to SPI master
*
******************************************************************************/
u8 DF_SPI_RW (u8 byte)
{
  /* Loop while DR register in not emplty */
  while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);

  /* Send byte through the SPI1 peripheral */
  SPI_I2S_SendData(SPI2, byte);

  /* Wait to receive a byte */
  while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);

  /* Return the byte read from the SPI bus */
  return SPI_I2S_ReceiveData(SPI2);
}

/*****************************************************************************
*
*   Function name : Read_DF_status
*
*   Returns :     One status byte. Consult Dataflash datasheet for further
*             decoding info
*
*   Parameters :   None
*
*   Purpose :     Status info concerning the Dataflash is busy or not.
*             Status info concerning compare between buffer and flash page
*             Status info concerning size of actual device
*
******************************************************************************/
u8 Read_DF_status(void)
{
  u8 result;
  AT45DB161_CS_HIGH();
  AT45DB161_CS_LOWD();
  result = DF_SPI_RW(StatusReg);         //send status register read op-code 0x57
  result = DF_SPI_RW(0x00);              //dummy write to get result
  AT45DB161_CS_HIGH();
  return result;                   //return the read status register value
}
/*****************************************************************************
*
*   Function name : Page_To_Buffer
*
*   Returns :     None
*
*   Parameters :   BufferNo   ->   Decides usage of either buffer 1 or 2
*             PageAdr     ->   Address of page to be transferred to buffer
*
*   Purpose :     Transfers a page from flash to dataflash SRAM buffer
*             
******************************************************************************/
void Page_To_Buffer (unsigned int PageAdr)
{
    AT45DB161_CS_HIGH();
    AT45DB161_CS_LOWD();
    DF_SPI_RW(FlashToBuf1Transfer);                   //transfer to buffer 1 op-code  0x53
    DF_SPI_RW((unsigned char)(PageAdr >> 6));//(16 - PageBits)));   //upper part of page address
    DF_SPI_RW((unsigned char)(PageAdr << 2));//(PageBits - 8)));   //lower part of page address
    DF_SPI_RW(0x00);//don't cares
    while(!(Read_DF_status() & 0x80));//monitor the status register, wait until busy-flag is high
    AT45DB161_CS_HIGH();
}
/*****************************************************************************
*
*   Function name : Buffer_Read_Byte
*
*   Returns :     One read byte (any value)
*unsigned char Buffer_Read_Byte (unsigned int IntPageAdr)
*   Parameters :   BufferNo   ->   Decides usage of either buffer 1 or 2
*             IntPageAdr   ->   Internal page address
*
*   Purpose :     Reads one byte from one of the dataflash
*             internal SRAM buffers
*
******************************************************************************/
unsigned char Buffer_Read_Byte (unsigned int IntPageAdr)
{
    unsigned char data;
    data='0'; 
    AT45DB161_CS_HIGH();
    AT45DB161_CS_LOWD();  
    DF_SPI_RW(Buf1Read);             //buffer 1 read op-code  0x54
    DF_SPI_RW(0x00);                 //don't cares
    DF_SPI_RW((unsigned char)(IntPageAdr>>8));//upper part of internal buffer address
    DF_SPI_RW((unsigned char)(IntPageAdr));   //lower part of internal buffer address
    DF_SPI_RW(0x00);                 //don't cares
    data = DF_SPI_RW(0x00);             //read byte
    AT45DB161_CS_HIGH();
    return data;                     //return the read data byte
}
/*****************************************************************************
*
*   Function name : Buffer_Read_Str
*
*   Returns :     None
*
*   Parameters :   BufferNo   ->   Decides usage of either buffer 1 or 2
*             IntPageAdr   ->   Internal page address
*             No_of_bytes   ->   Number of bytes to be read
*             *BufferPtr   ->   address of buffer to be used for read bytes
*
*   Purpose :     Reads one or more bytes from one of the dataflash
*             internal SRAM buffers, and puts read bytes into
*             buffer pointed to by *BufferPtr
*
******************************************************************************/
void Buffer_Read_Str (unsigned int IntPageAdr, unsigned int No_of_bytes, unsigned char *BufferPtr)
{
    unsigned int i;
    AT45DB161_CS_HIGH();
    AT45DB161_CS_LOWD();
    
    DF_SPI_RW(Buf1Read);             //buffer 1 read op-code 0x54
    DF_SPI_RW(0x00);                 //don't cares
    DF_SPI_RW((unsigned char)(IntPageAdr>>8));//upper part of internal buffer address
    DF_SPI_RW((unsigned char)(IntPageAdr));   //lower part of internal buffer address
    DF_SPI_RW(0x00);                 //don't cares
    for( i=0; i<No_of_bytes; i++)
    {
        *(BufferPtr) = DF_SPI_RW(0x00);     //read byte and put it in AVR buffer pointed to by *BufferPtr
        BufferPtr++;                 //point to next element in AVR buffer
    }
   AT45DB161_CS_HIGH();
}
/*****************************************************************************
*
*   Function name : Buffer_Write_Enable
*
*   Returns :     None
*
*   Parameters :   IntPageAdr   ->   Internal page address to start writing from
*             BufferAdr   ->   Decides usage of either buffer 1 or 2
*             
*   Purpose :     Enables continous write functionality to one of the dataflash buffers
*             buffers. NOTE : User must ensure that CS goes high to terminate
*             this mode before accessing other dataflash functionalities 
*
******************************************************************************/
void Buffer_Write_Enable (unsigned int IntPageAdr)
{
    AT45DB161_CS_HIGH();
    AT45DB161_CS_LOWD();
    DF_SPI_RW(Buf1Write);             //buffer 1 write op-code  0x84
    DF_SPI_RW(0x00);                 //don't cares
    DF_SPI_RW((unsigned char)(IntPageAdr>>8));//upper part of internal buffer address
    DF_SPI_RW((unsigned char)(IntPageAdr));   //lower part of internal buffer address
    AT45DB161_CS_HIGH();
   
}
/*****************************************************************************
*
*   Function name : Buffer_Write_Byte
*
*   Returns :     None
*
*   Parameters :   IntPageAdr   ->   Internal page address to write byte to
*             BufferAdr   ->   Decides usage of either buffer 1 or 2
*             Data     ->   Data byte to be written
*
*   Purpose :     Writes one byte to one of the dataflash
*             internal SRAM buffers
*
******************************************************************************/
void Buffer_Write_Byte (unsigned int IntPageAdr, unsigned char Data)
{
    AT45DB161_CS_HIGH();
    AT45DB161_CS_LOWD();
    DF_SPI_RW(Buf1Write);             //buffer 1 write op-code 0x84
    DF_SPI_RW(0x00);                 //don't cares
    DF_SPI_RW((unsigned char)(IntPageAdr>>8));//upper part of internal buffer address
    DF_SPI_RW((unsigned char)(IntPageAdr));   //lower part of internal buffer address
    DF_SPI_RW(Data);                 //write data byte
    AT45DB161_CS_HIGH();
}
/*****************************************************************************
*
*   Function name : Buffer_Write_Str
*
*   Returns :     None
*
*   Parameters :   BufferNo   ->   Decides usage of either buffer 1 or 2
*             IntPageAdr   ->   Internal page address
*             No_of_bytes   ->   Number of bytes to be written
*             *BufferPtr   ->   address of buffer to be used for copy of bytes
*                         from AVR buffer to dataflash buffer 1 (or 2)
*
*   Purpose :     Copies one or more bytes to one of the dataflash
*             internal SRAM buffers from AVR SRAM buffer
*             pointed to by *BufferPtr
*
******************************************************************************/
void Buffer_Write_Str (unsigned int IntPageAdr, unsigned int No_of_bytes, unsigned char *BufferPtr)
{
    unsigned int i;
    AT45DB161_CS_HIGH();
    AT45DB161_CS_LOWD();
    DF_SPI_RW(Buf1Write);             //buffer 1 write op-code 0x84
    DF_SPI_RW(0x00);                 //don't cares
    DF_SPI_RW((unsigned char)(IntPageAdr>>8));//upper part of internal buffer address
    DF_SPI_RW((unsigned char)(IntPageAdr));   //lower part of internal buffer address
    for( i=0; i<No_of_bytes; i++)
    {
        DF_SPI_RW(*(BufferPtr));         //write byte pointed at by *BufferPtr to dataflash buffer 1 location
        BufferPtr++;                 //point to next element in AVR buffer
    }
    AT45DB161_CS_HIGH();
}

/*****************************************************************************
*
*   Function name : Buffer_To_Page
*
*   Returns :     None
*
*   Parameters :   BufferAdr   ->   Decides usage of either buffer 1 or 2
*             PageAdr     ->   Address of flash page to be programmed
*
*   Purpose :     Transfers a page from dataflash SRAM buffer to flash
*             
******************************************************************************/
void Buffer_To_Page (unsigned int PageAdr)
{
   AT45DB161_CS_HIGH();
   AT45DB161_CS_LOWD();
   DF_SPI_RW(Buf1ToFlashWE);                     //buffer 1 to flash with erase op-code  0x83
   DF_SPI_RW((unsigned char)(PageAdr >> 6));     //(16 - PageBits)));   //upper part of page address
   DF_SPI_RW((unsigned char)(PageAdr << 2));     //(PageBits - 8)));   //lower part of page address
   DF_SPI_RW(0x00);                              //don't cares
   while(!(Read_DF_status() & 0x80));            //monitor the status register, wait until busy-flag is high
   AT45DB161_CS_HIGH();
}


/*****************************************************************************
*
*   Function name : Cont_Flash_Read_Enable
*
*   Returns :     None
*
*   Parameters :   PageAdr     ->   Address of flash page where cont.read starts from
*             IntPageAdr   ->   Internal page address where cont.read starts from
*
*   Purpose :     Initiates a continuous read from a location in the DataFlash
*
******************************************************************************/
void Cont_Flash_Read_Enable (unsigned int PageAdr, unsigned int IntPageAdr)
{
   AT45DB161_CS_HIGH();
   AT45DB161_CS_LOWD();
   DF_SPI_RW(ContArrayRead);                                   //Continuous Array Read op-code  0x68
   DF_SPI_RW((unsigned char)(PageAdr >> 6));//(16 - PageBits)));                 //upper part of page address
   DF_SPI_RW((unsigned char)((PageAdr << 2)+ (IntPageAdr>>8))); //(PageBits - 8))+ (IntPageAdr>>8)));   //lower part of page address and MSB of int.page adr.
   DF_SPI_RW((unsigned char)(IntPageAdr));                           //LSB byte of internal page address
   DF_SPI_RW(0x00);                                         //perform 4 dummy writes
   DF_SPI_RW(0x00);                                         //in order to intiate DataFlash
   DF_SPI_RW(0x00);                                         //address pointers
   DF_SPI_RW(0x00);
   AT45DB161_CS_HIGH();
}
void DF_Read_Str (unsigned int Page_no, unsigned int Len_of_bytes, unsigned char *BufferPtr)
{
    //OS_ENTER_CRITICAL();
  
	Page_To_Buffer(Page_no);
        
	Buffer_Read_Str(0,Len_of_bytes,BufferPtr);
	//OS_EXIT_CRITICAL();
}
void DF_Write_Str (unsigned int Page_no, unsigned int Len_of_bytes, unsigned char *BufferPtr)
{
	//OS_ENTER_CRITICAL();
	Buffer_Write_Str(0,Len_of_bytes,BufferPtr);
	Buffer_To_Page(Page_no);
	//OS_EXIT_CRITICAL();
}