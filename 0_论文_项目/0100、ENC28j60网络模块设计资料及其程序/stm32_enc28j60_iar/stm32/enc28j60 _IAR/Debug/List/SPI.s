///////////////////////////////////////////////////////////////////////////////
//                                                                            /
//                                                      21/Apr/2009  21:08:09 /
// IAR ANSI C/C++ Compiler V5.20.2.21007/W32 EVALUATION for ARM               /
// Copyright 1999-2008 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode     =  thumb                                                   /
//    Endian       =  little                                                  /
//    Source file  =  F:\b百利胜\enc28j60\enc28j60 _IAR\TCPIP\SPI.C           /
//    Command line =  "F:\b百利胜\enc28j60\enc28j60 _IAR\TCPIP\SPI.C" -lcN    /
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
//    List file    =  F:\b百利胜\enc28j60\enc28j60 _IAR\Debug\List\SPI.s      /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME SPI

        EXTERN GPIO_Init
        EXTERN RCC_APB2PeriphClockCmd
        EXTERN SPI_Cmd
        EXTERN SPI_I2S_GetFlagStatus
        EXTERN SPI_I2S_ReceiveData
        EXTERN SPI_I2S_SendData
        EXTERN SPI_Init

        PUBLIC SPI1_Init
        PUBLIC SPI1_ReadWrite


        SECTION `.text`:CODE:NOROOT(2)
        THUMB
SPI1_Init:
        PUSH     {LR}
        SUB      SP,SP,#+28
        MOVS     R1,#+1
        MOV      R0,#+4096
        BL       RCC_APB2PeriphClockCmd
        MOVS     R0,#+224
        STRH     R0,[SP, #+0]
        MOVS     R0,#+1
        STRB     R0,[SP, #+2]
        MOVS     R0,#+24
        STRB     R0,[SP, #+3]
        ADD      R1,SP,#+0
        LDR.N    R0,??SPI1_Init_0  ;; 0x40010800
        BL       GPIO_Init
        MOVS     R0,#+16
        STRH     R0,[SP, #+0]
        MOVS     R0,#+16
        STRB     R0,[SP, #+3]
        MOVS     R0,#+3
        STRB     R0,[SP, #+2]
        ADD      R1,SP,#+0
        LDR.N    R0,??SPI1_Init_0  ;; 0x40010800
        BL       GPIO_Init
        MOVS     R0,#+48
        STRH     R0,[SP, #+0]
        MOVS     R0,#+16
        STRB     R0,[SP, #+3]
        MOVS     R0,#+3
        STRB     R0,[SP, #+2]
        ADD      R1,SP,#+0
        LDR.N    R0,??SPI1_Init_0+0x4  ;; 0x40011000
        BL       GPIO_Init
        MOVS     R0,#+0
        STRH     R0,[SP, #+4]
        MOV      R0,#+260
        STRH     R0,[SP, #+6]
        MOVS     R0,#+0
        STRH     R0,[SP, #+8]
        MOVS     R0,#+0
        STRH     R0,[SP, #+10]
        MOVS     R0,#+0
        STRH     R0,[SP, #+12]
        MOV      R0,#+512
        STRH     R0,[SP, #+14]
        MOVS     R0,#+8
        STRH     R0,[SP, #+16]
        MOVS     R0,#+0
        STRH     R0,[SP, #+18]
        MOVS     R0,#+7
        STRH     R0,[SP, #+20]
        ADD      R1,SP,#+4
        LDR.N    R0,??DataTable5  ;; 0x40013000
        BL       SPI_Init
        MOVS     R1,#+1
        LDR.N    R0,??DataTable5  ;; 0x40013000
        BL       SPI_Cmd
        ADD      SP,SP,#+28
        POP      {PC}             ;; return
        Nop      
        DATA
??SPI1_Init_0:
        DC32     0x40010800
        DC32     0x40011000

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
SPI1_ReadWrite:
        PUSH     {R4,LR}
        MOVS     R4,R0
??SPI1_ReadWrite_0:
        MOVS     R1,#+2
        LDR.N    R0,??DataTable5  ;; 0x40013000
        BL       SPI_I2S_GetFlagStatus
        CMP      R0,#+0
        BEQ.N    ??SPI1_ReadWrite_0
        UXTB     R4,R4            ;; ZeroExt  R4,R4,#+24,#+24
        MOVS     R1,R4
        UXTH     R1,R1            ;; ZeroExt  R1,R1,#+16,#+16
        LDR.N    R0,??DataTable5  ;; 0x40013000
        BL       SPI_I2S_SendData
??SPI1_ReadWrite_1:
        MOVS     R1,#+1
        LDR.N    R0,??DataTable5  ;; 0x40013000
        BL       SPI_I2S_GetFlagStatus
        CMP      R0,#+0
        BEQ.N    ??SPI1_ReadWrite_1
        LDR.N    R0,??DataTable5  ;; 0x40013000
        BL       SPI_I2S_ReceiveData
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        POP      {R4,PC}          ;; return

        SECTION `.text`:CODE:NOROOT(2)
        DATA
??DataTable5:
        DC32     0x40013000

        END
// 
// 234 bytes in section .text
// 
// 234 bytes of CODE memory
//
//Errors: none
//Warnings: none
