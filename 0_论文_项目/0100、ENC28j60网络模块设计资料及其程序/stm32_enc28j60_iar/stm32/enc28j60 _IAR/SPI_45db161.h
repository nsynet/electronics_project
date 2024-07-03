#include "stm32f10x_lib.h"

/*******************************************************************************/
#define GPIO_45DB161              GPIOB
#define AT45DB161_CS              GPIO_Pin_11
#define AT45DB161_CS_LOWD()       GPIO_ResetBits(GPIO_45DB161, AT45DB161_CS)
#define AT45DB161_CS_HIGH()       GPIO_SetBits(GPIO_45DB161, AT45DB161_CS)

#define StatusReg                 0x57   // Status register
#define FlashToBuf1Transfer       0x53   // Main memory page to buffer 1 transfer
#define Buf1Read                  0x54   // Buffer 1 read
#define Buf1Write                 0x84   // Buffer 1 write
#define Buf1ToFlashWE             0x83   // Buffer 1 to main memory page program with built-in erase
#define ContArrayRead             0x68   // Continuous Array Read (Note : Only A/B-parts supported)

/*******************************************************************************/

void          AT45DB161_init (void);
void          df_SPI_init (void);
u8            DF_SPI_RW (u8 byte);

u8            Read_DF_status(void);
void          Page_To_Buffer (unsigned int PageAdr);

unsigned char Buffer_Read_Byte (unsigned int IntPageAdr);
void          Buffer_Read_Str (unsigned int IntPageAdr, unsigned int No_of_bytes, unsigned char *BufferPtr);

void          Buffer_Write_Byte (unsigned int IntPageAdr, unsigned char Data);
void          Buffer_Write_Str (unsigned int IntPageAdr, unsigned int No_of_bytes, unsigned char *BufferPtr);
void          Buffer_To_Page (unsigned int PageAdr);

void          Cont_Flash_Read_Enable (unsigned int PageAdr, unsigned int IntPageAdr);     //什么作用?
void          Buffer_Write_Enable (unsigned int IntPageAdr);                              //什么作用?

void          DF_Read_Str (unsigned int Page_no, unsigned int Len_of_bytes, unsigned char *BufferPtr);
void          DF_Write_Str (unsigned int Page_no, unsigned int Len_of_bytes, unsigned char *BufferPtr);