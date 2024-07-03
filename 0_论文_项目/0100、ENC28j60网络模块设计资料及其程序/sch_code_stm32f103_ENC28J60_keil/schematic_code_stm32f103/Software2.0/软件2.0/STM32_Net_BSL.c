
#include "rtl.h"                               
#include "STM32_Net_BSL.h"

OS_MUT SPI2_RWMutex;


/*******************************************************************************
* Function Name  : RCC_Configuration
* Description    : Configures the different system clocks.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void RCC_Configuration(void)
{
  ErrorStatus HSEStartUpStatus;
  /* RCC system reset(for debug purpose) */
  RCC_DeInit();

  /* Enable HSE */
  RCC_HSEConfig(RCC_HSE_ON);

  /* Wait till HSE is ready */
  HSEStartUpStatus = RCC_WaitForHSEStartUp();

  if(HSEStartUpStatus == SUCCESS)
  {
    /* Enable Prefetch Buffer */
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);

    /* Flash 2 wait state */
    FLASH_SetLatency(FLASH_Latency_2);
 
    /* HCLK = SYSCLK */
    RCC_HCLKConfig(RCC_SYSCLK_Div1); 
  
    /* PCLK2 = HCLK */
    RCC_PCLK2Config(RCC_HCLK_Div1); 

    /* PCLK1 = HCLK/2 */
    RCC_PCLK1Config(RCC_HCLK_Div2);

    /* PLLCLK = 8MHz * 9 = 72 MHz */
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_6);

    /* Enable PLL */ 
    RCC_PLLCmd(ENABLE);

    /* Wait till PLL is ready */
    while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
    {
    }

    /* Select PLL as system clock source */
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

    /* Wait till PLL is used as system clock source */
    while(RCC_GetSYSCLKSource() != 0x08)
    {
    }
  }   
}

/*******************************************************************************
* Function Name  : NVIC_Configuration
* Description    : Configures the nested vectored interrupt controller.
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

    /* Configure the NVIC Preemption Priority Bits */  
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);    
    
}

void spi_init (void) 
{
    static u8 Initialed = 0;
   /* Initialize and enable the SSP Interface module. */

   /* Enable clock for SPI, GPIOA and AFIO. */
    SPI_InitTypeDef   SPI_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    if(Initialed) return;
    Initialed = 1;

    //Enable Clock
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); 
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
    
    //SPI2
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15 ;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    //SD CS
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_SetBits(GPIOA, GPIO_Pin_8);

    //TP CS
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_SetBits(GPIOB, GPIO_Pin_12);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_Init(SPI2, &SPI_InitStructure);
    
    SPI_Cmd(SPI2, ENABLE);
    os_mut_init (SPI2_RWMutex);
}

void spi_hi_speed (BOOL on) 
{
   /* Set a SPI clock speed to desired value. */

   if (on == __TRUE) 
   {
      /* Max. 18 MBit used for Data Transfer. */
      SPI2->CR1 &= ~0x0038;

      //SPI2->CR1 |=  0x0008;
   }
   else if (on == 101)
   {
        SPI2->CR1 &= ~0x0038;
        SPI2->CR1 |=  0x0020;
   }
   else
   {
      /* Max. 400 kBit used in Card Initialization. */
      SPI2->CR1 |=  0x0038;
   }
}

void spi_ss (U32 ss) 
{
   /* Enable/Disable SPI Chip Select (drive it high or low). */     
    if(ss)
    {                
       GPIO_SetBits(GPIOA,GPIO_Pin_8);
       os_mut_release (SPI2_RWMutex);
    }
    else
    {
       os_mut_wait (SPI2_RWMutex, 0xFFFF); 
       GPIO_ResetBits(GPIOA,GPIO_Pin_8);        
    }
    
}

u8 spi_send (U8 outb) 
{
   /* Write and Read a byte on SPI interface. */
#define RXNE    0x01
#define TXE     0x02
#define BSY     0x80
   /* Wait if TXE cleared, Tx FIFO is full. */
   
   while (!(SPI2->SR & TXE));
   //while (SPI2->SR & BSY);
   SPI2->DR = outb;

   /* Wait if RNE cleared, Rx FIFO is empty. */
   while (!(SPI2->SR & RXNE));
   return (SPI2->DR);
//    while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) != RESET);
//    SPI_I2S_SendData(SPI2,outb);
    
//    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);
//    return SPI_I2S_ReceiveData(SPI2);
}

void tp_ss(u32 ss)
{
    if(ss)
    {              
       GPIO_SetBits(GPIOB,GPIO_Pin_12);
       os_mut_release (SPI2_RWMutex); 
    }
    else
    {
       os_mut_wait (SPI2_RWMutex, 0xFFFF); 
       GPIO_ResetBits(GPIOB,GPIO_Pin_12);       
    }       
}
