/*******************************************************************************                                      
  *                                               
  ******************************************************************************
  * 文件名称：UART.C
  * 描    述：串口通信模块
  * 创 建 者: 
  * 创建日期:    
  * 修 改 者:             
  * 修改日期: 
  * 版    本: v1.0.0
  ******************************************************************************
  * attention
  *
  ******************************************************************************
  */
/*Includes --------------------------------------------------------------------*/
  #include "includes.h"
  #include "UART.H" 
  #include <stdarg.h>
/* 类型定义 typedef-----------------------------------------------------------*/
/* 预定义字符     ------------------------------------------------------------*/
/* 宏定义        -------------------------------------------------------------*/
/* 变量定义          ---------------------------------------------------------*/ 



/*******************************************************************************
* 函数名称: Uart_Init()
* 功能描述: 串口初始化
* 输入参数: void
* 返回参数: 无
********************************************************************************/
void Uart_Init(void)
{
   
        GPIO_InitTypeDef  GPIO_InitStructure;
        USART_InitTypeDef USART_InitStructure;
                    
        RCC_AHBPeriphClockCmd( F072RB_COM_PORT_CLK   , ENABLE);   // 使能GPIOA端口
        RCC_APB1PeriphClockCmd(F072RB_COM_CLK   , ENABLE); // 使能串口2时钟
	    
        GPIO_PinAFConfig(F072RB_COM_PORT ,F072RB_COM_TX_SOURCE, F072RB_COM_TX_AF);
        GPIO_PinAFConfig(F072RB_COM_PORT ,F072RB_COM_RX_SOURCE, F072RB_COM_RX_AF);                           
/////////////////////////////////////////////////////////////////////////////////////        
        /* PA9==TX PA10-RX  */  
                                      
        GPIO_InitStructure.GPIO_Pin   = F072RB_COM_TX_PIN ;                 
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF; 
        GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
        GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
        GPIO_Init(F072RB_COM_PORT, &GPIO_InitStructure); 

        GPIO_InitStructure.GPIO_Pin   = F072RB_COM_RX_PIN ;
	      GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF; 
        GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;	
        GPIO_Init(F072RB_COM_PORT , &GPIO_InitStructure); 
///////////////////////////////////////////////////////////////////////////////////// 
           
        USART_InitStructure.USART_BaudRate = 115200;                 		      
        USART_InitStructure.USART_WordLength = USART_WordLength_8b;     	       
        USART_InitStructure.USART_StopBits = USART_StopBits_1;                         
        USART_InitStructure.USART_Parity = USART_Parity_No;                            
        USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
        USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;               
        USART_Init(F072RB_COM, &USART_InitStructure); 	//串口配置
        
        USART_ITConfig(F072RB_COM, USART_IT_RXNE,ENABLE);//打开中断	
				
        USART_Cmd(F072RB_COM, ENABLE);//使能串口2
        USART_ClearFlag(F072RB_COM, USART_FLAG_TC);  
	
}
/*******************************************************************************
* 函数名称: Uart_Put_Char()
* 功能描述: 通过UART发送一个字符
* 输入参数: void
* 返回参数: 无
********************************************************************************/
void Uart_Put_Char(unsigned char c)
{    
     while (USART_GetFlagStatus(F072RB_COM, USART_FLAG_TXE) == RESET);	
	   USART_SendData(F072RB_COM,c);  
     
	   
}

/*******************************************************************************
* 函数名称: Uart_Get_Char()
* 功能描述: 通过UART接收一个字符
* 输入参数: void
* 返回参数: 无
********************************************************************************/ 
unsigned char Uart_Get_Char(void)
{    
     unsigned char temp;
     while(USART_GetFlagStatus(F072RB_COM, USART_FLAG_RXNE) == RESET);//等待接收为空
     temp = (unsigned char)(USART_ReceiveData(F072RB_COM));
     return(temp);    
}
/*******************************************************************************
函数名称：fputc(int ch, FILE *f)
功    能：串口实现Printf()函数功能
参    数：无
返回值  ：
********************************************************************************/
int fputc(int ch, FILE *f)
{
  while (USART_GetFlagStatus(F072RB_COM, USART_FLAG_TXE) == RESET);
  USART_SendData(F072RB_COM,(uint8_t)ch);
  return (ch);
}








	
 
