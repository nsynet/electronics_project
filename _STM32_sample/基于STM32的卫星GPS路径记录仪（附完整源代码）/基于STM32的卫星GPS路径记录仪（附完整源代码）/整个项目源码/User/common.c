/*************************************************
STM32 ������ʼ������ wowbanui

2011/07/13 Ver. 0.1     �����������ʼ������
*************************************************/
#include "common.h"

void DelayUS(u32 nus)               //��ʱ ��λ us
{
    SysTick->LOAD=nus*9; //ʱ����� 
    SysTick->CTRL|=0x01; //��ʼ���� 
    while(!(SysTick->CTRL&(1<<16))); //�ȴ�ʱ�䵽�� 
    SysTick->CTRL=0X00000000; //�رռ����� 
    SysTick->VAL=0X00000000; //��ռ����� 
}

void RCC_Config(void)
{
    ErrorStatus HSEStartUpStatus;   
    //RCC system reset(for debug purpose)
    //��λ���е�RCC��Χ�豸�Ĵ�����
    //���ı��ڲ�������������λ(HSITRIM[4..0])
    //�Լ���������ƼĴ���(RCC_BDCR),
    //����״̬�Ĵ���RCC_CSR
    RCC_DeInit();

    //Enable HSE 
    //����HSE����
    //��������
    //RCC_HSE_ON   ����
    //RCC_HSE_OFF   �ر�
    //RCC_HSE_BYPASS ʹ���ⲿʱ������
    RCC_HSEConfig(RCC_HSE_ON);

    //Wait till HSE is ready
    //HSEStartUpStatusΪö�����ͱ���,2��ȡֵ
    //0ΪERROR, ��0ΪSUCCESS
    //�ȴ�HSE׼����,����ʱʱ�䵽���˳�
    HSEStartUpStatus = RCC_WaitForHSEStartUp();

    if(HSEStartUpStatus == SUCCESS)
    {
        //Enable Prefetch Buffer
        FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
    
        //Flash 2 wait state
        FLASH_SetLatency(FLASH_Latency_2);
     
        // HCLK = SYSCLK
        //����AHBʱ��,���ʱ�Ӵ�SYSCLK��Ƶ����
        //��Ƶϵ����1,2,4,8,16,64,128,256,512 
        RCC_HCLKConfig(RCC_SYSCLK_Div1); 
      
        //PCLK2 = HCLK
        //���õ���APB2ʱ�ӣ����ʱ�Ӵ�AHBʱ�ӷ�Ƶ����
        //��Ƶϵ��Ϊ1,2,4,8,16
        RCC_PCLK2Config(RCC_HCLK_Div1); 
    
        // PCLK1 = HCLK/2
        //���õ���APB1ʱ�ӣ����ʱ�Ӵ�AHBʱ�ӷ�Ƶ����
        //��Ƶϵ��Ϊ1,2,4,8,16
        RCC_PCLK1Config(RCC_HCLK_Div2);
    
        //PLLCLK = 8MHz * 9 = 72 MHz
        //����PLL��ʱ��Դ�ͳ˷�����
        //��һ����ڲ���Ϊʱ��Դ������3��
        //RCC_PLLSource_HSI_Div2
        //RCC_PLLSource_HSE_Div1
        //RCC_PLLSource_HSE_Div2
        //�˷����� RCC_PLLMul_2~RCC_PLLMul_16,֮���������
        RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);
    
        //Enable PLL 
        RCC_PLLCmd(ENABLE);
    
        //Wait till PLL is ready
        while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
        {}

        //Select PLL as system clock source
        //ѡ��ϵͳʱ��Դ����������
        //RCC_SYSCLKSource_HSI   �ڲ���������
        //RCC_SYSCLKSource_HSE   �ⲿ��������
        //RCC_SYSCLKSource_PLLCLK PLLʱ��
        RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
    
        //Wait till PLL is used as system clock source
        //����ϵͳ��ǰ��ʱ��Դ
        //����ֵ��3��
        //0x00 HSI�ǵ�ǰʱ��Դ
        //0x04 HSE�ǵ�ǰʱ��Դ
        //0x08 PLL�ǵ�ǰʱ��Դ
        while(RCC_GetSYSCLKSource() != 0x08)
        {}
        
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD|
                               RCC_APB2Periph_GPIOE|RCC_APB2Periph_GPIOF|RCC_APB2Periph_GPIOG,ENABLE);
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
    }
}


void NVIC_Config(void)
{
    #ifdef   VECT_TAB_RAM
        NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0);     //Set the Vector Table base location at 0x20000000
    #else    //VECT_TAB_FLASH
        NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);   //Set the Vector Table base location at 0x08000000
    #endif

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
}

void TIM_Config(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    //---------------------------------------------
    //����ʱ��
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    //---------------------------------------------
    //�ж�����
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    //
    //�жϼ�� 100ms
    TIM_TimeBaseStructure.TIM_Period = 3600;
    TIM_TimeBaseStructure.TIM_Prescaler = (36000000/36000)-1;
    TIM_TimeBaseStructure.TIM_ClockDivision=0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
    TIM_Cmd(TIM2, ENABLE);
}


void USART_Config(void)
{
    USART_InitTypeDef USART_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    //---------------------------------------------
    //USART GPIO ����
    //USART2_Tx | USART1_Tx
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
     
    //USART2_Rx | USART1_Rx
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    //---------------------------------------------
    //����ʱ�� *������USART ����ǰ����
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);


    //---------------------------------------------
    //�ж�����
    //����2�����жϴ� (GPS ����)    
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    //����1�����жϴ�                             
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    //--------------------------------------------
    // USART ����
    USART_InitStructure.USART_BaudRate = 9600;                      //BaudRate = 9600 baud
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;     //Word Length = 8 Bits
    USART_InitStructure.USART_StopBits = USART_StopBits_1;          //One Stop Bit
    USART_InitStructure.USART_Parity = USART_Parity_No;             //No parity
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;  //Hardware flow control disabled (RTS and CTS signals)
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx; //Receive and transmit enabled

    //��ʼ������2 (GPS)
    USART_Init(USART2, &USART_InitStructure);
    //-------------------------------------------------------------------------
    //      void USART_ITConfig(USART_TypeDef* USARTx, u16 USART_IT, FunctionalState NewState) 
    //    
    //      USART_IT        ����
    //      -----------------------------
    //      USART_IT_PE     ��ż�����ж� 
    //      USART_IT_TXE    �����ж� 
    //      USART_IT_TC     ��������ж� 
    //      USART_IT_RXNE   �����ж� 
    //      USART_IT_IDLE   ���������ж� 
    //      USART_IT_LBD    LIN�жϼ���ж� 
    //      USART_IT_CTS    CTS�ж� 
    //      USART_IT_ERR    �����ж� 
    //-------------------------------------------------------------------------
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
    USART_Cmd(USART2, ENABLE);

    //��ʼ������ 1
    USART_Init(USART1, &USART_InitStructure);
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
 //   USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
    USART_Cmd(USART1, ENABLE);

}

void LED_IO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_Out_PP;

    LED_Y_OFF();
    LED_G_OFF();
    LED_R_OFF();
    LED_B_OFF();

    GPIO_InitStructure.GPIO_Pin =   GPIO_Pin_11|GPIO_Pin_12;
                                    //PA.11(LED_FRONT_R),PA.12(LED_FRONT_G)
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin =   GPIO_Pin_8|GPIO_Pin_9;
                                    //PB.8(E),PB.9(RW)
    GPIO_Init(GPIOB, &GPIO_InitStructure);

}

