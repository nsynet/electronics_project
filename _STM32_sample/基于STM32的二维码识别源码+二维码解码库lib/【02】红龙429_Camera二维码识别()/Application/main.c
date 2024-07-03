/**
  * @file    main.c 
  * @author  WB R&D Team - openmcu666
  * @version V0.1
  * @date    2016.08.24
  * @brief   Main Program body
  */

#include "stm32f4xx.h"
#include "Gpio.h"
#include "usart.h"
#include "delay.h"
#include "stm32f429i_lcd.h"
#include "ov7670.h"
#include "ov2640.h"
#include "ov5642.h"
#include "mt9d111.h"
#include "CamConfig.h"
#include "sccb_bus.h"
#include "RTC.h"
#include "string.h"
#include "QR_Decoder.h"

//only for OV5642

/*****************************************************************************
**   Main Function  main()
******************************************************************************/
int main(void)
{
	uint16_t temper;
	char  qr_type_len=0;
  short qr_data_len=0;
  char qr_type_buf[10];
  char qr_data_buf[512];
  int addr=0;
  int i=0,j=0;
  char qr_num=0;
	
	
	USART1_Init();
	//LED_Init();
  Key_Init();
	BEEP_Init();
	printf("\r\n");
	printf("Hello Welcome.\r\n");
	printf("Thank You For Choosing Our Products.\r\n");
	printf("Thank You Very Much.\r\n");
	printf("This Example Camera.\r\n");
	printf("\r\n");
	printf("DCMI Initialization...\r\n");
	LCD_Init(); 
	LCD_LayerInit();
  LTDC_Cmd(ENABLE);
  LCD_SetLayer(LCD_BACKGROUND_LAYER);  
	LCD_SetTransparency(0xFF);
	LCD_Clear(BLACK);
	LCD_SetLayer(LCD_FOREGROUND_LAYER); //使用前景层
  LCD_SetTransparency(0xFF);          //设置透明度
  LCD_Clear_ARGB8888(0x40000000); 
   
  LCD_SetTextColor_ARGB8888(0xFFFFFFFF);
  LCD_DrawFullRect_ARGB8888(Frame_Width,Frame_Width);                              //绘制透明框
  
  LCD_DrawBox_ARGB8888(Frame_Width,Frame_Line_Length,Frame_Line_Size,0xD05CBDE7);  //绘制扫描框
	
	printf("Ok,Let's Go...\r\n");
	
	temper = ov7670_check();         //Camera是否为OV7670
	 
	if (temper == 0x7673)
	{
		printf("Cam ID : [0x%02x]\r\n",temper);
		if (ov7670_init())
		{
			printf("Init OV7670 reg Fail...\r\n");
		  while (1)
		  {
				
		  }
		}
		else
		{
			printf("Init OV7670 reg OK...\r\n");
			printf("Show Camer Data...\r\n");

		}		
		
	}
	else                                        
	{
    temper=ov2640_check();
	  
		if(temper==0x2642)                        //OV2640
		{
			if (ov2640_init())
		  {
			printf("Init OV2640 reg Fail...\r\n");
			while (1)
			{}
		  }
		  else
		 {
			printf("Cam ID : [0x%02x]\r\n",temper);
			printf("Init OV2640 reg OK...\r\n");
			printf("Show Camer Data...\r\n");

		 }
        
	 }
		else                                        //ov5642 or mt9d111
    {
			temper=ov5642_check();
			printf("Cam ID : [0x%02x]\r\n",temper);
		  if (temper==0x5642)//mt9d111_init
		  {
			  ov5642_init();
			  printf("Init ov5642 reg OK...\r\n");
			  printf("Show Camer Data...\r\n");	
		  }
		 else
		 {
			  printf("Init ov5642 reg Fail...\r\n");				
				while (1){}
		 }
    }
	}
//	if(RTC_Config())		 		                          //初始化RTC
//	{
//	  printf("\r\nLSE start failed\r\n");
//	}
//	RTC_Set_WakeUp(RTC_WakeUpClock_CK_SPRE_16bits,0);		//配置WAKE UP中断,1秒钟中断一次,用于统计帧率
  systick_delay_ms(100);
	ov5642_auto_focus();
	cam_start();
	
	while (1)
  {
    qr_num = QR_decoder();
		 if(qr_num)
      {
         GPIO_SetBits(BEEP_GPIO_PORT,BEEP_GPIO_PIN);
				 systick_delay_ms(100);
				 GPIO_ResetBits(BEEP_GPIO_PORT,BEEP_GPIO_PIN);
         //解码的数据是按照识别条码的个数封装好的二维数组,这些数据需要
         //根据识别条码的个数，按组解包并通过串口发送到上位机串口终端
         for(i=0;i < qr_num;i++)
         {
           qr_type_len = decoded_buf[i][addr++];//获取解码类型长度
               
            for(j=0;j < qr_type_len;j++)
                 qr_type_buf[j]=decoded_buf[i][addr++];//解码类型
               
             qr_data_len  = decoded_buf[i][addr++]<<8; //解码数据的高8位
             qr_data_len |= decoded_buf[i][addr++];    //解码数据的低8位
               
             for(j=0;j < qr_data_len;j++)
                 qr_data_buf[j]=decoded_buf[i][addr++];//获取解码数据
               
             USART1_Send_Data((unsigned char *)qr_type_buf, qr_type_len);//串口发送解码类型
             USART1_Send_Data((unsigned char *)":", 1);           
             USART1_Send_Data((unsigned char *)qr_data_buf, qr_data_len);//串口发送解码数据
             USART1_Send_Data((unsigned char *)"\r\n", 2); 
             addr =0;
          }
               
       } 
  }
}
