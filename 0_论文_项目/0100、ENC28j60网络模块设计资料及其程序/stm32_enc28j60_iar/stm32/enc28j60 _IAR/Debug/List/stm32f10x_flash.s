///////////////////////////////////////////////////////////////////////////////
//                                                                            /
//                                                      21/Apr/2009  21:08:10 /
// IAR ANSI C/C++ Compiler V5.20.2.21007/W32 EVALUATION for ARM               /
// Copyright 1999-2008 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode     =  thumb                                                   /
//    Endian       =  little                                                  /
//    Source file  =  F:\b百利胜\enc28j60\enc28j60                            /
//                    _IAR\EWARM_V2.0.2\library\src\stm32f10x_flash.c         /
//    Command line =  "F:\b百利胜\enc28j60\enc28j60                           /
//                    _IAR\EWARM_V2.0.2\library\src\stm32f10x_flash.c" -lcN   /
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
//                    _IAR\Debug\List\stm32f10x_flash.s                       /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME stm32f10x_flash

        PUBLIC FLASH_ClearFlag
        PUBLIC FLASH_EnableWriteProtection
        PUBLIC FLASH_EraseAllPages
        PUBLIC FLASH_EraseOptionBytes
        PUBLIC FLASH_ErasePage
        PUBLIC FLASH_GetFlagStatus
        PUBLIC FLASH_GetPrefetchBufferStatus
        PUBLIC FLASH_GetReadOutProtectionStatus
        PUBLIC FLASH_GetStatus
        PUBLIC FLASH_GetUserOptionByte
        PUBLIC FLASH_GetWriteProtectionOptionByte
        PUBLIC FLASH_HalfCycleAccessCmd
        PUBLIC FLASH_ITConfig
        PUBLIC FLASH_Lock
        PUBLIC FLASH_PrefetchBufferCmd
        PUBLIC FLASH_ProgramHalfWord
        PUBLIC FLASH_ProgramOptionByteData
        PUBLIC FLASH_ProgramWord
        PUBLIC FLASH_ReadOutProtection
        PUBLIC FLASH_SetLatency
        PUBLIC FLASH_Unlock
        PUBLIC FLASH_UserOptionByteConfig
        PUBLIC FLASH_WaitForLastOperation


        SECTION `.text`:CODE:NOROOT(2)
        THUMB
FLASH_SetLatency:
        LDR.W    R1,??DataTable99  ;; 0x40022000
        LDR      R1,[R1, #+0]
        ANDS     R1,R1,#0x38
        LDR.W    R2,??DataTable99  ;; 0x40022000
        STR      R1,[R2, #+0]
        LDR.W    R1,??DataTable99  ;; 0x40022000
        LDR      R1,[R1, #+0]
        ORRS     R0,R0,R1
        LDR.W    R1,??DataTable99  ;; 0x40022000
        STR      R0,[R1, #+0]
        BX       LR               ;; return

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
FLASH_HalfCycleAccessCmd:
        LDR.W    R1,??DataTable99  ;; 0x40022000
        LDR      R1,[R1, #+0]
        MOVS     R2,#+8
        BICS     R1,R1,R2
        LDR.W    R2,??DataTable99  ;; 0x40022000
        STR      R1,[R2, #+0]
        LDR.W    R1,??DataTable99  ;; 0x40022000
        LDR      R1,[R1, #+0]
        ORRS     R0,R0,R1
        LDR.W    R1,??DataTable99  ;; 0x40022000
        STR      R0,[R1, #+0]
        BX       LR               ;; return

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
FLASH_PrefetchBufferCmd:
        LDR.W    R1,??DataTable99  ;; 0x40022000
        LDR      R1,[R1, #+0]
        MOVS     R2,#+16
        BICS     R1,R1,R2
        LDR.W    R2,??DataTable99  ;; 0x40022000
        STR      R1,[R2, #+0]
        LDR.W    R1,??DataTable99  ;; 0x40022000
        LDR      R1,[R1, #+0]
        ORRS     R0,R0,R1
        LDR.W    R1,??DataTable99  ;; 0x40022000
        STR      R0,[R1, #+0]
        BX       LR               ;; return

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
FLASH_Unlock:
        LDR.N    R0,??FLASH_Unlock_0  ;; 0x40022004
        LDR.W    R1,??DataTable90  ;; 0x45670123
        STR      R1,[R0, #+0]
        LDR.N    R0,??FLASH_Unlock_0  ;; 0x40022004
        LDR.W    R1,??DataTable92  ;; 0xcdef89ab
        STR      R1,[R0, #+0]
        BX       LR               ;; return
        Nop      
        DATA
??FLASH_Unlock_0:
        DC32     0x40022004

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
FLASH_Lock:
        LDR.W    R0,??DataTable103  ;; 0x40022010
        LDR      R0,[R0, #+0]
        ORRS     R0,R0,#0x80
        LDR.W    R1,??DataTable103  ;; 0x40022010
        STR      R0,[R1, #+0]
        BX       LR               ;; return

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
FLASH_ErasePage:
        PUSH     {R3-R5,LR}
        MOVS     R4,R0
        MOVS     R5,#+4
        MOVW     R0,#+4095
        BL       FLASH_WaitForLastOperation
        MOVS     R5,R0
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        CMP      R5,#+4
        BNE.N    ??FLASH_ErasePage_0
        LDR.W    R0,??DataTable103  ;; 0x40022010
        LDR      R0,[R0, #+0]
        ORRS     R0,R0,#0x2
        LDR.W    R1,??DataTable103  ;; 0x40022010
        STR      R0,[R1, #+0]
        LDR.N    R0,??FLASH_ErasePage_1  ;; 0x40022014
        STR      R4,[R0, #+0]
        LDR.W    R0,??DataTable103  ;; 0x40022010
        LDR      R0,[R0, #+0]
        ORRS     R0,R0,#0x40
        LDR.W    R1,??DataTable103  ;; 0x40022010
        STR      R0,[R1, #+0]
        MOVW     R0,#+4095
        BL       FLASH_WaitForLastOperation
        MOVS     R5,R0
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        CMP      R5,#+1
        BEQ.N    ??FLASH_ErasePage_0
        LDR.W    R0,??DataTable103  ;; 0x40022010
        LDR      R0,[R0, #+0]
        MOVW     R1,#+8189
        ANDS     R0,R1,R0
        LDR.W    R1,??DataTable103  ;; 0x40022010
        STR      R0,[R1, #+0]
??FLASH_ErasePage_0:
        MOVS     R0,R5
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        POP      {R1,R4,R5,PC}    ;; return
        Nop      
        DATA
??FLASH_ErasePage_1:
        DC32     0x40022014

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
FLASH_EraseAllPages:
        PUSH     {R4,LR}
        MOVS     R4,#+4
        MOVW     R0,#+4095
        BL       FLASH_WaitForLastOperation
        MOVS     R4,R0
        UXTB     R4,R4            ;; ZeroExt  R4,R4,#+24,#+24
        CMP      R4,#+4
        BNE.N    ??FLASH_EraseAllPages_0
        LDR.W    R0,??DataTable103  ;; 0x40022010
        LDR      R0,[R0, #+0]
        ORRS     R0,R0,#0x4
        LDR.W    R1,??DataTable103  ;; 0x40022010
        STR      R0,[R1, #+0]
        LDR.W    R0,??DataTable103  ;; 0x40022010
        LDR      R0,[R0, #+0]
        ORRS     R0,R0,#0x40
        LDR.W    R1,??DataTable103  ;; 0x40022010
        STR      R0,[R1, #+0]
        MOVW     R0,#+4095
        BL       FLASH_WaitForLastOperation
        MOVS     R4,R0
        UXTB     R4,R4            ;; ZeroExt  R4,R4,#+24,#+24
        CMP      R4,#+1
        BEQ.N    ??FLASH_EraseAllPages_0
        LDR.W    R0,??DataTable103  ;; 0x40022010
        LDR      R0,[R0, #+0]
        MOVW     R1,#+8187
        ANDS     R0,R1,R0
        LDR.W    R1,??DataTable103  ;; 0x40022010
        STR      R0,[R1, #+0]
??FLASH_EraseAllPages_0:
        MOVS     R0,R4
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        POP      {R4,PC}          ;; return

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
FLASH_EraseOptionBytes:
        PUSH     {R4,LR}
        MOVS     R4,#+4
        MOVW     R0,#+4095
        BL       FLASH_WaitForLastOperation
        MOVS     R4,R0
        UXTB     R4,R4            ;; ZeroExt  R4,R4,#+24,#+24
        CMP      R4,#+4
        BNE.N    ??FLASH_EraseOptionBytes_0
        LDR.W    R0,??DataTable91  ;; 0x40022008
        LDR.W    R1,??DataTable90  ;; 0x45670123
        STR      R1,[R0, #+0]
        LDR.W    R0,??DataTable91  ;; 0x40022008
        LDR.W    R1,??DataTable92  ;; 0xcdef89ab
        STR      R1,[R0, #+0]
        LDR.W    R0,??DataTable103  ;; 0x40022010
        LDR      R0,[R0, #+0]
        ORRS     R0,R0,#0x20
        LDR.W    R1,??DataTable103  ;; 0x40022010
        STR      R0,[R1, #+0]
        LDR.W    R0,??DataTable103  ;; 0x40022010
        LDR      R0,[R0, #+0]
        ORRS     R0,R0,#0x40
        LDR.W    R1,??DataTable103  ;; 0x40022010
        STR      R0,[R1, #+0]
        MOVW     R0,#+4095
        BL       FLASH_WaitForLastOperation
        MOVS     R4,R0
        UXTB     R4,R4            ;; ZeroExt  R4,R4,#+24,#+24
        CMP      R4,#+4
        BNE.N    ??FLASH_EraseOptionBytes_1
        LDR.W    R0,??DataTable103  ;; 0x40022010
        LDR      R0,[R0, #+0]
        MOVW     R1,#+8159
        ANDS     R0,R1,R0
        LDR.W    R1,??DataTable103  ;; 0x40022010
        STR      R0,[R1, #+0]
        LDR.W    R0,??DataTable103  ;; 0x40022010
        LDR      R0,[R0, #+0]
        ORRS     R0,R0,#0x10
        LDR.W    R1,??DataTable103  ;; 0x40022010
        STR      R0,[R1, #+0]
        LDR.N    R0,??DataTable84  ;; 0x1ffff800
        MOVS     R1,#+165
        STRH     R1,[R0, #+0]
        MOVS     R0,#+15
        BL       FLASH_WaitForLastOperation
        MOVS     R4,R0
        UXTB     R4,R4            ;; ZeroExt  R4,R4,#+24,#+24
        CMP      R4,#+1
        BEQ.N    ??FLASH_EraseOptionBytes_0
        LDR.W    R0,??DataTable103  ;; 0x40022010
        LDR      R0,[R0, #+0]
        MOVW     R1,#+8175
        ANDS     R0,R1,R0
        LDR.W    R1,??DataTable103  ;; 0x40022010
        STR      R0,[R1, #+0]
        B.N      ??FLASH_EraseOptionBytes_0
??FLASH_EraseOptionBytes_1:
        UXTB     R4,R4            ;; ZeroExt  R4,R4,#+24,#+24
        CMP      R4,#+1
        BEQ.N    ??FLASH_EraseOptionBytes_0
        LDR.W    R0,??DataTable103  ;; 0x40022010
        LDR      R0,[R0, #+0]
        MOVW     R1,#+8175
        ANDS     R0,R1,R0
        LDR.W    R1,??DataTable103  ;; 0x40022010
        STR      R0,[R1, #+0]
??FLASH_EraseOptionBytes_0:
        MOVS     R0,R4
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        POP      {R4,PC}          ;; return

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
FLASH_ProgramWord:
        PUSH     {R4-R6,LR}
        MOVS     R5,R0
        MOVS     R4,R1
        MOVS     R6,#+4
        MOVS     R0,#+15
        BL       FLASH_WaitForLastOperation
        MOVS     R6,R0
        UXTB     R6,R6            ;; ZeroExt  R6,R6,#+24,#+24
        CMP      R6,#+4
        BNE.N    ??FLASH_ProgramWord_0
        LDR.W    R0,??DataTable103  ;; 0x40022010
        LDR      R0,[R0, #+0]
        ORRS     R0,R0,#0x1
        LDR.W    R1,??DataTable103  ;; 0x40022010
        STR      R0,[R1, #+0]
        STRH     R4,[R5, #+0]
        MOVS     R0,#+15
        BL       FLASH_WaitForLastOperation
        MOVS     R6,R0
        UXTB     R6,R6            ;; ZeroExt  R6,R6,#+24,#+24
        CMP      R6,#+4
        BNE.N    ??FLASH_ProgramWord_1
        LSRS     R0,R4,#+16
        STRH     R0,[R5, #+2]
        MOVS     R0,#+15
        BL       FLASH_WaitForLastOperation
        MOVS     R6,R0
        UXTB     R6,R6            ;; ZeroExt  R6,R6,#+24,#+24
        CMP      R6,#+1
        BEQ.N    ??FLASH_ProgramWord_0
        LDR.W    R0,??DataTable103  ;; 0x40022010
        LDR      R0,[R0, #+0]
        MOVW     R1,#+8190
        ANDS     R0,R1,R0
        LDR.W    R1,??DataTable103  ;; 0x40022010
        STR      R0,[R1, #+0]
        B.N      ??FLASH_ProgramWord_0
??FLASH_ProgramWord_1:
        UXTB     R6,R6            ;; ZeroExt  R6,R6,#+24,#+24
        CMP      R6,#+1
        BEQ.N    ??FLASH_ProgramWord_0
        LDR.W    R0,??DataTable103  ;; 0x40022010
        LDR      R0,[R0, #+0]
        MOVW     R1,#+8190
        ANDS     R0,R1,R0
        LDR.N    R1,??DataTable103  ;; 0x40022010
        STR      R0,[R1, #+0]
??FLASH_ProgramWord_0:
        MOVS     R0,R6
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        POP      {R4-R6,PC}       ;; return

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
FLASH_ProgramHalfWord:
        PUSH     {R4-R6,LR}
        MOVS     R5,R0
        MOVS     R4,R1
        MOVS     R6,#+4
        MOVS     R0,#+15
        BL       FLASH_WaitForLastOperation
        MOVS     R6,R0
        UXTB     R6,R6            ;; ZeroExt  R6,R6,#+24,#+24
        CMP      R6,#+4
        BNE.N    ??FLASH_ProgramHalfWord_0
        LDR.N    R0,??DataTable103  ;; 0x40022010
        LDR      R0,[R0, #+0]
        ORRS     R0,R0,#0x1
        LDR.N    R1,??DataTable103  ;; 0x40022010
        STR      R0,[R1, #+0]
        STRH     R4,[R5, #+0]
        MOVS     R0,#+15
        BL       FLASH_WaitForLastOperation
        MOVS     R6,R0
        UXTB     R6,R6            ;; ZeroExt  R6,R6,#+24,#+24
        CMP      R6,#+1
        BEQ.N    ??FLASH_ProgramHalfWord_0
        LDR.N    R0,??DataTable103  ;; 0x40022010
        LDR      R0,[R0, #+0]
        MOVW     R1,#+8190
        ANDS     R0,R1,R0
        LDR.N    R1,??DataTable103  ;; 0x40022010
        STR      R0,[R1, #+0]
??FLASH_ProgramHalfWord_0:
        MOVS     R0,R6
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        POP      {R4-R6,PC}       ;; return

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
FLASH_ProgramOptionByteData:
        PUSH     {R4-R6,LR}
        MOVS     R5,R0
        MOVS     R4,R1
        MOVS     R6,#+4
        MOVS     R0,#+15
        BL       FLASH_WaitForLastOperation
        MOVS     R6,R0
        UXTB     R6,R6            ;; ZeroExt  R6,R6,#+24,#+24
        CMP      R6,#+4
        BNE.N    ??FLASH_ProgramOptionByteData_0
        LDR.N    R0,??DataTable91  ;; 0x40022008
        LDR.N    R1,??DataTable90  ;; 0x45670123
        STR      R1,[R0, #+0]
        LDR.N    R0,??DataTable91  ;; 0x40022008
        LDR.N    R1,??DataTable92  ;; 0xcdef89ab
        STR      R1,[R0, #+0]
        LDR.N    R0,??DataTable103  ;; 0x40022010
        LDR      R0,[R0, #+0]
        ORRS     R0,R0,#0x10
        LDR.N    R1,??DataTable103  ;; 0x40022010
        STR      R0,[R1, #+0]
        UXTB     R4,R4            ;; ZeroExt  R4,R4,#+24,#+24
        STRH     R4,[R5, #+0]
        MOVS     R0,#+15
        BL       FLASH_WaitForLastOperation
        MOVS     R6,R0
        UXTB     R6,R6            ;; ZeroExt  R6,R6,#+24,#+24
        CMP      R6,#+1
        BEQ.N    ??FLASH_ProgramOptionByteData_0
        LDR.N    R0,??DataTable103  ;; 0x40022010
        LDR      R0,[R0, #+0]
        MOVW     R1,#+8175
        ANDS     R0,R1,R0
        LDR.N    R1,??DataTable103  ;; 0x40022010
        STR      R0,[R1, #+0]
??FLASH_ProgramOptionByteData_0:
        MOVS     R0,R6
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        POP      {R4-R6,PC}       ;; return

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
FLASH_EnableWriteProtection:
        PUSH     {R4-R8,LR}
        MOVW     R4,#+65535
        MOVW     R5,#+65535
        MOVW     R6,#+65535
        MOVW     R7,#+65535
        MOVS     R8,#+4
        MVNS     R0,R0
        MOVS     R1,R0
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        MOVS     R4,R1
        ANDS     R1,R0,#0xFF00
        LSRS     R1,R1,#+8
        MOVS     R5,R1
        ANDS     R1,R0,#0xFF0000
        LSRS     R1,R1,#+16
        MOVS     R6,R1
        LSRS     R0,R0,#+24
        MOVS     R7,R0
        MOVS     R0,#+15
        BL       FLASH_WaitForLastOperation
        MOV      R8,R0
        UXTB     R8,R8            ;; ZeroExt  R8,R8,#+24,#+24
        CMP      R8,#+4
        BNE.N    ??FLASH_EnableWriteProtection_0
        LDR.N    R0,??DataTable91  ;; 0x40022008
        LDR.N    R1,??DataTable90  ;; 0x45670123
        STR      R1,[R0, #+0]
        LDR.N    R0,??DataTable91  ;; 0x40022008
        LDR.N    R1,??DataTable92  ;; 0xcdef89ab
        STR      R1,[R0, #+0]
        LDR.N    R0,??DataTable103  ;; 0x40022010
        LDR      R0,[R0, #+0]
        ORRS     R0,R0,#0x10
        LDR.N    R1,??DataTable103  ;; 0x40022010
        STR      R0,[R1, #+0]
        UXTH     R4,R4            ;; ZeroExt  R4,R4,#+16,#+16
        CMP      R4,#+255
        BEQ.N    ??FLASH_EnableWriteProtection_1
        LDR.N    R0,??FLASH_EnableWriteProtection_2  ;; 0x1ffff808
        STRH     R4,[R0, #+0]
        MOVS     R0,#+15
        BL       FLASH_WaitForLastOperation
        MOV      R8,R0
??FLASH_EnableWriteProtection_1:
        UXTB     R8,R8            ;; ZeroExt  R8,R8,#+24,#+24
        CMP      R8,#+4
        BNE.N    ??FLASH_EnableWriteProtection_3
        UXTH     R5,R5            ;; ZeroExt  R5,R5,#+16,#+16
        CMP      R5,#+255
        BEQ.N    ??FLASH_EnableWriteProtection_3
        LDR.N    R0,??FLASH_EnableWriteProtection_2+0x4  ;; 0x1ffff80a
        STRH     R5,[R0, #+0]
        MOVS     R0,#+15
        BL       FLASH_WaitForLastOperation
        MOV      R8,R0
??FLASH_EnableWriteProtection_3:
        UXTB     R8,R8            ;; ZeroExt  R8,R8,#+24,#+24
        CMP      R8,#+4
        BNE.N    ??FLASH_EnableWriteProtection_4
        UXTH     R6,R6            ;; ZeroExt  R6,R6,#+16,#+16
        CMP      R6,#+255
        BEQ.N    ??FLASH_EnableWriteProtection_4
        LDR.N    R0,??FLASH_EnableWriteProtection_2+0x8  ;; 0x1ffff80c
        STRH     R6,[R0, #+0]
        MOVS     R0,#+15
        BL       FLASH_WaitForLastOperation
        MOV      R8,R0
??FLASH_EnableWriteProtection_4:
        UXTB     R8,R8            ;; ZeroExt  R8,R8,#+24,#+24
        CMP      R8,#+4
        BNE.N    ??FLASH_EnableWriteProtection_5
        UXTH     R7,R7            ;; ZeroExt  R7,R7,#+16,#+16
        CMP      R7,#+255
        BEQ.N    ??FLASH_EnableWriteProtection_5
        LDR.N    R0,??FLASH_EnableWriteProtection_2+0xC  ;; 0x1ffff80e
        STRH     R7,[R0, #+0]
        MOVS     R0,#+15
        BL       FLASH_WaitForLastOperation
        MOV      R8,R0
??FLASH_EnableWriteProtection_5:
        UXTB     R8,R8            ;; ZeroExt  R8,R8,#+24,#+24
        CMP      R8,#+1
        BEQ.N    ??FLASH_EnableWriteProtection_0
        LDR.N    R0,??DataTable103  ;; 0x40022010
        LDR      R0,[R0, #+0]
        MOVW     R1,#+8175
        ANDS     R0,R1,R0
        LDR.N    R1,??DataTable103  ;; 0x40022010
        STR      R0,[R1, #+0]
??FLASH_EnableWriteProtection_0:
        MOV      R0,R8
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        POP      {R4-R8,PC}       ;; return
        DATA
??FLASH_EnableWriteProtection_2:
        DC32     0x1ffff808
        DC32     0x1ffff80a
        DC32     0x1ffff80c
        DC32     0x1ffff80e

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
FLASH_ReadOutProtection:
        PUSH     {R3-R5,LR}
        MOVS     R4,R0
        MOVS     R5,#+4
        MOVW     R0,#+4095
        BL       FLASH_WaitForLastOperation
        MOVS     R5,R0
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        CMP      R5,#+4
        BNE.N    ??FLASH_ReadOutProtection_0
        LDR.N    R0,??DataTable91  ;; 0x40022008
        LDR.N    R1,??DataTable90  ;; 0x45670123
        STR      R1,[R0, #+0]
        LDR.N    R0,??DataTable91  ;; 0x40022008
        LDR.N    R1,??DataTable92  ;; 0xcdef89ab
        STR      R1,[R0, #+0]
        LDR.N    R0,??DataTable103  ;; 0x40022010
        LDR      R0,[R0, #+0]
        ORRS     R0,R0,#0x20
        LDR.N    R1,??DataTable103  ;; 0x40022010
        STR      R0,[R1, #+0]
        LDR.N    R0,??DataTable103  ;; 0x40022010
        LDR      R0,[R0, #+0]
        ORRS     R0,R0,#0x40
        LDR.N    R1,??DataTable103  ;; 0x40022010
        STR      R0,[R1, #+0]
        MOVW     R0,#+4095
        BL       FLASH_WaitForLastOperation
        MOVS     R5,R0
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        CMP      R5,#+4
        BNE.N    ??FLASH_ReadOutProtection_1
        LDR.N    R0,??DataTable103  ;; 0x40022010
        LDR      R0,[R0, #+0]
        MOVW     R1,#+8159
        ANDS     R0,R1,R0
        LDR.N    R1,??DataTable103  ;; 0x40022010
        STR      R0,[R1, #+0]
        LDR.N    R0,??DataTable103  ;; 0x40022010
        LDR      R0,[R0, #+0]
        ORRS     R0,R0,#0x10
        LDR.N    R1,??DataTable103  ;; 0x40022010
        STR      R0,[R1, #+0]
        UXTB     R4,R4            ;; ZeroExt  R4,R4,#+24,#+24
        CMP      R4,#+0
        BEQ.N    ??FLASH_ReadOutProtection_2
        LDR.N    R0,??DataTable84  ;; 0x1ffff800
        MOVS     R1,#+0
        STRH     R1,[R0, #+0]
        B.N      ??FLASH_ReadOutProtection_3
??FLASH_ReadOutProtection_2:
        LDR.N    R0,??DataTable84  ;; 0x1ffff800
        MOVS     R1,#+165
        STRH     R1,[R0, #+0]
??FLASH_ReadOutProtection_3:
        MOVW     R0,#+4095
        BL       FLASH_WaitForLastOperation
        MOVS     R5,R0
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        CMP      R5,#+1
        BEQ.N    ??FLASH_ReadOutProtection_0
        LDR.N    R0,??DataTable103  ;; 0x40022010
        LDR      R0,[R0, #+0]
        MOVW     R1,#+8175
        ANDS     R0,R1,R0
        LDR.N    R1,??DataTable103  ;; 0x40022010
        STR      R0,[R1, #+0]
        B.N      ??FLASH_ReadOutProtection_0
??FLASH_ReadOutProtection_1:
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        CMP      R5,#+1
        BEQ.N    ??FLASH_ReadOutProtection_0
        LDR.N    R0,??DataTable103  ;; 0x40022010
        LDR      R0,[R0, #+0]
        MOVW     R1,#+8159
        ANDS     R0,R1,R0
        LDR.N    R1,??DataTable103  ;; 0x40022010
        STR      R0,[R1, #+0]
??FLASH_ReadOutProtection_0:
        MOVS     R0,R5
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        POP      {R1,R4,R5,PC}    ;; return

        SECTION `.text`:CODE:NOROOT(2)
        DATA
??DataTable84:
        DC32     0x1ffff800

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
FLASH_UserOptionByteConfig:
        PUSH     {R3-R7,LR}
        MOVS     R6,R0
        MOVS     R4,R1
        MOVS     R5,R2
        MOVS     R7,#+4
        LDR.N    R0,??DataTable91  ;; 0x40022008
        LDR.N    R1,??DataTable90  ;; 0x45670123
        STR      R1,[R0, #+0]
        LDR.N    R0,??DataTable91  ;; 0x40022008
        LDR.N    R1,??DataTable92  ;; 0xcdef89ab
        STR      R1,[R0, #+0]
        MOVS     R0,#+15
        BL       FLASH_WaitForLastOperation
        MOVS     R7,R0
        UXTB     R7,R7            ;; ZeroExt  R7,R7,#+24,#+24
        CMP      R7,#+4
        BNE.N    ??FLASH_UserOptionByteConfig_0
        LDR.N    R0,??DataTable103  ;; 0x40022010
        LDR      R0,[R0, #+0]
        ORRS     R0,R0,#0x10
        LDR.N    R1,??DataTable103  ;; 0x40022010
        STR      R0,[R1, #+0]
        ORRS     R0,R4,R6
        ORRS     R0,R5,R0
        ORRS     R0,R0,#0xF8
        LDR.N    R1,??FLASH_UserOptionByteConfig_1  ;; 0x1ffff802
        STRH     R0,[R1, #+0]
        MOVS     R0,#+15
        BL       FLASH_WaitForLastOperation
        MOVS     R7,R0
        UXTB     R7,R7            ;; ZeroExt  R7,R7,#+24,#+24
        CMP      R7,#+1
        BEQ.N    ??FLASH_UserOptionByteConfig_0
        LDR.N    R0,??DataTable103  ;; 0x40022010
        LDR      R0,[R0, #+0]
        MOVW     R1,#+8175
        ANDS     R0,R1,R0
        LDR.N    R1,??DataTable103  ;; 0x40022010
        STR      R0,[R1, #+0]
??FLASH_UserOptionByteConfig_0:
        MOVS     R0,R7
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        POP      {R1,R4-R7,PC}    ;; return
        DATA
??FLASH_UserOptionByteConfig_1:
        DC32     0x1ffff802

        SECTION `.text`:CODE:NOROOT(2)
        DATA
??DataTable90:
        DC32     0x45670123

        SECTION `.text`:CODE:NOROOT(2)
        DATA
??DataTable91:
        DC32     0x40022008

        SECTION `.text`:CODE:NOROOT(2)
        DATA
??DataTable92:
        DC32     0xcdef89ab

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
FLASH_GetUserOptionByte:
        LDR.N    R0,??DataTable104  ;; 0x4002201c
        LDR      R0,[R0, #+0]
        LSRS     R0,R0,#+2
        BX       LR               ;; return

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
FLASH_GetWriteProtectionOptionByte:
        LDR.N    R0,??FLASH_GetWriteProtectionOptionByte_0  ;; 0x40022020
        LDR      R0,[R0, #+0]
        BX       LR               ;; return
        Nop      
        DATA
??FLASH_GetWriteProtectionOptionByte_0:
        DC32     0x40022020

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
FLASH_GetReadOutProtectionStatus:
        MOVS     R0,#+0
        LDR.N    R1,??DataTable104  ;; 0x4002201c
        LDR      R1,[R1, #+0]
        ANDS     R1,R1,#0x2
        SUBS     R2,R1,#+1
        SBCS     R2,R2,R2
        MVNS     R2,R2
        LSRS     R2,R2,#+31
        MOVS     R0,R2
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        BX       LR               ;; return

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
FLASH_GetPrefetchBufferStatus:
        MOVS     R0,#+0
        LDR.N    R1,??DataTable99  ;; 0x40022000
        LDR      R1,[R1, #+0]
        ANDS     R1,R1,#0x20
        SUBS     R2,R1,#+1
        SBCS     R2,R2,R2
        MVNS     R2,R2
        LSRS     R2,R2,#+31
        MOVS     R0,R2
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        BX       LR               ;; return

        SECTION `.text`:CODE:NOROOT(2)
        DATA
??DataTable99:
        DC32     0x40022000

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
FLASH_ITConfig:
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        CMP      R1,#+0
        BEQ.N    ??FLASH_ITConfig_0
        LDR.N    R1,??DataTable103  ;; 0x40022010
        LDR      R1,[R1, #+0]
        UXTH     R0,R0            ;; ZeroExt  R0,R0,#+16,#+16
        ORRS     R0,R0,R1
        LDR.N    R1,??DataTable103  ;; 0x40022010
        STR      R0,[R1, #+0]
        B.N      ??FLASH_ITConfig_1
??FLASH_ITConfig_0:
        LDR.N    R1,??DataTable103  ;; 0x40022010
        LDR      R1,[R1, #+0]
        UXTH     R0,R0            ;; ZeroExt  R0,R0,#+16,#+16
        MVNS     R0,R0
        ANDS     R0,R0,R1
        LDR.N    R1,??DataTable103  ;; 0x40022010
        STR      R0,[R1, #+0]
??FLASH_ITConfig_1:
        BX       LR               ;; return

        SECTION `.text`:CODE:NOROOT(2)
        DATA
??DataTable103:
        DC32     0x40022010

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
FLASH_GetFlagStatus:
        MOVS     R1,R0
        MOVS     R0,#+0
        UXTH     R1,R1            ;; ZeroExt  R1,R1,#+16,#+16
        CMP      R1,#+1
        BNE.N    ??FLASH_GetFlagStatus_0
        LDR.N    R0,??DataTable104  ;; 0x4002201c
        LDR      R0,[R0, #+0]
        ANDS     R1,R0,#0x1
        SUBS     R0,R1,#+1
        SBCS     R0,R0,R0
        MVNS     R0,R0
        LSRS     R0,R0,#+31
        B.N      ??FLASH_GetFlagStatus_1
??FLASH_GetFlagStatus_0:
        LDR.N    R0,??DataTable109  ;; 0x4002200c
        LDR      R0,[R0, #+0]
        ANDS     R1,R1,R0
        MOVS     R0,R1
        UXTH     R0,R0            ;; ZeroExt  R0,R0,#+16,#+16
        SUBS     R0,R0,#+1
        SBCS     R0,R0,R0
        MVNS     R0,R0
        LSRS     R0,R0,#+31
??FLASH_GetFlagStatus_1:
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        BX       LR               ;; return

        SECTION `.text`:CODE:NOROOT(2)
        DATA
??DataTable104:
        DC32     0x4002201c

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
FLASH_ClearFlag:
        LDR.N    R1,??DataTable109  ;; 0x4002200c
        UXTH     R0,R0            ;; ZeroExt  R0,R0,#+16,#+16
        STR      R0,[R1, #+0]
        BX       LR               ;; return

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
FLASH_GetStatus:
        MOVS     R0,#+4
        LDR.N    R1,??DataTable109  ;; 0x4002200c
        LDR      R1,[R1, #+0]
        TST      R1,#0x1
        BEQ.N    ??FLASH_GetStatus_0
        MOVS     R0,#+1
        B.N      ??FLASH_GetStatus_1
??FLASH_GetStatus_0:
        LDR.N    R0,??DataTable109  ;; 0x4002200c
        LDR      R0,[R0, #+0]
        TST      R0,#0x4
        BEQ.N    ??FLASH_GetStatus_2
        MOVS     R0,#+2
        B.N      ??FLASH_GetStatus_1
??FLASH_GetStatus_2:
        LDR.N    R0,??DataTable109  ;; 0x4002200c
        LDR      R0,[R0, #+0]
        TST      R0,#0x10
        BEQ.N    ??FLASH_GetStatus_3
        MOVS     R0,#+3
        B.N      ??FLASH_GetStatus_1
??FLASH_GetStatus_3:
        MOVS     R0,#+4
??FLASH_GetStatus_1:
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        BX       LR               ;; return

        SECTION `.text`:CODE:NOROOT(2)
        DATA
??DataTable109:
        DC32     0x4002200c

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
FLASH_WaitForLastOperation:
        PUSH     {R3-R5,LR}
        MOVS     R4,R0
        MOVS     R5,#+4
        BL       FLASH_GetStatus
        MOVS     R5,R0
        B.N      ??FLASH_WaitForLastOperation_0
??FLASH_WaitForLastOperation_1:
        BL       delay
        BL       FLASH_GetStatus
        MOVS     R5,R0
        SUBS     R4,R4,#+1
??FLASH_WaitForLastOperation_0:
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        CMP      R5,#+1
        BNE.N    ??FLASH_WaitForLastOperation_2
        CMP      R4,#+0
        BNE.N    ??FLASH_WaitForLastOperation_1
??FLASH_WaitForLastOperation_2:
        CMP      R4,#+0
        BNE.N    ??FLASH_WaitForLastOperation_3
        MOVS     R5,#+5
??FLASH_WaitForLastOperation_3:
        MOVS     R0,R5
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        POP      {R1,R4,R5,PC}    ;; return

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
delay:
        SUB      SP,SP,#+8
        MOVS     R0,#+0
        STR      R0,[SP, #+0]
        MOVS     R0,#+255
        STR      R0,[SP, #+0]
        B.N      ??delay_0
??delay_1:
        LDR      R0,[SP, #+0]
        SUBS     R0,R0,#+1
        STR      R0,[SP, #+0]
??delay_0:
        LDR      R0,[SP, #+0]
        CMP      R0,#+0
        BNE.N    ??delay_1
        ADD      SP,SP,#+8
        BX       LR               ;; return

        END
// 
// 1 642 bytes in section .text
// 
// 1 642 bytes of CODE memory
//
//Errors: none
//Warnings: none
