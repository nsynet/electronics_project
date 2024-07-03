/******************** (C) COPYRIGHT 2012 WildFire Team **************************
 * 文件名  ：main.c
 * 描述    ：通过电脑的超级终端向单片机发送数据，单片机将接收到的数据发回电脑，
 *         ：并显示在超级终端上。用的是串口1的中断方式。         
 * 实验平台：野火STM32开发板
 * 库版本  ：ST3.5.0
 *
 * 作者    ：wildfire team 
 * 论坛    ：http://www.amobbs.com/forum-1008-1.html
 * 淘宝    ：http://firestm32.taobao.com
*********************************************************************************/
#include "stm32f10x.h"
#include "usart1.h"
#include "gpio_key.h"
#include "mp3play.h "
#include "Touch.h"
#include "lcd.h" 

#define SRC_ADDR   2  //默认的源终端编号字节为3，即表示1号哨位分机
#define DATA_BUFF_LEN   20

//unsigned int analysis_data;
unsigned char mp3_name[]={"Y0000000.mp3"};//默认MP3文件
unsigned char char_name[]={"C0000000.txt"};//默认字符显示文件
unsigned char led_name[]={"L0280000.txt"};//默认字符显示文件
unsigned char standard_char[]={"0123456789ABCDEF"};
unsigned char lcd_name[]={"D0000000.txt"};
BYTE buffer[512];          /* 存放读取出的文件数据 */


unsigned char processing_data_buff[DATA_BUFF_LEN];
unsigned char SX_flag=1;//实战/训练标志位，SX_flag=1为实战状态，SX_flag=0为训练状态
unsigned char SQGD_flag=0;//申请供弹标志位，SQGD_flag=1表示申请供弹，SQGD_flag=0表示没有申请供弹	  
unsigned char YJKD_flag=0;//一键开弹标志位，YJKD_flag=1表示需要开弹，YJKD_flag=0表示不用开弹

uint8_t USART1_RECEIVE_NUM = 0;
uint8_t receive_data_buff[20];
uint8_t USART1_RECEIVE_FLAG=0;
uint8_t SD_FLAG = 1;

uint8_t UART0_RECEIVE_DATA(void);
unsigned char data_analysis( unsigned int *analysis_data );
void data_processing(void);
void LCD_Display(void);
/*
 * 函数名：main
 * 描述  ：主函数
 * 输入  ：无
 * 输出  ：无
 */
int main(void)
{	
	SysTick_Init();	
	/* USART1 config 1200 8-N-1 */
	USART1_Config();
	GPIO_INIT();
  NVIC_ConfigurationUART();
	NVIC_ConfigurationSD();
	VS1003_SPI_Init();		/* MP3硬件I/O初始化 */ 
	
	MP3_Start(); 			/* MP3就绪，准备播放，在vs1003.c实现 */ 
		LCD_Init();							/* LCD 初始化*/	
	Touch_init();						/*触摸初始化*/
	
	while(Touchl_Calibrate() !=0);	 	/*等待触摸屏校准完毕*/	
	Set_direction(0);	 //设置为横屏
	LCD_SHOW_INIT();    //显示初始界面
	
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
 * 函数名：main
 * 描述  ：串口接收
 * 输入  ：无
 * 输出  ：接收状态
 */
uint8_t UART0_RECEIVE_DATA(void)
{
	
	uint8_t uart1_RECEIVE_i,uart1_RECEIVE_j;
  do
	{

	}
	while(USART1_RECEIVE_NUM < 1);//接收第一个地址字节
//	SD_FLAG = 0;
  for(uart1_RECEIVE_i=0;(uart1_RECEIVE_i<500) && (USART1_RECEIVE_NUM < 20);uart1_RECEIVE_i++)//在500mS内(48MHz晶体下)，若收不到或剩下的5个字节数据，则清空接收数组，同时返回接收状态
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
	UINT br;					  /*读取出的字节数，用于判断是否到达文件尾*/
	FIL fsrc;          		 // file objects
	f_mount(0, &fs);
	res = f_open( &fsrc, lcd_name, FA_READ );
	while(res);
	br = 1;				  			     /* br 为全局变量 */
	
	while(1) 
	{			 
  	res = f_read( &fsrc, lcd_name, sizeof(lcd_name), &br ); /* 将文件里面的内容读到缓冲区 */						
  	if (res || br == 0) break;                          /* 错误或者到了文件尾 */

  }	
	f_close(&fsrc);	 /* 关闭打开的文件 */	
	LCD_Rectangle(0,0,320,120,WHITE);	/*显示区清白屏*/
	LCD_Str_ENCH_O(88,50,lcd_name,0);
}
//-----------------------------------------------------------------------------—-------------------------------
// 数据分析函数
//--------------------------------------------------------------------------------------------------------------------
//说明：对全局变量data_analysis分析其第一非零元素位置，例如第一个非零位为最高位为1，则输出1，第一个非零位为最低位为1，
//则输出8，若各位全为0，则输出0，同时，清除data_analysis的第一个非零元素
unsigned char data_analysis( unsigned int * analysis_data )
{
    unsigned char i;
	//分析数据最高非零位位置
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


//-----------------------------------------------------------------------------—-------------------------------
// 数据分析与处理函数
//-------------------------------------------------------------------------------------------------------------
//说明：将收到的20个UART0数据，由receive_data_buff数组存放于processing_data_buff数组中,
//然后逐一分析数据内容，并执行对应继电器的开断，完成后将processing_data_buff中处理后的对应位清零，
//接着继续分析处理processing_data_buff中后面的数据
void data_processing(void)
{
	unsigned char i,j;
	unsigned int analysis_data_a;
	unsigned int analysis_data_b;
	unsigned int *analysis_data_i;
	unsigned int *analysis_data_j;
	unsigned char a;
	//数组存储转换
	for(a=0; a<DATA_BUFF_LEN ; a++)
	{
		processing_data_buff[a]= receive_data_buff[a];
	}
	if((processing_data_buff[17] & 0x03)==0x02)//实战/训练状态标志
	    SX_flag=1;	//处于实战状态
	if((processing_data_buff[17] & 0x03)==0x01)//实战/训练状态标志
	    SX_flag=0;	//处于训练状态
	
	//上报以太网信息字节处理
	if(processing_data_buff[16] & 0x0f)
	{
	    //一、命令字节处理
		//命令字节组合得到16位分析数据
		analysis_data_a = processing_data_buff[3];
		analysis_data_a = analysis_data_a<<8;
		analysis_data_a = analysis_data_a | processing_data_buff[4];
		analysis_data_i = &analysis_data_a;
		//若是报警命令，则需要确定报警字节对应的一个转换字符(文件名的第5位)
		//报警字节组合得到16位分析数据
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
				  GPIO_SetBits(GPIOF,GPIO_Pin_5);   //警号打开
				  GPIO_SetBits(GPIOF,GPIO_Pin_1);   //报警打开
				 }
				 if(i==8)
				 {
					YJKD_flag=0;
				 }

				 
				 //确定mp3和字符文件的文件名
				 //(1)确定终端编号对应的两个转换字符(文件名的第2、3位)
				 char_name[1]= standard_char[processing_data_buff[1]>>4];
				 mp3_name[1]=	char_name[1];
				 led_name[1]=	char_name[1];
				 lcd_name[1]= char_name[1];
				 
				 char_name[2]= standard_char[processing_data_buff[1] & 0x0f];
				 mp3_name[2]= char_name[2];
				 led_name[2]=	char_name[2];	
				 lcd_name[2]= char_name[2];		
				 
				 //(2)确定命令字节对应的一个转换字符(文件名的第4位)
				 char_name[3]=standard_char[i];
				 mp3_name[3]=	char_name[3];
				 led_name[3]=	char_name[3];
				 lcd_name[3]= char_name[3];		

				 do
				 {
					 if( i== 1 )
					 {
						 //分析数据最高非零位位置，并清零
						 j=data_analysis( analysis_data_j );
						 //确定报警字节对应的一个转换字符(文件名的第5位)
						 char_name[4]=standard_char[j];
						 mp3_name[4]=	char_name[4];
						 led_name[4]=	char_name[4];	
						 lcd_name[4]= char_name[4];		
						 
						 //分析报警位
							switch(j)
							{
								case 1 :
								{ 
									GPIO_SetBits(GPIOG,GPIO_Pin_0);//暴狱指示灯点亮
									if(SX_flag)//处于实战状态，向网关报警
									{
											GPIO_SetBits(GPIOG,GPIO_Pin_1);//暴狱上报网关
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
									GPIO_SetBits(GPIOG,GPIO_Pin_2);//脱逃指示灯点亮
									if(SX_flag)//处于实战状态，向网关报警
									{
											GPIO_SetBits(GPIOG,GPIO_Pin_3);//脱逃上报网关
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
									GPIO_SetBits(GPIOG,GPIO_Pin_4);//挟持指示灯点亮
									if(SX_flag)//处于实战状态，向网关报警
									{
											GPIO_SetBits(GPIOG,GPIO_Pin_5);//挟持上报网关
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
									GPIO_SetBits(GPIOG,GPIO_Pin_6);//袭击指示灯点亮
									if(SX_flag)//处于实战状态，向网关报警
									{
											GPIO_SetBits(GPIOG,GPIO_Pin_7);//袭击上报网关
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
									GPIO_SetBits(GPIOG,GPIO_Pin_8);//灾害指示灯点亮
									if(SX_flag)//处于实战状态，向网关报警
									{
											GPIO_SetBits(GPIOG,GPIO_Pin_9);//灾害上报网关
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
									GPIO_SetBits(GPIOG,GPIO_Pin_10);//火灾指示灯点亮
									if(SX_flag)//处于实战状态，向网关报警
									{
											GPIO_SetBits(GPIOG,GPIO_Pin_11);//预留上报网关
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
									GPIO_SetBits(GPIOG,GPIO_Pin_12);//滋事指示灯点亮
									if(SX_flag)//处于实战状态，向网关报警
									{
											GPIO_SetBits(GPIOG,GPIO_Pin_13);//预留上报网关
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
					 else   //i!=1 无报警命令
					 {
						 char_name[4]=standard_char[0];
						 mp3_name[4]=	char_name[4];
						 led_name[4]=	char_name[4];
						 lcd_name[4]= char_name[4];		
					 }
						//(4)文件名剩下转换字符全部转换为字符0(文件名的第6、7、8位)			 
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
			 
			 if(i == 2)  //一键开弹命令
			 {
				 if(YJKD_flag==1)
				 {
					 GPIO_SetBits(GPIOG,GPIO_Pin_14);//打开子弹箱
					 GPIO_SetBits(GPIOG,GPIO_Pin_15);//打开枪弹柜
					 Delay_us(5000000);
					 GPIO_ResetBits(GPIOG,GPIO_Pin_14);//关闭子弹箱
					 GPIO_ResetBits(GPIOG,GPIO_Pin_15);//关闭弹箱柜
					 SQGD_flag=0;
					 YJKD_flag=0;
				 }
			 }
			 if( i == 3 ) //摄像头切换命令
			 {
					GPIOF->ODR ^= (1<<0);      //取反
			 }
			 if( i ==  4 ) //集合命令1（全装）
			 {
					GPIO_SetBits(GPIOF,GPIO_Pin_1);//报警打开
	//				 TMR2CN=0x04;//开启定时器02
					GPIO_SetBits(GPIOG,GPIO_Pin_15);		//打开枪弹柜		 
					Delay_us(5000000);					 
					GPIO_ResetBits(GPIOG,GPIO_Pin_15);	  //关闭枪弹柜
			 }
			 if(i == 5 ) //集合命令2（轻装）
			 {
				 GPIO_SetBits(GPIOF,GPIO_Pin_1);//报警打开
//				 TMR2CN=0x04;//开启定时器02
			 }
			 if(i == 6 ) //集合命令3（徒手）
			 {
					GPIO_SetBits(GPIOF,GPIO_Pin_1);//报警打开
//				  TMR2CN=0x04;//开启定时器02
			 }
			 //提醒巡逻命令（集中全部精力对目标负责）
			 if(i == 8 )  //全复位命令
			 {
				 GPIO_Write(GPIOG,0x0000); 
				 SQGD_flag=0;
				 YJKD_flag=0;
			 }
						//9、对应子弹箱打开命令处理
				if(i == 9 )
				{
					 if(SQGD_flag==1)
					 {
							GPIO_SetBits(GPIOG,GPIO_Pin_14);//打开子弹箱
							Delay_us(5000000);
							GPIO_ResetBits(GPIOG,GPIO_Pin_14); //关闭子弹箱
						SQGD_flag=0;
					 }	     
				}
				//10~11、打开哨位广播命令处理
				if(i == 10 )
				{
					GPIO_SetBits(GPIOF,GPIO_Pin_2);//打开哨位广播			 		     
				}
				if(i == 11 )
				{
					GPIO_ResetBits(GPIOF,GPIO_Pin_2);//关闭稍微广播
				}
				//12、打开监门
				if(i == 12)
				{
					 GPIO_SetBits(GPIOF,GPIO_Pin_3);//打开监门	JDQ27
					 Delay_us(5000000);
					 GPIO_ResetBits(GPIOF,GPIO_Pin_3);			 		     
				}
				//13、预留
				//14、预留
				//15、预留
				//16、预留
			}
			while( *analysis_data_i != 0 );
		}

			//二、请求字节处理
			//组合得到16位分析数据
			analysis_data_a=processing_data_buff[7];
	    analysis_data_a=analysis_data_a<<8;
	    analysis_data_a=analysis_data_a | processing_data_buff[8];
			analysis_data_i = &analysis_data_a;
		
			if(*analysis_data_i!=0)
			{
				do
			 {
			   //分析数据最高非零位位置，并清零
			   i=data_analysis( analysis_data_i );
			   if(i==1) //1、请求供弹
			   {
					//设置对应继电器
						if(receive_data_buff[11]==0xff)
						{
							 SQGD_flag=1;
						}
						YJKD_flag=1;
						GPIO_SetBits(GPIOF,GPIO_Pin_4);   //申请供弹
			   }
					//2、管控区监控请求处理				
				 if( i==2 )
				 {
						//设置对应继电器
				 }
				 	//3、预留
					//4、预留
					//5、预留
					//6、预留
					//7、预留
					//8、预留
					//9、预留
					//10、预留
					//11、预留
					//12、预留
					//13、预留
					//14、预留
					//15、预留
					//16、预留
			   //确定mp3和字符文件的文件名
			   //(1)确定终端编号对应的两个转换字符(文件名的第2、3位)
			   char_name[2]=standard_char[processing_data_buff[1]>>4];
			   mp3_name[2]=	char_name[2];
			   led_name[2]=	char_name[2];	
				 lcd_name[2]= char_name[2];		
				 
			   char_name[3]=standard_char[processing_data_buff[1] & 0x0f];
			   mp3_name[3]=char_name[3];
			   led_name[3]=	char_name[3];
				 lcd_name[3]= char_name[3];	
				 
			   //(2)命令字节对应的一个转换字符为字符0(文件名的第4位)
			   char_name[4]=standard_char[0];
			   mp3_name[4]=	char_name[4];
			   led_name[4]=	char_name[4];
				 lcd_name[4]= char_name[4];	
				 
			   //(3)报警字节对应的一个转换字符为字符0(文件名的第5位)
			   char_name[5]=standard_char[0];
			   mp3_name[5]=	char_name[5];
			   led_name[5]=	char_name[5];
				 lcd_name[5]= char_name[5];	
				 
			   //(4)请求字节对应的一个转换字符(文件名的第6位)
			   char_name[6]=standard_char[i];
			   mp3_name[6]=	char_name[6];
			   led_name[6]=	char_name[6];
				 lcd_name[6]= char_name[6];	
				 
			   //(4)文件名剩下转换字符全部转换为字符0(文件名的第7、8位)
			   char_name[7]=standard_char[0];
			   mp3_name[7]=	char_name[7];
			   led_name[7]=	char_name[7];
			   char_name[8]=standard_char[0];
			   mp3_name[8]=	char_name[8];
			   led_name[8]=	char_name[8];
				 lcd_name[8]= char_name[8];		
			   //字符显示与语音播放
			   display_play();
			 }
			while(*analysis_data_i != 0);
			}
			
			//状态字节处理
			analysis_data_a=processing_data_buff[9];
		  analysis_data_a=analysis_data_a<<8;
		  analysis_data_a=analysis_data_a|processing_data_buff[10];
			analysis_data_i = &analysis_data_a;
			
			if(*analysis_data_i!=0 )
			{
				do
				{
					 //分析数据最高非零位位置，并清零
					 i=data_analysis( analysis_data_i );
					 //1~2、弹箱状态处理
					 //3~4、高压电网状态处理
					 if( i== 3 )
					 {
						 GPIO_SetBits(GPIOF,GPIO_Pin_6);  //设置高压电网继电器
					 }
					 if( i==4 )
					 {
							GPIO_ResetBits(GPIOF,GPIO_Pin_6);
						}
						//5、预留
						//6、预留
						//7、预留
						//8、预留
						//9、警戒线  左1 左3 左4 左5 右1 右3 右4 右5
					 if( i==9 |i==10 |i==11 |i==12 |i==13 |i==14 |i==15 )
					 {
							GPIO_SetBits(GPIOF,GPIO_Pin_7);   //开启警戒线继电器
					 }
					 //确定mp3和字符文件的文件名
					 //(1)确定终端编号对应的两个转换字符(文件名的第2、3位)
					 char_name[2]=standard_char[processing_data_buff[1]>>4];
					 mp3_name[2]=	char_name[2];
					 led_name[2]=	char_name[2];	
					 lcd_name[2]= char_name[2];	
					 
					 char_name[3]=standard_char[processing_data_buff[1] & 0x0f];
					 mp3_name[3]=char_name[3];
					 led_name[3]=	char_name[3];
					 lcd_name[3]= char_name[3];		
					 
					 //(2)命令字节对应的一个转换字符为字符0(文件名的第4位)
					 char_name[4]=standard_char[0];
					 mp3_name[4]=	char_name[4];
					 led_name[4]=	char_name[4];
					 lcd_name[4]= char_name[4];	
					 
					 //(3)报警字节对应的一个转换字符为字符0(文件名的第5位)
					 char_name[5]=standard_char[0];
					 mp3_name[5]=	char_name[5];
					 led_name[5]=	char_name[5];
					 lcd_name[5]= char_name[5];		
					 
					 //(4)请求字节对应的一个转换字符(文件名的第6位)
					 char_name[6]=standard_char[0];
					 mp3_name[6]=	char_name[6];
					 led_name[6]=	char_name[6];
					 lcd_name[6]= char_name[6];		
					 
					 //(5)状态字节对应的一个转换字符(文件名的第7位)
					 char_name[7]=standard_char[i];
					 mp3_name[7]=	char_name[7];
					 led_name[7]=	char_name[7];
					 lcd_name[7]= char_name[7];		
					 
					 //(6)文件名剩下转换字符全部转换为字符0(文件名的第8位)
					 char_name[8]=standard_char[0];
					 mp3_name[8]=	char_name[8];
					 led_name[8]=	char_name[8];
					 lcd_name[8]= char_name[8];		
					 //字符显示与语音播放
					 display_play();
				}
				while(*analysis_data_i != 0);				
			}
		}
			
			//非上报字节处理
			if((processing_data_buff[16] & 0xF0) == 0x10)
			{
				analysis_data_a=processing_data_buff[17];
				analysis_data_a=analysis_data_a<<8;
				analysis_data_a=analysis_data_a|processing_data_buff[18];
				analysis_data_i = &analysis_data_a;
				
					do
					{
						 //分析数据最高非零位位置，并清零
						 i=data_analysis(analysis_data_i);
						//1、预留
						//2、预留
						//3、哨位侧翼接近
						//4、哨位后方接近
						//5、右监墙对射1
						//6、右监墙对射2
						//7、实战-训练切换（实战）
						//8、实战-训练切换（训练）
						//9、预留
						//10、预留
						//11、预留
						//12、预留
						//13、预留
						//14、预留
						//15、预留
						//16、预留
						
						 //确定mp3和字符文件的文件名
						 //(1)确定终端编号对应的两个转换字符(文件名的第2、3位)
						 char_name[2]=standard_char[processing_data_buff[1]>>4];
						 mp3_name[2]=	char_name[2];
						 led_name[2]=	char_name[2];	
						 lcd_name[2]= char_name[2];
						 
						 char_name[3]=standard_char[processing_data_buff[1] & 0x0f];
						 mp3_name[3]=char_name[3];
						 led_name[3]=	char_name[3];
						 lcd_name[3]= char_name[3];	
						 
						 //(2)命令字节对应的一个转换字符为字符0(文件名的第4位)
						 char_name[4]=standard_char[0];
						 mp3_name[4]=	char_name[4];
						 led_name[4]=	char_name[4];
						 lcd_name[4]= char_name[4];	
						 
						 //(3)报警字节对应的一个转换字符为字符0(文件名的第5位)
						 char_name[5]=standard_char[0];
						 mp3_name[5]=	char_name[5];
						 led_name[5]=	char_name[5];
						 lcd_name[5]= char_name[5];	
						 
						 //(4)请求字节对应的一个转换字符(文件名的第6位)
						 char_name[6]=standard_char[0];
						 mp3_name[6]=	char_name[6];
						 led_name[6]=	char_name[6];
						 lcd_name[6]= char_name[6];		
						 
						 //(5)状态字节对应的一个转换字符(文件名的第7位)
						 char_name[7]=standard_char[0];
						 mp3_name[7]=	char_name[7];
						 led_name[7]=	char_name[7];
						 lcd_name[7]= char_name[7];	
						 
						 //(4)非上报字节对应的一个转换字符(文件名的第8位)
						 char_name[8]=standard_char[i];
						 mp3_name[8]=	char_name[8];
						 led_name[8]=	char_name[8];
						 lcd_name[8]= char_name[8];		
						 //字符显示与语音播放
						 
						 display_play();
					}
					while(*analysis_data_i != 0);
			}	
}


