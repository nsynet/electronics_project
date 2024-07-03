


/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "user_uart.h"
#include "my_PWM_1k.h"
#include "stdio.h"
#include "ENC28J60.H"

//#include "STM32_Init.h"       // STM32 Initialization
#include "spi.h"
#include "simple_server.h"
//#include "SPI_45db161.h"

//#define LED             ( 1 << 5 )              // PB5: LED D2

//#define BP2             0x2000                     // PC13: BP2
//#define BP3             0x0001                     // PA0 : BP3

//#define UP              0x0800                     // PB11: UP
//#define RIGHT           0x1000                     // PB12: RIGHT
//#define LEFT            0x2000                     // PB13: LEFT
//#define DOWN            0x4000                     // PB14: DOWN
//#define OK              0x8000                     // PB15: OK

//#define JOYSTICK        0xF800                     // JOYSTICK ALL KEYS
/*----------------------------------------------------------*\
 | SOFTWARE DATA                                            |
\*----------------------------------------------------------*/
/*----------------------------------------------------------*\
 |  Delay                                                   |
 |  延时 Inserts a delay time.                              |
 |  nCount: 延时时间                                        |
 |  nCount: specifies the delay time length.                |
\*----------------------------------------------------------*/
void Delay(vu32 nCount) {
  for(; nCount != 0; nCount--);
  }
/*----------------------------------------------------------*\
 | SendChar                                                 |
 | Write character to Serial Port.                          |
\*----------------------------------------------------------*/
int SendChar (int ch)  {

  while (!(USART1->SR & USART_FLAG_TXE));
  USART1->DR = (ch & 0x1FF);

  return (ch);
}
/*----------------------------------------------------------*\
 | GetKey                                                   |
 | Read character to Serial Port.                           |
\*----------------------------------------------------------*/
/*int GetKey (void)  {

  while (!(USART1->SR & USART_FLAG_RXNE));

  return ((int)(USART1->DR & 0x1FF));
}
*/
const unsigned char enc28j60_MAC[6] = {0x11, 0x02, 0x03, 0x04, 0x05, 0x66};

/******************************************************************************/
#define BufferSize 32
u8 SPI1_Buffer_Tx[BufferSize] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09,
                                 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12,
                                 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B,
                                 0x1C, 0x1D, 0x1E, 0x1F, 0x20};

u8 SPI2_Buffer_Tx[BufferSize] = {0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59,
                                 0x5A, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F, 0x60, 0x61, 0x62,
                                 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B,
                                 0x6C, 0x6D, 0x6E, 0x6F, 0x70};


#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

/*******************************************************************************
* Function Name  : main
* Description    : Main program.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
int main(void)
{
#ifdef DEBUG
  debug();
#endif
  int rev = 0;
  //配置系统时钟,使用内部时钟
  RCC_HSI_Configuration();
  //配置  NVIC 和 Vector Table 
  NVIC_Configuration();
  UART1_NVIC_Configuration();
  UART1_GPIO_Configuration();
  UART1_Configuration(); 
  /*************************************************************************/
  USART_SendData(USART1, 0x39);
  USART_SendData(USART1, ',');
 while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
 {
  }
 USART_SendData(USART1, '\n');
  while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
 {
 }
  
  Uart1_PutString("this OK !__This first usar\n\n" , strlen("this OK !__This first usar\n\n"));
    printf("\n The printf is OK?");  
  //主循环 
  printf("\n The number is %f",112.123);

       

 // stm32_Init ();                                // STM32 setup
//  GPIOD->ODR &= ~(1<<9);//GPIOA->BRR = ENC28J60_CS;
//  GPIOD->ODR |= 1<<9;//GPIOA->BSRR = ENC28J60_CS;
  printf ("SPI1_Init starting...\r\n");
  SPI1_Init();
  printf ("enc28j60 init...\r\n");
   GPIO_ResetBits(GPIOC,  GPIO_Pin_5);
   GPIO_SetBits(GPIOC,  GPIO_Pin_5);
  //enc28j60Init((unsigned char *)enc28j60_MAC);  


    simple_server();

    enc28j60Init((unsigned char *)enc28j60_MAC);

    rev = enc28j60getrev();

    return rev;  
 /*  AT45DB161_init(); 
   GPIO_ResetBits(GPIOB,  GPIO_Pin_10);
   GPIO_ResetBits(GPIOB,  GPIO_Pin_12);
   GPIO_SetBits(GPIOB,  GPIO_Pin_10);
   GPIO_SetBits(GPIOB,  GPIO_Pin_12);
    
 
   
   DF_Read_Str(0,(unsigned int)sizeof(SPI1_Buffer_Tx),SPI1_Buffer_Tx);
   
   DF_Write_Str(0,(unsigned int)sizeof(SPI2_Buffer_Tx),SPI2_Buffer_Tx);
   
   DF_Read_Str(0,(unsigned int)sizeof(SPI1_Buffer_Tx),SPI1_Buffer_Tx);*/
   

   
  /*
   Buffer_Write_Byte (0, 0x01);
   Buffer_To_Page(0);
   
   Page_To_Buffer (0);
   Buffer_Read_Byte(0);
   
   
   USART_SendData(USART1,Buffer_Read_Byte(0));
   while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
   {
   }*/
   
/*
   for(i=0;i<30;i++)
  {
    printf("\n%d",my_PWM[i]);
  }
   */
  // while(1)
   //{
  // }
  }
/***************************************************************************/
//
//
//
/***************************************************************************/
void RCC_HSI_Configuration(void)//配置系统时钟,使用内部时钟
{
    //将外设 RCC寄存器重设为缺省值
    RCC_DeInit();
//    RCC_HSEConfig(RCC_HSE_ON);
    //预取指缓存使能
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);

     //设置代码延时值
    //FLASH_Latency_2  2 延时周期
    FLASH_SetLatency(FLASH_Latency_2);

    //设置 AHB 时钟（HCLK）
    //RCC_SYSCLK_Div1  AHB 时钟 =  系统时钟 
    RCC_HCLKConfig(RCC_SYSCLK_Div1);

     //设置高速 AHB 时钟（PCLK2）
    //RCC_HCLK_Div2  APB1 时钟  = HCLK / 2 
    RCC_PCLK2Config(RCC_HCLK_Div2);//这改为Div1与原来的Div2怎没什么变化

    //设置低速 AHB 时钟（PCLK1）
    //RCC_HCLK_Div2  APB1 时钟  = HCLK / 2 
    RCC_PCLK1Config(RCC_HCLK_Div2);   
    RCC_PLLConfig(RCC_PLLSource_HSI_Div2,RCC_PLLMul_16);//PLLCLK =8MHz/2*16=64MHz
    
    //使能或者失能 PLL
    RCC_PLLCmd(ENABLE);

    //等待指定的 RCC 标志位设置成功 等待PLL初始化成功
    while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
    {
   }
    //设置系统时钟（SYSCLK） 设置PLL为系统时钟源
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
    //RCC_SYSCLKConfig(RCC_SYSCLKSource_HSI);
    //等待PLL成功用作于系统时钟的时钟源
    //  0x00：HSI 作为系统时钟 
    //  0x04：HSE作为系统时钟 
    //  0x08：PLL作为系统时钟  
    while(RCC_GetSYSCLKSource() != 0x08) 
    {
    }


  //使能或者失能 APB2 外设时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
  
}
/*******************************************************************************
* Function Name  : NVIC_Configuration
* Description    : Configures NVIC and Vector Table base location.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void NVIC_Configuration(void)
{
#ifdef  VECT_TAB_RAM
  /* Set the Vector Table base location at 0x20000000 */
  NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0);
#else  /* VECT_TAB_FLASH  */
  /* Set the Vector Table base location at 0x08000000 */
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
#endif
}

/*******************************************************************************
* Function Name  : PUTCHAR_PROTOTYPE
* Description    : Retargets the C library printf function to the USART.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
PUTCHAR_PROTOTYPE
{
/* Write a character to the USART */
  USART_SendData(USART1, (u8) ch);

  /* Loop until the end of transmission */
  while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)  {
  }

  return ch;
}


#ifdef  DEBUG
/*******************************************************************************
* Function Name  : assert_failed
* Description    : Reports the name of the source file and the source line number
*                  where the assert_param error has occurred.
* Input          : - file:pointer to the source file name
*                  - line: assert_param error line source number
* Output         : None
* Return         : None
*******************************************************************************/
void assert_failed(u8* file, u32 line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
