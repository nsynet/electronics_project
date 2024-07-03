/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2007        */
/*-----------------------------------------------------------------------*/
/* This is a stub disk I/O module that acts as front end of the existing */
/* disk I/O modules and attach it to FatFs module with common interface. */
/*-----------------------------------------------------------------------*/

#include "diskio.h"
#include "stm32f42x_sd.h"
#include "string.h"
/*-----------------------------------------------------------------------*/
/* Correspondence between physical drive number and physical drive.      */
extern SD_CardInfo SDCardInfo;
/*-----------------------------------------------------------------------*/
/* Inidialize a Drive [�ú�������Ҫ��д��]                                                   */
/* ע�⣺�Ӻ����Ĵ�������������ν��drv�������������һ�������������ļ�Ӳ�������ͣ�����switch��俴
   drv = ATA = 0����ʾ��IDEӲ�̵�һ�ֽӿ�
   drv = MMC = 1: ��ʾ����MMC���Ľӿڣ�Ҳ����SD��
   drv = USB = 2: ��ʾ����USB�洢�豸�Ľӿ�
   �����￴��������Ӧ��Ҫѡ�����MMC�ӿ�,�����������Ϊ1*/
	 
#define BLOCK_SIZE            512 /* Block Size in Bytes */


DSTATUS disk_initialize (
	BYTE drv				/* Physical drive nmuber (0..) */
)
{	
  SD_Error Status;
  Status = SD_Init();
 
  if(Status == SD_OK)return 0;
  else return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Return Disk Status  [�ú�����Ҫ��д]                                                  */

DSTATUS disk_status (
	BYTE drv		/* Physical drive nmuber (0..) */
)
{
	return 0;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s) [�ú�����Ҫ��д]                                       */

DRESULT disk_read (
	BYTE drv,		/* Physical drive nmuber (0..) */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Sector address (LBA) */
	BYTE count		/* Number of sectors to read (1..255) */
)
{
	SD_Error Status = SD_OK;

//	if(count == 1 ){
//
//	    Status = SD_ReadBlock(buff, sector << 9, 512);
//#if defined (SD_DMA_MODE)
//	    /* Check if the Transfer is finished */
//	    Status = SD_WaitReadOperation();
//	    while(SD_GetStatus() != SD_TRANSFER_OK);
//#endif
//	}
//	else
	{
		
		/* Read block of many bytes from address 0 */
		Status = SD_ReadMultiBlocks(buff, sector << 9, 512, count);
#if defined (SD_DMA_MODE)
		/* Check if the Transfer is finished */
		Status = SD_WaitReadOperation();
		while(SD_GetStatus() != SD_TRANSFER_OK);
#endif
	}

	if(Status == SD_OK)
		return RES_OK;
	else 
		return RES_ERROR;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s) [�ú�����Ҫ��д]                                                      */

#if _READONLY == 0
DRESULT disk_write (
	BYTE drv,			/* Physical drive nmuber (0..) */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Sector address (LBA) */
	BYTE count			/* Number of sectors to write (1..255) */
)
{
	SD_Error Status = SD_OK;

	if(count == 1){
		/* Write block of 512 bytes on address 0 */

		Status =SD_WriteBlock((uint8_t *)buff,sector*BLOCK_SIZE, BLOCK_SIZE);
				/* Check if the Transfer is finished */
		SD_WaitWriteOperation();	  
		
		/* Wait until end of DMA transfer */
		while(SD_GetStatus() != SD_TRANSFER_OK);
		
#if defined (SD_DMA_MODE)
		/* Check if the Transfer is finished */
		Status = SD_WaitWriteOperation();
  	  while(SD_GetStatus() != SD_TRANSFER_OK);
#endif
	}
	else {
			/* Write multiple block of many bytes on address 0 */
			Status = SD_WriteMultiBlocks((u8 *)(&buff[0]), sector << 9, 512, count);
#if defined (SD_DMA_MODE)
			/* Check if the Transfer is finished */
			Status = SD_WaitWriteOperation();
			while(SD_GetStatus() != SD_TRANSFER_OK);
#endif
	}
     
	if(Status == SD_OK)
		return RES_OK;
	else 
		return RES_ERROR;
}

//DRESULT disk_write (
//	BYTE drv,			/* Physical drive nmuber (0..) */
//	const BYTE *buff,	/* Data to be written */
//	DWORD sector,		/* Sector address (LBA) */
//	BYTE count			/* Number of sectors to write (1..128) */
//)
//{
//	if (count > 1)
//	{
//		SD_WriteMultiBlocks((uint8_t *)buff, sector*BLOCK_SIZE, BLOCK_SIZE, count);
//		
//		/* Check if the Transfer is finished */
//		SD_WaitWriteOperation();	 
//		
//		/* Wait until end of DMA transfer */
//		while(SD_GetStatus() != SD_TRANSFER_OK); 
//	}
//	else
//	{
//		SD_WriteBlock((uint8_t *)buff,sector*BLOCK_SIZE, BLOCK_SIZE);
//		
//		/* Check if the Transfer is finished */
//		SD_WaitWriteOperation();	  
//		
//		/* Wait until end of DMA transfer */
//		while(SD_GetStatus() != SD_TRANSFER_OK);
//	}
//	return RES_OK;
//}

#endif /* _READONLY */



/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions [�ú�����Ҫ��д]                                              */

DRESULT disk_ioctl (
	BYTE drv,		/* Physical drive nmuber (0..) */
	BYTE ctrl,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	return RES_OK;
}

DWORD get_fattime ()
{
  return ((2015UL-1980) << 25)// Year
          | (8UL << 21)       // Month
          | (14UL << 16)      // Day
          | (9U << 11)        // Hour 
          | (0U << 5)         // Min
          | (0U >> 1)         // Sec
          ;
}
