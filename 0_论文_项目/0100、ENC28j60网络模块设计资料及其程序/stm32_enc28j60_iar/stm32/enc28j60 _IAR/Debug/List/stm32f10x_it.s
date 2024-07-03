///////////////////////////////////////////////////////////////////////////////
//                                                                            /
//                                                      21/Apr/2009  21:08:11 /
// IAR ANSI C/C++ Compiler V5.20.2.21007/W32 EVALUATION for ARM               /
// Copyright 1999-2008 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode     =  thumb                                                   /
//    Endian       =  little                                                  /
//    Source file  =  F:\b百利胜\enc28j60\enc28j60 _IAR\stm32f10x_it.c        /
//    Command line =  "F:\b百利胜\enc28j60\enc28j60 _IAR\stm32f10x_it.c"      /
//                    -lcN "F:\b百利胜\enc28j60\enc28j60 _IAR\Debug\List\"    /
//                    -lb "F:\b百利胜\enc28j60\enc28j60 _IAR\Debug\List\" -o  /
//                    "F:\b百利胜\enc28j60\enc28j60 _IAR\Debug\Obj\"          /
//                    --no_cse --no_unroll --no_inline --no_code_motion       /
//                    --no_tbaa --no_clustering --no_scheduling --debug       /
//                    --endian=little --cpu=Cortex-M3 -e --fpu=None           /
//                    --dlib_config "D:\Program Files\IAR Systems\Embedded    /
//                    Workbench 5.0 Evaluation\ARM\INC\DLib_Config_Full.h"    /
//                    -I "F:\b百利胜\enc28j60\enc28j60                        /
//                    _IAR\EWARM_V2.0.2\library\inc\" -I                      /
//                    "F:\b百利胜\enc28j60\enc28j60 _IAR\EWARM_V2.0.2\" -I    /
//                    "F:\b百利胜\enc28j60\enc28j60 _IAR\" -I                 /
//                    "F:\b百利胜\enc28j60\enc28j60 _IAR\TCPIP\" -I           /
//                    "D:\Program Files\IAR Systems\Embedded Workbench 5.0    /
//                    Evaluation\ARM\INC\" -Ol                                /
//    List file    =  F:\b百利胜\enc28j60\enc28j60                            /
//                    _IAR\Debug\List\stm32f10x_it.s                          /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME stm32f10x_it

        EXTERN USART_GetFlagStatus
        EXTERN USART_GetITStatus
        EXTERN USART_ReceiveData
        EXTERN USART_SendData

        PUBLIC ADC1_2_IRQHandler
        PUBLIC ADC3_IRQHandler
        PUBLIC BusFaultException
        PUBLIC CAN_RX1_IRQHandler
        PUBLIC CAN_SCE_IRQHandler
        PUBLIC DMA1_Channel1_IRQHandler
        PUBLIC DMA1_Channel2_IRQHandler
        PUBLIC DMA1_Channel3_IRQHandler
        PUBLIC DMA1_Channel4_IRQHandler
        PUBLIC DMA1_Channel5_IRQHandler
        PUBLIC DMA1_Channel6_IRQHandler
        PUBLIC DMA1_Channel7_IRQHandler
        PUBLIC DMA2_Channel1_IRQHandler
        PUBLIC DMA2_Channel2_IRQHandler
        PUBLIC DMA2_Channel3_IRQHandler
        PUBLIC DMA2_Channel4_5_IRQHandler
        PUBLIC DebugMonitor
        PUBLIC EXTI0_IRQHandler
        PUBLIC EXTI15_10_IRQHandler
        PUBLIC EXTI1_IRQHandler
        PUBLIC EXTI2_IRQHandler
        PUBLIC EXTI3_IRQHandler
        PUBLIC EXTI4_IRQHandler
        PUBLIC EXTI9_5_IRQHandler
        PUBLIC FLASH_IRQHandler
        PUBLIC FSMC_IRQHandler
        PUBLIC HardFaultException
        PUBLIC I2C1_ER_IRQHandler
        PUBLIC I2C1_EV_IRQHandler
        PUBLIC I2C2_ER_IRQHandler
        PUBLIC I2C2_EV_IRQHandler
        PUBLIC MemManageException
        PUBLIC NMIException
        PUBLIC PVD_IRQHandler
        PUBLIC PendSVC
        PUBLIC RCC_IRQHandler
        PUBLIC RTCAlarm_IRQHandler
        PUBLIC RTC_IRQHandler
        PUBLIC SDIO_IRQHandler
        PUBLIC SPI1_IRQHandler
        PUBLIC SPI2_IRQHandler
        PUBLIC SPI3_IRQHandler
        PUBLIC SVCHandler
        PUBLIC SysTickHandler
        PUBLIC TAMPER_IRQHandler
        PUBLIC TIM1_BRK_IRQHandler
        PUBLIC TIM1_CC_IRQHandler
        PUBLIC TIM1_TRG_COM_IRQHandler
        PUBLIC TIM1_UP_IRQHandler
        PUBLIC TIM2_IRQHandler
        PUBLIC TIM3_IRQHandler
        PUBLIC TIM4_IRQHandler
        PUBLIC TIM5_IRQHandler
        PUBLIC TIM6_IRQHandler
        PUBLIC TIM7_IRQHandler
        PUBLIC TIM8_BRK_IRQHandler
        PUBLIC TIM8_CC_IRQHandler
        PUBLIC TIM8_TRG_COM_IRQHandler
        PUBLIC TIM8_UP_IRQHandler
        PUBLIC UART4_IRQHandler
        PUBLIC UART5_IRQHandler
        PUBLIC USART1_IRQHandler
        PUBLIC USART2_IRQHandler
        PUBLIC USART3_IRQHandler
        PUBLIC USBWakeUp_IRQHandler
        PUBLIC USB_HP_CAN_TX_IRQHandler
        PUBLIC USB_LP_CAN_RX0_IRQHandler
        PUBLIC UsageFaultException
        PUBLIC WWDG_IRQHandler
        PUBLIC flog


        SECTION `.bss`:DATA:NOROOT(0)
flog:
        DS8 1

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
NMIException:
        BX       LR               ;; return

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
HardFaultException:
??HardFaultException_0:
        B.N      ??HardFaultException_0

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
MemManageException:
??MemManageException_0:
        B.N      ??MemManageException_0

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
BusFaultException:
??BusFaultException_0:
        B.N      ??BusFaultException_0

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
UsageFaultException:
??UsageFaultException_0:
        B.N      ??UsageFaultException_0

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
DebugMonitor:
        BX       LR               ;; return

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
SVCHandler:
        BX       LR               ;; return

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
PendSVC:
        BX       LR               ;; return

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
SysTickHandler:
        BX       LR               ;; return

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
WWDG_IRQHandler:
        BX       LR               ;; return

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
PVD_IRQHandler:
        BX       LR               ;; return

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
TAMPER_IRQHandler:
        BX       LR               ;; return

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
RTC_IRQHandler:
        BX       LR               ;; return

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
FLASH_IRQHandler:
        BX       LR               ;; return

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
RCC_IRQHandler:
        BX       LR               ;; return

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
EXTI0_IRQHandler:
        BX       LR               ;; return

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
EXTI1_IRQHandler:
        BX       LR               ;; return

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
EXTI2_IRQHandler:
        BX       LR               ;; return

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
EXTI3_IRQHandler:
        BX       LR               ;; return

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
EXTI4_IRQHandler:
        BX       LR               ;; return

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
DMA1_Channel1_IRQHandler:
        BX       LR               ;; return

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
DMA1_Channel2_IRQHandler:
        BX       LR               ;; return

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
DMA1_Channel3_IRQHandler:
        BX       LR               ;; return

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
DMA1_Channel4_IRQHandler:
        BX       LR               ;; return

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
DMA1_Channel5_IRQHandler:
        BX       LR               ;; return

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
DMA1_Channel6_IRQHandler:
        BX       LR               ;; return

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
DMA1_Channel7_IRQHandler:
        BX       LR               ;; return

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
ADC1_2_IRQHandler:
        BX       LR               ;; return

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
USB_HP_CAN_TX_IRQHandler:
        BX       LR               ;; return

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
USB_LP_CAN_RX0_IRQHandler:
        BX       LR               ;; return

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
CAN_RX1_IRQHandler:
        BX       LR               ;; return

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
CAN_SCE_IRQHandler:
        BX       LR               ;; return

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
EXTI9_5_IRQHandler:
        BX       LR               ;; return

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
TIM1_BRK_IRQHandler:
        BX       LR               ;; return

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
TIM1_UP_IRQHandler:
        BX       LR               ;; return

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
TIM1_TRG_COM_IRQHandler:
        BX       LR               ;; return

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
TIM1_CC_IRQHandler:
        BX       LR               ;; return

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
TIM2_IRQHandler:
        BX       LR               ;; return

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
TIM3_IRQHandler:
        BX       LR               ;; return

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
TIM4_IRQHandler:
        BX       LR               ;; return

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
I2C1_EV_IRQHandler:
        BX       LR               ;; return

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
I2C1_ER_IRQHandler:
        BX       LR               ;; return

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
I2C2_EV_IRQHandler:
        BX       LR               ;; return

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
I2C2_ER_IRQHandler:
        BX       LR               ;; return

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
SPI1_IRQHandler:
        BX       LR               ;; return

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
SPI2_IRQHandler:
        BX       LR               ;; return

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
USART1_IRQHandler:
        PUSH     {R7,LR}
        MOVW     R1,#+1317
        LDR.N    R0,??USART1_IRQHandler_0  ;; 0x40013800
        BL       USART_GetITStatus
        CMP      R0,#+0
        BEQ.N    ??USART1_IRQHandler_1
        LDR.N    R0,??USART1_IRQHandler_0  ;; 0x40013800
        BL       USART_ReceiveData
        MOVS     R1,R0
        UXTH     R1,R1            ;; ZeroExt  R1,R1,#+16,#+16
        LDR.N    R0,??USART1_IRQHandler_0  ;; 0x40013800
        BL       USART_SendData
??USART1_IRQHandler_2:
        MOVS     R1,#+128
        LDR.N    R0,??USART1_IRQHandler_0  ;; 0x40013800
        BL       USART_GetFlagStatus
        CMP      R0,#+0
        BEQ.N    ??USART1_IRQHandler_2
??USART1_IRQHandler_1:
        POP      {R0,PC}          ;; return
        Nop      
        DATA
??USART1_IRQHandler_0:
        DC32     0x40013800

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
USART2_IRQHandler:
        BX       LR               ;; return

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
USART3_IRQHandler:
        BX       LR               ;; return

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
EXTI15_10_IRQHandler:
        BX       LR               ;; return

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
RTCAlarm_IRQHandler:
        BX       LR               ;; return

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
USBWakeUp_IRQHandler:
        BX       LR               ;; return

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
TIM8_BRK_IRQHandler:
        BX       LR               ;; return

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
TIM8_UP_IRQHandler:
        BX       LR               ;; return

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
TIM8_TRG_COM_IRQHandler:
        BX       LR               ;; return

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
TIM8_CC_IRQHandler:
        BX       LR               ;; return

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
ADC3_IRQHandler:
        BX       LR               ;; return

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
FSMC_IRQHandler:
        BX       LR               ;; return

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
SDIO_IRQHandler:
        BX       LR               ;; return

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
TIM5_IRQHandler:
        BX       LR               ;; return

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
SPI3_IRQHandler:
        BX       LR               ;; return

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
UART4_IRQHandler:
        BX       LR               ;; return

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
UART5_IRQHandler:
        BX       LR               ;; return

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
TIM6_IRQHandler:
        BX       LR               ;; return

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
TIM7_IRQHandler:
        BX       LR               ;; return

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
DMA2_Channel1_IRQHandler:
        BX       LR               ;; return

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
DMA2_Channel2_IRQHandler:
        BX       LR               ;; return

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
DMA2_Channel3_IRQHandler:
        BX       LR               ;; return

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
DMA2_Channel4_5_IRQHandler:
        BX       LR               ;; return

        END
// 
//   1 byte  in section .bss
// 188 bytes in section .text
// 
// 188 bytes of CODE memory
//   1 byte  of DATA memory
//
//Errors: none
//Warnings: none
