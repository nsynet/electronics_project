/*************************************************
STM32 基本初始化代码 wowbanui

2011/07/13 Ver. 0.1     单独分离出初始化代码
*************************************************/
#include "common.h"

void DelayUS(u32 nus)               //延时 单位 us
{
    SysTick->LOAD=nus*9; //时间加载 
    SysTick->CTRL|=0x01; //开始倒数 
    while(!(SysTick->CTRL&(1<<16))); //等待时间到达 
    SysTick->CTRL=0X00000000; //关闭计数器 
    SysTick->VAL=0X00000000; //清空计数器 
}

void RCC_Config(void)
{
    ErrorStatus HSEStartUpStatus;   
    //RCC system reset(for debug purpose)
    //复位所有的RCC外围设备寄存器，
    //不改变内部高速振荡器调整位(HSITRIM[4..0])
    //以及备份域控制寄存器(RCC_BDCR),
    //控制状态寄存器RCC_CSR
    RCC_DeInit();

    //Enable HSE 
    //开启HSE振荡器
    //三个参数
    //RCC_HSE_ON   开启
    //RCC_HSE_OFF   关闭
    //RCC_HSE_BYPASS 使用外部时钟振荡器
    RCC_HSEConfig(RCC_HSE_ON);

    //Wait till HSE is ready
    //HSEStartUpStatus为枚举类型变量,2种取值
    //0为ERROR, 非0为SUCCESS
    //等待HSE准备好,若超时时间到则退出
    HSEStartUpStatus = RCC_WaitForHSEStartUp();

    if(HSEStartUpStatus == SUCCESS)
    {
        //Enable Prefetch Buffer
        FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
    
        //Flash 2 wait state
        FLASH_SetLatency(FLASH_Latency_2);
     
        // HCLK = SYSCLK
        //配置AHB时钟,这个时钟从SYSCLK分频而来
        //分频系数有1,2,4,8,16,64,128,256,512 
        RCC_HCLKConfig(RCC_SYSCLK_Div1); 
      
        //PCLK2 = HCLK
        //设置低速APB2时钟，这个时钟从AHB时钟分频而来
        //分频系数为1,2,4,8,16
        RCC_PCLK2Config(RCC_HCLK_Div1); 
    
        // PCLK1 = HCLK/2
        //设置低速APB1时钟，这个时钟从AHB时钟分频而来
        //分频系数为1,2,4,8,16
        RCC_PCLK1Config(RCC_HCLK_Div2);
    
        //PLLCLK = 8MHz * 9 = 72 MHz
        //设置PLL的时钟源和乘法因子
        //第一个入口参数为时钟源，共有3个
        //RCC_PLLSource_HSI_Div2
        //RCC_PLLSource_HSE_Div1
        //RCC_PLLSource_HSE_Div2
        //乘法因子 RCC_PLLMul_2~RCC_PLLMul_16,之间参数连续
        RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);
    
        //Enable PLL 
        RCC_PLLCmd(ENABLE);
    
        //Wait till PLL is ready
        while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
        {}

        //Select PLL as system clock source
        //选择系统时钟源，三个参数
        //RCC_SYSCLKSource_HSI   内部高速振荡器
        //RCC_SYSCLKSource_HSE   外部高速振荡器
        //RCC_SYSCLKSource_PLLCLK PLL时钟
        RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
    
        //Wait till PLL is used as system clock source
        //返回系统当前的时钟源
        //返回值有3种
        //0x00 HSI是当前时钟源
        //0x04 HSE是当前时钟源
        //0x08 PLL是当前时钟源
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
    //启动时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    //---------------------------------------------
    //中断配置
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    //
    //中断间隔 100ms
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
    //USART GPIO 配置
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
    //启动时钟 *必须在USART 配置前调用
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);


    //---------------------------------------------
    //中断配置
    //串口2接收中断打开 (GPS 接收)    
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    //串口1接收中断打开                             
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    //--------------------------------------------
    // USART 配置
    USART_InitStructure.USART_BaudRate = 9600;                      //BaudRate = 9600 baud
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;     //Word Length = 8 Bits
    USART_InitStructure.USART_StopBits = USART_StopBits_1;          //One Stop Bit
    USART_InitStructure.USART_Parity = USART_Parity_No;             //No parity
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;  //Hardware flow control disabled (RTS and CTS signals)
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx; //Receive and transmit enabled

    //初始化串口2 (GPS)
    USART_Init(USART2, &USART_InitStructure);
    //-------------------------------------------------------------------------
    //      void USART_ITConfig(USART_TypeDef* USARTx, u16 USART_IT, FunctionalState NewState) 
    //    
    //      USART_IT        描述
    //      -----------------------------
    //      USART_IT_PE     奇偶错误中断 
    //      USART_IT_TXE    发送中断 
    //      USART_IT_TC     传输完成中断 
    //      USART_IT_RXNE   接收中断 
    //      USART_IT_IDLE   空闲总线中断 
    //      USART_IT_LBD    LIN中断检测中断 
    //      USART_IT_CTS    CTS中断 
    //      USART_IT_ERR    错误中断 
    //-------------------------------------------------------------------------
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
    USART_Cmd(USART2, ENABLE);

    //初始化串口 1
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

