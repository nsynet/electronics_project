/******************** (C) COPYRIGHT 2012 WildFire Team **************************
 * 文件名  ：mp3play.c
 * 描述    ：mp3应用函数库         
 * 实验平台：野火STM32开发板
 * 硬件连接：-----------------
 *          |                 |
 *          |       无        |
 *          |                 |
 *           -----------------
 * 库版本  ：ST3.5.0
 * 作者    ：wildfire team 
 * 论坛    ：http://www.amobbs.com/forum-1008-1.html
 * 淘宝    ：http://firestm32.taobao.com
**********************************************************************************/
#include "mp3play.h"
extern unsigned char mp3_name[];
extern uint8_t SD_FLAG;




/*
 * 函数名：MP3_Play
 * 描述  ：读取SD卡里面的音频文件，并通过耳机播放出来
 *         支持的格式：mp3,mid,wma, 部分的wav
 * 输入  ：无
 * 输出  ：无
 * 说明  ：已添加支持长中文文件名
 */
void MP3_Play(void)
{
	FATFS fs;                  // Work area (file system object) for logical drive
	FRESULT res;
	UINT br;					  /*读取出的字节数，用于判断是否到达文件尾*/
	FIL fsrc;          		 // file objects
	FILINFO finfo;			 /*文件信息*/
//	unsigned char mp3_name[]={"Y0311000.mp3"};//默认MP3文件
//	unsigned char mp3_name1[]={"Y0000000.mp3"};
char j = 0;	
uint16_t count = 0;
BYTE buffer[512];          /* 存放读取出的文件数据 */
//	mp3_name1[1] = '0';
//	mp3_name1[2] = '3';
//  mp3_name1[3] = '1';
	//mp3_name1[4] = '1';
	//mp3_name1[5] = '0';
//	mp3_name1[6] = '0';
//mp3_name1[7] = '0';

	



	f_mount(0, &fs);
	res = f_open( &fsrc, mp3_name, FA_READ );
	while(res);
	br = 1;				  			     /* br 为全局变量 */
	TXDCS_SET( 0 );            /* 选择VS1003的数据接口	*/

	for (;;) 
	{			
		res = f_read( &fsrc, buffer, sizeof(buffer), &br );
		if ( res == 0 )
		{				
			count = 0;								/* 512字节完重新计数 */
			
			Delay_us( 1000 );         /* 10ms 延时 */
			SD_FLAG = 1;
			while ( count < 512)		  /* SD卡读取一个sector，一个sector为512字节 */	
			{ 						
				if ( DREQ != 0)	      /* 等待DREQ为高，请求数据输入 */
				{
				for (j=0; j<32; j++ ) /* VS1003的FIFO只有32个字节的缓冲 */
					{										
						VS1003_WriteByte( buffer[count] );										
						count++;
					}
				}
//				 if(SD_FLAG=0) break;
			}
		}
		if(SD_FLAG == 0) break;
		if (res || br == 0) break;   /* 出错或者到了MP3文件尾 */		   
	}
	count = 0;
	/* 根据VS1003的要求，在一曲结束后需发送2048个0来确保下一首的正常播放 */
	while ( count < 2048 )	
	{	
		if ( DREQ != 0 )
		{
			for ( j=0; j<32; j++ )
			{					   			
				VS1003_WriteByte( 0 );								
				count++;
			}
		}
	}
	count = 0;
	TXDCS_SET( 1 );   /* 关闭VS1003数据端口 */ 
	f_close(&fsrc);	 /* 关闭打开的文件 */
}


