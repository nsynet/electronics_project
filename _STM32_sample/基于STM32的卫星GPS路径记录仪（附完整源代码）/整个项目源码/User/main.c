//-------------------------------------------------------
// GPS Logger V2 2011 by wowbanui
//-------------------------------------------------------

#include "common.h"

#include "stm32f10x.h"
#include "S1D15E06.h"
#include "GPS.h"
#include "tf.h"
#include "stm32f10x_it.h"
#include "keyboard.h"
#include "W25Q16.h"
#include "logger.h"
#include "menu.h"
#include <stdio.h>


u8 FlagScreenUpdate=0;
u8 FlagGPSParseOK=0;
u8 Mode= MODE_SATINFO;
u8 LogStatus = LOG_STOP;


void TIM2_IRQHandler(void)
{
    //定时器中断超时
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    }
}


void USART2_IRQHandler(void)        //串口2接收中断 (GPS)
{
    if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
    {
        LED_G_ON();
        //串口数据中断检测:
        //每次接收到串口数据就 清零中断寄存器值
        TIM_SetCounter(TIM2,0);
        if (GPS_Parser(USART_ReceiveData(USART2))==GPS_PARSE_OK)
            FlagGPSParseOK=1;
        LED_G_OFF();
    }
}

void USART1_IRQHandler(void)        //串口1接收中断
{
    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {
      //  W25Q_WriteBytes(USART_ReceiveData(USART1),FlashAddr);
        USART_ReceiveData(USART1);
    }
}

void USART1_PutSrt(u8 *str)
{
    while(*str)
    {
        USART_SendData(USART1, *str++);
        while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
    }
}



int main(void)
{
    char strTemp[100];
    u8 t;


    //-初始化开始-------------------------
    RCC_Config();
    LCD_IO_Config();
    NVIC_Config();
    TIM_Config();
    USART_Config();
    LED_IO_Config();
    KBD_IO_Init();
    W25Q_IO_Config();

    LCD_Init();
    LCD_Clear();
    //-初始化完成-------------------------
    LCD_SetGray(GRAY_BLACK,GRAY_WHITE);
    LCD_PutStr12(0,0,"-= GPS记录器 V2 wowbanui =-");

    t=TF_Init();
    sprintf(strTemp,"TF 初始化结果: 0x%02X ",t);
    LCD_PutStr12(0,4,strTemp);


    LCD_PutStr12(0,4,"校验中文字库...");
    if (SYS_SheckSumFont()==FONT_TOTAL_CHECKSUM)
        LCD_PutStr12(90,4,"正确");
    else
        LCD_PutStr12(90,4,"错误");
            
    LOG_MountFlash();


    while(1)
    {
        switch(KBD_GetKey())
        {
            case KEY_MENU:
                USART_Cmd(USART2, DISABLE);
                Menu();
                USART_Cmd(USART2, ENABLE);
                FlagScreenUpdate=1;
                break;

            case KEY_UP:
                Mode++;
                if (Mode>MODE_LOG) Mode=MODE_SATINFO;
                FlagScreenUpdate=1;
                break;

            case KEY_DOWN:
                if (LogStatus==LOG_START)
                    LogStatus=LOG_STOP;
                else
                    LogStatus=LOG_START;
                FlagScreenUpdate=1; 
                break;

            case KEY_ENTER:
                break;
        }

        if (FlagGPSParseOK||FlagScreenUpdate)
        {
            if (FlagGPSParseOK)
            {
                FlagGPSParseOK=0;
                if (LogStatus==LOG_START) LOG_Point();
            }

            FlagScreenUpdate=0;

            GPS_View_Basic(LogStatus);
            LCD_GPS_Tab(Mode);
            switch(Mode)
            {
                case MODE_SATINFO:
                    GPS_View_SatInfo();
                    break;
                case MODE_SPEED:
                    GPS_View_Speed();
                    break;
                case MODE_LOG:
                    GPS_View_Log();
                    break;
            }
        }
    }
}

