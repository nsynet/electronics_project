///////////////////////////////////////////////////////////////////////////////
//                                                                            /
//                                                      21/Apr/2009  21:08:11 /
// IAR ANSI C/C++ Compiler V5.20.2.21007/W32 EVALUATION for ARM               /
// Copyright 1999-2008 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode     =  thumb                                                   /
//    Endian       =  little                                                  /
//    Source file  =  F:\b百利胜\enc28j60\enc28j60                            /
//                    _IAR\EWARM_V2.0.2\library\src\stm32f10x_rcc.c           /
//    Command line =  "F:\b百利胜\enc28j60\enc28j60                           /
//                    _IAR\EWARM_V2.0.2\library\src\stm32f10x_rcc.c" -lcN     /
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
//                    _IAR\Debug\List\stm32f10x_rcc.s                         /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME stm32f10x_rcc

        PUBLIC RCC_ADCCLKConfig
        PUBLIC RCC_AHBPeriphClockCmd
        PUBLIC RCC_APB1PeriphClockCmd
        PUBLIC RCC_APB1PeriphResetCmd
        PUBLIC RCC_APB2PeriphClockCmd
        PUBLIC RCC_APB2PeriphResetCmd
        PUBLIC RCC_AdjustHSICalibrationValue
        PUBLIC RCC_BackupResetCmd
        PUBLIC RCC_ClearFlag
        PUBLIC RCC_ClearITPendingBit
        PUBLIC RCC_ClockSecuritySystemCmd
        PUBLIC RCC_DeInit
        PUBLIC RCC_GetClocksFreq
        PUBLIC RCC_GetFlagStatus
        PUBLIC RCC_GetITStatus
        PUBLIC RCC_GetSYSCLKSource
        PUBLIC RCC_HCLKConfig
        PUBLIC RCC_HSEConfig
        PUBLIC RCC_HSICmd
        PUBLIC RCC_ITConfig
        PUBLIC RCC_LSEConfig
        PUBLIC RCC_LSICmd
        PUBLIC RCC_MCOConfig
        PUBLIC RCC_PCLK1Config
        PUBLIC RCC_PCLK2Config
        PUBLIC RCC_PLLCmd
        PUBLIC RCC_PLLConfig
        PUBLIC RCC_RTCCLKCmd
        PUBLIC RCC_RTCCLKConfig
        PUBLIC RCC_SYSCLKConfig
        PUBLIC RCC_USBCLKConfig
        PUBLIC RCC_WaitForHSEStartUp


        SECTION `.rodata`:CONST:NOROOT(2)
APBAHBPrescTable:
        DATA
        DC8 0, 0, 0, 0, 1, 2, 3, 4, 1, 2, 3, 4, 6, 7, 8, 9

        SECTION `.rodata`:CONST:NOROOT(2)
ADCPrescTable:
        DATA
        DC8 2, 4, 6, 8

        SECTION `.bss`:DATA:NOROOT(0)
HSEStatus:
        DS8 1

        SECTION `.bss`:DATA:NOROOT(2)
StartUpCounter:
        DS8 4

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
RCC_DeInit:
        LDR.W    R0,??DataTable48  ;; 0x40021000
        LDR      R0,[R0, #+0]
        ORRS     R0,R0,#0x1
        LDR.W    R1,??DataTable48  ;; 0x40021000
        STR      R0,[R1, #+0]
        LDR.N    R0,??DataTable47  ;; 0x40021004
        LDR      R0,[R0, #+0]
        LDR.N    R1,??RCC_DeInit_0  ;; 0xf8ff0000
        ANDS     R0,R1,R0
        LDR.N    R1,??DataTable47  ;; 0x40021004
        STR      R0,[R1, #+0]
        LDR.W    R0,??DataTable48  ;; 0x40021000
        LDR      R0,[R0, #+0]
        LDR.N    R1,??RCC_DeInit_0+0x4  ;; 0xfef6ffff
        ANDS     R0,R1,R0
        LDR.W    R1,??DataTable48  ;; 0x40021000
        STR      R0,[R1, #+0]
        LDR.W    R0,??DataTable48  ;; 0x40021000
        LDR      R0,[R0, #+0]
        MVNS     R1,#+262144
        ANDS     R0,R1,R0
        LDR.W    R1,??DataTable48  ;; 0x40021000
        STR      R0,[R1, #+0]
        LDR.N    R0,??DataTable47  ;; 0x40021004
        LDR      R0,[R0, #+0]
        MVNS     R1,#+8323072
        ANDS     R0,R1,R0
        LDR.N    R1,??DataTable47  ;; 0x40021004
        STR      R0,[R1, #+0]
        LDR.W    R0,??DataTable53  ;; 0x40021008
        MOVS     R1,#+0
        STR      R1,[R0, #+0]
        BX       LR               ;; return
        Nop      
        DATA
??RCC_DeInit_0:
        DC32     0xf8ff0000
        DC32     0xfef6ffff

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
RCC_HSEConfig:
        PUSH     {R7,LR}
        LDR.W    R1,??DataTable48  ;; 0x40021000
        LDR      R1,[R1, #+0]
        MVNS     R2,#+65536
        ANDS     R1,R2,R1
        LDR.W    R2,??DataTable48  ;; 0x40021000
        STR      R1,[R2, #+0]
        LDR.W    R1,??DataTable48  ;; 0x40021000
        LDR      R1,[R1, #+0]
        MVNS     R2,#+262144
        ANDS     R1,R2,R1
        LDR.W    R2,??DataTable48  ;; 0x40021000
        STR      R1,[R2, #+0]
        MOVS     R1,#+65536
        SUBS     R0,R0,R1
        BEQ.N    ??RCC_HSEConfig_0
        MOVS     R1,#+196608
        SUBS     R0,R0,R1
        BEQ.N    ??RCC_HSEConfig_1
        B.N      ??RCC_HSEConfig_2
??RCC_HSEConfig_0:
        LDR.W    R0,??DataTable48  ;; 0x40021000
        LDR      R0,[R0, #+0]
        ORRS     R0,R0,#0x10000
        LDR.W    R1,??DataTable48  ;; 0x40021000
        STR      R0,[R1, #+0]
        B.N      ??RCC_HSEConfig_2
??RCC_HSEConfig_1:
        LDR.W    R0,??DataTable48  ;; 0x40021000
        LDR      R0,[R0, #+0]
        ORRS     R0,R0,#0x50000
        LDR.W    R1,??DataTable48  ;; 0x40021000
        STR      R0,[R1, #+0]
??RCC_HSEConfig_2:
        POP      {R0,PC}          ;; return

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
RCC_WaitForHSEStartUp:
        PUSH     {R7,LR}
        MOVS     R0,#+0
??RCC_WaitForHSEStartUp_0:
        MOVS     R0,#+49
        BL       RCC_GetFlagStatus
        LDR.N    R1,??RCC_WaitForHSEStartUp_1  ;; HSEStatus
        STRB     R0,[R1, #+0]
        LDR.N    R0,??RCC_WaitForHSEStartUp_1+0x4  ;; StartUpCounter
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,#+1
        LDR.N    R1,??RCC_WaitForHSEStartUp_1+0x4  ;; StartUpCounter
        STR      R0,[R1, #+0]
        LDR.N    R0,??RCC_WaitForHSEStartUp_1  ;; HSEStatus
        LDRB     R0,[R0, #+0]
        CMP      R0,#+0
        BNE.N    ??RCC_WaitForHSEStartUp_2
        LDR.N    R0,??RCC_WaitForHSEStartUp_1+0x4  ;; StartUpCounter
        LDR      R0,[R0, #+0]
        MOVW     R1,#+511
        CMP      R0,R1
        BNE.N    ??RCC_WaitForHSEStartUp_0
??RCC_WaitForHSEStartUp_2:
        MOVS     R0,#+49
        BL       RCC_GetFlagStatus
        SUBS     R0,R0,#+1
        SBCS     R0,R0,R0
        MVNS     R0,R0
        LSRS     R0,R0,#+31
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        POP      {R1,PC}          ;; return
        Nop      
        DATA
??RCC_WaitForHSEStartUp_1:
        DC32     HSEStatus
        DC32     StartUpCounter

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
RCC_AdjustHSICalibrationValue:
        MOVS     R1,#+0
        LDR.W    R2,??DataTable48  ;; 0x40021000
        LDR      R2,[R2, #+0]
        MOVS     R1,R2
        MOVS     R2,#+248
        BICS     R1,R1,R2
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        ORRS     R1,R1,R0, LSL #+3
        LDR.W    R0,??DataTable48  ;; 0x40021000
        STR      R1,[R0, #+0]
        BX       LR               ;; return

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
RCC_HSICmd:
        LDR.N    R1,??RCC_HSICmd_0  ;; 0x42420000
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        STR      R0,[R1, #+0]
        BX       LR               ;; return
        DATA
??RCC_HSICmd_0:
        DC32     0x42420000

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
RCC_PLLConfig:
        MOVS     R2,#+0
        LDR.N    R3,??DataTable47  ;; 0x40021004
        LDR      R3,[R3, #+0]
        MOVS     R2,R3
        MVNS     R3,#+4128768
        ANDS     R2,R3,R2
        ORRS     R0,R1,R0
        ORRS     R2,R0,R2
        LDR.N    R0,??DataTable47  ;; 0x40021004
        STR      R2,[R0, #+0]
        BX       LR               ;; return

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
RCC_PLLCmd:
        LDR.N    R1,??RCC_PLLCmd_0  ;; 0x42420060
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        STR      R0,[R1, #+0]
        BX       LR               ;; return
        DATA
??RCC_PLLCmd_0:
        DC32     0x42420060

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
RCC_SYSCLKConfig:
        MOVS     R1,#+0
        LDR.N    R2,??DataTable47  ;; 0x40021004
        LDR      R2,[R2, #+0]
        MOVS     R1,R2
        MOVS     R2,#+3
        BICS     R1,R1,R2
        ORRS     R1,R0,R1
        LDR.N    R0,??DataTable47  ;; 0x40021004
        STR      R1,[R0, #+0]
        BX       LR               ;; return

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
RCC_GetSYSCLKSource:
        LDR.N    R0,??DataTable47  ;; 0x40021004
        LDR      R0,[R0, #+0]
        ANDS     R0,R0,#0xC
        BX       LR               ;; return

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
RCC_HCLKConfig:
        MOVS     R1,#+0
        LDR.N    R2,??DataTable47  ;; 0x40021004
        LDR      R2,[R2, #+0]
        MOVS     R1,R2
        MOVS     R2,#+240
        BICS     R1,R1,R2
        ORRS     R1,R0,R1
        LDR.N    R0,??DataTable47  ;; 0x40021004
        STR      R1,[R0, #+0]
        BX       LR               ;; return

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
RCC_PCLK1Config:
        MOVS     R1,#+0
        LDR.N    R2,??DataTable47  ;; 0x40021004
        LDR      R2,[R2, #+0]
        MOVS     R1,R2
        MVNS     R2,#+1792
        ANDS     R1,R2,R1
        ORRS     R1,R0,R1
        LDR.N    R0,??DataTable47  ;; 0x40021004
        STR      R1,[R0, #+0]
        BX       LR               ;; return

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
RCC_PCLK2Config:
        MOVS     R1,#+0
        LDR.N    R2,??DataTable47  ;; 0x40021004
        LDR      R2,[R2, #+0]
        MOVS     R1,R2
        MVNS     R2,#+14336
        ANDS     R1,R2,R1
        ORRS     R1,R1,R0, LSL #+3
        LDR.N    R0,??DataTable47  ;; 0x40021004
        STR      R1,[R0, #+0]
        BX       LR               ;; return

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
RCC_ITConfig:
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        CMP      R1,#+0
        BEQ.N    ??RCC_ITConfig_0
        LDR.N    R1,??RCC_ITConfig_1  ;; 0x40021009
        LDRB     R1,[R1, #+0]
        ORRS     R0,R0,R1
        LDR.N    R1,??RCC_ITConfig_1  ;; 0x40021009
        STRB     R0,[R1, #+0]
        B.N      ??RCC_ITConfig_2
??RCC_ITConfig_0:
        LDR.N    R1,??RCC_ITConfig_1  ;; 0x40021009
        LDRB     R1,[R1, #+0]
        MVNS     R0,R0
        ANDS     R0,R0,R1
        LDR.N    R1,??RCC_ITConfig_1  ;; 0x40021009
        STRB     R0,[R1, #+0]
??RCC_ITConfig_2:
        BX       LR               ;; return
        DATA
??RCC_ITConfig_1:
        DC32     0x40021009

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
RCC_USBCLKConfig:
        LDR.N    R1,??RCC_USBCLKConfig_0  ;; 0x424200d8
        STR      R0,[R1, #+0]
        BX       LR               ;; return
        Nop      
        DATA
??RCC_USBCLKConfig_0:
        DC32     0x424200d8

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
RCC_ADCCLKConfig:
        MOVS     R1,#+0
        LDR.N    R2,??DataTable47  ;; 0x40021004
        LDR      R2,[R2, #+0]
        MOVS     R1,R2
        MVNS     R2,#+49152
        ANDS     R1,R2,R1
        ORRS     R1,R0,R1
        LDR.N    R0,??DataTable47  ;; 0x40021004
        STR      R1,[R0, #+0]
        BX       LR               ;; return

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
RCC_LSEConfig:
        PUSH     {R7,LR}
        LDR.N    R1,??DataTable49  ;; 0x40021020
        MOVS     R2,#+0
        STRB     R2,[R1, #+0]
        LDR.N    R1,??DataTable49  ;; 0x40021020
        MOVS     R2,#+0
        STRB     R2,[R1, #+0]
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        CMP      R0,#+1
        BEQ.N    ??RCC_LSEConfig_0
        CMP      R0,#+4
        BEQ.N    ??RCC_LSEConfig_1
        B.N      ??RCC_LSEConfig_2
??RCC_LSEConfig_0:
        LDR.N    R0,??DataTable49  ;; 0x40021020
        MOVS     R1,#+1
        STRB     R1,[R0, #+0]
        B.N      ??RCC_LSEConfig_2
??RCC_LSEConfig_1:
        LDR.N    R0,??DataTable49  ;; 0x40021020
        MOVS     R1,#+5
        STRB     R1,[R0, #+0]
??RCC_LSEConfig_2:
        POP      {R0,PC}          ;; return

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
RCC_LSICmd:
        LDR.N    R1,??RCC_LSICmd_0  ;; 0x42420480
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        STR      R0,[R1, #+0]
        BX       LR               ;; return
        DATA
??RCC_LSICmd_0:
        DC32     0x42420480

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
RCC_RTCCLKConfig:
        LDR.N    R1,??DataTable49  ;; 0x40021020
        LDR      R1,[R1, #+0]
        ORRS     R0,R0,R1
        LDR.N    R1,??DataTable49  ;; 0x40021020
        STR      R0,[R1, #+0]
        BX       LR               ;; return

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
RCC_RTCCLKCmd:
        LDR.N    R1,??RCC_RTCCLKCmd_0  ;; 0x4242043c
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        STR      R0,[R1, #+0]
        BX       LR               ;; return
        DATA
??RCC_RTCCLKCmd_0:
        DC32     0x4242043c

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
RCC_GetClocksFreq:
        PUSH     {R4,LR}
        MOVS     R2,#+0
        MOVS     R1,#+0
        MOVS     R3,#+0
        MOVS     R12,#+0
        LDR.W    LR,??DataTable47  ;; 0x40021004
        LDR      LR,[LR, #+0]
        ANDS     LR,LR,#0xC
        MOV      R2,LR
        CMP      R2,#+0
        BEQ.N    ??RCC_GetClocksFreq_0
        CMP      R2,#+4
        BEQ.N    ??RCC_GetClocksFreq_1
        CMP      R2,#+8
        BEQ.N    ??RCC_GetClocksFreq_2
        B.N      ??RCC_GetClocksFreq_3
??RCC_GetClocksFreq_0:
        LDR.N    R1,??RCC_GetClocksFreq_4  ;; 0x7a1200
        STR      R1,[R0, #+0]
        B.N      ??RCC_GetClocksFreq_5
??RCC_GetClocksFreq_1:
        LDR.N    R1,??RCC_GetClocksFreq_4  ;; 0x7a1200
        STR      R1,[R0, #+0]
        B.N      ??RCC_GetClocksFreq_5
??RCC_GetClocksFreq_2:
        LDR.N    R1,??DataTable47  ;; 0x40021004
        LDR      R1,[R1, #+0]
        ANDS     R1,R1,#0x3C0000
        LSRS     R1,R1,#+18
        ADDS     R1,R1,#+2
        LDR.N    R2,??DataTable47  ;; 0x40021004
        LDR      R2,[R2, #+0]
        ANDS     R3,R2,#0x10000
        CMP      R3,#+0
        BNE.N    ??RCC_GetClocksFreq_6
        LDR.N    R2,??RCC_GetClocksFreq_4+0x4  ;; 0x3d0900
        MUL      R1,R1,R2
        STR      R1,[R0, #+0]
        B.N      ??RCC_GetClocksFreq_5
??RCC_GetClocksFreq_6:
        LDR.N    R2,??DataTable47  ;; 0x40021004
        LDR      R2,[R2, #+0]
        TST      R2,#0x20000
        BEQ.N    ??RCC_GetClocksFreq_7
        LDR.N    R2,??RCC_GetClocksFreq_4+0x4  ;; 0x3d0900
        MUL      R1,R1,R2
        STR      R1,[R0, #+0]
        B.N      ??RCC_GetClocksFreq_5
??RCC_GetClocksFreq_7:
        LDR.N    R2,??RCC_GetClocksFreq_4  ;; 0x7a1200
        MUL      R1,R1,R2
        STR      R1,[R0, #+0]
        B.N      ??RCC_GetClocksFreq_5
??RCC_GetClocksFreq_3:
        LDR.N    R1,??RCC_GetClocksFreq_4  ;; 0x7a1200
        STR      R1,[R0, #+0]
??RCC_GetClocksFreq_5:
        LDR.N    R1,??DataTable47  ;; 0x40021004
        LDR      R1,[R1, #+0]
        ANDS     R2,R1,#0xF0
        LSRS     R2,R2,#+4
        LDR.N    R1,??RCC_GetClocksFreq_4+0x8  ;; APBAHBPrescTable
        LDRB     R12,[R2, R1]
        LDR      R1,[R0, #+0]
        LSRS     R1,R1,R12
        STR      R1,[R0, #+4]
        LDR.N    R1,??DataTable47  ;; 0x40021004
        LDR      R1,[R1, #+0]
        ANDS     R2,R1,#0x700
        LSRS     R2,R2,#+8
        LDR.N    R1,??RCC_GetClocksFreq_4+0x8  ;; APBAHBPrescTable
        LDRB     R12,[R2, R1]
        LDR      R1,[R0, #+4]
        LSRS     R1,R1,R12
        STR      R1,[R0, #+8]
        LDR.N    R1,??DataTable47  ;; 0x40021004
        LDR      R1,[R1, #+0]
        ANDS     R2,R1,#0x3800
        LSRS     R2,R2,#+11
        LDR.N    R1,??RCC_GetClocksFreq_4+0x8  ;; APBAHBPrescTable
        LDRB     R12,[R2, R1]
        LDR      R1,[R0, #+4]
        LSRS     R1,R1,R12
        STR      R1,[R0, #+12]
        LDR.N    R1,??DataTable47  ;; 0x40021004
        LDR      R1,[R1, #+0]
        ANDS     R2,R1,#0xC000
        LSRS     R2,R2,#+14
        LDR.N    R1,??RCC_GetClocksFreq_4+0xC  ;; ADCPrescTable
        LDRB     R12,[R2, R1]
        LDR      R1,[R0, #+12]
        UDIV     R1,R1,R12
        STR      R1,[R0, #+16]
        POP      {R4,PC}          ;; return
        Nop      
        DATA
??RCC_GetClocksFreq_4:
        DC32     0x7a1200
        DC32     0x3d0900
        DC32     APBAHBPrescTable
        DC32     ADCPrescTable

        SECTION `.text`:CODE:NOROOT(2)
        DATA
??DataTable47:
        DC32     0x40021004

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
RCC_AHBPeriphClockCmd:
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        CMP      R1,#+0
        BEQ.N    ??RCC_AHBPeriphClockCmd_0
        LDR.N    R1,??RCC_AHBPeriphClockCmd_1  ;; 0x40021014
        LDR      R1,[R1, #+0]
        ORRS     R0,R0,R1
        LDR.N    R1,??RCC_AHBPeriphClockCmd_1  ;; 0x40021014
        STR      R0,[R1, #+0]
        B.N      ??RCC_AHBPeriphClockCmd_2
??RCC_AHBPeriphClockCmd_0:
        LDR.N    R1,??RCC_AHBPeriphClockCmd_1  ;; 0x40021014
        LDR      R1,[R1, #+0]
        MVNS     R0,R0
        ANDS     R0,R0,R1
        LDR.N    R1,??RCC_AHBPeriphClockCmd_1  ;; 0x40021014
        STR      R0,[R1, #+0]
??RCC_AHBPeriphClockCmd_2:
        BX       LR               ;; return
        DATA
??RCC_AHBPeriphClockCmd_1:
        DC32     0x40021014

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
RCC_APB2PeriphClockCmd:
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        CMP      R1,#+0
        BEQ.N    ??RCC_APB2PeriphClockCmd_0
        LDR.N    R1,??RCC_APB2PeriphClockCmd_1  ;; 0x40021018
        LDR      R1,[R1, #+0]
        ORRS     R0,R0,R1
        LDR.N    R1,??RCC_APB2PeriphClockCmd_1  ;; 0x40021018
        STR      R0,[R1, #+0]
        B.N      ??RCC_APB2PeriphClockCmd_2
??RCC_APB2PeriphClockCmd_0:
        LDR.N    R1,??RCC_APB2PeriphClockCmd_1  ;; 0x40021018
        LDR      R1,[R1, #+0]
        MVNS     R0,R0
        ANDS     R0,R0,R1
        LDR.N    R1,??RCC_APB2PeriphClockCmd_1  ;; 0x40021018
        STR      R0,[R1, #+0]
??RCC_APB2PeriphClockCmd_2:
        BX       LR               ;; return
        DATA
??RCC_APB2PeriphClockCmd_1:
        DC32     0x40021018

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
RCC_APB1PeriphClockCmd:
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        CMP      R1,#+0
        BEQ.N    ??RCC_APB1PeriphClockCmd_0
        LDR.N    R1,??RCC_APB1PeriphClockCmd_1  ;; 0x4002101c
        LDR      R1,[R1, #+0]
        ORRS     R0,R0,R1
        LDR.N    R1,??RCC_APB1PeriphClockCmd_1  ;; 0x4002101c
        STR      R0,[R1, #+0]
        B.N      ??RCC_APB1PeriphClockCmd_2
??RCC_APB1PeriphClockCmd_0:
        LDR.N    R1,??RCC_APB1PeriphClockCmd_1  ;; 0x4002101c
        LDR      R1,[R1, #+0]
        MVNS     R0,R0
        ANDS     R0,R0,R1
        LDR.N    R1,??RCC_APB1PeriphClockCmd_1  ;; 0x4002101c
        STR      R0,[R1, #+0]
??RCC_APB1PeriphClockCmd_2:
        BX       LR               ;; return
        DATA
??RCC_APB1PeriphClockCmd_1:
        DC32     0x4002101c

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
RCC_APB2PeriphResetCmd:
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        CMP      R1,#+0
        BEQ.N    ??RCC_APB2PeriphResetCmd_0
        LDR.N    R1,??RCC_APB2PeriphResetCmd_1  ;; 0x4002100c
        LDR      R1,[R1, #+0]
        ORRS     R0,R0,R1
        LDR.N    R1,??RCC_APB2PeriphResetCmd_1  ;; 0x4002100c
        STR      R0,[R1, #+0]
        B.N      ??RCC_APB2PeriphResetCmd_2
??RCC_APB2PeriphResetCmd_0:
        LDR.N    R1,??RCC_APB2PeriphResetCmd_1  ;; 0x4002100c
        LDR      R1,[R1, #+0]
        MVNS     R0,R0
        ANDS     R0,R0,R1
        LDR.N    R1,??RCC_APB2PeriphResetCmd_1  ;; 0x4002100c
        STR      R0,[R1, #+0]
??RCC_APB2PeriphResetCmd_2:
        BX       LR               ;; return
        DATA
??RCC_APB2PeriphResetCmd_1:
        DC32     0x4002100c

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
RCC_APB1PeriphResetCmd:
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        CMP      R1,#+0
        BEQ.N    ??RCC_APB1PeriphResetCmd_0
        LDR.N    R1,??RCC_APB1PeriphResetCmd_1  ;; 0x40021010
        LDR      R1,[R1, #+0]
        ORRS     R0,R0,R1
        LDR.N    R1,??RCC_APB1PeriphResetCmd_1  ;; 0x40021010
        STR      R0,[R1, #+0]
        B.N      ??RCC_APB1PeriphResetCmd_2
??RCC_APB1PeriphResetCmd_0:
        LDR.N    R1,??RCC_APB1PeriphResetCmd_1  ;; 0x40021010
        LDR      R1,[R1, #+0]
        MVNS     R0,R0
        ANDS     R0,R0,R1
        LDR.N    R1,??RCC_APB1PeriphResetCmd_1  ;; 0x40021010
        STR      R0,[R1, #+0]
??RCC_APB1PeriphResetCmd_2:
        BX       LR               ;; return
        DATA
??RCC_APB1PeriphResetCmd_1:
        DC32     0x40021010

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
RCC_BackupResetCmd:
        LDR.N    R1,??RCC_BackupResetCmd_0  ;; 0x42420440
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        STR      R0,[R1, #+0]
        BX       LR               ;; return
        DATA
??RCC_BackupResetCmd_0:
        DC32     0x42420440

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
RCC_ClockSecuritySystemCmd:
        LDR.N    R1,??RCC_ClockSecuritySystemCmd_0  ;; 0x4242004c
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        STR      R0,[R1, #+0]
        BX       LR               ;; return
        DATA
??RCC_ClockSecuritySystemCmd_0:
        DC32     0x4242004c

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
RCC_MCOConfig:
        LDR.N    R1,??RCC_MCOConfig_0  ;; 0x40021007
        STRB     R0,[R1, #+0]
        BX       LR               ;; return
        Nop      
        DATA
??RCC_MCOConfig_0:
        DC32     0x40021007

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
RCC_GetFlagStatus:
        MOVS     R1,R0
        MOVS     R2,#+0
        MOVS     R3,#+0
        MOVS     R0,#+0
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        LSRS     R12,R1,#+5
        UXTB     R12,R12          ;; ZeroExt  R12,R12,#+24,#+24
        MOV      R2,R12
        CMP      R2,#+1
        BNE.N    ??RCC_GetFlagStatus_0
        LDR.N    R0,??DataTable48  ;; 0x40021000
        LDR      R3,[R0, #+0]
        B.N      ??RCC_GetFlagStatus_1
??RCC_GetFlagStatus_0:
        CMP      R2,#+2
        BNE.N    ??RCC_GetFlagStatus_2
        LDR.N    R0,??DataTable49  ;; 0x40021020
        LDR      R3,[R0, #+0]
        B.N      ??RCC_GetFlagStatus_1
??RCC_GetFlagStatus_2:
        LDR.N    R0,??DataTable52  ;; 0x40021024
        LDR      R3,[R0, #+0]
??RCC_GetFlagStatus_1:
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        ANDS     R2,R1,#0x1F
        MOVS     R0,#+1
        LSLS     R0,R0,R2
        ANDS     R0,R0,R3
        SUBS     R0,R0,#+1
        SBCS     R0,R0,R0
        MVNS     R0,R0
        LSRS     R0,R0,#+31
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        BX       LR               ;; return

        SECTION `.text`:CODE:NOROOT(2)
        DATA
??DataTable48:
        DC32     0x40021000

        SECTION `.text`:CODE:NOROOT(2)
        DATA
??DataTable49:
        DC32     0x40021020

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
RCC_ClearFlag:
        LDR.N    R0,??DataTable52  ;; 0x40021024
        LDR      R0,[R0, #+0]
        ORRS     R0,R0,#0x1000000
        LDR.N    R1,??DataTable52  ;; 0x40021024
        STR      R0,[R1, #+0]
        BX       LR               ;; return

        SECTION `.text`:CODE:NOROOT(2)
        DATA
??DataTable52:
        DC32     0x40021024

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
RCC_GetITStatus:
        MOVS     R1,R0
        MOVS     R0,#+0
        LDR.N    R2,??DataTable53  ;; 0x40021008
        LDR      R2,[R2, #+0]
        ANDS     R1,R1,R2
        MOVS     R2,R1
        UXTB     R2,R2            ;; ZeroExt  R2,R2,#+24,#+24
        SUBS     R2,R2,#+1
        SBCS     R2,R2,R2
        MVNS     R2,R2
        LSRS     R2,R2,#+31
        MOVS     R0,R2
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        BX       LR               ;; return

        SECTION `.text`:CODE:NOROOT(2)
        DATA
??DataTable53:
        DC32     0x40021008

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
RCC_ClearITPendingBit:
        LDR.N    R1,??RCC_ClearITPendingBit_0  ;; 0x4002100a
        STRB     R0,[R1, #+0]
        BX       LR               ;; return
        Nop      
        DATA
??RCC_ClearITPendingBit_0:
        DC32     0x4002100a

        END
// 
//     5 bytes in section .bss
//    20 bytes in section .rodata
// 1 174 bytes in section .text
// 
// 1 174 bytes of CODE  memory
//    20 bytes of CONST memory
//     5 bytes of DATA  memory
//
//Errors: none
//Warnings: none
