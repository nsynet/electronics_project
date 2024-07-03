/*********************************Copyright (c)*********************************                                      
                                           
******************************************************************************
  * 文件名称：main.C
  * 描    述：
  * 创 建 者: 
  * 创建日期: 
  * 修 改 者:             
  * 修改日期: 
  * 版    本: 
  ******************************************************************************
  * attention
  *
  ******************************************************************************
  */
/*Includes --------------------------------------------------------------------*/
  #include "includes.h"
  
/* 类型定义 typedef------------------------------------------------------------*/
/* 预定义字符------------------------------------------------------------------*/
/* 宏定义----------------------------------------------------------------------*/
/* 变量定义--------------------------------------------------------------------*/
   unsigned short int systick_num=0x00; //systick计数器变量
	 unsigned short int key_num=0x00; //按键计数器变量
	 unsigned char Uart_buf[2]={0x00,0x00}; //缓冲接收缓冲器
	 unsigned char flag=0;

void delay_ms(unsigned char a)
{
		unsigned int i,j;
	  
	  while(a--)
		{
			for(i=0;i<100;i++)
			 for(j=0;j<95;j++);
		}	
}

void sleep_mode(void)
{
		RCC_APB1PeriphClockCmd( RCC_APB1Periph_PWR  , ENABLE); 
	  PWR_EnterSleepMode(PWR_SLEEPEntry_WFI);
	  //PWR_EnterSTOPMode( PWR_Regulator_LowPower , PWR_STOPEntry_WFI); 
}
	 
/********************************************************************************
函数名称: main()
功    能: 主函数入口
参    数: 无
返 回 值: 无	  
*********************************************************************************/
int main(void)
{              
	SystemInit(); //系统时钟配置函数，选择外部8M晶振
  Nvic_Init();	
  LED_Init();
	Key_Init();
	sleep_mode();
	//Timer1_Init(488,5);  //16526hz 
	//Timer1_Init(503,5);  //15870hz    拍照
	//Timer1_Init(511,5);  //15617hz
	//Timer1_Init(483,3);  //165260hz     切换摄像头
  while(1)
	{   
			GPIO_SetBits(LED_PORT ,GPIO_Pin_6);
		  delay_ms(250);
		  GPIO_ResetBits(LED_PORT ,GPIO_Pin_6);
		  delay_ms(250);
			if(flag)
			{
					flag = 0;
					PWR_EnterSleepMode(PWR_SLEEPEntry_WFI);
					
			}
			
  }									            	
}


#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif




/**
  * @}
  */ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
