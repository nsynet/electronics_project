///////////////////////////////////////////////////////////////////////////////
//                                                                            /
//                                                      21/Apr/2009  21:08:12 /
// IAR ANSI C/C++ Compiler V5.20.2.21007/W32 EVALUATION for ARM               /
// Copyright 1999-2008 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode     =  thumb                                                   /
//    Endian       =  little                                                  /
//    Source file  =  F:\b百利胜\enc28j60\enc28j60 _IAR\user_uart.c           /
//    Command line =  "F:\b百利胜\enc28j60\enc28j60 _IAR\user_uart.c" -lcN    /
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
//    List file    =  F:\b百利胜\enc28j60\enc28j60                            /
//                    _IAR\Debug\List\user_uart.s                             /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME user_uart

        EXTERN GPIO_Init
        EXTERN NVIC_Init
        EXTERN NVIC_PriorityGroupConfig
        EXTERN USART_Cmd
        EXTERN USART_GetFlagStatus
        EXTERN USART_ITConfig
        EXTERN USART_Init
        EXTERN USART_SendData

        PUBLIC UART1_Configuration
        PUBLIC UART1_GPIO_Configuration
        PUBLIC UART1_NVIC_Configuration
        PUBLIC Uart1_PutChar
        PUBLIC Uart1_PutString


        SECTION `.text`:CODE:NOROOT(2)
        THUMB
UART1_NVIC_Configuration:
        PUSH     {R7,LR}
        MOV      R0,#+1792
        BL       NVIC_PriorityGroupConfig
        MOVS     R0,#+37
        STRB     R0,[SP, #+0]
        MOVS     R0,#+0
        STRB     R0,[SP, #+2]
        MOVS     R0,#+1
        STRB     R0,[SP, #+3]
        ADD      R0,SP,#+0
        BL       NVIC_Init
        POP      {R0,PC}          ;; return

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
UART1_Configuration:
        PUSH     {LR}
        SUB      SP,SP,#+20
        MOV      R0,#+9600
        STR      R0,[SP, #+0]
        MOVS     R0,#+0
        STRH     R0,[SP, #+4]
        MOVS     R0,#+0
        STRH     R0,[SP, #+6]
        MOVS     R0,#+0
        STRH     R0,[SP, #+8]
        MOVS     R0,#+0
        STRH     R0,[SP, #+12]
        MOVS     R0,#+12
        STRH     R0,[SP, #+10]
        ADD      R1,SP,#+0
        LDR.N    R0,??DataTable4  ;; 0x40013800
        BL       USART_Init
        MOVS     R2,#+1
        MOVW     R1,#+1317
        LDR.N    R0,??DataTable4  ;; 0x40013800
        BL       USART_ITConfig
        MOVS     R1,#+1
        LDR.N    R0,??DataTable4  ;; 0x40013800
        BL       USART_Cmd
        ADD      SP,SP,#+20
        POP      {PC}             ;; return

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
UART1_GPIO_Configuration:
        PUSH     {R7,LR}
        MOV      R0,#+512
        STRH     R0,[SP, #+0]
        MOVS     R0,#+3
        STRB     R0,[SP, #+2]
        MOVS     R0,#+24
        STRB     R0,[SP, #+3]
        ADD      R1,SP,#+0
        LDR.N    R0,??UART1_GPIO_Configuration_0  ;; 0x40010800
        BL       GPIO_Init
        MOV      R0,#+1024
        STRH     R0,[SP, #+0]
        MOVS     R0,#+4
        STRB     R0,[SP, #+3]
        ADD      R1,SP,#+0
        LDR.N    R0,??UART1_GPIO_Configuration_0  ;; 0x40010800
        BL       GPIO_Init
        POP      {R0,PC}          ;; return
        Nop      
        DATA
??UART1_GPIO_Configuration_0:
        DC32     0x40010800

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
Uart1_PutChar:
        PUSH     {R4,LR}
        MOVS     R4,R0
        UXTB     R4,R4            ;; ZeroExt  R4,R4,#+24,#+24
        MOVS     R1,R4
        UXTH     R1,R1            ;; ZeroExt  R1,R1,#+16,#+16
        LDR.N    R0,??DataTable4  ;; 0x40013800
        BL       USART_SendData
??Uart1_PutChar_0:
        MOVS     R1,#+128
        LDR.N    R0,??DataTable4  ;; 0x40013800
        BL       USART_GetFlagStatus
        CMP      R0,#+0
        BEQ.N    ??Uart1_PutChar_0
        MOVS     R0,R4
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        POP      {R4,PC}          ;; return

        SECTION `.text`:CODE:NOROOT(2)
        DATA
??DataTable4:
        DC32     0x40013800

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
Uart1_PutString:
        PUSH     {R4-R6,LR}
        MOVS     R5,R0
        MOVS     R4,R1
        MOVS     R6,#+0
        B.N      ??Uart1_PutString_0
??Uart1_PutString_1:
        LDRB     R0,[R5, #+0]
        BL       Uart1_PutChar
        ADDS     R5,R5,#+1
        ADDS     R6,R6,#+1
??Uart1_PutString_0:
        UXTB     R6,R6            ;; ZeroExt  R6,R6,#+24,#+24
        UXTB     R4,R4            ;; ZeroExt  R4,R4,#+24,#+24
        CMP      R6,R4
        BCC.N    ??Uart1_PutString_1
        POP      {R4-R6,PC}       ;; return

        END
// 
// 236 bytes in section .text
// 
// 236 bytes of CODE memory
//
//Errors: none
//Warnings: none
