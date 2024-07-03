/******************** (C) COPYRIGHT 2012 WildFire Team **************************
 * �ļ���  ��main.c
 * ����    ��ͨ�����Եĳ����ն���Ƭ���������ݣ���Ƭ�������յ������ݷ��ص��ԣ�
 *         ������ʾ�ڳ����ն��ϡ��õ��Ǵ���1���жϷ�ʽ��         
 * ʵ��ƽ̨��Ұ��STM32������
 * ��汾  ��ST3.5.0
 *
 * ����    ��wildfire team 
 * ��̳    ��http://www.amobbs.com/forum-1008-1.html
 * �Ա�    ��http://firestm32.taobao.com
*********************************************************************************/
#include "stm32f10x.h"
#include "usart1.h"
#include "gpio_key.h"
#include "mp3play.h "
#include "Touch.h"
#include "lcd.h" 

#define SRC_ADDR   2  //Ĭ�ϵ�Դ�ն˱���ֽ�Ϊ3������ʾ1����λ�ֻ�
#define DATA_BUFF_LEN   20

//unsigned int analysis_data;
unsigned char mp3_name[]={"Y0000000.mp3"};//Ĭ��MP3�ļ�
unsigned char char_name[]={"C0000000.txt"};//Ĭ���ַ���ʾ�ļ�
unsigned char led_name[]={"L0280000.txt"};//Ĭ���ַ���ʾ�ļ�
unsigned char standard_char[]={"0123456789ABCDEF"};
unsigned char lcd_name[]={"D0000000.txt"};
BYTE buffer[512];          /* ��Ŷ�ȡ�����ļ����� */


unsigned char processing_data_buff[DATA_BUFF_LEN];
unsigned char SX_flag=1;//ʵս/ѵ����־λ��SX_flag=1Ϊʵս״̬��SX_flag=0Ϊѵ��״̬
unsigned char SQGD_flag=0;//���빩����־λ��SQGD_flag=1��ʾ���빩����SQGD_flag=0��ʾû�����빩��	  
unsigned char YJKD_flag=0;//һ��������־λ��YJKD_flag=1��ʾ��Ҫ������YJKD_flag=0��ʾ���ÿ���

uint8_t USART1_RECEIVE_NUM = 0;
uint8_t receive_data_buff[20];
uint8_t USART1_RECEIVE_FLAG=0;
uint8_t SD_FLAG = 1;

uint8_t UART0_RECEIVE_DATA(void);
unsigned char data_analysis( unsigned int *analysis_data );
void data_processing(void);
void LCD_Display(void);
/*
 * ��������main
 * ����  ��������
 * ����  ����
 * ���  ����
 */
int main(void)
{	
	SysTick_Init();	
	/* USART1 config 1200 8-N-1 */
	USART1_Config();
	GPIO_INIT();
  NVIC_ConfigurationUART();
	NVIC_ConfigurationSD();
	VS1003_SPI_Init();		/* MP3Ӳ��I/O��ʼ�� */ 
	
	MP3_Start(); 			/* MP3������׼�����ţ���vs1003.cʵ�� */ 
		LCD_Init();							/* LCD ��ʼ��*/	
	Touch_init();						/*������ʼ��*/
	
	while(Touchl_Calibrate() !=0);	 	/*�ȴ�������У׼���*/	
	Set_direction(0);	 //����Ϊ����
	LCD_SHOW_INIT();    //��ʾ��ʼ����
	
	while(1)
	{
		if(UART0_RECEIVE_DATA())
		{
			SD_FLAG=1;
			USART1_RECEIVE_NUM = 0;
			USART1_RECEIVE_FLAG=0;
			data_processing();
		}
	}
}

/*
 * ��������main
 * ����  �����ڽ���
 * ����  ����
 * ���  ������״̬
 */
uint8_t UART0_RECEIVE_DATA(void)
{
	
	uint8_t uart1_RECEIVE_i,uart1_RECEIVE_j;
  do
	{

	}
	while(USART1_RECEIVE_NUM < 1);//���յ�һ����ַ�ֽ�
//	SD_FLAG = 0;
  for(uart1_RECEIVE_i=0;(uart1_RECEIVE_i<500) && (USART1_RECEIVE_NUM < 20);uart1_RECEIVE_i++)//��500mS��(48MHz������)�����ղ�����ʣ�µ�5���ֽ����ݣ�����ս������飬ͬʱ���ؽ���״̬
  {
		for(uart1_RECEIVE_j=0;(uart1_RECEIVE_j<2368) && (USART1_RECEIVE_NUM < 20);uart1_RECEIVE_j++)
    {
    }
	}	
	if((uart1_RECEIVE_i >= 1000) || (receive_data_buff[19]!=0xC5))
	{
       USART1_RECEIVE_FLAG=0;
	}
	else
		USART1_RECEIVE_FLAG=1;
	return(USART1_RECEIVE_FLAG);
}

void display_play(void)
{
	LCD_Display();
	MP3_Play();
}


void LCD_Display(void)
{
	
//	unsigned char led_show[20];
	FATFS fs;                  // Work area (file system object) for logical drive
	FRESULT res;
	UINT br;					  /*��ȡ�����ֽ����������ж��Ƿ񵽴��ļ�β*/
	FIL fsrc;          		 // file objects
	f_mount(0, &fs);
	res = f_open( &fsrc, lcd_name, FA_READ );
	while(res);
	br = 1;				  			     /* br Ϊȫ�ֱ��� */
	
	while(1) 
	{			 
  	res = f_read( &fsrc, lcd_name, sizeof(lcd_name), &br ); /* ���ļ���������ݶ��������� */						
  	if (res || br == 0) break;                          /* ������ߵ����ļ�β */

  }	
	f_close(&fsrc);	 /* �رմ򿪵��ļ� */	
	LCD_Rectangle(0,0,320,120,WHITE);	/*��ʾ�������*/
	LCD_Str_ENCH_O(88,50,lcd_name,0);
}
//-----------------------------------------------------------------------------��-------------------------------
// ���ݷ�������
//--------------------------------------------------------------------------------------------------------------------
//˵������ȫ�ֱ���data_analysis�������һ����Ԫ��λ�ã������һ������λΪ���λΪ1�������1����һ������λΪ���λΪ1��
//�����8������λȫΪ0�������0��ͬʱ�����data_analysis�ĵ�һ������Ԫ��
unsigned char data_analysis( unsigned int * analysis_data )
{
    unsigned char i;
	//����������߷���λλ��
	if(*analysis_data == 0)
	{
	   return(0);
	}
	else
	{
		for(i=1;(*analysis_data>>(16-i))==0;i++)
		{
	
		};
		*analysis_data=(*analysis_data & (0xffff>>i));
		return(i);
	}
}


//-----------------------------------------------------------------------------��-------------------------------
// ���ݷ����봦����
//-------------------------------------------------------------------------------------------------------------
//˵�������յ���20��UART0���ݣ���receive_data_buff��������processing_data_buff������,
//Ȼ����һ�����������ݣ���ִ�ж�Ӧ�̵����Ŀ��ϣ���ɺ�processing_data_buff�д����Ķ�Ӧλ���㣬
//���ż�����������processing_data_buff�к��������
void data_processing(void)
{
	unsigned char i,j;
	unsigned int analysis_data_a;
	unsigned int analysis_data_b;
	unsigned int *analysis_data_i;
	unsigned int *analysis_data_j;
	unsigned char a;
	//����洢ת��
	for(a=0; a<DATA_BUFF_LEN ; a++)
	{
		processing_data_buff[a]= receive_data_buff[a];
	}
	if((processing_data_buff[17] & 0x03)==0x02)//ʵս/ѵ��״̬��־
	    SX_flag=1;	//����ʵս״̬
	if((processing_data_buff[17] & 0x03)==0x01)//ʵս/ѵ��״̬��־
	    SX_flag=0;	//����ѵ��״̬
	
	//�ϱ���̫����Ϣ�ֽڴ���
	if(processing_data_buff[16] & 0x0f)
	{
	    //һ�������ֽڴ���
		//�����ֽ���ϵõ�16λ��������
		analysis_data_a = processing_data_buff[3];
		analysis_data_a = analysis_data_a<<8;
		analysis_data_a = analysis_data_a | processing_data_buff[4];
		analysis_data_i = &analysis_data_a;
		//���Ǳ����������Ҫȷ�������ֽڶ�Ӧ��һ��ת���ַ�(�ļ����ĵ�5λ)
		//�����ֽ���ϵõ�16λ��������
		analysis_data_b=processing_data_buff[5];
		analysis_data_b=analysis_data_b<<8;
		analysis_data_b=analysis_data_b|processing_data_buff[6];
		analysis_data_j = &analysis_data_b;
		
		if(*analysis_data_i!=0)
		{
			do
			{
				i=data_analysis( analysis_data_i );
				if(i==1)
				 {
					YJKD_flag=1;				
				  GPIO_SetBits(GPIOF,GPIO_Pin_5);   //���Ŵ�
				  GPIO_SetBits(GPIOF,GPIO_Pin_1);   //������
				 }
				 if(i==8)
				 {
					YJKD_flag=0;
				 }

				 
				 //ȷ��mp3���ַ��ļ����ļ���
				 //(1)ȷ���ն˱�Ŷ�Ӧ������ת���ַ�(�ļ����ĵ�2��3λ)
				 char_name[1]= standard_char[processing_data_buff[1]>>4];
				 mp3_name[1]=	char_name[1];
				 led_name[1]=	char_name[1];
				 lcd_name[1]= char_name[1];
				 
				 char_name[2]= standard_char[processing_data_buff[1] & 0x0f];
				 mp3_name[2]= char_name[2];
				 led_name[2]=	char_name[2];	
				 lcd_name[2]= char_name[2];		
				 
				 //(2)ȷ�������ֽڶ�Ӧ��һ��ת���ַ�(�ļ����ĵ�4λ)
				 char_name[3]=standard_char[i];
				 mp3_name[3]=	char_name[3];
				 led_name[3]=	char_name[3];
				 lcd_name[3]= char_name[3];		

				 do
				 {
					 if( i== 1 )
					 {
						 //����������߷���λλ�ã�������
						 j=data_analysis( analysis_data_j );
						 //ȷ�������ֽڶ�Ӧ��һ��ת���ַ�(�ļ����ĵ�5λ)
						 char_name[4]=standard_char[j];
						 mp3_name[4]=	char_name[4];
						 led_name[4]=	char_name[4];	
						 lcd_name[4]= char_name[4];		
						 
						 //��������λ
							switch(j)
							{
								case 1 :
								{ 
									GPIO_SetBits(GPIOG,GPIO_Pin_0);//����ָʾ�Ƶ���
									if(SX_flag)//����ʵս״̬�������ر���
									{
											GPIO_SetBits(GPIOG,GPIO_Pin_1);//�����ϱ�����
									} 
									if(receive_data_buff[11]==0xff)
									{
										 SQGD_flag=1;
									}
									YJKD_flag=1;
									break;
								}
								case 2 :
								{
									GPIO_SetBits(GPIOG,GPIO_Pin_2);//����ָʾ�Ƶ���
									if(SX_flag)//����ʵս״̬�������ر���
									{
											GPIO_SetBits(GPIOG,GPIO_Pin_3);//�����ϱ�����
									} 
									if(receive_data_buff[11]==0xff)
									{
										 SQGD_flag=1;
									}
									YJKD_flag=1;
									break;
								}
								case 3 :
								{
									GPIO_SetBits(GPIOG,GPIO_Pin_4);//Ю��ָʾ�Ƶ���
									if(SX_flag)//����ʵս״̬�������ر���
									{
											GPIO_SetBits(GPIOG,GPIO_Pin_5);//Ю���ϱ�����
									} 
									if(receive_data_buff[11]==0xff)
									{
										 SQGD_flag=1;
									}
									YJKD_flag=1;
									break;
								}
								case 4 :
								{
									GPIO_SetBits(GPIOG,GPIO_Pin_6);//Ϯ��ָʾ�Ƶ���
									if(SX_flag)//����ʵս״̬�������ر���
									{
											GPIO_SetBits(GPIOG,GPIO_Pin_7);//Ϯ���ϱ�����
									} 
									if(receive_data_buff[11]==0xff)
									{
										 SQGD_flag=1;
									}
									YJKD_flag=1;
									break;
								}
								case 5 :
								{
									GPIO_SetBits(GPIOG,GPIO_Pin_8);//�ֺ�ָʾ�Ƶ���
									if(SX_flag)//����ʵս״̬�������ر���
									{
											GPIO_SetBits(GPIOG,GPIO_Pin_9);//�ֺ��ϱ�����
									} 
									if(receive_data_buff[11]==0xff)
									{
										 SQGD_flag=1;
									}
									YJKD_flag=1;
									break;
								}
								case 6 :
								{
									GPIO_SetBits(GPIOG,GPIO_Pin_10);//����ָʾ�Ƶ���
									if(SX_flag)//����ʵս״̬�������ر���
									{
											GPIO_SetBits(GPIOG,GPIO_Pin_11);//Ԥ���ϱ�����
									}
									if(receive_data_buff[11]==0xff)
									{
										 SQGD_flag=1;
									}
									YJKD_flag=1;
									break;
								}
								case 7 :
								{
									GPIO_SetBits(GPIOG,GPIO_Pin_12);//����ָʾ�Ƶ���
									if(SX_flag)//����ʵս״̬�������ر���
									{
											GPIO_SetBits(GPIOG,GPIO_Pin_13);//Ԥ���ϱ�����
									} 
									if(receive_data_buff[11]==0xff)
									{
										 SQGD_flag=1;
									}
									YJKD_flag=1;
									break;
								}
								default :break;
							 }
						 }
					 else   //i!=1 �ޱ�������
					 {
						 char_name[4]=standard_char[0];
						 mp3_name[4]=	char_name[4];
						 led_name[4]=	char_name[4];
						 lcd_name[4]= char_name[4];		
					 }
						//(4)�ļ���ʣ��ת���ַ�ȫ��ת��Ϊ�ַ�0(�ļ����ĵ�6��7��8λ)			 
					 char_name[5]=standard_char[0];
					 mp3_name[5]=	char_name[5];
					 led_name[5]=	char_name[5];
					 lcd_name[5]= char_name[5];		
					 
					 char_name[6]=standard_char[0];
					 mp3_name[6]=	char_name[6];
					 led_name[6]=	char_name[6];
					 lcd_name[6]= char_name[6];		
					 
					 char_name[7]=standard_char[0];
					 mp3_name[7]=	char_name[7];
					 led_name[7]=	char_name[7];
					 lcd_name[7]= char_name[7];		
				 
					 display_play();
				}
				while(*analysis_data_j != 0 );
			 
			 if(i == 2)  //һ����������
			 {
				 if(YJKD_flag==1)
				 {
					 GPIO_SetBits(GPIOG,GPIO_Pin_14);//���ӵ���
					 GPIO_SetBits(GPIOG,GPIO_Pin_15);//��ǹ����
					 Delay_us(5000000);
					 GPIO_ResetBits(GPIOG,GPIO_Pin_14);//�ر��ӵ���
					 GPIO_ResetBits(GPIOG,GPIO_Pin_15);//�رյ����
					 SQGD_flag=0;
					 YJKD_flag=0;
				 }
			 }
			 if( i == 3 ) //����ͷ�л�����
			 {
					GPIOF->ODR ^= (1<<0);      //ȡ��
			 }
			 if( i ==  4 ) //��������1��ȫװ��
			 {
					GPIO_SetBits(GPIOF,GPIO_Pin_1);//������
	//				 TMR2CN=0x04;//������ʱ��02
					GPIO_SetBits(GPIOG,GPIO_Pin_15);		//��ǹ����		 
					Delay_us(5000000);					 
					GPIO_ResetBits(GPIOG,GPIO_Pin_15);	  //�ر�ǹ����
			 }
			 if(i == 5 ) //��������2����װ��
			 {
				 GPIO_SetBits(GPIOF,GPIO_Pin_1);//������
//				 TMR2CN=0x04;//������ʱ��02
			 }
			 if(i == 6 ) //��������3��ͽ�֣�
			 {
					GPIO_SetBits(GPIOF,GPIO_Pin_1);//������
//				  TMR2CN=0x04;//������ʱ��02
			 }
			 //����Ѳ���������ȫ��������Ŀ�긺��
			 if(i == 8 )  //ȫ��λ����
			 {
				 GPIO_Write(GPIOG,0x0000); 
				 SQGD_flag=0;
				 YJKD_flag=0;
			 }
						//9����Ӧ�ӵ���������
				if(i == 9 )
				{
					 if(SQGD_flag==1)
					 {
							GPIO_SetBits(GPIOG,GPIO_Pin_14);//���ӵ���
							Delay_us(5000000);
							GPIO_ResetBits(GPIOG,GPIO_Pin_14); //�ر��ӵ���
						SQGD_flag=0;
					 }	     
				}
				//10~11������λ�㲥�����
				if(i == 10 )
				{
					GPIO_SetBits(GPIOF,GPIO_Pin_2);//����λ�㲥			 		     
				}
				if(i == 11 )
				{
					GPIO_ResetBits(GPIOF,GPIO_Pin_2);//�ر���΢�㲥
				}
				//12���򿪼���
				if(i == 12)
				{
					 GPIO_SetBits(GPIOF,GPIO_Pin_3);//�򿪼���	JDQ27
					 Delay_us(5000000);
					 GPIO_ResetBits(GPIOF,GPIO_Pin_3);			 		     
				}
				//13��Ԥ��
				//14��Ԥ��
				//15��Ԥ��
				//16��Ԥ��
			}
			while( *analysis_data_i != 0 );
		}

			//���������ֽڴ���
			//��ϵõ�16λ��������
			analysis_data_a=processing_data_buff[7];
	    analysis_data_a=analysis_data_a<<8;
	    analysis_data_a=analysis_data_a | processing_data_buff[8];
			analysis_data_i = &analysis_data_a;
		
			if(*analysis_data_i!=0)
			{
				do
			 {
			   //����������߷���λλ�ã�������
			   i=data_analysis( analysis_data_i );
			   if(i==1) //1�����󹩵�
			   {
					//���ö�Ӧ�̵���
						if(receive_data_buff[11]==0xff)
						{
							 SQGD_flag=1;
						}
						YJKD_flag=1;
						GPIO_SetBits(GPIOF,GPIO_Pin_4);   //���빩��
			   }
					//2���ܿ������������				
				 if( i==2 )
				 {
						//���ö�Ӧ�̵���
				 }
				 	//3��Ԥ��
					//4��Ԥ��
					//5��Ԥ��
					//6��Ԥ��
					//7��Ԥ��
					//8��Ԥ��
					//9��Ԥ��
					//10��Ԥ��
					//11��Ԥ��
					//12��Ԥ��
					//13��Ԥ��
					//14��Ԥ��
					//15��Ԥ��
					//16��Ԥ��
			   //ȷ��mp3���ַ��ļ����ļ���
			   //(1)ȷ���ն˱�Ŷ�Ӧ������ת���ַ�(�ļ����ĵ�2��3λ)
			   char_name[2]=standard_char[processing_data_buff[1]>>4];
			   mp3_name[2]=	char_name[2];
			   led_name[2]=	char_name[2];	
				 lcd_name[2]= char_name[2];		
				 
			   char_name[3]=standard_char[processing_data_buff[1] & 0x0f];
			   mp3_name[3]=char_name[3];
			   led_name[3]=	char_name[3];
				 lcd_name[3]= char_name[3];	
				 
			   //(2)�����ֽڶ�Ӧ��һ��ת���ַ�Ϊ�ַ�0(�ļ����ĵ�4λ)
			   char_name[4]=standard_char[0];
			   mp3_name[4]=	char_name[4];
			   led_name[4]=	char_name[4];
				 lcd_name[4]= char_name[4];	
				 
			   //(3)�����ֽڶ�Ӧ��һ��ת���ַ�Ϊ�ַ�0(�ļ����ĵ�5λ)
			   char_name[5]=standard_char[0];
			   mp3_name[5]=	char_name[5];
			   led_name[5]=	char_name[5];
				 lcd_name[5]= char_name[5];	
				 
			   //(4)�����ֽڶ�Ӧ��һ��ת���ַ�(�ļ����ĵ�6λ)
			   char_name[6]=standard_char[i];
			   mp3_name[6]=	char_name[6];
			   led_name[6]=	char_name[6];
				 lcd_name[6]= char_name[6];	
				 
			   //(4)�ļ���ʣ��ת���ַ�ȫ��ת��Ϊ�ַ�0(�ļ����ĵ�7��8λ)
			   char_name[7]=standard_char[0];
			   mp3_name[7]=	char_name[7];
			   led_name[7]=	char_name[7];
			   char_name[8]=standard_char[0];
			   mp3_name[8]=	char_name[8];
			   led_name[8]=	char_name[8];
				 lcd_name[8]= char_name[8];		
			   //�ַ���ʾ����������
			   display_play();
			 }
			while(*analysis_data_i != 0);
			}
			
			//״̬�ֽڴ���
			analysis_data_a=processing_data_buff[9];
		  analysis_data_a=analysis_data_a<<8;
		  analysis_data_a=analysis_data_a|processing_data_buff[10];
			analysis_data_i = &analysis_data_a;
			
			if(*analysis_data_i!=0 )
			{
				do
				{
					 //����������߷���λλ�ã�������
					 i=data_analysis( analysis_data_i );
					 //1~2������״̬����
					 //3~4����ѹ����״̬����
					 if( i== 3 )
					 {
						 GPIO_SetBits(GPIOF,GPIO_Pin_6);  //���ø�ѹ�����̵���
					 }
					 if( i==4 )
					 {
							GPIO_ResetBits(GPIOF,GPIO_Pin_6);
						}
						//5��Ԥ��
						//6��Ԥ��
						//7��Ԥ��
						//8��Ԥ��
						//9��������  ��1 ��3 ��4 ��5 ��1 ��3 ��4 ��5
					 if( i==9 |i==10 |i==11 |i==12 |i==13 |i==14 |i==15 )
					 {
							GPIO_SetBits(GPIOF,GPIO_Pin_7);   //���������߼̵���
					 }
					 //ȷ��mp3���ַ��ļ����ļ���
					 //(1)ȷ���ն˱�Ŷ�Ӧ������ת���ַ�(�ļ����ĵ�2��3λ)
					 char_name[2]=standard_char[processing_data_buff[1]>>4];
					 mp3_name[2]=	char_name[2];
					 led_name[2]=	char_name[2];	
					 lcd_name[2]= char_name[2];	
					 
					 char_name[3]=standard_char[processing_data_buff[1] & 0x0f];
					 mp3_name[3]=char_name[3];
					 led_name[3]=	char_name[3];
					 lcd_name[3]= char_name[3];		
					 
					 //(2)�����ֽڶ�Ӧ��һ��ת���ַ�Ϊ�ַ�0(�ļ����ĵ�4λ)
					 char_name[4]=standard_char[0];
					 mp3_name[4]=	char_name[4];
					 led_name[4]=	char_name[4];
					 lcd_name[4]= char_name[4];	
					 
					 //(3)�����ֽڶ�Ӧ��һ��ת���ַ�Ϊ�ַ�0(�ļ����ĵ�5λ)
					 char_name[5]=standard_char[0];
					 mp3_name[5]=	char_name[5];
					 led_name[5]=	char_name[5];
					 lcd_name[5]= char_name[5];		
					 
					 //(4)�����ֽڶ�Ӧ��һ��ת���ַ�(�ļ����ĵ�6λ)
					 char_name[6]=standard_char[0];
					 mp3_name[6]=	char_name[6];
					 led_name[6]=	char_name[6];
					 lcd_name[6]= char_name[6];		
					 
					 //(5)״̬�ֽڶ�Ӧ��һ��ת���ַ�(�ļ����ĵ�7λ)
					 char_name[7]=standard_char[i];
					 mp3_name[7]=	char_name[7];
					 led_name[7]=	char_name[7];
					 lcd_name[7]= char_name[7];		
					 
					 //(6)�ļ���ʣ��ת���ַ�ȫ��ת��Ϊ�ַ�0(�ļ����ĵ�8λ)
					 char_name[8]=standard_char[0];
					 mp3_name[8]=	char_name[8];
					 led_name[8]=	char_name[8];
					 lcd_name[8]= char_name[8];		
					 //�ַ���ʾ����������
					 display_play();
				}
				while(*analysis_data_i != 0);				
			}
		}
			
			//���ϱ��ֽڴ���
			if((processing_data_buff[16] & 0xF0) == 0x10)
			{
				analysis_data_a=processing_data_buff[17];
				analysis_data_a=analysis_data_a<<8;
				analysis_data_a=analysis_data_a|processing_data_buff[18];
				analysis_data_i = &analysis_data_a;
				
					do
					{
						 //����������߷���λλ�ã�������
						 i=data_analysis(analysis_data_i);
						//1��Ԥ��
						//2��Ԥ��
						//3����λ����ӽ�
						//4����λ�󷽽ӽ�
						//5���Ҽ�ǽ����1
						//6���Ҽ�ǽ����2
						//7��ʵս-ѵ���л���ʵս��
						//8��ʵս-ѵ���л���ѵ����
						//9��Ԥ��
						//10��Ԥ��
						//11��Ԥ��
						//12��Ԥ��
						//13��Ԥ��
						//14��Ԥ��
						//15��Ԥ��
						//16��Ԥ��
						
						 //ȷ��mp3���ַ��ļ����ļ���
						 //(1)ȷ���ն˱�Ŷ�Ӧ������ת���ַ�(�ļ����ĵ�2��3λ)
						 char_name[2]=standard_char[processing_data_buff[1]>>4];
						 mp3_name[2]=	char_name[2];
						 led_name[2]=	char_name[2];	
						 lcd_name[2]= char_name[2];
						 
						 char_name[3]=standard_char[processing_data_buff[1] & 0x0f];
						 mp3_name[3]=char_name[3];
						 led_name[3]=	char_name[3];
						 lcd_name[3]= char_name[3];	
						 
						 //(2)�����ֽڶ�Ӧ��һ��ת���ַ�Ϊ�ַ�0(�ļ����ĵ�4λ)
						 char_name[4]=standard_char[0];
						 mp3_name[4]=	char_name[4];
						 led_name[4]=	char_name[4];
						 lcd_name[4]= char_name[4];	
						 
						 //(3)�����ֽڶ�Ӧ��һ��ת���ַ�Ϊ�ַ�0(�ļ����ĵ�5λ)
						 char_name[5]=standard_char[0];
						 mp3_name[5]=	char_name[5];
						 led_name[5]=	char_name[5];
						 lcd_name[5]= char_name[5];	
						 
						 //(4)�����ֽڶ�Ӧ��һ��ת���ַ�(�ļ����ĵ�6λ)
						 char_name[6]=standard_char[0];
						 mp3_name[6]=	char_name[6];
						 led_name[6]=	char_name[6];
						 lcd_name[6]= char_name[6];		
						 
						 //(5)״̬�ֽڶ�Ӧ��һ��ת���ַ�(�ļ����ĵ�7λ)
						 char_name[7]=standard_char[0];
						 mp3_name[7]=	char_name[7];
						 led_name[7]=	char_name[7];
						 lcd_name[7]= char_name[7];	
						 
						 //(4)���ϱ��ֽڶ�Ӧ��һ��ת���ַ�(�ļ����ĵ�8λ)
						 char_name[8]=standard_char[i];
						 mp3_name[8]=	char_name[8];
						 led_name[8]=	char_name[8];
						 lcd_name[8]= char_name[8];		
						 //�ַ���ʾ����������
						 
						 display_play();
					}
					while(*analysis_data_i != 0);
			}	
}


