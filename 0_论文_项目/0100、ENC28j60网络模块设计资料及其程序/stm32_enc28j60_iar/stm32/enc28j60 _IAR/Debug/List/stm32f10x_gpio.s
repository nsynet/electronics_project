///////////////////////////////////////////////////////////////////////////////
//                                                                            /
//                                                      21/Apr/2009  21:08:10 /
// IAR ANSI C/C++ Compiler V5.20.2.21007/W32 EVALUATION for ARM               /
// Copyright 1999-2008 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode     =  thumb                                                   /
//    Endian       =  little                                                  /
//    Source file  =  F:\b百利胜\enc28j60\enc28j60                            /
//                    _IAR\EWARM_V2.0.2\library\src\stm32f10x_gpio.c          /
//    Command line =  "F:\b百利胜\enc28j60\enc28j60                           /
//                    _IAR\EWARM_V2.0.2\library\src\stm32f10x_gpio.c" -lcN    /
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
//                    _IAR\Debug\List\stm32f10x_gpio.s                        /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME stm32f10x_gpio

        EXTERN RCC_APB2PeriphResetCmd

        PUBLIC GPIO_AFIODeInit
        PUBLIC GPIO_DeInit
        PUBLIC GPIO_EXTILineConfig
        PUBLIC GPIO_EventOutputCmd
        PUBLIC GPIO_EventOutputConfig
        PUBLIC GPIO_Init
        PUBLIC GPIO_PinLockConfig
        PUBLIC GPIO_PinRemapConfig
        PUBLIC GPIO_ReadInputData
        PUBLIC GPIO_ReadInputDataBit
        PUBLIC GPIO_ReadOutputData
        PUBLIC GPIO_ReadOutputDataBit
        PUBLIC GPIO_ResetBits
        PUBLIC GPIO_SetBits
        PUBLIC GPIO_StructInit
        PUBLIC GPIO_Write
        PUBLIC GPIO_WriteBit


        SECTION `.text`:CODE:NOROOT(2)
        THUMB
GPIO_DeInit:
        PUSH     {R7,LR}
        LDR.N    R1,??GPIO_DeInit_0  ;; 0x40010800
        SUBS     R0,R0,R1
        BEQ.N    ??GPIO_DeInit_1
        MOV      R1,#+1024
        SUBS     R0,R0,R1
        BEQ.N    ??GPIO_DeInit_2
        MOV      R1,#+1024
        SUBS     R0,R0,R1
        BEQ.N    ??GPIO_DeInit_3
        MOV      R1,#+1024
        SUBS     R0,R0,R1
        BEQ.N    ??GPIO_DeInit_4
        MOV      R1,#+1024
        SUBS     R0,R0,R1
        BEQ.N    ??GPIO_DeInit_5
        MOV      R1,#+1024
        SUBS     R0,R0,R1
        BEQ.N    ??GPIO_DeInit_6
        MOV      R1,#+1024
        SUBS     R0,R0,R1
        BEQ.N    ??GPIO_DeInit_7
        B.N      ??GPIO_DeInit_8
??GPIO_DeInit_1:
        MOVS     R1,#+1
        MOVS     R0,#+4
        BL       RCC_APB2PeriphResetCmd
        MOVS     R1,#+0
        MOVS     R0,#+4
        BL       RCC_APB2PeriphResetCmd
        B.N      ??GPIO_DeInit_8
??GPIO_DeInit_2:
        MOVS     R1,#+1
        MOVS     R0,#+8
        BL       RCC_APB2PeriphResetCmd
        MOVS     R1,#+0
        MOVS     R0,#+8
        BL       RCC_APB2PeriphResetCmd
        B.N      ??GPIO_DeInit_8
??GPIO_DeInit_3:
        MOVS     R1,#+1
        MOVS     R0,#+16
        BL       RCC_APB2PeriphResetCmd
        MOVS     R1,#+0
        MOVS     R0,#+16
        BL       RCC_APB2PeriphResetCmd
        B.N      ??GPIO_DeInit_8
??GPIO_DeInit_4:
        MOVS     R1,#+1
        MOVS     R0,#+32
        BL       RCC_APB2PeriphResetCmd
        MOVS     R1,#+0
        MOVS     R0,#+32
        BL       RCC_APB2PeriphResetCmd
        B.N      ??GPIO_DeInit_8
??GPIO_DeInit_5:
        MOVS     R1,#+1
        MOVS     R0,#+64
        BL       RCC_APB2PeriphResetCmd
        MOVS     R1,#+0
        MOVS     R0,#+64
        BL       RCC_APB2PeriphResetCmd
        B.N      ??GPIO_DeInit_8
??GPIO_DeInit_6:
        MOVS     R1,#+1
        MOVS     R0,#+128
        BL       RCC_APB2PeriphResetCmd
        MOVS     R1,#+0
        MOVS     R0,#+128
        BL       RCC_APB2PeriphResetCmd
        B.N      ??GPIO_DeInit_8
??GPIO_DeInit_7:
        MOVS     R1,#+1
        MOV      R0,#+256
        BL       RCC_APB2PeriphResetCmd
        MOVS     R1,#+0
        MOV      R0,#+256
        BL       RCC_APB2PeriphResetCmd
??GPIO_DeInit_8:
        POP      {R0,PC}          ;; return
        DATA
??GPIO_DeInit_0:
        DC32     0x40010800

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
GPIO_AFIODeInit:
        PUSH     {R7,LR}
        MOVS     R1,#+1
        MOVS     R0,#+1
        BL       RCC_APB2PeriphResetCmd
        MOVS     R1,#+0
        MOVS     R0,#+1
        BL       RCC_APB2PeriphResetCmd
        POP      {R0,PC}          ;; return

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
GPIO_Init:
        PUSH     {R4-R6,LR}
        MOVS     R2,#+0
        MOVS     R4,#+0
        MOVS     R3,#+0
        MOVS     LR,#+0
        MOVS     R12,#+0
        MOVS     R5,#+0
        LDRB     R6,[R1, #+3]
        ANDS     R6,R6,#0xF
        MOVS     R2,R6
        LDRB     R6,[R1, #+3]
        TST      R6,#0x10
        BEQ.N    ??GPIO_Init_0
        LDRB     R3,[R1, #+2]
        ORRS     R2,R3,R2
??GPIO_Init_0:
        LDRH     R3,[R1, #+0]
        LSLS     R3,R3,#+24
        BEQ.N    ??GPIO_Init_1
        LDR      R12,[R0, #+0]
        MOVS     R3,#+0
        B.N      ??GPIO_Init_2
??GPIO_Init_3:
        LDRB     LR,[R1, #+3]
        CMP      LR,#+72
        BNE.N    ??GPIO_Init_4
        MOVS     LR,#+1
        LSLS     LR,LR,R3
        STR      LR,[R0, #+16]
??GPIO_Init_4:
        ADDS     R3,R3,#+1
??GPIO_Init_2:
        CMP      R3,#+8
        BCS.N    ??GPIO_Init_5
        MOVS     LR,#+1
        LSLS     LR,LR,R3
        LDRH     R4,[R1, #+0]
        ANDS     R4,LR,R4
        CMP      R4,LR
        BNE.N    ??GPIO_Init_4
        LSLS     LR,R3,#+2
        MOVS     R5,#+15
        LSLS     R5,R5,LR
        MVNS     R4,R5
        ANDS     R12,R4,R12
        LSLS     LR,R2,LR
        ORRS     R12,LR,R12
        LDRB     LR,[R1, #+3]
        CMP      LR,#+40
        BNE.N    ??GPIO_Init_3
        MOVS     LR,#+1
        LSLS     LR,LR,R3
        STR      LR,[R0, #+20]
        B.N      ??GPIO_Init_4
??GPIO_Init_5:
        STR      R12,[R0, #+0]
??GPIO_Init_1:
        LDRH     R3,[R1, #+0]
        MOV      R12,#+256
        CMP      R3,R12
        BCC.N    ??GPIO_Init_6
        LDR      R12,[R0, #+4]
        MOVS     R3,#+0
        B.N      ??GPIO_Init_7
??GPIO_Init_8:
        MOVS     LR,#+1
        MOVS     R4,R3
        ADDS     R4,R4,#+8
        LSLS     LR,LR,R4
        LDRH     R4,[R1, #+0]
        ANDS     R4,LR,R4
        CMP      R4,LR
        BNE.N    ??GPIO_Init_9
        LSLS     LR,R3,#+2
        MOVS     R5,#+15
        LSLS     R5,R5,LR
        MVNS     R4,R5
        ANDS     R12,R4,R12
        LSLS     LR,R2,LR
        ORRS     R12,LR,R12
        LDRB     LR,[R1, #+3]
        CMP      LR,#+40
        BNE.N    ??GPIO_Init_10
        MOVS     LR,#+1
        MOVS     R4,R3
        ADDS     R4,R4,#+8
        LSLS     LR,LR,R4
        STR      LR,[R0, #+20]
??GPIO_Init_10:
        LDRB     LR,[R1, #+3]
        CMP      LR,#+72
        BNE.N    ??GPIO_Init_9
        MOVS     LR,#+1
        MOVS     R4,R3
        ADDS     R4,R4,#+8
        LSLS     LR,LR,R4
        STR      LR,[R0, #+16]
??GPIO_Init_9:
        ADDS     R3,R3,#+1
??GPIO_Init_7:
        CMP      R3,#+8
        BCC.N    ??GPIO_Init_8
        STR      R12,[R0, #+4]
??GPIO_Init_6:
        POP      {R4-R6,PC}       ;; return

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
GPIO_StructInit:
        MOVW     R1,#+65535
        STRH     R1,[R0, #+0]
        MOVS     R1,#+2
        STRB     R1,[R0, #+2]
        MOVS     R1,#+4
        STRB     R1,[R0, #+3]
        BX       LR               ;; return

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
GPIO_ReadInputDataBit:
        MOVS     R2,R0
        MOVS     R0,#+0
        LDR      R2,[R2, #+8]
        ANDS     R1,R1,R2
        MOVS     R2,R1
        UXTH     R2,R2            ;; ZeroExt  R2,R2,#+16,#+16
        SUBS     R2,R2,#+1
        SBCS     R2,R2,R2
        MVNS     R2,R2
        LSRS     R2,R2,#+31
        MOVS     R0,R2
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        BX       LR               ;; return

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
GPIO_ReadInputData:
        LDR      R0,[R0, #+8]
        UXTH     R0,R0            ;; ZeroExt  R0,R0,#+16,#+16
        BX       LR               ;; return

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
GPIO_ReadOutputDataBit:
        MOVS     R2,R0
        MOVS     R0,#+0
        LDR      R2,[R2, #+12]
        ANDS     R1,R1,R2
        MOVS     R2,R1
        UXTH     R2,R2            ;; ZeroExt  R2,R2,#+16,#+16
        SUBS     R2,R2,#+1
        SBCS     R2,R2,R2
        MVNS     R2,R2
        LSRS     R2,R2,#+31
        MOVS     R0,R2
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        BX       LR               ;; return

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
GPIO_ReadOutputData:
        LDR      R0,[R0, #+12]
        UXTH     R0,R0            ;; ZeroExt  R0,R0,#+16,#+16
        BX       LR               ;; return

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
GPIO_SetBits:
        UXTH     R1,R1            ;; ZeroExt  R1,R1,#+16,#+16
        STR      R1,[R0, #+16]
        BX       LR               ;; return

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
GPIO_ResetBits:
        UXTH     R1,R1            ;; ZeroExt  R1,R1,#+16,#+16
        STR      R1,[R0, #+20]
        BX       LR               ;; return

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
GPIO_WriteBit:
        UXTB     R2,R2            ;; ZeroExt  R2,R2,#+24,#+24
        CMP      R2,#+0
        BEQ.N    ??GPIO_WriteBit_0
        UXTH     R1,R1            ;; ZeroExt  R1,R1,#+16,#+16
        STR      R1,[R0, #+16]
        B.N      ??GPIO_WriteBit_1
??GPIO_WriteBit_0:
        UXTH     R1,R1            ;; ZeroExt  R1,R1,#+16,#+16
        STR      R1,[R0, #+20]
??GPIO_WriteBit_1:
        BX       LR               ;; return

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
GPIO_Write:
        UXTH     R1,R1            ;; ZeroExt  R1,R1,#+16,#+16
        STR      R1,[R0, #+12]
        BX       LR               ;; return

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
GPIO_PinLockConfig:
        MOVS     R2,#+65536
        UXTH     R1,R1            ;; ZeroExt  R1,R1,#+16,#+16
        ORRS     R2,R1,R2
        STR      R2,[R0, #+24]
        UXTH     R1,R1            ;; ZeroExt  R1,R1,#+16,#+16
        STR      R1,[R0, #+24]
        STR      R2,[R0, #+24]
        LDR      R2,[R0, #+24]
        LDR      R0,[R0, #+24]
        MOVS     R2,R0
        BX       LR               ;; return

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
GPIO_EventOutputConfig:
        MOVS     R2,#+0
        LDR.N    R3,??GPIO_EventOutputConfig_0  ;; 0x40010000
        LDR      R3,[R3, #+0]
        MOVS     R2,R3
        MOVW     R3,#+65408
        ANDS     R2,R3,R2
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        ORRS     R2,R2,R0, LSL #+4
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        ORRS     R2,R1,R2
        LDR.N    R0,??GPIO_EventOutputConfig_0  ;; 0x40010000
        STR      R2,[R0, #+0]
        BX       LR               ;; return
        Nop      
        DATA
??GPIO_EventOutputConfig_0:
        DC32     0x40010000

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
GPIO_EventOutputCmd:
        LDR.N    R1,??GPIO_EventOutputCmd_0  ;; 0x4220001c
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        STR      R0,[R1, #+0]
        BX       LR               ;; return
        DATA
??GPIO_EventOutputCmd_0:
        DC32     0x4220001c

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
GPIO_PinRemapConfig:
        PUSH     {R4,LR}
        MOVS     R2,#+0
        MOVS     R12,#+0
        MOVS     R3,#+0
        MOVS     LR,#+0
        LDR.N    R4,??GPIO_PinRemapConfig_0  ;; 0x40010004
        LDR      R4,[R4, #+0]
        MOVS     R3,R4
        ANDS     R4,R0,#0xF0000
        LSRS     R4,R4,#+16
        MOV      LR,R4
        UXTH     R4,R0            ;; ZeroExt  R4,R0,#+16,#+16
        MOVS     R2,R4
        ANDS     R4,R0,#0x300000
        CMP      R4,#+3145728
        BNE.N    ??GPIO_PinRemapConfig_1
        MVNS     R12,#+251658240
        ANDS     R3,R12,R3
        LDR.W    R12,??GPIO_PinRemapConfig_0  ;; 0x40010004
        LDR      R12,[R12, #+0]
        MVNS     LR,#+251658240
        ANDS     R12,LR,R12
        LDR.W    LR,??GPIO_PinRemapConfig_0  ;; 0x40010004
        STR      R12,[LR, #+0]
        B.N      ??GPIO_PinRemapConfig_2
??GPIO_PinRemapConfig_1:
        TST      R0,#0x100000
        BEQ.N    ??GPIO_PinRemapConfig_3
        MOVS     R12,#+3
        LSLS     R12,R12,LR
        MVNS     R12,R12
        ANDS     R3,R12,R3
        ORRS     R3,R3,#0xF000000
        B.N      ??GPIO_PinRemapConfig_2
??GPIO_PinRemapConfig_3:
        LSRS     R12,R0,#+21
        UXTB     R12,R12          ;; ZeroExt  R12,R12,#+24,#+24
        LSLS     R12,R12,#+4
        LSLS     R12,R2,R12
        MVNS     R12,R12
        ANDS     R3,R12,R3
        ORRS     R3,R3,#0xF000000
??GPIO_PinRemapConfig_2:
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        CMP      R1,#+0
        BEQ.N    ??GPIO_PinRemapConfig_4
        LSRS     R0,R0,#+21
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        LSLS     R0,R0,#+4
        LSLS     R0,R2,R0
        ORRS     R3,R0,R3
??GPIO_PinRemapConfig_4:
        LDR.N    R0,??GPIO_PinRemapConfig_0  ;; 0x40010004
        STR      R3,[R0, #+0]
        POP      {R4,PC}          ;; return
        DATA
??GPIO_PinRemapConfig_0:
        DC32     0x40010004

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
GPIO_EXTILineConfig:
        MOVS     R2,#+0
        MOVS     R3,#+15
        ANDS     R12,R1,#0x3
        UXTB     R12,R12          ;; ZeroExt  R12,R12,#+24,#+24
        LSLS     R12,R12,#+2
        LSLS     R3,R3,R12
        MOVS     R2,R3
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        ASRS     R3,R1,#+2
        LDR.W    R12,??GPIO_EXTILineConfig_0  ;; 0x40010008
        LDR      R3,[R12, R3, LSL #+2]
        MVNS     R2,R2
        ANDS     R2,R2,R3
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        ASRS     R3,R1,#+2
        LDR.W    R12,??GPIO_EXTILineConfig_0  ;; 0x40010008
        STR      R2,[R12, R3, LSL #+2]
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        ASRS     R2,R1,#+2
        LDR.N    R3,??GPIO_EXTILineConfig_0  ;; 0x40010008
        LDR      R2,[R3, R2, LSL #+2]
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        ANDS     R3,R1,#0x3
        UXTB     R3,R3            ;; ZeroExt  R3,R3,#+24,#+24
        LSLS     R3,R3,#+2
        LSLS     R0,R0,R3
        ORRS     R0,R0,R2
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        ASRS     R1,R1,#+2
        LDR.N    R2,??GPIO_EXTILineConfig_0  ;; 0x40010008
        STR      R0,[R2, R1, LSL #+2]
        BX       LR               ;; return
        Nop      
        DATA
??GPIO_EXTILineConfig_0:
        DC32     0x40010008

        END
// 
// 930 bytes in section .text
// 
// 930 bytes of CODE memory
//
//Errors: none
//Warnings: none
