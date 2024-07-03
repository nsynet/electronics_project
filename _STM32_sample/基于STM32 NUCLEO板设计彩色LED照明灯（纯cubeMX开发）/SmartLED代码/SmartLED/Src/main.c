/**
  ******************************************************************************
  * File Name          : main.c
  * Date               : 17/12/2014 19:03:19
  * Description        : Main program body
  ******************************************************************************
  *
  * COPYRIGHT(c) 2014 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_hal.h"
#include "stdbool.h"
#include "string.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim2;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM2_Init(void);
static void MX_USART2_UART_Init(void);

/* USER CODE BEGIN PFP */
void scheduler( void );
bool serial2_in(uint8_t *pchByte);
bool serial2_out(uint8_t chByte) ;

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM2_Init();
  MX_USART2_UART_Init();

  /* USER CODE BEGIN 2 */
    HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_3);
  /* USER CODE END 2 */

  /* USER CODE BEGIN 3 */
  /* Infinite loop */
  while (1)
  {
	scheduler();
  }
  /* USER CODE END 3 */

}

/** System Clock Configuration
*/
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInit;

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL12;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV2;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1);

  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART2;
  PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_HSI;
  HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit);

  __SYSCFG_CLK_ENABLE();

}

/* TIM2 init function */
void MX_TIM2_Init(void)
{

  TIM_MasterConfigTypeDef sMasterConfig;
  TIM_OC_InitTypeDef sConfigOC;

  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 1875;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 256;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  HAL_TIM_PWM_Init(&htim2);

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig);

  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 80;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_1);

  sConfigOC.Pulse = 160;
  HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_2);

  sConfigOC.Pulse = 240;
  HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_3);
  
}

/* USART2 init function */
void MX_USART2_UART_Init(void)
{

  huart2.Instance = USART2;
  huart2.Init.BaudRate = 9600;//38400;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONEBIT_SAMPLING_DISABLED ;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  HAL_UART_Init(&huart2);

}

/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
*/
void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __GPIOC_CLK_ENABLE();
  __GPIOF_CLK_ENABLE();
  __GPIOA_CLK_ENABLE();
  __GPIOB_CLK_ENABLE();

  /*Configure GPIO pin : PC13 */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_EVT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : PA5 */
  GPIO_InitStruct.Pin = GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

/* USER CODE BEGIN 4 */

//! \name finit state machine state
//! @{
typedef enum {
    fsm_rt_err          = -1,    //!< fsm error, error code can be get from other interface
    fsm_rt_cpl          = 0,     //!< fsm complete
    fsm_rt_on_going     = 1,     //!< fsm on-going
} fsm_rt_t;
//! @}

static uint8_t analysis_val(uint8_t chValH, uint8_t chValL);
static void adjust(void);
static fsm_rt_t task_receive(void);
static fsm_rt_t task_adjust(void);
static fsm_rt_t task_delay(void);
static fsm_rt_t delay(void);
static bool s_bFlag = false;
static bool s_bRgbUpdataFlag = false;

static uint8_t s_chRVal = 0;
static uint8_t s_chGVal = 0;
static uint8_t s_chBVal = 0;
/****************************************************************************/
/**
 * Function Name: void scheduler( void )
 * Description: none
 *
 * Param:   none
 * Return:  none
 * Author:  
 ****************************************************************************/
void scheduler( void )
{
	uint8_t chByte;
    while(!serial2_out('S'));
    while(!serial2_out('m'));
    while(!serial2_out('a'));
    while(!serial2_out('r'));
    while(!serial2_out('t'));
    while(!serial2_out('L'));
	while(!serial2_out('E'));
	while(!serial2_out('D'));
    while(!serial2_out('\r'));
    while(!serial2_out('\n'));
    
    /* loop here forever */
	while(1){    
	//	if (serial2_in(&chByte)) {
    //       serial2_out(chByte);
    //    }
        task_receive();  
		task_adjust(); 		
        //task_delay();     
	}
		
}

#define TASK_RECEIVE_RESET_FSM()	\
	do{	\
		s_tState = START;\
	}while(0)	
static fsm_rt_t task_receive(void)
{
	static uint8_t s_chLedVal[8];
	static uint8_t s_chLedInd = 0;
	static uint32_t s_wReceiveCounter = 0x2ffff;
	static enum{
		START=0,
		IDLE,
		RECEIVE,
		ANALYSIS
	}s_tState = START;
	switch (s_tState){
		case START:
			s_chLedVal[0] = 0x00;
		s_chLedVal[1] = 0x00;
		s_chLedVal[2] = 0x00;
		s_chLedVal[3] = 0x00;
		s_chLedVal[4] = 0x00;
		s_chLedVal[5] = 0x00;
		s_chLedVal[6] = 0x00;
		s_chLedVal[7] = 0x00;
		
			s_chLedInd = 0;
			s_wReceiveCounter = 0x2ffff;
			s_tState = IDLE;
		
		case IDLE:
			if(serial2_in(&s_chLedVal[s_chLedInd])){
				if('*'==s_chLedVal[s_chLedInd]){/*'*' is start mark*/
					s_chLedInd++;
					s_tState = RECEIVE;
				}else{
					s_tState = IDLE;
				}
			}
			break;
			
		case RECEIVE:
			if(serial2_in(&s_chLedVal[s_chLedInd])){				
				if('#'==s_chLedVal[s_chLedInd]){/*'#' is end mark*/
					if(7 == s_chLedInd){/*verify length */
						s_tState = ANALYSIS;
					}else{
						s_tState = START;
					}					
				}
				s_chLedInd++;
			}
			/*time out*/
			s_wReceiveCounter--;
			if(0 == s_wReceiveCounter){
				TASK_RECEIVE_RESET_FSM();
			}
			break;
			
		case ANALYSIS:
			s_chRVal = analysis_val(s_chLedVal[1],s_chLedVal[2]);
			s_chGVal = analysis_val(s_chLedVal[3],s_chLedVal[4]);
			s_chBVal = analysis_val(s_chLedVal[5],s_chLedVal[6]);
			s_bRgbUpdataFlag = true;
			TASK_RECEIVE_RESET_FSM();
			return fsm_rt_cpl;
	}
	return fsm_rt_on_going;
}

static uint8_t analysis_val(uint8_t chValH, uint8_t chValL)
{
	uint8_t chVal = 0;
	if((chValH >= 48) && (chValH <= 57)){/*'0'~'9'*/
		chVal = (chValH-48)<<4;
	}else if((chValH >= 65) && (chValH <= 70)){/*'A'~'F'*/
		chVal = (chValH-55)<<4;
	}
	
	if((chValL >= 48) && (chValL <= 57)){/*'0'~'9'*/
		chVal |= chValL-48;
	}else if((chValH >= 65) && (chValH <= 70)){/*'A'~'F'*/
		chVal |= chValL-55;
	}
	
	return chVal;
}

#define TASK_ADJUST_RESET_FSM()	\
	do{	\
		s_tState = START;\
	}while(0)
static fsm_rt_t task_adjust(void)
{
	static enum{
		START=0,
		IDLE,
		ADJUST
	}s_tState = START;
	switch(s_tState){
		case START:
			s_tState = IDLE;
		
		case IDLE:
			if(s_bRgbUpdataFlag){
				s_bRgbUpdataFlag = false;
				s_tState = ADJUST;
			}
			break;
		
		case ADJUST:
			adjust();
			while(!serial2_out('O'));
			while(!serial2_out('K'));
			while(!serial2_out('\r'));
			while(!serial2_out('\n'));
			TASK_ADJUST_RESET_FSM();
			return fsm_rt_cpl;
	}
	
	return fsm_rt_on_going;
}

static void adjust(void)
{
	TIM_OC_InitTypeDef sConfigOC;
	
	sConfigOC.OCMode = TIM_OCMODE_PWM1;
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;

	sConfigOC.Pulse = s_chRVal;
	HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_1);

	sConfigOC.Pulse = s_chGVal;
	HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_2);

	sConfigOC.Pulse = s_chBVal;
	HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_3);
	
	HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_3);
}

#define TASK_DELAY_RESET_FSM()	\
	do{	\
		s_tTaskDelayState = TASK_DELAY_START;	\
	}while(0)	
static  fsm_rt_t task_delay(void)
{
	static enum{
		TASK_DELAY_START=0,
		TASK_DELAY_ING
	}s_tTaskDelayState = TASK_DELAY_START;
	switch(s_tTaskDelayState){
		case TASK_DELAY_START:
			s_bFlag = false;
		
		case TASK_DELAY_ING:
			if(fsm_rt_cpl == delay()){
				s_bFlag = true;
				TASK_DELAY_RESET_FSM();
				return fsm_rt_on_going;
			}
	}
	return fsm_rt_on_going;
}

#define DELAY_RESET_FSM()	\
	do{	\
		s_tDelaytState = DELAY_START;	\
	}while(0)
#define DEALY_TIME (0xff)
	
static fsm_rt_t delay(void)
{
	static enum{
		DELAY_START = 0,
		DELAY
	}s_tDelaytState = DELAY_START;
	static uint32_t s_wDelayCounter = 0;
	switch(s_tDelaytState){
		case DELAY_START:
			s_wDelayCounter = 0;
			s_tDelaytState = DELAY;

		case DELAY:
			s_wDelayCounter ++;
			if(s_wDelayCounter > DEALY_TIME){	
				s_wDelayCounter = 0;				
				DELAY_RESET_FSM();
				return fsm_rt_cpl;
			}
			break;
	}			
	return fsm_rt_on_going;
}

bool serial2_out(uint8_t chByte) 
{
    bool bUartState;
	if(HAL_UART_Transmit(&huart2,&chByte,1,10) == HAL_OK){
		bUartState = true;
	}else{
		bUartState = false;
	}
    return bUartState;
}

bool serial2_in(uint8_t *pchByte)
{
    bool bUartState;
    if(0 == pchByte){ 
         bUartState = false;
    }else{
        if(HAL_UART_Receive(&huart2,pchByte,1,0x1fffff) == HAL_OK){
			bUartState = true;
		}else{
			bUartState = false;
		}
	}
    return bUartState;
}
/* USER CODE END 4 */

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
