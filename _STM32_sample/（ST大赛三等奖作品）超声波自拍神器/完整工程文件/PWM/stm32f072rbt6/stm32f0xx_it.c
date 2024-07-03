/**
  ******************************************************************************
  * @file    Demonstrations/Src/stm32f0xx_it.c
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    18-June-2014
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2014 STMicroelectronics</center></h2>
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
#include "includes.h"
#include "stm32f0xx_it.h"

/** @addtogroup STM32F0xx_HAL_Demonstrations
  * @{
  */

/** @addtogroup Demo
  * @{
  */ 
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
   extern unsigned short int systick_num;
	 extern unsigned short int key_num;
   extern unsigned char Uart_buf[2];
	 
	 extern void delay_ms(unsigned char a);
	 extern void sleep_mode(void);
	 extern unsigned char flag;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M0 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
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
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
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

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
 
}

/******************************************************************************/
/*                 STM32F0xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) , for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f0xx.s).                                               */
/******************************************************************************/

/*******************************************************************************
* ��������: USART2_IRQHandler()
* ��������: ����2�жϴ�����
* �������: void
* ���ز���: ��
* ˵    ������
********************************************************************************/
void USART2_IRQHandler(void)
{


}
/*******************************************************************************
* ��������: EXTI4_15_IRQHandler()
* ��������: �ⲿ�жϴ�����
* �������: void
* ���ز���: ��
* ˵    ������
********************************************************************************/
void EXTI4_15_IRQHandler(void)  
{
    if(EXTI_GetITStatus(KEY1_BUTTON_EXTI_LINE) != RESET)  //��13
    {
			 GPIO_SetBits(LED_PORT ,LED_PIN_1);  //����LD2
			 Timer1_Init(503,5);  //15870hz    ����
			 delay_ms(250);
			 Timer1_Init(511,5);  //15617hz
			 delay_ms(250);
			 TIM_Cmd(F072B_TIMER_PWM, DISABLE);  //ʧ�ܶ�ʱ��
       EXTI_ClearITPendingBit(KEY1_BUTTON_EXTI_LINE);  //�����־λ
    }
		
		if(EXTI_GetITStatus(KEY2_BUTTON_EXTI_LINE) != RESET)  //��12
    {
			 GPIO_SetBits(LED_PORT ,LED_PIN_1); //����LD2
			 Timer1_Init(483,5);  //16526hz     �л�����ͷ
			 delay_ms(250);
			 Timer1_Init(511,5);  //15620hz
			 delay_ms(250);
			 TIM_Cmd(F072B_TIMER_PWM, DISABLE);  //ʧ�ܶ�ʱ��
       EXTI_ClearITPendingBit(KEY2_BUTTON_EXTI_LINE);  //�����־λ
    }
		GPIO_ResetBits(LED_PORT ,LED_PIN_1);  //Ϩ��LD2
		flag = 1;	
}

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
