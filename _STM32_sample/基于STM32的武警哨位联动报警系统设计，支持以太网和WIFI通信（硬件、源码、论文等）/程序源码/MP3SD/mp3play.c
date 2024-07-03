/******************** (C) COPYRIGHT 2012 WildFire Team **************************
 * �ļ���  ��mp3play.c
 * ����    ��mp3Ӧ�ú�����         
 * ʵ��ƽ̨��Ұ��STM32������
 * Ӳ�����ӣ�-----------------
 *          |                 |
 *          |       ��        |
 *          |                 |
 *           -----------------
 * ��汾  ��ST3.5.0
 * ����    ��wildfire team 
 * ��̳    ��http://www.amobbs.com/forum-1008-1.html
 * �Ա�    ��http://firestm32.taobao.com
**********************************************************************************/
#include "mp3play.h"
extern unsigned char mp3_name[];
extern uint8_t SD_FLAG;




/*
 * ��������MP3_Play
 * ����  ����ȡSD���������Ƶ�ļ�����ͨ���������ų���
 *         ֧�ֵĸ�ʽ��mp3,mid,wma, ���ֵ�wav
 * ����  ����
 * ���  ����
 * ˵��  �������֧�ֳ������ļ���
 */
void MP3_Play(void)
{
	FATFS fs;                  // Work area (file system object) for logical drive
	FRESULT res;
	UINT br;					  /*��ȡ�����ֽ����������ж��Ƿ񵽴��ļ�β*/
	FIL fsrc;          		 // file objects
	FILINFO finfo;			 /*�ļ���Ϣ*/
//	unsigned char mp3_name[]={"Y0311000.mp3"};//Ĭ��MP3�ļ�
//	unsigned char mp3_name1[]={"Y0000000.mp3"};
char j = 0;	
uint16_t count = 0;
BYTE buffer[512];          /* ��Ŷ�ȡ�����ļ����� */
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
	br = 1;				  			     /* br Ϊȫ�ֱ��� */
	TXDCS_SET( 0 );            /* ѡ��VS1003�����ݽӿ�	*/

	for (;;) 
	{			
		res = f_read( &fsrc, buffer, sizeof(buffer), &br );
		if ( res == 0 )
		{				
			count = 0;								/* 512�ֽ������¼��� */
			
			Delay_us( 1000 );         /* 10ms ��ʱ */
			SD_FLAG = 1;
			while ( count < 512)		  /* SD����ȡһ��sector��һ��sectorΪ512�ֽ� */	
			{ 						
				if ( DREQ != 0)	      /* �ȴ�DREQΪ�ߣ������������� */
				{
				for (j=0; j<32; j++ ) /* VS1003��FIFOֻ��32���ֽڵĻ��� */
					{										
						VS1003_WriteByte( buffer[count] );										
						count++;
					}
				}
//				 if(SD_FLAG=0) break;
			}
		}
		if(SD_FLAG == 0) break;
		if (res || br == 0) break;   /* ������ߵ���MP3�ļ�β */		   
	}
	count = 0;
	/* ����VS1003��Ҫ����һ���������跢��2048��0��ȷ����һ�׵��������� */
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
	TXDCS_SET( 1 );   /* �ر�VS1003���ݶ˿� */ 
	f_close(&fsrc);	 /* �رմ򿪵��ļ� */
}


