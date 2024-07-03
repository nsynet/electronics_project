///////////////////////////////////////////////////////////////////////////////
//                                                                            /
//                                                      21/Apr/2009  21:08:10 /
// IAR ANSI C/C++ Compiler V5.20.2.21007/W32 EVALUATION for ARM               /
// Copyright 1999-2008 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode     =  thumb                                                   /
//    Endian       =  little                                                  /
//    Source file  =  F:\b百利胜\enc28j60\enc28j60 _IAR\main.c                /
//    Command line =  "F:\b百利胜\enc28j60\enc28j60 _IAR\main.c" -lcN         /
//                    "F:\b百利胜\enc28j60\enc28j60 _IAR\Debug\List\" -lb     /
//                    "F:\b百利胜\enc28j60\enc28j60 _IAR\Debug\List\" -o      /
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
//    List file    =  F:\b百利胜\enc28j60\enc28j60 _IAR\Debug\List\main.s     /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME main

        EXTERN FLASH_PrefetchBufferCmd
        EXTERN FLASH_SetLatency
        EXTERN GPIO_ResetBits
        EXTERN GPIO_SetBits
        EXTERN NVIC_SetVectorTable
        EXTERN RCC_APB2PeriphClockCmd
        EXTERN RCC_DeInit
        EXTERN RCC_GetFlagStatus
        EXTERN RCC_GetSYSCLKSource
        EXTERN RCC_HCLKConfig
        EXTERN RCC_PCLK1Config
        EXTERN RCC_PCLK2Config
        EXTERN RCC_PLLCmd
        EXTERN RCC_PLLConfig
        EXTERN RCC_SYSCLKConfig
        EXTERN SPI1_Init
        EXTERN UART1_Configuration
        EXTERN UART1_GPIO_Configuration
        EXTERN UART1_NVIC_Configuration
        EXTERN USART_GetFlagStatus
        EXTERN USART_SendData
        EXTERN Uart1_PutString
        EXTERN enc28j60Init
        EXTERN enc28j60getrev
        EXTERN printf
        EXTERN simple_server

        PUBLIC Delay
        PUBLIC NVIC_Configuration
        PUBLIC RCC_HSI_Configuration
        PUBLIC SPI1_Buffer_Tx
        PUBLIC SPI2_Buffer_Tx
        PUBLIC SendChar
        PUBLIC enc28j60_MAC
        PUBLIC fputc
        PUBLIC main
        PUBLIC my_PWM


        SECTION `.data`:DATA:NOROOT(2)
// u16 __data my_PWM[30]
my_PWM:
        DATA
        DC16 200, 1400, 200, 1400, 400, 1200, 400, 1200, 800, 800, 800, 800
        DC16 200, 1480, 400, 1480, 800, 880, 200, 1320, 400, 1120, 800, 720
        DC16 60000, 5000, 40000, 25500, 30000, 30000

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
Delay:
        PUSH     {R0}
        SUB      SP,SP,#+4
        B.N      ??Delay_0
??Delay_1:
        LDR      R0,[SP, #+4]
        SUBS     R0,R0,#+1
        STR      R0,[SP, #+4]
??Delay_0:
        LDR      R0,[SP, #+4]
        CMP      R0,#+0
        BNE.N    ??Delay_1
        ADD      SP,SP,#+8
        BX       LR               ;; return

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
SendChar:
??SendChar_0:
        LDR.N    R1,??DataTable7  ;; 0x40013800
        LDRH     R1,[R1, #+0]
        TST      R1,#0x80
        BEQ.N    ??SendChar_0
        MOVS     R1,R0
        LSLS     R1,R1,#+23
        LSRS     R1,R1,#+23
        LDR.N    R2,??SendChar_1  ;; 0x40013804
        STRH     R1,[R2, #+0]
        BX       LR               ;; return
        Nop      
        DATA
??SendChar_1:
        DC32     0x40013804

        SECTION `.rodata`:CONST:NOROOT(2)
`?<Constant "this OK !__This first...">`:
        DATA
        DC8 "this OK !__This first usar\012\012"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:NOROOT(2)
`?<Constant "\\n The printf is OK?">`:
        DATA
        DC8 "\012 The printf is OK?"

        SECTION `.rodata`:CONST:NOROOT(2)
`?<Constant "\\n The number is %f">`:
        DATA
        DC8 "\012 The number is %f"
        DC8 0

        SECTION `.rodata`:CONST:NOROOT(2)
`?<Constant "SPI1_Init starting...\\r\\n">`:
        DATA
        DC8 "SPI1_Init starting...\015\012"

        SECTION `.rodata`:CONST:NOROOT(2)
`?<Constant "enc28j60 init...\\r\\n">`:
        DATA
        DC8 "enc28j60 init...\015\012"
        DC8 0

        SECTION `.rodata`:CONST:NOROOT(2)
enc28j60_MAC:
        DATA
        DC8 17, 2, 3, 4, 5, 102, 0, 0

        SECTION `.data`:DATA:NOROOT(2)
SPI1_Buffer_Tx:
        DATA
        DC8 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19
        DC8 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32

        SECTION `.data`:DATA:NOROOT(2)
SPI2_Buffer_Tx:
        DATA
        DC8 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97
        DC8 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111
        DC8 112

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
main:
        PUSH     {R4,LR}
        MOVS     R4,#+0
        BL       RCC_HSI_Configuration
        BL       NVIC_Configuration
        BL       UART1_NVIC_Configuration
        BL       UART1_GPIO_Configuration
        BL       UART1_Configuration
        MOVS     R1,#+57
        LDR.N    R0,??DataTable7  ;; 0x40013800
        BL       USART_SendData
        MOVS     R1,#+44
        LDR.N    R0,??DataTable7  ;; 0x40013800
        BL       USART_SendData
??main_0:
        MOVS     R1,#+128
        LDR.N    R0,??DataTable7  ;; 0x40013800
        BL       USART_GetFlagStatus
        CMP      R0,#+0
        BEQ.N    ??main_0
        MOVS     R1,#+10
        LDR.N    R0,??DataTable7  ;; 0x40013800
        BL       USART_SendData
??main_1:
        MOVS     R1,#+128
        LDR.N    R0,??DataTable7  ;; 0x40013800
        BL       USART_GetFlagStatus
        CMP      R0,#+0
        BEQ.N    ??main_1
        MOVS     R1,#+28
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        LDR.N    R0,??main_2      ;; `?<Constant "this OK !__This first...">`
        BL       Uart1_PutString
        LDR.N    R0,??main_2+0x4  ;; `?<Constant "\\n The printf is OK?">`
        BL       printf
        LDR.N    R2,??main_2+0x8  ;; 0x3b645a1d
        LDR.N    R3,??main_2+0xC  ;; 0x405c07df
        LDR.N    R0,??main_2+0x10  ;; `?<Constant "\\n The number is %f">`
        BL       printf
        LDR.N    R0,??main_2+0x14  ;; `?<Constant "SPI1_Init starting...\\r\\n">`
        BL       printf
        BL       SPI1_Init
        LDR.N    R0,??main_2+0x18  ;; `?<Constant "enc28j60 init...\\r\\n">`
        BL       printf
        MOVS     R1,#+32
        LDR.N    R0,??main_2+0x1C  ;; 0x40011000
        BL       GPIO_ResetBits
        MOVS     R1,#+32
        LDR.N    R0,??main_2+0x1C  ;; 0x40011000
        BL       GPIO_SetBits
        BL       simple_server
        LDR.N    R0,??main_2+0x20  ;; enc28j60_MAC
        BL       enc28j60Init
        BL       enc28j60getrev
        MOVS     R4,R0
        MOVS     R0,R4
        POP      {R4,PC}          ;; return
        Nop      
        DATA
??main_2:
        DC32     `?<Constant "this OK !__This first...">`
        DC32     `?<Constant "\\n The printf is OK?">`
        DC32     0x3b645a1d
        DC32     0x405c07df
        DC32     `?<Constant "\\n The number is %f">`
        DC32     `?<Constant "SPI1_Init starting...\\r\\n">`
        DC32     `?<Constant "enc28j60 init...\\r\\n">`
        DC32     0x40011000
        DC32     enc28j60_MAC

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
RCC_HSI_Configuration:
        PUSH     {R7,LR}
        BL       RCC_DeInit
        MOVS     R0,#+16
        BL       FLASH_PrefetchBufferCmd
        MOVS     R0,#+2
        BL       FLASH_SetLatency
        MOVS     R0,#+0
        BL       RCC_HCLKConfig
        MOV      R0,#+1024
        BL       RCC_PCLK2Config
        MOV      R0,#+1024
        BL       RCC_PCLK1Config
        MOVS     R1,#+3670016
        MOVS     R0,#+0
        BL       RCC_PLLConfig
        MOVS     R0,#+1
        BL       RCC_PLLCmd
??RCC_HSI_Configuration_0:
        MOVS     R0,#+57
        BL       RCC_GetFlagStatus
        CMP      R0,#+0
        BEQ.N    ??RCC_HSI_Configuration_0
        MOVS     R0,#+2
        BL       RCC_SYSCLKConfig
??RCC_HSI_Configuration_1:
        BL       RCC_GetSYSCLKSource
        CMP      R0,#+8
        BNE.N    ??RCC_HSI_Configuration_1
        MOVS     R1,#+1
        MOVS     R0,#+5
        BL       RCC_APB2PeriphClockCmd
        MOVS     R1,#+1
        MOV      R0,#+16384
        BL       RCC_APB2PeriphClockCmd
        POP      {R0,PC}          ;; return

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
NVIC_Configuration:
        PUSH     {R7,LR}
        MOVS     R1,#+0
        MOVS     R0,#+134217728
        BL       NVIC_SetVectorTable
        POP      {R0,PC}          ;; return

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
fputc:
        PUSH     {R4,LR}
        MOVS     R4,R0
        MOVS     R1,R4
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        UXTH     R1,R1            ;; ZeroExt  R1,R1,#+16,#+16
        LDR.N    R0,??DataTable7  ;; 0x40013800
        BL       USART_SendData
??fputc_0:
        MOVS     R1,#+128
        LDR.N    R0,??DataTable7  ;; 0x40013800
        BL       USART_GetFlagStatus
        CMP      R0,#+0
        BEQ.N    ??fputc_0
        MOVS     R0,R4
        POP      {R4,PC}          ;; return

        SECTION `.text`:CODE:NOROOT(2)
        DATA
??DataTable7:
        DC32     0x40013800

        END
// 
// 124 bytes in section .data
// 124 bytes in section .rodata
// 388 bytes in section .text
// 
// 388 bytes of CODE  memory
// 124 bytes of CONST memory
// 124 bytes of DATA  memory
//
//Errors: none
//Warnings: none
