/**
  ******************************************************************************
  * @file    Project/STM32F4xx_StdPeriph_Templates/stm32f4xx_it.c 
  * @author  MCD Application Team
  * @version V1.5.0
  * @date    06-March-2015
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2015 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_it.h"
#include "Gpio.h"
#include "stdio.h"
#include "stm32f42x_sd.h"
#include "ov7670.h"
#include "ov5642.h"
#include "ov2640.h"
#include "CamConfig.h"
#include "stm32f429i_lcd.h"
/** @addtogroup STM32F4xx_StdPeriph_Examples
  * @{
  */

/** @addtogroup SysTick_Example
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/
u8 CamFrame=0;
u8 DMA_Batch=0;	
u8 CamStop=0;
static uint16_t LineMum =0;//传输行数
/**
  * @brief  This function handles DCMI exception.
  * @param  None
  * @retval None
  */
void DCMI_IRQHandler(void)
{  	 
	//if (DCMI_GetITStatus(DCMI_IT_VSYNC) != RESET)
  if (DCMI_GetITStatus(DCMI_IT_FRAME) != RESET) 		
	{	  
	  LineMum=0;
	  DCMI_CaptureCmd(DISABLE);
		get_image(LCD_FRAME_BUFFER,LCD_PIXEL_WIDTH,LCD_PIXEL_HEIGHT);
        
    LCD_DrawLineScan_ARGB8888();              //绘制扫描线
    DCMI_CaptureCmd(ENABLE);   
		DCMI_ClearITPendingBit(DCMI_IT_FRAME); //清除帧中断标志位
	}

}

/**
  * @brief  This function handles RTC WKUP exception.
  * @param  None
  * @retval None
  */
void RTC_WKUP_IRQHandler(void)
{    
	if(RTC_GetFlagStatus(RTC_FLAG_WUTF)==SET)
	{ 
		printf("CamFrame:%d帧\r\n",CamFrame);//打印帧率
		CamFrame=0;
		RTC_ClearFlag(RTC_FLAG_WUTF);	
	}   
	EXTI_ClearITPendingBit(EXTI_Line22);			
}

/**
  * @brief  This function handles DMA2_Stream1 exception.
  * @param  None
  * @retval None
  */
void DMA2_Stream1_IRQHandler(void)
{
  if(  DMA_GetITStatus(DMA2_Stream1,DMA_IT_TCIF1) == SET )    
  {
		LineMum++;

    if(LineMum==LCD_PIXEL_HEIGHT)
		{		
			LineMum=0;                                                                       //传完一帧图像
		}		
    Cam_DMA_Config(LCD_FRAME_BUFFER+(LCD_PIXEL_WIDTH*2*(LineMum)),LCD_PIXEL_WIDTH*2/4);//传输一行
    DMA_ClearITPendingBit(DMA2_Stream1,DMA_IT_TCIF1);                                  //清除传输完成标记
	}
}

/**
  * @brief  This function handles SDIO exception.
  * @param  None
  * @retval None
  */
void SDIO_IRQHandler(void)
{
	SD_ProcessIRQSrc();
}  


#ifdef SD_DMA_MODE
	void SD_SDIO_DMA_IRQHANDLER(void)
	{
		SD_ProcessDMAIRQ();  
	} 

#endif
	
		
/**
  * @brief  This function handles EXTI15_10 exception.
  * @param  None
  * @retval None
  */
void EXTI15_10_IRQHandler(void) 
{

  if(EXTI_GetITStatus(EXTI_Line11) != RESET)
	{ 		
    EXTI_ClearITPendingBit(EXTI_Line11);                   /* Clear the EXTI line  pending bit */
	}
	
}
/**
  * @brief  This function handles EXTI0 exception.
  * @param  None
  * @retval None
  */
void EXTI0_IRQHandler(void) 
{

	 if(EXTI_GetITStatus(EXTI_Line0) != RESET)
  { 		
    EXTI_ClearITPendingBit(EXTI_Line0);                   /* Clear the EXTI line  pending bit */
  }
	
}
//void DMA2_Stream3_IRQHandler (void)
//{
//  /* Process DMA2 Stream3 or DMA2 Stream6 Interrupt Sources */
// 
//}


extern uint32_t ntime;
void SysTick_Handler(void)
{	
	if(ntime>0)ntime--;
}



void DMA2_Stream4_IRQHandler(void) 	
{
	/* Test on DMA Stream Transfer Complete interrupt */
	if(DMA_GetITStatus(DMA2_Stream4, DMA_IT_TCIF4))
	{
		
	}
}


/**
  * @brief   This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}


/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f4xx.s).                                               */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

void USART3_IRQHandler(void)
{

}

/**
  * @brief  This function handles CAN1 RX0 request.
  * @param  None
  * @retval None
  */
void CAN1_RX0_IRQHandler(void)
{

}

/**
  * @brief  This function handles CAN2 RX0 request.
  * @param  None
  * @retval None
  */
void CAN2_RX0_IRQHandler(void)
{

	
}
/**
  * @}
  */ 

/**
  * @}
  */ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
