/*********************************Copyright (c)*********************************                                      
                                           
******************************************************************************
  * �ļ����ƣ�main.C
  * ��    ����
  * �� �� ��: 
  * ��������: 
  * �� �� ��:             
  * �޸�����: 
  * ��    ��: 
  ******************************************************************************
  * attention
  *
  ******************************************************************************
  */
/*Includes --------------------------------------------------------------------*/
  #include "includes.h"
  
/* ���Ͷ��� typedef------------------------------------------------------------*/
/* Ԥ�����ַ�------------------------------------------------------------------*/
/* �궨��----------------------------------------------------------------------*/
/* ��������--------------------------------------------------------------------*/
   unsigned short int systick_num=0x00; //systick����������
	 unsigned short int key_num=0x00; //��������������
	 unsigned char Uart_buf[2]={0x00,0x00}; //������ջ�����
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
��������: main()
��    ��: ���������
��    ��: ��
�� �� ֵ: ��	  
*********************************************************************************/
int main(void)
{              
	SystemInit(); //ϵͳʱ�����ú�����ѡ���ⲿ8M����
  Nvic_Init();	
  LED_Init();
	Key_Init();
	sleep_mode();
	//Timer1_Init(488,5);  //16526hz 
	//Timer1_Init(503,5);  //15870hz    ����
	//Timer1_Init(511,5);  //15617hz
	//Timer1_Init(483,3);  //165260hz     �л�����ͷ
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
